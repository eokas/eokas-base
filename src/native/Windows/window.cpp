
#include "../Window.h"

namespace eokas
{
    LRESULT CALLBACK OnWindowEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
            // Quit when we close the main window
            case WM_CLOSE:
            {
                PostQuitMessage(0);
                return 0;
            }

            /*
            // Quit when we click the "quit" button
            case WM_COMMAND:
            {
                if (reinterpret_cast<HWND>(lParam) == button)
                {
                    PostQuitMessage(0);
                    return 0;
                }
            }
             */
        }

        return DefWindowProc(handle, message, wParam, lParam);
    }

    Window::Window(const String& title, bool isFullScreen)
        : mTitle(title)
        , mIsFullScreen(isFullScreen)
        , mHandle(nullptr)
    { }

    void Window::create()
    {
        HINSTANCE instance = GetModuleHandle(nullptr);

        // Define a class for our main window
        WNDCLASS windowClass;
        windowClass.style         = 0;
        windowClass.lpfnWndProc   = &OnWindowEvent;
        windowClass.cbClsExtra    = 0;
        windowClass.cbWndExtra    = 0;
        windowClass.hInstance     = instance;
        windowClass.hIcon         = nullptr;
        windowClass.hCursor       = nullptr;
        windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
        windowClass.lpszMenuName  = nullptr;
        windowClass.lpszClassName = TEXT("WindowClassName");
        RegisterClass(&windowClass);

        // Let's create the main window
        HWND window = CreateWindow(
            TEXT("WindowClassName"), TEXT(mTitle.cstr()),
            WS_OVERLAPPED | WS_MAXIMIZE | WS_MINIMIZE,
            200, 200,
            660, 520,
            nullptr,
            nullptr,
            instance,
            nullptr
        );

        this->mHandle = window;
    }
}
