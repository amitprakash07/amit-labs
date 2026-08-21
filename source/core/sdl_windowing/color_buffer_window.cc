#include "core/sdl_windowing/color_buffer_window.h"

#include "core/sdl_windowing/output_path.h"
#include "core/sdl_windowing/settings_panel.h"

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>

namespace amit::sdl_windowing
{
    namespace
    {
        struct HeldKeys
        {
            bool key_w      = false;
            bool key_s      = false;
            bool key_a      = false;
            bool key_d      = false;
            bool key_up     = false;
            bool key_down   = false;
            bool key_left   = false;
            bool key_right  = false;
            bool key_e      = false;
            bool key_q      = false;
            bool key_escape = false;

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

        bool CreateSdlPresentSurface(int            window_width,
                                     int            window_height,
                                     int            texture_width,
                                     int            texture_height,
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
                                             window_width,
                                             window_height,
                                             0,
                                             out_window,
                                             out_renderer))
            {
                std::fprintf(stderr, "SDL_CreateWindowAndRenderer failed: %s\n", SDL_GetError());
                SDL_Quit();
                return false;
            }

            *out_texture = SDL_CreateTexture(
                *out_renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, texture_width, texture_height);
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

        bool BlitColorBufferToTexture(SDL_Texture* texture, const graphics::ColorBuffer& color_buffer)
        {
            const int width     = static_cast<int>(color_buffer.GetWidth().value);
            const int height    = static_cast<int>(color_buffer.GetHeight().value);
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
            return true;
        }

        bool PresentColorBuffer(SDL_Renderer* renderer, SDL_Texture* texture, const graphics::ColorBuffer& color_buffer)
        {
            if (!BlitColorBufferToTexture(texture, color_buffer))
            {
                return false;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderTexture(renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
            return true;
        }

        InteractiveInput MakeInteractiveInput(float          dt_seconds,
                                              bool           window_quit,
                                              const HeldKeys& held,
                                              bool           allow_movement_keys)
        {
            InteractiveInput input{.dt_seconds = dt_seconds, .quit = window_quit || held.Quit()};
            if (allow_movement_keys)
            {
                input.move_forward  = held.MoveForward();
                input.move_backward = held.MoveBackward();
                input.move_left     = held.MoveLeft();
                input.move_right    = held.MoveRight();
                input.move_up       = held.MoveUp();
                input.move_down     = held.MoveDown();
            }
            return input;
        }

        bool InitImGuiForSdlRenderer(SDL_Window* window, SDL_Renderer* renderer)
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui::StyleColorsDark();

            if (!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer))
            {
                std::fprintf(stderr, "ImGui_ImplSDL3_InitForSDLRenderer failed\n");
                ImGui::DestroyContext();
                return false;
            }
            if (!ImGui_ImplSDLRenderer3_Init(renderer))
            {
                std::fprintf(stderr, "ImGui_ImplSDLRenderer3_Init failed\n");
                ImGui_ImplSDL3_Shutdown();
                ImGui::DestroyContext();
                return false;
            }
            return true;
        }

        void ShutdownImGui()
        {
            ImGui_ImplSDLRenderer3_Shutdown();
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext();
        }

        bool SaveWindowScreenshot(SDL_Renderer* renderer, const char* filename)
        {
            SDL_Surface* surface = SDL_RenderReadPixels(renderer, nullptr);
            if (surface == nullptr)
            {
                std::fprintf(stderr, "SDL_RenderReadPixels failed: %s\n", SDL_GetError());
                return false;
            }

            const bool ok = SDL_SaveBMP(surface, filename);
            if (!ok)
            {
                std::fprintf(stderr, "SDL_SaveBMP failed: %s\n", SDL_GetError());
            }
            SDL_DestroySurface(surface);
            return ok;
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
        if (!CreateSdlPresentSurface(width, height, width, height, window_title, &window, &renderer, &texture))
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
                                       const std::function<bool(const InteractiveInput&)>& update_and_render,
                                       InteractiveUiState*                                 ui_state)
    {
        static_assert(sizeof(graphics::Rgb8) == 3, "Rgb8 must be tightly packed RGB24 for SDL texture upload");

        const int framebuffer_width  = static_cast<int>(color_buffer.GetWidth().value);
        const int framebuffer_height = static_cast<int>(color_buffer.GetHeight().value);
        if (framebuffer_width <= 0 || framebuffer_height <= 0 || color_buffer.GetData() == nullptr)
        {
            std::fprintf(stderr, "RunInteractiveColorBufferLoop: empty color buffer\n");
            return false;
        }

        const int sidebar_width =
            (ui_state != nullptr && ui_state->sidebar_width_px > 0) ? ui_state->sidebar_width_px : 320;
        const int window_width = framebuffer_width + sidebar_width;

        SDL_Window*   window   = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Texture*  texture  = nullptr;
        if (!CreateSdlPresentSurface(window_width,
                                     framebuffer_height,
                                     framebuffer_width,
                                     framebuffer_height,
                                     window_title,
                                     &window,
                                     &renderer,
                                     &texture))
        {
            return false;
        }

        if (!InitImGuiForSdlRenderer(window, renderer))
        {
            DestroySdlPresentSurface(window, renderer, texture);
            return false;
        }

        std::printf("Interactive SDL loop — click the window, then WASD/arrows move, Q/E down/up, Esc to quit.\n");

        HeldKeys held{};
        bool     running                  = true;
        bool     request_save_screenshot  = false;
        Uint64   last_ticks_ns            = SDL_GetTicksNS();

        while (running)
        {
            bool      window_quit = false;
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                ImGui_ImplSDL3_ProcessEvent(&event);

                switch (event.type)
                {
                case SDL_EVENT_QUIT:
                    window_quit = true;
                    break;
                case SDL_EVENT_WINDOW_FOCUS_LOST:
                    held = {};
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

            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            const bool allow_movement_keys = !ImGui::GetIO().WantCaptureKeyboard;
            if (!allow_movement_keys)
            {
                held.key_w = held.key_s = held.key_a = held.key_d = false;
                held.key_up = held.key_down = held.key_left = held.key_right = false;
                held.key_e = held.key_q = false;
            }

            const InteractiveInput input = MakeInteractiveInput(dt_seconds, window_quit, held, allow_movement_keys);
            if (input.quit || !update_and_render(input))
            {
                running = false;
                ImGui::EndFrame();
                break;
            }

            if (!BlitColorBufferToTexture(texture, color_buffer))
            {
                ShutdownImGui();
                DestroySdlPresentSurface(window, renderer, texture);
                return false;
            }

            const app_settings::SettingsRegistry* settings =
                ui_state != nullptr ? ui_state->settings_registry : nullptr;
            const std::string* output_text = ui_state != nullptr ? ui_state->output_text : nullptr;
            bool* request_save_ppm = ui_state != nullptr ? ui_state->request_save_ppm : nullptr;
            DrawSidePanels(framebuffer_width,
                           framebuffer_height,
                           sidebar_width,
                           settings,
                           output_text,
                           request_save_ppm,
                           &request_save_screenshot);

            ImGui::Render();

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            const SDL_FRect framebuffer_dst{0.0f,
                                            0.0f,
                                            static_cast<float>(framebuffer_width),
                                            static_cast<float>(framebuffer_height)};
            SDL_RenderTexture(renderer, texture, nullptr, &framebuffer_dst);

            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

            if (request_save_screenshot)
            {
                constexpr const char* kScreenshotFilename = "window_screenshot.bmp";
                const std::string     screenshot_path     = MakeOutputFilePath(kScreenshotFilename);
                if (SaveWindowScreenshot(renderer, screenshot_path.c_str()))
                {
                    std::printf("Saved %s (full window including ImGui)\n", screenshot_path.c_str());
                    if (ui_state != nullptr && ui_state->action_status != nullptr)
                    {
                        *ui_state->action_status =
                            std::string("Saved ") + screenshot_path + " (full window including ImGui)";
                    }
                }
                else if (ui_state != nullptr && ui_state->action_status != nullptr)
                {
                    *ui_state->action_status = "Screenshot save failed";
                }
                request_save_screenshot = false;
            }

            SDL_RenderPresent(renderer);
        }

        ShutdownImGui();
        DestroySdlPresentSurface(window, renderer, texture);
        return true;
    }

}  // namespace amit::sdl_windowing
