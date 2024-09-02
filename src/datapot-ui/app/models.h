#pragma once

#include "base/main.h"
#include "datapot/library.h"

namespace eokas::datapot {
    class Model {
    public:
        static Model& instance() {
            static Model sInstance;
            return sInstance;
        }
        
        Model();
        virtual ~Model();
        
        Library* createLibrary(const String& name);
        
    private:
        std::vector<Library*> mLibraries = {};
    };
}
