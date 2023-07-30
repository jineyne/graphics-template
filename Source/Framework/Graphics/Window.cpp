#include "Window.h"

#include <GLFW/glfw3.h>

#include "Utility/Exceptions.h"

namespace gt {
    Window::Window(const WindowDesc &desc) 
            : windowPtr(nullptr), title(desc.title), width(desc.width), height(desc.height) {

        GLFWmonitor *monitor = nullptr;
        if (desc.fullscreen) {
            monitor = glfwGetPrimaryMonitor();
        }

        GLFWwindow *parent = nullptr;
        if (desc.parent != nullptr) {
            parent = (GLFWwindow *) desc.parent->getWindowPtr();
        }

        glfwWindowHint(GLFW_VISIBLE, (desc.hidden ? GLFW_FALSE : GLFW_TRUE));

#ifdef DEBUG_MODE
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

        windowPtr = glfwCreateWindow(desc.width, desc.height, desc.title.c_str(), monitor, parent);
        if (windowPtr == nullptr) {
            throw invalid_state("Failed to create window: " + desc.title);
        }

        auto glfwWindowPtr = (GLFWwindow *) windowPtr;

        glfwSetWindowUserPointer(glfwWindowPtr, this);

        glfwSetWindowPosCallback(glfwWindowPtr, (GLFWwindowposfun) Window::NativeWindowMoved);
        glfwSetWindowSizeCallback(glfwWindowPtr, (GLFWwindowsizefun) Window::NativeWindowResized);
        glfwSetCursorEnterCallback(glfwWindowPtr, (GLFWcursorenterfun) Window::NativeWindowCursorEnter);
        glfwSetWindowFocusCallback(glfwWindowPtr, (GLFWwindowfocusfun) Window::NativeWindowFocused);
    }

    void Window::NativeWindowMoved(void *window, int xpos, int ypos) {
        auto original = (Window *) glfwGetWindowUserPointer((GLFWwindow *) window);
        if (original == nullptr) {
            return;
        }

        original->left = xpos;
        original->top = ypos;
    }

    void Window::NativeWindowResized(void *window, int width, int height) {
        auto original = (Window *) glfwGetWindowUserPointer((GLFWwindow *) window);
        if (original == nullptr) {
            return;
        }

        original->width = width;
        original->height = height;
    }

    void Window::NativeWindowCursorEnter(void *window, int entered) {
        auto original = (Window *) glfwGetWindowUserPointer((GLFWwindow *) window);
        if (original == nullptr) {
            return;
        }

        // do stuff
    }

    void Window::NativeWindowFocused(void *window, int focusted) {
        auto original = (Window *) glfwGetWindowUserPointer((GLFWwindow *) window);
        if (original == nullptr) {
            return;
        }

        // do stuff
    }

    void Window::show() {
        glfwShowWindow((GLFWwindow *) windowPtr);
    }

    void Window::hide() {
        glfwHideWindow((GLFWwindow *) windowPtr);
    }

    void Window::minimize() {
        glfwIconifyWindow((GLFWwindow *) windowPtr);
    }

    void Window::maximize(bool maximized) {
        if (maximized) {
            glfwMaximizeWindow((GLFWwindow *) windowPtr);
        } else {
            glfwRestoreWindow((GLFWwindow *) windowPtr);
        }
    }

    void Window::swapBuffers(uint32_t mask) {
        glfwSwapBuffers((GLFWwindow *) windowPtr);
    }

    void Window::resize(int32_t width, int32_t height) {
        this->width = width;
        this->height = height;

        glfwSetWindowSize((GLFWwindow *) windowPtr, width, height);
    }

    void Window::move(int32_t left, int32_t top) {
        this->left = left;
        this->top = top;

        glfwSetWindowPos((GLFWwindow *) windowPtr, left, top);
    }

    void Window::setTitle(const std::string& title) {
        this->title = title;

        glfwSetWindowTitle((GLFWwindow *) windowPtr, title.c_str());
    }

    bool Window::isRunning() const {
        return !glfwWindowShouldClose((GLFWwindow *) windowPtr);
    }
}
