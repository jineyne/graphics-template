#pragma once

#include "pch.h"
#include "Utility/ISerializable.h"

namespace gt {
    class Resource : public ISerializable {
    private:
        std::string path;
        bool initialized;

    public:
        virtual void initialize();
        bool isInitialized() const { return initialized; }

        virtual void serialize(std::ostream &os);
        virtual void deserialize(std::istream &is);

    private:
        friend class Resources;

        DEFINE_THIS_PTR(Resource);
    };
}
