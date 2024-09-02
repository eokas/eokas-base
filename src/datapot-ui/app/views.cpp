#include "./views.h"
#include "./controllers.h"

namespace eokas::datapot {
    MyMainMenuBar::MyMainMenuBar() {
        auto file = this->addChild<Menu>("File");
        actionFileNew = file->addChild<MenuItem>("New");
        actionFileNew->onClick = std::bind(&Controller::actionNewFile, &Controller::instance());
        actionFileOpen = file->addChild<MenuItem>("Open ...");
        actionFileOpen->onClick = std::bind(&Controller::actionOpenFile, &Controller::instance());
        actionFileSave = file->addChild<MenuItem>("Save ...");
        actionFileSave->onClick = std::bind(&Controller::actionSaveFile, &Controller::instance());
        
        auto edit = this->addChild<Menu>("Edit");
        
        auto about = this->addChild<Menu>("About");
    }
    
    MyMainWindow::MyMainWindow() {
        mainMenuBar = this->addChild<MyMainMenuBar>();
    }
}
