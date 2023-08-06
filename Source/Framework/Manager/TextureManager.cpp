#include "TextureManager.h"

#include <filesystem>

#include <FreeImagePlus.h>

#include "Graphics/Texture.h"
#include "Importer/Importer.h"

namespace gt {
    void FreeImageLoadErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
        // Callback method as required by FreeImage to report problems
        const char* typeName = FreeImage_GetFormatFromFIF(fif);
        if (typeName) {
            spdlog::error("FreeImage Error: '{0}' when loading format {1}", message, typeName);
        } else {
            spdlog::error("FreeImage Error: {0}", message);
        }
    }

    std::shared_ptr<Resource> TextureManager::import(const std::string &path, const std::shared_ptr<ImporterOptions> &options) {
        std::filesystem::path p(path);
        if (!std::filesystem::exists(p)) {
            spdlog::error("Unable to find texture: {0}", path);
            return nullptr;
        }

        std::ifstream file(p, std::ios::binary | std::ios::ate);
        if (!file) {
            spdlog::error("Unable to open file: {0}", path);
            return nullptr;
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if (!file.read(buffer.data(), size)) {
            spdlog::error("Failed to read file: {0}", path);
            return nullptr;
        }

        FIMEMORY* memory = FreeImage_OpenMemory(reinterpret_cast<BYTE*>(buffer.data()), buffer.size());
        FREE_IMAGE_FORMAT fiFormat = FreeImage_GetFileTypeFromMemory(memory, 0);
        FIBITMAP* bitmap = FreeImage_LoadFromMemory(fiFormat, memory);

        int width = FreeImage_GetWidth(bitmap);
        int height = FreeImage_GetHeight(bitmap);
        int bpp = FreeImage_GetBPP(bitmap);
        int pitch = FreeImage_GetPitch(bitmap);
        uint8_t *bits = (uint8_t *) FreeImage_GetBits(bitmap);

        int channels = bpp / 8;

        PixelFormat format;
        switch (channels) {
        case 3:
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
            format = PixelFormat::RGB8;
#else
            format = PixelFormat::BGR8;
#endif
            break;

        case 4:
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
            format = PixelFormat::RGBA8;
#else
            format = PixelFormat::BGRA8;
#endif
            break;

        defualt:
            format = PixelFormat::Unknown;
            spdlog::error("Texture channels not support!");
            return nullptr;
        }

        TextureDesc desc {};
        desc.type = TextureType::_2d;
        desc.format = format;

        desc.width = width;
        desc.height = height;

        uint8_t *data = (uint8_t *) malloc(width * height * bpp);

        uint8_t *src, *dst = data;

        for (uint32_t y = 0; y < height; ++y) {
            src = bits + y * pitch;
            memcpy(dst, src, pitch);

            dst += pitch;
        }

        auto texture = Texture::New(desc);
        texture->write(data, width * height * bpp);

        free(data);

        return std::static_pointer_cast<Resource>(texture);
    }

    bool TextureManager::isExtensionSupported(const std::string &ext) const {
        return std::find(extensions.begin(), extensions.end(), ext) != extensions.end();
    }

    bool TextureManager::isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t byteCount) const {
        return true;
    }

    void TextureManager::onStartUp() {
        spdlog::debug("Initialise FreeImage");
        FreeImage_Initialise();

        for (int i = 0; i < FreeImage_GetFIFCount(); ++i){
            if ((FREE_IMAGE_FORMAT) i == FIF_DDS) {
                continue;
            }

            std::string ext = std::string(FreeImage_GetFIFExtensionList((FREE_IMAGE_FORMAT) i));
            std::string temp;
            
            std::stringstream ss(ext);
            while (std::getline(ss, temp, ',')) {
                extensions.push_back(temp);
            }

        }

        FreeImage_SetOutputMessage(FreeImageLoadErrorHandler);

        Importer::Instance().registerAssetImporter(this);
    }

    void TextureManager::onShutDown() {
        spdlog::debug("Deinitialise FreeImage");
        FreeImage_DeInitialise();

        Importer::Instance().unregisterAssetImporter(this);
    }
}
