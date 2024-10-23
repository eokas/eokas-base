
#ifndef  _EOKAS_BASE_HOM_H_
#define  _EOKAS_BASE_HOM_H_

/**
 * HOM: Hierarchical Object Model.
 * It is similar with DOM (Document Object Model)
 * Widely used in JSON
 * */

#include "./string.h"
#include <utility>

namespace eokas {
    
    enum class HomType {
        Null, Number, Boolean, String, Array, Object,
    };
    
    struct HomValue {
        HomType type;
        
        explicit HomValue(HomType type)
            : type(type) {
        }
    };
    
    using HomValueRef = std::shared_ptr<HomValue>;
    using HomValueArray = std::vector<HomValueRef>;
    using HomValueMap = std::map<String, HomValueRef>;
    
    struct HomNull : public HomValue {
        explicit HomNull()
            : HomValue(HomType::Null) {
        }
    };
    
    struct HomNumber : public HomValue {
        f64_t value;
        
        explicit HomNumber(f64_t value = 0)
            : HomValue(HomType::Number), value(value) {
        }
    };
    
    struct HomBoolean : public HomValue {
        bool value;
        
        explicit HomBoolean(bool value = false)
            : HomValue(HomType::Boolean), value(value) {
        }
    };
    
    struct HomString : public HomValue {
        String value;
        
        explicit HomString(const String& value = "")
            : HomValue(HomType::String), value(std::move(value)) {
        }
    };
    
    struct HomArray : public HomValue {
        HomValueArray value;
        
        explicit HomArray(const HomValueArray& val = {});
        
        HomValueRef get(size_t index);
        void set(size_t index, HomValueRef val);
        HomArray& add(HomValueRef val);
    };
    
    struct HomObject : public HomValue {
        HomValueMap value;
        
        explicit HomObject(const HomValueMap& value = {});
        
        HomValueRef& get(const String& key);
        void set(const String& key, HomValueRef val);
    };
    
    class HOM {
    public:
        HOM();
        HOM(HomValueRef val);
        HOM(HomType type);
        HOM(f64_t val);
        HOM(bool val);
        HOM(const String& val);
        virtual ~HOM();
        
        HomType type() const;
        bool isNull() const;
        bool isNumber() const;
        bool isBoolean() const;
        bool isString() const;
        bool isObject() const;
        bool isArray() const;
        
        f64_t asNumber() const;
        bool asBoolean() const;
        String asString() const;
        
        HOM get(size_t index);
        void set(size_t index, const HOM& val);
        void add(const HOM& val);
        void foreach(const std::function<void(const HOM& val)>& func) const;
        
        HOM get(const String& key);
        void set(const String& key, const HOM& val);
        void foreach(const std::function<void(const String& key, const HOM& val)>& func) const;
    
    private:
        HomValueRef mValue;
    };
    
}

#endif//_EOKAS_BASE_HOM_H_
