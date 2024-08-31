
#ifndef _EOKAS_DATAPOT_LIBRARY_H_
#define _EOKAS_DATAPOT_LIBRARY_H_

#include "./header.h"
#include "./Value.h"

namespace eokas::datapot {
    class Library {
    public:
        Library(const String& name);
        virtual ~Library();
        
        const String& name() const { return mName; }
        
        bool addSchema(Schema* schema);
        Schema* getSchama(const String& name);

        Value* make(Schema* schema);
        Value* make(i64_t val);
        Value* make(f64_t val);
        Value* make(bool val);
        Value* make(const String& val);
        
        Value* get(const String& name);
        Value* get(Value* list, u64_t index);
        Value* get(Value* object, const String& field);
        
        void set(const String& name, Value* val);
        void set(Value* list, u64_t index, Value* val);
        void set(Value* object, const String& field, Value* val);
        
        bool get(Value* ptr, i64_t* val);
        bool get(Value* ptr, f64_t* val);
        bool get(Value* ptr, bool* val);
        bool get(Value* ptr, String& val);
 
        bool set(Value* ptr, i32_t val);
        bool set(Value* ptr, i64_t val);
        bool set(Value* ptr, f64_t val);
        bool set(Value* ptr, bool val);
        bool set(Value* ptr, const String& val);
        
    private:
        String mName;
        std::map<String, Schema*> mSchemaTable;
        std::map<String, Value*> mValueTable;
        std::vector<Schema*> mSchemaHeap;
        std::vector<Value*> mValueHeap;
        Memory mMemory;
    };
}

#endif//_EOKAS_DATAPOT_LIBRARY_H_
