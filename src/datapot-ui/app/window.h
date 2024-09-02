#pragma once
#include "imwidgets/widget.h"
#include "./menu.h"

namespace eokas::datapot {
    struct MyMainWindow :public MainWindow {
        MyMainMenuBar* mainMenuBar;
        
        MyMainWindow();
    };
}
