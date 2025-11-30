#ifndef JPABASESHAPE_H
#define JPABASESHAPE_H

#include <dolphin/gx.h>

class JPAEmitterWorkData;
class JPABaseParticle;
class JKRHeap;

/**
 * @ingroup jsystem-jparticle
 * 
 */
struct JPABaseShapeData {
    // Common header.
    /* 0x00 */ u8 mMagic[4];
    /* 0x04 */ u32 mSize;

    /* 0x08 */ u32 mFlags;
    /* 0x0C */ s16 mClrPrmAnmOffset;
    /* 0x0E */ s16 mClrEnvAnmOffset;
    /* 0x10 */ f32 mBaseSizeX;
    /* 0x14 */ f32 mBaseSizeY;
    /* 0x18 */ u16 mBlendModeCfg;
    /* 0x1A */ u8 mAlphaCompareCfg;
    /* 0x1B */ u8 mAlphaRef0;
    /* 0x1C */ u8 mAlphaRef1;
    /* 0x1D */ u8 mZModeCfg;
    /* 0x1E */ u8 mTexFlg;
    /* 0x1F */ u8 mTexAnmNum;
    /* 0x20 */ u8 mTexIdx;
    /* 0x21 */ u8 mClrFlg;
    /* 0x22 */ u8 mClrPrmKeyNum;
    /* 0x23 */ u8 mClrEnvKeyNum;
    /* 0x24 */ s16 mClrAnmFrmMax;
    /* 0x26 */ GXColor mClrPrm;
    /* 0x2A */ GXColor mClrEnv;
    /* 0x2E */ u8 mAnmRndm;
    /* 0x2F */ u8 mClrAnmRndmMask;
    /* 0x30 */ u8 mTexAnmRndmMask;
};  // Size: 0x34

/**
 * @ingroup jsystem-jparticle
 * 
 */
struct JPABaseShape {
public:
    JPABaseShape(u8 const*, JKRHeap*);
    void setGX(JPAEmitterWorkData*) const;

    static GXBlendMode st_bm[3];
    static GXBlendFactor st_bf[10];
    static GXLogicOp st_lo[16];
    static GXCompare st_c[8];
    static GXAlphaOp st_ao[4];
    static GXTevColorArg st_ca[6][4];
    static GXTevAlphaArg st_aa[2][4];

    GXBlendMode getBlendMode() const { return st_bm[mpData->mBlendModeCfg & 0x03]; }
    GXBlendFactor getBlendSrc() const { return st_bf[(mpData->mBlendModeCfg >> 2) & 0x0F]; }
    GXBlendFactor getBlendDst() const { return st_bf[(mpData->mBlendModeCfg >> 6) & 0x0F]; }
    GXLogicOp getLogicOp() const { return st_lo[(mpData->mBlendModeCfg >> 10) & 0x0F]; }
    GXBool getZCompLoc() const { return (GXBool)((mpData->mZModeCfg >> 5) & 0x01); }

    GXBool getZEnable() const { return (GXBool)(mpData->mZModeCfg & 0x01); }
    GXCompare getZCmp() const { return st_c[(mpData->mZModeCfg >> 1) & 0x07]; }
    GXBool getZUpd() const { return (GXBool)((mpData->mZModeCfg >> 4) & 0x01); }

    GXCompare getAlphaCmp0() const { return st_c[mpData->mAlphaCompareCfg & 0x07]; }
    u8 getAlphaRef0() const { return mpData->mAlphaRef0; }
    GXAlphaOp getAlphaOp() const { return st_ao[(mpData->mAlphaCompareCfg >> 3) & 0x03]; }
    GXCompare getAlphaCmp1() const { return st_c[(mpData->mAlphaCompareCfg >> 5) & 0x07]; }
    u8 getAlphaRef1() const { return mpData->mAlphaRef1; }

    const GXTevColorArg* getTevColorArg() const { return st_ca[(mpData->mFlags >> 0x0F) & 0x07]; }
    const GXTevAlphaArg* getTevAlphaArg() const { return st_aa[(mpData->mFlags >> 0x12) & 0x01]; }

    u32 getType() const { return (mpData->mFlags >> 0) & 0x0F; }
    u32 getDirType() const { return (mpData->mFlags >> 4) & 0x07; }
    u32 getRotType() const { return (mpData->mFlags >> 7) & 0x07; }
    u32 getBasePlaneType() const { return (mpData->mFlags >> 10) & 0x01; }
    u32 getTilingS() const { return (mpData->mFlags >> 25) & 0x01; }
    u32 getTilingT() const { return (mpData->mFlags >> 26) & 0x01; }
    BOOL isGlblClrAnm() const { return mpData->mFlags & 0x00001000; }
    BOOL isGlblTexAnm() const { return mpData->mFlags & 0x00004000; }
    BOOL isPrjTex() const { return mpData->mFlags & 0x00100000; }
    bool isDrawFwdAhead() const { return !!(mpData->mFlags & 0x00200000); }
    bool isDrawPrntAhead() const { return !!(mpData->mFlags & 0x00400000); }
    bool isClipOn() const { return !!(mpData->mFlags & 0x00800000); }
    BOOL isTexCrdAnm() const { return mpData->mFlags & 0x01000000; }
    bool isNoDrawParent() const { return !!(mpData->mFlags & 0x08000000); }
    bool isNoDrawChild() const { return !!(mpData->mFlags & 0x10000000); }

    BOOL isPrmAnm() const { return mpData->mClrFlg & 0x02; }
    BOOL isEnvAnm() const { return mpData->mClrFlg & 0x08; }
    u8 getClrAnmType() const { return (mpData->mClrFlg >> 4) & 0x07; }
    s16 getClrAnmMaxFrm() const { return mpData->mClrAnmFrmMax; }
    void getPrmClr(GXColor* dst) { *dst = mpData->mClrPrm; }
    void getPrmClr(s16 idx, GXColor* dst) { *dst = mpPrmClrAnmTbl[idx]; }
    void getEnvClr(GXColor* dst) { *dst = mpData->mClrEnv; }
    void getEnvClr(s16 idx, GXColor* dst) { *dst = mpEnvClrAnmTbl[idx]; }

    BOOL isTexAnm() const { return mpData->mTexFlg & 0x01; }
    u8 getTexAnmType() const { return (mpData->mTexFlg >> 2) & 0x07; }
    u32 getTexIdx() const { return mpData->mTexIdx; }
    u8 getTexIdx(u8 idx) const { return mpTexIdxAnimTbl[idx]; }

    f32 getBaseSizeX() const { return mpData->mBaseSizeX; }
    f32 getBaseSizeY() const { return mpData->mBaseSizeY; }
    u8 getClrLoopOfstMask() const { return mpData->mClrAnmRndmMask; }
    u32 getClrLoopOfst(u32 param_1) const { return getClrLoopOfstMask() & param_1; }
    u8 getTexLoopOfstMask() const { return mpData->mTexAnmRndmMask; }
    u32 getTexLoopOfst(u8 param_1) const { return getTexLoopOfstMask() & param_1; }
    u8 getLoopOfstValue() const { return mpData->mAnmRndm; }

    f32 getIncTransX() const { return ((f32*)mpTexCrdMtxAnmTbl)[5]; }
    f32 getInitTransX() const { return ((f32*)mpTexCrdMtxAnmTbl)[0]; }
    f32 getIncTransY() const { return ((f32*)mpTexCrdMtxAnmTbl)[6]; }
    f32 getInitTransY() const { return ((f32*)mpTexCrdMtxAnmTbl)[1]; }
    f32 getIncScaleX() const { return ((f32*)mpTexCrdMtxAnmTbl)[7]; }
    f32 getInitScaleX() const { return ((f32*)mpTexCrdMtxAnmTbl)[2]; }
    f32 getIncScaleY() const { return ((f32*)mpTexCrdMtxAnmTbl)[8]; }
    f32 getInitScaleY() const { return ((f32*)mpTexCrdMtxAnmTbl)[3]; }
    f32 getIncRot() const { return ((f32*)mpTexCrdMtxAnmTbl)[9]; }
    f32 getInitRot() const { return ((f32*)mpTexCrdMtxAnmTbl)[4]; }
    u8 getTexAnmKeyNum() const { return mpData->mTexAnmNum; }

public:
    /* 0x00 */ const JPABaseShapeData* mpData;
    /* 0x04 */ const void* mpTexCrdMtxAnmTbl;
    /* 0x08 */ const u8* mpTexIdxAnimTbl;
    /* 0x0C */ GXColor* mpPrmClrAnmTbl;
    /* 0x10 */ GXColor* mpEnvClrAnmTbl;
};

/**
 * @ingroup jsystem-jparticle
 * 
 */
struct JPAClrAnmKeyData {
    /* 0x0 */ s16 index;
    /* 0x2 */ GXColor color;
};

void JPACalcTexIdxNormal(JPAEmitterWorkData*);
void JPACalcTexIdxRepeat(JPAEmitterWorkData*);
void JPACalcTexIdxReverse(JPAEmitterWorkData*);
void JPACalcTexIdxMerge(JPAEmitterWorkData*);
void JPACalcTexIdxRandom(JPAEmitterWorkData*);
void JPACalcPrm(JPAEmitterWorkData*);
void JPACalcEnv(JPAEmitterWorkData*);
void JPACalcClrIdxNormal(JPAEmitterWorkData*);
void JPACalcClrIdxRepeat(JPAEmitterWorkData*);
void JPACalcClrIdxReverse(JPAEmitterWorkData*);
void JPACalcClrIdxMerge(JPAEmitterWorkData*);
void JPACalcClrIdxRandom(JPAEmitterWorkData*);

void JPACalcTexIdxNormal(JPAEmitterWorkData*, JPABaseParticle*);
void JPACalcTexIdxRepeat(JPAEmitterWorkData*, JPABaseParticle*);
void JPACalcTexIdxReverse(JPAEmitterWorkData*, JPABaseParticle*);
void JPACalcTexIdxMerge(JPAEmitterWorkData*, JPABaseParticle*);
void JPACalcTexIdxRandom(JPAEmitterWorkData*, JPABaseParticle*);
void JPACalcPrm(JPAEmitterWorkData*, JPABaseParticle*);
void JPACalcEnv(JPAEmitterWorkData*, JPABaseParticle*);
void JPACalcClrIdxNormal(JPAEmitterWorkData*, JPABaseParticle*);
void JPACalcClrIdxRepeat(JPAEmitterWorkData*, JPABaseParticle*);
void JPACalcClrIdxReverse(JPAEmitterWorkData*, JPABaseParticle*);
void JPACalcClrIdxMerge(JPAEmitterWorkData*, JPABaseParticle*);
void JPACalcClrIdxRandom(JPAEmitterWorkData*, JPABaseParticle*);
void JPACalcColorCopy(JPAEmitterWorkData*, JPABaseParticle*);

void JPADrawStripe(JPAEmitterWorkData*);
void JPADrawStripeX(JPAEmitterWorkData*);
void JPADrawEmitterCallBackB(JPAEmitterWorkData*);
void JPALoadTex(JPAEmitterWorkData*);
void JPALoadTexAnm(JPAEmitterWorkData*);
void JPAGenTexCrdMtxIdt(JPAEmitterWorkData*);
void JPAGenCalcTexCrdMtxAnm(JPAEmitterWorkData*);
void JPAGenTexCrdMtxAnm(JPAEmitterWorkData*);
void JPAGenTexCrdMtxPrj(JPAEmitterWorkData*);
void JPALoadPosMtxCam(JPAEmitterWorkData*);
void JPASetLineWidth(JPAEmitterWorkData*);
void JPASetPointSize(JPAEmitterWorkData*);
void JPARegistPrm(JPAEmitterWorkData*);
void JPARegistEnv(JPAEmitterWorkData*);
void JPARegistPrmEnv(JPAEmitterWorkData*);

void JPADrawPoint(JPAEmitterWorkData*, JPABaseParticle*);
void JPADrawLine(JPAEmitterWorkData*, JPABaseParticle*);
void JPADrawRotBillboard(JPAEmitterWorkData*, JPABaseParticle*);
void JPADrawBillboard(JPAEmitterWorkData*, JPABaseParticle*);
void JPADrawRotDirection(JPAEmitterWorkData*, JPABaseParticle*);
void JPADrawDirection(JPAEmitterWorkData*, JPABaseParticle*);
void JPADrawRotation(JPAEmitterWorkData*, JPABaseParticle*);
void JPADrawDBillboard(JPAEmitterWorkData*, JPABaseParticle*);
void JPADrawRotYBillboard(JPAEmitterWorkData*, JPABaseParticle*);
void JPADrawYBillboard(JPAEmitterWorkData*, JPABaseParticle*);
void JPADrawParticleCallBack(JPAEmitterWorkData*, JPABaseParticle*);
void JPALoadTexAnm(JPAEmitterWorkData*, JPABaseParticle*);
void JPASetPointSize(JPAEmitterWorkData*, JPABaseParticle*);
void JPASetLineWidth(JPAEmitterWorkData*, JPABaseParticle*);
void JPALoadCalcTexCrdMtxAnm(JPAEmitterWorkData*, JPABaseParticle*);
void JPARegistAlpha(JPAEmitterWorkData*, JPABaseParticle*);
void JPARegistEnv(JPAEmitterWorkData*, JPABaseParticle*);
void JPARegistAlphaEnv(JPAEmitterWorkData*, JPABaseParticle*);
void JPARegistPrmAlpha(JPAEmitterWorkData*, JPABaseParticle*);
void JPARegistPrmAlphaEnv(JPAEmitterWorkData*, JPABaseParticle*);

#endif /* JPABASESHAPE_H */
