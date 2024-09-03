#ifndef _EOKAS_NATIVE_UTILS_H_
#define _EOKAS_NATIVE_UTILS_H_

#include "./header.h"

namespace eokas {
    bool OpenFileDialog(String& selectedPath, const std::map<String, String>& filters);
    bool OpenDirectoryDialog(String& selectedPath, const String& defaultPath);
}

#endif//_EOKAS_NATIVE_UTILS_H_
