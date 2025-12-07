/**
 * @file d_a_obj_flag2.cpp
 *
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_flag2.h"
#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_set_bgobj.h"
#include "d/d_a_obj.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"

#if DEBUG
class daObjFlag2_Hio_c : public JORReflexible {
public:
    daObjFlag2_Hio_c() {
        mCount = 0;
        default_set();
    }

    void default_set() { mAttr = daObjFlag2_c::M_attr; }

    void genMessage(JORMContext* ctx) {
        ctx->genLabel("§ 旗オブジェパラメータ設定  §\n", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
        ctx->genSlider("重力", &mAttr.mGravity, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
        ctx->genSlider("バネ係数", &mAttr.mSpringCoeeficient, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                       0x200, 0x18);
        ctx->genSlider("減衰率", &mAttr.mDecayRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff,
                       0x200, 0x18);
        ctx->genSlider("風係数", &mAttr.mWindCoefficient, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                       0x200, 0x18);
        ctx->genSlider("竜巻", &mAttr.mTornado, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    }

    void ct() {
        if (mCount++ == 0) {
            daObj::HioVarious_c::init(this, "旗"); // Flag
        }
    }

    void dt() {
        if (--mCount == 0) {
            daObj::HioVarious_c::clean(this);
        }
    }

    /* 0x4 */ int mCount;
    /* 0x8 */ daObjFlag2_Attr_c mAttr;
};

daObjFlag2_Hio_c M_hio;
#endif

daObjFlag2_Attr_c const daObjFlag2_c::M_attr = {
    -2.0f, 0.74f, 0.68f, 40.0f, 100.0f,
};

static Vec l_pennant_flag_pos[21] = {
    0.0f,   0.0f,   500.0f, 0.0f,   30.0f,  400.0f, 0.0f,    -30.0f, 400.0f, 0.0f,   60.0f,
    300.0f, 0.0f,   0.0f,   300.0f, 0.0f,   -60.0f, 300.0f,  0.0f,   90.0f,  200.0f, 0.0f,
    30.0f,  200.0f, 0.0f,   -30.0f, 200.0f, 0.0f,   -90.0f,  200.0f, 0.0f,   120.0f, 100.0f,
    0.0f,   60.0f,  100.0f, 0.0f,   0.0f,   100.0f, 0.0f,    -60.0f, 100.0f, 0.0f,   -120.0f,
    100.0f, 0.0f,   150.0f, 0.0f,   0.0f,   90.0f,  0.0f,    0.0f,   30.0f,  0.0f,   0.0f,
    -30.0f, 0.0f,   0.0f,   -90.0f, 0.0f,   0.0f,   -150.0f, 0.0f,
};

void FlagCloth_c::initFlagPos(cXyz* pFlagPos, fopAc_ac_c* i_actor) {
    mpFlagPosition = pFlagPos;
    cXyz direction;
    f32 power;
    dKyw_get_AllWind_vec(mpFlagPosition, &direction, &power);
    mDoMtx_stack_c::transS(*mpFlagPosition);
    cMtx_copy(mDoMtx_stack_c::get(), mModelMtx);
    cXyz* pPos = getPos();
    for (int i = 0; i < 21; i++, pPos++) {
        pPos->set(l_pennant_flag_pos[i]);
        if (i != 15 && i != 20) {
            pPos->x += cM_rndFX(10.0f);
            pPos->y += cM_rndFX(10.0f);
            pPos->z += cM_rndFX(10.0f);
        }
    }
    
    cXyz* pNormal = getNormal();
    for (int i = 0; i < 21; i++, pNormal++) {
        calcFlagNormal(pNormal, i);
    }
    calcFlagNormalBack();
    initCcSphere(i_actor);
}

static f32 l_texCoord_64x64[42] = {
    1.0,  0.5,  0.8f, 0.4f, 0.8f, 0.6f, 0.6f, 0.3f, 0.6f, 0.5,  0.6f, 0.7f, 0.4f, 0.2f,
    0.4f, 0.4f, 0.4f, 0.6f, 0.4f, 0.8f, 0.2f, 0.1f, 0.2f, 0.3f, 0.2f, 0.5,  0.2f, 0.7f,
    0.2f, 0.9f, 0.0,  0.0,  0.0,  0.2f, 0.0,  0.4f, 0.0,  0.6f, 0.0,  0.8f, 0.0,  1.0,
};

static f32 l_texCoord[42] = {
    0.0,  0.5,  0.2f, 0.4f, 0.2f, 0.6f, 0.4f, 0.3f, 0.4f, 0.5,  0.4f, 0.7f, 0.6f, 0.2f,
    0.6f, 0.4f, 0.6f, 0.6f, 0.6f, 0.8f, 0.8f, 0.1f, 0.8f, 0.3f, 0.8f, 0.5,  0.8f, 0.7f,
    0.8f, 0.9f, 1.0,  0.0,  1.0,  0.2f, 1.0,  0.4f, 1.0,  0.6f, 1.0,  0.8f, 1.0,  1.0,
};

static u8 l_pennant_flagDL[152] ATTRIBUTE_ALIGN(32) = {
    0x98, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x04, 0x04, 0x04, 0x05,
    0x05, 0x05, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x0D, 0x0D, 0x0D, 0x0E, 0x0E, 0x0E, 0x13, 0x13,
    0x13, 0x14, 0x14, 0x14, 0x98, 0x00, 0x09, 0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04,
    0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x12, 0x12, 0x12, 0x13,
    0x13, 0x13, 0x98, 0x00, 0x07, 0x03, 0x03, 0x03, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x0B, 0x0B,
    0x0B, 0x0C, 0x0C, 0x0C, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x98, 0x00, 0x05, 0x06, 0x06, 0x06,
    0x0A, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x90, 0x00, 0x03, 0x10,
    0x10, 0x10, 0x0A, 0x0A, 0x0A, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

cXyz FlagCloth_c::calcFlagFactor(cXyz* param_1, cXyz* param_2, cXyz* param_3, int param_4) {
    static int rel_pos_idx_tbl[126] = {
        1,  2,  -1, -1, -1, -1, 3,  4,  2,  0,  -1, -1, 0,  1,  4,  5,  -1, -1, 6,  7,  4,
        1,  -1, -1, 1,  3,  7,  8,  5,  2,  2,  4,  8,  9,  -1, -1, 10, 11, 7,  3,  -1, -1,
        3,  6,  11, 12, 8,  4,  4,  7,  12, 13, 9,  5,  5,  8,  13, 14, -1, -1, 15, 16, 11,
        6,  -1, -1, 6,  10, 16, 17, 12, 7,  7,  11, 17, 18, 13, 8,  8,  12, 18, 19, 14, 9,
        9,  13, 19, 20, -1, -1, 16, 10, -1, -1, -1, -1, 17, 11, 10, 15, -1, -1, 18, 12, 11,
        16, -1, -1, 19, 13, 12, 17, -1, -1, 20, 14, 13, 18, -1, -1, 14, 19, -1, -1, -1, -1,
    };

    cXyz flagFactor(0.0f, 0.0f, 0.0f);
    if (param_4 == 15 || param_4 == 20) {
        return cXyz::Zero;
    }

    int* pRelPosIdx = rel_pos_idx_tbl + param_4 * 6;
    flagFactor = param_2[param_4] * param_3->inprod(param_2[param_4]);
    flagFactor.y += mGravity;
    for (int i = 0; i < 6; i++, pRelPosIdx++) {
        if (*pRelPosIdx == -1) {
            break;
        }
        f32 fVar1;
        if (*pRelPosIdx == 0 || param_4 == 0 || abs(*pRelPosIdx - param_4) > 1) {
            fVar1 = 104.40307f;
        } else {
            fVar1 = 60.0f;
        }
        calcFlagFactorSub(param_1 + param_4, param_1 + *pRelPosIdx, &flagFactor, fVar1);
    }
    return flagFactor;
}

void FlagCloth_c::calcFlagFactorSub(cXyz* param_1, cXyz* param_2, cXyz* param_3, f32 param_4) {
    cXyz acStack_2c = *param_2 - *param_1;
    param_4 = acStack_2c.abs() - param_4;
    cXyz cStack_38 = acStack_2c.normZC();
    param_4 *= mSpringRate;
    cStack_38 *= param_4;
    *param_3 += cStack_38;
}

void FlagCloth_c::calcFlagNormal(cXyz* o_normal, int param_2) {
    static int rel_pos_idx_tbl[147] = {
        1,  2,  -1, -1, -1, -1, -1, 3,  4,  2,  0,  -1, -1, -1, 0,  1,  4,  5,  -1, -1, -1,
        6,  7,  4,  1,  -1, -1, -1, 1,  3,  7,  8,  5,  2,  1,  2,  4,  8,  9,  -1, -1, -1,
        10, 11, 7,  3,  -1, -1, -1, 3,  6,  11, 12, 8,  4,  3,  4,  7,  12, 13, 9,  5,  4,
        5,  8,  13, 14, -1, -1, -1, 15, 16, 11, 6,  -1, -1, -1, 6,  10, 16, 17, 12, 7,  6,
        7,  11, 17, 18, 13, 8,  7,  8,  12, 18, 19, 14, 9,  8,  9,  13, 19, 20, -1, -1, -1,
        16, 10, -1, -1, -1, -1, -1, 17, 11, 10, 15, -1, -1, -1, 18, 12, 11, 16, -1, -1, -1,
        19, 13, 12, 17, -1, -1, -1, 20, 14, 13, 18, -1, -1, -1, 14, 19, -1, -1, -1, -1, -1,
    };
    cXyz* pPos = getPos();
    cXyz acStack_30(pPos[param_2]);
    cXyz cStack_3c;
    cXyz cStack_48;
    cXyz cStack_54;
    int* piVar3 = rel_pos_idx_tbl + param_2 * 7;
    cXyz cStack_60(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 6; i++, piVar3++) {
        if (piVar3[1] == -1) {
            break;
        }
        cStack_3c = pPos[piVar3[0]] - acStack_30;
        cStack_48 = pPos[piVar3[1]] - acStack_30;
        cStack_54 = cStack_48.outprod(cStack_3c);
        cStack_60 += cStack_54;
    }
    cStack_60.normalizeZP();
    o_normal->set(cStack_60);
}

inline void FlagCloth_c::calcFlagNormalBack() {
    cXyz* pNormal = getNormal();
    cXyz* pNormalBack = getNormalBack();
    for (int i = 0; i < 21; pNormal++, pNormalBack++, i++) {
        pNormalBack->set(-pNormal->x, -pNormal->y, -pNormal->z);
    }
}

void FlagCloth_c::initCcSphere(fopAc_ac_c* i_actor) {
    const static dCcD_SrcSph ccSphSrc = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0x10000, 0x11}, 0x0}},  // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},              // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x1},              // mGObjTg
            {0x0},                                           // mGObjCo
        },                                                   // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 100.0f}  // mSph
        }  // mSphAttr
    };

    mStts.Init(0xff, 0xff, i_actor);
    mSph.Set(ccSphSrc);
    mSph.SetStts(&mStts);
    mSph.SetC(getTargetPos());
}

void FlagCloth_c::setCcSphere() {
    if (mSph.ChkTgHit()) {
        if (mSph.GetTgHitObj()->ChkAtType(AT_TYPE_BOOMERANG)) {
            cXyz* pVec = getVec();
            for (int i = 0; i < 21; i++, pVec++) {
                if (i != 15 && i != 20) {
                    pVec->y += mTornado;
                }
            }
        }
    }
    mSph.SetC(getTargetPos());
    dComIfG_Ccsp()->Set(&mSph);
}

void FlagCloth_c::execute() {
    cXyz direction;
    f32 power;
    dKyw_get_AllWind_vec(mpFlagPosition, &direction, &power);
    direction.normalizeZP();
    direction *= power * mWindRate;
    cXyz* pPos = getPos();
    cXyz* pNormal = getNormal();
    cXyz* pNormal2 = pNormal;
    cXyz* pVec = getVec();
    cXyz cStack_40;
    for (int i = 0; i < 21; pVec++, i++) {
        cXyz cStack_40 = calcFlagFactor(pPos, pNormal, &direction, i);
        *pVec += cStack_40;
        *pVec *= mDecayRate;
    }

    pNormal = getVec();
    for (int i = 0; i < 21; pPos++, pNormal++, i++) {
        *pPos += *pNormal;
    }

    for (int i = 0; i < 21; pNormal2++, i++) {
        calcFlagNormal(pNormal2, i);
    }
    calcFlagNormalBack();
    setCcSphere();
    DCStoreRangeNoSync(getPos(), sizeof(cXyz) * 21);
    DCStoreRangeNoSync(getNormal(), sizeof(cXyz) * 21);
    DCStoreRangeNoSync(getNormalBack(), sizeof(cXyz) * 21);
}

void FlagCloth_c::draw() {
    j3dSys.reinitGX();
    GXSetNumIndStages(0);
    dKy_setLight_again();
    dKy_GxFog_tevstr_set(&mTevStr);
    dKy_setLight_mine(&mTevStr);
    g_env_light.settingTevStruct(0x10, mpFlagPosition, &mTevStr);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_CLR_RGB, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
    GXSetArray(GX_VA_POS, getPos(), sizeof(cXyz));
    GXSetArray(GX_VA_NRM, getNormal(), sizeof(cXyz));
    GXSetArray(GX_VA_TEX0, mpTexCoord, 8);
    GXSetZCompLoc(GX_FALSE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXLoadTexObj(&mTexObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, 0xff, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c);
    GXSetNumTevStages(1);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    dKy_Global_amb_set(&mTevStr);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, 0xff});
    GXSetTevKColor(GX_KCOLOR0, (GXColor){0, 0, 0, 0xff});
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_KONST, GX_CC_TEXC, GX_CC_RASC, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_4, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_KONST, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K3_A);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    Mtx viewModelMtx;
    cMtx_concat(j3dSys.getViewMtx(), mModelMtx, viewModelMtx);
    GXLoadPosMtxImm(viewModelMtx, 0);
    GXLoadNrmMtxImm(viewModelMtx, 0);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetCullMode(GX_CULL_BACK);
    GXCallDisplayList(l_pennant_flagDL, 0x80);
    GXSetArray(GX_VA_NRM, getNormalBack(), sizeof(cXyz));
    GXSetCullMode(GX_CULL_FRONT);
    GXCallDisplayList(l_pennant_flagDL, 0x80);
    J3DShape::resetVcdVatCache();
}

void daObjFlag2_c::create_init() {
    int* pOffset = (int*)dComIfG_getObjectRes(daSetBgObj_c::getArcName(this), "spec.dat");
    f32 offset = (u16)*pOffset;
    mFlagPosition.set(current.pos.x, current.pos.y + offset, current.pos.z);
    fopAcM_setCullSizeBox(this, -600.0f, -offset, -600.0f, 600.0f, 400.0f, 600.0f);
    eyePos.set(mFlagCloth.getTargetPos());
    mFlagCloth.initFlagPos(&mFlagPosition, this);
    mFlagCloth.setSpringRate(attr().mSpringCoeeficient);
    mFlagCloth.setWindRate(attr().mWindCoefficient);
    mFlagCloth.setDecayRate(attr().mDecayRate);
    mFlagCloth.setGravity(attr().mGravity);
    mFlagCloth.setTornado(attr().mTornado);
    dKy_tevstr_init(&mFlagCloth.mTevStr, fopAcM_GetRoomNo(this), 0xff);
    initBaseMtx();
    initCollision();

    #if DEBUG
    M_hio.ct();
    #endif
}

void daObjFlag2_c::initBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    fopAcM_SetMtx(this, mModel->getBaseTRMtx());
}

void daObjFlag2_c::initCollision() {
    const static dCcD_SrcCyl ccCylSrc = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x79}},  // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},          // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4},          // mGObjTg
            {0x0},                                       // mGObjCo
        },                                               // mObjInf
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            15.0f,               // mRadius
            30.0f                // mHeight
        }  // mCyl
    };
    mStts.Init(0xff, 0xff, this);
    mCyl.Set(ccCylSrc);
    mCyl.SetStts(&mStts);
    mCyl.SetC(current.pos);
    mCyl.SetR(15.0f);
    mCyl.SetH(mFlagPosition.y + 200.0f);
    setCollision();
}

void daObjFlag2_c::setCollision() {
    dComIfG_Ccsp()->Set(&mCyl);
}

int daObjFlag2_c::createHeap() {
    s8 flagNum = (u8)shape_angle.x;
    if (mFlagValid) {
        char acStack_40[16];
        sprintf(acStack_40, "flag%02d.bti", flagNum);
        shape_angle.setall(0);
        current.angle.setall(0);
        ResTIMG* image = (ResTIMG*)dComIfG_getObjectRes(mFlagName, "flag.bti");
        JUT_ASSERT(838, image != NULL);
        GXInitTexObj(mFlagCloth.getImageTexObj(), (u8*)image + image->imageOffset, image->width,
                     image->height, (GXTexFmt)image->format, (GXTexWrapMode)image->wrapS,
                     (GXTexWrapMode)image->wrapT, image->mipmapCount > 1 ? GX_TRUE : GX_FALSE);
        if (image->mipmapCount > 1) {
            GXInitTexObjLOD(mFlagCloth.getImageTexObj(), (GXTexFilter)image->minFilter,
                            (GXTexFilter)image->magFilter, image->minLOD * 0.125f,
                            image->maxLOD * 0.125f, image->LODBias * 0.01f, image->biasClamp,
                            image->doEdgeLOD, (GXAnisotropy)image->maxAnisotropy);
        }
        if (image->width == image->height) {
            mFlagCloth.setTexCoord_p(l_texCoord_64x64);
        } else if (image->width == (image->height << 1)) {
            mFlagCloth.setTexCoord_p(l_texCoord);
        } else {
            JUT_ASSERT(861, FALSE);
        }
    }
    J3DModelData* modelData_pole = (J3DModelData*)dComIfG_getObjectRes(daSetBgObj_c::getArcName(this), "model0.bmd");
    JUT_ASSERT(865, modelData_pole != NULL);
    mModel = mDoExt_J3DModel__create(modelData_pole, 0x80000, 0x11000084);
    return mModel != NULL ? TRUE : FALSE;
}

static int createSolidHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjFlag2_c*>(i_this)->createHeap();
}

int daObjFlag2_c::create() {
    fopAcM_ct(this, daObjFlag2_c);
    s8 flagNum = (u8)shape_angle.x;
    if (flagNum <= -1 || flagNum > 99) {
        mFlagValid = false;
    } else {
        mFlagValid = true;
        sprintf(mFlagName, "FlagObj%02d", flagNum);
        int rv = dComIfG_resLoad(&mFlagPhase, mFlagName);
        if (rv != cPhs_COMPLEATE_e) {
            return rv;
        }
    }

    int rv = dComIfG_resLoad(&mArcPhase, daSetBgObj_c::getArcName(this));
    if (rv == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, createSolidHeap, 0x820) == 0) {
            return cPhs_ERROR_e;
        } else {
            create_init();
        }
    }
    return rv;
}

int daObjFlag2_c::draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    dComIfGd_setListBG();
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    mDoExt_modelUpdateDL(mModel);
    if (mFlagValid) {
        j3dSys.getDrawBuffer(0)->entryImm(&mFlagCloth, 0);
    }
    dComIfGd_setList();
    return 1;
}

static int daObjFlag2_Draw(daObjFlag2_c* i_this) {
    return i_this->draw();
}

int daObjFlag2_c::execute() {
    if (!mFlagValid) {
        return 1;
    }

    #if DEBUG
    mFlagCloth.setSpringRate(attr().mSpringCoeeficient);
    mFlagCloth.setWindRate(attr().mWindCoefficient);
    mFlagCloth.setDecayRate(attr().mDecayRate);
    mFlagCloth.setGravity(attr().mGravity);
    mFlagCloth.setTornado(attr().mTornado);
    #endif

    cXyz direction;
    f32 power;
    dKyw_get_AllWind_vec(&mFlagPosition, &direction, &power);
    if (power > 0.0f) {
        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_FLAG_TRAILING, &mFlagPosition, power * 127.0f, 0,
                                      1.0f, 1.0f, -1.0f, -1.0f, 0);
    }
    mFlagCloth.execute();
    eyePos = mFlagCloth.getTargetPos();
    setCollision();
    return 1;
}

static int daObjFlag2_Execute(daObjFlag2_c* i_this) {
    return i_this->execute();
}

static int daObjFlag2_IsDelete(daObjFlag2_c* i_this) {
    return 1;
}

static int daObjFlag2_Delete(daObjFlag2_c* i_this) {
    fopAcM_GetID(i_this);
    i_this->~daObjFlag2_c();
    return 1;
}

daObjFlag2_c::~daObjFlag2_c() {
    #if DEBUG
    M_hio.dt();
    #endif

    if (mFlagValid) {
        dComIfG_resDelete(&mFlagPhase, mFlagName);
    }
    dComIfG_resDelete(&mArcPhase, daSetBgObj_c::getArcName(this));
}

static int daObjFlag2_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjFlag2_c*>(i_this)->create();
}

static actor_method_class l_daObjFlag2_Method = {
    (process_method_func)daObjFlag2_Create,
    (process_method_func)daObjFlag2_Delete,
    (process_method_func)daObjFlag2_Execute,
    (process_method_func)daObjFlag2_IsDelete,
    (process_method_func)daObjFlag2_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Flag2 = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Flag2,         // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjFlag2_c),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  27,                     // mPriority
  &l_daObjFlag2_Method,   // sub_method
  0x00044180,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
