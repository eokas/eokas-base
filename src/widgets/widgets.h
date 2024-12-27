#ifndef _EOKAS_WIDGETS_WIDGETS_H_
#define _EOKAS_WIDGETS_WIDGETS_H_

#include "./header.h"
#include "./anchor.h"
#include "./renderer.h"

namespace eokas {
    class UIWidget {
    public:
        UIWidget(const String& name);
        virtual ~UIWidget();
        
        const String& id() const { return mID; }
        const String& name() const { return mName; }
        
        const UIAnchor& anchor() const { return mAnchor; }
        UIAnchor& anchor() { return mAnchor; }
        
        bool dirty() { return mDirty; }
        void dirty(bool val) { mDirty = val; }
        
        virtual UIShape* render();
    
    protected:
        String mID;
        String mName;
        UIAnchor mAnchor;
        bool mDirty;
        UIShape mShape;
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
            ChildBase*& child = this->mChildren.emplace_back();
            child = new Child(std::forward<Args>(args)...);
            child->mParent = &this->mAnchor;
            return (Child*)child;
        }
        
        const std::vector<ChildBase*>& children() const { return mChildren; }
        
        void clear() {
            _DeleteList(mChildren);
        }
    
    protected:
        std::vector<ChildBase*> mChildren = {};
    };
    
    class UIViewport : public UIWidget { };
    
    class UIImage : public UIWidget { };
    
    class UIText : public UIWidget { };
    
    class UIButton : public UIWidget { };
    
    class UIToggle : public UIWidget { };
    
    class UISelector : public UIWidget { };
    
    class UIVerticalListView : public UIContainer<UIWidget> { };
    
    class UIHorizontalListView : public UIContainer<UIWidget> { };
    
    class UIGridView : public UIContainer<UIWidget> { };
    
    class UITableView : public UIContainer<UIWidget> { };
    
    class UITreeView : public UIContainer<UIWidget> { };
}

#endif//_EOKAS_WIDGETS_WIDGETS_H_