#pragma once

#include "Graphics.h"

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

    private:
        void checkCompileError(uint32_t id);
        void checkLinkError();

        DEFINE_THIS_PTR(Shader);
    };
}

