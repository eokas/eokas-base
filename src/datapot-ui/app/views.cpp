#include "./views.h"
#include "./logic.h"

namespace eokas::datapot {
    MyMainMenuBar::MyMainMenuBar()
    {
        auto file = this->add<Menu>("File");
        actionFileNew = file->add<MenuItem>("New Library ...");
        actionFileNew->onClick = std::bind(&Logic::actionFileNew, &Logic::instance());
        actionFileOpen = file->add<MenuItem>("Open Library ...");
        actionFileOpen->onClick = std::bind(&Logic::actionFileOpen, &Logic::instance());
        actionFileSave = file->add<MenuItem>("Save ...");
        actionFileSave->onClick = std::bind(&Logic::actionFileSave, &Logic::instance());
        
        auto edit = this->add<Menu>("Edit");
        
        auto about = this->add<Menu>("About");
    }
    
    MyCreateLibraryDialog::MyCreateLibraryDialog()
        : Dialog("CreateLibrary", true) {
        fieldName = this->add<FieldInput>("Library Name");
        fieldHome = this->add<FieldDirectory>("Library Home");
        button = this->add<Button>("Create");
        button->onClick = [this]() {
            String name = fieldName->value;
            String home = fieldHome->value;
            if (!name.isEmpty() && !home.isEmpty()) {
                Logic::instance().createLibrary(name, home);
            }
        };
    }
    
    MyToastDialog::MyToastDialog()
        : Dialog("", false) {
        text = this->add<Text>("");
    }
    
    void MyToastDialog::open(const eokas::String& content) {
        this->text->content = content;
        Dialog::open();
    }
    
    MyMainWindow::MyMainWindow()
        : MainWindow("datapot", true)
    {
        mainMenuBar = this->add<MyMainMenuBar>();
        createLibraryDialog = this->add<MyCreateLibraryDialog>();
        toastDialog = this->add<MyToastDialog>();
    }
}
