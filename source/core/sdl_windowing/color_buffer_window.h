#ifndef CORE_SDL_WINDOWING_COLOR_BUFFER_WINDOW_H_
#define CORE_SDL_WINDOWING_COLOR_BUFFER_WINDOW_H_

#include <functional>

#include "core/graphics/image_2d.h"

namespace amit::sdl_windowing
{
    // Shows a CPU color buffer in an SDL window until the user closes it.
    // Keeps presenting the same frame (no re-render). Returns false on SDL failure.
    bool ShowColorBufferWindow(const graphics::ColorBuffer& color_buffer, const char* window_title);

    // Keyboard state sampled once per frame for the interactive loop.
    struct InteractiveInput
    {
        float dt_seconds    = 0.0f;
        bool  move_forward  = false;
        bool  move_backward = false;
        bool  move_left     = false;
        bool  move_right    = false;
        bool  move_up       = false;
        bool  move_down     = false;
        bool  quit          = false;
    };

    // Opens an SDL window and runs a simple render loop.
    // Each frame: poll input -> call update_and_render(input) -> present color_buffer.
    // Loop ends on window close, Escape, or when update_and_render returns false.
    bool RunInteractiveColorBufferLoop(graphics::ColorBuffer&                              color_buffer,
                                       const char*                                         window_title,
                                       const std::function<bool(const InteractiveInput&)>& update_and_render);

}  // namespace amit::sdl_windowing

#endif
