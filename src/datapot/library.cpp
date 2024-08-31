#include "./library.h"
#include "./schema.h"
#include "./value.h"

namespace eokas::datapot {
    Library::Library(const String& name)
        : mName(name)
        , mSchemaTable()
        , mValueTable()
        , mSchemaHeap()
        , mValueHeap()
        , mMemory()
    {
        this->addSchema(new IntSchema());
        this->addSchema(new FloatSchema());
        this->addSchema(new BoolSchema());
        this->addSchema(new StringSchema());
    }
    
    Library::~Library() {
        mValueTable.clear();
        mSchemaTable.clear();
        
        _DeleteList(mValueHeap);
        _DeleteList(mSchemaHeap);
    }
    
    bool Library::addSchema(Schema* schema) {
        if(this->getSchama(schema->name()) != nullptr) {
            return false;
        }
        mSchemaHeap.push_back(schema);
        mSchemaTable.insert(std::make_pair(schema->name(), schema));
    }
    
    Schema* Library::getSchama(const String& name) {
        auto iter = mSchemaTable.find(name);
        if(iter == mSchemaTable.end()) {
            return nullptr;
        }
        return iter->second;
    }
    
    Value* Library::make(Schema* schema) {
        if(schema == nullptr)
            return nullptr;
        
        if(schema->type() == SchemaType::Int) {
            Value value;
            value.schema = schema;
            value.value.i64 = 0;
            mMemory.values.push_back(value);
            return &mMemory.values.back();
        }
        else if(schema->type() == SchemaType::Float) {
            Value value;
            value.schema = schema;
            value.value.f64 = 0;
            mMemory.values.push_back(value);
            return &mMemory.values.back();
        }
        else if(schema->type() == SchemaType::Bool) {
            Value value;
            value.schema = schema;
            value.value.i64 = 1;
            mMemory.values.push_back(value);
            return &mMemory.values.back();
        }
        else if(schema->type() == SchemaType::String) {
            i64_t index = mMemory.strings.size();
            Value value;
            value.schema = schema;
            value.value.i64 = index;
            mMemory.strings.push_back("");
            mMemory.values.push_back(value);
            return &mMemory.values.back();
        }
        else if(schema->type() == SchemaType::List)
        {
            i64_t index = mMemory.lists.size();
            Value value;
            value.schema = schema;
            value.value.i64 = index;
            mMemory.lists.push_back(List{});
            mMemory.values.push_back(value);
            return &mMemory.values.back();
        }
        else if(schema->type() == SchemaType::Struct)
        {
            StructSchema* structSchema = dynamic_cast<StructSchema*>(schema);
            
            i64_t index = mMemory.objects.size();
            Value value;
            value.schema = schema;
            value.value.i64 = index;
            mMemory.objects.push_back(Object{});
            mMemory.values.push_back(value);
            
            Object& object = mMemory.objects.emplace_back();
            for(size_t i = 0; i < structSchema->getMemberCount(); i++) {
                auto* member = structSchema->getMember(i);
                Value* v = this->make(member->schema);
                
            }
            
            return &mMemory.values.back();
        }
        
        return nullptr;
    }
    
    Value* Library::make(i64_t val) {
        Schema* schema = this->getSchama("Int");
        
        Value value;
        value.schema = schema;
        value.value.i64 = val;
        mMemory.values.push_back(value);
        return &mMemory.values.back();
    }
    
    Value* Library::make(f64_t val) {
        Schema* schema = this->getSchama("Float");
        
        Value value;
        value.schema = schema;
        value.value.f64 = val;
        mMemory.values.push_back(value);
        return &mMemory.values.back();
    }
    
    Value* Library::make(bool val) {
        Schema* schema = this->getSchama("Bool");
        
        Value value;
        value.schema = schema;
        value.value.i64 = val ? 1 : 0;
        mMemory.values.push_back(value);
        return &mMemory.values.back();
    }
    
    Value* Library::make(const String& val) {
        Schema* schema = this->getSchama("String");
        
        i64_t index = mMemory.strings.size();
        Value value;
        value.schema = schema;
        value.value.i64 = index;
        mMemory.strings.push_back(val);
        mMemory.values.push_back(value);
        return &mMemory.values.back();
    }
    
    Value* Library::get(const String& name) {
        auto iter = mValueTable.find(name);
        if(iter == mValueTable.end())
            return nullptr;
        return iter->second;
    }
    
    Value* Library::get(Value* list, u64_t index) {
        if(!list->schema->is(SchemaType::List))
            return nullptr;
        if(list->value.i64 < 0 || list->value.i64 >= mMemory.lists.size())
            return nullptr;
        List& ref = mMemory.lists.at(list->value.i64);
        if(index < 0 || index >= ref.elements.size())
            return nullptr;
        return &ref.elements.at(index);
    }
    
    Value* Library::get(Value* object, const String& field) {
        if(!object->schema->is(SchemaType::Struct))
            return nullptr;
        if(object->value.i64 < 0 || object->value.i64 >= mMemory.objects.size())
            return nullptr;
        Object& ref = mMemory.objects.at(object->value.i64);
        auto iter = ref.members.find(field);
        if(iter == ref.members.end())
            return nullptr;
        return &iter->second;
    }
    
    void Library::set(const String& name, Value* val) {
        mValueTable[name] = val;
    }
    
    void Library::set(Value* list, u64_t index, Value* val) {
        if(!list->schema->is(SchemaType::List))
            return;
        if(list->value.i64 < 0 || list->value.i64 >= mMemory.lists.size())
            return;
        List& ref = mMemory.lists.at(list->value.i64);
        if(index < 0 || index >= ref.elements.size())
            return;
        ref.elements[index] = *val;
    }
    
    void Library::set(Value* object, const String& field, Value* val) {
        if(!object->schema->is(SchemaType::Struct))
            return;
        if(object->value.i64 < 0 || object->value.i64 >= mMemory.objects.size())
            return;
        Object& ref = mMemory.objects.at(object->value.i64);
        ref.members[field] = *val;
    }
    
    bool Library::get(Value* ptr, i64_t* val) {
        if(!ptr->schema->is(SchemaType::Int))
            return false;
        *val = ptr->value.i64;
        return true;
    }
    
    bool Library::get(Value* ptr, f64_t* val) {
        if(!ptr->schema->is(SchemaType::Float))
            return false;
        *val = ptr->value.f64;
        return true;
    }
    
    bool Library::get(Value* ptr, bool* val) {
        if(!ptr->schema->is(SchemaType::Bool))
            return false;
        *val = ptr->value.i64 != 0;
        return true;
    }
    
    bool Library::get(Value* ptr, String& val) {
        if(!ptr->schema->is(SchemaType::String))
            return false;
        i64_t index = ptr->value.i64;
        if(index < 0 || index > mMemory.strings.size())
            return false;
        val = mMemory.strings.at(index);
        return true;
    }
    
    bool Library::set(Value* ptr, i32_t val) {
        if(!ptr->schema->is(SchemaType::Int))
            return false;
        ptr->value.i64 = val;
        return true;
    }
    
    bool Library::set(Value* ptr, i64_t val) {
        if(!ptr->schema->is(SchemaType::Int))
            return false;
        ptr->value.i64 = val;
        return true;
    }
    
    bool Library::set(Value* ptr, f64_t val) {
        if(!ptr->schema->is(SchemaType::Float))
            return false;
        ptr->value.f64 = val;
        return true;
    }
    
    bool Library::set(Value* ptr, bool val) {
        if(!ptr->schema->is(SchemaType::Bool))
            return false;
        ptr->value.i64 = val ? 1 : 0;
        return true;
    }
    
    bool Library::set(Value* ptr, const String& val) {
        if(!ptr->schema->is(SchemaType::String))
            return false;
        
        i64_t index = mMemory.strings.size();
        for(size_t i = 0; i < mMemory.strings.size(); i++) {
            if(val == mMemory.strings.at(i)) {
                index = i;
                break;
            }
        }
        if(index == mMemory.strings.size()) {
            mMemory.strings.push_back(val);
        }
        
        ptr->value.i64 = index;
        
        return true;
    }
}

