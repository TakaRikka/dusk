#ifndef NW4HBM_LYT_ANIMATION_H
#define NW4HBM_LYT_ANIMATION_H

#include "lyt_types.h"
#include "resourceAccessor.h"
#include "resources.h"


#define TexMtxMax 10
#define IndTexMtxMax 3

namespace nw4hbm {
    namespace lyt {

        class Pane;
        class Material;

        enum {
            /* 0 */ ANIMTARGET_PANE_TRANSX = 0,
            /* 1 */ ANIMTARGET_PANE_TRANSY,
            /* 2 */ ANIMTARGET_PANE_TRANSZ,

            /* 3 */ ANIMTARGET_PANE_ROTX,
            /* 4 */ ANIMTARGET_PANE_ROTY,
            /* 5 */ ANIMTARGET_PANE_ROTZ,

            /* 6 */ ANIMTARGET_PANE_SCALEX,
            /* 7 */ ANIMTARGET_PANE_SCALEY,

            /* 8 */ ANIMTARGET_PANE_SIZEX,
            /* 9 */ ANIMTARGET_PANE_SIZEY,

            /* 10 */ ANIMTARGET_PANE_MAX,
            /* 16 */ ANIMTARGET_PANE_COLOR_ALPHA = 16,
            /* 17 */ ANIMTARGET_PANE_COLOR_MAX,
        };

        enum {
            /* 0 */ ANIMTARGET_VERTEXCOLOR_LT_RED = 0,
            /* 1 */ ANIMTARGET_VERTEXCOLOR_LT_GREEN,
            /* 2 */ ANIMTARGET_VERTEXCOLOR_LT_BLUE,
            /* 3 */ ANIMTARGET_VERTEXCOLOR_LT_ALPHA,

            /* 4 */ ANIMTARGET_VERTEXCOLOR_RT_RED,
            /* 5 */ ANIMTARGET_VERTEXCOLOR_RT_GREEN,
            /* 6 */ ANIMTARGET_VERTEXCOLOR_RT_BLUE,
            /* 7 */ ANIMTARGET_VERTEXCOLOR_RT_ALPHA,

            /* 8 */ ANIMTARGET_VERTEXCOLOR_LB_RED,
            /* 9 */ ANIMTARGET_VERTEXCOLOR_LB_GREEN,
            /* 10 */ ANIMTARGET_VERTEXCOLOR_LB_BLUE,
            /* 11 */ ANIMTARGET_VERTEXCOLOR_LB_ALPHA,

            /* 12 */ ANIMTARGET_VERTEXCOLOR_RB_RED,
            /* 13 */ ANIMTARGET_VERTEXCOLOR_RB_GREEN,
            /* 14 */ ANIMTARGET_VERTEXCOLOR_RB_BLUE,
            /* 15 */ ANIMTARGET_VERTEXCOLOR_RB_ALPHA,

            /* 16 */ ANIMTARGET_VERTEXCOLOR_MAX
        };

        enum {
            /* 0 */ ANIMTARGET_MATCOLOR_MATR = 0,
            /* 1 */ ANIMTARGET_MATCOLOR_MATG,
            /* 2 */ ANIMTARGET_MATCOLOR_MATB,
            /* 3 */ ANIMTARGET_MATCOLOR_MATA,

            /* 4 */ ANIMTARGET_MATCOLOR_TEV0R,
            /* 5 */ ANIMTARGET_MATCOLOR_TEV0G,
            /* 6 */ ANIMTARGET_MATCOLOR_TEV0B,
            /* 7 */ ANIMTARGET_MATCOLOR_TEV0A,

            /* 8 */ ANIMTARGET_MATCOLOR_TEV1R,
            /* 9 */ ANIMTARGET_MATCOLOR_TEV1G,
            /* 10 */ ANIMTARGET_MATCOLOR_TEV1B,
            /* 11 */ ANIMTARGET_MATCOLOR_TEV1A,

            /* 12 */ ANIMTARGET_MATCOLOR_TEV2R,
            /* 13 */ ANIMTARGET_MATCOLOR_TEV2G,
            /* 14 */ ANIMTARGET_MATCOLOR_TEV2B,
            /* 15 */ ANIMTARGET_MATCOLOR_TEV2A,

            /* 16 */ ANIMTARGET_MATCOLOR_TEVK0R,
            /* 17 */ ANIMTARGET_MATCOLOR_TEVK0G,
            /* 18 */ ANIMTARGET_MATCOLOR_TEVK0B,
            /* 19 */ ANIMTARGET_MATCOLOR_TEVK0A,

            /* 20 */ ANIMTARGET_MATCOLOR_TEVK1R,
            /* 21 */ ANIMTARGET_MATCOLOR_TEVK1G,
            /* 22 */ ANIMTARGET_MATCOLOR_TEVK1B,
            /* 23 */ ANIMTARGET_MATCOLOR_TEVK1A,

            /* 24 */ ANIMTARGET_MATCOLOR_TEVK2R,
            /* 25 */ ANIMTARGET_MATCOLOR_TEVK2G,
            /* 26 */ ANIMTARGET_MATCOLOR_TEVK2B,
            /* 27 */ ANIMTARGET_MATCOLOR_TEVK2A,

            /* 28 */ ANIMTARGET_MATCOLOR_TEVK3R,
            /* 29 */ ANIMTARGET_MATCOLOR_TEVK3G,
            /* 30 */ ANIMTARGET_MATCOLOR_TEVK3B,
            /* 31 */ ANIMTARGET_MATCOLOR_TEVK3A,

            /* 32 */ ANIMTARGET_MATCOLOR_MAX
        };

        enum {
            /* 0 */ ANIMTARGET_TEXSRT_TRANSX = 0,
            /* 1 */ ANIMTARGET_TEXSRT_TRANSY,
            /* 2 */ ANIMTARGET_TEXSRT_ROT,
            /* 3 */ ANIMTARGET_TEXSRT_SCALEX,
            /* 4 */ ANIMTARGET_TEXSRT_SCALEY,
            /* 5 */ ANIMTARGET_TEXSRT_MAX
        };

        enum {
            /* 0 */ ANIMTARGET_TEXPATTURN_IMAGE = 0,
            /* 1 */ ANIMTARGET_TEXPATTURN_MAX
        };

        enum {
            /* 0 */ ANIMCURVE_NONE = 0,
            /* 1 */ ANIMCURVE_STEP,
            /* 2 */ ANIMCURVE_HERMITE,
            /* 3 */ ANIMCURVE_MAX
        };

        class AnimTransform {
        public:
            AnimTransform();

            /* 0x08 */ virtual ~AnimTransform();
            /* 0x0C */ virtual void SetResource(const res::AnimationBlock* pRes,
                                                ResourceAccessor* pResAccessor) = 0;
            /* 0x10 */ virtual void Bind(Pane* pane, bool bRecursive) = 0;
            /* 0x14 */ virtual void Bind(Material* pMaterial) = 0;
            /* 0x18 */ virtual void Animate(u32 idx, Pane* pane) = 0;
            /* 0x1C */ virtual void Animate(u32 idx, Material* pMaterial) = 0;

            f32 GetFrameMax() const { return GetFrameSize(); }
            u16 GetFrameSize() const;

            void SetFrame(f32 frame) { mFrame = frame; }

            bool IsLoopData() const;

            /* 0x00 (vtable) */
            /* 0x04 */ ut::LinkListNode mLink;

        protected:
            /* 0x0C */ const res::AnimationBlock* mpRes;
            /* 0x10 */ f32 mFrame;
        };  // size = 0x14
        typedef ut::LinkList<AnimTransform, offsetof(AnimTransform, mLink)> AnimTransformList;

        class AnimationLink {
        public:
            AnimationLink() : mLink(), mbDisable(false) { Reset(); }
            ~AnimationLink() {}

            AnimTransform* GetAnimTransform() const { return mAnimTrans; }

            u16 GetIndex() const { return mIdx; }
            bool IsEnable() const { return !mbDisable; }

            void SetEnable(bool bEnable) { mbDisable = !bEnable; }

            void Reset() { SetAnimTransform(NULL, 0); }

            void SetAnimTransform(AnimTransform* animTrans, u16 idx) {
                mAnimTrans = animTrans;
                mIdx = idx;
            }

            /* 0x00 */ ut::LinkListNode mLink;

        private:
            /* 0x08 */ AnimTransform* mAnimTrans;
            /* 0x0C */ u16 mIdx;
            /* 0x0E */ bool mbDisable;
        };  // size = 0x10
        typedef ut::LinkList<AnimationLink, offsetof(AnimationLink, mLink)> AnimationLinkList;

        class AnimTransformBasic : public AnimTransform {
        public:
            AnimTransformBasic();

            /* 0x08 */ virtual ~AnimTransformBasic();
            /* 0x0C */ virtual void SetResource(const res::AnimationBlock* pRes,
                                                ResourceAccessor* pResAccessor);
            /* 0x10 */ virtual void Bind(Pane* pane, bool bRecursive);
            /* 0x14 */ virtual void Bind(Material* pMaterial);
            /* 0x18 */ virtual void Animate(u32 idx, Pane* pane);
            /* 0x1C */ virtual void Animate(u32 idx, Material* pMaterial);

        private:
            /* 0x00 (base) */
            /* 0x14 */ void** mpFileResAry;
            /* 0x18 */ AnimationLink* mAnimLinkAry;
            /* 0x1C */ u16 mAnimLinkNum;
        };  // size = 0x20

        namespace detail {
            AnimationLink* FindAnimationLink(AnimationLinkList* animList, AnimTransform* animTrans);
        }

    }  // namespace lyt
}  // namespace nw4hbm

#endif
