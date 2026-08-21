#ifndef CORE_SDL_WINDOWING_COLOR_BUFFER_WINDOW_H_
#define CORE_SDL_WINDOWING_COLOR_BUFFER_WINDOW_H_

#include <functional>
#include <string>

#include "core/common/settings_registry.h"
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

    // Optional ImGui side-panel content. Live ColorBuffer stays scene-sized; the SDL window grows
    // by a right sidebar for Settings (inputs) and Output (e.g. frame stats).
    // request_save_ppm: set by Settings "Save PPM"; the app clears it after exporting the ColorBuffer.
    // Screenshot capture is handled inside the loop after ImGui is drawn (full window including UI).
    // action_status: optional; filled with a short message after screenshot (and usable by the app for PPM too).
    struct InteractiveUiState
    {
        app_settings::SettingsRegistry* settings_registry = nullptr;
        const std::string*              output_text       = nullptr;
        bool*                           request_save_ppm  = nullptr;
        std::string*                    action_status     = nullptr;
        int                             sidebar_width_px  = 320;
    };

    // Opens an SDL window and runs a simple render loop.
    // Each frame: poll input -> update_and_render -> blit ColorBuffer (left) -> ImGui sidebar -> Present.
    // ui_state may be null. Movement keys are ignored while ImGui wants keyboard.
    bool RunInteractiveColorBufferLoop(graphics::ColorBuffer&                              color_buffer,
                                       const char*                                         window_title,
                                       const std::function<bool(const InteractiveInput&)>& update_and_render,
                                       InteractiveUiState*                                 ui_state = nullptr);

}  // namespace amit::sdl_windowing

#endif
