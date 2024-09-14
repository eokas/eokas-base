#include "./widget.h"
#include "imgui/imgui.h"
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
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMove;
        
        if(this->bOpenPopup) {
            ImGui::OpenPopup(*name);
            this->bOpenPopup = false;
        }
        
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
    
    void UISeparator::render(float deltaTime) {
        ImGui::Separator();
    }
    
    void UIText::render(float deltaTime) {
        ImGui::Text(*value.string());
    }
    
    void UILink::render(float deltaTime) {
        ImGui::TextLinkOpenURL(*label, *url);
    }
    
    void UIButton::render(float deltaTime) {
        auto size = ImGui::GetContentRegionAvail();
        ImGui::SetNextItemWidth(size.x);
        if(ImGui::Button(this->label.cstr())) {
            if(this->onClick) this->onClick();
        }
    }
    
    void UIInput::render(float deltaTime) {
        char buffer[255] = {0};
        String valueStr = value.string();
        u32_t len = valueStr.length() < 255 ? valueStr.length() : 255;
        memcpy(buffer, valueStr.cstr(), len);
        
        ImGui::InputText(*name, buffer, sizeof(buffer));

        value = buffer;
    }

    void UIEnum::render(float deltaTime) {
        std::vector<const char*> rawItems;
        for(auto& item : items) {
            rawItems.push_back(*item);
        }

        int index = this->value;
        ImGui::Combo(*name, &index, rawItems.data(), (int)rawItems.size());
        this->value = index;
    }

    void UIDirectorySelector::render(float deltaTime) {
        ImGui::Text(*value.string());
        ImGui::SameLine();
        if(ImGui::Button(" --- ")) {
            String selectedPath;
            OpenDirectoryDialog(selectedPath, "");
            this->value = selectedPath;
        }
    }
    
    void UIListView::render(float deltaTime) {
        if(ImGui::BeginListBox(*label, ImVec2(size.x, size.y))) {
            this->renderChildren(deltaTime);
            ImGui::EndListBox();
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
    
    void UIPropertiesView::render(float deltaTime) {
        ImGui::Columns(2, *name);
        for(auto* prop : mProperties) {
            ImGui::TextUnformatted(*prop->label);
            ImGui::NextColumn();
            prop->widget->render(deltaTime);
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
    }
    
    UIPropertiesView::Property* UIPropertiesView::addString(const String& label, const String& value) {
        return this->addProperty<UIText>(label, value);
    }
    
    UIPropertiesView::Property* UIPropertiesView::addInput(const String& label, const String& value, bool password) {
        return this->addProperty<UIInput>(label, value, password);
    }
    
    UIPropertiesView::Property* UIPropertiesView::addEnum(const String& label, const std::vector<String>& list, u32_t index) {
        return this->addProperty<UIEnum>(label, list, index);
    }

    UIPropertiesView::Property* UIPropertiesView::addDirectory(const String& label) {
        return this->addProperty<UIDirectorySelector>(label);
    }
}