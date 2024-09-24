#pragma once

#include "imwidgets/widget.h"

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
    
    struct MyLibraryCreatorDialog :public UIDialog {
        UIPropertiesView* properties;
        UIButton* button;
        
        MyLibraryCreatorDialog();
    };
    
    struct MySchemaCreatorDialog : public UIDialog {
        UIPropertiesView* properties;
        UIPropertiesView::Property* schemaName;
        UIPropertiesView::Property* schemaType;
        UIButton* button;
        
        MySchemaCreatorDialog();
    };
    
    struct MySchemaSelectorDialog : public UIDialog {
        MySchemaSelectorDialog();
        void open();
    };
    
    struct MySchemaListView : public UIView {
        MySchemaListView();
        void reloadSchemaList();
    };
    
    struct MyMemberCreatorDialog : public UIDialog {
        MyMemberCreatorDialog(UIPropertiesView* properties);
    };
    
    struct MySchemaPropertiesView : public UIView {
        UIPropertiesView* properties;
        UIPropertiesView* members;

        MySchemaPropertiesView();
        void reloadSchemaProperties();
    };
    
    struct MySchemaBrowserWindow :public UIWindow {
        UIButton* newSchema;
        MySchemaListView* schemaList;
        MySchemaPropertiesView* schemaProperties;
        
        MySchemaBrowserWindow();
    };

    struct MyObjectCreatorDialog : public UIDialog {
        UIPropertiesView* properties;

        MyObjectCreatorDialog();
    };

    struct MyObjectListView : public UIView {
        MyObjectListView();
        void reloadObjects();
    };

    struct MyObjectBrowserWindow :public UIWindow {
        UIButton* newObject;
        MyObjectListView* objectList;

        MyObjectBrowserWindow();
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
