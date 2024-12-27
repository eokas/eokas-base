#ifndef _EOKAS_WIDGETS_RENDERER_H_
#define _EOKAS_WIDGETS_RENDERER_H_

#include "./header.h"

namespace eokas {
    struct UIGeometry {
        std::vector<Vector3> positions;
        std::vector<Vector4> colors;
        std::vector<Vector2> uv;
        std::vector<u32_t> indices;
        u32_t material;
    };
    
    struct UIMaterial {
        String path;
    };
    
    struct UIShape {
        std::vector<UIGeometry> geometries;
        std::vector<UIMaterial> materials;
        
        void clear() {
            this->geometries.clear();
            this->materials.clear();
        }
    };
    
    class UIPrimitive {
    };
    
    class UITexture {
    };
    
    class UIRenderer {
    public:
        virtual UIPrimitive* createPrimitive(const UIGeometry& geom) = 0;
        virtual UITexture* createTexture(const String& path) = 0;
        
        virtual void setPrimitive(UIPrimitive* primitive) = 0;
        virtual void setTexture(UITexture* texture) = 0;
        
        virtual void render() = 0;
    };
}

#endif//_EOKAS_WIDGETS_RENDERER_H_
