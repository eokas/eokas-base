#ifndef _EOKAS_DATAPOT_SCHEMA_H_
#define _EOKAS_DATAPOT_SCHEMA_H_

#include "./header.h"
#include "./value.h"

namespace eokas::datapot {

    enum class SchemaType {
        Int,
        Float,
        Bool,
        String,
        List,
        Struct,
    };

    class Schema {
    public:
        struct Member {
            String name;
            Schema* schema;
        };

        struct ListBody {
            Schema* element = nullptr;
        };

        struct StructBody {
            std::vector<Member> members = {};
        };


        Schema(SchemaType type, const String& name);
        virtual ~Schema();
        
        bool operator==(const Schema& other) const;
        bool operator!=(const Schema& other) const;

        SchemaType type() const { return mType; }
        String name() const { return mName; }
        bool is(SchemaType type) const { return mType == type; }

        void setElement(Schema* schema);
        Schema* getElement() const;

        void addMember(const String& name, Schema* schema);
        void setMember(const String& name, Schema* schema);
        void delMember(const String& name);
        Member* getMember(const String& name);
        Member* getMember(size_t index);
        size_t getMemberCount();
        size_t getMemberIndex(const String& name);

    private:
        SchemaType mType;
        String mName;
        union {
            ListBody* listBody;
            StructBody* structBody;
        } mBody;
    };

    class SchemaHeap {
    public:
        SchemaHeap();
        virtual ~SchemaHeap();
        
        Schema* add(SchemaType type, const String& name);
        Schema* get(const String& name);
        Schema* get(size_t index);
        size_t count() const;
        size_t indexOf(Schema* schema);

    private:
        std::map<String, size_t> mSchemaMap;
        std::vector<Schema*> mSchemas;
    };
}

#endif//_EOKAS_DATAPOT_SCHEMA_H_
