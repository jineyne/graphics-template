#pragma once

#include "pch.h"

#include "Resource/Resource.h"

namespace gt {
    struct ImporterOptions {
    };

    class SpecificImporter {
    public:
        virtual ~SpecificImporter() = default;

    public:
        virtual bool isExtensionSupported(const std::string &ext) const = 0;
        virtual bool isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t byteCount) const = 0;

        virtual std::shared_ptr<Resource> import(const std::string &path, const std::shared_ptr<ImporterOptions> &options) = 0;
    };
}
