#include "pch.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Graphics/Renderer.h"
#include "Utility/CommandArgsParser.h"

int entry(int argc, char **argv) {
    gt::CommandArgsParser::Get().parse(argc, argv);

#ifdef DEBUG_MODE
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif

    gt::Renderer::StartUp();

    {
        gt::WindowDesc wdesc{};
        wdesc.title = "graphics-template";
        wdesc.width = atoll(gt::CommandArgsParser::Get().getArgumentValue("width", "1024").c_str());
        wdesc.height = atoll(gt::CommandArgsParser::Get().getArgumentValue("height", "720").c_str());
        wdesc.fullscreen = false;
        wdesc.hidden = false;

        auto window = gt::Renderer::Instance().initialize(wdesc);
        spdlog::debug("Initialize FreeImage");
        FreeImage_Initialise();

        spdlog::debug("Initialize OpenAL");
        ALCdevice *alDevice = alcOpenDevice(NULL);
        if (!alDevice) {
            spdlog::critical("Failed to create openal device");
            return -1;
        }

        ALCcontext *alContext;
        alContext = alcCreateContext(alDevice, NULL);
        if (!alContext) {
            spdlog::critical("Failed to create openal context");
            return -1;
        }

        if (!alcMakeContextCurrent(alContext)) {
            spdlog::critical("Failed to set openal context");
            return -1;
        } 

        gt::Renderer renderer;
        gt::VertexBuffer vbo;

        gt::BufferLayout layout = {{
            { gt::BufferType::Float3, false },
        }};
        vbo.setLayout(layout);

        float vertices[] = {
             0.5f,  0.5f, 0.0f,  // 우측 상단
             0.5f, -0.5f, 0.0f,  // 우측 하단
            -0.5f, -0.5f, 0.0f,  // 좌측 하단
            -0.5f,  0.5f, 0.0f   // 좌측 상단
        };  
        vbo.write((uint8_t *) vertices, sizeof(vertices));

        gt::IndexBuffer ibo;
        uint32_t indices[] = { 0, 1, 3, 1, 2, 3 };
        ibo.write((uint8_t *) indices, sizeof(indices));

        gt::ShaderDesc desc{};
        desc.vertexSource = "#version 330 core\n" \
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}";
        desc.fragmentSource = "#version 330 core\n" 
"out vec4 FragColor;\n"
"void main() {\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}";

        gt::Shader shader(desc);

        renderer.setShader(&shader);
        renderer.setVertexBuffer(&vbo);
        renderer.setIndexBuffer(&ibo);

        // 메인루프
        while (window->isRunning()) {
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0, 1, 0, 1);

            renderer.drawIndexed(0, 6, 0, 4);

            window->swapBuffers();
            glfwPollEvents();
        }

        spdlog::debug("Deinitialise OpenAL");
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(alContext);
        alcCloseDevice(alDevice);

        spdlog::debug("Deinitialise FreeImage");
        FreeImage_DeInitialise();

    }

    gt::Renderer::ShutDown();
    
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
