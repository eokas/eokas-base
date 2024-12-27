#ifndef _EOKAS_WIDGETS_CANVAS_H_
#define _EOKAS_WIDGETS_CANVAS_H_

#include "./header.h"
#include "./widgets.h"
#include "./renderer.h"

namespace eokas {
    class UICanvas {
    public:
        UICanvas();
        
        void resize(const Rect& rect);
        
        void add(UIWidget* widget);
        
        void render();
    
    private:
        Rect mRect;
        std::vector<UIWidget*> mWidgets;
        std::vector<UIShape*> mShapes;
    };
}

#endif//_EOKAS_WIDGETS_CANVAS_H_