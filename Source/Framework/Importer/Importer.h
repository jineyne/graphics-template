#pragma once

#include "pch.h"

#include "Utility/Module.h"

#include "SpecificImporter.h"

namespace gt {
    class Importer : public Module<Importer> {
    private:
        std::vector<SpecificImporter *> importers;

    public:
        ~Importer();

    public:
        std::shared_ptr<Resource> import(const std::string &path, const std::shared_ptr<ImporterOptions> &options = nullptr);

        template <class T>
        std::shared_ptr<T> import(const std::string &path, const std::shared_ptr<ImporterOptions> &options = nullptr) {
            return std::static_pointer_cast<T>(import(path, options));
        }

        bool supportsFileType(const std::string &extension) const;
        void registerAssetImporter(SpecificImporter *importer);
        void unregisterAssetImporter(SpecificImporter *importer);

    private:
        SpecificImporter *getImporterForFile(const std::string &path) const;
        SpecificImporter *prepareForImport(const std::string &path) const;
    };
}
