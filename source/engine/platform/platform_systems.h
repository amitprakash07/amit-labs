#ifndef ENGINE_PLATFORM_PLATFORM_SYSTEMS_H_
#define ENGINE_PLATFORM_PLATFORM_SYSTEMS_H_

#include <map>

#include "platform.h"
#include "platform_objects.h"

namespace amit
{
    class IPlatformSystem
    {
    public:
        IPlatformSystem()                  = delete;
        virtual bool StartUp()             = 0;
        virtual bool ShutDown()            = 0;
        virtual bool IsInitialized() const = 0;
        virtual ~IPlatformSystem()         = default;

    protected:
        IPlatformSystem(PlatformHandlerType handler_type)
        {
            handler_type_ = handler_type;
        }

        bool Registered() const
        {
            return handler_type_ != PlatformHandlerType::UnRegistered;
        }

        PlatformHandlerType handler_type_;
    };

    class FileSystem : public IPlatformSystem
    {
    public:
        friend class Platform;
        ~FileSystem() override = default;

        static FilePath GetCurrentModulePath();

        bool Copy(const char*  path_source,
                  const char*  path_target,
                  bool         overwrite     = false,
                  bool         modify_time   = false,
                  std::string* error_message = nullptr);

        bool CreateDirectoryIfNecessary(const std::string& directory_path, std::string* error_message = nullptr);

        bool DoesFileExist(const std::string& file_path, std::string* error_message = nullptr);

        bool GetLastWriteTime(const std::string& file_path,
                              uint64_t&          last_write_time,
                              std::string*       error_message = nullptr);

        bool StartUp() override
        {
            return true;
        }

        bool ShutDown() override
        {
            return true;
        }

        bool IsInitialized() const override
        {
            return true;
        }

    private:
        FileSystem()
            : IPlatformSystem(PlatformHandlerType::File)
        {
        }
    };

    class ProcessSystem : public IPlatformSystem
    {
    public:
        friend class Platform;
        ~ProcessSystem() override = default;

        bool ExecuteCommand(const std::string& command, std::string* error_message = nullptr);

        bool ExecuteCommand(const std::string& command,
                            const std::string* optional_arguments = nullptr,
                            std::string*       error_message      = nullptr);

        bool StartUp() override
        {
            return true;
        }

        bool ShutDown() override
        {
            return true;
        }

        bool IsInitialized() const override
        {
            return true;
        }

    private:
        ProcessSystem()
            : IPlatformSystem(PlatformHandlerType::Process)
        {
        }
    };

    typedef unsigned long long ErrorCode;
    class ErrorSystem : public IPlatformSystem
    {
        friend class Platform;

    public:
        ErrorCode   GetLastError(std::string* error_message = nullptr);
        std::string GetFormattedErrorMessage(ErrorCode error_code);
        std::string GetFormattedLastError()
        {
            return GetFormattedErrorMessage(GetLastError());
        }

        bool StartUp() override
        {
            return true;
        }

        bool ShutDown() override
        {
            return true;
        }

        bool IsInitialized() const override
        {
            return true;
        }

    private:
        ErrorSystem()
            : IPlatformSystem(PlatformHandlerType::Error)
        {
        }
    };

    enum class LoggingMessageType
    {

    };

    class LoggingSystem : public IPlatformSystem
    {
        friend class Platform;

    public:
        void Log(LoggingMessageType type, const char* formatted_message, ...);

        bool StartUp() override
        {
            return true;
        }

        bool ShutDown() override
        {
            return true;
        }

        bool IsInitialized() const override
        {
            return true;
        }

    private:
        LoggingSystem()
            : IPlatformSystem(PlatformHandlerType::Logging)
        {
        }
    };

    using Icon          = void*;
    using Cursor        = Icon;
    using Brush         = void*;
    using WindowStyle   = void*;
    using ModuleHandle  = void*;
    using WindowHandle  = void*;
    using ProcessHandle = void*;
    using Param         = void*;
    using ResultPtr     = void*;
    using Menu          = void*;
    using WndMessage    = unsigned int;

    // TO DO: Incompatible signature
    typedef ResultPtr(__stdcall* WndProc)(WindowHandle, WndMessage, Param, Param);

    struct Window
    {
        Window();
        void         SetToFullResolution();
        WindowHandle parent_window;
        ModuleHandle module_handle;
        WndProc      wnd_proc;
        Icon         icon;
        Icon         small_icon;
        Cursor       cursor;
        Brush        background;
        int          width;
        int          height;
        std::string  class_name;
        std::string  menu_name;
        std::string  title_caption;
        int          show_state;
    };

    class WindowingSystem : public IPlatformSystem
    {
        friend class Platform;

    public:
        WindowHandle ConstructWindow(const Window* window_info = nullptr);

        WindowHandle GetMainWindowHandle() const
        {
            return main_window_handle_;
        }

        virtual ~WindowingSystem();

    private:
        //static ResultPtr CALLBACK DefaultWndProc(Window* window, WndMessage message, Param w_param, Param l_param);

        bool DeleteWindow(WindowHandle wnd_handle, bool delete_main_window = false);

        WindowingSystem(int win_main_display_state);

        bool StartUp() override
        {
            return true;
        }

        bool ShutDown() override
        {
            return true;
        }

        bool IsInitialized() const override
        {
            return true;
        }

        std::map<WindowHandle, const Window*> all_window_map_;
        Window*                               main_window_;
        WindowHandle                          main_window_handle_;
        int                                   win_main_display_state_;
    };
}  // namespace amit

#endif