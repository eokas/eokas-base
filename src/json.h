
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
        Null,
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

    static JsonValue::Ref make(const String& value)
    {
        return JsonValue::Ref(new JsonValue_Identifier(value));
    }

    static String pick(const JsonValue::Ref& json)
    {
        auto ptr = (JsonValue_Identifier*)json.get();
        return ptr != nullptr ? ptr->value : "";
    }
};

struct JsonValue_Null :public JsonValue
{
    explicit JsonValue_Null()
            : JsonValue(Type::Null)
    {}

    static JsonValue::Ref make()
    {
        return JsonValue::Ref(new JsonValue_Null());
    }
};

struct JsonValue_Number :public JsonValue
{
    f64_t value;

    explicit JsonValue_Number(f64_t value = 0)
        : JsonValue(Type::Number), value(value)
    {}

    static JsonValue::Ref make(f64_t value)
    {
        return JsonValue::Ref(new JsonValue_Number(value));
    }

    static f64_t pick(const JsonValue::Ref& json)
    {
        auto ptr = (JsonValue_Number*)json.get();
        return ptr != nullptr ? ptr->value : 0;
    }
};

struct JsonValue_Boolean :public JsonValue
{
    bool value;

    explicit JsonValue_Boolean(bool value = false)
        : JsonValue(Type::Boolean), value(value)
    {}

    static JsonValue::Ref make(bool value)
    {
        return JsonValue::Ref(new JsonValue_Boolean(value));
    }

    static f64_t pick(const JsonValue::Ref& json)
    {
        auto ptr = (JsonValue_Boolean*)json.get();
        return ptr != nullptr ? ptr->value : "";
    }
};

struct JsonValue_String :public JsonValue
{
    String value;

    explicit JsonValue_String(String  value)
        : JsonValue(Type::String), value(std::move(value))
    {}

    static JsonValue::Ref make(const String& value)
    {
        return JsonValue::Ref(new JsonValue_String(value));
    }

    static String pick(const JsonValue::Ref& json)
    {
        auto ptr = (JsonValue_String*)json.get();
        return ptr != nullptr ? ptr->value : "";
    }
};

struct JsonValue_Array :public JsonValue
{
    std::vector<JsonValue::Ref> value;

    explicit JsonValue_Array(const std::vector<JsonValue::Ref> value = {})
        : JsonValue(Type::Array), value(value)
    { }

    static JsonValue::Ref make(const std::vector<JsonValue::Ref>& value)
    {
        return JsonValue::Ref(new JsonValue_Array(value));
    }

    static std::vector<JsonValue::Ref> pick(const JsonValue::Ref& json)
    {
        auto ptr = (JsonValue_Array*)json.get();
        return ptr != nullptr ? ptr->value : std::vector<JsonValue::Ref>();
    }
};

struct JsonValue_Object :public JsonValue
{
    std::map<String, JsonValue::Ref> value;

    explicit JsonValue_Object(const std::map<String, JsonValue::Ref> value = {})
        :JsonValue(Type::Object), value(value)
    {}

    static JsonValue::Ref make(const std::map<String, JsonValue::Ref>& value)
    {
        return JsonValue::Ref(new JsonValue_Object(value));
    }

    static std::map<String, JsonValue::Ref> pick(const JsonValue::Ref& json)
    {
        auto ptr = (JsonValue_Object*)json.get();
        return ptr != nullptr ? ptr->value : std::map<String, JsonValue::Ref>();
    }
};

struct Json
{
    static String stringify(const JsonValue::Ref& json);

    static JsonValue::Ref parse(const String& source);
};

_EndNamespace(eokas)

#endif//_EOKAS_BASE_JSON_H_
