#ifndef _EOKAS_WIDGETS_RENDERER_H_
#define _EOKAS_WIDGETS_RENDERER_H_

#include "./header.h"

namespace eokas {
    class UIVertex {
        Vector3 position;
        Vector4 color;
        Vector2 uv;
    };
    
    class UIGeometry {
    
    };
    
    class UITexture {
    
    };
    
    class UIRenderer {
    public:
        virtual UIGeometry* createGeometry() = 0;
        virtual UITexture* createTexture() = 0;
        
        virtual void setGeometry(UIGeometry* geometry) = 0;
        virtual void setTe
    };
}

#endif//_EOKAS_WIDGETS_RENDERER_H_
