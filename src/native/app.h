
#ifndef _EOKAS_NATIVE_APP_H_
#define _EOKAS_NATIVE_APP_H_

#include "./header.h"

namespace eokas
{
    class App
    {
    public:
        App(int argc, char** argv);

        int exec();
    };
}

#endif//_EOKAS_NATIVE_APP_H_
