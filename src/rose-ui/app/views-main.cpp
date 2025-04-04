#include "./views-main.h"
#include "./logic.h"
#include "datapot/library.h"

namespace eokas::datapot {
    MyMainMenuBar::MyMainMenuBar() {
        auto file = this->add<UIMenu>("File");
        actionFileNew = file->add<UIMenuItem>("New Library ...");
        actionFileNew->onClick = []() {
            if(Logic::instance().actionFileNew) Logic::instance().actionFileNew();
        };
        actionFileOpen = file->add<UIMenuItem>("Open Library ...");
        actionFileOpen->onClick = [](){
            Logic::instance().openLibrary();
        };
        actionFileSave = file->add<UIMenuItem>("Save ...");
        actionFileSave->onClick = [](){
            if(Logic::instance().actionFileSave) Logic::instance().actionFileSave();
        };
        
        auto edit = this->add<UIMenu>("Edit");
        
        auto about = this->add<UIMenu>("About");
    }
    
    MyToastDialog::MyToastDialog()
        : UIDialog(Vector2(500, 100), false) {
        text = this->add<UIText>("");
    }
    
    void MyToastDialog::open(const eokas::String& content) {
        this->text->value = content;
        this->show();
    }
    
    MyMainWindow::MyMainWindow()
        : UIMainWindow(true)
    {
        this->name = "DataPot";
        mainMenuBar = this->add<MyMainMenuBar>();
        toast = this->add<MyToastDialog>();
        
        libraryCreator = this->add<MyLibraryCreatorDialog>();
        
        schemaBrowser = this->add<MySchemaBrowserWindow>();
        schemaCreator = this->add<MySchemaCreatorDialog>();
        schemaSelector = this->add<MySchemaSelectorDialog>();
        
        objectBrowser = this->add<MyObjectBrowserWindow>();
        objectCreator = this->add<MyObjectCreatorDialog>();
        
        Logic::instance().onError = [this](const Result& error) {
            toast->open(error.message);
        };
        Logic::instance().actionFileNew = [this]() {
            libraryCreator->show();
        };
        Logic::instance().onCreateLibrary = [this]() {
            schemaBrowser->schemaList->reloadSchemaList();
        };
        Logic::instance().onOpenLibrary = [this]() {
            schemaBrowser->schemaList->reloadSchemaList();
        };
        Logic::instance().actionNewSchema = [this]() {
            schemaCreator->show();
        };
        Logic::instance().onCreateSchema = [this]() {
            schemaBrowser->schemaList->reloadSchemaList();
        };
        Logic::instance().onSelectSchema = [this]() {
            schemaBrowser->schemaProperties->reloadSchemaProperties();
        };

        Logic::instance().actionNewObject = [this]() {
            objectCreator->show();
        };
    }
}
