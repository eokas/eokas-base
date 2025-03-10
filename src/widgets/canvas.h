#ifndef _EOKAS_WIDGETS_CANVAS_H_
#define _EOKAS_WIDGETS_CANVAS_H_

#include "./header.h"
#include "./widgets.h"
#include "./renderer.h"

namespace eokas {
    class UICanvas {
    public:
        UICanvas(const Rect& rect);
        
        void resize(const Rect& rect);
        
        void drawLine(const Vector2& start, const Vector2& end, const Color& color);
        void drawTriangle(const Vector2& v0, const Vector2& v1, const Vector2& v2, const Color& color);
        void drawRect(const Vector2& origin, const Vector2& size, const Color& color);
        void drawCircle(const Vector2& center, f32_t radius, const Color& color);

        void fillLine(const Vector2& start, const Vector2& end, const Color& color);
        void fillTriangle(const Vector2& v0, const Vector2& v1, const Vector2& v2, const Color& color);
        void fillRect(const Vector2& origin, const Vector2& size, const Color& color);
        void fillCircle(const Vector2& center, f32_t radius, const Color& color);
        
        void render();
    
    private:
        Rect mRect;
        std::vector<UIWidget*> mWidgets;
        std::vector<UIShape*> mShapes;
    };
}

#endif//_EOKAS_WIDGETS_CANVAS_H_