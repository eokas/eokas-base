#include "./widget.h"
#include "imgui/imgui.h"

namespace eokas::datapot {
    Widget::Widget() { }
    
    Widget::~Widget() {
        _DeleteList(children);
    }
    
    void Widget::init() { }
    
    void Widget::quit() { }
    
    void Widget::render(float deltaTime) { }
    
    void MainMenuBar::render(float deltaTime) {
        if (ImGui::BeginMainMenuBar()) {
            for(Widget* child : children) {
                child->render(deltaTime);
            }
            ImGui::EndMainMenuBar();
        }
    }
    
    void MenuBar::render(float deltaTime) {
        if (ImGui::BeginMenuBar()) {
            for(Widget* child : children) {
                child->render(deltaTime);
            }
            ImGui::EndMenuBar();
        }
    }
    
    void Menu::render(float deltaTime) {
        if(ImGui::BeginMenu(this->label.cstr(), this->enabled)) {
            for(Widget* child : children) {
                child->render(deltaTime);
            }
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
            for(Widget* child : children) {
                child->render(deltaTime);
            }
            
            ImGui::End();
        }
    }
}