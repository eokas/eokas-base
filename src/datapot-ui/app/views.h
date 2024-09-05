#pragma once

#include "imwidgets/widget.h"

namespace eokas::datapot {
    struct MyMainMenuBar :public MainMenuBar {
        MenuItem* actionFileNew;
        MenuItem* actionFileOpen;
        MenuItem* actionFileSave;
        
        MyMainMenuBar();
    };
    
    struct MyCreateLibraryDialog :public Dialog {
        FieldInput* fieldName;
        FieldDirectory* fieldHome;
        Button* button;
        
        MyCreateLibraryDialog();
    };
    
    struct MySchemaBrowserWindow :public Window {
        MySchemaBrowserWindow();
    };
    
    struct MyObjectBrowserWindow :public Window {
        MyObjectBrowserWindow();
    };
    
    struct MyToastDialog :public Dialog {
        Text* text;
        
        MyToastDialog();
        void open(const String& content);
    };
    
    struct MyMainWindow :public MainWindow {
        MyMainMenuBar* mainMenuBar;
        MySchemaBrowserWindow* schemaBrowser;
        MyObjectBrowserWindow* objectBrowser;
        MyCreateLibraryDialog* createLibrary;
        MyToastDialog* toast;
        
        MyMainWindow();
    };
}
