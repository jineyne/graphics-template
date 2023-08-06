#pragma once

#include "Graphics.h"

#include "Resource/Resource.h"

namespace gt {
    enum class TextureType : uint8_t {
        _2d
    };

    ENUM_IO_OPERATORS(TextureType, uint8_t);

    enum class PixelFormat : uint32_t {
        Unknown,
        R8, RG8, RGB8, RGBA8, BGR8, BGRA8,
    };

    struct TextureDesc {
        uint32_t width;
        uint32_t height;

        TextureType type;
        PixelFormat format;
    };

    class Texture : public Resource {
    private:
        uint32_t textureId;

        TextureDesc desc;

    protected:
        Texture(const TextureDesc &desc);

    public:
        ~Texture();

    public:
        static std::shared_ptr<Texture> New(const TextureDesc &desc);

    public:
        void bind();

        void write(uint8_t *data, size_t size);

        virtual void serialize(std::ostream &os) override;
        virtual void deserialize(std::istream &is) override;

    private:
        int getTextureType();
        int getTextureFormat();

        friend class Resources;
    };
}
