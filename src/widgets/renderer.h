#ifndef _EOKAS_WIDGETS_RENDERER_H_
#define _EOKAS_WIDGETS_RENDERER_H_

#include "./header.h"

namespace eokas
{
    struct UIGeometry
    {
        std::vector<Vector3> positions;
        std::vector<Vector4> colors;
        std::vector<Vector2> uv;
        std::vector<u32_t> indices;
        u32_t material;
    };
    
    struct UIMaterial
    {
        Color color;
        String texture;
        String vertexShader;
        String pixelShader;
    };
    
    struct UIShape
    {
        std::vector<UIGeometry> geometries;
        std::vector<UIMaterial> materials;
        
        void clear()
        {
            this->geometries.clear();
            this->materials.clear();
        }
    };
    
    struct UIRenderPrimitive
    {
        DynamicBuffer::Ref vertexBuffer;
        uint32_t vertexLength;
        uint32_t vertexStride;
        
        DynamicBuffer::Ref indexBuffer;
        uint32_t indexLength;
        Format indexFormat;
    };
    
    class UIRenderMaterial
    {
        Color color;
        Texture::Ref texture;
        PipelineState::Ref PSO;
    };
    
    class UIRenderer
    {
    public:
        explicit UIRenderer(Device::Ref device);
        virtual ~UIRenderer();
        
        UIRenderPrimitive* createPrimitive(const UIGeometry& geom);
        UIRenderMaterial* createMaterial(const UIMaterial& mat);
        
        void setPrimitive(UIRenderPrimitive* primitive);
        void setMaterial(UIRenderMaterial* material);
        
        void render();

    private:
        Device::Ref mDevice;
        CommandBuffer::Ref mCommandBuffer;
        UIRenderPrimitive* mPrimitive;
        UIRenderMaterial* mMaterial;
    };
}

#endif//_EOKAS_WIDGETS_RENDERER_H_
