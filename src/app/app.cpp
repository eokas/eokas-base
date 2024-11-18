#include "app/app.h"
#include "native/utils.h"

namespace eokas {
    bool App::init() {
        String configJson;
        if(!File::readText("./app.json", configJson))
            return false;
        HomNode json = JSON::parse(configJson);
        HomNode window = json.get("Window");
        bool fullScreen = window.get("fullscreen").asBoolean();
        f64_t width = window.get("width").asNumber();
        f64_t height = window.get("height").asNumber();
        
        
        
        mModules.init();
        return true;
    }
    
    void App::quit() {
        mModules.quit();
    }
    
    void App::tick(float deltaTime) {
        mModules.quit();
    }
    
    ModuleManager& App::modules() {
        return mModules;
    }
}