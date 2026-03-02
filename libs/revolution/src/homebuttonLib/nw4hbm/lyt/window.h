#ifndef NW4HBM_LYT_WINDOW_H
#define NW4HBM_LYT_WINDOW_H

#include "common.h"
#include "pane.h"
#include "resources.h"

namespace nw4hbm {
    namespace lyt {

        class Window : public Pane {
        private:
            typedef struct Content {
                /* 0x00 */ ut::Color vtxColors[4];
                /* 0x10 */ detail::TexCoordAry texCoordAry;
            } Content;

            typedef struct Frame {
                /* 0x00 */ u8 textureFlip;
                /* 0x04 */ Material* pMaterial;
            } Frame;

        public:
            Window(const res::Window* pBlock, const ResBlockSet& resBlockSet);

            /* 0x08 */ virtual ~Window();
            /* 0x0C */ NW4HBM_UT_RUNTIME_TYPEINFO;
            /* 0x18 */ virtual void DrawSelf(const DrawInfo& drawInfo);
            /* 0x20 */ virtual void AnimateSelf(u32 option);
            /* 0x24 */ virtual ut::Color GetVtxColor(u32 idx) const;
            /* 0x28 */ virtual void SetVtxColor(u32 idx, ut::Color value);
            /* 0x34 */ virtual u8 GetVtxColorElement(u32 idx) const;
            /* 0x38 */ virtual void SetVtxColorElement(u32 idx, u8 value);
            /* 0x40 */ virtual Material* FindMaterialByName(const char* findName, bool bRecursive);
            /* 0x50 */ virtual void UnbindAnimationSelf(AnimTransform* animTrans);
            /* 0x54 */ virtual AnimationLink* FindAnimationLink(AnimTransform* animTrans);
            /* 0x58 */ virtual void SetAnimationEnable(AnimTransform* animTrans, bool bEnable,
                                                       bool bRecursive);
            /* 0x64 */ virtual Material* GetContentMaterial() const;
            /* 0x68 */ virtual Material* GetFrameMaterial(u32 frameIdx) const;
            /* 0x6C */ virtual void DrawContent(const math::VEC2& basePt,
                                                const WindowFrameSize& frameSize, u8 alpha);
            /* 0x70 */ virtual void DrawFrame(const math::VEC2& basePt, const Frame& frame,
                                              const WindowFrameSize& frameSize, u8 alpha);
            /* 0x74 */ virtual void DrawFrame4(const math::VEC2& basePt, const Frame* frames,
                                               const WindowFrameSize& frameSize, u8 alpha);
            /* 0x78 */ virtual void DrawFrame8(const math::VEC2& basePt, const Frame* frames,
                                               const WindowFrameSize& frameSize, u8 alpha);

            WindowFrameSize GetFrameSize(u8 frameNum, const Frame* frames);

        private:
            /* 0x00 (base) */
            /* 0x0D4 */ InflationLRTB mContentInflation;
            /* 0x0E4 */ Content mContent;
            /* 0x0FC */ Frame* mFrames;
            /* 0x100 */ u8 mFrameNum;
        };  // size = 0x104

    }  // namespace lyt
}  // namespace nw4hbm

#endif
