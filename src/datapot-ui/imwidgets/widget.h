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
        String name = "";
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
        bool dockSpace = false;
        
        UIMainWindow(bool dockSpace = false)
            : dockSpace(dockSpace) {}
        
        virtual void render(float deltaTime) override;
    };
    
    class UIWindow : public UIContainer<UIWidget> {
    public:
        bool docking = false;
        
        UIWindow(bool docking = false)
            : docking(docking) { }
        
        virtual void render(float deltaTime) override;
    };
    
    class UIDialog : public UIContainer<UIWidget> {
    public:
        Vector2 size = Vector2(100, 80);
        bool modal = true;
        
        UIDialog(const Vector2& size, bool modal = true)
            : size(size), modal(modal) {
        }
        
        virtual void render(float deltaTime) override;
        
        void show();
        void hide();
        
    private:
        bool bOpenPopup = false;
    };
    
    class UIView : public UIContainer<UIWidget> {
    public:
        using Flags = u32_t;
        enum Flags_ {
            Flags_None = 0,
            Flags_Borders = 1 << 0,
            Flags_ResizeX = 1 << 1,
            Flags_ResizeY = 1 << 2,
        };
        
        Vector2 size;
        Flags flags;
        
        UIView(const Vector2& size, Flags flags = Flags_None)
            : size(size), flags(flags) {};
        
        virtual void render(float deltaTime) override;
    };
    
    class UILayout : public UIContainer<UIWidget> {
    public:
        enum Type {
            Horizontal, Vertical
        };
        Type type;
        
        UILayout(Type type) : type(type) {}
        
        virtual void render(float deltaTime) override;
    };
    
    class UIGroup : public UIContainer<UIWidget> {
    public:
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
        enum Flags_ {
            Flags_None = 0,
            Flags_Password = 1 << 0,
            Flags_ReadOnly = 1 << 1
        };
        using Flags = u32_t;

        Flags flags;
        
        UIInput(const StringValue& value, Flags flags = Flags_None)
            : UIField(value), flags(flags) {}
            
        virtual void render(float deltaTime) override;
    };
    
    class UIEnum : public UIField {
    public:
        std::vector<String> items;
        
        UIEnum(const std::vector<String>& items, u32_t index)
            : UIField(index), items(items) { }
        
        virtual void render(float deltaTime) override;
    };
    
    class UISelector : public UIField {
    public:
        std::function<void(StringValue&)> onSelect;
        
        UISelector(std::function<void(StringValue&)> onSelect = {})
            : UIField(""), onSelect(onSelect) { }
        
        virtual void render(float deltaTime) override;
    };
    
    class UIFileSelector : public UIField {
    public:
        std::map<String, String> filters;
        
        UIFileSelector(const std::map<String, String>& filters)
            : UIField(""), filters(filters) { }
        
        virtual void render(float deltaTime) override;
    };

    class UIFolderSelector : public UIField {
    public:
        UIFolderSelector()
            : UIField("") { }

        virtual void render(float deltaTime) override;
    };
    
    class UICheckable : public UIWidget {
    public:
        bool checked = false;
        bool changed = false;
        std::function<void()> onChange = {};
    };
    
    class UICheckBox : public UICheckable {
    public:
        String label = "";
        
        UICheckBox(const String& label)
            : label(label) { }
            
        virtual void render(float deltaTime) override;
    };
    
    class UICheckButton : public UICheckable {
    public:
        String label = "";
        
        UICheckButton(const String& label)
            : label(label) { }
            
        virtual void render(float deltaTime) override;
    };
    
    class UISwitcher : public UICheckable {
    public:
        virtual void render(float deltaTime) override;
    };

    class UICheckList : public UIContainer<UICheckable> {
    public:
        bool multiCheck = false;
        
        UICheckList(bool multiCheck = false)
            : multiCheck(multiCheck)
        { }
        
        virtual void render(float deltaTime) override;
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
            
            bool labelEditable = false;
            bool valueEditable = true;
            
            Property(const String& label, UIField* widget)
                : label(label), value(widget->value), widget(widget)
            { }
        };
        
        bool insertable = false;
        bool removable = false;
        std::function<void()> onInsert;
        
        UIPropertiesView()
            : mProperties(), mChildren() {}

        virtual ~UIPropertiesView();
        
        virtual void render(float deltaTime) override;
        
        template<typename FieldWidget, typename... Args>
        Property* addProperty(const String& label, Args&&... args) {
            auto* widget = new FieldWidget(std::forward<Args>(args)...);
            mChildren.push_back(widget);

            auto* prop = new Property(label, widget);
            mProperties.push_back(prop);
            return prop;
        }
        
        Property* addString(const String& label, const String& value);
        Property* addInput(const String& label, const String& value, UIInput::Flags flags = UIInput::Flags_None);
        Property* addEnum(const String& label, const std::vector<String>& list, u32_t index);
        Property* addFile(const String& label, const std::map<String, String>& filters);
        Property* addFolder(const String& label);
        
    private:
        std::vector<Property*> mProperties;
        std::vector<UIWidget*> mChildren;
        
        void clearProperties();
        std::vector<Property*>::iterator removeProperty(std::vector<Property*>::iterator iter);
    };
}

