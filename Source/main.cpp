#include "pch.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Graphics/GraphicsAPI.h"
#include "Importer/Importer.h"
#include "Manager/TextureManager.h"
#include "Utility/CommandArgsParser.h"

int entry(int argc, char **argv) {
    gt::CommandArgsParser::Get().parse(argc, argv);

#ifdef DEBUG_MODE
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif

    gt::GraphicsAPI::StartUp();

    {
        gt::WindowDesc wdesc{};
        wdesc.title = "graphics-template";
        wdesc.width = atoll(gt::CommandArgsParser::Get().getArgumentValue("width", "1024").c_str());
        wdesc.height = atoll(gt::CommandArgsParser::Get().getArgumentValue("height", "720").c_str());
        wdesc.fullscreen = false;
        wdesc.hidden = false;

        auto window = gt::GraphicsAPI::Instance().initialize(wdesc);

        gt::Importer::StartUp();

        gt::TextureManager::StartUp();

        auto vbo = gt::VertexBuffer::New();

        gt::BufferLayout layout = {
            {
                {gt::BufferType::Float3, false},
                {gt::BufferType::Float3, false},
                {gt::BufferType::TexCoord, false},
            }
        };
        vbo->setLayout(layout);

        float vertices[] = {
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 우측 상단
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 우측 하단
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 좌측 하단
            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 좌측 상단
        };
        vbo->write((uint8_t*) vertices, sizeof(vertices));

        auto ibo = gt::IndexBuffer::New();
        uint32_t indices[] = {0, 1, 3, 1, 2, 3};
        ibo->write((uint8_t*) indices, sizeof(indices));

        gt::ShaderDesc desc{};
        desc.vertexSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec3 aColor;\n"
            "layout (location = 2) in vec2 aTexCoord;\n"
            "out vec3 Color;\n"
            "out vec2 TexCoord;\n"
            "void main() {\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "   Color = aColor;\n"
            "   TexCoord = aTexCoord;\n"
            "}";
        desc.fragmentSource = "#version 330 core\n"
            "in vec3 Color;\n"
            "in vec2 TexCoord;\n"
            "out vec4 FragColor;\n"
            "uniform sampler2D MainTexture;\n"
            "void main() {\n"
            "    FragColor = texture(MainTexture, TexCoord);\n"
            "}";

        auto shader = gt::Shader::New(desc);

        auto texture = gt::Importer::Instance().import<gt::Texture>("C:/Users/jiney/Pictures/2-format6001.jpg");
        shader->setTexture("MainTexture", texture);

        auto &gapi = gt::GraphicsAPI::Instance();
        gapi.setShader(shader);
        gapi.setVertexBuffer(vbo);
        gapi.setIndexBuffer(ibo);

        // 메인루프
        while (window->isRunning()) {
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0, 1, 0, 1);

            gapi.drawIndexed(0, 6, 0, 4);

            window->swapBuffers();
            glfwPollEvents();
        }

        gt::TextureManager::ShutDown();
        gt::Importer::ShutDown();
    }

    gt::GraphicsAPI::ShutDown();

    return 0;
}

int main(int argc, char **argv) {
    int success = false;

    try {
        success = entry(argc, argv);
    } catch (std::exception e) {
        spdlog::critical(e.what());
    }

    return success;
}
