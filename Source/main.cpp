#include <glfw/glfw3.h>

int main() {
    // GLFW 초기화 
    GLFWwindow *window = nullptr;
    if (!glfwInit()) {
        return -1;
    }

    // 윈도우 생성
    window = glfwCreateWindow(640, 480, "Hello, World!", NULL, NULL);
    if (!window) {
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
    
    // 종료
    glfwTerminate();
    return 0;
}

