#include "Buffer.h"

#include <glad/glad.h>
#include <gl/gl.h>

#include "GraphicsAPI.h"

namespace gt {
    static uint8_t getTypeSize(BufferType type) {
        switch (type) {
        case BufferType::Float: return 1 * sizeof(float);
        case BufferType::Float2: return 2 * sizeof(float);
        case BufferType::Float3: return 3 * sizeof(float);
        case BufferType::Float4: return 4 * sizeof(float);

        case BufferType::Int: return 1 * sizeof(int);
        case BufferType::Int2: return 2 * sizeof(int);
        case BufferType::Int3: return 3 * sizeof(int);
        case BufferType::Int4: return 4 * sizeof(int);

        case BufferType::Color: return 4 * sizeof(float);
        case BufferType::TexCoord: return 2 * sizeof(float);

        default: return 0;
        }
    }

    BufferLayout::Element::Element(BufferType type, bool normalized)
        : size(getTypeSize(type))
          , type(type)
          , normalized(normalized)
          , offset(0) {}


    uint8_t BufferLayout::Element::getComponentCount() const {
        switch (type) {
        case BufferType::Float: return 1;
        case BufferType::Float2: return 2;
        case BufferType::Float3: return 3;
        case BufferType::Float4: return 4;

        case BufferType::Int: return 1;
        case BufferType::Int2: return 2;
        case BufferType::Int3: return 3;
        case BufferType::Int4: return 4;

        case BufferType::Color: return 4;
        case BufferType::TexCoord: return 2;

        default: return 0;
        }
    }


    BufferLayout::BufferLayout() : stride(0) { }

    BufferLayout::BufferLayout(const std::vector<Element> &elements) : elements(elements) {
        calculateStride();
    }

    void BufferLayout::calculateStride() {
        stride = 0;
        for (auto i = 0; i < elements.size(); ++i) {
            auto &element = elements[i];
            element.offset = stride;
            stride += element.size;
        }
    }

    Buffer::Buffer(uint32_t type) : bufferType(type) {
        glGenBuffers(1, &bufferId);
        CHECK_GL_ERROR();
    }

    void Buffer::bind() const {
        glBindBuffer(bufferType, bufferId);
        CHECK_GL_ERROR();
    }

    VertexBuffer::VertexBuffer() : Buffer(GL_ARRAY_BUFFER) {}
    VertexBuffer::~VertexBuffer() {
        if (!_thisPtr.expired()) {
            gt::GraphicsAPI::Instance().notifyVertexBufferDestroyed(getThisPtr());
        }
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::New() {
        auto buffer = std::shared_ptr<VertexBuffer>(new VertexBuffer());
        buffer->setThisPtr(buffer);

        return buffer;
    }

    void VertexBuffer::write(uint8_t *data, size_t size) {
        bind();

        glBufferData(getBufferType(), size, data, GL_STATIC_DRAW);
        CHECK_GL_ERROR();
    }

    IndexBuffer::IndexBuffer() : Buffer(GL_ELEMENT_ARRAY_BUFFER) {}


    std::shared_ptr<IndexBuffer> IndexBuffer::New() {
        auto buffer = std::shared_ptr<IndexBuffer>(new IndexBuffer());
        buffer->setThisPtr(buffer);

        return buffer;
    }

    void IndexBuffer::write(uint8_t *data, size_t size) {
        bind();

        glBufferData(getBufferType(), size, data, GL_STATIC_DRAW);
        CHECK_GL_ERROR();
    }
}
