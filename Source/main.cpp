#include "pch.h"

// glfw 에러 콜백 
void glfwErrorCallback(int error, const char* description) {
    spdlog::error("GLFW Error ({0}): {1}", error, description);
}

int main() {
#ifdef DEBUG_MODE
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif

    spdlog::debug("Initialize GLFW");

    // GLFW 초기화 
    GLFWwindow *window = nullptr;
    if (!glfwInit()) {
        spdlog::critical("Failed to initialize GLFW");
        return -1;
    }

    glfwSetErrorCallback(glfwErrorCallback);

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


    spdlog::debug("Create window");
    // 윈도우 생성
    window = glfwCreateWindow(640, 480, "Hello, World!", NULL, NULL);
    if (!window) {
        spdlog::critical("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // 메인루프
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
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

