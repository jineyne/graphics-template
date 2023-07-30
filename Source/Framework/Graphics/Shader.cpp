#include "Shader.h"

#include <glad/glad.h>
#include <gl/gl.h>


namespace gt {
    Shader::Shader(const ShaderDesc &desc) : desc(desc) {
        shaderId = glCreateProgram();
        CHECK_GL_ERROR();
    }

    std::shared_ptr<Shader> Shader::New(const ShaderDesc &desc) {
        auto shader = std::shared_ptr<Shader>(new Shader(desc));
        shader->setThisPtr(shader);

        shader->compile();

        return shader;
    }

    void Shader::compile() {
        const char *vsrc = desc.vertexSource.c_str();
        const char *fsrc = desc.fragmentSource.c_str();

        uint32_t vshader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vshader, 1, &vsrc, NULL);
        CHECK_GL_ERROR();

        glCompileShader(vshader);
        CHECK_GL_ERROR();

        checkCompileError(vshader);

        uint32_t fshader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fshader, 1, &fsrc, NULL);
        CHECK_GL_ERROR();

        glCompileShader(fshader);
        CHECK_GL_ERROR();

        checkCompileError(fshader);

        glAttachShader(shaderId, vshader);
        CHECK_GL_ERROR();

        glAttachShader(shaderId, fshader);
        CHECK_GL_ERROR();

        glLinkProgram(shaderId);
        CHECK_GL_ERROR();

        checkLinkError();

        bind();
        CHECK_GL_ERROR();

        glDeleteShader(vshader);
        CHECK_GL_ERROR();

        glDeleteShader(fshader);
        CHECK_GL_ERROR();
    }

    void Shader::bind() {
        glUseProgram(shaderId);
        CHECK_GL_ERROR();
    }

    void Shader::checkCompileError(uint32_t id) {
        int success;
        char log[512];

        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id, 512, NULL, log);
            throw std::invalid_argument(log);
        }
    }

    void Shader::checkLinkError() {
        int success;
        char log[512];
        glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderId, 512, NULL, log);
            throw std::invalid_argument(log);
        }
    }
}
