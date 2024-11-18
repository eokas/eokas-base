
#ifndef _EOKAS_APP_APP_H_
#define _EOKAS_APP_APP_H_

#include "./header.h"
#include "./module.h"

namespace eokas
{
    class App
    {
    public:
        bool init();
        void quit();
        void tick(float deltaTime);
        
        ModuleManager& modules();

        virtual void* window() const = 0;
        
    private:
        ModuleManager mModules;
    };
}

#endif//_EOKAS_APP_APP_H_
