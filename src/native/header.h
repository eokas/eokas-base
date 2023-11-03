
#ifndef _EOKAS_NATIVE_HEADER_H_
#define _EOKAS_NATIVE_HEADER_H_

#include <eokas-base/main.h>

#if (_EOKAS_OS == _EOKAS_OS_WIN32 || _EOKAS_OS == _EOKAS_OS_WIN64)
#include <Windows.h>
using WindowHandle = HWND;
#elif (_EOKAS_OS == _EOKAS_OS_LINUX)
using WindowHandle = void*
#elif (_EOKAS_OS == _EOKAS_OS_MACOS)
    using WindowHandle = void*
#elif (_EOKAS_OS == _EOKAS_OS_IOS)
    using WindowHandle = void*
#elif (_EOKAS_OS == _EOKAS_OS_ANDROID)
    using WindowHandle = void*
#endif

#endif //_EOKAS_NATIVE_HEADER_H_
