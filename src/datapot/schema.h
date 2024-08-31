#ifndef _EOKAS_DATAPOT_SCHEMA_H_
#define _EOKAS_DATAPOT_SCHEMA_H_

#include "./header.h"
#include "./Value.h"

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
        Schema(SchemaType type, const String& name);
        virtual ~Schema();
        
        SchemaType type() const { return mType; }
        String name() const { return mName; }
        bool is(SchemaType type) const { return mType == type; }

    private:
        SchemaType mType;
        String mName;
    };
    
    class IntSchema :public Schema {
    public:
        IntSchema();
    };
    
    class FloatSchema :public Schema {
    public:
        FloatSchema();
    };
    
    class BoolSchema :public Schema {
    public:
        BoolSchema();
    };
    
    class StringSchema :public Schema {
    public:
        StringSchema();
    };
    
    class ListSchema :public Schema {
    public:
        ListSchema(const String& name);
    };
    
    class StructSchema :public Schema {
    public:
        struct Member {
            String name;
            Schema* schema;
        };
        
    public:
        StructSchema(const String& name);
        virtual ~StructSchema();
        
        void addMember(const String& name, Schema* schema);
        void setMember(const String& name, Schema* schema);
        void delMember(const String& name);
        Member* getMember(const String& name);
        Member* getMember(int index);
        size_t getMemberCount();
        size_t getMemberIndex(const String& name);
        
    private:
        std::vector<Member> mMembers = {};
    };
}

#endif//_EOKAS_DATAPOT_SCHEMA_H_
