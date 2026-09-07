#include "id_allocator.hpp"

namespace dusk::mods::svc {

void id_allocator_exhausted() {
    CRASH("ID allocator exhausted!");
}

}  // namespace dusk::mods::svc