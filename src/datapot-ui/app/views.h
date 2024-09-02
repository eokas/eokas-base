#pragma once

#include "imwidgets/widget.h"

namespace eokas::datapot {
    struct MyMainMenuBar :public MainMenuBar {
        MyMainMenuBar();
        
        MenuItem* actionFileNew;
        MenuItem* actionFileOpen;
        MenuItem* actionFileSave;
    };
    
    struct MyMainWindow :public MainWindow {
        MyMainWindow();
        
        MyMainMenuBar* mainMenuBar;
    };
}
