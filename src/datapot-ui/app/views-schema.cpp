#include "./views-schema.h"
#include "./logic.h"
#include "datapot/library.h"

namespace eokas::datapot {
    MySchemaCreatorDialog::MySchemaCreatorDialog()
        : UIDialog(Vector2(400, 200), true) {
        this->name = "Create Schema";
        
        std::vector<String> schemaTypeNames = {"None", "Int", "Float", "Bool", "String", "List", "Struct"};
        
        properties = this->add<UIPropertiesView>();
        schemaName = properties->addInput("Schema Name", "", false);
        schemaType = properties->addEnum("Schema Type", schemaTypeNames, 0);
        button = this->add<UIButton>("Create");
        button->onClick = [this](){
            String name = schemaName->value.string().trim();
            auto type = (SchemaType)(int)schemaType->value;
            if(!name.isEmpty() && type != SchemaType::None) {
                Result result = Logic::instance().createSchema(type, name);
                if(result.ok) {
                    this->hide();
                }
            }
        };
    }
    
    MySchemaSelectorDialog::MySchemaSelectorDialog()
        : UIDialog(Vector2(400, 200), true) {
    }
    
    void MySchemaSelectorDialog::open() {
        this->clear();
        
        auto library = Logic::instance().library;
        if(library != nullptr) {
            this->add<UIText>("Schemas");
            this->add<UISeparator>();
            
            for (u32_t index = 0; index < library->getSchemaCount(); index++) {
                Schema* schema = library->getSchema(index);
                auto button = this->add<UIButton>(schema->name());
                button->onClick = [schema](){
                    Logic::instance().selectSchema(schema);
                };
            }
        }
        
        this->show();
    }
    
    MySchemaListView::MySchemaListView()
        : UIView(Vector2(300, 0), Flags_Borders | Flags_ResizeX) {
        this->reloadSchemaList();
    }
    
    void MySchemaListView::reloadSchemaList() {
        this->clear();
        
        auto library = Logic::instance().library;
        if(library != nullptr) {
            this->add<UIText>("Schemas");
            this->add<UISeparator>();
            
            for (u32_t index = 0; index < library->getSchemaCount(); index++) {
                Schema* schema = library->getSchema(index);
                auto button = this->add<UIButton>(schema->name());
                button->onClick = [schema](){
                    Logic::instance().selectSchema(schema);
                };
            }
        }
    }
    
    MySchemaPropertiesView::MySchemaPropertiesView()
        : UIView(Vector2(0, 500), Flags_Borders | Flags_ResizeY) {
        properties = nullptr;
        members = nullptr;
    }
    
    void MySchemaPropertiesView::reloadSchemaProperties() {
        this->clear();
        
        Schema* schema = Logic::instance().schema;
        if(schema != nullptr) {
            SchemaType schemaType = schema->type();
            std::vector<String> schemaTypeNames = {"None", "Int", "Float", "Bool", "String", "List", "Struct"};
            
            this->add<UIText>("Properties");
            this->add<UISeparator>();
            
            properties = this->add<UIPropertiesView>();
            properties->addString("Name", schema->name());
            properties->addEnum("Type", schemaTypeNames, (int)schema->type());
            
            String indent("    ");
            if(schemaType == SchemaType::List) {
                auto elementSchema = schema->getElement();
                properties->addString(indent + "Element Schema", elementSchema->name());
            }
            if(schemaType == SchemaType::Struct) {
                this->add<UIText>("Struct Members");
                members = this->add<UIPropertiesView>();
                members->insertable = true;
                members->removable = true;
                members->onInsert = [this](){
                    auto newprop = members->addProperty<UISelector>("");
                    newprop->labelEditable = true;
                    newprop->valueEditable = true;
                    dynamic_cast<UISelector*>(newprop->widget)->onSelect = [](StringValue& value){
                    
                    };
                };
                for(u32_t index = 0; index < schema->getMemberCount(); index++) {
                    auto member = schema->getMember(index);
                    members->addString(indent + member->name, member->schema->name());
                }
            }
        }
    }
    
    MySchemaBrowserWindow::MySchemaBrowserWindow()
        : UIWindow(true) {
        name = "Schemas";
        
        newSchema = this->add<UIButton>("New Schema");
        newSchema->onClick = [](){
            if(Logic::instance().actionNewSchema) Logic::instance().actionNewSchema();
        };
        
        auto horizontal = this->add<UILayout>(UILayout::Horizontal);
        schemaList = horizontal->add<MySchemaListView>();
        schemaProperties = horizontal->add<MySchemaPropertiesView>();
    }

}