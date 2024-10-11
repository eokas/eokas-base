
#ifndef _EOKAS_NATIVE_WINDOW_H_
#define _EOKAS_NATIVE_WINDOW_H_

#include "./header.h"

#if (_EOKAS_OS == _EOKAS_OS_WIN32 || _EOKAS_OS == _EOKAS_OS_WIN64)
#include <Windows.h>
using WindowHandle = HWND;
#elif (_EOKAS_OS == _EOKAS_OS_LINUX)
using WindowHandle = void*;
#elif (_EOKAS_OS == _EOKAS_OS_MACOS)
using WindowHandle = void*;
#elif (_EOKAS_OS == _EOKAS_OS_IOS)
using WindowHandle = void*;
#elif (_EOKAS_OS == _EOKAS_OS_ANDROID)
    using WindowHandle = void*;
#endif

namespace eokas
{
    struct WindowDesc;
    
    class Window
    {
    public:
        Window();
        virtual ~Window();
        
        bool create();
        void close();
        
        const WindowHandle& getHandle() const;
        
        const String& getTitle() const;
        void setTitle(const String& title);
        
        bool isFullScreen() const;
        void setFullScree(bool val);
        
        const Rect& getRect() const;
        void setRect(const Rect& rect);
        
        void minimize();
        void maximize();
        
    private:
        WindowDesc* mDesc;
    };
}

#endif //_EOKAS_NATIVE_WINDOW_H_
