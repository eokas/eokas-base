#pragma once

#include "base/main.h"
#include "imgui/imgui.h"

namespace eokas::datapot {
    void ImWidgets_Text(const String& id, const String& text);
    void ImWidgets_Input(const String& id, StringValue& value, ImGuiInputTextFlags flags);
    void ImWidgets_Combo(const String& id, int& value, const std::vector<String>& items, ImGuiComboFlags flags);
    void ImWidgets_Selector(const String& id, StringValue& value, std::function<void(StringValue&)> onSelect);
    void ImWidgets_FileSelector(const String& id, StringValue& value, const std::map<String, String>& filters);
    void ImWidgets_FolderSelector(const String& id, StringValue& value);
}
