
#ifndef  _EOKAS_BASE_JSON_H_
#define  _EOKAS_BASE_JSON_H_

#include <utility>

#include "string.h"

_BeginNamespace(eokas)

struct JsonValue
{
    using Ref = std::shared_ptr<JsonValue>;

    enum class Type
    {
        Identifier,
        Number,
        Boolean,
        String,
        Array,
        Object,
    };

    Type type;

    explicit JsonValue(Type type)
        :type(type)
    {}
};

struct JsonValue_Identifier :public JsonValue
{
    String value;

    explicit JsonValue_Identifier(const String& value = "")
        : JsonValue(Type::Identifier), value(value)
    {}
};

struct JsonValue_Number :public JsonValue
{
    f64_t value;

    explicit JsonValue_Number(f64_t value = 0)
        : JsonValue(Type::Number), value(value)
    {}
};

struct JsonValue_Boolean :public JsonValue
{
    bool value;

    explicit JsonValue_Boolean(bool value = false)
    : JsonValue(Type::Boolean), value(value)
    {}
};

struct JsonValue_String :public JsonValue
{
    String value;

    explicit JsonValue_String(String  value)
        : JsonValue(Type::String), value(std::move(value))
    {}
};

struct JsonValue_Array :public JsonValue
{
    std::vector<JsonValue::Ref> value;

    explicit JsonValue_Array(const std::vector<JsonValue::Ref> value = {})
        : JsonValue(Type::Array), value(value)
    { }
};

struct JsonValue_Object :public JsonValue
{
    std::map<String, JsonValue::Ref> value;

    explicit JsonValue_Object(const std::map<String, JsonValue::Ref> value = {})
        :JsonValue(Type::Object), value(value)
    {}
};

struct Json
{
    static JsonValue::Ref makeIdentifier(const String& value)
    {
        return JsonValue::Ref(new JsonValue_Identifier(value));
    }

    static String getIdentifier(const JsonValue::Ref& json)
    {
        auto ptr = (JsonValue_Identifier*)json.get();
        return ptr != nullptr ? ptr->value : "";
    }

    static JsonValue::Ref makeNumber(f64_t value)
    {
        return JsonValue::Ref(new JsonValue_Number(value));
    }

    static f64_t getNumber(const JsonValue::Ref& json)
    {
        auto ptr = (JsonValue_Number*)json.get();
        return ptr != nullptr ? ptr->value : "";
    }

    static JsonValue::Ref makeBoolean(bool value)
    {
        return JsonValue::Ref(new JsonValue_Boolean(value));
    }

    static f64_t getBoolean(const JsonValue::Ref& json)
    {
        auto ptr = (JsonValue_Boolean*)json.get();
        return ptr != nullptr ? ptr->value : "";
    }

    static JsonValue::Ref makeString(const String& value)
    {
        return JsonValue::Ref(new JsonValue_String(value));
    }

    static String getString(const JsonValue::Ref& json)
    {
        auto ptr = (JsonValue_String*)json.get();
        return ptr != nullptr ? ptr->value : "";
    }

    static JsonValue::Ref makeArray(const std::vector<JsonValue::Ref>& value)
    {
        return JsonValue::Ref(new JsonValue_Array(value));
    }

    static std:: getArray(const JsonValue::Ref& json)
    {
        auto ptr = (JsonValue_Number*)json.get();
        return ptr != nullptr ? ptr->value : "";
    }

    static JsonValue::Ref makeObject(const std::map<String, JsonValue::Ref>& value)
    {
        return JsonValue::Ref(new JsonValue_Object(value));
    }

    static String stringify(const JsonValue::Ref& json);

    static JsonValue::Ref parse(const String& source);
};

_EndNamespace(eokas)

#endif//_EOKAS_BASE_JSON_H_
