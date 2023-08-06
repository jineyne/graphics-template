#include "Texture.h"

#include <glad/glad.h>

namespace gt {
    Texture::Texture(const TextureDesc &desc) : desc(desc) {
        glGenTextures(1, &textureId);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &textureId);
    }

    std::shared_ptr<Texture> Texture::New(const TextureDesc &desc) {
        auto texture = std::shared_ptr<Texture>(new Texture(desc));
        texture->setThisPtr(texture);

        texture->initialize();

        return texture;
    }

    void Texture::bind() {
        glBindTexture(getTextureType(), textureId);
    }

    void Texture::write(uint8_t *data, size_t size) {
        bind();

        glTexImage2D(getTextureType(), 0, GL_RGBA, desc.width, desc.height, 0, getTextureFormat(), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(getTextureType());
    }

    void Texture::serialize(std::ostream &os) {
        Resource::serialize(os);

        os << desc.type;
    }

    void Texture::deserialize(std::istream &is) {
        Resource::deserialize(is);

        is >> desc.type;
    }

    int Texture::getTextureType() {
        switch (desc.type) {
        case TextureType::_2d:
            return GL_TEXTURE_2D;
            break;

        default:
            return GL_NONE;
        }
    }
    int Texture::getTextureFormat() {
        switch (desc.format) {
        case PixelFormat::R8:
            return GL_R8;
        case PixelFormat::RG8:
            return GL_RG8;
        case PixelFormat::RGB8:
            return GL_RGB8;
        case PixelFormat::RGBA8:
            return GL_RGBA8;
        case PixelFormat::BGR8:
            return GL_BGR;
        case PixelFormat::BGRA8:
            return GL_BGRA;
        default:
            return GL_NONE;
        }
    }
}
