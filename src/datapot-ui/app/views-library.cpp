#include "./views-library.h"
#include "./logic.h"
#include "datapot/library.h"

namespace eokas::datapot {
    MyLibraryCreatorDialog::MyLibraryCreatorDialog()
        : UIDialog(Vector2(400, 200), true) {
        this->name = "Create Library";
        
        properties = this->add<UIPropertiesView>();
        auto libraryName = properties->addInput("Library Name", "");
        auto libraryHome = properties->addFolder("Library Home");
        button = this->add<UIButton>("Create");
        button->onClick = [=]() {
            String name = libraryName->value.string().trim();
            String home = libraryHome->value.string().trim();
            if (!name.isEmpty() && !home.isEmpty()) {
                Result result = Logic::instance().createLibrary(name, home);
                if(result.ok) {
                    this->hide();
                }
            }
        };
    }
    
}