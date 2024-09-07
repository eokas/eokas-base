#pragma once

#include "datapot/library.h"

namespace eokas::datapot {
    struct Result {
        bool ok = true;
        String message = "";
        
        Result(bool ok, const String& message = "")
            : ok(ok), message(message) { }
    };
    
    class Logic {
    public:
        static Logic& instance() {
            static Logic sInstance;
            return sInstance;
        }
        
        static String stringifySchemaType(SchemaType type);
        static SchemaType parseSchemaType(const String& str);
        
        Logic();
        virtual ~Logic();
        
        std::function<void(const Result& error)> onError;
        Result result(bool ok, const String& msg = "");
        
        class Library* library = nullptr;
        class Schema* schema = nullptr;
        
        std::function<void()> actionFileNew;
        std::function<void()> actionFileOpen;
        std::function<void()> actionFileSave;
        
        std::function<void()> actionNewSchema;
        
        Result createLibrary(const String& name, const String& home);
        std::function<void()> onCreateLibrary;
        
        Result openLibrary();
        std::function<void()> onOpenLibrary;
        
        Result selectSchema(Schema* schema);
        std::function<void()> onSelectSchema;
        
        Result createSchema(const String& type, const String& name);
        std::function<void()> onCreateSchema;
    };
}