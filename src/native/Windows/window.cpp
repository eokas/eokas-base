
#include "../Window.h"

namespace eokas
{
    struct WindowDesc {
        WindowHandle handle = nullptr;
        String title = "";
        Rect rect = {};
        bool fullScreen = false;
    };
    
    LRESULT CALLBACK OnWindowEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
            // Quit when we close the main window
            case WM_CLOSE: {
                PostQuitMessage(0);
                return 0;
            }

            /*
            // Quit when we click the "quit" button
            case WM_COMMAND: {
                if (reinterpret_cast<HWND>(lParam) == button) {
                    PostQuitMessage(0);
                    return 0;
                }
            }
             */
        }

        return DefWindowProc(handle, message, wParam, lParam);
    }
    
    Window::Window()
        : mDesc(new WindowDesc()) {
        
    }
    
    Window::~Window() {
        this->close();
        _DeletePointer(mDesc);
    }
    
    bool Window::create() {
        HINSTANCE instance = GetModuleHandle(nullptr);
        
        // Define a class for our main window
        WNDCLASS wc;
        wc.style         = 0;
        wc.lpfnWndProc   = &OnWindowEvent;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = instance;
        wc.hIcon         = nullptr;
        wc.hCursor       = nullptr;
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
        wc.lpszMenuName  = nullptr;
        wc.lpszClassName = TEXT("WindowClassName");
        RegisterClass(&wc);
        
        // Let's create the main window
        mDesc->handle = CreateWindow(
            TEXT("WindowClassName"), TEXT(mDesc->title.cstr()),
            WS_OVERLAPPED | WS_MAXIMIZE | WS_MINIMIZE,
            CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT, CW_USEDEFAULT,
            nullptr,
            nullptr,
            instance,
            nullptr
        );
        
        return mDesc->handle != nullptr;
    }
    
    void Window::close() {
        if(mDesc->handle != nullptr) {
            DestroyWindow(mDesc->handle);
            mDesc->handle = NULL;
        }
    }
    
    const WindowHandle& Window::getHandle() const {
        return mDesc->handle;
    }
    
    const String& Window::getTitle() const {
        return mDesc->title;
    }
    
    void Window::setTitle(const String& title) {
        mDesc->title = title;
        if(mDesc->handle != nullptr) {
            SetWindowText(mDesc->handle, title.cstr());
        }
    }
    
    bool Window::isFullScreen() const {
        return mDesc->fullScreen;
    }
    
    void Window::setFullScree(bool val) {
        mDesc->fullScreen = val;
        if(mDesc->handle != nullptr) {
            if(val) {
                SetWindowLong(mDesc->handle, GWL_STYLE, WS_POPUP);
            }
            else {
                SetWindowLong(mDesc->handle, GWL_STYLE, WS_OVERLAPPEDWINDOW);
            }
        }
    }
    
    const Rect& Window::getRect() const {
        return mDesc->rect;
    }
    
    void Window::setRect(const Rect& rect) {
        mDesc->rect = rect;
        if(mDesc->handle != nullptr) {
            SetWindowPos(mDesc->handle, nullptr, rect.x, rect.y, rect.width, rect.height, SWP_FRAMECHANGED);
        }
    }
    
    void Window::minimize() {
        if(mDesc->handle != nullptr) {
            ShowWindow(mDesc->handle, SW_MINIMIZE);
        }
    }
    
    void Window::maximize() {
        if(mDesc->handle != nullptr) {
            if (!IsZoomed(mDesc->handle)) {
                ShowWindow(mDesc->handle, SW_MAXIMIZE);
            } else {
                ShowWindow(mDesc->handle, SW_RESTORE);
            }
        }
    }
}
