#include "./models.h"

namespace eokas::datapot {
    Model::Model() { }
    
    Model::~Model() {
        _DeleteList(mLibraries)
    }
    
    Library* Model::createLibrary(const String& name) {
        Library* lib = new Library(name);
        this->mLibraries.push_back(lib);
        return lib;
    }
}
