#include "./logic.h"
#include "utils//utils.h"

namespace eokas::datapot {
    String Logic::stringifySchemaType(SchemaType type) {
        if(type == SchemaType::Int) return "Int";;
        if(type == SchemaType::Float) return "Float";
        if(type == SchemaType::Bool) return "Bool";
        if(type == SchemaType::String) return "String";
        if(type == SchemaType::List) return "List";
        if(type == SchemaType::String) return "Struct";
        return "";
    }
    
    SchemaType Logic::parseSchemaType(const String& str) {
        if(str == "Int") return SchemaType::Int;
        if(str == "Float") return SchemaType::Float;
        else if(str == "Bool") return SchemaType::Bool;
        else if(str == "String") return SchemaType::String;
        else if(str == "List") return SchemaType::List;
        else if(str == "Struct") return SchemaType::Struct;
        return SchemaType::None;
    }
    
    Logic::Logic() {}
    
    Logic::~Logic() {
        _DeletePointer(this->library);
    }
    
    Result Logic::result(bool ok, const eokas::String& msg) {
        Result r{ ok, msg };
        if(!ok && this->onError) {
            this->onError(r);
        }
        return r;
    }
    
    Result Logic::createLibrary(const eokas::String& name, const eokas::String& home) {
        if(this->library != nullptr) {
            _DeletePointer(this->library);
        }
        
        String fileName = String::format("%s.datapot", name.cstr());
        String filePath = File::combinePath(home, fileName);
        if(File::exists(filePath)) {
            String msg = String::format("The file: %s is existed already.", filePath.cstr());
            return result(false, msg);
        }
        
        this->library = new Library(filePath);
        this->library->save(filePath);
        if(this->onCreateLibrary) this->onCreateLibrary();
        
        return result(true);
    }
    
    Result Logic::openLibrary() {
        String filePath;
        std::map<String, String> fileFiltr;
        fileFiltr["DataPot Library File"] = "*.datapot";
        if(!eokas::OpenFileDialog(filePath, fileFiltr)) {
            return result(false, "Not selected a datapot library file.");
        }
        
        if(this->library != nullptr) {
            _DeletePointer(this->library);
        }
        
        this->library = new Library(filePath);
        if(!this->library->load(filePath)) {
            return result(false, "The library load failed.");
        }
        
        if(this->onOpenLibrary) this->onOpenLibrary();
        
        return result(true);
    }
    
    Result Logic::selectSchema(Schema* newSchema) {
        if(this->schema == newSchema)
            return result(true);
        
        this->schema = newSchema;
        
        if(this->onSelectSchema) this->onSelectSchema();
        
        return result(true);
    }
    
    Result Logic::createSchema(const String& type, const String& name) {
        if(this->library == nullptr) {
            return result(false, "Current library is null");
        }
        
        SchemaType t = Logic::parseSchemaType(type);
        if(t == SchemaType::None) {
            return result(false, "Unknown schema type.");
        }
        
        auto newSchema = this->library->addSchema(t, name);
        if(newSchema == nullptr) {
            return result(false, "Create schema failed.");
        }
        
        if(this->onCreateSchema) this->onCreateSchema();
        
        return this->selectSchema(newSchema);
    }
}
