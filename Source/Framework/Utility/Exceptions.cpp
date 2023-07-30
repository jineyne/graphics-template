#include "Exceptions.h"

namespace gt {
    invalid_state::invalid_state(const char *msg) : std::exception(msg) { }

    invalid_state::invalid_state(const std::string &msg) : invalid_state(msg.c_str()) { }
}
