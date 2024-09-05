#include "./logic.h"
#include "utils/utils.h"
#include "datapot/library.h"

namespace eokas::datapot {
    Logic::Logic() {}
    
    Logic::~Logic() {
        _DeletePointer(this->library);
    }
    
    void Logic::actionFileNew() {
        mainWindow->createLibrary->show();
    }
    
    void Logic::actionFileOpen() {
        String filePath = "";
        std::map<String, String> filter;
        filter["DataPot Library File"] = "*.datapot";
        if(OpenFileDialog(filePath, filter)) {
        
        }
    }
    
    void Logic::actionFileSave() {
    
    }
    
    Result Logic::createLibrary(const eokas::String& name, const eokas::String& home) {
        if(this->library != nullptr) {
            _DeletePointer(this->library);
        }
        
        String fileName = String::format("%s.datapot", name.cstr());
        String filePath = File::combinePath(home, fileName);
        if(File::exists(filePath)) {
            String msg = String::format("The file: %s is existed already.", filePath.cstr());
            mainWindow->toast->open(msg);
            return Result{false, msg};
        }
        
        this->library = new Library(filePath);
        this->library->save(filePath);
        
        return Result{true};
    }
}
