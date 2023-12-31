#include "GraphicsAPI.h"

#include <glad/glad.h>
#include <gl/gl.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

#include "Framework/Utility/CommandArgsParser.h"

namespace gt {
    // glfw 에러 콜백 
    void glfwErrorCallback(int error, const char* description) {
        spdlog::error("GLFW Error ({0}): {1}", error, description);
    }

    size_t GraphicsAPI::Hash::operator()(const VertexArrayObject &vao) const {
        size_t seed = 0;

        std::hash<uint32_t> hasher;
        for (auto i = 0; i < vao.buffers.size(); ++i) {
            seed ^= hasher(vao.buffers[i]->getBufferId()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        return seed;
    }

    bool GraphicsAPI::Equal::operator()(const VertexArrayObject &a, const VertexArrayObject &b) const {
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

    std::shared_ptr<Window> GraphicsAPI::initialize(const WindowDesc &desc) {
        spdlog::debug("Create window: {0}: {1}x{2}", desc.title, desc.width, desc.height);

        // 윈도우 생성
        auto window = std::make_shared<Window>(desc);

        glfwMakeContextCurrent((GLFWwindow *) window->getWindowPtr());

        glfwSetErrorCallback(glfwErrorCallback);
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            throw invalid_state("Failed to initialize OpenGL");
        }

        CHECK_GL_ERROR();

        return window;
    }

    void GraphicsAPI::setVertexBuffer(const std::shared_ptr<VertexBuffer> &buffer) {
        activeVertexBuffer = buffer;
    }

    void GraphicsAPI::setIndexBuffer(const std::shared_ptr<IndexBuffer> &buffer) {
        activeIndexBuffer = buffer;
    }

    void GraphicsAPI::setShader(const std::shared_ptr<Shader> &shader) {
        activeShader = shader;
    }

    void GraphicsAPI::draw(uint32_t offset, uint32_t count) {
        activeShader->bind();

        auto vao = findVertexArrayObject(activeVertexBuffer, activeVertexBuffer->getLayout());
        glBindVertexArray(vao.id);
        CHECK_GL_ERROR();

        glDrawArrays(GL_TRIANGLES, offset, count);
        CHECK_GL_ERROR();
    }

    void GraphicsAPI::drawIndexed(uint32_t indexOffset, uint32_t indexCount, uint32_t vertexOffset, uint32_t vertexCount) {
        activeShader->bind();

        auto vao = findVertexArrayObject(activeVertexBuffer, activeVertexBuffer->getLayout());
        glBindVertexArray(vao.id);
        CHECK_GL_ERROR();

        activeIndexBuffer->bind();
        CHECK_GL_ERROR();

        glDrawElementsBaseVertex(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, vertexOffset);
        CHECK_GL_ERROR();
    }

    void GraphicsAPI::onStartUp() {
    // GLFW 초기화 
        spdlog::debug("Initialize GLFW");
        if (!glfwInit()) {
            throw invalid_state("Failed to initialize GLFW");
        }
    }
    
    void GraphicsAPI::onShutDown() {
        // 종료
        spdlog::debug("Dinitialize GLFW");
        glfwTerminate();
    }

    void GraphicsAPI::notifyVertexBufferDestroyed(const std::shared_ptr<VertexBuffer> &buffer) {
        VertexArrayObject vao { 0, { buffer } };

        auto findIt = objects.find(vao);
        if (findIt != objects.end()) {
            objects.erase(findIt);
        }
    }

    const GraphicsAPI::VertexArrayObject &GraphicsAPI::findVertexArrayObject(const std::shared_ptr<VertexBuffer> &buffer,
            const BufferLayout &layout) {
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

            case BufferType::Int:
            case BufferType::Int2:
            case BufferType::Int3:
            case BufferType::Int4:
                type = GL_INT;
                break;

            case BufferType::Color:
                type = GL_UNSIGNED_BYTE;
                break;

            case BufferType::TexCoord:
                type = GL_FLOAT;
                break;

            default:
            case BufferType::None:
                break;
            }

            void *ptr = (void *) element.offset;

            bool isInteger = type == GL_SHORT || type == GL_UNSIGNED_SHORT || type == GL_INT ||
                    type == GL_UNSIGNED_INT || type == GL_UNSIGNED_BYTE;
            
            if (isInteger) {
                glVertexAttribIPointer(i, element.getComponentCount(), type, layout.getStride(), ptr);
                CHECK_GL_ERROR();
            } else {
                glVertexAttribPointer(i, element.getComponentCount(), type, GL_FALSE, layout.getStride(), ptr);
                CHECK_GL_ERROR();
            }
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
