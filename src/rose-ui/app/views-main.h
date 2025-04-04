#pragma once

#include "imwidgets/widget.h"
#include "./views-library.h"
#include "./views-schema.h"
#include "./views-object.h"

namespace eokas::datapot {
    struct MyMainMenuBar :public UIMainMenuBar {
        UIMenuItem* actionFileNew;
        UIMenuItem* actionFileOpen;
        UIMenuItem* actionFileSave;
        
        MyMainMenuBar();
    };
    
    struct MyToastDialog :public UIDialog {
        UIText* text;
        
        MyToastDialog();
        void open(const String& content);
    };
    
    struct MyMainWindow :public UIMainWindow {
        MyMainMenuBar* mainMenuBar;
        MyToastDialog* toast;
        
        MyLibraryCreatorDialog* libraryCreator;
        
        MySchemaBrowserWindow* schemaBrowser;
        MySchemaCreatorDialog* schemaCreator;
        MySchemaSelectorDialog* schemaSelector;
        
        MyObjectBrowserWindow* objectBrowser;
        MyObjectCreatorDialog* objectCreator;
        
        MyMainWindow();
    };
}
