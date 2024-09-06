#pragma once

#include "base/main.h"

namespace eokas::datapot {
    class Widget;
    
    template<typename ChildBase>
    class ContainerWidget;
    
    class MainMenuBar;
    class MenuBar;
    class Menu;
    class MenuItem;
    
    
    class Widget {
    public:
        bool visible = true;
        
        Widget();
        virtual ~Widget();
        
        virtual void init();
        virtual void quit();
        virtual void render(float deltaTime);
    };
    
    template<typename ChildBase>
    class ContainerWidget : public Widget {
    public:
        ContainerWidget() = default;
        
        virtual ~ContainerWidget() override {
            _DeleteList(children);
        }
        
        template<typename Child, typename... Args>
        Child* add(Args&&... args) {
            ChildBase*& child = this->children.emplace_back();
            child = new Child(std::forward<Args>(args)...);
            return (Child*)child;
        }
        
    protected:
        std::vector<ChildBase*> children = {};
        
        void renderChildren(float deltaTime) {
            for(Widget* child : children) {
                if(child->visible) {
                    child->render(deltaTime);
                }
            }
        }
    };
    
    class MainMenuBar : public ContainerWidget<Menu> {
    public:
        virtual void render(float deltaTime) override;
    };
    
    class MenuBar : public ContainerWidget<Menu> {
    public:
        virtual void render(float deltaTime) override;
    };
    
    class Menu : public ContainerWidget<MenuItem> {
    public:
        String label = "";
        bool enabled = true;
        
        Menu(const String& label, bool enabled = true)
            :label(label)
            ,enabled(enabled) {}
        
        virtual void render(float deltaTime) override;
    };
    
    class MenuItem : public Widget {
    public:
        String label = "";
        std::function<void()> onClick = {};
        
        MenuItem(const String label, const std::function<void()>& onClick = {})
            : label(label)
            , onClick(onClick) {}
        
        virtual void render(float deltaTime) override;
    };
    
    class MainWindow : public ContainerWidget<Widget> {
    public:
        String name = "Window";
        bool dockSpace = false;
        
        MainWindow(const String& name, bool dockSpace = false)
            : ContainerWidget()
            , name(name)
            , dockSpace(dockSpace) {}
        
        virtual void render(float deltaTime) override;
    };
    
    class Window :public ContainerWidget<Widget> {
    public:
        String name = "Window";
        bool docking = false;
        
        Window(const String& name, bool docking = false)
            : name(name)
            , docking(docking) { }
        
        virtual void render(float deltaTime) override;
        
    protected:
        bool isOpened = true;
    };
    
    class Dialog :public ContainerWidget<Widget> {
    public:
        String name = "Dialog";
        bool modal = true;
        
        Dialog(const String& name, bool modal = true)
            : name(name)
            , modal(modal) {
        }
        
        virtual void render(float deltaTime) override;
        
        void show();
        void hide();
        
    private:
        bool bOpenPopup = false;
    };
    
    class Text : public Widget {
    public:
        String content = "";
        
        Text(const String& content)
            : content(content) { }
            
        virtual void render(float deltaTime) override;
    };
    
    class Link : public Widget {
    public:
        String label = "";
        String url = "";
        
        Link(const String& label, const String& url)
            : label(label), url(url) { }
        
        virtual void render(float deltaTime) override;
    };
    
    class Button : public Widget {
    public:
        String label = "Button";
        std::function<void()> onClick;
        
        Button(const String& label)
            : label(label) { }
        
        virtual void render(float deltaTime) override;
    };
    
    class FieldText : public Widget {
    public:
        String label = "";
        String text = "";
        
        FieldText(const String& label, const String& text)
            : label(label)
            , text(text) { }
        
        virtual void render(float deltaTime) override;
    };
    
    class FieldInput : public Widget {
    public:
        String label;
        StringValue value;
        
        FieldInput(const String& label)
            : label(label) { }
        
        virtual void render(float deltaTime) override;
    };
    
    class FieldDirectory : public Widget {
    public:
        String label;
        String value;
        
        FieldDirectory(const String& label)
            : label(label) { }
            
        virtual void render(float deltaTime) override;
    };
    
    class TreeNode : public ContainerWidget<TreeNode> {
    public:
        String label = "TreeNode";
        bool selected = false;
        
        virtual void render(float deltaTime) override;
    };
}

