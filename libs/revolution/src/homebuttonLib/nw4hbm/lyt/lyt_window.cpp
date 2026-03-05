#include "window.h"

#include "layout.h"

#include <new>

struct TextureFlipInfo {
    /* 0x00 */ u8 coords[4][2];
    /* 0x08 */ u8 idx[2];
};  // size = 0x0A

namespace {
    // pretend this is nw4hbm::lyt
    using namespace nw4hbm;
    using namespace nw4hbm::lyt;

    // NOTE the misspelling of GetTextureFlipInfo
    TextureFlipInfo& GetTexutreFlipInfo(u8 textureFlip);

    void GetLTFrameSize(math::VEC2* pPt, Size* pSize, const math::VEC2& basePt, const Size& winSize,
                        const WindowFrameSize& frameSize);
    void GetLTTexCoord(math::VEC2* texCds, const Size& polSize, const Size& texSize,
                       u8 textureFlip);
    void GetRTFrameSize(math::VEC2* pPt, Size* pSize, const math::VEC2& basePt, const Size& winSize,
                        const WindowFrameSize& frameSize);
    void GetRTTexCoord(math::VEC2* texCds, const Size& polSize, const Size& texSize,
                       u8 textureFlip);
    void GetLBFrameSize(math::VEC2* pPt, Size* pSize, const math::VEC2& basePt, const Size& winSize,
                        const WindowFrameSize& frameSize);
    void GetLBTexCoord(math::VEC2* texCds, const Size& polSize, const Size& texSize,
                       u8 textureFlip);
    void GetRBFrameSize(math::VEC2* pPt, Size* pSize, const math::VEC2& basePt, const Size& winSize,
                        const WindowFrameSize& frameSize);
    void GetRBTexCoord(math::VEC2* texCds, const Size& polSize, const Size& texSize,
                       u8 textureFlip);
}  // unnamed namespace

namespace nw4hbm {
    namespace lyt {

        NW4HBM_UT_GET_DERIVED_RUNTIME_TYPEINFO(Window, Pane);

    }  // namespace lyt
}  // namespace nw4hbm

//! TODO clean up

namespace {

    TextureFlipInfo& GetTexutreFlipInfo(u8 textureFlip) {
        // clang-format off
    static TextureFlipInfo flipInfos[] =            //    0    1    2    3
    {                                    // in order of    LT    RT    LB    RB
        {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}, {0, 1}},    //    0    1    2    3                no flip
        {{{1, 0}, {0, 0}, {1, 1}, {0, 1}}, {0, 1}},    //    1    0    3    2                horizontal flip
        {{{0, 1}, {1, 1}, {0, 0}, {1, 0}}, {0, 1}},    //    2    3    0    1                vertical flip
        {{{0, 1}, {0, 0}, {1, 1}, {1, 0}}, {1, 0}},    //    2    0    3    1, index flip    cw  90 deg
        {{{1, 1}, {0, 1}, {1, 0}, {0, 0}}, {0, 1}},    //    3    2    1    0                cw 180 deg
        {{{1, 0}, {1, 1}, {0, 0}, {0, 1}}, {1, 0}}    //    1    3    0    2, index flip    cw 270 deg (ccw 90 deg)
    };
        // clang-format on

        NW4HBM_ASSERT(50, textureFlip < TEXTUREFLIP_MAX);
        return flipInfos[textureFlip];
    }

    void GetLTFrameSize(math::VEC2* pPt, Size* pSize, const math::VEC2& basePt, const Size& winSize,
                        const WindowFrameSize& frameSize) {
        *pPt = basePt;
        pSize->width = winSize.width - frameSize.r;
        pSize->height = frameSize.t;
    }

    void GetLTTexCoord(math::VEC2* texCds, const Size& polSize, const Size& texSize,
                       u8 textureFlip) {
        TextureFlipInfo& flipInfo = GetTexutreFlipInfo(textureFlip);
        int ix = flipInfo.idx[FLIPINDEX_X];
        int iy = flipInfo.idx[FLIPINDEX_Y];

        const math::VEC2 tSz(texSize.width, texSize.height);

        texCds[VERTEXCOLOR_LT][ix] = texCds[VERTEXCOLOR_LB][ix] =
            flipInfo.coords[VERTEXCOLOR_LT][ix];

        texCds[VERTEXCOLOR_LT][iy] = texCds[VERTEXCOLOR_RT][iy] =
            flipInfo.coords[VERTEXCOLOR_LT][iy];

        texCds[VERTEXCOLOR_RB][ix] = texCds[VERTEXCOLOR_RT][ix] =
            flipInfo.coords[VERTEXCOLOR_LT][ix] +
            polSize.width /
                ((flipInfo.coords[VERTEXCOLOR_RT][ix] - flipInfo.coords[VERTEXCOLOR_LT][ix]) *
                 tSz[ix]);

        texCds[VERTEXCOLOR_RB][iy] = texCds[VERTEXCOLOR_LB][iy] =
            flipInfo.coords[VERTEXCOLOR_LT][iy] +
            polSize.height /
                ((flipInfo.coords[VERTEXCOLOR_LB][iy] - flipInfo.coords[VERTEXCOLOR_LT][iy]) *
                 tSz[iy]);
    }

    void GetRTFrameSize(math::VEC2* pPt, Size* pSize, const math::VEC2& basePt, const Size& winSize,
                        const WindowFrameSize& frameSize) {
        *pPt = math::VEC2(basePt.x + winSize.width - frameSize.r, basePt.y);
        pSize->width = frameSize.r;
        pSize->height = winSize.height - frameSize.b;
    }

    void GetRTTexCoord(math::VEC2* texCds, const Size& polSize, const Size& texSize,
                       u8 textureFlip) {
        TextureFlipInfo& flipInfo = GetTexutreFlipInfo(textureFlip);
        int ix = flipInfo.idx[FLIPINDEX_X];
        int iy = flipInfo.idx[FLIPINDEX_Y];

        const math::VEC2 tSz(texSize.width, texSize.height);

        texCds[VERTEXCOLOR_RT][ix] = texCds[VERTEXCOLOR_RB][ix] =
            flipInfo.coords[VERTEXCOLOR_RT][ix];

        texCds[VERTEXCOLOR_RT][iy] = texCds[VERTEXCOLOR_LT][iy] =
            flipInfo.coords[VERTEXCOLOR_RT][iy];

        texCds[VERTEXCOLOR_LB][ix] = texCds[VERTEXCOLOR_LT][ix] =
            flipInfo.coords[VERTEXCOLOR_RT][ix] +
            polSize.width /
                ((flipInfo.coords[VERTEXCOLOR_LT][ix] - flipInfo.coords[VERTEXCOLOR_RT][ix]) *
                 tSz[ix]);

        texCds[VERTEXCOLOR_LB][iy] = texCds[VERTEXCOLOR_RB][iy] =
            flipInfo.coords[VERTEXCOLOR_RT][iy] +
            polSize.height /
                ((flipInfo.coords[VERTEXCOLOR_RB][iy] - flipInfo.coords[VERTEXCOLOR_RT][iy]) *
                 tSz[iy]);
    }

    void GetLBFrameSize(math::VEC2* pPt, Size* pSize, const math::VEC2& basePt, const Size& winSize,
                        const WindowFrameSize& frameSize) {
        *pPt = math::VEC2(basePt.x, basePt.y + frameSize.t);
        pSize->width = frameSize.l;
        pSize->height = winSize.height - frameSize.t;
    }

    void GetLBTexCoord(math::VEC2* texCds, const Size& polSize, const Size& texSize,
                       u8 textureFlip) {
        TextureFlipInfo& flipInfo = GetTexutreFlipInfo(textureFlip);
        int ix = flipInfo.idx[FLIPINDEX_X];
        int iy = flipInfo.idx[FLIPINDEX_Y];

        const math::VEC2 tSz(texSize.width, texSize.height);

        texCds[VERTEXCOLOR_LB][ix] = texCds[VERTEXCOLOR_LT][ix] =
            flipInfo.coords[VERTEXCOLOR_LB][ix];

        texCds[VERTEXCOLOR_LB][iy] = texCds[VERTEXCOLOR_RB][iy] =
            flipInfo.coords[VERTEXCOLOR_LB][iy];

        texCds[VERTEXCOLOR_RT][ix] = texCds[VERTEXCOLOR_RB][ix] =
            flipInfo.coords[VERTEXCOLOR_LB][ix] +
            polSize.width /
                ((flipInfo.coords[VERTEXCOLOR_RB][ix] - flipInfo.coords[VERTEXCOLOR_LB][ix]) *
                 tSz[ix]);

        texCds[VERTEXCOLOR_RT][iy] = texCds[VERTEXCOLOR_LT][iy] =
            flipInfo.coords[VERTEXCOLOR_LB][iy] +
            polSize.height /
                ((flipInfo.coords[VERTEXCOLOR_LT][iy] - flipInfo.coords[VERTEXCOLOR_LB][iy]) *
                 tSz[iy]);
    }

    void GetRBFrameSize(math::VEC2* pPt, Size* pSize, const math::VEC2& basePt, const Size& winSize,
                        const WindowFrameSize& frameSize) {
        *pPt = math::VEC2(basePt.x + frameSize.l, basePt.y + winSize.height - frameSize.b);
        pSize->width = winSize.width - frameSize.l;
        pSize->height = frameSize.b;
    }

    void GetRBTexCoord(math::VEC2* texCds, const Size& polSize, const Size& texSize,
                       u8 textureFlip) {
        TextureFlipInfo& flipInfo = GetTexutreFlipInfo(textureFlip);
        int ix = flipInfo.idx[FLIPINDEX_X];
        int iy = flipInfo.idx[FLIPINDEX_Y];

        const math::VEC2 tSz(texSize.width, texSize.height);

        texCds[VERTEXCOLOR_RB][ix] = texCds[VERTEXCOLOR_RT][ix] =
            flipInfo.coords[VERTEXCOLOR_RB][ix];

        texCds[VERTEXCOLOR_RB][iy] = texCds[VERTEXCOLOR_LB][iy] =
            flipInfo.coords[VERTEXCOLOR_RB][iy];

        texCds[VERTEXCOLOR_LT][ix] = texCds[VERTEXCOLOR_LB][ix] =
            flipInfo.coords[VERTEXCOLOR_RB][ix] +
            polSize.width /
                ((flipInfo.coords[VERTEXCOLOR_LB][ix] - flipInfo.coords[VERTEXCOLOR_RB][ix]) *
                 tSz[ix]);

        texCds[VERTEXCOLOR_LT][iy] = texCds[VERTEXCOLOR_RT][iy] =
            flipInfo.coords[VERTEXCOLOR_RB][iy] +
            polSize.height /
                ((flipInfo.coords[VERTEXCOLOR_RT][iy] - flipInfo.coords[VERTEXCOLOR_RB][iy]) *
                 tSz[iy]);
    }

}  // unnamed namespace

namespace nw4hbm {
    namespace lyt {

        Window::Window(const res::Window* pBlock, const ResBlockSet& resBlockSet) : Pane(pBlock) {
            mContentInflation = pBlock->inflation;

            NW4HBM_ASSERT_CHECK_NULL(193, resBlockSet.pMaterialList);
            const u32* const matOffsTbl = detail::ConvertOffsToPtr<u32>(
                resBlockSet.pMaterialList, sizeof(*resBlockSet.pMaterialList));

            const res::WindowContent* pResContent =
                detail::ConvertOffsToPtr<res::WindowContent>(pBlock, pBlock->contentOffset);

            for (int i = 0; i < (int)ARRAY_SIZE(mContent.vtxColors); i++) {
                mContent.vtxColors[i] = pResContent->vtxCols[i];
            }

            if (pResContent->texCoordNum) {
                u8 texCoordNum = ut::Min<u8>(pResContent->texCoordNum, 8);
                mContent.texCoordAry.Reserve(texCoordNum);

                if (!mContent.texCoordAry.IsEmpty()) {
                    mContent.texCoordAry.Copy(&pResContent[1], texCoordNum);
                }
            }

            if (void* pMemMaterial = Layout::AllocMemory(sizeof(Material))) {
                const res::Material* pResMaterial = detail::ConvertOffsToPtr<res::Material>(
                    resBlockSet.pMaterialList, matOffsTbl[pResContent->materialIdx]);

                mpMaterial = new (pMemMaterial) Material(pResMaterial, resBlockSet);
            }

            mFrameNum = 0;
            mFrames = NULL;

            if (pBlock->frameNum) {
                if ((mFrames = static_cast<Frame*>(
                         Layout::AllocMemory(sizeof(*mFrames) * pBlock->frameNum))))
                {
                    mFrameNum = pBlock->frameNum;
                    const u32* frameOffsetTable =
                        detail::ConvertOffsToPtr<u32>(pBlock, pBlock->frameOffsetTableOffset);

                    for (int i = 0; i < mFrameNum; i++) {
                        const res::WindowFrame* pResWindowFrame =
                            detail::ConvertOffsToPtr<res::WindowFrame>(pBlock, frameOffsetTable[i]);

                        mFrames[i].textureFlip = pResWindowFrame->textureFlip;
                        mFrames[i].pMaterial = NULL;

                        if (void* pMemMaterial = Layout::AllocMemory(sizeof(Material))) {
                            const res::Material* pResMaterial =
                                detail::ConvertOffsToPtr<res::Material>(
                                    resBlockSet.pMaterialList,
                                    matOffsTbl[pResWindowFrame->materialIdx]);

                            mFrames[i].pMaterial =
                                new (pMemMaterial) Material(pResMaterial, resBlockSet);
                        }
                    }
                }
            }
        }

        Window::~Window() {
            if (mFrames) {
                for (int i = 0; i < mFrameNum; i++) {
                    mFrames[i].pMaterial->~Material();
                    Layout::FreeMemory(mFrames[i].pMaterial);
                }

                Layout::FreeMemory(mFrames);
            }

            if (mpMaterial && !mpMaterial->IsUserAllocated()) {
                mpMaterial->~Material();
                Layout::FreeMemory(mpMaterial);
                mpMaterial = NULL;
            }

            mContent.texCoordAry.Free();
        }

        Material* Window::FindMaterialByName(const char* findName, bool bRecursive) {
            if (mpMaterial && detail::EqualsMaterialName(mpMaterial->GetName(), findName)) {
                return mpMaterial;
            }

            for (int i = 0; i < mFrameNum; i++) {
                if (detail::EqualsMaterialName(mFrames[i].pMaterial->GetName(), findName)) {
                    return mFrames[i].pMaterial;
                }
            }

            if (bRecursive) {
                for (PaneList::Iterator it = mChildList.GetBeginIter();
                     it != mChildList.GetEndIter(); it++)
                {
                    Material* pMat = it->FindMaterialByName(findName, true);
                    if (pMat) {
                        return pMat;
                    }
                }
            }

            return NULL;
        }

        AnimationLink* Window::FindAnimationLink(AnimTransform* pAnimTrans) {
            if (AnimationLink* ret = Pane::FindAnimationLink(pAnimTrans)) {
                return ret;
            }

            for (int i = 0; i < mFrameNum; i++) {
                if (AnimationLink* ret = mFrames[i].pMaterial->FindAnimationLink(pAnimTrans)) {
                    return ret;
                }
            }

            return NULL;
        }

        void Window::SetAnimationEnable(AnimTransform* pAnimTrans, bool bEnable, bool bRecursive) {
            for (int i = 0; i < mFrameNum; i++) {
                mFrames[i].pMaterial->SetAnimationEnable(pAnimTrans, bEnable);
            }

            Pane::SetAnimationEnable(pAnimTrans, bEnable, bRecursive);
        }

        ut::Color Window::GetVtxColor(u32 idx) const {
            NW4HBM_ASSERT(360, idx < VERTEXCOLOR_MAX);
            return mContent.vtxColors[idx];
        }

        void Window::SetVtxColor(u32 idx, ut::Color value) {
            NW4HBM_ASSERT(371, idx < VERTEXCOLOR_MAX);
            mContent.vtxColors[idx] = value;
        }

        u8 Window::GetVtxColorElement(u32 idx) const {
            return detail::GetVtxColorElement(mContent.vtxColors, idx);
        }

        void Window::SetVtxColorElement(u32 idx, u8 value) {
            detail::SetVtxColorElement(mContent.vtxColors, idx, value);
        }

        void Window::DrawSelf(const DrawInfo& drawInfo) {
            LoadMtx(drawInfo);
            WindowFrameSize frameSize = GetFrameSize(mFrameNum, mFrames);

            math::VEC2 basePt = GetVtxPos();

            DrawContent(basePt, frameSize, mGlbAlpha);

            switch (mFrameNum) {
            case 1:
                DrawFrame(basePt, *mFrames, frameSize, mGlbAlpha);
                break;

            case 4:
                DrawFrame4(basePt, mFrames, frameSize, mGlbAlpha);
                break;

            case 8:
                DrawFrame8(basePt, mFrames, frameSize, mGlbAlpha);
                break;
            }
        }

        void Window::AnimateSelf(u32 option) {
            Pane::AnimateSelf(option);

            if (detail::TestBit<>(mFlag, 0) || !(option & 1)) {
                for (int i = 0; i < mFrameNum; i++) {
                    mFrames[i].pMaterial->Animate();
                }
            }
        }

        void Window::UnbindAnimationSelf(AnimTransform* pAnimTrans) {
            for (int i = 0; i < mFrameNum; i++) {
                mFrames[i].pMaterial->UnbindAnimation(pAnimTrans);
            }

            Pane::UnbindAnimationSelf(pAnimTrans);
        }

        void Window::DrawContent(const math::VEC2& basePt, const WindowFrameSize& frameSize,
                                 u8 alpha) {
            bool bUseVtxCol = mpMaterial->SetupGX(
                detail::IsModulateVertexColor(mContent.vtxColors, alpha), alpha);

            detail::SetVertexFormat(bUseVtxCol, mContent.texCoordAry.GetSize());

            // clang-format off
    detail::DrawQuad(
        math::VEC2(basePt.x + frameSize.l - mContentInflation.l,
                   basePt.y + frameSize.t - mContentInflation.t),
        Size(mSize.width    - frameSize.l + mContentInflation.l
                            - frameSize.r + mContentInflation.r,
             mSize.height   - frameSize.t + mContentInflation.t
                            - frameSize.b + mContentInflation.b),
        mContent.texCoordAry.GetSize(),
        mContent.texCoordAry.GetArray(),
        bUseVtxCol ? mContent.vtxColors : NULL,
        alpha
    );
            // clang-format on
        }

        void Window::DrawFrame(const math::VEC2& basePt, const Frame& frame,
                               const WindowFrameSize& frameSize, u8 alpha) {
            bool bUseVtxCol =
                frame.pMaterial->SetupGX(detail::IsModulateVertexColor(NULL, alpha), alpha);
            detail::SetVertexFormat(bUseVtxCol, GX_TEXMAP1);

            const Size texSize = detail::GetTextureSize(frame.pMaterial, GX_TEXMAP0);
            const ut::Color vtxColors[VERTEXCOLOR_MAX];

            detail::TexCoords texCds[1];

            math::VEC2 polPt;
            Size polSize;

#define DRAW_QUAD_FOR_FRAME_1(corner_, frameIdx_)                                                  \
    {                                                                                              \
        Get##corner_##FrameSize(&polPt, &polSize, basePt, mSize, frameSize);                       \
        Get##corner_##TexCoord(*texCds, polSize, texSize, frameIdx_);                              \
        detail::DrawQuad(polPt, polSize, GX_TEXMAP1, texCds, bUseVtxCol ? vtxColors : NULL,        \
                         alpha);                                                                   \
    }

            DRAW_QUAD_FOR_FRAME_1(LT, TEXTUREFLIP_NONE);
            DRAW_QUAD_FOR_FRAME_1(RT, TEXTUREFLIP_H);
            DRAW_QUAD_FOR_FRAME_1(RB, TEXTUREFLIP_180);
            DRAW_QUAD_FOR_FRAME_1(LB, TEXTUREFLIP_V);

#undef DRAW_QUAD_FOR_FRAME_1
        }

        void Window::DrawFrame4(const math::VEC2& basePt, const Frame* frames,
                                const WindowFrameSize& frameSize, u8 alpha) {
            ut::Color vtxColors[4];
            detail::TexCoords texCds[1];
            math::VEC2 polPt;
            Size polSize;
            bool bModVtxCol = detail::IsModulateVertexColor(NULL, alpha);

#define DRAW_FRAME_4_QUAD_(corner_, frameIdx_)                                                     \
    do {                                                                                           \
        bool bUseVtxCol = frames[frameIdx_].pMaterial->SetupGX(bModVtxCol, alpha);                 \
                                                                                                   \
        Get##corner_##FrameSize(&polPt, &polSize, basePt, mSize, frameSize);                       \
        Get##corner_##TexCoord(*texCds, polSize,                                                   \
                               detail::GetTextureSize(frames[frameIdx_].pMaterial, 0),             \
                               frames[frameIdx_].textureFlip);                                     \
                                                                                                   \
        detail::SetVertexFormat(bUseVtxCol, 1);                                                    \
                                                                                                   \
        detail::DrawQuad(polPt, polSize, 1, texCds, bUseVtxCol ? vtxColors : NULL, alpha);         \
    } while (0)

            DRAW_FRAME_4_QUAD_(LT, 0);
            DRAW_FRAME_4_QUAD_(RT, 1);
            DRAW_FRAME_4_QUAD_(RB, 3);
            DRAW_FRAME_4_QUAD_(LB, 2);

#undef DRAW_FRAME_4_QUAD_
        }

        void Window::DrawFrame8(const math::VEC2& basePt, const Frame* frames,
                                const WindowFrameSize& frameSize, u8 alpha) {
            ut::Color vtxColors[4];
            detail::TexCoords texCds[1];
            Size polSize;
            bool bModVtxCol = detail::IsModulateVertexColor(NULL, alpha);

#define DRAW_FRAME_8_QUAD_(corner_, frameIdx_, polSizeInit_, basePtInit_)                          \
    do {                                                                                           \
        bool bUseVtxCol = frames[frameIdx_].pMaterial->SetupGX(bModVtxCol, alpha);                 \
        polSize = Size polSizeInit_;                                                               \
                                                                                                   \
        Get##corner_##TexCoord(*texCds, polSize,                                                   \
                               detail::GetTextureSize(frames[frameIdx_].pMaterial, 0),             \
                               frames[frameIdx_].textureFlip);                                     \
                                                                                                   \
        detail::SetVertexFormat(bUseVtxCol, 1);                                                    \
                                                                                                   \
        detail::DrawQuad(VEC_CTOR_ basePtInit_, polSize, 1, texCds, bUseVtxCol ? vtxColors : NULL, \
                         alpha);                                                                   \
    } while (0)

#define VEC_CTOR_  // avoid copy construction specifically for this first call
            DRAW_FRAME_8_QUAD_(LT, 0, (frameSize.l, frameSize.t), basePt);
#undef VEC_CTOR_

#define VEC_CTOR_ math::VEC2

            DRAW_FRAME_8_QUAD_(LT, 6, (mSize.width - frameSize.l - frameSize.r, frameSize.t),
                               (basePt.x + frameSize.l, basePt.y));
            DRAW_FRAME_8_QUAD_(RT, 1, (frameSize.r, frameSize.t),
                               (basePt.x + mSize.width - frameSize.r, basePt.y));
            DRAW_FRAME_8_QUAD_(RT, 5, (frameSize.r, mSize.height - frameSize.t - frameSize.b),
                               (basePt.x + mSize.width - frameSize.r, basePt.y + frameSize.t));
            DRAW_FRAME_8_QUAD_(
                RB, 3, (frameSize.r, frameSize.b),
                (basePt.x + mSize.width - frameSize.r, basePt.y + mSize.height - frameSize.b));
            DRAW_FRAME_8_QUAD_(RB, 7, (mSize.width - frameSize.l - frameSize.r, frameSize.b),
                               (basePt.x + frameSize.l, basePt.y + mSize.height - frameSize.b));
            DRAW_FRAME_8_QUAD_(LB, 2, (frameSize.l, frameSize.b),
                               (basePt.x, basePt.y + mSize.height - frameSize.b));
            DRAW_FRAME_8_QUAD_(LB, 4, (frameSize.l, mSize.height - frameSize.t - frameSize.b),
                               (basePt.x, basePt.y + frameSize.t));

#undef VEC_CTOR_
#undef DRAW_FRAME_8_QUAD_
        }

        WindowFrameSize Window::GetFrameSize(u8 frameNum, const Frame* frames) {
            WindowFrameSize ret = {};

            switch (frameNum) {
            case 1: {
                Size texSize = detail::GetTextureSize(frames->pMaterial, 0);
                ret.l = texSize.width;
                ret.t = texSize.height;

                ret.r = texSize.width;
                ret.b = texSize.height;
            } break;

            case 4:
            case 8: {
                Size texSize = detail::GetTextureSize(frames[0].pMaterial, 0);
                ret.l = texSize.width;
                ret.t = texSize.height;

                texSize = detail::GetTextureSize(frames[3].pMaterial, 0);
                ret.r = texSize.width;
                ret.b = texSize.height;
            } break;
            }

            return ret;
        }

        Material* Window::GetFrameMaterial(u32 frameIdx) const {
            NW4HBM_ASSERT(658, frameIdx < WINDOWFRAME_MAX);
            if (frameIdx >= mFrameNum) {
                return NULL;
            }

            return mFrames[frameIdx].pMaterial;
        }

        Material* Window::GetContentMaterial() const {
            return GetMaterial();
        }

    }  // namespace lyt
}  // namespace nw4hbm
