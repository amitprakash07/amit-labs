#ifndef CORE_COMMON_SETTINGS_REGISTRY_H_
#define CORE_COMMON_SETTINGS_REGISTRY_H_

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace amit::app_settings
{
    enum class SettingType : std::uint8_t
    {
        kFloat,
        kBool,
        kInt
    };

    struct SettingEntry
    {
        SettingType type  = SettingType::kFloat;
        std::string label;
        void*       value = nullptr;
        float       float_min = 0.0f;
        float       float_max = 1.0f;
        int         int_min   = 0;
        int         int_max   = 100;
    };

    // ImGui-agnostic registry of live setting pointers. UI layers walk Entries().
    class SettingsRegistry
    {
    public:
        void AddFloat(const char* label, float* value, float min_value, float max_value)
        {
            SettingEntry entry;
            entry.type      = SettingType::kFloat;
            entry.label     = label != nullptr ? label : "float";
            entry.value     = value;
            entry.float_min = min_value;
            entry.float_max = max_value;
            entries_.push_back(std::move(entry));
        }

        void AddBool(const char* label, bool* value)
        {
            SettingEntry entry;
            entry.type  = SettingType::kBool;
            entry.label = label != nullptr ? label : "bool";
            entry.value = value;
            entries_.push_back(std::move(entry));
        }

        void AddInt(const char* label, int* value, int min_value, int max_value)
        {
            SettingEntry entry;
            entry.type    = SettingType::kInt;
            entry.label   = label != nullptr ? label : "int";
            entry.value   = value;
            entry.int_min = min_value;
            entry.int_max = max_value;
            entries_.push_back(std::move(entry));
        }

        const std::vector<SettingEntry>& Entries() const
        {
            return entries_;
        }

        std::size_t Size() const
        {
            return entries_.size();
        }

        bool Empty() const
        {
            return entries_.empty();
        }

    private:
        std::vector<SettingEntry> entries_;
    };

}  // namespace amit::app_settings

#endif
