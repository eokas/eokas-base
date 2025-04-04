#pragma once

#include "imwidgets/widget.h"

namespace eokas::datapot {
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
}
