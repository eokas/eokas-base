
#include "./hom.h"

namespace eokas {
    
    HomArray::HomArray(const HomValueArray& val)
        : HomValue(HomType::Array), value(val) {
    }
    
    HomValueRef HomArray::get(size_t index) {
        if (index > this->value.size())
            return nullptr;
        return this->value[index];
    }
    
    void HomArray::set(size_t index, HomValueRef val) {
        this->value[index] = val;
    }
    
    HomArray& HomArray::add(HomValueRef val) {
        this->value.push_back(val);
        return *this;
    }
    
    HomObject::HomObject(const HomValueMap& value)
        : HomValue(HomType::Object), value(std::move(value)) {
    }
    
    HomValueRef& HomObject::get(const eokas::String& key) {
        return this->value[key];
    }
    
    void HomObject::set(const String& key, HomValueRef val) {
        this->value[key] = std::move(val);
    }
    
    HOM::HOM()
        : mValue(nullptr) {}

    HOM::HOM(HomValueRef val)
        : mValue(std::move(val)) {
    }
    
    HOM::HOM(HomType type) {
        if(type == HomType::Null) {
            mValue = std::make_shared<HomNull>();
        }
        else if(type == HomType::Number) {
            mValue = std::make_shared<HomNumber>();
        }
        else if(type == HomType::Boolean) {
            mValue = std::make_shared<HomBoolean>();
        }
        else if(type == HomType::String) {
            mValue = std::make_shared<HomString>();
        }
        else if(type == HomType::Object) {
            mValue = std::make_shared<HomObject>();
        }
        else if(type == HomType::Array) {
            mValue = std::make_shared<HomArray>();
        }
    }

    HOM::HOM(f64_t val)
        : mValue(std::make_shared<HomNumber>(val)) { }
    
    HOM::HOM(bool val)
        : mValue(std::make_shared<HomBoolean>(val)) { }
        
    HOM::HOM(const String& val)
        : mValue(std::make_shared<HomString>(val)) { }
        
    HOM::~HOM() {
    }
    
    HomType HOM::type() const {
        return mValue != nullptr ? mValue->type : HomType::Null;
    }
    
    bool HOM::isNull() const {
        return mValue != nullptr && mValue->type == HomType::Null;
    }
    
    bool HOM::isNumber() const {
        return mValue != nullptr && mValue->type == HomType::Number;
    }
    
    bool HOM::isBoolean() const {
        return mValue != nullptr && mValue->type == HomType::Boolean;
    }
    
    bool HOM::isString() const {
        return mValue != nullptr && mValue->type == HomType::String;
    }
    
    bool HOM::isObject() const {
        return mValue != nullptr && mValue->type == HomType::Object;
    }
    
    bool HOM::isArray() const {
        return mValue != nullptr && mValue->type == HomType::Array;
    }
    
    f64_t HOM::asNumber() const {
        return mValue != nullptr && mValue->type == HomType::Number
            ? ((HomNumber*)mValue.get())->value
            : 0;
    }
    
    bool HOM::asBoolean() const {
        return mValue != nullptr && mValue->type == HomType::Boolean &&
        ((HomBoolean*) mValue.get())->value;
    }
    
    String HOM::asString() const {
        return mValue != nullptr && mValue->type == HomType::String
            ? ((HomString*)mValue.get())->value
            : "";
    }
    
    HOM HOM::get(size_t index) {
        if(mValue == nullptr || mValue->type != HomType::Array)
            return HOM{HomType::Null};
        return HOM{((HomArray*)mValue.get())->get(index)};
    }
    
    void HOM::set(size_t index, const HOM& val) {
        if(mValue == nullptr || mValue->type != HomType::Array)
            return;
        ((HomArray*)mValue.get())->set(index, val.mValue);
    }
    
    void HOM::add(const HOM& val) {
        if(mValue == nullptr || mValue->type != HomType::Array)
            return;
        ((HomArray*)mValue.get())->add(val.mValue);
    }
    
    void HOM::foreach(const std::function<void(const HOM&)>& func) const {
        if(!func)
            return;
        if(mValue == nullptr || mValue->type != HomType::Array)
            return;
        HomValueArray& list = ((HomArray*)mValue.get())->value;
        for(auto& val : list) {
            func(val);
        }
    }
    
    HOM HOM::get(const String& key) {
        if(mValue == nullptr || mValue->type != HomType::Object)
            return HOM{};
        return HOM{((HomObject*)mValue.get())->get(key)};
    }
    
    void HOM::set(const String& key, const HOM& val) {
        if(mValue == nullptr || mValue->type != HomType::Object)
            return;
        ((HomObject*)mValue.get())->set(key, val.mValue);
    }
    
    void HOM::foreach(const std::function<void(const String& key, const HOM& val)>& func) const {
        if(!func)
            return;
        if(mValue == nullptr || mValue->type != HomType::Object)
            return;
        HomValueMap& map = ((HomObject*)mValue.get())->value;
        for(auto& pair : map) {
            func(pair.first, pair.second);
        }
    }
}
