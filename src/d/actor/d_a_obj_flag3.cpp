/**
 * @file d_a_obj_flag3.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_flag3.h"
#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "d/actor/d_a_set_bgobj.h"
#include "d/d_com_inf_game.h"
#include "d/d_a_obj.h"

class daObjFlag3_Hio_c : public JORReflexible {
public:
    daObjFlag3_Hio_c() {
        mCount = 0;
        default_set();
    }

    void default_set() { mAttr = daObjFlag3_c::M_attr; }

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
            daObj::HioVarious_c::init(this, "四角旗"); // Four cornered Flag
        }
    }

    void dt() {
        if (--mCount == 0) {
            daObj::HioVarious_c::clean(this);
        }
    }

    /* 0x4 */ int mCount;
    /* 0x8 */ daObjFlag3_Attr_c mAttr;
};

#if DEBUG
daObjFlag3_Hio_c M_hio;
#endif

daObjFlag3_Attr_c const daObjFlag3_c::M_attr = {
    -2.0f, 0.6f, 0.68f, 40.0f, 100.0f,
};

inline cXyz FlagCloth2_c::calcFlagFactor(cXyz* param_1, cXyz* param_2, cXyz* param_3, int param_4) {
    cXyz flagFactor(0.0f, 0.0f, 0.0f);
    if (param_4 == 0 || param_4 == 30) {
        return cXyz::Zero;
    }

    int local_38[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
    if (param_4 == 5) {
        local_38[0] = param_4 + 6;
        local_38[1] = param_4 + 5;
        local_38[2] = param_4 - 1;
    } else if (param_4 == 35) {
        local_38[0] = param_4 - 1;
        local_38[1] = param_4 - 7;
        local_38[2] = param_4 - 6;
    } else if (param_4 > 0 && param_4 < 5) {
        local_38[0] = param_4 + 1;
        local_38[1] = param_4 + 7;
        local_38[2] = param_4 + 6;
        local_38[3] = param_4 + 5;
        local_38[4] = param_4 - 1;
    } else {
        if (param_4 > 30 && param_4 < 35) {
            local_38[0] = param_4 - 1;
            local_38[1] = param_4 - 7;
            local_38[2] = param_4 - 6;
            local_38[3] = param_4 - 5;
            local_38[4] = param_4 + 1;
        } else {
            if (param_4 % 6 == 0) {
                local_38[0] = param_4 - 6;
                local_38[1] = param_4 - 5;
                local_38[2] = param_4 + 1;
                local_38[3] = param_4 + 7;
                local_38[4] = param_4 + 6;
            } else if (param_4 % 6 == 5) {
                local_38[0] = param_4 + 6;
                local_38[1] = param_4 + 5;
                local_38[2] = param_4 - 1;
                local_38[3] = param_4 - 7;
                local_38[4] = param_4 - 6;
            } else {
                local_38[0] = param_4 - 7;
                local_38[1] = param_4 - 6;
                local_38[2] = param_4 - 5;
                local_38[3] = param_4 + 1;
                local_38[4] = param_4 + 7;
                local_38[5] = param_4 + 6;
                local_38[6] = param_4 + 5;
                local_38[7] = param_4 - 1;
            }
        }
    }

    int* pRelPosIdx = local_38;
    flagFactor = param_2[param_4] * param_3->inprod(param_2[param_4]);
    flagFactor.y += mGravity;
    for (int i = 0; i < 8; i++, pRelPosIdx++) {
        if (*pRelPosIdx == -1) {
            break;
        }
        f32 fVar1;
        if (abs(*pRelPosIdx - param_4) == 1 || abs(*pRelPosIdx - param_4) == 6) {
            fVar1 = 60.0f;
        } else {
            fVar1 = 84.85281f;
        }
        calcFlagFactorSub(param_1 + param_4, param_1 + *pRelPosIdx, &flagFactor, fVar1);
    }
    return flagFactor;
}

inline void FlagCloth2_c::calcFlagFactorSub(cXyz* param_1, cXyz* param_2, cXyz* param_3, f32 param_4) {
    cXyz acStack_2c = *param_2 - *param_1;
    param_4 = acStack_2c.abs() - param_4;
    cXyz cStack_38 = acStack_2c.normZC();
    param_4 *= mSpringRate;
    cStack_38 *= param_4;
    *param_3 += cStack_38;
}

inline void FlagCloth2_c::calcFlagNormal(cXyz* param_1, int param_2) {
    cXyz* pPos = getPos();
    cXyz cStack_44(pPos[param_2]);
    int local_38[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
    if (param_2 != 0 && param_2 != 30) {
        if (param_2 == 5) {
            local_38[0] = param_2 + 6;
            local_38[1] = param_2 + 5;
            local_38[2] = param_2 - 1;
        } else if (param_2 == 35) {
            local_38[0] = param_2 - 1;
            local_38[1] = param_2 - 7;
            local_38[2] = param_2 - 6;
        } else if (param_2 > 0 && param_2 < 5) {
            local_38[0] = param_2 + 1;
            local_38[1] = param_2 + 7;
            local_38[2] = param_2 + 6;
            local_38[3] = param_2 + 5;
            local_38[4] = param_2 - 1;
        } else {
            if (param_2 > 30 && param_2 < 35) {
                local_38[0] = param_2 - 1;
                local_38[1] = param_2 - 7;
                local_38[2] = param_2 - 6;
                local_38[3] = param_2 - 5;
                local_38[4] = param_2 + 1;
            } else {
                if (param_2 % 6 == 0) {
                    local_38[0] = param_2 - 6;
                    local_38[1] = param_2 - 5;
                    local_38[2] = param_2 + 1;
                    local_38[3] = param_2 + 7;
                    local_38[4] = param_2 + 6;
                } else if (param_2 % 6 == 5) {
                    local_38[0] = param_2 + 6;
                    local_38[1] = param_2 + 5;
                    local_38[2] = param_2 - 1;
                    local_38[3] = param_2 - 7;
                    local_38[4] = param_2 - 6;
                } else {
                    local_38[0] = param_2 - 7;
                    local_38[1] = param_2 - 6;
                    local_38[2] = param_2 - 5;
                    local_38[3] = param_2 + 1;
                    local_38[4] = param_2 + 7;
                    local_38[5] = param_2 + 6;
                    local_38[6] = param_2 + 5;
                    local_38[7] = param_2 - 1;
                    local_38[8] = local_38[0];
                }
            }
        }
    }
    cXyz cStack_50;
    cXyz cStack_5c;
    cXyz cStack_68;
    int* piVar2 = local_38;
    cXyz cStack_74(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 8; piVar2++, i++) {
        if (piVar2[1] == -1) {
            break;
        }
        cStack_50 = pPos[piVar2[0]] - cStack_44;
        cStack_5c = pPos[piVar2[1]] - cStack_44;
        cStack_68 = cStack_5c.outprod(cStack_50);
        cStack_74 += cStack_68;
    }
    cStack_74.normalizeZP();
    param_1->set(cStack_74);
}

void FlagCloth2_c::initCcSphere(fopAc_ac_c*) {
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
}

inline void FlagCloth2_c::draw() {
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
    GXSetArray(GX_VA_TEX0, mTexCoord, 8);
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

    for (int i = 0; i < 5; i++) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 12);
        for (int j = 0; j < 6; j++) {
            GXPosition1x8(i + j * 6);
            GXPosition1x8(i + j * 6);
            GXTexCoord1x8(i + j * 6);
            GXPosition1x8(i + j * 6 + 1);
            GXPosition1x8(i + j * 6 + 1);
            GXTexCoord1x8(i + j * 6 + 1);
        }
        GXEnd();
    }

    GXSetArray(GX_VA_NRM, getNormalBack(), sizeof(cXyz));
    GXSetCullMode(GX_CULL_FRONT);

    for (int i = 0; i < 5; i++) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 12);
        for (int j = 0; j < 6; j++) {
            GXPosition1x8(i + j * 6);
            GXPosition1x8(i + j * 6);
            GXTexCoord1x8(i + j * 6);
            GXPosition1x8(i + j * 6 + 1);
            GXPosition1x8(i + j * 6 + 1);
            GXTexCoord1x8(i + j * 6 + 1);
        }
        GXEnd();
    }

    J3DShape::resetVcdVatCache();
}

int daObjFlag3_c::createHeap() {
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
    }
    J3DModelData* modelData_pole = (J3DModelData*)dComIfG_getObjectRes(daSetBgObj_c::getArcName(this), "model0.bmd");
    JUT_ASSERT(865, modelData_pole != NULL);
    mModel = mDoExt_J3DModel__create(modelData_pole, 0x80000, 0x11000084);
    return mModel != NULL ? TRUE : FALSE;
}

static int createSolidHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjFlag3_c*>(i_this)->createHeap();
}

inline int daObjFlag3_c::draw() {
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

static int daObjFlag3_Draw(daObjFlag3_c* i_this) {
    return i_this->draw();
}

static int daObjFlag3_Execute(daObjFlag3_c* i_this) {
    return i_this->execute();
}

int daObjFlag3_c::execute() {
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

    mFlagCloth.execute();
    eyePos.set(mFlagCloth.getTargetPos());
    return 1;
}


void FlagCloth2_c::execute() {
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
    for (int i = 0; i < 36; pVec++, i++) {
        cXyz cStack_40 = calcFlagFactor(pPos, pNormal, &direction, i);
        *pVec += cStack_40;
        *pVec *= mDecayRate;
    }

    pNormal = getVec();
    for (int i = 0; i < 36; pPos++, pNormal++, i++) {
        *pPos += *pNormal;
    }

    for (int i = 0; i < 36; pNormal2++, i++) {
        calcFlagNormal(pNormal2, i);
    }
    calcFlagNormalBack();
    DCStoreRangeNoSync(getPos(), sizeof(cXyz) * 36);
    DCStoreRangeNoSync(getNormal(), sizeof(cXyz) * 36);
    DCStoreRangeNoSync(getNormalBack(), sizeof(cXyz) * 36);
}

static int daObjFlag3_IsDelete(daObjFlag3_c* i_this) {
    return 1;
}

static int daObjFlag3_Delete(daObjFlag3_c* i_this) {
    fopAcM_GetID(i_this);
    i_this->~daObjFlag3_c();
    return 1;
}

daObjFlag3_c::~daObjFlag3_c() {
    #if DEBUG
    M_hio.dt();
    #endif

    if (mFlagValid) {
        dComIfG_resDelete(&mFlagPhase, mFlagName);
    }
    dComIfG_resDelete(&mArcPhase, daSetBgObj_c::getArcName(this));
}

FlagCloth2_c::~FlagCloth2_c() {}

static int daObjFlag3_Create(fopAc_ac_c* i_this) {
    fopAcM_GetID(i_this);
    return static_cast<daObjFlag3_c*>(i_this)->create();
}

void daObjFlag3_c::initBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    fopAcM_SetMtx(this, mModel->getBaseTRMtx());
}

void daObjFlag3_c::create_init() {
    int* pOffset = (int*)dComIfG_getObjectRes(daSetBgObj_c::getArcName(this), "spec.dat");
    f32 offset = (u16)*pOffset;
    field_0x1320.set(current.pos.x, current.pos.y + offset, current.pos.z);
    fopAcM_setCullSizeBox(this, -600.0f, -offset, -600.0f, 600.0f, 400.0f, 600.0f);
    eyePos.set(mFlagCloth.getTargetPos());
    mFlagCloth.initFlagPos(&field_0x1320, this);
    mFlagCloth.setSpringRate(attr().mSpringCoeeficient);
    mFlagCloth.setWindRate(attr().mWindCoefficient);
    mFlagCloth.setDecayRate(attr().mDecayRate);
    mFlagCloth.setGravity(attr().mGravity);
    mFlagCloth.setTornado(attr().mTornado);
    initBaseMtx();

    #if DEBUG
    M_hio.ct();
    #endif
}

void FlagCloth2_c::initFlagPos(cXyz* param_0, fopAc_ac_c* param_1) {
    mpFlagPosition = param_0;
    cXyz direction;
    f32 power;
    dKyw_get_AllWind_vec(mpFlagPosition, &direction, &power);
    mDoMtx_stack_c::transS(*mpFlagPosition);
    cMtx_copy(mDoMtx_stack_c::get(), mModelMtx);
    f32 dVar7 = -150.0f;
    f32 dVar8 = 0.0f;
    cXyz* pPos = getPos();
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++, pPos++) {
            pPos->set(0.0f, dVar7, dVar8);
            if (i != 0 && i != 30) {
                pPos->x += cM_rndFX(10.0f);
                pPos->y += cM_rndFX(10.0f);
                pPos->z += cM_rndFX(10.0f);
            }
            dVar8 += 60.0f;
        }
        dVar7 += 60.0f;
        dVar8 = 0.0f;
    }

    cXyz* pNormal = getNormal();
    for (int i = 0; i < 36; i++, pNormal++) {
        calcFlagNormal(pNormal, i);
    }
    calcFlagNormalBack();
    initTexCoord();
}

int daObjFlag3_c::create() {
    fopAcM_ct(this, daObjFlag3_c);
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
        if (fopAcM_entrySolidHeap(this, createSolidHeap, 0x4000) == 0) {
            return cPhs_ERROR_e;
        } else {
            create_init();
        }
    }
    return rv;
}

static actor_method_class l_daObjFlag3_Method = {
    (process_method_func)daObjFlag3_Create,
    (process_method_func)daObjFlag3_Delete,
    (process_method_func)daObjFlag3_Execute,
    (process_method_func)daObjFlag3_IsDelete,
    (process_method_func)daObjFlag3_Draw,
};

actor_process_profile_definition g_profile_Obj_Flag3 = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Flag3,         // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjFlag3_c),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  28,                     // mPriority
  &l_daObjFlag3_Method,   // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
