#include "bounding.h"

#include "common.h"
#include "drawInfo.h"
#include "pane.h"

namespace nw4hbm {
    namespace lyt {

        NW4HBM_UT_GET_DERIVED_RUNTIME_TYPEINFO(Bounding, Pane);

        Bounding::Bounding(const res::Bounding* pBlock, const ResBlockSet&) : Pane(pBlock) {}

        Bounding::~Bounding() {}

        void Bounding::DrawSelf(const DrawInfo& drawInfo) {
            if (drawInfo.IsDebugDrawMode()) {
                LoadMtx(drawInfo);
                detail::DrawLine(GetVtxPos(), mSize, ut::Color(0x00ff00ff));
            }
        }

    }  // namespace lyt
}  // namespace nw4hbm
