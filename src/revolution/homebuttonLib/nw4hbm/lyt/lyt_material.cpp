#include "common.h"
#include "layout.h"
#include "material.h"

#include "../math/triangular.h"

#include <revolution/tpl.h>

#include <string>

static const GXColorS10 DefaultBlackColor = {0, 0, 0, 0};

static inline void SetDefaultWhiteColor(GXColorS10* p) {
    p->r = 255;
    p->g = 255;
    p->b = 255;
    p->a = 255;
}

static inline bool IsDefaultWhiteColor(GXColorS10* p) {
    return p->r == 255 && p->g == 255 && p->b == 255 && p->a == 255;
}

namespace {
    using namespace nw4hbm;
    using namespace nw4hbm::lyt;

    bool operator==(const GXColorS10& a, const GXColorS10& b) {
        return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
    }

    bool operator!=(const GXColorS10& a, const GXColorS10& b) {
        return !(a == b);
    }

    inline u32 GetTexMtx(u32 texMtxIdx) {
        return texMtxIdx * 3 + GX_TEXMTX0;
    }

    inline u32 GetTexMtxIdx(u32 texMtx) {
        return (texMtx - GX_TEXMTX0) / 3;
    }

    void CalcTextureMtx(math::MTX34* pMtx, const TexSRT& texSRT) {
        math::VEC2 center(0.5f, 0.5f);

        f32 cosR = math::CosDeg(texSRT.rotate);
        f32 sinR = math::SinDeg(texSRT.rotate);

        f32 a0 = cosR * texSRT.scale.x, a1 = -sinR * texSRT.scale.y;
        pMtx->mtx[0][0] = a0;
        pMtx->mtx[0][1] = a1;
        pMtx->mtx[0][2] = 0.0f;
        pMtx->mtx[0][3] = texSRT.translate.x + center.x + a0 * -center.x + a1 * -center.y;

        a0 = sinR * texSRT.scale.x;
        a1 = cosR * texSRT.scale.y;
        pMtx->mtx[1][0] = a0;
        pMtx->mtx[1][1] = a1;
        pMtx->mtx[1][2] = 0.0f;
        pMtx->mtx[1][3] = texSRT.translate.y + center.y + a0 * -center.x + a1 * -center.y;

        pMtx->mtx[2][0] = 0.0f;
        pMtx->mtx[2][1] = 0.0f;
        pMtx->mtx[2][2] = 1.0f;
        pMtx->mtx[2][3] = 0.0f;
    }

    void CalcIndTexMtx(Mtx23 mtx, const TexSRT& texSRT) {
        f32 cosR = math::CosDeg(texSRT.rotate);
        f32 sinR = math::SinDeg(texSRT.rotate);

        mtx[0][0] = cosR * texSRT.scale.x;
        mtx[0][1] = -sinR * texSRT.scale.y;
        mtx[0][2] = texSRT.translate.x;

        mtx[1][0] = sinR * texSRT.scale.x;
        mtx[1][1] = cosR * texSRT.scale.y;
        mtx[1][2] = texSRT.translate.y;
    }

    void SetColorComponentValue(ut::Color* pCol, u32 compIdx, s16 value) {
        const u8 u8Val = ut::Min<s16>(ut::Max<s16>(value, 0), 0xFF);

        switch (compIdx) {
        case 0: {
            pCol->r = u8Val;
            break;
        }
        case 1: {
            pCol->g = u8Val;
            break;
        }
        case 2: {
            pCol->b = u8Val;
            break;
        }
        case 3: {
            pCol->a = u8Val;
            break;
        }
        }
    }

    void SetIndTexMtx(GXIndTexMtxID id, const Mtx23 mtx) {
        f32 m00, m01, m02;
        f32 m10, m11, m12;

        f32 a00, a01, a02;
        f32 a10, a11, a12;

        s8 scaleExp = 0;

        m00 = mtx[0][0];
        m01 = mtx[0][1];
        m02 = mtx[0][2];
        m10 = mtx[1][0];
        m11 = mtx[1][1];
        m12 = mtx[1][2];

        a00 = math::FAbs(m00);
        a01 = math::FAbs(m01);
        a02 = math::FAbs(m02);
        a10 = math::FAbs(m10);
        a11 = math::FAbs(m11);
        a12 = math::FAbs(m12);

        if (a00 >= 1.0f || a01 >= 1.0f || a02 >= 1.0f || a10 >= 1.0f || a11 >= 1.0f || a12 >= 1.0f)
        {
            do {
                if (scaleExp >= 46) {
                    break;
                }

                scaleExp++;

                m00 /= 2.0f;
                m01 /= 2.0f;
                m02 /= 2.0f;
                m10 /= 2.0f;
                m11 /= 2.0f;
                m12 /= 2.0f;

                a00 /= 2.0f;
                a01 /= 2.0f;
                a02 /= 2.0f;
                a10 /= 2.0f;
                a11 /= 2.0f;
                a12 /= 2.0f;

            } while (a00 >= 1.0f || a01 >= 1.0f || a02 >= 1.0f || a10 >= 1.0f || a11 >= 1.0f ||
                     a12 >= 1.0f);
        } else if (a00 < 0.5f && a01 < 0.5f && a02 < 0.5f && a10 < 0.5f && a11 < 0.5f && a12 < 0.5f)
        {
            do {
                scaleExp--;

                m00 *= 2.0f;
                m01 *= 2.0f;
                m02 *= 2.0f;
                m10 *= 2.0f;
                m11 *= 2.0f;
                m12 *= 2.0f;

                a00 *= 2.0f;
                a01 *= 2.0f;
                a02 *= 2.0f;
                a10 *= 2.0f;
                a11 *= 2.0f;
                a12 *= 2.0f;
            } while (a00 < 0.5f && a01 < 0.5f && a02 < 0.5f && a10 < 0.5f && a11 < 0.5f &&
                     a12 < 0.5f && scaleExp > -17);
        }

        Mtx23 outMtx = {{m00, m01, m02}, {m10, m11, m12}};

        GXSetIndTexMtx(id, outMtx, scaleExp);
    }

    void InitTexSRT(TexSRT* texSRTs, u32 num) {
        for (u32 i = 0; i < num; i++) {
            texSRTs[i].translate = math::VEC2(0.0f, 0.0f);
            texSRTs[i].rotate = 0.0f;
            texSRTs[i].scale = math::VEC2(1.0f, 1.0f);
        }
    }

    u32 CalcOffsetTexSRTAry(const detail::BitGXNums& bitGXNums) {
        return (sizeof(res::TexMap) * 8) * bitGXNums.texMap;
    }

    u32 CalcOffsetTexCoordGenAry(const detail::BitGXNums& bitGXNums) {
        return CalcOffsetTexSRTAry(bitGXNums) + sizeof(TexSRT) * bitGXNums.texSRT;
    }

    u32 CalcOffsetChanCtrlAry(const detail::BitGXNums& bitGXNums) {
        return CalcOffsetTexCoordGenAry(bitGXNums) + sizeof(TexCoordGen) * bitGXNums.texCoordGen;
    }

    u32 CalcOffsetMatColAry(const detail::BitGXNums& bitGXNums) {
        return CalcOffsetChanCtrlAry(bitGXNums) + sizeof(ChanCtrl) * bitGXNums.chanCtrl;
    }

    u32 CalcOffsetTevSwapAry(const detail::BitGXNums& bitGXNums) {
        return CalcOffsetMatColAry(bitGXNums) + sizeof(ut::Color) * bitGXNums.matCol;
    }

    u32 CalcOffsetGetAlphaCompare(const detail::BitGXNums& bitGXNums) {
        return CalcOffsetTevSwapAry(bitGXNums) + (sizeof(TevSwapMode) + 3) * bitGXNums.tevSwap;
    }

    u32 CalcOffsetBlendMode(const detail::BitGXNums& bitGXNums) {
        return CalcOffsetGetAlphaCompare(bitGXNums) + sizeof(AlphaCompare) * bitGXNums.alpComp;
    }

    u32 CalcOffsetIndirectStageAry(const detail::BitGXNums& bitGXNums) {
        return CalcOffsetBlendMode(bitGXNums) + sizeof(BlendMode) * bitGXNums.blendMode;
    }

    u32 CalcOffsetIndTexSRTAry(const detail::BitGXNums& bitGXNums) {
        return CalcOffsetIndirectStageAry(bitGXNums) + sizeof(IndirectStage) * bitGXNums.indStage;
    }

    u32 CalcOffsetTevStageAry(const detail::BitGXNums& bitGXNums) {
        return CalcOffsetIndTexSRTAry(bitGXNums) + (sizeof(TevStage) + 4) * bitGXNums.indSRT;
    }

    void CopyGXTexObj(GXTexObj* pDst, const GXTexObj* pSrc) {
        *pDst = *pSrc;
        GXInitTexObjUserData(pDst, NULL);
    }
}  // namespace

namespace nw4hbm {
    namespace lyt {

        Material::Material() {
            Init();
            memset(mName, 0, sizeof(mName));
        }

        Material::Material(const res::Material* pRes, const ResBlockSet& resBlockSet)
            : mAnimList() {
            Init();
            SetName(pRes->name);

            for (int i = 0; i < (int)ARRAY_SIZE(mTevCols); i++) {
                mTevCols[i] = pRes->tevCols[i];
            }

            for (int i = 0; i < (int)ARRAY_SIZE(mTevKCols); i++) {
                mTevKCols[i] = pRes->tevKCols[i];
            }

            u32 resOffs = sizeof(res::Material);

            const res::TexMap* const pResTexMap =
                detail::ConvertOffsToPtr<res::TexMap>(pRes, resOffs);
            resOffs += pRes->resNum.GetTexMapNum() * sizeof(res::TexMap);

            const TexSRT* const resTexSRTs = detail::ConvertOffsToPtr<TexSRT>(pRes, resOffs);
            resOffs += pRes->resNum.GetTexSRTNum() * sizeof(TexSRT);

            const TexCoordGen* const resTexCoordGens =
                detail::ConvertOffsToPtr<TexCoordGen>(pRes, resOffs);
            resOffs += pRes->resNum.GetTexCoordGenNum() * sizeof(TexCoordGen);

            u8 texMapNum = ut::Min<u8>(pRes->resNum.GetTexMapNum(), GX_MAX_TEXMAP);
            u8 texSRTNum = ut::Min<u8>(pRes->resNum.GetTexSRTNum(), MAX_TEX_SRT);
            u8 texCoordGenNum = ut::Min<u8>(pRes->resNum.GetTexCoordGenNum(), GX_MAX_TEXCOORD);

            bool allocChanCtrl = pRes->resNum.GetChanCtrlNum() != 0;
            bool allocMatCol = pRes->resNum.GetMatColNum() != 0;
            bool allocTevSwap = pRes->resNum.HasTevSwapTable();
            bool allocAlpComp = pRes->resNum.HasAlphaCompare();
            bool allocBlendMode = pRes->resNum.HasBlendMode();

            u8 indTexSRTNum = ut::Min<u8>(pRes->resNum.GetIndTexSRTNum(), MAX_IND_SRT);
            u8 indStageNum = ut::Min<u8>(pRes->resNum.GetIndTexStageNum(), GX_MAX_INDTEXSTAGE);
            u8 tevStageNum = ut::Min<u8>(pRes->resNum.GetTevStageNum(), GX_MAX_TEVSTAGE);

            ReserveGXMem(texMapNum, texSRTNum, texCoordGenNum, tevStageNum, allocTevSwap,
                         indStageNum, indTexSRTNum, allocChanCtrl, allocMatCol, allocAlpComp,
                         allocBlendMode);

            if (mpGXMem != NULL) {
                SetTextureNum(texMapNum);
                if (texMapNum) {
                    NW4HBM_ASSERT_CHECK_NULL(469, resBlockSet.pTextureList);

                    const res::Texture* const textures = detail::ConvertOffsToPtr<res::Texture>(
                        resBlockSet.pTextureList, sizeof(*resBlockSet.pTextureList));

                    GXTexObj* const texMaps = GetTexMapAry();

                    for (u8 i = 0; i < mGXMemNum.texMap; i++) {
                        NW4HBM_ASSERT(475, pResTexMap[i].texIdx < resBlockSet.pTextureList->texNum);

                        const char* fileName = detail::ConvertOffsToPtr<char>(
                            textures, textures[pResTexMap[i].texIdx].nameStrOffset);

                        void* pTplRes = resBlockSet.pResAccessor->GetResource(RESOURCE_TYPE_TEXTURE,
                                                                              fileName, NULL);

                        SetTexture(i, static_cast<TPLPalette*>(pTplRes));
                        GXInitTexObjWrapMode(&texMaps[i],
                                             static_cast<GXTexWrapMode>(pResTexMap[i].wrapS),
                                             static_cast<GXTexWrapMode>(pResTexMap[i].wrapT));
                    }
                }

                TexSRT* texSRTs = GetTexSRTAry();
                NW4HBM_ASSERT(486, texSRTNum <= mGXMemNum.texSRT);

                for (int i = 0; i < texSRTNum; i++) {
                    texSRTs[i].translate = resTexSRTs[i].translate;
                    texSRTs[i].rotate = resTexSRTs[i].rotate;
                    texSRTs[i].scale = resTexSRTs[i].scale;
                }

                TexCoordGen* texCoordGens = GetTexCoordGenAry();
                SetTexCoordGenNum(texCoordGenNum);
                for (int i = 0; i < mGXMemNum.texCoordGen; i++) {
                    texCoordGens[i] = resTexCoordGens[i];
                }

                if (allocChanCtrl) {
                    NW4HBM_ASSERT(504, IsChanCtrlCap());
                    const ChanCtrl* pResChanCtrl =
                        detail::ConvertOffsToPtr<ChanCtrl>(pRes, resOffs);
                    *GetChanCtrlAry() = *pResChanCtrl;
                    resOffs += sizeof(ChanCtrl);
                }

                if (allocMatCol) {
                    NW4HBM_ASSERT(512, IsMatColorCap());
                    const ut::Color* pResMatCol =
                        detail::ConvertOffsToPtr<ut::Color>(pRes, resOffs);
                    *GetMatColAry() = *pResMatCol;
                    resOffs += sizeof(ut::Color);
                }

                if (allocTevSwap) {
                    NW4HBM_ASSERT(520, IsTevSwapCap());
                    const TevSwapMode* pResTevSwap =
                        detail::ConvertOffsToPtr<TevSwapMode>(pRes, resOffs);
                    TevSwapMode* tevSwaps = GetTevSwapAry();
                    for (int i = 0; i < GX_MAX_TEVSWAP; i++) {
                        tevSwaps[i] = pResTevSwap[i];
                    }
                    resOffs += GX_MAX_TEVSWAP * sizeof(TevSwapMode);
                }

                if (indTexSRTNum) {
                    NW4HBM_ASSERT(532, indTexSRTNum <= mGXMemNum.indSRT);
                    TexSRT* indTexSRTs = GetIndTexSRTAry();
                    const TexSRT* pResIndMtx = detail::ConvertOffsToPtr<TexSRT>(pRes, resOffs);
                    for (int i = 0; i < indTexSRTNum; i++) {
                        indTexSRTs[i] = pResIndMtx[i];
                    }
                }
                resOffs += sizeof(TexSRT) * pRes->resNum.GetIndTexSRTNum();

                if (indStageNum) {
                    SetIndStageNum(indStageNum);

                    IndirectStage* indirectStages = GetIndirectStageAry();
                    const IndirectStage* pResIndStg =
                        detail::ConvertOffsToPtr<IndirectStage>(pRes, resOffs);
                    for (int i = 0; i < indStageNum; i++) {
                        indirectStages[i] = pResIndStg[i];
                    }
                }
                resOffs += sizeof(IndirectStage) * pRes->resNum.GetIndTexStageNum();

                if (tevStageNum) {
                    SetTevStageNum(tevStageNum);

                    TevStage* tevStages = GetTevStageAry();
                    const TevStage* pResTevStg = detail::ConvertOffsToPtr<TevStage>(pRes, resOffs);
                    for (int i = 0; i < tevStageNum; i++) {
                        tevStages[i] = pResTevStg[i];
                    }
                }
                resOffs += sizeof(TevStage) * pRes->resNum.GetTevStageNum();

                if (allocAlpComp) {
                    NW4HBM_ASSERT(568, IsAlphaCompareCap());
                    const AlphaCompare* pResAlphaCompare =
                        detail::ConvertOffsToPtr<AlphaCompare>(pRes, resOffs);
                    *GetAlphaComparePtr() = *pResAlphaCompare;
                    resOffs += sizeof(AlphaCompare);
                }

                if (allocBlendMode) {
                    NW4HBM_ASSERT(576, IsBlendModeCap());
                    const BlendMode* pResBlendMode =
                        detail::ConvertOffsToPtr<BlendMode>(pRes, resOffs);
                    *GetBlendModePtr() = *pResBlendMode;
                    resOffs += sizeof(BlendMode);
                }
            }
        }

        void Material::Init() {
            mTevCols[TEVCOLOR_REG0] = DefaultBlackColor;
            SetDefaultWhiteColor(&mTevCols[TEVCOLOR_REG1]);
            SetDefaultWhiteColor(&mTevCols[TEVCOLOR_REG2]);

            InitBitGXNums(&mGXMemCap);
            InitBitGXNums(&mGXMemNum);

            mbUserAllocated = false;
            mpGXMem = NULL;
        }

        Material::~Material() {
            UnbindAllAnimation();

            if (mpGXMem != NULL) {
                Layout::FreeMemory(mpGXMem);
                mpGXMem = NULL;
            }
        }

        void Material::InitBitGXNums(detail::BitGXNums* ptr) {
            ptr->texMap = 0;
            ptr->texSRT = 0;
            ptr->texCoordGen = 0;
            ptr->indSRT = 0;
            ptr->indStage = 0;
            ptr->tevSwap = false;
            ptr->tevStage = 0;
            ptr->chanCtrl = false;
            ptr->matCol = false;
            ptr->alpComp = false;
            ptr->blendMode = 0;
        }

        void Material::ReserveGXMem(u8 texMapNum, u8 texSRTNum, u8 texCoordGenNum, u8 tevStageNum,
                                    bool allocTevSwap, u8 indStageNum, u8 indSRTNum,
                                    bool allocChanCtrl, bool allocMatCol, bool allocAlpComp,
                                    bool allocBlendMode) {
            NW4HBM_ASSERT(661, texMapNum <= GX_MAX_TEXMAP);
            NW4HBM_ASSERT(662, texSRTNum <= TexMtxMax);
            NW4HBM_ASSERT(663, texCoordGenNum <= GX_MAX_TEXCOORD);
            NW4HBM_ASSERT(664, tevStageNum <= GX_MAX_TEVSTAGE);
            NW4HBM_ASSERT(665, indStageNum <= GX_MAX_INDTEXSTAGE);
            NW4HBM_ASSERT(666, indSRTNum <= IndTexMtxMax);

            int tevSwapNum = allocTevSwap ? 1 : 0;
            int chanCtrlNum = allocChanCtrl ? 1 : 0;
            int matColNum = allocMatCol ? 1 : 0;
            int alpCompNum = allocAlpComp ? 1 : 0;
            int blendModeNum = allocBlendMode ? 1 : 0;

            if (mGXMemCap.texMap >= texMapNum && mGXMemCap.texSRT >= texSRTNum &&
                mGXMemCap.texCoordGen >= texCoordGenNum && mGXMemCap.tevStage >= tevStageNum &&
                mGXMemCap.tevSwap >= tevSwapNum && mGXMemCap.indStage >= indStageNum &&
                mGXMemCap.indSRT >= indSRTNum && mGXMemCap.chanCtrl >= chanCtrlNum &&
                mGXMemCap.matCol >= matColNum && mGXMemCap.alpComp >= alpCompNum &&
                mGXMemCap.blendMode >= blendModeNum)
            {
                return;
            }

            if (mpGXMem != NULL) {
                Layout::FreeMemory(mpGXMem);
                mpGXMem = NULL;
                InitBitGXNums(&mGXMemCap);
                InitBitGXNums(&mGXMemNum);
            }

            mpGXMem = Layout::AllocMemory(
                4 * (matColNum + chanCtrlNum + texCoordGenNum + tevSwapNum + alpCompNum +
                     blendModeNum + indStageNum) +
                ((int)sizeof(TexSRT) * texSRTNum) + ((int)sizeof(GXTexObj) * texMapNum) +
                ((int)sizeof(TexSRT) * indSRTNum) + ((int)sizeof(TevStage) * tevStageNum));

            if (mpGXMem == NULL) {
                return;
            }

            mGXMemCap.texMap = texMapNum;
            mGXMemCap.texSRT = texSRTNum;
            mGXMemCap.texCoordGen = texCoordGenNum;
            mGXMemCap.indSRT = indSRTNum;
            mGXMemCap.indStage = indStageNum;
            mGXMemCap.tevSwap = tevSwapNum;
            mGXMemCap.tevStage = tevStageNum;
            mGXMemCap.chanCtrl = chanCtrlNum;
            mGXMemCap.matCol = matColNum;
            mGXMemCap.alpComp = alpCompNum;
            mGXMemCap.blendMode = blendModeNum;

            mGXMemNum.texSRT = mGXMemCap.texSRT;
            InitTexSRT(GetTexSRTAry(), mGXMemNum.texSRT);

            mGXMemNum.indSRT = mGXMemCap.indSRT;
            InitTexSRT(GetIndTexSRTAry(), mGXMemNum.indSRT);

            mGXMemNum.chanCtrl = mGXMemCap.chanCtrl;
            if (mGXMemNum.chanCtrl) {
                *GetChanCtrlAry() = ChanCtrl();
            }

            mGXMemNum.matCol = mGXMemCap.matCol;
            if (mGXMemNum.matCol) {
                *GetMatColAry() = ut::Color::WHITE;
            }

            mGXMemNum.tevSwap = mGXMemCap.tevSwap;
            if (mGXMemNum.tevSwap) {
                TevSwapMode* tevSwaps = GetTevSwapAry();
                tevSwaps[GX_TEV_SWAP0].Set(GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
                tevSwaps[GX_TEV_SWAP1].Set(GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
                tevSwaps[GX_TEV_SWAP2].Set(GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
                tevSwaps[GX_TEV_SWAP3].Set(GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
            }

            mGXMemNum.alpComp = mGXMemCap.alpComp;
            if (mGXMemNum.alpComp) {
                *GetAlphaComparePtr() = AlphaCompare();
            }

            mGXMemNum.blendMode = mGXMemCap.blendMode;
            if (mGXMemNum.blendMode) {
                *GetBlendModePtr() = BlendMode();
            }
        }

        const GXTexObj* Material::GetTexMapAry() const {
            return detail::ConvertOffsToPtr<GXTexObj>(mpGXMem, 0);
        }

        GXTexObj* Material::GetTexMapAry() {
            return detail::ConvertOffsToPtr<GXTexObj>(mpGXMem, 0);
        }

        const TexSRT* Material::GetTexSRTAry() const {
            return detail::ConvertOffsToPtr<TexSRT>(mpGXMem, CalcOffsetTexSRTAry(mGXMemCap));
        }

        TexSRT* Material::GetTexSRTAry() {
            return detail::ConvertOffsToPtr<TexSRT>(mpGXMem, CalcOffsetTexSRTAry(mGXMemCap));
        }

        const TexCoordGen* Material::GetTexCoordGenAry() const {
            return detail::ConvertOffsToPtr<TexCoordGen>(mpGXMem,
                                                         CalcOffsetTexCoordGenAry(mGXMemCap));
        }

        TexCoordGen* Material::GetTexCoordGenAry() {
            return detail::ConvertOffsToPtr<TexCoordGen>(mpGXMem,
                                                         CalcOffsetTexCoordGenAry(mGXMemCap));
        }

        const ChanCtrl* Material::GetChanCtrlAry() const {
            return detail::ConvertOffsToPtr<ChanCtrl>(mpGXMem, CalcOffsetChanCtrlAry(mGXMemCap));
        }

        ChanCtrl* Material::GetChanCtrlAry() {
            return detail::ConvertOffsToPtr<ChanCtrl>(mpGXMem, CalcOffsetChanCtrlAry(mGXMemCap));
        }

        const ut::Color* Material::GetMatColAry() const {
            return detail::ConvertOffsToPtr<ut::Color>(mpGXMem, CalcOffsetMatColAry(mGXMemCap));
        }

        ut::Color* Material::GetMatColAry() {
            return detail::ConvertOffsToPtr<ut::Color>(mpGXMem, CalcOffsetMatColAry(mGXMemCap));
        }

        const TevSwapMode* Material::GetTevSwapAry() const {
            return detail::ConvertOffsToPtr<TevSwapMode>(mpGXMem, CalcOffsetTevSwapAry(mGXMemCap));
        }

        TevSwapMode* Material::GetTevSwapAry() {
            return detail::ConvertOffsToPtr<TevSwapMode>(mpGXMem, CalcOffsetTevSwapAry(mGXMemCap));
        }

        const AlphaCompare* Material::GetAlphaComparePtr() const {
            return detail::ConvertOffsToPtr<AlphaCompare>(mpGXMem,
                                                          CalcOffsetGetAlphaCompare(mGXMemCap));
        }

        AlphaCompare* Material::GetAlphaComparePtr() {
            return detail::ConvertOffsToPtr<AlphaCompare>(mpGXMem,
                                                          CalcOffsetGetAlphaCompare(mGXMemCap));
        }

        const BlendMode* Material::GetBlendModePtr() const {
            return detail::ConvertOffsToPtr<BlendMode>(mpGXMem, CalcOffsetBlendMode(mGXMemCap));
        }

        BlendMode* Material::GetBlendModePtr() {
            return detail::ConvertOffsToPtr<BlendMode>(mpGXMem, CalcOffsetBlendMode(mGXMemCap));
        }

        const IndirectStage* Material::GetIndirectStageAry() const {
            return detail::ConvertOffsToPtr<IndirectStage>(mpGXMem,
                                                           CalcOffsetIndirectStageAry(mGXMemCap));
        }

        IndirectStage* Material::GetIndirectStageAry() {
            return detail::ConvertOffsToPtr<IndirectStage>(mpGXMem,
                                                           CalcOffsetIndirectStageAry(mGXMemCap));
        }

        const TexSRT* Material::GetIndTexSRTAry() const {
            return detail::ConvertOffsToPtr<TexSRT>(mpGXMem, CalcOffsetIndTexSRTAry(mGXMemCap));
        }

        TexSRT* Material::GetIndTexSRTAry() {
            return detail::ConvertOffsToPtr<TexSRT>(mpGXMem, CalcOffsetIndTexSRTAry(mGXMemCap));
        }

        const TevStage* Material::GetTevStageAry() const {
            return detail::ConvertOffsToPtr<TevStage>(mpGXMem, CalcOffsetTevStageAry(mGXMemCap));
        }

        TevStage* Material::GetTevStageAry() {
            return detail::ConvertOffsToPtr<TevStage>(mpGXMem, CalcOffsetTevStageAry(mGXMemCap));
        }

        void Material::SetName(const char* name) {
            std::strncpy(mName, name, ARRAY_SIZE(mName));
        }

        void Material::SetTextureNum(u8 num) {
            NW4HBM_ASSERT(907, num <= mGXMemCap.texMap);

            if (num) {
                NW4HBM_ASSERT_CHECK_NULL(910, mpGXMem);
                GXTexObj* texMaps = GetTexMapAry();

                for (u32 i = mGXMemNum.texMap; i < num; i++) {
                    memset(&texMaps[i], 0, sizeof(*texMaps));
                }

                mGXMemNum.texMap = num;
            }
        }

        void Material::SetTexCoordGenNum(u8 num) {
            NW4HBM_ASSERT(924, num <= mGXMemCap.texCoordGen);

            if (num) {
                NW4HBM_ASSERT_CHECK_NULL(927, mpGXMem);
                TexCoordGen* const texCoordGens = GetTexCoordGenAry();

                for (u32 i = mGXMemNum.texCoordGen; i < num; i++) {
                    texCoordGens[i] = TexCoordGen();
                }

                mGXMemNum.texCoordGen = num;
            }
        }

        void Material::SetTevStageNum(u8 num) {
            NW4HBM_ASSERT(941, num <= mGXMemCap.tevStage);

            if (num) {
                NW4HBM_ASSERT_CHECK_NULL(944, mpGXMem);
                TevStage* tevStages = GetTevStageAry();

                for (u32 i = mGXMemNum.tevStage; i < num; i++) {
                    tevStages[i] = TevStage();
                }

                mGXMemNum.tevStage = num;
            }
        }

        void Material::SetIndStageNum(u8 num) {
            NW4HBM_ASSERT(958, num <= mGXMemCap.indStage);

            if (num) {
                NW4HBM_ASSERT_CHECK_NULL(961, mpGXMem);
                IndirectStage* const indStages = GetIndirectStageAry();

                for (u32 i = mGXMemNum.indStage; i < num; i++) {
                    indStages[i] = IndirectStage();
                }

                mGXMemNum.indStage = num;
            }
        }

        void Material::GetTexture(GXTexObj* pTexObj, u8 texMapIdx) const {
            NW4HBM_ASSERT_CHECK_NULL(988, pTexObj);
            NW4HBM_ASSERT(989, texMapIdx < mGXMemNum.texMap);
            CopyGXTexObj(pTexObj, &GetTexMapAry()[texMapIdx]);
        }

        void Material::SetTexture(u8 texMapIdx, TPLPalette* pTplRes) {
            NW4HBM_ASSERT(1010, texMapIdx < mGXMemNum.texMap);
            GXTexObj* pDstTexObj = &GetTexMapAry()[texMapIdx];
            detail::InitGXTexObjFromTPL(pDstTexObj, pTplRes, 0);
        }

        void Material::SetTextureNoWrap(u8 texMapIdx, TPLPalette* pTplRes) {
            NW4HBM_ASSERT(1033, texMapIdx < mGXMemNum.texMap);
            GXTexObj* pDstTexObj = &GetTexMapAry()[texMapIdx];
            GXTexWrapMode wrapS = GXGetTexObjWrapS(pDstTexObj);
            GXTexWrapMode wrapT = GXGetTexObjWrapT(pDstTexObj);
            detail::InitGXTexObjFromTPL(pDstTexObj, pTplRes, 0);
            GXInitTexObjWrapMode(pDstTexObj, wrapS, wrapT);
        }

        void Material::SetTexture(u8 texMapIdx, const GXTexObj& texObj) {
            NW4HBM_ASSERT(1061, texMapIdx < mGXMemNum.texMap);
            GXTexObj* pDstTexObj = &GetTexMapAry()[texMapIdx];
            CopyGXTexObj(pDstTexObj, &texObj);
        }

        void Material::SetColorElement(u32 colorType, s16 value) {
            switch (colorType) {
            case ANIMTARGET_MATCOLOR_MATR:
            case ANIMTARGET_MATCOLOR_MATG:
            case ANIMTARGET_MATCOLOR_MATB:
            case ANIMTARGET_MATCOLOR_MATA: {
                if (mGXMemNum.matCol >= 1) {
                    ut::Color* matCols = GetMatColAry();
                    SetColorComponentValue(matCols, colorType & 3, value);
                }

                break;
            }
            case ANIMTARGET_MATCOLOR_TEV0R:
            case ANIMTARGET_MATCOLOR_TEV0G:
            case ANIMTARGET_MATCOLOR_TEV0B:
            case ANIMTARGET_MATCOLOR_TEV0A:
            case ANIMTARGET_MATCOLOR_TEV1R:
            case ANIMTARGET_MATCOLOR_TEV1G:
            case ANIMTARGET_MATCOLOR_TEV1B:
            case ANIMTARGET_MATCOLOR_TEV1A:
            case ANIMTARGET_MATCOLOR_TEV2R:
            case ANIMTARGET_MATCOLOR_TEV2G:
            case ANIMTARGET_MATCOLOR_TEV2B:
            case ANIMTARGET_MATCOLOR_TEV2A: {
                u32 regIdx = (colorType - 4) / 4;
                switch ((colorType - 4) % 4) {
                case 0: {
                    mTevCols[regIdx].r = value;
                    break;
                }
                case 1: {
                    mTevCols[regIdx].g = value;
                    break;
                }
                case 2: {
                    mTevCols[regIdx].b = value;
                    break;
                }
                case 3: {
                    mTevCols[regIdx].a = value;
                    break;
                }
                }
                break;
            }
            case ANIMTARGET_MATCOLOR_TEVK0R:
            case ANIMTARGET_MATCOLOR_TEVK0G:
            case ANIMTARGET_MATCOLOR_TEVK0B:
            case ANIMTARGET_MATCOLOR_TEVK0A:
            case ANIMTARGET_MATCOLOR_TEVK1R:
            case ANIMTARGET_MATCOLOR_TEVK1G:
            case ANIMTARGET_MATCOLOR_TEVK1B:
            case ANIMTARGET_MATCOLOR_TEVK1A:
            case ANIMTARGET_MATCOLOR_TEVK2R:
            case ANIMTARGET_MATCOLOR_TEVK2G:
            case ANIMTARGET_MATCOLOR_TEVK2B:
            case ANIMTARGET_MATCOLOR_TEVK2A:
            case ANIMTARGET_MATCOLOR_TEVK3R:
            case ANIMTARGET_MATCOLOR_TEVK3G:
            case ANIMTARGET_MATCOLOR_TEVK3B:
            case ANIMTARGET_MATCOLOR_TEVK3A: {
                u32 regIdx = (colorType - 16) / 4;
                SetColorComponentValue(&mTevKCols[regIdx], (colorType - 16) & 3, value);
                break;
            }
            }
        }

        bool Material::SetupGX(bool bModVtxCol, u8 alpha) {
            static GXTevKColorSel kColSels[GX_MAX_TEXMAP] = {
                GX_TEV_KCSEL_K3_A, GX_TEV_KCSEL_K3_B, GX_TEV_KCSEL_K3_G, GX_TEV_KCSEL_K3_R,
                GX_TEV_KCSEL_K2_A, GX_TEV_KCSEL_K2_B, GX_TEV_KCSEL_K2_G, GX_TEV_KCSEL_K2_R,
            };

            static GXTevKAlphaSel kAlpSels[GX_MAX_TEXMAP] = {
                GX_TEV_KASEL_K3_A, GX_TEV_KASEL_K3_B, GX_TEV_KASEL_K3_G, GX_TEV_KASEL_K3_R,
                GX_TEV_KASEL_K2_A, GX_TEV_KASEL_K2_B, GX_TEV_KASEL_K2_G, GX_TEV_KASEL_K2_R,
            };

            bool bUseVtxCol = true;
            bool bUseMatCol = false;

            GXSetNumChans(1);

            if (IsChanCtrlCap()) {
                ChanCtrl* const chanCtrls = GetChanCtrlAry();

                GXSetChanCtrl(GX_COLOR0, false, GX_SRC_REG, chanCtrls->GetColorSrc(), GX_LIGHT_NULL,
                              GX_DF_NONE, GX_AF_NONE);
                GXSetChanCtrl(GX_ALPHA0, false, GX_SRC_REG, chanCtrls->GetAlphaSrc(), GX_LIGHT_NULL,
                              GX_DF_NONE, GX_AF_NONE);

                bUseVtxCol = chanCtrls->GetColorSrc() == GX_SRC_VTX ||
                             chanCtrls->GetAlphaSrc() == GX_SRC_VTX;
                bUseMatCol = chanCtrls->GetColorSrc() == GX_SRC_REG ||
                             chanCtrls->GetAlphaSrc() == GX_SRC_REG;
            } else {
                GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                              GX_AF_NONE);
            }

            bool bUseRasStage = false;

            if (bUseVtxCol) {
                bUseRasStage = bUseRasStage || bModVtxCol;
            }

            if (bUseMatCol) {
                ut::Color matCol(ut::Color::WHITE);

                if (IsMatColorCap()) {
                    matCol = *GetMatColAry();
                }

                matCol = detail::MultipleAlpha(matCol, alpha);
                GXSetChanMatColor(GX_COLOR0A0, matCol);

                bUseRasStage = bUseRasStage || matCol != ut::Color::WHITE;
            }

            bool bSetTexMtx = false;
            bool bUseTexMtx[MAX_TEX_SRT];

            for (int i = 0; i < (int)ARRAY_SIZE(bUseTexMtx); i++) {
                bUseTexMtx[i] = false;
            }

            GXSetNumTexGens(mGXMemNum.texCoordGen);

            if (mGXMemNum.texCoordGen) {
                TexCoordGen* texCoordGens = GetTexCoordGenAry();

                for (int i = 0; i < mGXMemNum.texCoordGen; i++) {
                    NW4HBM_ASSERT(1288, texCoordGens[i].GetTexGenType() != GX_TG_MTX3x4);
                    u32 texMtx = texCoordGens[i].GetTexMtx();

                    if (texCoordGens[i].GetTexGenType() == GX_TG_MTX2x4) {
                        NW4HBM_ASSERT(1294, texMtx == GX_IDENTITY ||
                                                GetTexMtxIdx(texMtx) < mGXMemNum.texSRT);

                        if (texMtx != GX_IDENTITY) {
                            bUseTexMtx[GetTexMtxIdx(texMtx)] = true;
                            bSetTexMtx = true;
                        }
                    }

                    GXSetTexCoordGen(static_cast<GXTexCoordID>(i), texCoordGens[i].GetTexGenType(),
                                     texCoordGens[i].GetTexGenSrc(), texMtx);
                }
            }

            if (bSetTexMtx) {
                TexSRT* const texSRTs = GetTexSRTAry();
                for (u8 i = 0; i < mGXMemNum.texSRT; i++) {
                    if (bUseTexMtx[i]) {
                        math::MTX34 texMtx;
                        CalcTextureMtx(&texMtx, texSRTs[i]);
                        GXLoadTexMtxImm(texMtx, GetTexMtx(i), GX_MTX2x4);
                    }
                }
            }

            if (mGXMemNum.texMap) {
                u32 tlutName = GX_TLUT0;
                GXTexObj* texMaps = GetTexMapAry();

                for (int i = 0; i < mGXMemNum.texMap; i++) {
                    GXTexObj texMap = texMaps[i];
                    int texFmt = GXGetTexObjFmt(&texMap);

                    if (texFmt == GX_TF_C4 || texFmt == GX_TF_C8) {
                        TPLClutHeader* pClutHeader =
                            static_cast<TPLClutHeader*>(GXGetTexObjUserData(&texMap));

                        if (pClutHeader) {
                            GXTlutObj tlutObj;

                            GXInitTlutObj(&tlutObj, pClutHeader->data, pClutHeader->format,
                                          pClutHeader->numEntries);
                            GXLoadTlut(&tlutObj, tlutName);
                            GXInitTexObjTlut(&texMap, tlutName);

                            tlutName++;
                        }
                    }

                    GXLoadTexObj(&texMap, (GXTexMapID)i);
                }
            }

            GXSetTevColorS10(GX_TEVREG0, mTevCols[TEVCOLOR_REG0]);
            GXSetTevColorS10(GX_TEVREG1, mTevCols[TEVCOLOR_REG1]);
            GXSetTevColorS10(GX_TEVREG2, mTevCols[TEVCOLOR_REG2]);

            GXSetTevKColor(GX_KCOLOR0, mTevKCols[GX_KCOLOR0]);
            GXSetTevKColor(GX_KCOLOR1, mTevKCols[GX_KCOLOR1]);
            GXSetTevKColor(GX_KCOLOR2, mTevKCols[GX_KCOLOR2]);
            GXSetTevKColor(GX_KCOLOR3, mTevKCols[GX_KCOLOR3]);

            if (IsTevSwapCap()) {
                TevSwapMode* tevSwaps = GetTevSwapAry();
                for (int i = 0; i < GX_MAX_TEVSWAP; i++) {
                    GXSetTevSwapModeTable(static_cast<GXTevSwapSel>(i), tevSwaps[i].GetR(),
                                          tevSwaps[i].GetG(), tevSwaps[i].GetB(),
                                          tevSwaps[i].GetA());
                }
            } else {
                GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE,
                                      GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN,
                                      GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE,
                                      GX_CH_ALPHA);
            }

            bool bSetIndTexMtx = false;
            bool bUseIndTexMtx[MAX_IND_SRT];

            for (int i = 0; i < (int)ARRAY_SIZE(bUseIndTexMtx); i++) {
                bUseIndTexMtx[i] = false;
            }

            if (mGXMemNum.tevStage) {
                GXSetNumTevStages(mGXMemNum.tevStage);

                TevStage* tevStages = GetTevStageAry();
                for (int i = 0; i < mGXMemNum.tevStage; i++) {
                    GXTevStageID tevStage = static_cast<GXTevStageID>(i);

                    GXSetTevOrder(tevStage, tevStages[i].GetTexCoordGen(), tevStages[i].GetTexMap(),
                                  tevStages[i].GetColorChan());

                    GXSetTevSwapMode(tevStage, tevStages[i].GetRasSwapSel(),
                                     tevStages[i].GetTexSwapSel());

                    GXSetTevColorIn(tevStage, tevStages[i].GetColorInA(),
                                    tevStages[i].GetColorInB(), tevStages[i].GetColorInC(),
                                    tevStages[i].GetColorInD());

                    GXSetTevColorOp(tevStage, tevStages[i].GetColorOp(),
                                    tevStages[i].GetColorBias(), tevStages[i].GetColorScale(),
                                    tevStages[i].IsColorClamp(), tevStages[i].GetColorOutReg());

                    GXSetTevKColorSel(tevStage, tevStages[i].GetKColorSel());

                    GXSetTevAlphaIn(tevStage, tevStages[i].GetAlphaInA(),
                                    tevStages[i].GetAlphaInB(), tevStages[i].GetAlphaInC(),
                                    tevStages[i].GetAlphaInD());

                    GXSetTevAlphaOp(tevStage, tevStages[i].GetAlphaOp(),
                                    tevStages[i].GetAlphaBias(), tevStages[i].GetAlphaScale(),
                                    tevStages[i].IsAlphaClamp(), tevStages[i].GetAlphaOutReg());

                    GXSetTevKAlphaSel(tevStage, tevStages[i].GetKAlphaSel());

                    GXIndTexMtxID indMtxSel = tevStages[i].GetIndMtxSel();
                    GXSetTevIndirect(tevStage, tevStages[i].GetIndStage(),
                                     tevStages[i].GetIndFormat(), tevStages[i].GetIndBiasSel(),
                                     indMtxSel, tevStages[i].GetIndWrapS(),
                                     tevStages[i].GetIndWrapT(), tevStages[i].IsIndAddPrev(),
                                     tevStages[i].IsIndUtcLod(), tevStages[i].GetIndAlphaSel());

                    if (GX_ITM_0 <= indMtxSel && indMtxSel <= GX_ITM_2) {
                        bUseIndTexMtx[indMtxSel - 1] = true;
                        bSetIndTexMtx = true;
                    }
                }

                bUseRasStage = true;
            } else {
                u8 tevStageID = GX_TEVSTAGE0;

                if (mGXMemNum.texMap == GX_TEXMAP0) {
                    GXTevStageID tevStage = static_cast<GXTevStageID>(tevStageID);

                    GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                    GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_C1, GX_CC_RASC, GX_CC_ZERO);
                    GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_A1, GX_CA_RASA, GX_CA_ZERO);

                    tevStageID++;
                    bUseRasStage = true;
                } else {
                    if (mGXMemNum.texMap == GX_TEXMAP1) {
                        GXTevStageID tevStage = static_cast<GXTevStageID>(tevStageID);

                        GXSetTevOrder(tevStage, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                        GXSetTevColorIn(tevStage, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
                        GXSetTevAlphaIn(tevStage, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);

                        tevStageID++;
                    } else {
                        if (mGXMemNum.texMap == GX_TEXMAP2) {
                            GXTevStageID tevStage;

                            tevStage = static_cast<GXTevStageID>(tevStageID);
                            GXSetTevOrder(tevStage, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                            GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
                                            GX_CC_TEXC);
                            GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
                                            GX_CA_TEXA);

                            tevStageID++;

                            tevStage = static_cast<GXTevStageID>(tevStageID);
                            GXSetTevOrder(tevStage, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                            GXSetTevColorIn(tevStage, GX_CC_TEXC, GX_CC_CPREV, GX_CC_KONST,
                                            GX_CC_ZERO);
                            GXSetTevAlphaIn(tevStage, GX_CA_TEXA, GX_CA_APREV, GX_CA_KONST,
                                            GX_CA_ZERO);

                            GXSetTevKColorSel(tevStage, kColSels[GX_TEXMAP0]);
                            GXSetTevKAlphaSel(tevStage, kAlpSels[GX_TEXMAP0]);

                            tevStageID++;
                        } else {
                            for (int i = 0; i < mGXMemNum.texMap; i++) {
                                GXTevStageID tevStage = static_cast<GXTevStageID>(tevStageID);

                                GXSetTevOrder(tevStage, static_cast<GXTexCoordID>(i),
                                              static_cast<GXTexMapID>(i), GX_COLOR_NULL);

                                GXTevColorArg colDIn = i == GX_TEXMAP0 ? GX_CC_ZERO : GX_CC_CPREV;
                                GXTevAlphaArg alpDIn = i == GX_TEXMAP0 ? GX_CA_ZERO : GX_CA_APREV;

                                GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST,
                                                colDIn);
                                GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST,
                                                alpDIn);

                                GXSetTevKColorSel(tevStage, kColSels[i]);
                                GXSetTevKAlphaSel(tevStage, kAlpSels[i]);

                                tevStageID++;
                            }
                        }

                        if (mTevCols[TEVCOLOR_REG0] != DefaultBlackColor ||
                            !IsDefaultWhiteColor(&mTevCols[TEVCOLOR_REG1]))
                        {
                            GXTevStageID tevStage = static_cast<GXTevStageID>(tevStageID);

                            GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
                                          GX_COLOR_NULL);
                            GXSetTevColorIn(tevStage, GX_CC_C0, GX_CC_C1, GX_CC_CPREV, GX_CC_ZERO);
                            GXSetTevAlphaIn(tevStage, GX_CA_A0, GX_CA_A1, GX_CA_APREV, GX_CA_ZERO);

                            tevStageID++;
                        }
                    }

                    if (bUseRasStage) {
                        GXTevStageID tevStage = static_cast<GXTevStageID>(tevStageID);

                        GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                        GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
                        GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);

                        tevStageID++;
                    }
                }

                const u8 tevStageNum = tevStageID;
                for (u8 id = 0; id < tevStageNum; id++) {
                    GXTevStageID tevStage = static_cast<GXTevStageID>(id);

                    GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
                    GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);

                    GXSetTevDirect(tevStage);
                    GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
                }

                GXSetNumTevStages(tevStageNum);
            }

            if (bSetIndTexMtx) {
                TexSRT* indTexSRTs = GetIndTexSRTAry();

                for (int i = 0; i < mGXMemNum.indSRT; i++) {
                    if (bUseIndTexMtx[i]) {
                        Mtx23 mtx;
                        CalcIndTexMtx(mtx, indTexSRTs[i]);
                        SetIndTexMtx(static_cast<GXIndTexMtxID>(i + 1), mtx);
                    }
                }
            }

            GXSetNumIndStages(mGXMemNum.indStage);

            if (mGXMemNum.indStage) {
                IndirectStage* indirectStages = GetIndirectStageAry();

                for (int i = 0; i < mGXMemNum.indStage; i++) {
                    GXIndTexStageID indStage = static_cast<GXIndTexStageID>(i);

                    GXSetIndTexOrder(indStage, indirectStages[i].GetTexCoordGen(),
                                     indirectStages[i].GetTexMap());
                    GXSetIndTexCoordScale(indStage, indirectStages[i].GetScaleS(),
                                          indirectStages[i].GetScaleT());
                }
            }

            if (IsAlphaCompareCap()) {
                AlphaCompare* pAlpComp = GetAlphaComparePtr();

                GXSetAlphaCompare(pAlpComp->GetComp0(), pAlpComp->GetRef0(), pAlpComp->GetOp(),
                                  pAlpComp->GetComp1(), pAlpComp->GetRef1());
            } else {
                GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
            }

            if (IsBlendModeCap()) {
                BlendMode* pBlendMode = GetBlendModePtr();
                GXSetBlendMode(pBlendMode->GetType(), pBlendMode->GetSrcFactor(),
                               pBlendMode->GetDstFactor(), pBlendMode->GetOp());
            } else {
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
            }

            return bUseRasStage && bUseVtxCol;
        }

        void Material::BindAnimation(AnimTransform* animTrans) {
            animTrans->Bind(this);
        }

        void Material::UnbindAnimation(AnimTransform* animTrans) {
            for (AnimationLinkList::Iterator it = mAnimList.GetBeginIter();
                 it != mAnimList.GetEndIter();)
            {
                AnimationLinkList::Iterator currIt = it++;

                if (animTrans == NULL || currIt->GetAnimTransform() == animTrans) {
                    mAnimList.Erase(currIt);
                    currIt->Reset();
                }
            }
        }

        void Material::UnbindAllAnimation() {
            UnbindAnimation(NULL);
        }

        void Material::Animate() {
            for (AnimationLinkList::Iterator it = mAnimList.GetBeginIter();
                 it != mAnimList.GetEndIter(); it++)
            {
                if (it->IsEnable()) {
                    AnimTransform* animTrans = it->GetAnimTransform();
                    animTrans->Animate(it->GetIndex(), this);
                }
            }
        }

        void Material::AddAnimationLink(AnimationLink* animationLink) {
            mAnimList.PushBack(animationLink);
        }

        AnimationLink* Material::FindAnimationLink(AnimTransform* animTrans) {
            return detail::FindAnimationLink(&mAnimList, animTrans);
        }

        void Material::SetAnimationEnable(AnimTransform* animTrans, bool bEnable) {
            if (AnimationLink* animLink = FindAnimationLink(animTrans)) {
                animLink->SetEnable(bEnable);
            }
        }

        Size detail::GetTextureSize(Material* pMaterial, u8 texMapIdx) {
            if (texMapIdx >= pMaterial->GetTextureNum()) {
                return Size(0.0f, 0.0f);
            }

            GXTexObj texObj;
            pMaterial->GetTexture(&texObj, texMapIdx);

            return Size(GXGetTexObjWidth(&texObj), GXGetTexObjHeight(&texObj));
        }
    }  // namespace lyt
}  // namespace nw4hbm
