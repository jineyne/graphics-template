#pragma once

#include "pch.h"

#include "Importer/SpecificImporter.h"
#include "Utility/Module.h"

namespace gt {
    class TextureManager : public Module<TextureManager>, public SpecificImporter {
    private:
        std::vector<std::string> extensions;

    public:
        std::shared_ptr<Resource> import(const std::string &path, const std::shared_ptr<ImporterOptions> &options) override;

        bool isExtensionSupported(const std::string &ext) const override;
        bool isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t byteCount) const override;

    protected:
        void onStartUp() override;
        void onShutDown() override;

    };
}
