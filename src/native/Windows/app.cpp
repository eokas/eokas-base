
#include "../App.h"

namespace eokas
{
    App::App(int argc, char **argv)
    {

    }

    int App::exec()
    {
        MSG message;
        message.message = static_cast<UINT>(~WM_QUIT);
        while (message.message != WM_QUIT)
        {
            if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
            else
            {

            }
        }

        return 0;
    }
}