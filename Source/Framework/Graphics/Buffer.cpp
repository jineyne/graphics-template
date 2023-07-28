#include "Buffer.h"

#include <gl/glew.h>
#include <gl/gl.h>

namespace gt {

    static uint8_t getTypeSize(BufferType type) {
        switch (type) {
            case BufferType::Float:  return 1 * sizeof(float);
            case BufferType::Float2: return 2 * sizeof(float);
            case BufferType::Float3: return 3 * sizeof(float);
            case BufferType::Float4: return 4 * sizeof(float);
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
            case BufferType::Float:  return 1;
            case BufferType::Float2: return 2;
            case BufferType::Float3: return 3;
            case BufferType::Float4: return 4;
            default: return 0;
        }
    }


    VertexBuffer::VertexBuffer() {
        glGenBuffers(1, &bufferId);
    }

    void VertexBuffer::bind() const {
        glBindTexture(GL_TEXTURE_2D, bufferId);
    }
}
