#pragma once

#include "./models.h"
#include "./views.h"

namespace eokas::datapot {
    class Controller {
    public:
        static Controller& instance() {
            static Controller sInstance;
            return sInstance;
        }
        
        void actionNewFile();
        void actionOpenFile();
        void actionSaveFile();
    };
}