#include "./widget.h"
#include "imgui/imgui.h"
#include "native/utils.h"

namespace eokas::datapot {
    UIWidget::UIWidget() { }
    
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
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
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
        
        ImGui::Begin(this->name.cstr(), &visible, window_flags);
        this->renderChildren(deltaTime);
        ImGui::End();
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
    
    void UIDialog::render(float deltaTime) {
        static ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMove;
        
        if(this->bOpenPopup) {
            ImGui::OpenPopup(this->name.cstr());
            this->bOpenPopup = false;
        }
        
        if(this->modal) {
            if (ImGui::BeginPopupModal(this->name.cstr(), &visible, window_flags)) {
                this->renderChildren(deltaTime);
                ImGui::EndPopup();
            }
        }
        else {
            if (ImGui::BeginPopup(this->name.cstr(), window_flags)) {
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
    
    void UIText::render(float deltaTime) {
        ImGui::Text(*content);
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
    
    void UIFieldText::render(float deltaTime) {
        ImGui::Columns(2, *group);
        ImGui::TextUnformatted(*label);
        ImGui::NextColumn();
        ImGui::TextUnformatted(*value);
        ImGui::Columns();
    }
    
    void UIFieldInput::render(float deltaTime) {
        char buffer[255] = {0};
        String valueStr = value.string();
        memcpy_s(buffer, 255, valueStr.cstr(), valueStr.length());
        
        ImGui::Columns(2, *group);
        ImGui::TextUnformatted(*label);
        ImGui::NextColumn();
        ImGui::InputText(*inputName, buffer, sizeof(buffer));
        ImGui::Columns();
        
        value = buffer;
    }
    
    void UIFieldDirectory::render(float deltaTime) {
        ImGui::Columns(2, *group);
        ImGui::TextUnformatted(*label);
        ImGui::NextColumn();
        ImGui::Text(*value);
        ImGui::SameLine();
        if(ImGui::Button(" --- ")) {
            OpenDirectoryDialog(this->value, "");
        }
        ImGui::Columns();
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
}