
#include "json.h"
#include "ascil.h"

_BeginNamespace(eokas)

struct JsonLexer
{
    String mSource;
    size_t mPosition;

    explicit JsonLexer(const String& source)
        :mSource(source)
        ,mPosition(0)
    {}

    String nextName()
    {
        char c = this->nextCleanChar();
        switch (c)
        {
            case '\0': // eof
                return "";

            case '\'':
            case '"': {
                auto str = this->nextString(c);
                return static_cast<JsonValue_String*>(str.get())->value;
            }
            default:
                if(_ascil_is_alpha_(c))
                {
                    auto id = this->nextIdentifier(c);
                    return static_cast<JsonValue_Identifier*>(id.get())->value;
                }
        }

        return "";
    }

    JsonValue::Ref nextValue()
    {
        char c = this->nextChar();
        switch(c)
        {
            case '[':
                return this->nextArray();
            case '{':
                return this->nextObject();
            case '\'':
            case '\"':
                return this->nextString(c);
            default:
                if(_ascil_is_number(c) || c == '+' || c == '-')
                {
                    return this->nextNumber(c);
                }
                else if(_ascil_is_alpha_(c))
                {
                    return this->nextIdentifier(c);
                }
                return JsonValue();
        }
    }

    JsonValue::Ref nextArray()
    {
        std::vector<JsonValue::Ref> list;

        int count = 0;

        char first = this->nextCleanChar();
        if(first == ']')
        {
            return JsonValue::Ref(new JsonValue_Array(list));
        }
        else if(first == '\0')
        {
            mPosition -= 1;
        }

        while(true)
        {
            JsonValue::Ref value = this->nextValue();
            list.push_back(value);

            char c = this->nextCleanChar();
            switch(c)
            {
                case ']':
                    return JsonValue::Ref(new JsonValue_Array(list));
                case ',':
                    continue;
                default:
                    return nullptr;
            }
        }
    }

    JsonValue::Ref nextObject()
    {
        std::map<String, JsonValue::Ref> object;

        /* Peek to see if this is the empty object. */
        char first = this->nextCleanChar();
        if (first == '}')
        {
            return JsonValue::Ref(new JsonValue_Object(object));
        }
        else if (first != '\0')
        {
            mPosition--;
        }

        while (true)
        {
            auto name = this->nextName();

            /*
             * Expect the name/value separator to be either a colon ':', an
             * equals sign '=', or an arrow "=>". The last two are bogus but we
             * include them because that's what the original implementation did.
             */
            char separator = this->nextCleanChar();
            if (separator != ':' && separator != '=')
            {
                // error: "Expected ':' after " + name
                return nullptr;
            }
            if (mPosition < mSource.length() && mSource.substr(mPosition, 1).at(0) == '>')
            {
                mPosition++;
            }
            object[name] = this->nextValue();

            switch (this->nextCleanChar())
            {
                case '}':
                    return object;
                case ';':
                case ',':
                    continue;
                default: // error: Unterminated object
                    return nullptr;
            }
        }
    }

    JsonValue::Ref nextNumber(char first)
    {
        String str(first);
        if(first != '+' && first != '-')
        {
            char c = this->nextChar();
            if(c == '.')
            {
                str += c;
                c = this->nextChar();
                while(_ascil_is_number(c))
                {
                    str += c;
                    c = this->nextChar();
                }
            }
            else
            {
                c = this->nextChar();
                while(_ascil_is_number(c))
                {
                    str += c;
                    c = this->nextChar();
                }
                if(c == '.')
                {
                    str += c;
                    c = this->nextChar();
                }
                while(_ascil_is_number(c))
                {
                    str += c;
                    c = this->nextChar();
                }
            }
        }
        else
        {
            char c = this->nextChar();
            while(_ascil_is_number(c))
            {
                str += c;
                c = this->nextChar();
            }
            if(c == '.')
            {
                str += c;
                c = this->nextChar();
            }
            while(_ascil_is_number(c))
            {
                str += c;
                c = this->nextChar();
            }
        }

        mPosition -= 1;

        auto value = String::stringToValue<f64_t>(str);
        return JsonValue::Ref(new JsonValue_Number(value));
    }

    JsonValue::Ref nextString(char quote)
    {
        String str = "";

        size_t start = mPosition;
        for(char c = this->nextChar(); c != '\0'; c = this->nextChar())
        {
            if(c == quote)
            {
                str += mSource.substr(start, mPosition - start - 1);
                return JsonValue::Ref(new JsonValue_String(str));
            }

            if(c == '\\')
            {
                str += mSource.substr(start, mPosition - start - 1);
                str += this->escapeChar();
                start = mPosition;
            }
        }

        // error : Unterminated string
        return JsonValue::Ref(new JsonValue_String(""));
    }

    JsonValue::Ref nextIdentifier(char first)
    {
        String str(first);
        char c = this->nextChar();
        while(_ascil_is_alpha_number(c))
        {
            str += c;
            c = this->nextChar();
        }

        mPosition -= 1;

        return JsonValue::Ref(new JsonValue_Identifier(str));
    }

    char escapeChar()
    {
        char c = this->nextChar();
        switch(c) {
            case 'u': {
                if (mPosition + 4 >= mSource.length()) {
                    // error: Unterminated escape sequence
                    return ' ';
                }

                String hex = mSource.substr(mPosition, 4);
                mPosition += 4;

                // TODO: Int.parse(hex, 16) as char
                return ' ';
            }

            case 'a': return '\a';
            case 'b': return '\b';
            case 'f': return '\f';
            case 't': return '\t';
            case 'n': return '\n';
            case 'r': return '\r';
            case 'v': return '\v';

            case '\'':
            case '"':
            case '\\':
            default:
                return c;
        }
    }

    char nextCleanChar()
    {
        for(char c = this->nextChar(); c != '\0'; c = this->nextChar())
        {
            switch(c)
            {
                case ':': case '\t': case '\n': case '\r':
                    continue;
                case '#':  // php # comment
                    this->skipToEndOfLine();
                    continue;
                case '/':
                {
                    char next = this->nextChar();
                    if (next == '*')  // c-style /**/ comment
                    {
                        size_t commentEnd = mSource.find("*/", mPosition);
                        if (commentEnd == -1) // error
                            return '\0';
                        mPosition = commentEnd + 2;
                        continue;
                    }
                    else if (next == '/') // cpp style // comment
                    {
                        this->skipToEndOfLine();
                        continue;
                    }
                    else
                    {
                        return c;
                    }
                }
                default:
                    return c;
            }
        }

        return '\0';
    }

    void skipComment()
    {
        char current = mSource[mPosition];
        if(current == '/')
        {

        }
        else if(current == '#') // php # comment
        {
            this->skipToEndOfLine();
        }
    }

    void skipToEndOfLine()
    {
        char c = this->nextChar();
        while(c != '\0')
        {
            if(c == '\r' || c == '\n')
                break;
            c = this->nextChar();
        }
    }

    char nextChar()
    {
        if(mPosition >= mSource.length())
            return '\0';
        char c = mSource.at(mPosition);
        mPosition += 1;
        return c;
    }
};

String Json::stringify(const JsonValue::Ref& json)
{
    switch(json->type)
    {
        case JsonValue::Type::Number: {
            auto value = static_cast<JsonValue_Number *>(json.get())->value;
            return String::valueToString(value);
        }
        case JsonValue::Type::Boolean: {
            auto value = static_cast<JsonValue_Boolean *>(json.get())->value;
            return String::valueToString(value);
        }
        case JsonValue::Type::String: {
            auto value = static_cast<JsonValue_String *>(json.get())->value;
            return String::format("\"%s\"", value.cstr());
        }
        case JsonValue::Type::Array: {
            String str = "[";
            bool first = true;
            auto value = static_cast<JsonValue_Array*>(json.get())->value;
            for(const auto& i : value)
            {
                if(first)
                {
                    str +=  ", ";
                    first = false;
                }
                str += stringify(i);
            }
            str += "]";
            return str;
        }
        case JsonValue::Type::Object:
        {
            String str = "{";
            bool first = true;
            auto value = static_cast<JsonValue_Object*>(json.get())->value;
            for(const auto& i : value)
            {
                if(first)
                {
                    str +=  ", ";
                    first = false;
                }
                str += String::format("\"%s\":", i.first.cstr());
                str += stringify(i.second);
            }
            str += "}";
            return str;
        }
    }
    return "";
}

JsonValue::Ref Json::parse(const String& source)
{
    JsonLexer lexer(source);
    return lexer.nextValue();
}

_EndNamespace(eokas)
