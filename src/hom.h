
#ifndef  _EOKAS_BASE_HOM_H_
#define  _EOKAS_BASE_HOM_H_

#include <utility>

#include "string.h"

_BeginNamespace(eokas)

struct HomValue
{
    using Ref = std::shared_ptr<HomValue>;

    enum class Type
    {
        Identifier,
        Null,
        Number,
        Boolean,
        String,
        Array,
        Object,
    };

    Type type;

    explicit HomValue(Type type)
        :type(type)
    {}
};

struct HomIdentifier : public HomValue
{
    String value;

    explicit HomIdentifier(const String& value = "")
        : HomValue(Type::Identifier), value(value)
    {}

    static HomValue::Ref make(const String& value)
    {
        return HomValue::Ref(new HomIdentifier(value));
    }

    static String pick(const HomValue::Ref& json)
    {
        auto ptr = (HomIdentifier*)json.get();
        return ptr != nullptr ? ptr->value : "";
    }
};

struct HomNull : public HomValue
{
    explicit HomNull()
        : HomValue(Type::Null)
    {}

    static HomValue::Ref make()
    {
        return HomValue::Ref(new HomNull());
    }
};

struct HomNumber : public HomValue
{
    f64_t value;

    explicit HomNumber(f64_t value = 0)
        : HomValue(Type::Number), value(value)
    {}

    static HomValue::Ref make(f64_t value)
    {
        return HomValue::Ref(new HomNumber(value));
    }

    static f64_t pick(const HomValue::Ref& json)
    {
        auto ptr = (HomNumber*)json.get();
        return ptr != nullptr ? ptr->value : 0;
    }
};

struct HomBoolean : public HomValue
{
    bool value;

    explicit HomBoolean(bool value = false)
        : HomValue(Type::Boolean), value(value)
    {}

    static HomValue::Ref make(bool value)
    {
        return HomValue::Ref(new HomBoolean(value));
    }

    static bool pick(const HomValue::Ref& json)
    {
        auto ptr = (HomBoolean*)json.get();
        return ptr != nullptr ? ptr->value : false;
    }
};

struct HomString : public HomValue
{
    String value;

    explicit HomString(String  value)
        : HomValue(Type::String), value(std::move(value))
    {}

    static HomValue::Ref make(const String& value)
    {
        return HomValue::Ref(new HomString(value));
    }

    static String pick(const HomValue::Ref& json)
    {
        auto ptr = (HomString*)json.get();
        return ptr != nullptr ? ptr->value : "";
    }
};

struct HomArray : public HomValue
{
    std::vector<HomValue::Ref> value;

    explicit HomArray(const std::vector<HomValue::Ref> value = {})
        : HomValue(Type::Array), value(value)
    { }

    static HomValue::Ref make(const std::vector<HomValue::Ref>& value)
    {
        return HomValue::Ref(new HomArray(value));
    }

    static std::vector<HomValue::Ref> pick(const HomValue::Ref& json)
    {
        auto ptr = (HomArray*)json.get();
        return ptr != nullptr ? ptr->value : std::vector<HomValue::Ref>();
    }
};

struct HomObject : public HomValue
{
    std::map<String, HomValue::Ref> value;

    explicit HomObject(const std::map<String, HomValue::Ref> value = {})
        : HomValue(Type::Object), value(value)
    {}

    static HomValue::Ref make(const std::map<String, HomValue::Ref>& value)
    {
        return HomValue::Ref(new HomObject(value));
    }

    static std::map<String, HomValue::Ref> pick(const HomValue::Ref& json)
    {
        auto ptr = (HomObject*)json.get();
        return ptr != nullptr ? ptr->value : std::map<String, HomValue::Ref>();
    }
};

_EndNamespace(eokas)

#endif//_EOKAS_BASE_HOM_H_
