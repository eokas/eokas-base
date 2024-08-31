#include "./library.h"
#include "./schema.h"
#include "./value.h"

namespace eokas::datapot {
    Library::Library(const String& name)
        : mName(name)
        , mSchemas()
        , mValues(mSchemas)
        , mRoot() { }
    
    Library::~Library() {
        mValues.clear();
        mRoot.clear();
    }
    
    Schema* Library::addSchema(SchemaType type, const String& name) {
        return mSchemas.add(type, name);
    }
    
    Schema* Library::getSchama(const String& name) {
        return mSchemas.get(name);
    }

    Value* Library::make(Schema* schema) {
        return mValues.make(schema);
    }

    Value* Library::make(i32_t val) {
        return mValues.make(val);
    }
    
    Value* Library::make(i64_t val) {
        return mValues.make(val);
    }
    
    Value* Library::make(f64_t val) {
        return mValues.make(val);
    }
    
    Value* Library::make(bool val) {
        return mValues.make(val);
    }
    
    Value* Library::make(const String& val) {
        return mValues.make(val);
    }

    bool Library::get(Value* ptr, i32_t& val) {
        return mValues.get(ptr, val);
    }

    bool Library::get(Value* ptr, i64_t& val) {
        return mValues.get(ptr, val);
    }

    bool Library::get(Value* ptr, f64_t& val) {
        return mValues.get(ptr, val);
    }

    bool Library::get(Value* ptr, bool& val) {
        return mValues.get(ptr, val);
    }

    bool Library::get(Value* ptr, String& val) {
        return mValues.get(ptr, val);
    }

    bool Library::set(Value* ptr, i32_t val) {
        return mValues.set(ptr, val);
    }

    bool Library::set(Value* ptr, i64_t val) {
        return mValues.set(ptr, val);
    }

    bool Library::set(Value* ptr, f64_t val) {
        return mValues.set(ptr, val);
    }

    bool Library::set(Value* ptr, bool val) {
        return mValues.set(ptr, val);
    }

    bool Library::set(Value* ptr, const String& val) {
        return mValues.set(ptr, val);
    }
    
    Value* Library::get(const String& name) {
        auto iter = mRoot.find(name);
        if(iter == mRoot.end())
            return nullptr;
        return iter->second;
    }

    bool Library::get(const String& name, i32_t& val) {
        Value* ptr = this->get(name);
        return mValues.get(ptr, val);
    }

    bool Library::get(const String& name, i64_t& val) {
        Value* ptr = this->get(name);
        return mValues.get(ptr, val);
    }

    bool Library::get(const String& name, f64_t& val) {
        Value* ptr = this->get(name);
        return mValues.get(ptr, val);
    }

    bool Library::get(const String& name, bool& val) {
        Value* ptr = this->get(name);
        return mValues.get(ptr, val);
    }

    bool Library::get(const String& name, String& val) {
        Value* ptr = this->get(name);
        return mValues.get(ptr, val);
    }

    void Library::set(const String& name, Value* val) {
        mRoot[name] = val;
    }

    void Library::set(const String& name, i32_t val) {
        Value* value = mValues.make(val);
        mRoot[name] = value;
    }

    void Library::set(const String& name, i64_t val) {
        Value* value = mValues.make(val);
        mRoot[name] = value;
    }

    void Library::set(const String& name, f64_t val) {
        Value* value = mValues.make(val);
        mRoot[name] = value;
    }

    void Library::set(const String& name, bool val) {
        Value* value = mValues.make(val);
        mRoot[name] = value;
    }

    void Library::set(const String& name, const String& val) {
        Value* value = mValues.make(val);
        mRoot[name] = value;
    }
    
    Value* Library::get(Value* list, size_t index) {
        return mValues.get(list, index);
    }

    bool Library::get(Value* list, size_t index, i32_t& val) {
        Value* ptr = mValues.get(list, index);
        return mValues.get(ptr, val);
    }

    bool Library::get(Value* list, size_t index, i64_t& val) {
        Value* ptr = mValues.get(list, index);
        return mValues.get(ptr, val);
    }

    bool Library::get(Value* list, size_t index, f64_t& val) {
        Value* ptr = mValues.get(list, index);
        return mValues.get(ptr, val);
    }

    bool Library::get(Value* list, size_t index, bool& val) {
        Value* ptr = mValues.get(list, index);
        return mValues.get(ptr, val);
    }

    bool Library::get(Value* list, size_t index, String& val) {
        Value* ptr = mValues.get(list, index);
        return mValues.get(ptr, val);
    }

    bool Library::set(Value* list, size_t index, Value* val) {
        return mValues.set(list, index, val);
    }

    bool Library::set(Value* list, size_t index, i32_t val) {
        Value* value = mValues.make(val);
        return mValues.set(list, index, value);
    }

    bool Library::set(Value* list, size_t index, i64_t val) {
        Value* value = mValues.make(val);
        return mValues.set(list, index, value);
    }

    bool Library::set(Value* list, size_t index, f64_t val) {
        Value* value = mValues.make(val);
        return mValues.set(list, index, value);
    }

    bool Library::set(Value* list, size_t index, bool val) {
        Value* value = mValues.make(val);
        return mValues.set(list, index, value);
    }

    bool Library::set(Value* list, size_t index, const String& val) {
        Value* value = mValues.make(val);
        return mValues.set(list, index, value);
    }

    bool Library::push(Value* list, Value* val) {
        return mValues.push(list, val);
    }

    bool Library::push(Value* list, i32_t val) {
        Value* value = mValues.make(val);
        return mValues.push(list, value);
    }
    bool Library::push(Value* list, i64_t val) {
        Value* value = mValues.make(val);
        return mValues.push(list, value);
    }

    bool Library::push(Value* list, f64_t val) {
        Value* value = mValues.make(val);
        return mValues.push(list, value);
    }

    bool Library::push(Value* list, bool val) {
        Value* value = mValues.make(val);
        return mValues.push(list, value);
    }

    bool Library::push(Value* list, const String& val) {
        Value* value = mValues.make(val);
        return mValues.push(list, value);
    }

    bool Library::pop(Value* list, Value* val) {
        return mValues.pop(list, val);
    }

    bool Library::pop(Value* list, i32_t& val) {
        Value ptr{};
        if(!mValues.pop(list, &ptr))
            return false;
        return mValues.get(&ptr, val);
    }

    bool Library::pop(Value* list, i64_t& val) {
        Value ptr{};
        if(!mValues.pop(list, &ptr))
            return false;
        return mValues.get(&ptr, val);
    }

    bool Library::pop(Value* list, f64_t& val) {
        Value ptr{};
        if(!mValues.pop(list, &ptr))
            return false;
        return mValues.get(&ptr, val);
    }

    bool Library::pop(Value* list, bool& val) {
        Value ptr{};
        if(!mValues.pop(list, &ptr))
            return false;
        return mValues.get(&ptr, val);
    }

    bool Library::pop(Value* list, String& val) {
        Value ptr{};
        if(!mValues.pop(list, &ptr))
            return false;
        return mValues.get(&ptr, val);
    }
    
    Value* Library::get(Value* object, const String& field) {
        return mValues.get(object, field);
    }

    bool Library::get(Value* object, const String& field, i32_t& val) {
        Value* ptr = mValues.get(object, field);
        return mValues.get(ptr, val);
    }

    bool Library::get(Value* object, const String& field, i64_t& val) {
        Value* ptr = mValues.get(object, field);
        return mValues.get(ptr, val);
    }

    bool Library::get(Value* object, const String& field, f64_t& val) {
        Value* ptr = mValues.get(object, field);
        return mValues.get(ptr, val);
    }

    bool Library::get(Value* object, const String& field, bool& val) {
        Value* ptr = mValues.get(object, field);
        return mValues.get(ptr, val);
    }

    bool Library::get(Value* object, const String& field, String& val) {
        Value* ptr = mValues.get(object, field);
        return mValues.get(ptr, val);
    }
    
    bool Library::set(Value* object, const String& field, Value* val) {
        return mValues.set(object, field, val);
    }

    bool Library::set(Value* object, const String& field, i32_t val) {
        Value* value = mValues.make(val);
        return mValues.set(object, field, value);
    }

    bool Library::set(Value* object, const String& field, i64_t val) {
        Value* value = mValues.make(val);
        return mValues.set(object, field, value);
    }

    bool Library::set(Value* object, const String& field, f64_t val) {
        Value* value = mValues.make(val);
        return mValues.set(object, field, value);
    }

    bool Library::set(Value* object, const String& field, bool val) {
        Value* value = mValues.make(val);
        return mValues.set(object, field, value);
    }

    bool Library::set(Value* object, const String& field, const String& val) {
        Value* value = mValues.make(val);
        return mValues.set(object, field, value);
    }
}

