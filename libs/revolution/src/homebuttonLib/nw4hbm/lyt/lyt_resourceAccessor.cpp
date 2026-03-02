#include "resourceAccessor.h"

namespace nw4hbm {
    namespace lyt {

        ResourceAccessor::~ResourceAccessor() {}

        ResourceAccessor::ResourceAccessor() {}

        ut::Font* ResourceAccessor::GetFont(const char*) {
            return NULL;
        }

    }  // namespace lyt
}  // namespace nw4hbm
