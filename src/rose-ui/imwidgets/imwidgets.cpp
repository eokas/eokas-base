#include "./imwidgets.h"
#include "imgui/imgui.h"
#include "native/utils.h"

namespace eokas::datapot {
    void ImWidgets_Text(const String& id, const String& text) {
        ImGui::Text("%s", text.cstr());
    }
    
    void ImWidgets_Input(const String& id, StringValue& value, ImGuiInputTextFlags flags) {
        char buffer[255] = {0};
        String valuestr = value.string();
        u32_t len = valuestr.length() < 255 ? valuestr.length() : 255;
        memcpy(buffer, valuestr.cstr(), len);
        
        ImVec2 size = ImGui::GetContentRegionAvail();
        ImGui::SetNextItemWidth(size.x);
        ImGui::InputText(*id, buffer, sizeof(buffer), flags);
        
        value =buffer;
    }
    
    void ImWidgets_Combo(const String& id, int& value, const std::vector<String>& items, ImGuiComboFlags flags) {
        std::vector<const char*> rawItems;
        for(auto& item : items) {
            rawItems.push_back(*item);
        }
        
        ImVec2 size = ImGui::GetContentRegionAvail();
        ImGui::SetNextItemWidth(size.x);
        
        ImGui::Combo(*id, &value, rawItems.data(), (int)rawItems.size());
    }
    
    void ImWidgets_Selector(const String& id, StringValue& value, std::function<void(StringValue&)> onSelect) {
        ImVec2 layoutSize = ImGui::GetContentRegionAvail();
        ImVec2 buttonSize = ImGui::CalcTextSize("--");
        float buttonX = layoutSize.x - buttonSize.x;
        
        ImVec2 spacing = ImGui::GetStyle().ItemSpacing;
        float textWidth = layoutSize.x - buttonSize.x - spacing.x;
        
        ImGui::BeginDisabled();
        ImGui::SetNextItemWidth(textWidth);
        String text = value.string();
        ImGui::InputText(*id, (char*)text.cstr(), text.length(), ImGuiInputTextFlags_ReadOnly);
        ImGui::EndDisabled();
        
        ImGui::SameLine(buttonX);
        if(ImGui::Button("--")) {
            if(onSelect) {
                onSelect(value);
            }
        }
    }
    
    void ImWidgets_FileSelector(const String& id, StringValue& value, const std::map<String, String>& filters) {
        ImWidgets_Selector(id, value, [&](StringValue& value){
            String path;
            OpenFileDialog(path, filters);
            value = path;
        });
    }
    
    void ImWidgets_FolderSelector(const String& id, StringValue& value) {
        ImWidgets_Selector(id, value, [&](StringValue& value){
            String path;
            OpenFolderDialog(path, "");
            value = path;
        });
    }
}
