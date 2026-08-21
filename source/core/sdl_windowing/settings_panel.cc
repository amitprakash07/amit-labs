#include "core/sdl_windowing/settings_panel.h"

#include <imgui.h>

namespace amit::sdl_windowing
{
    namespace
    {
        constexpr ImGuiWindowFlags kSidePanelFlags =
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoBringToFrontOnFocus;

        void DrawSettingsSection(const app_settings::SettingsRegistry& settings_registry)
        {
            if (settings_registry.Empty())
            {
                ImGui::TextUnformatted("No settings registered.");
                return;
            }

            for (const app_settings::SettingEntry& entry : settings_registry.Entries())
            {
                if (entry.value == nullptr)
                {
                    continue;
                }

                switch (entry.type)
                {
                case app_settings::SettingType::kFloat:
                    ImGui::SliderFloat(entry.label.c_str(),
                                       static_cast<float*>(entry.value),
                                       entry.float_min,
                                       entry.float_max);
                    break;
                case app_settings::SettingType::kBool:
                    ImGui::Checkbox(entry.label.c_str(), static_cast<bool*>(entry.value));
                    break;
                case app_settings::SettingType::kInt:
                    ImGui::SliderInt(
                        entry.label.c_str(), static_cast<int*>(entry.value), entry.int_min, entry.int_max);
                    break;
                }
            }
        }
    }  // namespace

    void DrawSidePanels(int                                   framebuffer_width,
                        int                                   framebuffer_height,
                        int                                   sidebar_width,
                        const app_settings::SettingsRegistry* settings_registry,
                        const std::string*                    output_text,
                        bool*                                 request_save_ppm,
                        bool*                                 request_save_screenshot)
    {
        if (framebuffer_width <= 0 || framebuffer_height <= 0 || sidebar_width <= 0)
        {
            return;
        }

        const float sidebar_x  = static_cast<float>(framebuffer_width);
        const float sidebar_w  = static_cast<float>(sidebar_width);
        const float total_h    = static_cast<float>(framebuffer_height);
        const float settings_h = total_h * 0.5f;
        const float output_h   = total_h - settings_h;

        ImGui::SetNextWindowPos(ImVec2(sidebar_x, 0.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(sidebar_w, settings_h), ImGuiCond_Always);
        if (ImGui::Begin("Settings", nullptr, kSidePanelFlags))
        {
            if (settings_registry != nullptr)
            {
                DrawSettingsSection(*settings_registry);
            }
            else
            {
                ImGui::TextUnformatted("No settings registered.");
            }

            if (request_save_ppm != nullptr || request_save_screenshot != nullptr)
            {
                ImGui::Separator();
            }
            if (request_save_ppm != nullptr)
            {
                if (ImGui::Button("Save PPM", ImVec2(-1.0f, 0.0f)))
                {
                    *request_save_ppm = true;
                }
            }
            if (request_save_screenshot != nullptr)
            {
                if (ImGui::Button("Save Screenshot", ImVec2(-1.0f, 0.0f)))
                {
                    *request_save_screenshot = true;
                }
            }
        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(sidebar_x, settings_h), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(sidebar_w, output_h), ImGuiCond_Always);
        if (ImGui::Begin("Output", nullptr, kSidePanelFlags))
        {
            if (output_text != nullptr && !output_text->empty())
            {
                ImGui::TextUnformatted(output_text->c_str());
            }
            else
            {
                ImGui::TextUnformatted("No output.");
            }
        }
        ImGui::End();
    }

}  // namespace amit::sdl_windowing
