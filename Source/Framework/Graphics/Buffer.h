#pragma once

#include "Graphics.h"

namespace gt {
    enum class BufferType : uint8_t {
        None = 0,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        Color,
        TexCoord,
    };

    class BufferLayout {
    public:
        struct Element {
            uint32_t size;
            BufferType type;
            bool normalized = false;
            size_t offset;

            Element(BufferType type, bool normalized = false);

            uint8_t getComponentCount() const;
        };

    private:
        std::vector<Element> elements;
        uint32_t stride;

    public:
        BufferLayout();
        BufferLayout(const std::vector<Element> &elements);

    private:
        void calculateStride();

    public:
        size_t getElementSize() const { return elements.size(); }
        const Element &getElement(int index) const { return elements[index]; }

        uint32_t getStride() const { return stride; }
    };

    class Buffer {
    private:
        uint32_t bufferId;
        uint32_t bufferType;

    public:
        Buffer(uint32_t type);

    public:
        void bind() const;
        virtual void write(uint8_t *data, size_t size) = 0;

        uint32_t getBufferId() const { return bufferId; }
        uint32_t getBufferType() const { return bufferType; }
    };

    class VertexBuffer : public Buffer {
    private:
        BufferLayout layout;
        uint32_t arrayId;

    protected:
        VertexBuffer();

    public:
        ~VertexBuffer();

    public:
        static std::shared_ptr<VertexBuffer> New();

    public:
        void write(uint8_t *data, size_t size) override;

        void setLayout(const BufferLayout &layout) { this->layout = layout; }
        const BufferLayout &getLayout() const { return layout; }

        void setArrayId(uint32_t &id) { this->arrayId = id; }
        uint32_t getArrayId() const { return this->arrayId; }

        DEFINE_THIS_PTR(VertexBuffer);
    };

    class IndexBuffer : public Buffer {
    protected:
        IndexBuffer();

    public:
        static std::shared_ptr<IndexBuffer> New();

    public:
        void write(uint8_t *data, size_t size) override;

        DEFINE_THIS_PTR(IndexBuffer);
    };
}
