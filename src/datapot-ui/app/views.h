#pragma once

#include "imwidgets/widget.h"

namespace eokas::datapot {
    struct MyMainMenuBar :public UIMainMenuBar {
        UIMenuItem* actionFileNew;
        UIMenuItem* actionFileOpen;
        UIMenuItem* actionFileSave;
        
        MyMainMenuBar();
    };
    
    struct MyCreateLibraryDialog :public UIDialog {
        UIFieldInput* fieldName;
        UIFieldDirectory* fieldHome;
        UIButton* button;
        
        MyCreateLibraryDialog();
    };
    
    struct MyCreateSchemaDialog : public UIDialog {
        UIFieldInput* fieldName;
        UIFieldInput* fieldType;
        UIButton* button;
        
        MyCreateSchemaDialog();
    };
    
    struct MySchemaListView : public UIView {
        MySchemaListView();
        void reloadSchemaList();
    };
    
    struct MySchemaPropertiesView : public UIView {
        MySchemaPropertiesView();
        void reloadSchemaProperties();
    };
    
    struct MySchemaBrowserWindow :public UIWindow {
        UIButton* newSchema;
        MySchemaListView* schemaList;
        MySchemaPropertiesView* schemaProperties;
        
        MySchemaBrowserWindow();
    };
    
    
    
    struct MyObjectBrowserWindow :public UIWindow {
        MyObjectBrowserWindow();
    };
    
    struct MyToastDialog :public UIDialog {
        UIText* text;
        
        MyToastDialog();
        void open(const String& content);
    };
    
    struct MyMainWindow :public UIMainWindow {
        MyMainMenuBar* mainMenuBar;
        MySchemaBrowserWindow* schemaBrowser;
        MyObjectBrowserWindow* objectBrowser;
        MyCreateLibraryDialog* createLibrary;
        MyCreateSchemaDialog* createSchema;
        MyToastDialog* toast;
        
        MyMainWindow();
    };
}
