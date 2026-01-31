#ifndef NW4HBM_LYT_TYPES_H
#define NW4HBM_LYT_TYPES_H

#include <revolution/types.h>

#include <revolution/gx.h>

#include "../math/types.h"
#include "../ut/LinkList.h"

namespace nw4hbm {
    namespace lyt {
        static const u32 RESOURCE_TYPE_ANIMATION = 'anim';
        static const u32 RESOURCE_TYPE_LAYOUT = 'blyt';
        static const u32 RESOURCE_TYPE_FONT = 'font';
        static const u32 RESOURCE_TYPE_TEXTURE = 'timg';

        enum {
            VERTEXCOLOR_LT = 0,
            VERTEXCOLOR_RT,
            VERTEXCOLOR_LB,
            VERTEXCOLOR_RB,

            VERTEXCOLOR_MAX
        };

        enum {
            TEVCOLOR_REG0 = 0,
            TEVCOLOR_REG1,
            TEVCOLOR_REG2,

            TEVCOLOR_MAX
        };

        enum {
            HORIZONTALPOSITION_LEFT = 0,
            HORIZONTALPOSITION_CENTER,
            HORIZONTALPOSITION_RIGHT,
            HORIZONTALPOSITION_MAX
        };
        enum {
            VERTICALPOSITION_TOP = 0,
            VERTICALPOSITION_CENTER,
            VERTICALPOSITION_BOTTOM,
            VERTICALPOSITION_MAX
        };

        enum {
            ORIGINTYPE_TOPLEFT = 0,
            ORIGINTYPE_CENTER,

            ORIGINTYPE_MAX
        };

        enum {
            TEXTCOLOR_TOP = 0,
            TEXTCOLOR_BOTTOM,

            TEXTCOLOR_MAX
        };

        enum {
            WINDOWFRAME_LT = 0,
            WINDOWFRAME_RT,
            WINDOWFRAME_LB,
            WINDOWFRAME_RB,

            WINDOWFRAME_L,
            WINDOWFRAME_R,
            WINDOWFRAME_T,
            WINDOWFRAME_B,

            WINDOWFRAME_MAX
        };

        enum {
            TEXTUREFLIP_NONE,
            TEXTUREFLIP_H,
            TEXTUREFLIP_V,
            TEXTUREFLIP_90,
            TEXTUREFLIP_180,
            TEXTUREFLIP_270,

            TEXTUREFLIP_MAX
        };

        enum {
            FLIPINDEX_X,
            FLIPINDEX_Y,

            FLIPINDEX_MAX
        };

        namespace detail {
            template <typename T>
            const T* ConvertOffsToPtr(const void* baseAddress, unsigned offset) {
                return reinterpret_cast<const T*>(reinterpret_cast<unsigned>(baseAddress) + offset);
            }

            template <typename T>
            T* ConvertOffsToPtr(void* baseAddress, unsigned offset) {
                return reinterpret_cast<T*>(reinterpret_cast<unsigned>(baseAddress) + offset);
            }

            template <typename T>
            inline void SetBit(T* pBits, int pos, bool val) {
                const T mask = (T)(~(1 << pos));
                *pBits &= mask;
                *pBits |= (val ? 1 : 0) << pos;
            }

            template <typename T>
            inline bool TestBit(T bits, int pos) {
                const T mask = (T)(1 << pos);
                return 0 != (bits & mask);
            }

            template <typename T>
            T GetBits(T bits, int pos, int len) {
                T mask = static_cast<T>(~(-1 << len));
                return static_cast<T>((bits >> pos) & mask);
            }
        }  // namespace detail

        typedef struct Size {
            Size() : width(), height() {}

            Size(f32 aWidth, f32 aHeight) : width(aWidth), height(aHeight) {}

            Size(const Size& other) : width(other.width), height(other.height) {}

            friend bool operator==(const Size& a, const Size& b) {
                return a.width == b.width && a.height == b.height;
            }

            /* 0x00 */ f32 width;
            /* 0x04 */ f32 height;
        } Size;  // size = 0x08

        typedef struct TexSRT {
            /* 0x00 */ math::VEC2 translate;
            /* 0x08 */ f32 rotate;
            /* 0x0C */ math::VEC2 scale;
        } TexSRT;

        class TexCoordGen {
        public:
            TexCoordGen() : reserve(0) { Set(GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY); }

            u32 GetTexMtx() const { return texMtx; }

            GXTexGenType GetTexGenType() const { return static_cast<GXTexGenType>(texGenType); }
            GXTexGenSrc GetTexGenSrc() const { return static_cast<GXTexGenSrc>(texGenSrc); }

            void Set(GXTexGenType aTexGenType, GXTexGenSrc aTexGenSrc, u32 aTexMtx) {
                texGenType = aTexGenType;
                texGenSrc = aTexGenSrc;
                texMtx = aTexMtx;
            }

        private:
            /* 0x00 */ u8 texGenType;
            /* 0x01 */ u8 texGenSrc;
            /* 0x02 */ u8 texMtx;
            /* 0x03 */ u8 reserve;
        };

        class IndirectStage {
        public:
            IndirectStage() { Set(GX_TEXCOORD0, GX_TEXMAP0, GX_ITS_1, GX_ITS_1); }

            GXTexCoordID GetTexCoordGen() const { return static_cast<GXTexCoordID>(texCoordGen); }

            GXTexMapID GetTexMap() const { return static_cast<GXTexMapID>(texMap); }

            GXIndTexScale GetScaleS() const { return static_cast<GXIndTexScale>(scaleS); }
            GXIndTexScale GetScaleT() const { return static_cast<GXIndTexScale>(scaleT); }

            void Set(GXTexCoordID aTexCoordGen, GXTexMapID aTexMap, GXIndTexScale aScaleS,
                     GXIndTexScale aScaleT) {
                texCoordGen = aTexCoordGen;

                texMap = aTexMap;
                scaleS = aScaleS;
                scaleT = aScaleT;
            }

        private:
            /* 0x00 */ u8 texCoordGen;
            /* 0x01 */ u8 texMap;
            /* 0x02 */ u8 scaleS;
            /* 0x03 */ u8 scaleT;
        };

        class TevSwapMode {
        public:
            GXTevColorChan GetR() const { return static_cast<GXTevColorChan>((swap) & 0x03); }
            GXTevColorChan GetG() const { return static_cast<GXTevColorChan>((swap >> 2) & 0x03); }
            GXTevColorChan GetB() const { return static_cast<GXTevColorChan>((swap >> 4) & 0x03); }
            GXTevColorChan GetA() const { return static_cast<GXTevColorChan>((swap >> 6) & 0x03); }

            void Set(GXTevColorChan r, GXTevColorChan g, GXTevColorChan b, GXTevColorChan a) {
                swap = r | g << 2 | b << 4 | a << 6;
            }

        private:
            /* 0x00 */ u8 swap;
        };

        class TevStageInOp {
        public:
            u8 GetA() const { return ab & 0x0F; }
            u8 GetB() const { return (ab >> 4) & 0x0F; }
            u8 GetC() const { return cd & 0x0F; }
            u8 GetD() const { return (cd >> 4) & 0x0F; }

            u8 GetScale() const { return (op >> 6) & 0x03; }
            u8 GetBias() const { return (op >> 4) & 0x03; }
            u8 GetOp() const { return op & 0x0F; }

            u8 GetKSel() const { return (cl >> 3) & 0x1F; }
            u8 GetOutReg() const { return (cl >> 1) & 0x03; }
            bool IsClamp() const { return static_cast<bool>(cl & 0x01); }

            void SetIn(u8 a, u8 b, u8 c, u8 d) {
                ab = a | b << 4;
                cd = c | d << 4;
            }

            void SetOp(u8 aOp, u8 bias, u8 scale, bool clamp, u8 outReg, u8 kSel) {
                op = aOp | bias << 4 | scale << 6;
                cl = (clamp ? 1 : 0) | outReg << 1 | kSel << 3;
            }

        private:
            /* 0x00 */ u8 ab;
            /* 0x01 */ u8 cd;
            /* 0x02 */ u8 op;
            /* 0x03 */ u8 cl;
        };

        class TevStage {
        public:
            TevStage() {
                SetOrder(GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0, GX_TEV_SWAP0, GX_TEV_SWAP0);
                SetColorIn(GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
                SetAlphaIn(GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
                SetColorOp(GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV,
                           GX_TEV_KCSEL_K0);
                SetAlphaOp(GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV,
                           GX_TEV_KASEL_K0_R);
                SetIndirect(GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_OFF, GX_ITW_OFF,
                            GX_ITW_OFF, false, false, GX_ITBA_OFF);
            }

            GXTexCoordID GetTexCoordGen() const { return static_cast<GXTexCoordID>(texCoordGen); }
            GXChannelID GetColorChan() const { return static_cast<GXChannelID>(colChan); }

            GXTexMapID GetTexMap() const {
                return static_cast<GXTexMapID>((swapSel & 1) << 8 | texMap);
            }

            GXTevSwapSel GetTexSwapSel() const {
                return static_cast<GXTevSwapSel>((swapSel >> 3) & 0x03);
            }
            GXTevSwapSel GetRasSwapSel() const {
                return static_cast<GXTevSwapSel>((swapSel >> 1) & 0x03);
            }

            GXTevColorArg GetColorInA() const { return static_cast<GXTevColorArg>(colIn.GetA()); }
            GXTevColorArg GetColorInB() const { return static_cast<GXTevColorArg>(colIn.GetB()); }
            GXTevColorArg GetColorInC() const { return static_cast<GXTevColorArg>(colIn.GetC()); }
            GXTevColorArg GetColorInD() const { return static_cast<GXTevColorArg>(colIn.GetD()); }

            GXTevOp GetColorOp() const { return static_cast<GXTevOp>(colIn.GetOp()); }
            GXTevBias GetColorBias() const { return static_cast<GXTevBias>(colIn.GetBias()); }
            GXTevScale GetColorScale() const { return static_cast<GXTevScale>(colIn.GetScale()); }

            bool IsColorClamp() const { return colIn.IsClamp(); }

            GXTevRegID GetColorOutReg() const { return static_cast<GXTevRegID>(colIn.GetOutReg()); }

            GXTevKColorSel GetKColorSel() const {
                return static_cast<GXTevKColorSel>(colIn.GetKSel());
            }

            GXTevAlphaArg GetAlphaInA() const { return static_cast<GXTevAlphaArg>(alpIn.GetA()); }
            GXTevAlphaArg GetAlphaInB() const { return static_cast<GXTevAlphaArg>(alpIn.GetB()); }
            GXTevAlphaArg GetAlphaInC() const { return static_cast<GXTevAlphaArg>(alpIn.GetC()); }
            GXTevAlphaArg GetAlphaInD() const { return static_cast<GXTevAlphaArg>(alpIn.GetD()); }

            GXTevOp GetAlphaOp() const { return static_cast<GXTevOp>(alpIn.GetOp()); }
            GXTevBias GetAlphaBias() const { return static_cast<GXTevBias>(alpIn.GetBias()); }
            GXTevScale GetAlphaScale() const { return static_cast<GXTevScale>(alpIn.GetScale()); }

            bool IsAlphaClamp() const { return alpIn.IsClamp(); }

            GXTevRegID GetAlphaOutReg() const { return static_cast<GXTevRegID>(alpIn.GetOutReg()); }

            GXTevKAlphaSel GetKAlphaSel() const {
                return static_cast<GXTevKAlphaSel>(alpIn.GetKSel());
            }

            GXIndTexStageID GetIndStage() const { return static_cast<GXIndTexStageID>(indStage); }
            GXIndTexMtxID GetIndMtxSel() const {
                return static_cast<GXIndTexMtxID>((indBiMt >> 3) & 0x0F);
            }
            GXIndTexBiasSel GetIndBiasSel() const {
                return static_cast<GXIndTexBiasSel>(indBiMt & 0x07);
            }

            GXIndTexWrap GetIndWrapS() const { return static_cast<GXIndTexWrap>(indWrap & 0x07); }
            GXIndTexWrap GetIndWrapT() const {
                return static_cast<GXIndTexWrap>((indWrap >> 3) & 0x07);
            }

            GXIndTexAlphaSel GetIndAlphaSel() const {
                return static_cast<GXIndTexAlphaSel>((indFoAdUtAl >> 4) & 0x03);
            }

            bool IsIndUtcLod() const { return static_cast<bool>((indFoAdUtAl >> 3) & 0x01); }
            bool IsIndAddPrev() const { return static_cast<bool>((indFoAdUtAl >> 2) & 0x01); }

            GXIndTexFormat GetIndFormat() const {
                return static_cast<GXIndTexFormat>(indFoAdUtAl & 0x03);
            }

            void SetOrder(GXTexCoordID aTexCoordGen, GXTexMapID aTexMap, GXChannelID aColChan,
                          GXTevSwapSel rasSel, GXTevSwapSel texSel) {
                texCoordGen = aTexCoordGen;
                colChan = aColChan;
                texMap = aTexMap;
                swapSel = aTexMap >> 8 | rasSel << 1 | texSel << 3;
            }

            void SetColorIn(GXTevColorArg a, GXTevColorArg b, GXTevColorArg c, GXTevColorArg d) {
                colIn.SetIn(a, b, c, d);
            }

            void SetColorOp(GXTevOp op, GXTevBias bias, GXTevScale scale, bool clamp,
                            GXTevRegID outReg, GXTevKColorSel kSel) {
                colIn.SetOp(op, bias, scale, clamp, outReg, kSel);
            }

            void SetAlphaIn(GXTevAlphaArg a, GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d) {
                alpIn.SetIn(a, b, c, d);
            }

            void SetAlphaOp(GXTevOp op, GXTevBias bias, GXTevScale scale, bool clamp,
                            GXTevRegID outReg, GXTevKAlphaSel kSel) {
                alpIn.SetOp(op, bias, scale, clamp, outReg, kSel);
            }

            void SetIndirect(GXIndTexStageID stage, GXIndTexFormat format, GXIndTexBiasSel biasSel,
                             GXIndTexMtxID mtxSel, GXIndTexWrap wrapS, GXIndTexWrap wrapT,
                             bool addPrev, bool utcLod, GXIndTexAlphaSel alphaSel) {
                indStage = stage;
                indBiMt = biasSel | mtxSel << 3;
                indWrap = wrapS | wrapT << 3;
                indFoAdUtAl =
                    format | (addPrev ? 1 : 0) << 2 | (utcLod ? 1 : 0) << 3 | alphaSel << 4;
            }

        private:
            /* 0x00 */ u8 texCoordGen;
            /* 0x01 */ u8 colChan;
            /* 0x02 */ u8 texMap;
            /* 0x03 */ u8 swapSel;
            /* 0x04 */ TevStageInOp colIn;
            /* 0x08 */ TevStageInOp alpIn;
            /* 0x0C */ u8 indStage;
            /* 0x0D */ u8 indBiMt;
            /* 0x0E */ u8 indWrap;
            /* 0x0F */ u8 indFoAdUtAl;
        };

        class ChanCtrl {
        public:
            ChanCtrl() : reserve1(0), reserve2(0) { Set(GX_SRC_VTX, GX_SRC_VTX); }

            GXColorSrc GetColorSrc() const { return static_cast<GXColorSrc>(matSrcCol); }
            GXColorSrc GetAlphaSrc() const { return static_cast<GXColorSrc>(matSrcAlp); }

            void Set(GXColorSrc colSrc, GXColorSrc alpSrc) {
                matSrcCol = colSrc;
                matSrcAlp = alpSrc;
            }

        private:
            /* 0x00 */ u8 matSrcCol;
            /* 0x01 */ u8 matSrcAlp;
            /* 0x02 */ u8 reserve1;
            /* 0x03 */ u8 reserve2;
        };

        class AlphaCompare {
        public:
            AlphaCompare() { Set(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0); }

            GXCompare GetComp0() const { return static_cast<GXCompare>(comp & 0x0F); }
            GXCompare GetComp1() const { return static_cast<GXCompare>((comp >> 4) & 0x0F); }

            GXAlphaOp GetOp() const { return static_cast<GXAlphaOp>(op); }

            u8 GetRef0() const { return ref0; }
            u8 GetRef1() const { return ref1; }

            void Set(GXCompare aComp0, u8 aRef0, GXAlphaOp aOp, GXCompare aComp1, u8 aRef1) {
                comp = aComp0 | aComp1 << 4;
                op = aOp;
                ref0 = aRef0;
                ref1 = aRef1;
            }

        private:
            /* 0x00 */ u8 comp;
            /* 0x01 */ u8 op;
            /* 0x02 */ u8 ref0;
            /* 0x03 */ u8 ref1;
        };

        class BlendMode {
        public:
            BlendMode() { Set(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET); }

            GXBlendMode GetType() const { return static_cast<GXBlendMode>(type); }
            GXBlendFactor GetSrcFactor() const { return static_cast<GXBlendFactor>(srcFactor); }
            GXBlendFactor GetDstFactor() const { return static_cast<GXBlendFactor>(dstFactor); }
            GXLogicOp GetOp() const { return static_cast<GXLogicOp>(op); }

            void Set(GXBlendMode aType, GXBlendFactor aSrcFactor, GXBlendFactor aDstFactor,
                     GXLogicOp aOp) {
                type = aType;
                srcFactor = aSrcFactor;
                dstFactor = aDstFactor;
                op = aOp;
            }

        private:
            /* 0x00 */ u8 type;
            /* 0x01 */ u8 srcFactor;
            /* 0x02 */ u8 dstFactor;
            /* 0x03 */ u8 op;
        };

    }  // namespace lyt
}  // namespace nw4hbm

#endif
