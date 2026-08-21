#ifndef CORE_SDL_WINDOWING_SETTINGS_PANEL_H_
#define CORE_SDL_WINDOWING_SETTINGS_PANEL_H_

#include <string>

#include "core/common/settings_registry.h"

namespace amit::sdl_windowing
{
    // Draws Settings (top) and Output (bottom) ImGui panels fixed to the right of the framebuffer.
    void DrawSidePanels(int                                   framebuffer_width,
                        int                                   framebuffer_height,
                        int                                   sidebar_width,
                        const app_settings::SettingsRegistry* settings_registry,
                        const std::string*                    output_text,
                        bool*                                 request_save_ppm,
                        bool*                                 request_save_screenshot);

}  // namespace amit::sdl_windowing

#endif
