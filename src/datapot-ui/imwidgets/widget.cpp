#include "./widget.h"
#include "./imwidgets.h"
#include "native/utils.h"
#include <cstring>

namespace eokas::datapot {
    static u64_t gWidgetIndex = 0;
    
    UIWidget::UIWidget()
    {
        this->name = String::format("##Widget-%llu", gWidgetIndex++);
    }
    
    UIWidget::~UIWidget() {
    }
    
    void UIWidget::init() { }
    
    void UIWidget::quit() { }
    
    void UIWidget::render(float deltaTime) { }
    
    void UIMainMenuBar::render(float deltaTime) {
        if (ImGui::BeginMainMenuBar()) {
            this->renderChildren(deltaTime);
            ImGui::EndMainMenuBar();
        }
    }
    
    void UIMenuBar::render(float deltaTime) {
        if (ImGui::BeginMenuBar()) {
            this->renderChildren(deltaTime);
            ImGui::EndMenuBar();
        }
    }
    
    void UIMenu::render(float deltaTime) {
        if(ImGui::BeginMenu(this->label.cstr(), this->enabled)) {
            this->renderChildren(deltaTime);
            ImGui::EndMenu();
        }
    }
    
    void UIMenuItem::render(float deltaTime) {
        if(ImGui::MenuItem(this->label.cstr())) {
            if(this->onClick) this->onClick();
        }
    }

    void UIMainWindow::render(float deltaTime) {
        ImGuiIO& io = ImGui::GetIO();
        
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        
        static const ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_NoDocking;
        
        ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y - ImGui::GetFrameHeight()));
        
        if(ImGui::Begin(this->name.cstr(), nullptr, window_flags)) {
            if (this->dockSpace && (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)) {
                ImGuiID dockspace_id = ImGui::GetID((this->name + "-dockspace").cstr());
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }
            
            this->renderChildren(deltaTime);
        }
        ImGui::End();
    }
    
    void UIWindow::render(float deltaTime) {
        static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
        
        static ImGuiDockNodeFlags dockspace_flags = ~ImGuiDockNodeFlags_PassthruCentralNode;
        
        if (!this->docking)
            window_flags |= ImGuiWindowFlags_NoDocking;
        
        ImGui::Begin(*name, &visible, window_flags);
        this->renderChildren(deltaTime);
        ImGui::End();
    }
    
    void UIDialog::render(float deltaTime) {
        static ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoCollapse;
        
        if(this->bOpenPopup) {
            ImGui::OpenPopup(*name);
            this->bOpenPopup = false;
        }
        
        ImVec2 parentSize = ImGui::GetWindowSize();
        ImVec2 windowPos = ImVec2((parentSize.x - size.x) * 0.5f, (parentSize.y - size.y) * 0.5f);
        ImGui::SetNextWindowPos(windowPos);
        ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
        
        if(this->modal) {
            if (ImGui::BeginPopupModal(*name, &visible, window_flags)) {
                this->renderChildren(deltaTime);
                ImGui::EndPopup();
            }
        }
        else {
            if (ImGui::BeginPopup(*name, window_flags)) {
                this->renderChildren(deltaTime);
                ImGui::EndPopup();
            }
        }
    }
    
    void UIDialog::show() {
        this->visible = true;
        this->bOpenPopup = true;
    }
    
    void UIDialog::hide() {
        this->visible = false;
        this->bOpenPopup = false;
    }
    
    void UIView::render(float deltaTime) {
        ImGuiChildFlags childFlags = 0;
        if(this->flags & Flags_Borders) childFlags |= ImGuiChildFlags_Borders;
        if(this->flags & Flags_ResizeX) childFlags |= ImGuiChildFlags_ResizeX;
        if(this->flags & Flags_ResizeY) childFlags |= ImGuiChildFlags_ResizeY;
        
        ImGui::BeginChild(*name, ImVec2(size.x, size.y), childFlags);
        this->renderChildren(deltaTime);
        ImGui::EndChild();
    }
    
    void UILayout::render(float deltaTime) {
        if(this->type == Type::Horizontal) {
            std::vector<UIWidget*> visibles;
            for(auto child : children) {
                if(child->visible) {
                    visibles.push_back(child);
                }
            }
            
            for(size_t i = 0; i < visibles.size(); i++){
                if(i > 0) {
                    ImGui::SameLine();
                }
                visibles[i]->render(deltaTime);
            }
        }
        else {
            this->renderChildren(deltaTime);
        }
    }
    
    void UIGroup::render(float deltaTime) {
        ImGui::BeginGroup();
        this->renderChildren(deltaTime);
        ImGui::EndGroup();
    }
    
    void UISeparator::render(float deltaTime) {
        ImGui::Separator();
    }
    
    void UIText::render(float deltaTime) {
        ImGui::Text("%s", *value.string());
    }
    
    void UILink::render(float deltaTime) {
        ImGui::TextLinkOpenURL(*label, *url);
    }
    
    void UIButton::render(float deltaTime) {
        if(ImGui::Button(this->label.cstr())) {
            if(this->onClick) this->onClick();
        }
    }
    
    void UIInput::render(float deltaTime) {
        ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_None;
        if(flags & Flags_Password) input_flags |= ImGuiInputTextFlags_Password;
        if(flags & Flags_ReadOnly) input_flags |= ImGuiInputTextFlags_ReadOnly;
        
        ImWidgets_Input(name, value, input_flags);
    }

    void UIEnum::render(float deltaTime) {
        int index = this->value;
        ImWidgets_Combo(name, index, items, ImGuiComboFlags_None);
        this->value = index;
    }
    
    void UISelector::render(float deltaTime) {
        ImWidgets_Selector(name, value, onSelect);
    }
    
    void UIFileSelector::render(float deltaTime) {
        ImWidgets_FileSelector(name, value, filters);
    }

    void UIFolderSelector::render(float deltaTime) {
        ImWidgets_FolderSelector(name, value);
    }
    
    void UICheckBox::render(float deltaTime) {
        this->changed = false;
        
        bool lastChecked = this->checked;
        
        ImGui::PushID(*name);
        ImGui::Checkbox(*label, &checked);
        ImGui::PopID();
        
        if(this->checked != lastChecked) {
            this->changed = true;
            if(this->onChange) {
                this->onChange();
            }
        }
    }
    
    void UICheckButton::render(float deltaTime) {
        this->changed = false;
        
        ImU32 color = ImGui::GetColorU32(this->checked ? ImGuiCol_ButtonActive : ImGuiCol_Button);
        
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushID(*name);
        if(ImGui::Button(*label)) {
            this->checked = !this->checked;
            this->changed = true;
            if(this->onChange) {
                this->onChange();
            }
        }
        ImGui::PopID();
        ImGui::PopStyleColor();
    }
    
    void UICheckList::render(float deltaTime) {
        ImGui::BeginGroup();
        this->renderChildren(deltaTime);
        ImGui::EndGroup();
        
        if(!this->multiCheck) {
            size_t checkedIndex = -1;
            {
                for (size_t index = 0; index < this->children.size(); index++) {
                    auto child = this->children[index];
                    if (child->changed && child->checked) {
                        checkedIndex = index;
                        break;
                    }
                }   
            }
            if(checkedIndex >= 0 && checkedIndex < this->children.size()) {
                for (size_t index = 0; index < this->children.size(); index++) {
                    auto child = this->children[index];
                    if(index != checkedIndex) {
                        child->checked = false;
                    }
                }
            }
        }
    }
    
    void UITreeNode::render(float deltaTime) {
        ImGuiTreeNodeFlags tree_node_flags =
            ImGuiTreeNodeFlags_SpanAvailWidth |
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_OpenOnDoubleClick |
                ImGuiTreeNodeFlags_NavLeftJumpsBackHere;
        
        if (this->children.empty())
            tree_node_flags |= ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_Leaf;
        if (this->selected)
            tree_node_flags |= ImGuiTreeNodeFlags_Selected;
        
        // ImGui::SetNextItemSelectionUserData((ImGuiSelectionUserData)(intptr_t)node);
        // ImGui::SetNextItemStorageID((ImGuiID)node->UID);
        
        if (ImGui::TreeNodeEx(*label, tree_node_flags))
        {
            this->renderChildren(deltaTime);
            ImGui::TreePop();
        }
        else if (ImGui::IsItemToggledOpen())
        {
            // TreeCloseAndUnselectChildNodes(node, selection);
        }
    }
    
    void UITable::render(float deltaTime) {
        if(ImGui::BeginTable(*name, mColumns.size())) {
            for(auto& col : mColumns) {
                ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_None;
                ImGui::TableSetupColumn(*col.label, column_flags);
            }
            
            size_t rowCount = this->children.size() % mColumns.size() == 0 ?
                this->children.size() / mColumns.size() :
                this->children.size() / mColumns.size() + 1;
            size_t colCount = mColumns.size();
            
            for(size_t row = 0; row < rowCount; row++) {
                for(size_t col = 0; col < colCount; col++) {
                    size_t index = row * colCount + col;
                    if(index >= this->children.size()) {
                        break;
                    }
                    auto child = this->children.at(index);
                    if(child->visible) {
                        child->render(deltaTime);
                    }
                }
            }
            
            ImGui::EndTable();
        }
    }
    
    void UITable::setColumn(const String& label, UITable::ColumnFlags flags) {
        auto& col = mColumns.emplace_back();
        col.label = label;
        col.flags = flags;
    }
    
    UIPropertiesView::~UIPropertiesView() {
        this->clearProperties();
    }
    
    void UIPropertiesView::render(float deltaTime) {
        int columns = this->insertable || this->removable ? 3 : 2;
        ImGuiTableFlags flags = ImGuiTableFlags_None;
        
        if(ImGui::BeginTable(*name, columns, flags)) {
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Value");
            if(columns > 2) {
                ImGui::TableSetupColumn("Operator", ImGuiTableColumnFlags_WidthFixed, 50);
            }
            
            auto iter = mProperties.begin();
            while(iter != mProperties.end()) {
                auto* prop = *iter;
                
                ImGui::TableNextRow();
                
                ImGui::TableNextColumn();
                if(prop->labelEditable) {
                    String editableName = String::format("%s-%d-label", *name, ImGui::TableGetRowIndex());
                    StringValue editableValue;
                    ImWidgets_Input(editableName, editableValue, ImGuiInputTextFlags_None);
                    prop->label = editableValue.string();
                }
                else {
                    ImGui::TextUnformatted(*prop->label);
                }
                
                ImGui::TableNextColumn();
                if(prop->valueEditable) {
                    prop->widget->render(deltaTime);
                }
                else {
                    ImGui::BeginDisabled();
                    prop->widget->render(deltaTime);
                    ImGui::EndDisabled();
                }
                
                if(columns > 2) {
                    ImGui::TableNextColumn();
                    if (this->removable) {
                        
                        ImGui::PushID(*String::format("%s-%d-REMOVE", *name, ImGui::TableGetRowIndex()));
                        if (ImGui::Button("-", ImVec2(30, 0))) {
                            ImGui::PopID();
                            iter = this->removeProperty(iter);
                            continue;
                        }
                        ImGui::PopID();
                    }
                }
                
                ++ iter;
            }
            
            if(this->insertable) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                // column 0
                ImGui::TableNextColumn();
                // column 1
                ImGui::TableNextColumn();
                // column 2
                ImGui::PushID(*String::format("%s-%d-ADD", *name, ImGui::TableGetRowIndex()));
                if (ImGui::Button("+", ImVec2(30, 0))) {
                    if (this->onInsert) this->onInsert();
                }
                ImGui::PopID();
            }
            
            ImGui::EndTable();
        }
    }
    
    UIPropertiesView::Property* UIPropertiesView::addString(const String& label, const String& value) {
        return this->addProperty<UIText>(label, value);
    }
    
    UIPropertiesView::Property* UIPropertiesView::addInput(const String& label, const String& value, UIInput::Flags flags) {
        return this->addProperty<UIInput>(label, value, flags);
    }
    
    UIPropertiesView::Property* UIPropertiesView::addEnum(const String& label, const std::vector<String>& list, u32_t index) {
        return this->addProperty<UIEnum>(label, list, index);
    }
    
    UIPropertiesView::Property* UIPropertiesView::addFile(const String& label, const std::map<String, String>& filters) {
        return this->addProperty<UIFileSelector>(label, filters);
    }

    UIPropertiesView::Property* UIPropertiesView::addFolder(const String& label) {
        return this->addProperty<UIFolderSelector>(label);
    }
    
    void UIPropertiesView::clearProperties() {
        _DeleteList(mProperties);
        _DeleteList(mChildren);
    }
    
    std::vector<UIPropertiesView::Property*>::iterator UIPropertiesView::removeProperty(std::vector<Property*>::iterator iter) {
        Property* prop = *iter;
        auto pos = std::find(mChildren.begin(), mChildren.end(), prop->widget);
        mChildren.erase(pos);
        _DeletePointer(prop->widget);
        _DeletePointer(prop);
        return mProperties.erase(iter);
    }
}
