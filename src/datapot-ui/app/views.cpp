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
        properties = this->add<UIPropertiesView>("Library.Create.Info");
        auto libraryName = properties->addString("Library Name", "");
        auto libraryHome = properties->addDirectory("Library Home");
        button = this->add<UIButton>("Create");
        button->onClick = [=, this]() {
            String name = libraryName->value.string().trim();
            String home = libraryHome->value.string().trim();
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
        
        properties = this->add<UIPropertiesView>("Schema.Create.Info");
        auto schemaName = properties->addInput("Schema Name", "", false);
        auto schemaType = properties->addEnum("Schema Type", schemaTypeNames, 0);
        button = this->add<UIButton>("Create");
        button->onClick = [&, this](){
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
        properties = nullptr;
    }
    
    void MySchemaPropertiesView::reloadSchemaProperties() {
        this->clear();
        
        Schema* schema = Logic::instance().schema;
        if(schema != nullptr) {
            SchemaType schemaType = schema->type();
            std::vector<String> schemaTypeNames = {"None", "Int", "Float", "Bool", "String", "List", "Struct"};

            this->add<UIText>("Properties");
            this->add<UISeparator>();

            properties = this->add<UIPropertiesView>("Schema.Properties");
            properties->addString("Name", schema->name());
            properties->addEnum("Type", schemaTypeNames, (int)schema->type());

            String indent("    ");
            if(schemaType == SchemaType::List) {
                auto elementSchema = schema->getElement();
                properties->addString(indent+"Element Schema", elementSchema->name());
            }
            if(schemaType == SchemaType::Struct) {
                this->add<UIText>("Members");
                for(u32_t index = 0; index < schema->getMemberCount(); index++) {
                    auto member = schema->getMember(index);
                    properties->addString(indent+member->name, member->schema->name());
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
        this->text->value = content;
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
