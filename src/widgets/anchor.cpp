#include "./anchor.h"

namespace eokas {
    UIAnchor::UIAnchor(UIAnchor* parent)
        : mParent(parent)
        , mPivot()
        , mOffset()
        , mBounds() {
        
    }
    
    UIAnchor::~UIAnchor() { }
    
    const Vector2& UIAnchor::offset() const {
        return mOffset;
    }
    
    void UIAnchor::offset(const Vector2& val) {
        mOffset = val;
    }
    
    Vector2 UIAnchor::position() const {
        return mParent != nullptr ? mParent->position() + mOffset : mOffset;
    }
    
    void UIAnchor::position(const Vector2& val) {
        mOffset = mParent != nullptr ? val - mParent->position() : val;
    }
    
    const Vector2& UIAnchor::pivot() const {
        return mPivot;
    }
    
    void UIAnchor::pivot(const Vector2& val) {
        mPivot = val;
    }
    
    const Bounds2& UIAnchor::bounds() const {
        return mBounds;
    }
    
    void UIAnchor::bounds(const Bounds2& val) {
        mBounds = val;
    }
    
    Vector2 UIAnchor::size() const {
        return mBounds.size();
    }
    
    void UIAnchor::size(const Vector2& val) {
        mBounds.size(val);
    }
    
    f32_t UIAnchor::left() const {
        f32_t base = mParent != nullptr ? mParent->mBounds.min.x : 0;
        return (mOffset.x + mBounds.min.x) - base;
    }
    
    void UIAnchor::left(f32_t val) {
        f32_t current = this->left();
        f32_t delta = val - current;
        mBounds.min.x += delta;
    }
    
    f32_t UIAnchor::right() const {
        f32_t base = mParent != nullptr ? mParent->mBounds.max.x : 0;
        return base - (mOffset.x + mBounds.max.x);
    }
    
    void UIAnchor::right(f32_t val) {
        f32_t current = this->right();
        f32_t delta = val - current;
        mBounds.max.x += delta;
    }
    
    f32_t UIAnchor::top() const {
        f32_t base = mParent != nullptr ? mParent->mBounds.min.y : 0;
        return (mOffset.y + mBounds.min.y) - base;
    }
    
    void UIAnchor::top(f32_t val) {
        f32_t current = this->top();
        f32_t delta = val - current;
        mBounds.min.y += delta;
    }
    
    f32_t UIAnchor::bottom() const {
        f32_t base = mParent != nullptr ? mParent->mBounds.max.y : 0;
        return base - (mOffset.y + mBounds.max.y);
    }
    
    void UIAnchor::bottom(f32_t val) {
        f32_t current = this->bottom();
        f32_t delta = val - current;
        mBounds.max.y += delta;
    }
}
