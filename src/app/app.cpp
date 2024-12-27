#include "app/app.h"
#include "native/utils.h"

namespace eokas
{
    bool App::init()
    {
        String configJson;
        if (!File::readText("../config/config.json", configJson))
            return false;
        std::shared_ptr<HomValue> json = JSON::parse(configJson);
        
        mModules.init();
        return true;
    }
    
    void App::quit()
    {
        mModules.quit();
    }
    
    void App::tick(float deltaTime)
    {
        mModules.quit();
    }
    
    ModuleManager& App::modules()
    {
        return mModules;
    }
}