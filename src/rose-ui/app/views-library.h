#pragma once

#include "imwidgets/widget.h"

namespace eokas::datapot {
    struct MyLibraryCreatorDialog :public UIDialog {
        UIPropertiesView* properties;
        UIButton* button;
        
        MyLibraryCreatorDialog();
    };
}