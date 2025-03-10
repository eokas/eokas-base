#include "./canvas.h"

namespace eokas {
    UICanvas::UICanvas(const Rect& rect)
        : mRect(rect)
        , mWidgets()
        , mShapes() {
        
    }
    
    void UICanvas::resize(const Rect& rect) {
        mRect = rect;
    }
    
    void UICanvas::render() {
        mShapes.clear();
        for(UIWidget* widget : mWidgets) {
            auto* shape = widget->render();
            mShapes.push_back(shape);
        }
        
        for(UIShape* shape : mShapes) {
        
        }
    }
}
