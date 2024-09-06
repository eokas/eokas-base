#include "./widget.h"
#include "imgui/imgui.h"
#include "native/utils.h"

namespace eokas::datapot {
    Widget::Widget() { }
    
    Widget::~Widget() {
    }
    
    void Widget::init() { }
    
    void Widget::quit() { }
    
    void Widget::render(float deltaTime) { }
    
    void MainMenuBar::render(float deltaTime) {
        if (ImGui::BeginMainMenuBar()) {
            this->renderChildren(deltaTime);
            ImGui::EndMainMenuBar();
        }
    }
    
    void MenuBar::render(float deltaTime) {
        if (ImGui::BeginMenuBar()) {
            this->renderChildren(deltaTime);
            ImGui::EndMenuBar();
        }
    }
    
    void Menu::render(float deltaTime) {
        if(ImGui::BeginMenu(this->label.cstr(), this->enabled)) {
            this->renderChildren(deltaTime);
            ImGui::EndMenu();
        }
    }
    
    void MenuItem::render(float deltaTime) {
        if(ImGui::MenuItem(this->label.cstr())) {
            this->onClick();
        }
    }

    void MainWindow::render(float deltaTime) {
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
    
    void Window::render(float deltaTime) {
        static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
        
        static ImGuiDockNodeFlags dockspace_flags = ~ImGuiDockNodeFlags_PassthruCentralNode;
        
        if (!this->docking)
            window_flags |= ImGuiWindowFlags_NoDocking;
        
        ImGui::Begin(this->name.cstr(), &visible, window_flags);
        this->renderChildren(deltaTime);
        ImGui::End();
    }
    
    void Dialog::render(float deltaTime) {
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
    
    void Dialog::show() {
        this->visible = true;
        this->bOpenPopup = true;
    }
    
    void Dialog::hide() {
        this->visible = false;
        this->bOpenPopup = false;
    }
    
    void Text::render(float deltaTime) {
        ImGui::Text(*content);
    }
    
    void Link::render(float deltaTime) {
        ImGui::TextLinkOpenURL(*label, *url);
    }
    
    void Button::render(float deltaTime) {
        if(ImGui::Button(this->label.cstr())) {
            this->onClick();
        }
    }
    
    void FieldText::render(float deltaTime) {
        ImGui::Columns(2, "##field");
        ImGui::TextUnformatted(*label);
        ImGui::NextColumn();
        ImGui::TextUnformatted(*text);
        ImGui::Columns();
    }
    
    void FieldInput::render(float deltaTime) {
        static char buffer[255] = { 0 };
        
        ImGui::Columns(2, "##field");
        ImGui::TextUnformatted(*label);
        ImGui::NextColumn();
        ImGui::InputText(" ", buffer, sizeof(buffer));
        ImGui::Columns();
        
        value = buffer;
    }
    
    void FieldDirectory::render(float deltaTime) {
        ImGui::Columns(2, "##field");
        ImGui::TextUnformatted(*label);
        ImGui::NextColumn();
        ImGui::Text(*value);
        ImGui::SameLine();
        if(ImGui::Button(" --- ")) {
            OpenDirectoryDialog(this->value, "");
        }
        ImGui::Columns();
    }
    
    void TreeNode::render(float deltaTime) {
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