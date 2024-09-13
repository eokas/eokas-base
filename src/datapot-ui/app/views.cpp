#include "./views.h"
#include "./logic.h"
#include "datapot/library.h"

namespace eokas::datapot {
    MyMainMenuBar::MyMainMenuBar()
    {
        auto file = this->add<UIMenu>("File");
        actionFileNew = file->add<UIMenuItem>("New Library ...");
        actionFileNew->onClick = []() {
            if(Logic::instance().actionFileNew) Logic::instance().actionFileNew();
        };
        actionFileOpen = file->add<UIMenuItem>("Open Library ...");
        actionFileOpen->onClick = [](){
            Logic::instance().openLibrary();
        };
        actionFileSave = file->add<UIMenuItem>("Save ...");
        actionFileSave->onClick = [](){
            if(Logic::instance().actionFileSave) Logic::instance().actionFileSave();
        };
        
        auto edit = this->add<UIMenu>("Edit");
        
        auto about = this->add<UIMenu>("About");
    }
    
    MyCreateLibraryDialog::MyCreateLibraryDialog()
        : UIDialog("CreateLibrary", true) {
        fieldName = this->add<UIFieldInput>("CreateLibrary.LibraryInfo", "Library Name");
        fieldHome = this->add<UIFieldDirectory>("CreateLibrary.LibraryInfo", "Library Home");
        button = this->add<UIButton>("Create");
        button->onClick = [this]() {
            String name = fieldName->value;
            String home = fieldHome->value;
            if (!name.isEmpty() && !home.isEmpty()) {
                Result result = Logic::instance().createLibrary(name, home);
                if(result.ok) {
                    this->hide();
                }
            }
        };
    }
    
    MyCreateSchemaDialog::MyCreateSchemaDialog()
        : UIDialog("CreateSchema", true) {
        std::vector<String> schemaTypeNames = {"None", "Int", "Float", "Bool", "String", "List", "Struct"};
        
        auto properties = this->add<UIPropertiesView>("Schema.Create.Info");
        properties->addInput("Schema Name", false, "");
        properties->addCombo("Schema Type", {"Int", "Float", "Bool", "String", "List", "Struct"}, 0);
        
        fieldName = this->add<UIFieldInput>("CreateSchema.SchemaInfo", "Schema Name");
        fieldType = this->add<UIFieldCombo>("CreateSchema.SchemaInfo", "Schema Type");
        button = this->add<UIButton>("Create");
        button->onClick = [this](){
            String name = fieldName->value;
            String type = fieldType->value;
            if(!name.isEmpty() && !type.isEmpty()) {
                Result result = Logic::instance().createSchema(type, name);
                if(result.ok) {
                    this->hide();
                }
            }
        };
    }
    
    MySchemaListView::MySchemaListView()
        : UIView("SchemaList", Vector2(300, 0), Flags_Borders | Flags_ResizeX) {
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
        : UIView("SchemaProperties", Vector2(0, 500), Flags_Borders | Flags_ResizeY) {
        
    }
    
    void MySchemaPropertiesView::reloadSchemaProperties() {
        this->clear();
        
        Schema* schema = Logic::instance().schema;
        if(schema != nullptr) {
            this->add<UIText>("Properties");
            this->add<UISeparator>();
            
            SchemaType schemaType = schema->type();
            this->add<UIFieldText>("SchemaProperties", "Name", schema->name());
            this->add<UIFieldText>("SchemaProperties", "Type", Logic::stringifySchemaType(schemaType));
            if(schemaType == SchemaType::List) {
                auto elementSchema = schema->getElement();
                this->add<UIFieldText>("SchemaProperties", "    Element Schema", elementSchema->name());
            }
            if(schemaType == SchemaType::Struct) {
                this->add<UIText>("Members");
                for(u32_t index = 0; index < schema->getMemberCount(); index++) {
                    auto member = schema->getMember(index);
                    this->add<UIFieldText>("SchemaProperties", String("    ")+member->name, member->schema->name());
                }
            }
        }
    }
    
    MySchemaBrowserWindow::MySchemaBrowserWindow()
        : UIWindow("Schemas", true) {
        newSchema = this->add<UIButton>("New Schema");
        newSchema->onClick = [](){
            if(Logic::instance().actionNewSchema) Logic::instance().actionNewSchema();
        };
        
        auto horizontal = this->add<UILayout>(UILayout::Horizontal);
        schemaList = horizontal->add<MySchemaListView>();
        schemaProperties = horizontal->add<MySchemaPropertiesView>();
    }
    
    MyObjectBrowserWindow::MyObjectBrowserWindow()
        : UIWindow("Objects", true){
        
    }
    
    MyToastDialog::MyToastDialog()
        : UIDialog("", false) {
        text = this->add<UIText>("");
    }
    
    void MyToastDialog::open(const eokas::String& content) {
        this->text->content = content;
        this->show();
    }
    
    MyMainWindow::MyMainWindow()
        : UIMainWindow("datapot", true)
    {
        mainMenuBar = this->add<MyMainMenuBar>();
        schemaBrowser = this->add<MySchemaBrowserWindow>();
        objectBrowser = this->add<MyObjectBrowserWindow>();
        createLibrary = this->add<MyCreateLibraryDialog>();
        createSchema = this->add<MyCreateSchemaDialog>();
        toast = this->add<MyToastDialog>();
        
        Logic::instance().onError = [this](const Result& error) {
            toast->open(error.message);
        };
        Logic::instance().actionFileNew = [this]() {
            createLibrary->show();
        };
        Logic::instance().onCreateLibrary = [this]() {
            schemaBrowser->schemaList->reloadSchemaList();
        };
        Logic::instance().onOpenLibrary = [this]() {
            schemaBrowser->schemaList->reloadSchemaList();
        };
        Logic::instance().actionNewSchema = [this]() {
            createSchema->show();
        };
        Logic::instance().onCreateSchema = [this]() {
            schemaBrowser->schemaList->reloadSchemaList();
        };
        Logic::instance().onSelectSchema = [this]() {
            schemaBrowser->schemaProperties->reloadSchemaProperties();
        };
    }
}
