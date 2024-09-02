#include "./widget.h"
#include "imgui/imgui.h"

namespace eokas::datapot {
    Widget::Widget() { }
    
    Widget::~Widget() {
    }
    
    void Widget::init() { }
    
    void Widget::quit() { }
    
    void Widget::render(float deltaTime) { }
    
    ContainerWidget::ContainerWidget() { }
    
    ContainerWidget::~ContainerWidget() {
        _DeleteList(children);
    }
    
    void ContainerWidget::renderChildren(float deltaTime) {
        for(Widget* child : children) {
            if(child->visible) {
                child->render(deltaTime);
            }
        }
    }
    
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
        static const char* content_window_name = "##niagara_perf_viewer_main_window";
        static const ImGuiWindowFlags content_window_flags =
            ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoNavFocus;
        ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
        ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y - ImGui::GetFrameHeight()));
        if (ImGui::Begin(content_window_name, nullptr, content_window_flags))
        {
            this->renderChildren(deltaTime);
            ImGui::End();
        }
    }
}