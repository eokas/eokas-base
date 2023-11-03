
#include "hom.h"

namespace eokas {
    
    HomArray::HomArray(const HomValueArray& val)
        : HomValue(HomType::Array), value(val) {
    }
    
    HomValueRef& HomArray::operator[](size_t index) {
        return this->value[index];
    }
    
    f64_t HomArray::getNumber(size_t index, f64_t defaultValue) {
        f64_t val = defaultValue;
        this->get(index, val);
        return val;
    }
    
    bool HomArray::getBoolean(size_t index, bool defaultValue) {
        bool val = defaultValue;
        this->get(index, val);
        return val;
    }
    
    String HomArray::getString(size_t index, const String& defaultValue) {
        String val = defaultValue;
        this->get(index, val);
        return val;
    }
    
    HomValueArray HomArray::getArray(size_t index, const HomValueArray& defaultValue) {
        HomValueArray val = defaultValue;
        this->get(index, val);
        return val;
    }
    
    HomValueMap HomArray::getObject(size_t index, const HomValueMap& defaultValue) {
        HomValueMap val = defaultValue;
        this->get(index, val);
        return val;
    }
    
    bool HomArray::get(size_t index, HomValueRef& val) {
        if (index > this->value.size())
            return false;
        val = this->value[index];
        return true;
    }
    
    bool HomArray::get(size_t index, HomType type, HomValueRef& val) {
        if (index > this->value.size())
            return false;
        auto& ref = this->value[index];
        if (ref == nullptr || ref->type != type)
            return false;
        val = ref;
        return true;
    }
    
    bool HomArray::get(size_t index, f64_t& val) {
        HomValueRef ref;
        if (!this->get(index, HomType::Number, ref))
            return false;
        val = HomNumber::pick(ref);
        return true;
    }
    
    bool HomArray::get(size_t index, bool& val) {
        HomValueRef ref;
        if (!this->get(index, HomType::Boolean, ref))
            return false;
        val = HomBoolean::pick(ref);
        return true;
    }
    
    bool HomArray::get(size_t index, String& val) {
        HomValueRef ref;
        if (!this->get(index, HomType::String, ref))
            return false;
        val = HomString::pick(ref);
        return true;
    }
    
    bool HomArray::get(size_t index, HomValueArray& val) {
        HomValueRef ref;
        if (!this->get(index, HomType::Array, ref))
            return false;
        val = HomArray::pick(ref);
        return true;
    }
    
    bool HomArray::get(size_t index, HomValueMap& val) {
        HomValueRef ref;
        if (!this->get(index, HomType::Object, ref))
            return false;
        val = HomObject::pick(ref);
        return true;
    }
    
    void HomArray::set(size_t index, HomValueRef val) {
        this->value[index] = val;
    }
    
    void HomArray::set(size_t index, f64_t val) {
        this->value[index] = HomNumber::make(val);
    }
    
    void HomArray::set(size_t index, bool val) {
        this->value[index] = HomBoolean::make(val);
    }
    
    void HomArray::set(size_t index, const String& val) {
        this->value[index] = HomString::make(val);
    }
    
    void HomArray::set(size_t index, const HomValueArray& val) {
        this->value[index] = HomArray::make(val);
    }
    
    void HomArray::set(size_t index, const HomValueMap& val) {
        this->value[index] = HomObject::make(val);
    }
    
    HomArray& HomArray::add(HomValueRef val) {
        this->value.push_back(val);
        return *this;
    }
    
    HomArray& HomArray::add(f64_t val) {
        this->value.push_back(HomNumber::make(val));
        return *this;
    }
    
    HomArray& HomArray::add(bool val) {
        this->value.push_back(HomBoolean::make(val));
        return *this;
    }
    
    HomArray& HomArray::add(const String& val) {
        this->value.push_back(HomString::make(val));
        return *this;
    }
    
    HomArray& HomArray::add(const HomValueArray& val) {
        this->value.push_back(HomArray::make(val));
        return *this;
    }
    
    HomArray& HomArray::add(const HomValueMap& val) {
        this->value.push_back(HomObject::make(val));
        return *this;
    }
    
    HomValueRef HomArray::make(const HomValueArray& value) {
        return HomValueRef(new HomArray(value));
    }
    
    HomValueArray HomArray::pick(const HomValueRef& value) {
        auto ptr = (HomArray*) value.get();
        return ptr != nullptr ? ptr->value : HomValueArray();
    }
    
    HomObject::HomObject(const HomValueMap& value)
        : HomValue(HomType::Object), value(std::move(value)) {
    }
    
    HomValueRef& HomObject::operator[](const String& key) {
        return this->value[key];
    }
    
    HomValueRef& HomObject::getValue(const eokas::String& key) {
        return this->value[key];
    }
    
    f64_t HomObject::getNumber(const String& key, f64_t defaultValue) {
        f64_t val = defaultValue;
        this->get(key, val);
        return val;
    }
    
    bool HomObject::getBoolean(const String& key, bool defaultValue) {
        bool val = defaultValue;
        this->get(key, val);
        return val;
    }
    
    String HomObject::getString(const String& key, const String& defaultValue) {
        String val = defaultValue;
        this->get(key, val);
        return val;
    }
    
    HomValueArray HomObject::getArray(const String& key, const HomValueArray& defaultValue) {
        HomValueArray val = defaultValue;
        this->get(key, val);
        return val;
    }
    
    HomValueMap HomObject::getObject(const String& key, const HomValueMap& defaultValue) {
        HomValueMap val = defaultValue;
        this->get(key, val);
        return val;
    }
    
    bool HomObject::get(const String& key, HomValueRef& val) {
        auto iter = this->value.find(key);
        if (iter == this->value.end())
            return false;
        val = iter->second;
        return true;
    }
    
    bool HomObject::get(const String& key, HomType type, HomValueRef& val) {
        auto iter = this->value.find(key);
        if (iter == this->value.end())
            return false;
        if (iter->second == nullptr || iter->second->type != type)
            return false;
        val = iter->second;
        return true;
    }
    
    bool HomObject::get(const String& key, f64_t& val) {
        HomValueRef ref;
        if (!this->get(key, HomType::Number, ref))
            return false;
        val = HomNumber::pick(ref);
        return true;
    }
    
    bool HomObject::get(const String& key, bool& val) {
        HomValueRef ref;
        if (!this->get(key, HomType::Boolean, ref))
            return false;
        val = HomBoolean::pick(ref);
        return true;
    }
    
    bool HomObject::get(const String& key, String& val) {
        HomValueRef ref;
        if (!this->get(key, HomType::String, ref))
            return false;
        val = HomString::pick(ref);
        return true;
    }
    
    bool HomObject::get(const String& key, HomValueArray& val) {
        HomValueRef ref;
        if (!this->get(key, HomType::Array, ref))
            return false;
        val = HomArray::pick(ref);
        return true;
    }
    
    bool HomObject::get(const String& key, HomValueMap& val) {
        HomValueRef ref;
        if (!this->get(key, HomType::Object, ref))
            return false;
        val = HomObject::pick(ref);
        return true;
    }
    
    void HomObject::set(const String& key, HomValueRef val) {
        this->value[key] = std::move(val);
    }
    
    void HomObject::set(const String& key, f64_t val) {
        this->value[key] = HomNumber::make(val);
    }
    
    void HomObject::set(const String& key, bool val) {
        this->value[key] = HomBoolean::make(val);
    }
    
    void HomObject::set(const String& key, const String& val) {
        this->value[key] = HomString::make(val);
    }
    
    void HomObject::set(const String& key, const HomValueArray& val) {
        this->value[key] = HomArray::make(val);
    }
    
    void HomObject::set(const String& key, const HomValueMap& val) {
        this->value[key] = HomObject::make(val);
    }
    
    HomValueRef HomObject::make(const HomValueMap& value) {
        return HomValueRef(new HomObject(value));
    }
    
    HomValueMap HomObject::pick(const HomValueRef& value) {
        auto ptr = (HomObject*) value.get();
        return ptr != nullptr ? ptr->value : HomValueMap();
    }
    
}
