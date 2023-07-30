#pragma once

#include "pch.h"

namespace gt {
    // 윈도우 생성에 필요한 정보들
    struct WindowDesc {
        std::string title;
        uint32_t width;
        uint32_t height;

        bool fullscreen;
        bool hidden;

        class Window *parent = nullptr;
    };

    class Window {
    private:
        void *windowPtr;

        std::string title;

        uint32_t left;
        uint32_t top;

        uint32_t width;
        uint32_t height;

    public:
        Window(const WindowDesc &desc);

    public:
        static void NativeWindowMoved(void *window, int xpos, int ypos);
        static void NativeWindowResized(void *window, int width, int height);
        static void NativeWindowCursorEnter(void *window, int entered);
        static void NativeWindowFocused(void *window, int focusted);

    public:
        void show();
        void hide();

        void minimize();
        void maximize(bool maximized = true);

        void swapBuffers(uint32_t mask = 0xffffffff);

        void resize(int32_t width, int32_t height);
        void move(int32_t left, int32_t top);

        const uint32_t &getLeft() const { return left; }
        const uint32_t &getTop() const { return top; }

        const uint32_t &getWidth() const { return width; }
        const uint32_t &getHeight() const { return height; }

        void setTitle(const std::string& title);
        const std::string &getTitle() const { return title; } 

        auto *getWindowPtr() const { return windowPtr; }
        bool isRunning() const;
    };
}
