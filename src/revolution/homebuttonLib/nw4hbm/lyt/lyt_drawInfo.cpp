#include "drawInfo.h"

#include <string>

namespace nw4hbm {
    namespace lyt {

        DrawInfo::DrawInfo() : mLocationAdjustScale(1.0f, 1.0f), mGlobalAlpha(1.0f) {
            std::memset(&mFlag, 0, sizeof(mFlag));

            math::MTX34Identity(&mViewMtx);
        }

        DrawInfo::~DrawInfo() {}

    }  // namespace lyt
}  // namespace nw4hbm
