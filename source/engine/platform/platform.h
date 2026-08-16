#ifndef ENGINE_PLATFORM_PLATFORM_H_
#define ENGINE_PLATFORM_PLATFORM_H_

#include <string>

namespace amit
{
    class FileSystem;
    class ProcessSystem;
    class ErrorSystem;
    class LoggingSystem;
    class WindowingSystem;
    class IPlatformSystem;

    using ProgramInstance = void*;

    struct EngineStartupInfo
    {
        ProgramInstance instance;
        int             win_display_state;
    };

    enum class PlatformHandlerType : int8_t
    {
        UnRegistered = -1,
        File         = 0,
        Process,
        Error,
        Logging,
        Windowing,
        Count
    };

    class Platform
    {
    public:
        virtual ~Platform() = default;
        static [[nodiscard]] Platform* Instance();

        IPlatformSystem* GetHandler(PlatformHandlerType handler_type) const
        {
            return platform_handlers_[static_cast<uint8_t>(handler_type)];
        }

        bool IsHandlerInitialized(PlatformHandlerType handler_type) const
        {
            return GetHandler(handler_type) != nullptr;
        }

        // Environment variable
        bool GetEnvVar(const char* env_var, std::string& env_val, std::string* error_message = nullptr);

        bool SetEnvVar(const char* env_var, const std::string& env_val, std::string* error_message = nullptr);

    protected:
        static [[nodiscard]] bool Initialize(const EngineStartupInfo* engine_startup_info = nullptr);
        static [[nodiscard]] bool ShutDown();

    private:
        Platform()
            : platform_handlers_{}
            , initialized_(false)
        {
        }

        IPlatformSystem* platform_handlers_[static_cast<uint8_t>(PlatformHandlerType::Count)];
        bool              initialized_;
        static Platform*  platform_;
    };

    inline ErrorSystem* ErrorHandler()
    {
        return reinterpret_cast<ErrorSystem*>(Platform::Instance()->GetHandler(PlatformHandlerType::Error));
    }

    inline FileSystem* FileHandler()
    {
        return reinterpret_cast<FileSystem*>(Platform::Instance()->GetHandler(PlatformHandlerType::File));
    }

    inline ProcessSystem* ProcessHandler()
    {
        return reinterpret_cast<ProcessSystem*>(Platform::Instance()->GetHandler(PlatformHandlerType::Process));
    }

    inline LoggingSystem* LoggingHandler()
    {
        return reinterpret_cast<LoggingSystem*>(Platform::Instance()->GetHandler(PlatformHandlerType::Logging));
    }

    inline WindowingSystem* WindowHandler()
    {
        return reinterpret_cast<WindowingSystem*>(Platform::Instance()->GetHandler(PlatformHandlerType::Windowing));
    }



}  // namespace amit

#endif
