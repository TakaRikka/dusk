#ifndef NW4HBM_LYT_LAYOUT_H
#define NW4HBM_LYT_LAYOUT_H

#include <revolution/mem.h>
#include <revolution/types.h>

#include "animation.h"
#include "drawInfo.h"
#include "group.h"
#include "lyt_types.h"
#include "resourceAccessor.h"


#include "../ut/LinkList.h"
#include "../ut/TagProcessorBase.h"
#include "../ut/WideTagProcessor.h"

#include "macros.h"

namespace nw4hbm {
    namespace lyt {

        class Layout {
        public:
            Layout();

            /* 0x08 */ virtual ~Layout();
            /* 0x0C */ virtual bool Build(const void* lytResBuf, ResourceAccessor* pResAcsr);
            /* 0x10 */ virtual AnimTransform* CreateAnimTransform(const void* anmResBuf,
                                                                  ResourceAccessor* pResAcsr);
            /* 0x14 */ virtual void BindAnimation(AnimTransform* animTrans);
            /* 0x18 */ virtual void UnbindAnimation(AnimTransform* animTrans);
            /* 0x1C */ virtual void UnbindAllAnimation();
            /* 0x20 */ virtual void SetAnimationEnable(AnimTransform* animTrans,
                                                       bool bEnable = true);
            /* 0x24 */ virtual void CalculateMtx(const DrawInfo& drawInfo);
            /* 0x28 */ virtual void Draw(const DrawInfo& drawInfo);
            /* 0x2C */ virtual void Animate(u32 option = 0);
            /* 0x30 */ virtual void SetTagProcessor(ut::WideTagProcessor* pTagProcessor) NO_INLINE;

            const ut::Rect GetLayoutRect() const;
            Pane* GetRootPane() const { return mpRootPane; }
            GroupContainer* GetGroupContainer() const { return mpGroupContainer; }

            static MEMAllocator* GetAllocator() { return mspAllocator; }
            static void SetAllocator(MEMAllocator* allocator) { mspAllocator = allocator; }

            static void* AllocMemory(u32 size) { return MEMAllocFromAllocator(mspAllocator, size); }
            static void FreeMemory(void* ptr) { MEMFreeToAllocator(mspAllocator, ptr); }

            static Pane* BuildPaneObj(s32 kind, const void* dataPtr,
                                      const ResBlockSet& resBlockSet) NO_INLINE;

        private:
            /* 0x00 (vtable) */
            /* 0x04 */ AnimTransformList mAnimTransList;
            /* 0x10 */ Pane* mpRootPane;
            /* 0x14 */ GroupContainer* mpGroupContainer;
            /* 0x18 */ Size mLayoutSize;
            /* 0x20 */ u8 mOriginType;

            static MEMAllocator* mspAllocator;
        };

    }  // namespace lyt
}  // namespace nw4hbm

#endif
