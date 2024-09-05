#pragma once

#include "./views.h"

namespace eokas::datapot {
    struct Result {
        bool ok = true;
        String message = "";
        
        Result(bool ok, const String& message = "")
            :ok(ok), message(message) { }
    };
    
    class Logic {
    public:
        static Logic& instance() {
            static Logic sInstance;
            return sInstance;
        }
        
        MyMainWindow* mainWindow = nullptr;
        
        void actionFileNew();
        void actionFileOpen();
        void actionFileSave();
        
        Result createLibrary(const String& name, const String& home);
        
    private:
        class Library* library;
    };
}