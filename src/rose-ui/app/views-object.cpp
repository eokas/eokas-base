#include "./views-object.h"
#include "./logic.h"
#include "datapot/library.h"

namespace eokas::datapot {
    MyObjectCreatorDialog::MyObjectCreatorDialog()
        : UIDialog(Vector2(600, 300), true) {
        this->name = "Create Object";
        
        properties = this->add<UIPropertiesView>();
        auto objectName = properties->addInput("Name", "");
        auto schemaSelector = properties->addProperty<UISelector>("Schema");
        dynamic_cast<UISelector*>(schemaSelector->widget)->onSelect = [](StringValue& value) {
        
        };
    }
    
    MyObjectListView::MyObjectListView()
        : UIView(Vector2(300, 0), Flags_Borders) {
        this->reloadObjects();
    }
    
    void MyObjectListView::reloadObjects() {
        this->clear();
        
        auto library = Logic::instance().library;
        if(library != nullptr) {
            std::vector<String> rootNameList;
            library->getRootNames(rootNameList);
            for(auto& rootName : rootNameList) {
                this->add<UIButton>(rootName);
            }
        }
    }
    
    MyObjectBrowserWindow::MyObjectBrowserWindow()
        : UIWindow(true){
        name = "Objects";
        
        newObject = this->add<UIButton>("Create Object");
        newObject->onClick = []() {
            if(Logic::instance().actionNewObject) Logic::instance().actionNewObject();
        };
        
        objectList = this->add<MyObjectListView>();
    }
}
