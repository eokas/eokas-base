#pragma once

#include "base/main.h"

namespace eokas::datapot {
    enum class WidgetType {
        None, MenuBar, Menu, MenuItem,
    };
    
    struct Widget {
        Widget();
        virtual ~Widget();
        
        virtual void init();
        virtual void quit();
        virtual void render(float deltaTime);
        
        template<typename ChildType, typename... Args>
        ChildType* addChild(Args&&... args) {
            ChildType* child = new ChildType(std::forward<Args>(args)...);
            this->children.push_back(child);
            return child;
        }
        
    protected:
        std::vector<Widget*> children = {};
    };
    
    struct MainMenuBar : public Widget {
        virtual void render(float deltaTime) override;
    };
    
    struct MenuBar : public Widget {
        virtual void render(float deltaTime) override;
    };
    
    struct Menu : public Widget {
        String label = "";
        bool enabled = true;
        
        Menu(const String& label, bool enabled = true)
            :label(label)
            ,enabled(enabled)
        {}
        
        virtual void render(float deltaTime) override;
    };
    
    struct MenuItem : public Widget {
        String label = "";
        std::function<void()> onClick = {};
        
        MenuItem(const String label, const std::function<void()>& onClick = {})
            : label(label)
            , onClick(onClick)
        {}
        
        virtual void render(float deltaTime) override;
    };
    
    struct MainWindow : public Widget {
        virtual void render(float deltaTime) override;
    };
}

