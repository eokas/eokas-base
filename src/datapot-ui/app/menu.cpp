
#include "./menu.h"

namespace eokas::datapot {
    MyMainMenuBar::MyMainMenuBar() {
        auto file = this->addChild<Menu>("File");
        file->addChild<MenuItem>("Open ...");
        
        auto edit = this->addChild<Menu>("Edit");
        
        auto about = this->addChild<Menu>("About");
    }
}
