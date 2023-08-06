#pragma once

#include "pch.h"

#include <alc.h>

#include "Importer/SpecificImporter.h"

namespace gt::importers {
    class ALCImporter : public SpecificImporter {
    private:
        ALCdevice *device;
        ALCcontext *context;

    public:
        ALCImporter();
        ~ALCImporter();

    public:
        bool isExtensionSupported(const std::string &ext) const override;
        bool isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t byteCount) const override;

        std::shared_ptr<Resource> import(const std::string &path, const std::shared_ptr<ImporterOptions> &options) override;

    };
}
