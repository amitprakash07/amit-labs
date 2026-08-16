#include <iostream>

#include "common/string_pool.h"
#include "win32_includes.h"
#include "win32_resource.h"
#include "platform/platform_systems.h"

amit::Window::Window()
{
    parent_window = nullptr;
    module_handle = GetModuleHandle(nullptr);
    icon          = nullptr;
    small_icon    = nullptr;
    cursor        = nullptr;
    background    = nullptr;
    width         = 0;
    height        = 0;
    class_name    = "Engine: Default window";
    menu_name     = "Engine: Default window";
    title_caption = "Engine: Default window";
    wnd_proc      = nullptr;
    show_state    = 0;
}

void amit::Window::SetToFullResolution()
{
    HWND desktopResolution = GetDesktopWindow();
    RECT fullResolution;
    GetWindowRect(desktopResolution, &fullResolution);
    width  = fullResolution.right;
    height = fullResolution.bottom;
}

namespace amit
{
    namespace util
    {
        WNDCLASSEX PlatformWndFrom(const Window& wnd)
        {
            WNDCLASSEX wnd_class_ex = {};
            wnd_class_ex.cbSize     = sizeof(WNDCLASSEX);
            wnd_class_ex.hInstance  = static_cast<HINSTANCE>(wnd.module_handle);
            wnd_class_ex.style      = 0;

            if (wnd.wnd_proc == nullptr)
            {
                wnd_class_ex.lpfnWndProc = DefWindowProc;
            }

            wnd_class_ex.cbClsExtra = 0;

            wnd_class_ex.cbWndExtra = 0;
            if (wnd.icon == nullptr)
            {
                wnd_class_ex.hIcon = LoadIcon(wnd_class_ex.hInstance, MAKEINTRESOURCE(IDI_BIG));
            }

            if (wnd.small_icon == nullptr)
            {
                wnd_class_ex.hIconSm = LoadIcon(wnd_class_ex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
            }

            if (wnd.cursor == nullptr)
            {
                wnd_class_ex.hCursor = LoadCursor(nullptr, IDC_ARROW);
            }

            if (wnd.background)
            {
                wnd_class_ex.hbrBackground = static_cast<HBRUSH>(IntToPtr(COLOR_BACKGROUND + 1));
            }

            wnd_class_ex.lpszMenuName  = wnd.menu_name.c_str();
            wnd_class_ex.lpszClassName = wnd.class_name.c_str();

            return wnd_class_ex;
        }
    }  // namespace util
}  // namespace amit

amit::WindowingSystem::WindowingSystem(int win_main_display_state)
    : IPlatformSystem(PlatformHandlerType::Windowing)
    , main_window_(nullptr)
    , main_window_handle_(nullptr)
    , win_main_display_state_(win_main_display_state)
{
    if (main_window_ == nullptr)
    {
        main_window_                = new Window();
        WNDCLASSEX wnd_class_info   = util::PlatformWndFrom(*main_window_);
        const ATOM registered_class = RegisterClassEx(&wnd_class_info);

        if (registered_class == NULL)
        {
            const char* errorCaption = "No Main Window Class";
            std::string errorMessage("Windows failed to register the main window's class: ");
            errorMessage += ErrorHandler()->GetFormattedLastError();
            MessageBox(nullptr, errorMessage.c_str(), errorCaption, MB_OK | MB_ICONERROR);
        }

        main_window_handle_ = ConstructWindow(main_window_);
    }
}

amit::WindowHandle amit::WindowingSystem::ConstructWindow(const Window* window_info)
{
    // Create the main window
    const Window* use_window = window_info;

    if (window_info == nullptr)
    {
        use_window = main_window_;
    }

    std::string window_caption_string = "Amit Prakash's - WindowsUtil Game";

    constexpr DWORD window_style         = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
    constexpr DWORD windowStyle_extended = WS_EX_OVERLAPPEDWINDOW;
    constexpr int   width                = CW_USEDEFAULT;
    constexpr int   height               = CW_USEDEFAULT;
    constexpr int   position_x           = CW_USEDEFAULT;
    constexpr int   position_y           = CW_USEDEFAULT;
    constexpr HWND  parent               = nullptr;
    constexpr HMENU menu                 = nullptr;
    const HINSTANCE instance             = static_cast<HINSTANCE>(use_window->module_handle);
    void*           userData             = nullptr;

    HWND window_handle = CreateWindowEx(windowStyle_extended,
                                        use_window->class_name.c_str(),
                                        window_caption_string.c_str(),
                                        window_style,
                                        position_x,
                                        position_y,
                                        width,
                                        height,
                                        parent,
                                        menu,
                                        instance,
                                        userData);
    if (window_handle == nullptr)
    {
        const char* errorCaption = "No Main Window";
        std::string errorMessage("Windows failed to create the main window: ");
        errorMessage += ErrorHandler()->GetFormattedLastError();
        MessageBox(nullptr, errorMessage.c_str(), errorCaption, MB_OK | MB_ICONERROR);
        return nullptr;
    }

    const int desiredWidth  = use_window->width;
    const int desiredHeight = use_window->height;

    // Calculate how much of the window is coming from the "non-client area"
    // (the borders and title bar)
    RECT window_coordinates;
    struct
    {
        long width;
        long height;
    } non_client_area_size;

    // Get the coordinates of the entire window
    if (GetWindowRect(window_handle, &window_coordinates) == FALSE)
    {
        std::string errorMessage("Windows failed to get the coordinates of the main window: ");
        errorMessage += ErrorHandler()->GetFormattedLastError();
        MessageBox(nullptr, errorMessage.c_str(), nullptr, MB_OK | MB_ICONERROR);
        return nullptr;
    }

    // Get the dimensions of the client area
    RECT client_dimensions;
    if (GetClientRect(window_handle, &client_dimensions) == FALSE)
    {
        std::string errorMessage(
            "Windows failed to get the dimensions of the main window's client "
            "area: ");
        errorMessage += ErrorHandler()->GetFormattedLastError();
        MessageBox(nullptr, errorMessage.c_str(), nullptr, MB_OK | MB_ICONERROR);
        return nullptr;
    }

    // Get the difference between them
    non_client_area_size.width  = (window_coordinates.right - window_coordinates.left) - client_dimensions.right;
    non_client_area_size.height = (window_coordinates.bottom - window_coordinates.top) - client_dimensions.bottom;

    // Resize the window
    BOOL shouldTheResizedWindowBeRedrawn = TRUE;
    if (MoveWindow(window_handle,
                   window_coordinates.left,
                   window_coordinates.top,
                   desiredWidth + non_client_area_size.width,
                   desiredHeight + non_client_area_size.height,
                   shouldTheResizedWindowBeRedrawn) == FALSE)
    {
        std::string errorMessage("Windows failed to resize the main window: ");
        errorMessage += ErrorHandler()->GetFormattedLastError();
        MessageBox(nullptr, errorMessage.c_str(), nullptr, MB_OK | MB_ICONERROR);
        return nullptr;
    }

    // Display the window in the initial state that Windows requested
    ShowWindow(window_handle, win_main_display_state_);

    all_window_map_[window_handle] = window_info;

    return window_handle;
}

//amit::ResultPtr CALLBACK amit::WindowingSystem::DefaultWndProc(Window*    window,
//                                                                   WndMessage message,
//                                                                   Param      w_param,
//                                                                   Param      l_param)
//{
//    switch (message)
//    {
//    case WM_CHAR:
//    {
//        // This isn't usually the best way to handle keyboard input in a real-time
//        // game, but it is a convenient way to handle an exit key (The WPARAM
//        // input parameter indicates which key was pressed, but this example
//        // program only cares about the escape key)
//        // if (i_wParam == VK_ESCAPE)
//        //{
//        //	// You do _not_ need to do this in your game,
//        //	// but this example program shows you how to display a "message
//        // box"
//        //	// that asks the user for confirmation
//        //	int result;
//        //	{
//        //		const char* caption = "Exit Program?";
//        //		const char* message = "Are you sure you want to quit?";
//        //		result = MessageBox(s_mainWindow, message, caption,
//        // MB_YESNO | MB_ICONQUESTION);
//        //	}
//        //	if (result == IDYES)
//        //	{
//        //		// Instruct Windows to send a WM_QUIT message
//        //		{
//        //			// The exit code is ignored,
//        //			// but just as an example of what's possible a
//        // different exit code is sent here
//        //			// than when the user closes the program in the
//        // usual way
//        //			// (by clicking the X in the upper-right hand
//        // corner).
//        //			/*int exitCode = 0;
//        //			PostQuitMessage(exitCode);*/
//        //			SendMessage(i_window, WM_NCDESTROY, i_wParam,
//        // i_lParam);
//        //		}
//        //		// For WM_CHAR messages, return 0 to indicate that it
//        // was processed 		return 0;
//        //	}
//        //	else
//        //	{
//        //		// If the user doesn't select "Yes" to quit
//        //		// then do nothing to keep running the program
//        //	}
//
//        //}
//        // If the key press wasn't handled pass it on to Windows to process in the
//        // default way
//        amit::common::utils::StringHash temp = amit::EngineCore::getStringPool()->FindString("KeyDown");
//        amit::common::SharedPointer<amit::Windows::WindowingSystem> tempWindowingSystem =
//            amit::EngineCore::getWindowingSystem();
//        amit::EngineCore::getMessagingSystem()->sendMessage(
//            temp, tempWindowingSystem.CastSharedPointer<amit::common::RTTI>(), reinterpret_cast<void*>(i_wParam));
//        break;
//    }
//    case WM_KEYDOWN:
//    {
//        amit::common::utils::StringHash temp = amit::EngineCore::getStringPool()->FindString("KeyDown");
//        amit::common::SharedPointer<amit::Windows::WindowingSystem> tempWindowingSystem =
//            amit::EngineCore::getWindowingSystem();
//        amit::EngineCore::getMessagingSystem()->sendMessage(
//            temp, tempWindowingSystem.CastSharedPointer<amit::common::RTTI>(), reinterpret_cast<void*>(i_wParam));
//        break;
//    }
//    case WM_LBUTTONDOWN:
//    {
//        amit::common::utils::StringHash temp = amit::EngineCore::getStringPool()->FindString("MouseButtonDownEvent");
//        amit::common::SharedPointer<amit::MouseController> tempMouseController =
//            amit::EngineCore::getMouseInputController();
//        WindowsParam windowsParameter;
//        windowsParameter.windowsMessage = i_message;
//        windowsParameter.wParam         = i_wParam;
//        windowsParameter.lParam         = i_lParam;
//        amit::EngineCore::getMessagingSystem()->sendMessage(
//            temp, tempMouseController.CastSharedPointer<amit::common::RTTI>(), reinterpret_cast<void*>(&windowsParameter));
//        break;
//    }
//    case WM_MOUSEMOVE:
//    {
//        amit::common::utils::StringHash temp = amit::EngineCore::getStringPool()->FindString("MouseMoveEvent");
//        amit::common::SharedPointer<amit::MouseController> tempMouseController =
//            amit::EngineCore::getMouseInputController();
//        WindowsParam windowsParameter;
//        windowsParameter.windowsMessage = i_message;
//        windowsParameter.wParam         = i_wParam;
//        windowsParameter.lParam         = i_lParam;
//        amit::EngineCore::getMessagingSystem()->sendMessage(
//            temp, tempMouseController.CastSharedPointer<amit::common::RTTI>(), reinterpret_cast<void*>(&windowsParameter));
//        break;
//    }
//    case WM_LBUTTONUP:
//    {
//        amit::common::utils::StringHash temp = amit::EngineCore::getStringPool()->FindString("MouseButtonUpEvent");
//        amit::common::SharedPointer<amit::MouseController> tempMouseController =
//            amit::EngineCore::getMouseInputController();
//        WindowsParam windowsParameter;
//        windowsParameter.windowsMessage = i_message;
//        windowsParameter.wParam         = i_wParam;
//        windowsParameter.lParam         = i_lParam;
//        amit::EngineCore::getMessagingSystem()->sendMessage(
//            temp, tempMouseController.CastSharedPointer<amit::common::RTTI>(), reinterpret_cast<void*>(&windowsParameter));
//        break;
//    }
//
//    // The window's nonclient area is being destroyed
//    case WM_NCDESTROY:
//    {
//        // mWindowingSystem->s_mainWindow = nullptr;
//        // When the main window is destroyed
//        // a WM_QUIT message should be sent
//        // (if this isn't done the application would continue to run with no
//        // window). This is where the exitCode in WaitForShutdown() comes from:
//        // mWindowingSystem->s_mainWindow = nullptr;
//        amit::EngineCore::getWindowingSystem()->s_mainWindow = nullptr;
//        int exitCode                                           = 0;  // Arbitrary de facto success code
//        PostQuitMessage(exitCode);                                   // This sends a WM_QUIT message
//                                                                     // For WM_NCDESTROY messages, return 0 to
//                                                                     // indicate that it was processed
//
//        return 0;
//    }
//    }  // Switch
//       // Pass any messages that weren't handled on to Windows
//    return DefWindowProc(window, message, w_param, l_param);
//}

amit::WindowingSystem::~WindowingSystem()
{
    for (auto it : all_window_map_)
    {
        DeleteWindow(it.first);
    }

    DeleteWindow(main_window_, true);
}

bool amit::WindowingSystem::DeleteWindow(WindowHandle wnd_handle, bool delete_main_window)
{
    bool          success  = true;
    const Window* wnd_info = all_window_map_[wnd_handle];

    if (!delete_main_window && wnd_info->module_handle != main_window_->module_handle)
    {
        if (UnregisterClass(wnd_info->class_name.c_str(), static_cast<HINSTANCE>(wnd_info->module_handle)) == FALSE)
        {
            const char* errorCaption = "Couldn't Unregister Main Window Class";
            std::string errorMessage("Windows failed to unregister the main window's class: ");
            errorMessage += ErrorHandler()->GetFormattedLastError();
            MessageBox(nullptr, errorMessage.c_str(), errorCaption, MB_OK | MB_ICONERROR);
            success &= false;
        }
    }

    if (!delete_main_window && wnd_handle != main_window_handle_)
    {
        if (DestroyWindow(static_cast<HWND>(wnd_handle)) == FALSE)
        {
            const char* errorCaption = "Couldn't Destroy Main Window";
            std::string errorMessage("Windows failed to destroy the main window: ");
            errorMessage += ErrorHandler()->GetFormattedLastError();
            MessageBox(nullptr, errorMessage.c_str(), errorCaption, MB_OK | MB_ICONERROR);
            success &= false;
        }
    }

    return success;
}
