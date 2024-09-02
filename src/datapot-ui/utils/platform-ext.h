#pragma once

#include "base/main.h"

#if _WIN32
#include <windows.h>
#include <commdlg.h>

namespace eokas::datapot {
    bool OpenFileDialog(String& filePath, const std::map<String, String> filters) {
        OPENFILENAME ofn;
        ZeroMemory(&ofn, sizeof(ofn));
        
        char filenameBuffer[260] = {0};
        
        std::vector<char> filterData;
        for(auto& entry : filters) {
            std::string title = entry.first.cstr();
            std::string extension = entry.second.cstr();
            filterData.insert(filterData.end(), title.begin(), title.end());
            filterData.push_back('\0');
            filterData.insert(filterData.end(), extension.begin(), extension.end());
            filterData.push_back('\0');
        }
        filterData.push_back('\0');
        
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = filenameBuffer;
        ofn.nMaxFile = sizeof(filenameBuffer);
        ofn.lpstrFilter = filterData.data();
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        
        if (!GetOpenFileNameA(&ofn))
            return false;
        
        filePath = ofn.lpstrFile;
        return true;
    }
}

#endif
