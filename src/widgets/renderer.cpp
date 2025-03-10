
#include <utility>

#include "./renderer.h"

namespace eokas
{
    struct Utils
    {
        static std::string readTextFile(const std::string& fileName)
        {
            const size_t maxFileSize = 4096;
            
            char buffer[maxFileSize] = {0};
            memset(buffer, 0, maxFileSize);
            
            FILE* fd = NULL;
            fopen_s(&fd, fileName.c_str(), "rb");
            if (fd == NULL)
            {
                throw std::exception("Open shader file failed.");
            }
            
            fseek(fd, 0, SEEK_END);
            size_t size = ftell(fd);
            if (size > maxFileSize)
            {
                throw std::exception("The shader file is too large.");
            }
            
            fseek(fd, 0, SEEK_SET);
            fread(buffer, size, 1, fd);
            fclose(fd);
            
            return buffer;
        }
        
        static Program::Ref compileShader(Device::Ref device, const std::string& file, ProgramType type, ProgramTarget target, const char* entry)
        {
            ProgramOptions options;
            options.name = file;
            options.source = readTextFile(file);
            options.type = type;
            options.target = target;
            options.entry = entry;
            return device->createProgram(options);
        }
    };
    
    UIRenderer::UIRenderer(Device::Ref device)
        : mDevice(std::move(device))
    {
        auto vs = Utils::compileShader(mDevice, "../shaders/002-texture.hlsl", ProgramType::Vertex, ProgramTarget::SM_5_0, "VSMain");
        auto ps = Utils::compileShader(mDevice, "../shaders/002-texture.hlsl", ProgramType::Fragment, ProgramTarget::SM_5_0, "PSMain");
        
        std::vector<VertexElement> vElements;
        vElements.push_back({"POSITION", 0, 0, Format::R32G32B32_FLOAT});
        vElements.push_back({"COLOR", 0, 12, Format::R32G32B32A32_FLOAT});
        vElements.push_back({"TEXCOORD", 0, 28, Format::R32G32_FLOAT});
        
        mPipelineState = mDevice->createPipelineState();
        mPipelineState->begin();
        mPipelineState->setProgram(ProgramType::Vertex, vs);
        mPipelineState->setProgram(ProgramType::Fragment, ps);
        mPipelineState->setVertexElements(vElements);
        mPipelineState->setTexture(0, mTexture);
        mPipelineState->end();
        
        mCommandBuffer = mDevice->createCommandBuffer(mPipelineState);
    }
    
    UIRenderer::~UIRenderer()
    {}
    
    UIRenderPrimitive* UIRenderer::createPrimitive(const UIGeometry& geom)
    {
    
    }
    
    UIRenderMaterial* UIRenderer::createMaterial(const UIMaterial& mat)
    {}
    
    void UIRenderer::setPrimitive(UIRenderPrimitive* primitive)
    {
        mPrimitive = primitive;
    }
    
    void UIRenderer::setMaterial(UIRenderMaterial* material)
    {
        mMaterial = material;
    }
    
    void UIRenderer::render()
    {
        mCommandBuffer->setTopology(Topology::TriangleList);
        mCommandBuffer->setVertexBuffer(
            mPrimitive->vertexBuffer,
            mPrimitive->vertexLength,
            mPrimitive->vertexStride);
        mCommandBuffer->setIndexBuffer(
            mPrimitive->indexBuffer,
            mPrimitive->indexLength,
            mPrimitive->indexFormat);
    }
}
