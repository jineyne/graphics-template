#pragma once

#include "Graphics.h"

#include "Buffer.h"
#include "Shader.h"

namespace gt {
    class Renderer {
        struct VertexArrayObject {
            uint32_t id;
            std::vector<VertexBuffer *> buffers;
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
        VertexBuffer *activeVertexBuffer;
        IndexBuffer *activeIndexBuffer;

        Shader *activeShader;

    public:
        void setVertexBuffer(VertexBuffer *buffer);
        void setIndexBuffer(IndexBuffer *buffer);

        void setShader(Shader *shader);

        void draw(uint32_t offset, uint32_t count);
        void drawIndexed(uint32_t indexOffset, uint32_t indexCount, uint32_t vertexOffset, uint32_t vertexCount);
        
    public:
        const VertexArrayObject &findVertexArrayObject(VertexBuffer *buffers, const BufferLayout &layout);
    };
}
