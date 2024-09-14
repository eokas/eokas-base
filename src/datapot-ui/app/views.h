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
        UIPropertiesView* properties;
        UIButton* button;
        
        MyCreateLibraryDialog();
    };
    
    struct MyCreateSchemaDialog : public UIDialog {
        UIPropertiesView* properties;
        UIPropertiesView::Property* schemaName;
        UIPropertiesView::Property* schemaType;
        UIButton* button;
        
        MyCreateSchemaDialog();
    };
    
    struct MySchemaListView : public UIView {
        MySchemaListView();
        void reloadSchemaList();
    };
    
    struct MySchemaPropertiesView : public UIView {
        UIPropertiesView* properties;

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
