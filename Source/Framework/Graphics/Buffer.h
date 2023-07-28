#pragma once

#include "pch.h"

namespace gt {
    enum class BufferType : uint8_t {
        None = 0, Float, Float2, Float3, Float4
    };


    class BufferLayout {
    private:
        struct Element {
            uint32_t size;
            BufferType type;
            bool normalized = false;
            size_t offset;

            Element(BufferType type, bool normalized = false);

            uint8_t getComponentCount() const;
        };

        std::vector<Element> elements;
        uint32_t stride;

    private:
        void calculateStride();

    public:
        size_t getElementSize() const { return elements.size(); }
        const Element &getElement(int index) const { return elements[index]; }
    };

    class VertexBuffer {
    private:
        uint32_t bufferId;
        BufferLayout layout;

    public:
        VertexBuffer();

    public:
        void setLayout(const BufferLayout &layout) { this->layout = layout; }
        const BufferLayout &getLayout() const { return layout; }

        void bind() const;

        uint32_t getBufferId() const { return bufferId; }
    };
}
