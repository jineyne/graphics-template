#include "Graphics.h"

#include <glad/glad.h>

const char *get_gl_error_string(GLenum errorCode) {
    switch (errorCode) {
    case GL_INVALID_OPERATION:
        return "INVALID_OPERATION";

    case GL_INVALID_ENUM:
        return "INVALID_ENUM";

    case GL_INVALID_VALUE:
        return "INVALID_VALUE";

    case GL_OUT_OF_MEMORY:
        return "OUT_OF_MEMORY";

    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "INVALID_FRAMEBUFFER_OPERATION";
    }

    return "UNKNOWN_ERROR";
}

void check_gl_error(const char *function, const char *file, int32_t line) {
    GLenum errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) {
        GLuint finite = 255;
        std::stringstream ss;
        ss << "OpenGL error in " << function << " [" << file << ":" << line << "]:";

        GLenum newErrorCode = GL_NO_ERROR;

        while (errorCode != GL_NO_ERROR && finite--) {
            if (newErrorCode != errorCode) {
                const char *errorString = get_gl_error_string(errorCode);
                if (errorString) {
                    ss << "\n\t - " << errorString;
                }

                errorCode = newErrorCode;
            }

            newErrorCode = glGetError();
        }

        spdlog::warn(ss.str());
    }
}
