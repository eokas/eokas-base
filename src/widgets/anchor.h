#ifndef _EOKAS_WIDGETS_ANCHOR_H_
#define _EOKAS_WIDGETS_ANCHOR_H_

#include "./header.h"

namespace eokas {
    class UIAnchor {
    public:
        explicit UIAnchor(UIAnchor* parent = nullptr);
        virtual ~UIAnchor();
        
        const Vector2& offset() const;
        void offset(const Vector2& val);
        
        Vector2 position() const;
        void position(const Vector2& val);
        
        const Vector2& pivot() const;
        void pivot(const Vector2& val);
        
        const Bounds2& bounds() const;
        void bounds(const Bounds2& val);
        
        Vector2 size() const;
        void size(const Vector2& val);
        
        f32_t left() const;
        void left(f32_t val);
        
        f32_t right() const;
        void right(f32_t val);
        
        f32_t top() const;
        void top(f32_t val);
        
        f32_t bottom() const;
        void bottom(f32_t val);
    
    protected:
        UIAnchor* mParent;
        Vector2 mOffset;
        Vector2 mPivot;
        Bounds2 mBounds;
    };
}

#endif//_EOKAS_WIDGETS_ANCHOR_H_