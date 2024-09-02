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
        
        template<typename ChildType, typename... Args>
        ChildType* addChild(Args&&... args) {
            ChildType* child = new ChildType(std::forward<Args>(args)...);
            this->children.push_back(child);
            return child;
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
            ,enabled(enabled)
        {}
        
        virtual void render(float deltaTime) override;
    };
    
    class MenuItem : public Widget {
    public:
        String label = "";
        std::function<void()> onClick = {};
        
        MenuItem(const String label, const std::function<void()>& onClick = {})
            : label(label)
            , onClick(onClick)
        {}
        
        virtual void render(float deltaTime) override;
    };
    
    class MainWindow : public ContainerWidget {
    public:
        virtual void render(float deltaTime) override;
    };
}

