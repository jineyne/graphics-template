#pragma once

#include "pch.h"

namespace gt {
    class ISerializable {
    public:
        virtual void serialize(std::ostream &os) = 0;
        virtual void deserialize(std::istream &is) = 0;
    };
}
