#include "ALCImporter.h"

#include "Utility/Exceptions.h"

namespace gt::importers {
    ALCImporter::ALCImporter() {
        spdlog::debug("Initialize OpenAL");
        device = alcOpenDevice(NULL);
        if (!device) {
            throw invalid_state("Failed to create openal device");
        }

        context = alcCreateContext(device, NULL);
        if (!context) {
            throw invalid_state("Failed to create openal context");
        }

        if (!alcMakeContextCurrent(context)) {
            throw invalid_state("Failed to set openal context");
        } 
    }

    ALCImporter::~ALCImporter() {
        spdlog::debug("Deinitialise OpenAL");
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        alcCloseDevice(device);
    }

    bool ALCImporter::isExtensionSupported(const std::string &ext) const {
        return false;
    }

    bool ALCImporter::isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t byteCount) const {
        return false;
    }

    std::shared_ptr<Resource> ALCImporter::import(const std::string &path, const std::shared_ptr<ImporterOptions> &options) {
        return nullptr;
    }
}
