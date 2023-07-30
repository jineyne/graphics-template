#pragma once

#include "pch.h"

namespace gt {
    class invalid_state : public std::exception {
    public:
        invalid_state(const char *msg);
        invalid_state(const std::string &msg);
    };
}
