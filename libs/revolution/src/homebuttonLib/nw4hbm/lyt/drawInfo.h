#ifndef NW4HBM_LYT_DRAW_INFO_H
#define NW4HBM_LYT_DRAW_INFO_H

#include <revolution/gx.h>

#include "../math/types.h"

#include "../ut/Rect.h"

namespace nw4hbm {
    namespace lyt {

        class DrawInfo {
        public:
            DrawInfo();

            /* 0x08 */ virtual ~DrawInfo();

            void SetViewRect(const ut::Rect& rect) { mViewRect = rect; }

            const math::MTX34& GetViewMtx() const { return mViewMtx; }
            void SetViewMtx(const math::MTX34& value) { mViewMtx = value; }

            const math::VEC2& GetLocationAdjustScale() const { return mLocationAdjustScale; }
            void SetLocationAdjustScale(const math::VEC2& scale) { mLocationAdjustScale = scale; }

            bool IsMultipleViewMtxOnDraw() const { return mFlag.mulViewDraw; }
            void SetMultipleViewMtxOnDraw(bool bEnable) { mFlag.mulViewDraw = bEnable; }

            bool IsInfluencedAlpha() const { return mFlag.influencedAlpha; }
            void SetInfluencedAlpha(bool bEnable) { mFlag.influencedAlpha = bEnable; }

            bool IsLocationAdjust() const { return mFlag.locationAdjust; }
            void SetLocationAdjust(bool bEnable) { mFlag.locationAdjust = bEnable; }

            bool IsInvisiblePaneCalculateMtx() const { return mFlag.invisiblePaneCalculateMtx; }
            void SetInvisiblePaneCalculateMtx(bool bEnable) {
                mFlag.invisiblePaneCalculateMtx = bEnable;
            }

            bool IsDebugDrawMode() const { return mFlag.debugDrawMode; }
            void SetDebugDrawMode(bool bEnable) { mFlag.debugDrawMode = bEnable; }

            bool IsYAxisUp() const { return mViewRect.bottom - mViewRect.top < 0.0f; }

            f32 GetGlobalAlpha() const { return mGlobalAlpha; }
            void SetGlobalAlpha(f32 alpha) { mGlobalAlpha = alpha; }

        protected:
            /* 0x00 (vtable) */
            /* 0x04 */ math::MTX34 mViewMtx;
            /* 0x34 */ ut::Rect mViewRect;
            /* 0x44 */ math::VEC2 mLocationAdjustScale;
            /* 0x4C */ f32 mGlobalAlpha;
            /* 0x50 */ struct {
                u8 mulViewDraw : 1;                // 10000000
                u8 influencedAlpha : 1;            // 01000000
                u8 locationAdjust : 1;             // 00100000
                u8 invisiblePaneCalculateMtx : 1;  // 00010000
                u8 debugDrawMode : 1;              // 00001000
            } mFlag;
        };

    }  // namespace lyt
}  // namespace nw4hbm

#endif
