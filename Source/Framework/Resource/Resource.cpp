#include "Resource.h"

namespace gt {
    void Resource::initialize() {
        initialized = true;
    }

    void Resource::serialize(std::ostream &os) {
        os << path;
    }

    void Resource::deserialize(std::istream &is) {
        is >> path;
    }
}
