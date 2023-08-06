#pragma once

#include "Graphics.h"

#include "Texture.h"

namespace gt {
    struct ShaderDesc {
        std::string vertexSource;
        std::string fragmentSource;
    };

    class Shader {
    private:
        ShaderDesc desc;
        bool compiled;
        uint32_t shaderId;

    protected:
        Shader(const ShaderDesc &desc);

    public:
        static std::shared_ptr<Shader> New(const ShaderDesc &desc);

    public:
        void compile();

        void bind();

        void setTexture(const std::string &name, const std::shared_ptr<Texture> &texture);

    private:
        void checkCompileError(uint32_t id);
        void checkLinkError();

        DEFINE_THIS_PTR(Shader);
    };
}

