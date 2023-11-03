
#ifndef _EOKAS_NATIVE_WINDOW_H_
#define _EOKAS_NATIVE_WINDOW_H_

#include "./header.h"

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
