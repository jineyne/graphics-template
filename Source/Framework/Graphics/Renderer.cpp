#include "Renderer.h"

#include <glad/glad.h>
#include <gl/gl.h>

namespace gt {
    size_t Renderer::Hash::operator()(const VertexArrayObject &vao) const {
        size_t seed = 0;

        std::hash<uint32_t> hasher;
        for (auto i = 0; i < vao.buffers.size(); ++i) {
            seed ^= hasher(vao.buffers[i]->getBufferId()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        return seed;
    }

    bool Renderer::Equal::operator()(const VertexArrayObject &a, const VertexArrayObject &b) const {
        /*if (a.id != b.id) {
            return false;
        }*/

        if (a.buffers.size() != b.buffers.size()) {
            return false;
        }

        size_t size = a.buffers.size();
        for (int i = 0; i < size; ++i) {
            if (a.buffers[i]->getBufferId() != b.buffers[i]->getBufferId()) {
                return false;
            }
        }

        return true;
    }

    void Renderer::setVertexBuffer(VertexBuffer *buffer) {
        activeVertexBuffer = buffer;
    }

    void Renderer::setIndexBuffer(IndexBuffer *buffer) {
        activeIndexBuffer = buffer;
    }

    void Renderer::setShader(Shader *shader) {
        activeShader = shader;
    }

    void Renderer::draw(uint32_t offset, uint32_t count) {
        activeShader->bind();

        auto vao = findVertexArrayObject(activeVertexBuffer, activeVertexBuffer->getLayout());
        glBindVertexArray(vao.id);
        CHECK_GL_ERROR();

        glDrawArrays(GL_TRIANGLES, offset, count);
        CHECK_GL_ERROR();
    }

    void Renderer::drawIndexed(uint32_t indexOffset, uint32_t indexCount, uint32_t vertexOffset, uint32_t vertexCount) {
        activeShader->bind();

        auto vao = findVertexArrayObject(activeVertexBuffer, activeVertexBuffer->getLayout());
        glBindVertexArray(vao.id);
        CHECK_GL_ERROR();

        activeIndexBuffer->bind();
        CHECK_GL_ERROR();

        glDrawElementsBaseVertex(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, vertexOffset);
        CHECK_GL_ERROR();
    }

    const Renderer::VertexArrayObject &Renderer::findVertexArrayObject(VertexBuffer *buffer, const BufferLayout &layout) {
        VertexArrayObject vao { 0, { buffer } };

        auto findIt = objects.find(vao);
        if (findIt != objects.end()) {
            return *findIt;
        }

        // buffer
        auto stride = 0;

        glGenVertexArrays(1, &vao.id);
        CHECK_GL_ERROR();

        glBindVertexArray(vao.id);
        CHECK_GL_ERROR();

        buffer->bind();

        for (auto i = 0; i < layout.getElementSize(); i++) {
            auto element = layout.getElement(i);

            GLenum type = 0;

            switch (element.type) {
            case BufferType::Float:
            case BufferType::Float2:
            case BufferType::Float3:
            case BufferType::Float4:
                type = GL_FLOAT;
                break;

            case BufferType::Color:
                type = GL_UNSIGNED_BYTE;
                break;

            default:
            case BufferType::None:
                break;
            }

            glVertexAttribPointer(stride, element.getComponentCount(), type, GL_FALSE, element.size, (void*) 0);
            CHECK_GL_ERROR();

            glEnableVertexAttribArray(i);  
            CHECK_GL_ERROR();
        }

        // TODO: 나중에 리스트로 해서 buffer 별로 semantic 데이터 가질 수 있게 업데이트 하기 
        vao.buffers.resize(1);
        vao.buffers[0] = buffer;
        buffer->setArrayId(vao.id);

        auto it = objects.insert(vao);
        return *it.first;
    }
}
