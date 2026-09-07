#include "helpers/cast.hpp"
#include "global.h"

namespace dusk::helpers::cast::_impl {

void overrun_high() {
    CRASH("bounded cast overran!");
}

void overrun_low() {
    CRASH("bounded cast overran!");
}

}  // namespace dusk::helpers::cast::_impl
