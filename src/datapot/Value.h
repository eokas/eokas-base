#ifndef _EOKAS_DATAPOT_VALUE_H_
#define _EOKAS_DATAPOT_VALUE_H_

#include "./header.h"

namespace eokas::datapot {
    
    struct List {
        std::vector<Value> elements;
    };
    
    struct Object {
        std::map<String, Value> members;
    };
    
    struct Value {
        Schema* schema;
        union {
            i64_t i64;
            f64_t f64;
        } value;
    };
    
    struct Memory {
        std::vector<Value> values;
        
        std::vector<List> lists;
        std::vector<Object> objects;
        
        std::vector<String> strings;
    };
}

#endif//_EOKAS_DATAPOT_VALUE_H_