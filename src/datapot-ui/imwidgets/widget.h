#pragma once

#include "base/main.h"

namespace eokas::datapot {
    class UIWidget;
    
    template<typename ChildBase>
    class UIContainer;
    
    class UIMainMenuBar;
    class UIMenuBar;
    class UIMenu;
    class UIMenuItem;
    
    
    class UIWidget {
    public:
        bool visible = true;
        
        UIWidget();
        virtual ~UIWidget();
        
        virtual void init();
        virtual void quit();
        virtual void render(float deltaTime);
    };
    
    template<typename ChildBase>
    class UIContainer : public UIWidget {
    public:
        UIContainer() = default;
        
        virtual ~UIContainer() override {
            this->clear();
        }
        
        template<typename Child, typename... Args>
        Child* add(Args&&... args) {
            ChildBase*& child = this->children.emplace_back();
            child = new Child(std::forward<Args>(args)...);
            return (Child*)child;
        }
        
        void clear() {
            _DeleteList(children);
        }
        
    protected:
        std::vector<ChildBase*> children = {};
        
        void renderChildren(float deltaTime) {
            for(UIWidget* child : children) {
                if(child->visible) {
                    child->render(deltaTime);
                }
            }
        }
    };

    class UIField : public UIWidget {
    public:
        StringValue value;

        UIField(const StringValue& value)
            : value(value)
        { }
    };
    
    class UIMainMenuBar : public UIContainer<UIMenu> {
    public:
        virtual void render(float deltaTime) override;
    };
    
    class UIMenuBar : public UIContainer<UIMenu> {
    public:
        virtual void render(float deltaTime) override;
    };
    
    class UIMenu : public UIContainer<UIMenuItem> {
    public:
        String label = "";
        bool enabled = true;
        
        UIMenu(const String& label, bool enabled = true)
            :label(label)
            ,enabled(enabled) {}
        
        virtual void render(float deltaTime) override;
    };
    
    class UIMenuItem : public UIWidget {
    public:
        String label = "";
        std::function<void()> onClick = {};
        
        UIMenuItem(const String label, const std::function<void()>& onClick = {})
            : label(label)
            , onClick(onClick) {}
        
        virtual void render(float deltaTime) override;
    };
    
    class UIMainWindow : public UIContainer<UIWidget> {
    public:
        String name = "Window";
        bool dockSpace = false;
        
        UIMainWindow(const String& name, bool dockSpace = false)
            : UIContainer()
            , name(name)
            , dockSpace(dockSpace) {}
        
        virtual void render(float deltaTime) override;
    };
    
    class UIWindow :public UIContainer<UIWidget> {
    public:
        String name = "Window";
        bool docking = false;
        
        UIWindow(const String& name, bool docking = false)
            : name(name)
            , docking(docking) { }
        
        virtual void render(float deltaTime) override;
    };
    
    class UIDialog :public UIContainer<UIWidget> {
    public:
        String name = "Dialog";
        bool modal = true;
        
        UIDialog(const String& name, bool modal = true)
            : name(name)
            , modal(modal) {
        }
        
        virtual void render(float deltaTime) override;
        
        void show();
        void hide();
        
    private:
        bool bOpenPopup = false;
    };
    
    class UIView :public UIContainer<UIWidget> {
    public:
        using Flags = u32_t;
        enum Flags_ {
            Flags_None = 0,
            Flags_Borders = 1 << 0,
            Flags_ResizeX = 1 << 1,
            Flags_ResizeY = 1 << 2,
        };
        
        String name;
        Vector2 size;
        Flags flags;
        
        UIView(const String& name, const Vector2& size, Flags flags = Flags_None)
            : name(name), size(size), flags(flags) {};
        
        virtual void render(float deltaTime) override;
    };
    
    class UILayout :public UIContainer<UIWidget> {
    public:
        enum Type {
            Horizontal, Vertical
        };
        Type type;
        
        UILayout(Type type) : type(type) {}
        
        virtual void render(float deltaTime) override;
    };
    
    class UISeparator : public UIWidget {
    public:
        virtual void render(float deltaTime) override;
    };
    
    class UIText : public UIField {
    public:
        UIText(const StringValue& content)
            : UIField(content) { }
            
        virtual void render(float deltaTime) override;
    };
    
    class UILink : public UIWidget {
    public:
        String label = "";
        String url = "";
        
        UILink(const String& label, const String& url)
            : label(label), url(url) { }
        
        virtual void render(float deltaTime) override;
    };
    
    class UIButton : public UIWidget {
    public:
        String label = "Button";
        std::function<void()> onClick;
        
        UIButton(const String& label)
            : label(label) { }
        
        virtual void render(float deltaTime) override;
    };
    
    class UIInput : public UIField {
    public:
        bool password;
        
        UIInput(const StringValue& value, bool password)
            : UIField(value), password(password) {}
            
        virtual void render(float deltaTime) override;
    };
    
    class UIEnum : public UIField {
    public:
        std::vector<String> items;
        
        UIEnum(const std::vector<String>& items, u32_t index)
            : UIField(index), items(items) { }
        
        virtual void render(float deltaTime) override;
    };

    class UIDirectorySelector : public UIField {
    public:
        UIDirectorySelector()
            : UIField("") { }

        virtual void render(float deltaTime) override;
    };

    class UIListView : public UIContainer<UIWidget> {
    public:
        String label = "";
        Vector2 size = Vector2::ZERO;
        
        UIListView(const String& label, const Vector2& size)
            : label(label)
            , size(size) { }
        
        virtual void render(float deltaTime) override;
        
        float width;
        float height;
    };
    
    class UITreeNode : public UIContainer<UITreeNode> {
    public:
        String label = "TreeNode";
        bool selected = false;
        
        virtual void render(float deltaTime) override;
    };
    
    class UITable : public UIContainer<UIWidget> {
    public:
        enum TableFlags_ {
            TableFlags_None,
        };
        using TableFlags = u32_t;
        
        enum ColumnFlags_ {
            ColumnFlags_None,
        };
        using ColumnFlags = u32_t;
        
        struct Column {
            String label = "";
            ColumnFlags flags = ColumnFlags_None;
        };
        
        String name;
        TableFlags flags;
        
        
        UITable(const String& name, TableFlags flags = TableFlags_None)
            : name(name), flags(flags) {}
        
        virtual void render(float deltaTime) override;
        
        void setColumn(const String& label, ColumnFlags flags);
        
    private:
        std::vector<Column> mColumns = {};
    };
    
    class UIPropertiesView : public UIWidget {
    public:
        struct Property {
            String label = "";
            StringValue& value;
            UIField* widget = nullptr;

            Property(const String& label, UIField* widget)
                : label(label), value(widget->value), widget(widget)
            { }
        };
        
        String name;
        
        UIPropertiesView(const String& name)
            : name(name), mProperties(), mChildren() {}

        virtual ~UIPropertiesView() {
            _DeleteList(mChildren);
        }
        
        virtual void render(float deltaTime) override;
        
        template<typename FieldWidget, typename... Args>
        Property* addProperty(const String& label, Args&&... args) {
            auto* widget = new FieldWidget(std::forward<Args>(args)...);
            mChildren.push_back(widget);

            auto& prop = mProperties.emplace_back(label, widget);
            return &prop;
        }
        
        Property* addString(const String& label, const String& value);
        Property* addInput(const String& label, const String& value, bool password);
        Property* addEnum(const String& label, const std::vector<String>& list, u32_t index);
        Property* addDirectory(const String& label);
        
    private:
        std::vector<Property> mProperties;
        std::vector<UIWidget*> mChildren;
    };
}

