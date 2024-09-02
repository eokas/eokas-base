#include "./window.h"

namespace eokas::datapot {
    MyMainWindow::MyMainWindow() {
        mainMenuBar = this->addChild<MyMainMenuBar>();
    }
}
