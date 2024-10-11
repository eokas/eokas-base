#pragma once

#include "imwidgets/widget.h"

namespace eokas::datapot {
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
}
