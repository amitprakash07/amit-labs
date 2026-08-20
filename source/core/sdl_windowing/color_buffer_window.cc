#include "core/sdl_windowing/color_buffer_window.h"

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <SDL3/SDL.h>

namespace amit::sdl_windowing
{
    namespace
    {
        struct HeldKeys
        {
            bool key_w         = false;
            bool key_s         = false;
            bool key_a         = false;
            bool key_d         = false;
            bool key_up        = false;
            bool key_down      = false;
            bool key_left      = false;
            bool key_right     = false;
            bool key_e         = false;
            bool key_q         = false;
            bool key_escape    = false;

            bool MoveForward() const
            {
                return key_w || key_up;
            }
            bool MoveBackward() const
            {
                return key_s || key_down;
            }
            bool MoveLeft() const
            {
                return key_a || key_left;
            }
            bool MoveRight() const
            {
                return key_d || key_right;
            }
            bool MoveUp() const
            {
                return key_e;
            }
            bool MoveDown() const
            {
                return key_q;
            }
            bool Quit() const
            {
                return key_escape;
            }
        };

        void ApplyKeyEvent(HeldKeys& held, const SDL_KeyboardEvent& key, bool is_down)
        {
            switch (key.scancode)
            {
            case SDL_SCANCODE_W:
                held.key_w = is_down;
                break;
            case SDL_SCANCODE_S:
                held.key_s = is_down;
                break;
            case SDL_SCANCODE_A:
                held.key_a = is_down;
                break;
            case SDL_SCANCODE_D:
                held.key_d = is_down;
                break;
            case SDL_SCANCODE_UP:
                held.key_up = is_down;
                break;
            case SDL_SCANCODE_DOWN:
                held.key_down = is_down;
                break;
            case SDL_SCANCODE_LEFT:
                held.key_left = is_down;
                break;
            case SDL_SCANCODE_RIGHT:
                held.key_right = is_down;
                break;
            case SDL_SCANCODE_E:
                held.key_e = is_down;
                break;
            case SDL_SCANCODE_Q:
                held.key_q = is_down;
                break;
            case SDL_SCANCODE_ESCAPE:
                held.key_escape = is_down;
                break;
            default:
                break;
            }
        }

        bool CreateSdlPresentSurface(int            width,
                                     int            height,
                                     const char*    window_title,
                                     SDL_Window**   out_window,
                                     SDL_Renderer** out_renderer,
                                     SDL_Texture**  out_texture)
        {
            if (!SDL_Init(SDL_INIT_VIDEO))
            {
                std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
                return false;
            }

            if (!SDL_CreateWindowAndRenderer(window_title != nullptr ? window_title : "amit-labs",
                                             width,
                                             height,
                                             0,
                                             out_window,
                                             out_renderer))
            {
                std::fprintf(stderr, "SDL_CreateWindowAndRenderer failed: %s\n", SDL_GetError());
                SDL_Quit();
                return false;
            }

            // STREAMING + LockTexture is the path meant for per-frame CPU uploads.
            // UpdateTexture on a STREAMING texture is unreliable on some backends.
            *out_texture =
                SDL_CreateTexture(*out_renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, width, height);
            if (*out_texture == nullptr)
            {
                std::fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());
                SDL_DestroyRenderer(*out_renderer);
                SDL_DestroyWindow(*out_window);
                SDL_Quit();
                return false;
            }

            SDL_SetTextureScaleMode(*out_texture, SDL_SCALEMODE_NEAREST);
            SDL_RaiseWindow(*out_window);
            return true;
        }

        void DestroySdlPresentSurface(SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* texture)
        {
            if (texture != nullptr)
            {
                SDL_DestroyTexture(texture);
            }
            if (renderer != nullptr)
            {
                SDL_DestroyRenderer(renderer);
            }
            if (window != nullptr)
            {
                SDL_DestroyWindow(window);
            }
            SDL_Quit();
        }

        bool PresentColorBuffer(SDL_Renderer* renderer, SDL_Texture* texture, const graphics::ColorBuffer& color_buffer)
        {
            const int width  = static_cast<int>(color_buffer.GetWidth().value);
            const int height = static_cast<int>(color_buffer.GetHeight().value);
            const int src_pitch = width * static_cast<int>(sizeof(graphics::Rgb8));

            void* dst_pixels = nullptr;
            int   dst_pitch  = 0;
            if (!SDL_LockTexture(texture, nullptr, &dst_pixels, &dst_pitch))
            {
                std::fprintf(stderr, "SDL_LockTexture failed: %s\n", SDL_GetError());
                return false;
            }

            const auto* src = reinterpret_cast<const std::uint8_t*>(color_buffer.GetData());
            auto*       dst = static_cast<std::uint8_t*>(dst_pixels);
            if (dst_pitch == src_pitch)
            {
                std::memcpy(dst, src, static_cast<size_t>(src_pitch) * static_cast<size_t>(height));
            }
            else
            {
                for (int y = 0; y < height; ++y)
                {
                    std::memcpy(dst + static_cast<size_t>(y) * static_cast<size_t>(dst_pitch),
                                src + static_cast<size_t>(y) * static_cast<size_t>(src_pitch),
                                static_cast<size_t>(src_pitch));
                }
            }

            SDL_UnlockTexture(texture);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderTexture(renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
            return true;
        }

        InteractiveInput MakeInteractiveInput(float dt_seconds, bool window_quit, const HeldKeys& held)
        {
            return InteractiveInput{
                .dt_seconds    = dt_seconds,
                .move_forward  = held.MoveForward(),
                .move_backward = held.MoveBackward(),
                .move_left     = held.MoveLeft(),
                .move_right    = held.MoveRight(),
                .move_up       = held.MoveUp(),
                .move_down     = held.MoveDown(),
                .quit          = window_quit || held.Quit(),
            };
        }
    }  // namespace

    bool ShowColorBufferWindow(const graphics::ColorBuffer& color_buffer, const char* window_title)
    {
        static_assert(sizeof(graphics::Rgb8) == 3, "Rgb8 must be tightly packed RGB24 for SDL texture upload");

        const int width  = static_cast<int>(color_buffer.GetWidth().value);
        const int height = static_cast<int>(color_buffer.GetHeight().value);
        if (width <= 0 || height <= 0 || color_buffer.GetData() == nullptr)
        {
            std::fprintf(stderr, "ShowColorBufferWindow: empty color buffer\n");
            return false;
        }

        SDL_Window*   window   = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Texture*  texture  = nullptr;
        if (!CreateSdlPresentSurface(width, height, window_title, &window, &renderer, &texture))
        {
            return false;
        }

        if (!PresentColorBuffer(renderer, texture, color_buffer))
        {
            DestroySdlPresentSurface(window, renderer, texture);
            return false;
        }

        std::printf("SDL window open — close the window to continue.\n");

        bool running = true;
        while (running)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT)
                {
                    running = false;
                }
            }

            if (!PresentColorBuffer(renderer, texture, color_buffer))
            {
                DestroySdlPresentSurface(window, renderer, texture);
                return false;
            }
        }

        DestroySdlPresentSurface(window, renderer, texture);
        return true;
    }

    bool RunInteractiveColorBufferLoop(graphics::ColorBuffer&                              color_buffer,
                                       const char*                                         window_title,
                                       const std::function<bool(const InteractiveInput&)>& update_and_render)
    {
        static_assert(sizeof(graphics::Rgb8) == 3, "Rgb8 must be tightly packed RGB24 for SDL texture upload");

        const int width  = static_cast<int>(color_buffer.GetWidth().value);
        const int height = static_cast<int>(color_buffer.GetHeight().value);
        if (width <= 0 || height <= 0 || color_buffer.GetData() == nullptr)
        {
            std::fprintf(stderr, "RunInteractiveColorBufferLoop: empty color buffer\n");
            return false;
        }

        SDL_Window*   window   = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Texture*  texture  = nullptr;
        if (!CreateSdlPresentSurface(width, height, window_title, &window, &renderer, &texture))
        {
            return false;
        }

        std::printf("Interactive SDL loop — click the window, then WASD/arrows move, Q/E down/up, Esc to quit.\n");

        HeldKeys held{};
        bool     running       = true;
        bool     has_focus     = true;
        Uint64   last_ticks_ns = SDL_GetTicksNS();

        while (running)
        {
            bool      window_quit = false;
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_EVENT_QUIT:
                    window_quit = true;
                    break;
                case SDL_EVENT_WINDOW_FOCUS_GAINED:
                    has_focus = true;
                    break;
                case SDL_EVENT_WINDOW_FOCUS_LOST:
                    has_focus = false;
                    held      = {};
                    break;
                case SDL_EVENT_KEY_DOWN:
                    ApplyKeyEvent(held, event.key, true);
                    break;
                case SDL_EVENT_KEY_UP:
                    ApplyKeyEvent(held, event.key, false);
                    break;
                default:
                    break;
                }
            }

            const Uint64 now_ticks_ns = SDL_GetTicksNS();
            const float  dt_seconds   = static_cast<float>(now_ticks_ns - last_ticks_ns) / 1'000'000'000.0f;
            last_ticks_ns             = now_ticks_ns;

            const InteractiveInput input = MakeInteractiveInput(dt_seconds, window_quit, held);
            if (!has_focus && (input.move_forward || input.move_backward || input.move_left || input.move_right ||
                               input.move_up || input.move_down))
            {
                // Keys should already be cleared on focus loss; keep this as a guard.
            }

            if (input.quit || !update_and_render(input))
            {
                running = false;
                break;
            }

            if (!PresentColorBuffer(renderer, texture, color_buffer))
            {
                DestroySdlPresentSurface(window, renderer, texture);
                return false;
            }
        }

        DestroySdlPresentSurface(window, renderer, texture);
        return true;
    }

}  // namespace amit::sdl_windowing
