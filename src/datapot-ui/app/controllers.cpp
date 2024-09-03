#include "./controllers.h"
#include "utils/utils.h"

namespace eokas::datapot {
    void Controller::actionNewFile() {
        String filePath = "";
        if(OpenDirectoryDialog(filePath, "")) {
        
        }
    }
    
    void Controller::actionOpenFile() {
        String filePath = "";
        std::map<String, String> filter;
        filter["DataPot Library File"] = "*.datapot";
        if(OpenFileDialog(filePath, filter)) {
        
        }
    }
    
    void Controller::actionSaveFile() {
    
    }
}
