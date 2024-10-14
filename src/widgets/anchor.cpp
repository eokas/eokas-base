#include "./anchor.h"

namespace eokas {
    Anchor::Anchor(Anchor* parent)
        : mParent(parent)
        , mPivot()
        , mOffset()
        , mBounds() {
        
    }
    
    Anchor::~Anchor() { }
    
    const Vector2& Anchor::offset() const {
        return mOffset;
    }
    
    void Anchor::offset(const Vector2& val) {
        mOffset = val;
    }
    
    const Vector2& Anchor::pivot() const {
        return mPivot;
    }
    
    void Anchor::pivot(const Vector2& val) {
        mPivot = val;
    }
    
    const Bounds2 Anchor::bounds() const {
        return mBounds;
    }
    
    void Anchor::bounds(const Bounds2& val) {
        mBounds = val;
    }
    
    Vector2 Anchor::size() const {
        return mBounds.size();
    }
    
    void Anchor::size(const Vector2& val) {
        mBounds.size(val);
    }
    
    f32_t Anchor::left() const {
        f32_t base = mParent != nullptr ? mParent->mBounds.min.x : 0;
        return (mOffset.x + mBounds.min.x) - base;
    }
    
    void Anchor::left(f32_t val) {
        f32_t current = this->left();
        f32_t delta = val - current;
        mBounds.min.x += delta;
    }
    
    f32_t Anchor::right() const {
        f32_t base = mParent != nullptr ? mParent->mBounds.max.x : 0;
        return base - (mOffset.x + mBounds.max.x);
    }
    
    void Anchor::right(f32_t val) {
        f32_t current = this->right();
        f32_t delta = val - current;
        mBounds.max.x += delta;
    }
    
    f32_t Anchor::top() const {
        f32_t base = mParent != nullptr ? mParent->mBounds.min.y : 0;
        return (mOffset.y + mBounds.min.y) - base;
    }
    
    void Anchor::top(f32_t val) {
        f32_t current = this->top();
        f32_t delta = val - current;
        mBounds.min.y += delta;
    }
    
    f32_t Anchor::bottom() const {
        f32_t base = mParent != nullptr ? mParent->mBounds.max.y : 0;
        return base - (mOffset.y + mBounds.max.y);
    }
    
    void Anchor::bottom(f32_t val) {
        f32_t current = this->bottom();
        f32_t delta = val - current;
        mBounds.max.y += delta;
    }
}
