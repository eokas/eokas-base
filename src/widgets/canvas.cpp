#include "./canvas.h"

namespace eokas {
    UICanvas::UICanvas()
        : mRect()
        , mWidgets()
        , mShapes() {
        
    }
    
    void UICanvas::add(UIWidget* widget) {
        mWidgets.push_back(widget);
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
