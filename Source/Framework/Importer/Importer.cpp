#include "Importer.h"

#include <filesystem>

#include <spdlog/spdlog.h>

namespace gt {

    Importer::~Importer() {
        for (auto &importer : importers) {
            if (importer != nullptr)
                delete importer;
        }

        importers.clear();
    }
    
    std::shared_ptr<Resource> Importer::import(const std::string &path, const std::shared_ptr<ImporterOptions> &options) {
        auto *importer = prepareForImport(path);
        if (importer == nullptr) {
            return nullptr;
        }

        auto output = importer->import(path, options);
        return output;
    }

    bool Importer::supportsFileType(const std::string &extension) const {
        for (const auto &importer : importers) {
            if (importer != nullptr && importer->isExtensionSupported(extension)) {
                return true;
            }
        }

        return false;
    }

    void Importer::registerAssetImporter(SpecificImporter *importer) {
        if (!importer) {
            spdlog::warn("Trying to register a null asset importer!");
            return;
        }

        importers.push_back(importer);
    }

    void Importer::unregisterAssetImporter(SpecificImporter *importer) {
        auto it = std::find(importers.begin(), importers.end(), importer);
        if (it != importers.end()) {
            importers.erase(it);
        }
    }

    SpecificImporter *Importer::getImporterForFile(const std::string &path) const {
        std::string ext = path.substr(path.find_last_of(".") + 1);
        if (ext.empty()) {
            return nullptr;
        }

        if (!supportsFileType(ext)) {
            spdlog::warn("There is no importer for the provided file type. ({0})", path);
            return nullptr;
        }

        for (const auto &importer : importers) {
            if (importer != nullptr && importer->isExtensionSupported(ext)) {
                return importer;
            }
        }

        return nullptr;
    }

    SpecificImporter *Importer::prepareForImport(const std::string &path) const {
        std::filesystem::path p(path);
        if (!std::filesystem::is_regular_file(p)) {
            spdlog::warn("Trying to import asset that doesn't exists. Asset path: {0}", path);
            return nullptr;
        }

        auto *importer = getImporterForFile(path);
        if (importer == nullptr) {
            return nullptr;
        }

        return importer;
    }
}
