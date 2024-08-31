#include "./schema.h"

namespace eokas::datapot {
    Schema::Schema(SchemaType type, const String& name)
        : mType(type)
        , mName(name) { }
    
    Schema::~Schema() { }
    
    IntSchema::IntSchema()
        : Schema(SchemaType::Int, "Int") { }
    
    FloatSchema::FloatSchema()
        : Schema(SchemaType::Float, "Float") { }
    
    BoolSchema::BoolSchema()
        : Schema(SchemaType::Bool, "Bool") { }
    
    StringSchema::StringSchema()
        : Schema(SchemaType::String, "String") { }
    
    ListSchema::ListSchema(const String& name)
        : Schema(SchemaType::List, name) { }
    
    StructSchema::StructSchema(const String& name)
        : Schema(SchemaType::Struct, name)
        , mMembers() { }
    
    StructSchema::~StructSchema() {
        mMembers.clear();
    }
    
    void StructSchema::addMember(const String& name, Schema* schema) {
        Member& mem = mMembers.emplace_back();
        mem.name = name;
        mem.schema = schema;
    }
    
    void StructSchema::setMember(const String& name, Schema* schema) {
        Member* exists = this->getMember(name);
        if(exists != nullptr) {
            exists->schema = schema;
        }
        else {
            this->addMember(name, schema);
        }
    }
    
    void StructSchema::delMember(const String& name) {
        auto iter = mMembers.begin();
        while(iter != mMembers.end()) {
            if(iter->name == name) {
                iter = mMembers.erase(iter);
                continue;
            }
            ++ iter;
        }
    }
    
    StructSchema::Member* StructSchema::getMember(const String& name) {
        for(auto& mem : mMembers) {
            if(mem.name == name)
                return &mem;
        }
        return nullptr;
    }
    
    StructSchema::Member* StructSchema::getMember(int index) {
        return &mMembers.at(index);
    }
    
    size_t StructSchema::getMemberCount() {
        return mMembers.size();
    }
    
    size_t StructSchema::getMemberIndex(const String& name) {
        for(size_t i = 0; i < mMembers.size(); i++) {
            if(mMembers[i].name == name) {
                return i;
            }
        }
        return -1;
    }
}
