#pragma once

#include "Graphics.h"

#include "Buffer.h"
#include "Shader.h"
#include "Window.h"

#include "Utility/Module.h"

namespace gt {
    class GraphicsAPI : public Module<GraphicsAPI> {
        struct VertexArrayObject {
            uint32_t id;
            std::vector<std::shared_ptr<VertexBuffer>> buffers;
        };

        class Hash {
        public:
            size_t operator()(const VertexArrayObject &vao) const;
        };

        class Equal {
        public:
            bool operator()(const VertexArrayObject &a, const VertexArrayObject &b) const;
        };

        std::unordered_set<VertexArrayObject, Hash, Equal> objects;

    private:
        std::shared_ptr<VertexBuffer> activeVertexBuffer;
        std::shared_ptr<IndexBuffer> activeIndexBuffer;

        std::shared_ptr<Shader> activeShader;

    public:
        std::shared_ptr<Window> initialize(const WindowDesc &desc);

        void setVertexBuffer(const std::shared_ptr<VertexBuffer> &buffer);
        void setIndexBuffer(const std::shared_ptr<IndexBuffer> &buffer);

        void setShader(const std::shared_ptr<Shader> &shader);

        void draw(uint32_t offset, uint32_t count);
        void drawIndexed(uint32_t indexOffset, uint32_t indexCount, uint32_t vertexOffset, uint32_t vertexCount);

        void notifyVertexBufferDestroyed(const std::shared_ptr<VertexBuffer> &buffer);

    protected:
        void onStartUp() override;
        void onShutDown() override;

        
    private:
        const VertexArrayObject &findVertexArrayObject(const std::shared_ptr<VertexBuffer> &buffers, const BufferLayout &layout);
    };
}
