#pragma once

#include <stdexcept>

#include "../utility/log.hpp"

#define RUNTIME_ERROR(msg) std::runtime_error(std::string(msg) + " on line " TOSTRING(__LINE__) " of " __FILENAME__)
