
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
    class Window
    {
    public:
        Window(const String& title, bool isFullScreen);

        void create();

        AccessValue<Vector2> position;
        AccessValue<Vector2> size;

    private:
        String mTitle;
        bool mIsFullScreen;
        WindowHandle mHandle;
    };
}

#endif //_EOKAS_NATIVE_WINDOW_H_
