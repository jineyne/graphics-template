#include "pch.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Framework/Graphics/Renderer.h"
#include "Framework/Utility/CommandArgsParser.h"

// glfw 에러 콜백 
void glfwErrorCallback(int error, const char* description) {
    spdlog::error("GLFW Error ({0}): {1}", error, description);
}

int entry(int argc, char **argv) {
    gt::CommandArgsParser::Get().parse(argc, argv);

#ifdef DEBUG_MODE
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif

    // GLFW 초기화 
    spdlog::debug("Initialize GLFW");
    if (!glfwInit()) {
        spdlog::critical("Failed to initialize GLFW");
        return -1;
    }

    size_t width = atoll(gt::CommandArgsParser::Get().getArgumentValue("width", "1024").c_str());
    size_t height = atoll(gt::CommandArgsParser::Get().getArgumentValue("height", "720").c_str());

    spdlog::debug("Create window: {0}x{1}", width, height);

    // 윈도우 생성
    GLFWwindow *window = glfwCreateWindow(width, height, "Graphics Template", NULL, NULL);
    if (!window) {
        spdlog::critical("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetErrorCallback(glfwErrorCallback);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        spdlog::critical("Failed to initialize OpenGL");
        return -1;
    }

    CHECK_GL_ERROR();

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
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 1, 0, 1);

        renderer.drawIndexed(0, 6, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    spdlog::debug("Deinitialise OpenAL");
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(alContext);
    alcCloseDevice(alDevice);

    spdlog::debug("Deinitialise FreeImage");
    FreeImage_DeInitialise();
    
    // 종료
    spdlog::debug("Dinitialize GLFW");
    glfwTerminate();

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
