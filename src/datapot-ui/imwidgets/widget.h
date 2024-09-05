#pragma once

#include "base/main.h"

namespace eokas::datapot {
    class Widget {
    public:
        bool visible = true;
        
        Widget();
        virtual ~Widget();
        
        virtual void init();
        virtual void quit();
        virtual void render(float deltaTime);
    };
    
    class ContainerWidget : public Widget {
    public:
        ContainerWidget();
        virtual ~ContainerWidget();
        
        template<typename Child, typename... Args>
        Child* add(Args&&... args) {
            Widget*& child = this->children.emplace_back();
            child = new Child(std::forward<Args>(args)...);
            return (Child*)child;
        }
        
    protected:
        std::vector<Widget*> children = {};
        
        void renderChildren(float deltaTime);
    };
    
    class MainMenuBar : public ContainerWidget {
    public:
        virtual void render(float deltaTime) override;
    };
    
    class MenuBar : public ContainerWidget {
    public:
        virtual void render(float deltaTime) override;
    };
    
    class Menu : public ContainerWidget {
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
    
    class MainWindow : public ContainerWidget {
    public:
        String name = "Window";
        bool dockSpace = false;
        
        MainWindow(const String& name, bool dockSpace = false)
            : ContainerWidget()
            , name(name)
            , dockSpace(dockSpace) {}
        
        virtual void render(float deltaTime) override;
    };
    
    class Window :public ContainerWidget {
    public:
        String name = "Window";
        bool docking = false;
        
        Window(const String& name, bool docking = false)
            : name(name)
            , docking(docking) { }
        
        virtual void render(float deltaTime) override;
    };
    
    class Dialog :public ContainerWidget {
    public:
        String name = "Dialog";
        bool modal = true;
        
        Dialog(const String& name, bool modal = true)
            : name(name)
            , modal(modal) {
        }
        
        virtual void render(float deltaTime) override;
        
        void open();
        
    private:
        bool isOpened = false;
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
}

