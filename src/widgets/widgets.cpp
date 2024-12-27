#include "./widgets.h"

namespace eokas {
    static u32_t gsSerialNumber = 0;
    
    UIWidget::UIWidget(const String& name)
        : mName(name)
        , mID(String::format("%s%lu", name.cstr(), ++gsSerialNumber))
        , mAnchor()
        , mShape() {
        
    }
    
    UIWidget::~UIWidget() {
        mShape.clear();
    }
    
    UIShape* UIWidget::render() {
        if(mDirty) {
            auto offset = mAnchor.offset();
            auto size = mAnchor.size();
            auto pivot = mAnchor.pivot();
            
            float l = offset.x + size.x * (0.0f - pivot.x);
            float r = offset.x + size.x * (1.0f - pivot.x);
            float t = offset.y + size.y * (0.0f - pivot.y);
            float b = offset.y + size.y * (1.0f - pivot.y);
            
            auto& geometry = mShape.geometries.emplace_back();
            geometry.positions.push_back(Vector3(l, t, 0));
            geometry.positions.push_back(Vector3(r, t, 0));
            geometry.positions.push_back(Vector3(r, b, 0));
            geometry.positions.push_back(Vector3(l, b, 0));
            
            mDirty = false;
        }
        
        return &mShape;
    }
}
