#ifndef NW4HBM_LYT_BOUNDING_H
#define NW4HBM_LYT_BOUNDING_H

#include "pane.h"

namespace nw4hbm {
    namespace lyt {

        class Bounding : public Pane {
        public:
            Bounding(const res::Bounding* pBlock, const ResBlockSet& resBlockSet);

            /* 0x08 */ virtual ~Bounding();
            /* 0x0C */ NW4HBM_UT_RUNTIME_TYPEINFO;
            /* 0x18 */ virtual void DrawSelf(const DrawInfo& drawInfo);
        };

    }  // namespace lyt
}  // namespace nw4hbm

#endif
