// d_particle is odd in that it doesn't appear to include dolzel.pch.
// It uses ...data pooling, but weak data from the PCH (e.g. Z2Calc::cNullVec)
// isn't present like would be expected for a TU using pooling.
//
// Note that it _is_ possible that it does include the PCH and there's just some
// not-yet-understood behavior causing the weak data to be stripped anyway.
// Notably, TWW's version of this TU _does_ use the PCH, but it also includes
// weak data from it (unlike here).

#include "d/d_particle.h"
#include "d/d_jnt_col.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"
#include "JSystem/JParticle/JPAEmitterManager.h"
#include "JSystem/JParticle/JPAResourceManager.h"
#include "JSystem/JMath/JMATrigonometric.h"
#include "stdio.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_lib.h"
#include "m_Do/m_Do_graphic.h"
#include "f_op/f_op_actor_mng.h"
#include "d/actor/d_a_player.h"
#include "SSystem/SComponent/c_math.h"

extern dPa_particleTracePcallBack_c JPTracePCB4;

void dPa_cleanupGX() {
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXEnableTexOffsets(GX_TEXCOORD0, GX_ENABLE, GX_ENABLE);
    GXEnableTexOffsets(GX_TEXCOORD1, GX_ENABLE, GX_ENABLE);
    GXEnableTexOffsets(GX_TEXCOORD2, GX_ENABLE, GX_ENABLE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetCoPlanar(GX_FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGB8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGB8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
    GXSetCurrentMtx(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetNumChans(0);
}

static GXColor l_lifeBallColor[3] = {
    {0xEB, 0x20, 0x78, 0xFF},
    {0x20, 0xF1, 0x9B, 0xFF},
    {0xEB, 0xD7, 0x2F, 0xFF},
};

dPa_followEcallBack::dPa_followEcallBack(u8 param_0, u8 param_1) {
    field_0x11 = param_0;
    field_0x12 = param_1;
    field_0x13 = 0;
    mpEmitter = NULL;
}

void dPa_followEcallBack::execute(JPABaseEmitter* param_0) {
    if ((field_0x12 == 0) && ((field_0x10 & 2) == 0)) {
        param_0->setGlobalTranslation(field_0x8->x, field_0x8->y, field_0x8->z);
        if (field_0xc != NULL) {
            JGeometry::TVec3<s16> aTStack_24;
            aTStack_24.x = field_0xc->x;
            aTStack_24.y = field_0xc->y;
            aTStack_24.z = field_0xc->z;
            param_0->setGlobalRotation(aTStack_24);
        }
    }

    if (param_0->isEnableDeleteEmitter()) {
        end();
    }
    if (isEnd()) {
        s16 globalAlpha = param_0->getGlobalAlpha() & 0xff;
        cLib_chaseS(&globalAlpha, 1, 4);
        param_0->setGlobalAlpha(globalAlpha);
    }
}

void dPa_followEcallBack::draw(JPABaseEmitter*) {}

void dPa_followEcallBack::setup(JPABaseEmitter* i_emitter, cXyz const* param_1,
                                csXyz const* param_2, s8) {
    remove();

    if (field_0x13 == 0) {
        mpEmitter = i_emitter;
        i_emitter->becomeImmortalEmitter();
    }

    if (field_0x12 == 0) {
        if (field_0x11 != 0) {
            i_emitter->becomeContinuousParticle();
        }
        field_0x8 = param_1;
        field_0xc = param_2;
        field_0x10 = 0;
    }
}

void dPa_followEcallBack::end() {
    if (mpEmitter != NULL) {
        mpEmitter->becomeInvalidEmitter();
        mpEmitter->quitImmortalEmitter();
        mpEmitter->setEmitterCallBackPtr(NULL);
        field_0x10 |= 1;
        mpEmitter = NULL;
    }
}

dPa_modelEcallBack dPa_modelEcallBack::mEcallback;

dPa_modelPcallBack dPa_modelEcallBack::mPcallback;

dPa_modelEcallBack::model_c* dPa_modelEcallBack::mModel;

/* 80450E9C 0001+00 data_80450E9C None */
/* 80450E9D 0001+00 data_80450E9D None */
/* 80450E9E 0002+00 data_80450E9E None */
static u8 struct_80450E9C;
static u8 struct_80450E9D;

//
void dPa_light8EcallBack::setup(JPABaseEmitter* param_0, cXyz const* param_1,
                                    csXyz const* param_2, s8 param_3) {
    param_0->setDrawTimes(2);
    param_0->setParticleCallBackPtr(dPa_control_c::getLight8PcallBack());
}

void dPa_gen_b_light8EcallBack::setup(JPABaseEmitter* param_0, cXyz const* param_1,
                                          csXyz const* param_2, s8 param_3) {
    param_0->setDrawTimes(2);
    param_0->setParticleCallBackPtr(dPa_control_c::getGen_b_Light8PcallBack());
    param_0->setUserWork(param_3);
}

void dPa_gen_d_light8EcallBack::setup(JPABaseEmitter* param_0, cXyz const* param_1,
                                          csXyz const* param_2, s8 param_3) {
    param_0->setDrawTimes(2);
    param_0->setParticleCallBackPtr(dPa_control_c::getGen_d_Light8PcallBack());
    param_0->setUserWork(param_3);
}

static void dPa_group_id_change(u32* param_0, u8* param_1) {
    if (*param_1 >= 15) {
        return;
    }

    if (*param_0 & 1) {
        param_1[0] = 0;
    } else if (*param_0 & 2) {
        param_1[0] = 1;
    } else if (*param_0 & 4) {
        param_1[0] = 2;
    } else if (*param_0 & 8) {
        param_1[0] = 3;
    } else if (*param_0 & 0x4000) {
        param_1[0] = 4;
    } else if (*param_0 & 0x8000) {
        param_1[0] = 5;
    }

    if (*param_0 & 0x10) {
        switch(*param_1) {
        case 0:
            *param_1 = 8;
            break;
        case 1:
            *param_1 = 9;
            break;
        case 2:
            *param_1 = 10;
            break;
        case 3:
            *param_1 = 11;
            break;
        case 4:
            *param_1 = 6;
            break;
        case 5:
            *param_1 = 7;
            break;
        }
    }

    if (*param_0 & 0x80) {
        *param_1 = 13;
    } else if (*param_0 & 0x1000) {
        *param_1 = 12;
    } else if (*param_0 & 0x2000) {
        *param_1 = 14;
    }
}

static void initiateLighting8(_GXColor& param_0, s16 param_1) {
    GXSetChanCtrl(GX_COLOR0, true, GX_SRC_REG, GX_SRC_VTX, 0xfe, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetChanCtrl(GX_ALPHA0, false, GX_SRC_REG, GX_SRC_VTX, 0xfe, GX_DF_CLAMP, GX_AF_SPOT);
    s32 r,g,b;
    GXColor color0;
    color0.r = r = (param_1 << 1) & 0x3e;
    color0.g = g = (param_1 >> 4) & 0x3e;
    color0.b = b = (param_1 >> 9) & 0x3e;
    if (daPy_py_c::checkNowWolfPowerUp()) {
        f32 fVar2;
        f32 fVar1 = (g_env_light.bg_amb_col[0].r / 255.0f);
        fVar2 = r + 0x10;
        color0.r = fVar2 * (4.0f * (fVar1));

        fVar2 = (g_env_light.bg_amb_col[0].g / 255.0f);
        fVar1 = g + 0x10;
        color0.g = fVar1 * (4.0f * (fVar2));

        fVar1 = b + 0x10;
        fVar2 = (g_env_light.bg_amb_col[0].b / 255.0f);
        color0.b = fVar1 * (4.0f * (fVar2));
    }

    if (color0.r == 0x3e) {
        color0.r = param_0.r;
    }
    if (color0.g == 0x3e) {
        color0.g = param_0.g;
    }
    if (color0.b == 0x3e) {
        color0.b = param_0.b;
    }
    GXSetChanAmbColor(GX_COLOR0A0, color0);
    GXSetChanMatColor(GX_COLOR0A0, g_whiteColor);
}

static void initiate_b_Lighting8(_GXColor& param_0) {
    GXSetChanCtrl(GX_COLOR0, true, GX_SRC_REG, GX_SRC_VTX, 0xfe, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetChanCtrl(GX_ALPHA0, false, GX_SRC_REG, GX_SRC_VTX, 0xfe, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetChanAmbColor(GX_COLOR0A0, param_0);
    GXSetChanMatColor(GX_COLOR0A0, g_whiteColor);
}

static void drawFirst(JPABaseEmitter* param_0) {
    GXSetColorUpdate(false);
    GXSetAlphaCompare(GX_GEQUAL, param_0->getGlobalAlpha(), GX_AOP_OR, GX_GEQUAL, param_0->getGlobalAlpha());
    GXSetZMode(true, GX_LEQUAL, true);
    GXSetZCompLoc(false);
}

static void drawFirst_l8(JPABaseEmitter* param_0) {
    GXSetColorUpdate(false);
}

static void drawSecond_light8(JPABaseEmitter* param_0) {
    s16 sVar1 = (param_0->mGlobalPrmClr.r);
    s16 sVar2 = (sVar1 << 8);
    s16 sVar3 = sVar2 + param_0->mGlobalPrmClr.g;
    dKy_setLight_nowroom_grass(dComIfGp_roomControl_getStayNo(), 0.0f);
    GXSetColorUpdate(1);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(true, GX_LEQUAL, false);
    GXSetZCompLoc(0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetNumChans(1);
    GXColor local_14;
    local_14.r = g_env_light.bg_amb_col[0].r;
    local_14.g = g_env_light.bg_amb_col[0].g;
    local_14.b = g_env_light.bg_amb_col[0].b;
    local_14.a = g_env_light.bg_amb_col[0].a;
    initiateLighting8(local_14, sVar3);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, false, 0x7d);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_4, true, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
}

static void drawSecond_b_light8(JPABaseEmitter* param_0) {
    dScnKy_env_light_c* envLight = dKy_getEnvlight();
    dKy_setLight_nowroom(dComIfGp_roomControl_getStayNo());
    GXSetColorUpdate(true);
    GXSetNumChans(1);
    GXColor local_18;
    if (param_0->getUserWork() == 0) {
        local_18.r = envLight->actor_amb_col.r;
        local_18.g = envLight->actor_amb_col.g;
        local_18.b = envLight->actor_amb_col.b;
        local_18.a = envLight->actor_amb_col.a;
    } else {
        local_18.r = envLight->bg_amb_col[0].r;
        local_18.g = envLight->bg_amb_col[0].g;
        local_18.b = envLight->bg_amb_col[0].b;
        local_18.a = envLight->bg_amb_col[0].a;
    }
    initiate_b_Lighting8(local_18);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_4, true, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
}

static void static_light8EcallBack(JPABaseEmitter* param_0) {
    GXFlush();
    GXLoadPosMtxImm(j3dSys.getViewMtx(), 0);
    GXLoadNrmMtxImm(j3dSys.getViewMtx(), 0);
    GXInvalidateVtxCache();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_CLR_RGB, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    switch (param_0->getDrawCount()) {
    case 1:
        drawFirst(param_0);
        break;
    case 2:
        drawSecond_light8(param_0);
        break;
    }
}

static void static_gen_b_light8EcallBack(JPABaseEmitter* param_0) {
    GXFlush();
    Mtx identity;
    MTXIdentity(identity);
    GXLoadPosMtxImm(identity, 0);
    GXLoadNrmMtxImm(identity, 0);
    GXInvalidateVtxCache();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_CLR_RGB, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    switch (param_0->getDrawCount()) {
    case 1:
        drawFirst_l8(param_0);
        break;
    case 2:
        drawSecond_b_light8(param_0);
        break;
    }
}

static void static_gen_d_light8EcallBack(JPABaseEmitter* param_0) {
    GXFlush();
    GXLoadPosMtxImm(j3dSys.getViewMtx(), 0);
    GXLoadNrmMtxImm(j3dSys.getViewMtx(), 0);
    GXInvalidateVtxCache();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_CLR_RGB, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    switch (param_0->getDrawCount()) {
    case 1:
        drawFirst_l8(param_0);
        break;
    case 2:
        drawSecond_b_light8(param_0);
        break;
    }
}

void dPa_light8EcallBack::draw(JPABaseEmitter* param_0) {
    static_light8EcallBack(param_0);
}

void dPa_gen_b_light8EcallBack::draw(JPABaseEmitter* param_0) {
    static_gen_b_light8EcallBack(param_0);
}

void dPa_gen_d_light8EcallBack::draw(JPABaseEmitter* param_0) {
    static_gen_d_light8EcallBack(param_0);
}

static void dPa_setWindPower(JPABaseParticle* param_0) {
    f32 local_58;
    JGeometry::TVec3<f32> aTStack_18;
    JGeometry::TVec3<f32> aTStack_24;
    JGeometry::TVec3<f32> aTStack_30;
    cXyz cStack_3c;
    param_0->getOffsetPosition(aTStack_24);
    dKyw_get_AllWind_vec((cXyz*)&aTStack_24, &cStack_3c, &local_58);
    cStack_3c.y = 0.65f;
    JGeometry::TVec3<f32> aTStack_48;
    JGeometry::setTVec3f(cStack_3c * (9.0f * local_58), *(Vec*)&aTStack_48);
    aTStack_24 += aTStack_48;
    param_0->setOffsetPosition(aTStack_24);
}

void dPa_modelPcallBack::draw(JPABaseEmitter* param_0, JPABaseParticle* param_1) {
    Mtx local_74;
    Mtx local_44;
    MTXIdentity(local_74);
    MTXIdentity(local_44);
    f32 f31 = -90.0f / 16384.0f * param_1->getRotateAngle();
    if (f31) {
        switch(dPa_modelEcallBack::getRotAxis(param_0)) {
        case 0:
            MTXRotRad(local_44, 0x79, DEG_TO_RAD(f31));
            break;
        case 1:
            MTXRotRad(local_44, 0x78, DEG_TO_RAD(f31));
            break;
        case 2:
            MTXRotRad(local_44, 0x7a, DEG_TO_RAD(f31));
            break;
        case 3:
            Vec vec = {1.0f, 1.0f, 1.0f};
            MTXRotAxisRad(local_44, &vec, DEG_TO_RAD(f31));
            break;
        }

        MTXConcat(local_74, local_44, local_74);
    }
    JGeometry::TVec3<f32> local_cc;
    param_1->getGlobalPosition(local_cc);
    local_74[0][3] = local_cc.x;
    local_74[1][3] = local_cc.y;
    local_74[2][3] = local_cc.z;
    JGeometry::TVec3<f32> local_fc;
    param_0->getGlobalParticleScale(local_fc);
    local_fc.x *= param_1->getParticleScaleX();
    local_fc.y *= param_1->getParticleScaleY();
    Mtx auStack_c0;
    MTXScale(auStack_c0, local_fc.x, local_fc.y, local_fc.x);
    MTXConcat(local_74, auStack_c0, local_74);
    dPa_modelEcallBack::drawModel(param_0, local_74);
    param_1->setInvisibleParticleFlag();
}

bool dPa_modelEcallBack::model_c::set(J3DModelData* param_0, dKy_tevstr_c const& param_1,
                                          u8 rotAxis, void* param_3, u8 param_4, u8 param_5) {
    if (field_0x0 != NULL) {
        return false;
    }

    field_0x0 = param_0;
    field_0x8.mLightObj = param_1.mLightObj;

    // Supposedly dKy_tevstr_c::operator= but it's too big to inline
    J3DLightObj* end;
    J3DLightObj* src;
    J3DLightObj* dst;
    dst = (J3DLightObj*)field_0x8.mLights;
    src = (J3DLightObj*)param_1.mLights;
    end = field_0x8.mLights + 6;
    do {
        *dst = *src;
        dst++;
        src++;
    } while (dst < end);
    field_0x8.field_0x32c = param_1.field_0x32c;
    field_0x8.mLightPosWorld = param_1.mLightPosWorld;
    field_0x8.field_0x344 = param_1.field_0x344;
    typedef struct Arr{
        int field_0x0[2];
    };
    *(Arr*)&field_0x8.AmbCol = *(Arr*)&param_1.AmbCol;
    *(Arr*)&field_0x8.FogCol = *(Arr*)&param_1.FogCol;
    *(Arr*)&field_0x8.TevColor = *(Arr*)&param_1.TevColor;
    *(int*)&field_0x8.TevKColor = *(int*)&param_1.TevKColor;
    *(int*)&field_0x8.mLightInf = *(int*)&param_1.mLightInf;

    field_0x8.mFogStartZ = param_1.mFogStartZ;
    field_0x8.mFogEndZ = param_1.mFogEndZ;
    field_0x8.pat_ratio = param_1.pat_ratio;
    field_0x8.field_0x374 = param_1.field_0x374;
    field_0x8.Material_id = param_1.Material_id;
    field_0x8.Type = param_1.Type;
    field_0x8.mInitTimer = param_1.mInitTimer;
    field_0x8.UseCol = param_1.UseCol;
    field_0x8.PrevCol = param_1.PrevCol;
    field_0x8.wether_pat1 = param_1.wether_pat1;
    field_0x8.wether_pat0 = param_1.wether_pat0;
    field_0x8.room_no = param_1.room_no;
    field_0x8.YukaCol = param_1.YukaCol;
    field_0x8.mLightMode = param_1.mLightMode;
    field_0x8.Material_use_fg = param_1.Material_use_fg;
    field_0x8.field_0x384 = param_1.field_0x384;
    field_0x8.field_0x385 = param_1.field_0x385;
    mRotAxis = rotAxis;
    field_0x391 = param_5;
    field_0x4 = (J3DAnmBase*)param_3;
    field_0x392 = param_4;

    return true;
}

void dPa_modelEcallBack::model_c::setup() {
    if (field_0x4 != NULL) {
        if (field_0x392 == 0) {
            J3DAnmTexPattern* pattern = (J3DAnmTexPattern*)field_0x4;
            pattern->searchUpdateMaterialID(field_0x0);
            pattern->setFrame(field_0x391);
            field_0x0->entryTexNoAnimator(pattern);
        } else {
            J3DAnmTevRegKey* tevRegKey = (J3DAnmTevRegKey*)field_0x4;
            tevRegKey->searchUpdateMaterialID(field_0x0);
            tevRegKey->setFrame(field_0x391);
            field_0x0->entryTevRegAnimator(tevRegKey);
        }
    }

    field_0x0->syncJ3DSys();
    J3DShape::resetVcdVatCache();
}


void dPa_modelEcallBack::model_c::cleanup() {
    if (field_0x4 == NULL) {
        return;
    }

    if (field_0x392 == 0) {
        field_0x0->removeTexNoAnimator((J3DAnmTexPattern*)field_0x4);
    } else {
        field_0x0->removeTevRegAnimator((J3DAnmTevRegKey*)field_0x4);
    }
}

void dPa_modelEcallBack::model_c::draw(f32 (*param_0)[4]) {
    J3DJoint* joint = field_0x0->getJointNodePointer(0);
    for (J3DMaterial* material = joint->getMesh(); material != NULL; material = material->getNext())
    {
        material->loadSharedDL();
        material->getShape()->loadPreDrawSetting();
        if (material->getMaterialAnm() != NULL) {
            material->getMaterialAnm()->calc(material);
        }
        material->calc(param_0);
        u16 texNo = material->getTevBlock()->getTexNo(0);
        field_0x0->getTexture()->loadGX(texNo, GX_TEXMAP0);
        dKy_setLight_again();
        dKy_Global_amb_set(&field_0x8);
        dKy_GxFog_tevstr_set(&field_0x8);
        Mtx auStack_48;
        MTXConcat(j3dSys.getViewMtx(), param_0, auStack_48);
        GXLoadPosMtxImm(auStack_48, 0);
        GXLoadNrmMtxImm(auStack_48, 0);
        material->getShape()->simpleDrawCache();
    }
}

void dPa_modelEcallBack::draw(JPABaseEmitter* param_0) {
    if (param_0->isEnableDeleteEmitter()) {
        param_0->quitImmortalEmitter();
        param_0->setEmitterCallBackPtr(NULL);
        param_0->setParticleCallBackPtr(NULL);
        resetModel(param_0);
    } else {
        setupModel(param_0);
    }
}

void dPa_modelEcallBack::setup(JPABaseEmitter* param_0, cXyz const* param_1,
                                   csXyz const* param_2, s8 param_3) {
    param_0->setEmitterCallBackPtr(NULL);
    if (mModel != NULL) {
        int sVar1 = struct_80450E9C;
        for (int i = 0; i < sVar1; i++) {
            int x = struct_80450E9D / sVar1;
            struct_80450E9D -= (x) * sVar1;
            if (mModel[struct_80450E9D].getModelData() == NULL) {
                param_0->setUserWork(struct_80450E9D++);
                param_0->setParticleCallBackPtr(&mPcallback);
                return;
            }
            struct_80450E9D++;
        }
    }
}

void dPa_modelEcallBack::create(u8 param_0) {
    mModel = new model_c[param_0];
    struct_80450E9C = param_0;
    struct_80450E9D = 0;
}

void dPa_modelEcallBack::remove() {
    if (mModel != NULL) {
        delete [] mModel;
        mModel = NULL;
    }
}

int dPa_modelEcallBack::setModel(JPABaseEmitter* param_0, J3DModelData* param_1,
                                      dKy_tevstr_c const& param_2, u8 param_3, void* param_4,
                                      u8 param_5, u8 param_6) {
    model_c* pModel = getModel(param_0);
    if (pModel != NULL) {
        pModel->set(param_1, param_2, param_3, param_4, param_5, param_6);
        param_0->setEmitterCallBackPtr((JPAEmitterCallBack*)&mEcallback);
        param_0->becomeImmortalEmitter();
        return 1;
    }
    return 0;
}

void dPa_modelEcallBack::resetModel(JPABaseEmitter* param_0) {
    model_c* pModel = getModel(param_0);
    if (pModel) {
        pModel->reset();
    }
}

void dPa_modelEcallBack::setupModel(JPABaseEmitter* param_0) {
    model_c* pModel = getModel(param_0);
    if (pModel) {
        pModel->setup();
    }
}

void dPa_modelEcallBack::drawModel(JPABaseEmitter* param_0, f32 (*param_1)[4]) {
    model_c* pModel = getModel(param_0);
    if (pModel) {
        pModel->draw(param_1);
    }
}

void dPa_modelEcallBack::cleanupModel(JPABaseEmitter* param_0) {
    model_c* pModel = getModel(param_0);
    if (pModel) {
        pModel->cleanup();
    }
    dPa_cleanupGX();
}

dPa_modelEcallBack::model_c* dPa_modelEcallBack::getModel(JPABaseEmitter* param_0) {
    if (mModel != NULL && param_0 != NULL && param_0->getUserWork() != 0xffffffff) {
        return mModel + param_0->getUserWork();
    }
    return NULL;
}

u8 dPa_modelEcallBack::getRotAxis(JPABaseEmitter* param_0) {
    model_c* pModel = getModel(param_0);
    if (pModel) {
        return pModel->getRotAxis();
    }
    return 0;
}

void dPa_selectTexEcallBack::draw(JPABaseEmitter* param_0) {
    param_0->loadTexture(field_0x4, GX_TEXMAP0);
}

dPa_simpleEcallBack::dPa_simpleEcallBack() {
    mEmitter = NULL;
    field_0xc = 0;
}

void dPa_simpleEcallBack::executeAfter(JPABaseEmitter* param_0) {
    int currentCreateNumber = param_0->getCurrentCreateNumber();
    if (currentCreateNumber <= 0) {
        field_0xc = 0;
    } else {
        dPa_simpleData_c* pData = mData;
        param_0->playCreateParticle();
        for (; field_0xc != 0; field_0xc--) {
            Vec local_3c;
            if ((int)mDoLib_clipper::clip(j3dSys.getViewMtx(), *(Vec*)pData, 200.0f) == 0) {
                JGeometry::TVec3<f32> aTStack_30;
                param_0->getLocalTranslation(aTStack_30);
                param_0->setGlobalTranslation(pData->field_0x00.x, pData->field_0x00.y + aTStack_30.y, pData->field_0x00.z);
                param_0->setGlobalPrmColor(pData->field_0x0c.r, pData->field_0x0c.g, pData->field_0x0c.b);
                param_0->setGlobalAlpha(pData->field_0x0c.a);
                param_0->setGlobalEnvColor(pData->field_0x10.r, pData->field_0x10.g, pData->field_0x10.b);
                for (int i = 0; i < currentCreateNumber; i++) {
                    JPABaseParticle* particle = (JPABaseParticle*)param_0->createParticle();
                    if (particle == NULL)
                        break;
                    particle->setOffsetPosition(pData->field_0x00.x, pData->field_0x00.y + aTStack_30.y, pData->field_0x00.z);
                }
            }
            pData++;
        }
    }
    param_0->stopCreateParticle();
}

void dPa_simpleEcallBack::draw(JPABaseEmitter* param_0) {
    /* empty function */
}

JPABaseEmitter* dPa_simpleEcallBack::create(JPAEmitterManager* param_0, u16 id, u8 param_2) {
    mEmitter = NULL;
    field_0xa = param_2;
    mID = id;
    field_0xe = 0x20;
    mData = new dPa_simpleData_c[field_0xe];
    createEmitter(param_0);
    return mEmitter;
}

dPa_simpleData_c::~dPa_simpleData_c() {}

dPa_simpleData_c::dPa_simpleData_c() {
    /* empty function */
}

JPABaseEmitter* dPa_simpleEcallBack::createEmitter(JPAEmitterManager* param_0) {
    JPAResourceManager* this_00 = param_0->getResourceManager(dPa_control_c::getRM_ID(mID));
    u32 uVar1 = this_00->getResUserWork(mID);
    if (mEmitter == NULL) {
        static JGeometry::TVec3<f32> pos(0.0f, 0.0f, 0.0f);

        mEmitter = param_0->createSimpleEmitterID(pos, mID, field_0xa, dPa_control_c::getRM_ID(mID), NULL, NULL);
        if (mEmitter == NULL) {
            return NULL;
        }
        if ((uVar1 & 0x100) != 0) {
            mEmitter->setParticleCallBackPtr(dPa_control_c::getFsenthPcallBack());
        }
        mEmitter->setEmitterCallBackPtr(this);
        mEmitter->becomeContinuousParticle();
        mEmitter->stopCreateParticle();
    }
    return mEmitter;
}

u32 dPa_simpleEcallBack::set(cXyz const* i_pos, dKy_tevstr_c const* param_2, u8 param_3,
                                  _GXColor const& param_4, _GXColor const& param_5, int param_6,
                                  f32 param_7) {
    f32 dVar7 = param_7;
    if (param_2 != NULL && param_2->Material_use_fg != 124) {
        OS_REPORT("\x1b[43;30mSimple Particle tevstr nonset !!\n");
        return 0;
    }

    dPa_simpleData_c* pData = mData;
    if (pData == NULL || field_0xc >= field_0xe) {
        if (pData != NULL) {
            OS_REPORT("\x1b[43;30mSimple Particle Set Over !!\n");
        } else {
            OS_REPORT("\x1b[43;30mSimple Particle Nothing !!\n");
        }
        return 0;
    }
    pData = mData + field_0xc;
    pData->field_0x00 = *i_pos;
    u32 uVar5 = dPa_control_c::getEmitterManager()->getResourceManager(dPa_control_c::getRM_ID(mID))->getResUserWork(mID);
    u32 uVar6 = (uVar5 & 0xef0000) >> 0x10;
    if (uVar6 < 100) {
        dVar7 = uVar6 / 99.0f;
    }
    if (uVar6 < 100) {
        dVar7 = uVar6 / 99.0f;
    }
    if ((uVar5 & 0x20) != 0) {
        GXColor local_5c;
        GXColor local_60;
        GXColor local_64 = {0xff, 0xff, 0xff, 0xff};
        GXColor local_68 = {0xff, 0xff, 0xff, 0xff};
        if (&param_5 != NULL) {
            local_64 = param_5;
        }
        if (&param_4 != NULL) {
            local_68 = param_4;
        }
        dKy_ParticleColor_get_actor((cXyz*)i_pos, (dKy_tevstr_c*)param_2, &local_5c, &local_60,
                                                &local_64, &local_68, dVar7);
        pData->field_0x0c.r = local_60.r;
        pData->field_0x0c.g = local_60.g;
        pData->field_0x0c.b = local_60.b;
        pData->field_0x10.r = local_5c.r;
        pData->field_0x10.g = local_5c.g;
        pData->field_0x10.b = local_5c.b;
    } else {
        if ((uVar5 & 0x40) != 0) {
            GXColor local_6c;
            GXColor local_70;
            GXColor local_74 = {0xff, 0xff, 0xff, 0xff};
            GXColor local_78 = {0xff, 0xff, 0xff, 0xff};
            if (&param_5 != NULL) {
                local_74 = param_5;
            }
            if (&param_4 != NULL) {
                local_78 = param_4;
            }
            dKy_ParticleColor_get_bg((cXyz*)i_pos, (dKy_tevstr_c*)param_2, &local_6c, &local_70, &local_74, &local_78,
                                     dVar7);
            dVar7 = g_env_light.bg_light_influence +
                    (1.0f - g_env_light.bg_light_influence) * dVar7;
            OS_REPORT("\nwork__ratio=[%f]", dVar7);
            local_6c = dKy_light_influence_col(&local_6c, dVar7);
            local_70 = dKy_light_influence_col(&local_70, dVar7);
            pData->field_0x0c.r = local_70.r;
            pData->field_0x0c.g = local_70.g;
            pData->field_0x0c.b = local_70.b;
            pData->field_0x10.r = local_6c.r;
            pData->field_0x10.g = local_6c.g;
            pData->field_0x10.b = local_6c.b;
        } else {
            if (&param_4 != NULL) {
                pData->field_0x0c.r = param_4.r;
                pData->field_0x0c.g = param_4.g;
                pData->field_0x0c.b = param_4.b;
            }
            if (&param_5 != NULL) {
                pData->field_0x10.r = param_5.r;
                pData->field_0x10.g = param_5.g;
                pData->field_0x10.b = param_5.b;
            }
        }
    }
    pData->field_0x0c.a = param_3;
    pData->field_0x10.a = param_6;
    field_0xc++;
    return 1;
}

void dPa_windPcallBack::execute(JPABaseEmitter* param_0, JPABaseParticle* param_1) {
    dPa_setWindPower(param_1);
}

void dPa_control_c::level_c::emitter_c::onActive() {
    mStatus |= 1;

    if (dComIfGp_event_runCheck()) {
        onEventMove();
    } else {
        offEventMove();
    }
}

void dPa_control_c::level_c::emitter_c::entry(u32 id, u16 name, JPABaseEmitter* i_emitter,
                                              dPa_levelEcallBack* p_callback) {
    mId = id;
    mNameId = name;
    onActive();
    mEmitter = i_emitter;
    mCallback = p_callback;
    mPre = NULL;
    mNext = NULL;
    mEmitter->becomeImmortalEmitter();
}

void dPa_control_c::level_c::emitter_c::cleanup() {
    JPAEmitterCallBack* pdVar1 = mEmitter->getEmitterCallBackPtr();
    if (mCallback != NULL) {
        mCallback->cleanup();
    }
    if (pdVar1 != dPa_control_c::getGen_b_Light8EcallBack() &&
         pdVar1 != dPa_control_c::getGen_d_Light8EcallBack() &&
         pdVar1 != dPa_control_c::getLight8EcallBack()) {
        mEmitter->setEmitterCallBackPtr(NULL);
        JPAParticleCallBack* cb = mEmitter->getParticleCallBackPtr();
        if (cb == &JPTracePCB4 || cb == dPa_control_c::getParticleTracePCB()) {
            mEmitter->setParticleCallBackPtr(NULL);
        }
    }
    mEmitter->becomeInvalidEmitter();
    mEmitter->quitImmortalEmitter();
    mId = 0;
}

dPa_control_c::level_c::level_c() {
    field_0x0 = 0;
    mFreeNo = 0;

    for (int i = 0; i < 0x100; i++) {
        mTable[i] = NULL;
    }
}

void dPa_control_c::level_c::execute(dPa_control_c::level_c::emitter_c* i_emitter) {
    if (!i_emitter->isActive()) {
        if (i_emitter->isEventMove() || !dComIfGp_event_runCheck()) {
            i_emitter->getEmitter()->playCalcEmitter();
            cutTable(i_emitter);
            mFreeNo = ((i_emitter - mEmitter) / sizeof(emitter_c));
        } else {
            i_emitter->getEmitter()->stopCalcEmitter();
        }
    } else {
        i_emitter->offActive();
    }
}

void dPa_control_c::level_c::execute() {
    emitter_c* emitter = mEmitter;
    
    for (int i = 0; i < EMITTER_MAX; i++) {
        if (emitter->getId() != 0) {
            execute(emitter);
        }
        emitter++;
    }
}

void dPa_control_c::level_c::cleanup() {
    emitter_c* emitter = mEmitter;
    
    for (int i = 0; i < EMITTER_MAX; i++) {
        if (emitter->getId() != 0) {
            emitter->clearStatus();
            cutTable(emitter);
            mFreeNo = i;
        }
        emitter++;
    }
}

dPa_control_c::level_c::emitter_c* dPa_control_c::level_c::get(u32 id) {
    if (id == 0) {
        return NULL;
    }

    for (emitter_c* emitter = mTable[id & 0xFF]; emitter != NULL; emitter = emitter->getNext()) {
        if (id == emitter->getId()) {
            return emitter;
        }
    }

    return NULL;
}

void dPa_control_c::level_c::forceOnEventMove(u32 id) {
    emitter_c* emitter = get(id);

    if (emitter != NULL) {
        emitter->onEventMove();
    }
}

void dPa_control_c::level_c::allForceOnEventMove() {
    emitter_c* emitter = mEmitter;

    for (int i = 0; i < EMITTER_MAX; i++) {
        if (emitter->getId() != 0) {
            emitter->onEventMove();
        }
        emitter++;
    }
}

JPABaseEmitter* dPa_control_c::level_c::getEmitter(u32 id) {
    emitter_c* emitter = get(id);
    
    if (emitter == NULL) {
        return NULL;
    }

    return emitter->getEmitter();
}

u32 dPa_control_c::level_c::entry(u16 i_name, JPABaseEmitter* i_emitter,
                                       dPa_levelEcallBack* i_callback) {
    for (int i = 0; i < EMITTER_MAX; i++) {
        if (mEmitter[mFreeNo].getId() == 0) {
            u32 uvar1 = field_0x0 + 1;
            field_0x0 = uvar1;

            if (uvar1 == 0) {
                field_0x0++;
            }

            mEmitter[mFreeNo].entry(field_0x0, i_name, i_emitter, i_callback);
            addTable(&mEmitter[mFreeNo]);
            return field_0x0;
        }

        mFreeNo = (mFreeNo + 1) % EMITTER_MAX;
    }

    i_emitter->becomeInvalidEmitter();
    i_emitter->quitImmortalEmitter();
    return 0;
}

void dPa_control_c::level_c::addTable(dPa_control_c::level_c::emitter_c* i_emitter) {
    u32 id = i_emitter->getId() & 0xFF;

    if (mTable[id] == NULL) {
        mTable[id] = i_emitter;
        return;
    }

    mTable[id]->setPre(i_emitter);
    i_emitter->setNext(mTable[id]);
    mTable[id] = i_emitter;
}

void dPa_control_c::level_c::cutTable(dPa_control_c::level_c::emitter_c* i_emitter) {
    emitter_c* pre = i_emitter->getPre();
    emitter_c* next = i_emitter->getNext();

    if (pre == NULL) {
        mTable[i_emitter->getId() & 0xFF] = next;
    } else {
        pre->setNext(next);
    }

    if (next != NULL) {
        next->setPre(pre);
    }

    i_emitter->cleanup();
}

dPa_selectTexEcallBack dPa_control_c::mTsubo[] = {
    dPa_selectTexEcallBack(0),
    dPa_selectTexEcallBack(1),
    dPa_selectTexEcallBack(2),
    dPa_selectTexEcallBack(3),
    dPa_selectTexEcallBack(4),
    dPa_selectTexEcallBack(5),
    dPa_selectTexEcallBack(6),
    dPa_selectTexEcallBack(7),
};

dPa_setColorEcallBack dPa_control_c::mLifeBall[3] = {
    dPa_setColorEcallBack(l_lifeBallColor[0]),
    dPa_setColorEcallBack(l_lifeBallColor[1]),
    dPa_setColorEcallBack(l_lifeBallColor[2]),
};

JPAEmitterManager* dPa_control_c::mEmitterMng;

dPa_wbPcallBack_c dPa_control_c::mWaterBubblePcallBack;

dPa_fsenthPcallBack dPa_control_c::mFsenthPcallBack;

dPa_light8EcallBack dPa_control_c::mLight8EcallBack;

dPa_light8PcallBack dPa_control_c::mLight8PcallBack;

dPa_gen_b_light8EcallBack dPa_control_c::m_b_Light8EcallBack;

dPa_gen_b_light8PcallBack dPa_control_c::m_b_Light8PcallBack;

dPa_gen_d_light8EcallBack dPa_control_c::m_d_Light8EcallBack;

dPa_gen_d_light8PcallBack dPa_control_c::m_d_Light8PcallBack;

u8 dPa_control_c::mStatus;

Mtx dPa_control_c::mWindViewMatrix;

dPa_particleTracePcallBack_c dPa_control_c::mParticleTracePCB;

dPa_control_c::dPa_control_c() {
    m_resHeap = JKRCreateExpHeap(0x96000, mDoExt_getArchiveHeap(), false);
    mHeap = NULL;
    m_sceneHeap = NULL;
    field_0x18 = 0xFF;
    mEmitterMng = NULL;
    field_0x19 = 0;
    mCommonResMng = NULL;
    mSceneResMng = NULL;
    m_sceneRes = NULL;

    MTXIdentity(mWindViewMatrix);
}

u8 dPa_control_c::getRM_ID(u16 param_0) {
    return (param_0 >> 15) & 1;
}

void dPa_control_c::createCommon(void const* param_0) {
    mHeap = mDoExt_createSolidHeapFromSystem(0, 0);
    mCommonResMng = new (mHeap, 0) JPAResourceManager(param_0, mHeap);
    ResTIMG* pRVar3 = mDoGph_gInf_c::getFrameBufferTimg();
    mCommonResMng->swapTexture(pRVar3, "dummy");
    mEmitterMng = new (mHeap, 0) JPAEmitterManager(3000, 250, *(JKRHeap**)this, 0x13, 2);
    mEmitterMng->entryResourceManager(mCommonResMng, 0);
    JKRHeap* prevHeap = mDoExt_setCurrentHeap(mHeap);
    for (u16 i = 0; i < 5; i++) {
        u16 uVar1 = dPa_name::j_o_id[i];
        if (mCommonResMng->checkUserIndexDuplication(uVar1)) {
            u32 local_30 = mCommonResMng->getResUserWork(uVar1);
            u8 local_38 = 0;
            dPa_group_id_change(&local_30, &local_38);
            newSimple(uVar1, local_38, &local_30);
        }
    }
    field_0x1a = field_0x19;
    mDoExt_setCurrentHeap(prevHeap);
    mDoExt_adjustSolidHeap(mHeap);
}

void dPa_control_c::createRoomScene() {
    m_sceneHeap = mDoExt_createSolidHeapFromGame(0, 0);
    mSceneResMng = new (m_sceneHeap, 0) JPAResourceManager(m_sceneRes, m_sceneHeap);
    ResTIMG* pRVar3 = mDoGph_gInf_c::getFrameBufferTimg();
    mSceneResMng->swapTexture(pRVar3, "dummy");
    mEmitterMng->entryResourceManager(mSceneResMng, 1);
    JKRHeap* prevHeap = mDoExt_setCurrentHeap(m_sceneHeap);
    for (u16 i = 0; i < 14; i++) {
        u16 uVar1 = dPa_name::s_o_id[i];
        if (mSceneResMng->checkUserIndexDuplication(uVar1)) {
            u32 local_30 = mSceneResMng->getResUserWork(uVar1);
            u8 local_38 = 0;
            dPa_group_id_change(&local_30, &local_38);
            newSimple(uVar1, local_38, &local_30);
        }
    }
    mDoExt_setCurrentHeap(prevHeap);
    mDoExt_adjustSolidHeap(m_sceneHeap);
}

bool dPa_control_c::readScene(u8 param_0, mDoDvdThd_toMainRam_c** param_1) {
    if (param_0 == 0xff || param_0 == field_0x18) {
        return 0;
    }

    if (m_sceneRes) {
        JKRHeap::free(m_sceneRes, NULL);
        m_sceneRes = NULL;
    }
    field_0x18 = param_0;
    static char jpcName[32];
    sprintf(jpcName, "/res/Particle/Pscene%03d.jpc", param_0);
    *param_1 = mDoDvdThd_toMainRam_c::create(jpcName, 0, m_resHeap);
    return 1;
}

void dPa_control_c::createScene(void const* param_0) {
    for (int i = 0; i < field_0x1a; i++) {
        field_0x1c[i].createEmitter(mEmitterMng);
    }
    if (param_0 != NULL) {
        m_sceneRes = (void*)param_0;
    }
    if (m_sceneRes != NULL) {
        createRoomScene();
    }
    dPa_modelEcallBack::create(0x80);
}

bool dPa_control_c::removeRoomScene(bool param_0) {
    if (m_sceneHeap == NULL) {
        return false;
    }

    mEmitterMng->clearResourceManager(true);
    mDoExt_destroySolidHeap(m_sceneHeap);
    m_sceneHeap = NULL;
    mSceneResMng = NULL;
    field_0x19 = field_0x1a;
    if (param_0) {
        JKRHeap::free(m_sceneRes, NULL);
        m_sceneRes = NULL;
        field_0x18 = 0xff;
    }

    return true;
}

void dPa_control_c::removeScene(bool param_0) {
    field_0x210.cleanup();
    removeRoomScene(param_0);
    for (int i = 0; i < field_0x1a; i++) {
        field_0x1c[i].removeEmitter();
    }

    mEmitterMng->forceDeleteAllEmitter();
    dPa_modelEcallBack::remove();
}

void dPa_control_c::cleanup() {
    field_0x210.cleanup();
}

void dPa_control_c::calc3D() {
    if (isStatus(2)) {
        offStatus(2);
    } else {
        for (u8 i = 0; i <= 13; i++) {
            mEmitterMng->calc(i);
        }
        field_0x210.execute();
    }
}

void dPa_control_c::calc2D() {
    for (u8 i = 14; i <= 16; i++) {
        mEmitterMng->calc(i);
    }
}

void dPa_control_c::calcMenu() {
    if (mEmitterMng != NULL) {
        for (u8 i = 17; i <= 18; i++) {
            mEmitterMng->calc(i);
        }
    }
}

void dPa_control_c::draw(JPADrawInfo* param_0, u8 param_1) {
    if (mEmitterMng != NULL) {
        j3dSys.reinitGX();
        dKy_setLight_again();

        if (param_1 >= 6 && param_1 <= 11) {
            dKy_GxFog_set();
        }

        mEmitterMng->draw(param_0, param_1);
        GXSetAlphaUpdate(GX_FALSE);
        GXSetNumIndStages(0);
        GXSetClipMode(GX_CLIP_ENABLE);
    }
}

void dPa_control_c::setHitMark(u16 param_1, fopAc_ac_c* param_2, cXyz const* param_3,
                               csXyz const* param_4, cXyz const* param_5, u32 param_6) {
    cXyz cStack_34;
    csXyz cStack_78;
    cXyz const* pPos = param_3;
    csXyz const* pAngle = param_4;
    if (param_2 != NULL) {
        if (fopAcM_CheckStatus(param_2, 0x40000000)) {
            return;
        }
        fopAcM_OnStatus(param_2, 0x40000000);
        if (fopAcM_GetJntCol(param_2) != NULL) {
            dJntCol_c* jntCol = fopAcM_GetJntCol(param_2);
            if (jntCol->getHitmarkPosAndAngle(pPos, pAngle, &cStack_34, &cStack_78,
                                              param_1 == 6) >= 0)
            {
                pPos = &cStack_34;
                pAngle = &cStack_78;
            }
        }
    }
    if (param_1 == 3) {
        dComIfGp_particle_set(0x114, pPos, pAngle, param_5);
        dComIfGp_particle_set(0x115, pPos, 0, param_5);
        dComIfGp_particle_set(0x116, pPos, 0, param_5);
        if ((param_6 & 2) != 0 && param_2 != NULL && fopAcM_GetGroup(param_2) == 2) {
            dComIfGp_particle_set(0x2ed, pPos, 0, param_5);
        }
        dComIfGp_particle_set(0x2ee, pPos, 0, param_5);
        dKy_SordFlush_set(*pPos, 1);
    } else if (param_1 == 2 || param_1 == 5 || param_1 == 9 || param_1 == 8 || param_1 == 6) {
        dComIfGp_particle_set(0x117, pPos, pAngle, param_5);
        dComIfGp_particle_set(0x118, pPos, 0, param_5);
        if (param_1 != 9) {
            dKy_SordFlush_set(*pPos, 0);
        }
    } else if (param_1 == 7) {
        cXyz cStack_40;
        if (param_5 != NULL) {
            cStack_40 = *param_5 * 0.5f;
        } else {
            cStack_40.set(0.5f, 0.5f, 0.5f);
        }
        dComIfGp_particle_set(0x119, pPos, pAngle, &cStack_40);
        dComIfGp_particle_set(0x11a, pPos, 0, &cStack_40);
        dComIfGp_particle_set(0x2ef, pPos, 0, &cStack_40);
        if ((param_6 & 2) != 0 && param_2 != NULL && fopAcM_GetGroup(param_2) == 2) {
            dComIfGp_particle_set(0x2f0, pPos, 0, &cStack_40);
        }
    } else if (param_1 == 1 || param_1 == 4) {
        dComIfGp_particle_set(0x119, pPos, pAngle, param_5);
        dComIfGp_particle_set(0x11a, pPos, 0, param_5);
        dComIfGp_particle_set(0x2ef, pPos, 0, param_5);
        if ((param_6 & 2) != 0 && param_2 != NULL && fopAcM_GetGroup(param_2) == 2) {
            dComIfGp_particle_set(0x2f0, pPos, 0, param_5);
        }
        dKy_SordFlush_set(*pPos, 1);
    }
}

void dPa_control_c::setWaterRipple(u32* param_0, cBgS_PolyInfo& param_1, cXyz const* param_2,
                                       f32 param_3, dKy_tevstr_c const* param_4,
                                       cXyz const* param_5, s8 param_6) {
    f32 rate = 0.05f + 0.15f * (0.1f * param_3);
    if (rate > 1.0f) {
        rate = 1.0f;
    }
    csXyz local_50;
    cM3dGPla acStack_48;
    if (dComIfG_Bgsp().GetTriPla(param_1, &acStack_48)) {
        f32 uVar5 = acStack_48.mNormal.absXZ();
        local_50.x = cM_atan2s(uVar5, acStack_48.mNormal.y);
        local_50.y = acStack_48.mNormal.atan2sX_Z();
        local_50.z = 0;
    }

    static u16 const particleID[2] = {0x01B3, 0x01B2};
    for (int i = 0; i < 2; i++, param_0++) {
        *param_0 = dComIfGp_particle_setPolyColor(
            *param_0, particleID[i], param_1, param_2, param_4,
            &local_50, param_5, 0, 0, param_6, 0);
        JPABaseEmitter* emitter = getEmitter(*param_0);
        if (emitter != NULL) {
            emitter->setRate(rate);
        }
    }
}

/* 8004CA90-8004CF90 0473D0 0500+00 3/3 20/20 418/418 .text
 * set__13dPa_control_cFUcUsPC4cXyzPC12dKy_tevstr_cPC5csXyzPC4cXyzUcP18dPa_levelEcallBackScPC8_GXColorPC8_GXColorPC4cXyzf
 */
JPABaseEmitter* dPa_control_c::set(u8 param_0, u16 param_1, cXyz const* i_pos,
                                   dKy_tevstr_c const* param_3, csXyz const* i_rotation,
                                   cXyz const* i_scale, u8 i_alpha, dPa_levelEcallBack* param_7,
                                   s8 param_8, _GXColor const* param_9, _GXColor const* param_10,
                                   cXyz const* param_11, f32 param_12) {
    u8 local_e0 = getRM_ID(param_1);
    JPAResourceManager* local_a8 = mEmitterMng->getResourceManager(local_e0);
    if (local_a8 == NULL) {
        return NULL;
    }

    u32 local_ac = local_a8->getResUserWork(param_1);
    if (param_3 != NULL && param_3->Material_use_fg != 0x7c) {
        return NULL;
    }

    dPa_group_id_change(&local_ac, &param_0);
    JGeometry::TVec3<f32> aTStack_78;
    aTStack_78.set(i_pos->x, i_pos->y, i_pos->z);
    JPABaseEmitter* this_00 = mEmitterMng->createSimpleEmitterID(
        aTStack_78, param_1, param_0,
        local_e0, NULL, NULL);

    if (this_00 == NULL) {
        return NULL;
    }

    if (i_rotation != NULL) {
        JGeometry::TVec3<s16> aTStack_a4;
        aTStack_a4.x = i_rotation->x;
        aTStack_a4.y = i_rotation->y;
        aTStack_a4.z = i_rotation->z;
        this_00->setGlobalRotation(aTStack_a4);
    }
    if (i_scale != NULL) {
        JGeometry::TVec3<f32> aTStack_84;
        aTStack_84.x = i_scale->x;
        aTStack_84.y = i_scale->y;
        aTStack_84.z = i_scale->z;
        this_00->setGlobalScale(aTStack_84);
    }

    this_00->setGlobalAlpha(i_alpha);
    if (param_7 != NULL) {
        this_00->setEmitterCallBackPtr(param_7);
        param_7->setup(this_00, i_pos, i_rotation, param_8);
    }
    if ((local_ac & 0x100) != 0) {
        this_00->setParticleCallBackPtr(getFsenthPcallBack());
    }
    if ((local_ac & 0x800) != 0) {
        this_00->setEmitterCallBackPtr(getGen_d_Light8EcallBack());
        if ((local_ac & 0x20) != 0) {
            getGen_d_Light8EcallBack()->setup(this_00, i_pos, i_rotation, 0);
        } else {
            getGen_d_Light8EcallBack()->setup(this_00, i_pos, i_rotation, 1);
        }
    } else {
        if ((local_ac & 0x400) != 0) {
            this_00->setEmitterCallBackPtr(getGen_b_Light8EcallBack());
            if ((local_ac & 0x20) != 0) {
                getGen_b_Light8EcallBack()->setup(this_00, i_pos, i_rotation, 0);
            } else {
                getGen_b_Light8EcallBack()->setup(this_00, i_pos, i_rotation, 1);
            }
        } else {
            if ((local_ac & 0xef0000) >> 0x10 < 100) {
                param_12 = ((local_ac & 0xef0000) >> 0x10) / 99.0f;
            }
            if ((local_ac & 0x20) != 0) {
                GXColor local_b0;
                GXColor local_b4;
                GXColor local_b8 = {0xff, 0xff, 0xff, 0xff};
                GXColor local_bc = {0xff, 0xff, 0xff, 0xff};
                if (param_10 != NULL) {
                    local_b8 = *param_10;
                }
                if (param_9 != NULL) {
                    local_bc = *param_9;
                }
                dKy_ParticleColor_get_actor((cXyz*)i_pos, (dKy_tevstr_c*)param_3, &local_b0,
                                            &local_b4, &local_b8, &local_bc, param_12);
                this_00->setGlobalEnvColor(local_b0.r, local_b0.g, local_b0.b);
                this_00->setGlobalPrmColor(local_b4.r, local_b4.g, local_b4.b);
            } else {
                if ((local_ac & 0x40) != 0) {
                    GXColor local_c0;
                    GXColor local_c4;
                    GXColor local_c8 = {0xff, 0xff, 0xff, 0xff};
                    GXColor local_cc = {0xff, 0xff, 0xff, 0xff};
                    if (param_10 != NULL) {
                        local_c8 = *param_10;
                    }
                    if (param_9 != NULL) {
                        local_cc = *param_9;
                    }
                    dKy_ParticleColor_get_bg((cXyz*)i_pos, (dKy_tevstr_c*)param_3, &local_c0,
                                            &local_c4, &local_c8, &local_cc, param_12);
                    param_12 = g_env_light.bg_light_influence + (1.0f - g_env_light.bg_light_influence) * param_12;
                    local_c0 = dKy_light_influence_col(&local_c0, param_12);
                    local_c4 = dKy_light_influence_col(&local_c4, param_12);
                    this_00->setGlobalEnvColor(local_c0.r, local_c0.g, local_c0.b);
                    this_00->setGlobalPrmColor(local_c4.r, local_c4.g, local_c4.b);
                } else {
                    if (param_9 != NULL) {
                        this_00->setGlobalPrmColor(param_9->r, param_9->g, param_9->b);
                    }
                    if (param_10 != NULL) {
                        this_00->setGlobalEnvColor(param_10->r, param_10->g, param_10->b);
                    }
                }
            }
        }
    }
    if (param_11 != NULL) {
        JGeometry::TVec3<f32> aTStack_9c;
        aTStack_9c.set(param_11->x, param_11->y, param_11->z);
        this_00->setGlobalParticleScale(aTStack_9c);
    }
    return this_00;
}

s32 dPa_control_c::getPolyColor(cBgS_PolyInfo& param_0, int param_1, _GXColor* param_2,
                                     _GXColor* param_3, u8* param_4, f32* param_5) {
    if (!dComIfG_Bgsp().ChkPolySafe(param_0)) {
        return 0;
    }

    if (param_1 == NULL) {
        dKy_pol_eff_prim_get(&param_0, param_2);
        dKy_pol_eff_env_get(&param_0, param_3);
        *param_4 = dKy_pol_eff_alpha_get(&param_0);
        *param_5 = dKy_pol_eff_ratio_get(&param_0);
    } else {
        dKy_pol_eff2_prim_get(&param_0, param_2);
        dKy_pol_eff2_env_get(&param_0, param_3);
        *param_4 = dKy_pol_eff2_alpha_get(&param_0);
        *param_5 = dKy_pol_eff2_ratio_get(&param_0);
    }

    return 1;
}

/* 8004D068-8004D128 0479A8 00C0+00 0/0 1/1 17/17 .text
 * setPoly__13dPa_control_cFUsR13cBgS_PolyInfoPC4cXyzPC12dKy_tevstr_cPC5csXyzPC4cXyziP18dPa_levelEcallBackScPC4cXyz
 */
JPABaseEmitter* dPa_control_c::setPoly(u16 param_0, cBgS_PolyInfo& param_1, cXyz const* param_2,
                                dKy_tevstr_c const* param_3, csXyz const* param_4,
                                cXyz const* param_5, int param_6, dPa_levelEcallBack* param_7,
                                s8 param_8, cXyz const* param_9) {
    u8 local_18;
    float local_c;
    GXColor a_Stack_10;
    GXColor a_Stack_14;

    if (getPolyColor(param_1, param_6, &a_Stack_10,
                         &a_Stack_14, &local_18, &local_c)) {
        return setNormal(param_0, param_2, param_3, param_4, param_5, local_18,
                          param_7, param_8, &a_Stack_10, &a_Stack_14, param_9, local_c);
    }
    return NULL;
}

bool dPa_control_c::newSimple(u16 param_0, u8 param_1, u32* param_2) {
    if (field_0x19 >= 0x19) {
        OSReport("\x1B[43;30m１エミッター登録数オーバー！！\n");
        return false;
    }
    if (field_0x1c[field_0x19].create(mEmitterMng, param_0, param_1) == 0) {
        return false;;
    }
    field_0x19++;
    return true;
}

u32 dPa_control_c::setSimple(u16 param_0, cXyz const* i_pos, dKy_tevstr_c const* param_2,
                                  u8 param_3, _GXColor const& param_4, _GXColor const& param_5,
                                  int param_6, f32 param_7) {
    dPa_simpleEcallBack* cb = getSimple(param_0);
    if (cb == NULL) {
        return 0;
    }

    return cb->set(i_pos, param_2, param_3, param_4, param_5, param_6, param_7);
}

dPa_simpleEcallBack* dPa_control_c::getSimple(u16 param_0) {
    dPa_simpleEcallBack* arr = field_0x1c;
    for (int i = 0; i < field_0x19; i++, arr++) {
        if (param_0 == arr->getID()) {
            return arr;
        }
    }
    return NULL;
}

static void dPa_kankyocolor_set(f32 param_0, JPABaseEmitter* param_1,
                                    dKy_tevstr_c const* param_2, u32 param_3, cXyz const* param_4,
                                    _GXColor const* param_5, _GXColor const* param_6) {
    f32 fVar1 = param_0;
    if ((param_3 & 0xef0000) >> 0x10 < 100) {
        fVar1 = ((param_3 & 0xef0000) >> 0x10) / 99.0f;
    }

    if ((param_3 & 0x20) != 0) {
        GXColor local_60;
        GXColor local_64;
        GXColor local_68 = {0xff, 0xff, 0xff, 0xff};
        GXColor local_6c = {0xff, 0xff, 0xff, 0xff};
        if (param_6 != NULL) {
            local_68 = *param_6;
        }
        if (param_5 != NULL) {
            local_6c = *param_5;
        }
        dKy_ParticleColor_get_actor((cXyz*)param_4, (dKy_tevstr_c*)param_2, &local_60, &local_64,
                                              &local_68, &local_6c, fVar1);
        param_1->setGlobalEnvColor(local_60.r, local_60.g, local_60.b);
        param_1->setGlobalPrmColor(local_64.r, local_64.g, local_64.b);
    } else if ((param_3 & 0x40) != 0) {
        GXColor local_70;
        GXColor local_74;
        GXColor local_78 = {0xff, 0xff, 0xff, 0xff};
        GXColor local_7c = {0xff, 0xff, 0xff, 0xff};
        if (param_6 != NULL) {
            local_78 = *param_6;
        }
        if (param_5 != NULL) {
            local_7c = *param_5;
        }
        dKy_ParticleColor_get_bg((cXyz*)param_4, (dKy_tevstr_c*)param_2, &local_70, &local_74,
                                &local_78, &local_7c, fVar1);
        fVar1 = g_env_light.bg_light_influence + (1.0f - g_env_light.bg_light_influence) * fVar1;
        local_70 = dKy_light_influence_col(&local_70, fVar1);
        //_GXColor::operator_ = (&local_70, (_GXColor*)&local_80);
        local_74 = dKy_light_influence_col(&local_74, fVar1);
        //_GXColor::operator_ = (&local_74, (_GXColor*)&local_84);
        param_1->setGlobalEnvColor(local_70.r, local_70.g, local_70.b);
        param_1->setGlobalPrmColor(local_74.r, local_74.g, local_74.b);
    }
}

/* 8004D4CC-8004D6A4 047E0C 01D8+00 3/3 11/11 364/364 .text
 * set__13dPa_control_cFUlUcUsPC4cXyzPC12dKy_tevstr_cPC5csXyzPC4cXyzUcP18dPa_levelEcallBackScPC8_GXColorPC8_GXColorPC4cXyzf
 */
u32 dPa_control_c::set(u32 param_0, u8 param_1, u16 param_2, cXyz const* pos,
                       dKy_tevstr_c const* param_4, csXyz const* i_rotation, cXyz const* i_scale,
                       u8 alpha, dPa_levelEcallBack* param_8, s8 param_9, _GXColor const* param_10,
                       _GXColor const* param_11, cXyz const* param_12, f32 param_13) {
    level_c::emitter_c* this_00 = field_0x210.get(param_0);
    u8 uVar7 = getRM_ID(param_2);
    JPAResourceManager* this_01 = mEmitterMng->getResourceManager(uVar7);
    u32 uVar3 = this_01->getResUserWork(param_2);
    if (this_00 != NULL) {
        if (param_2 == this_00->getNameId()) {
            this_00->onActive();
            JPABaseEmitter* pJVar4 = this_00->getEmitter();
            pJVar4->playCalcEmitter();
            if (this_00->getCallback() == 0) {
                pJVar4->setGlobalTranslation(pos->x, pos->y, pos->z);
                if (i_rotation != NULL) {
                    JGeometry::TVec3<s16> aTStack_64;
                    aTStack_64.x = i_rotation->x;
                    aTStack_64.y = i_rotation->y;
                    aTStack_64.z = i_rotation->z;
                    pJVar4->setGlobalRotation(aTStack_64);
                }
            }
            if (pJVar4->checkStatus(JPAEmtrStts_StopEmit)) {
                pJVar4->playCreateParticle();
            }
            dPa_kankyocolor_set(param_13, pJVar4, param_4, uVar3, pos, param_10, param_11);
            return this_00->getId();
        }
        field_0x210.cutTable(this_00);
    }
    if (param_8 != NULL) {
        param_8->cleanup();
    }
    JPABaseEmitter* pJVar5 = set(param_1, param_2, pos, param_4, i_rotation, i_scale, alpha,
                             param_8, param_9, param_10, param_11, param_12, param_13);
    if (pJVar5 == NULL) {
        return 0;
    } 
    
    return field_0x210.entry(param_2, pJVar5, param_8);
}

/* 8004D6A4-8004D770 047FE4 00CC+00 1/1 3/3 10/10 .text
 * setPoly__13dPa_control_cFUlUsR13cBgS_PolyInfoPC4cXyzPC12dKy_tevstr_cPC5csXyzPC4cXyziP18dPa_levelEcallBackScPC4cXyz
 */
u32 dPa_control_c::setPoly(u32 param_0, u16 param_1, cBgS_PolyInfo& param_2,
                               cXyz const* param_3, dKy_tevstr_c const* param_4,
                               csXyz const* param_5, cXyz const* param_6, int param_7,
                               dPa_levelEcallBack* param_8, s8 param_9, cXyz const* param_10) {
    f32 local_8;
    GXColor local_10;
    GXColor local_c;
    u8 local_4;
    if (getPolyColor(param_2, param_7, &local_10,
                         &local_c, &local_4, (f32*)&local_8)) {
        return setNormal(param_0, param_1, param_3, param_4, param_5, param_6,
                          local_4, param_8, param_9, &local_10, &local_c, param_10,
                          local_8);
    }

    return 0;
}

u32 dPa_control_c::setStopContinue(u32 param_0) {
    level_c::emitter_c* emitter = field_0x210.get(param_0);
    if (emitter != NULL) {
        emitter->onActive();
        emitter->getEmitter()->stopCreateParticle();
        return emitter->getId();
    }
    return 0;
}

/* 8004D7C4-8004D988 048104 01C4+00 0/0 1/1 0/0 .text
 * setSimpleFoot__13dPa_control_cFUlPUlR13cBgS_PolyInfoPC4cXyzPC12dKy_tevstr_ciPC5csXyzPC4cXyzP18dPa_levelEcallBackScPC4cXyz
 */
u32 dPa_control_c::setSimpleFoot(u32 param_0, u32* param_1, cBgS_PolyInfo& param_2,
                                      cXyz const* param_3, dKy_tevstr_c const* param_4, int param_5,
                                      csXyz const* param_6, cXyz const* param_7,
                                      dPa_levelEcallBack* param_8, s8 param_9,
                                      cXyz const* param_10) {
    f32 local_44;
    u8 local_50;
    GXColor a_Stack_48;
    GXColor a_Stack_4c;
    if (getPolyColor(param_2, 0, &a_Stack_48, &a_Stack_4c, &local_50,
                     &local_44) == 0)
    {
        return 0;
    }

    u8 effType = dKy_pol_efftype_get(&param_2);
    if (effType >= 6) {
        return 0;
    }
    static u16 effectIDTable[6][2] = {
        {0xE6, 0xE7}, {0x143, 0x146}, {0x1AF, 0x1B8},
        {0x881B, 0x881E}, {0x881C, 0x881F}, {0x86AE, 0x86AF},
    };
    u32 rv = setNormal(param_0, effectIDTable[effType][param_5],
        param_3, param_4, param_6, param_7, local_50, param_8, param_9, &a_Stack_48,
        &a_Stack_4c, param_10, local_44);
    u8 effType2 = dKy_pol_efftype2_get(&param_2);
    int polyAtt = dComIfG_Bgsp().GetPolyAtt0(param_2);
    if (effType2 < 6 && (polyAtt != 1 || polyAtt != 3)) {
        getPolyColor(param_2, 1, &a_Stack_48, &a_Stack_4c,
                        &local_50, &local_44);
        *param_1 = setNormal(*param_1, effectIDTable[effType2][param_5],
            param_3, param_4, param_6, param_7, local_50, param_8, param_9, &a_Stack_48,
            &a_Stack_4c, param_10, local_44);
    }
    return rv;
}

/* 8004D988-8004DC28 0482C8 02A0+00 0/0 1/1 0/0 .text
 * setCommonPoly__13dPa_control_cFPUlP13cBgS_PolyInfoPC4cXyzPC4cXyzPC12dKy_tevstr_cUlUlPC5csXyzPC4cXyzSc
 */
u16 dPa_control_c::setCommonPoly(u32* param_0, cBgS_PolyInfo* param_1, cXyz const* param_2,
                                      cXyz const* param_3, dKy_tevstr_c const* param_4, u32 param_5,
                                      u32 param_6, csXyz const* param_7, cXyz const* param_8,
                                      s8 param_9) {
    f32 local_30;
    GXColor a_Stack_34;
    GXColor a_Stack_38;
    int j;
    int i;
    u8 bVar3;
    u16* puVar7;
    const cXyz* local_40;
    u32* ptr;
    u16 uVar5 = 0;
    u8 local_4c;
    if (dComIfG_Bgsp().ChkPolySafe(*param_1) == 0) {
        return 0;
    }

    int local_44 = dComIfG_Bgsp().GetPolyAtt0(*param_1);
    for (i = 0; i < 2; i++) {
        if (getPolyColor(*param_1, i, &a_Stack_34, &a_Stack_38, &local_4c, &local_30) == 0) {
            return 0xffff;
        }
        if ((param_6 & 0x80000000) != 0) {
            if (i == 1 && dKy_camera_water_in_status_check() && (local_44 == 1 || local_44 == 3)) {
                bVar3 = 5;
            } else {
                bVar3 = 6;
            }
        } else {
            if (i == 0) {
                bVar3 = dKy_pol_efftype_get(param_1);
            } else if (local_44 == 1 || local_44 == 3) {
                bVar3 = 6;
            } else {
                bVar3 = dKy_pol_efftype2_get(param_1);
            }
        }

        if (bVar3 >= 6 || (param_6 & (1 << bVar3)) != 0 ||
            (bVar3 == 2 && (param_6 & 0x10000) != 0 && i == 1))
        {
            uVar5 |= (0xff << (i << 3));
        } else {
            static u16 ringID[6][4] = {
                {0x00E7, 0xFFFF, 0xFFFF, 0xFFFF}, {0x0146, 0xFFFF, 0xFFFF, 0xFFFF},
                {0x01AF, 0x01B0, 0x01B1, 0xFFFF}, {0x881E, 0xFFFF, 0xFFFF, 0xFFFF},
                {0x881F, 0x8820, 0xFFFF, 0xFFFF}, {0x86AF, 0xFFFF, 0xFFFF, 0xFFFF},
            };
            static u16 normalID[6][4] = {
                {0x00E6, 0xFFFF, 0xFFFF, 0xFFFF}, {0x0143, 0xFFFF, 0xFFFF, 0xFFFF},
                {0x01B8, 0x01B9, 0x01BA, 0x01BB}, {0x881B, 0xFFFF, 0xFFFF, 0xFFFF},
                {0x881C, 0x881D, 0xFFFF, 0xFFFF}, {0x86AE, 0xFFFF, 0xFFFF, 0xFFFF},
            };
            uVar5 |= bVar3 << (i << 3);
            if (bVar3 == 2 && ((param_6 & 0x10000) != 0)) {
                puVar7 = ringID[bVar3];
                local_40 = param_2;
            } else {
                if ((param_5 & (1 << bVar3)) != 0) {
                    puVar7 = ringID[bVar3];
                    local_40 = param_3;
                } else {
                    puVar7 = normalID[bVar3];
                    local_40 = param_3;
                }
            }
            ptr = param_0 + 4*i;
            for ( j = 0; j < 4; j++, ptr++, puVar7++) {
                if (puVar7[0] == 0xffff) {
                    break;
                }
                ptr[0] = setNormal(ptr[0], puVar7[0], local_40, param_4, param_7, param_8,
                                  local_4c, NULL, param_9, &a_Stack_34, &a_Stack_38, NULL, local_30);
                dComIfGp_particle_levelEmitterOnEventMove(ptr[0]);
            }
        }
    }

    return uVar5;
}

void dPa_wbPcallBack_c::execute(JPABaseEmitter* param_0, JPABaseParticle* param_1) {
    JGeometry::TVec3<f32> local_18;
    param_1->getGlobalPosition(local_18);
    cXyz cStack_24(local_18.x, local_18.y, local_18.z);
    if (fopAcM_wt_c::waterCheck(&cStack_24) && cStack_24.y > fopAcM_wt_c::getWaterY()) {
        param_1->setInvisibleParticleFlag();
        param_1->setDeleteParticleFlag();
    }
}

void dPa_fsenthPcallBack::execute(JPABaseEmitter* param_0, JPABaseParticle* param_1) {
    if (g_env_light.now_senses_effect == 1 && g_env_light.senses_effect_strength > 0.0f) {
        param_0->setGlobalAlpha(255.0f * g_env_light.senses_effect_strength);
        param_0->playDrawParticle();
    } else {
        param_0->setGlobalAlpha(0);
        param_0->stopDrawParticle();
    }
}

void dPa_fsenthPcallBack::draw(JPABaseEmitter* param_0, JPABaseParticle* param_1) {
    /* empty function */
}

void dPa_light8PcallBack::execute(JPABaseEmitter* param_0, JPABaseParticle* param_1) {
    /* empty function */
}

void dPa_gen_b_light8PcallBack::execute(JPABaseEmitter* param_0, JPABaseParticle* param_1) {
    /* empty function */
}

void dPa_gen_d_light8PcallBack::execute(JPABaseEmitter* param_0, JPABaseParticle* param_1) {
    /* empty function */
}

void dPa_light8PcallBack::draw(JPABaseEmitter* param_1, JPABaseParticle* param_2) {
    Mtx local_60;
    Mtx auStack_90;
    Mtx auStack_c0;
    Mtx auStack_f0;
    JGeometry::TVec3<f32> local_100;
    JGeometry::TVec3<f32> local_10c;
    JGeometry::TVec3<f32> local_118;
    JGeometry::TVec3<f32> local_124;
    JGeometry::TVec3<f32> local_130;
    u8 globalAlpha = param_1->getGlobalAlpha() & 0xff;
    JGeometry::TVec3<f32> local_13c;
    JGeometry::TVec3<f32> local_148;
    JGeometry::TVec3<f32> local_154;
    JGeometry::TVec3<f32> local_160;
    JGeometry::TVec3<f32> local_16c;
    dPa_setWindPower(param_2);
    MTXIdentity(local_60);
    MTXIdentity(auStack_90);
    param_2->getBaseAxis(local_10c);
    param_2->getLocalPosition(local_118);
    if (local_118.isZero()) {
        local_118.set(0.0f, 1.0f, 0.0f);
    } else {
        local_118.normalize();
    }
    local_124.cross(local_10c, local_118);
    if (local_124.isZero()) {
        local_124.set(0.0f, 0.0f, 1.0f);
    } else {
        local_124.normalize();
    }
    local_10c.cross(local_118, local_124);
    local_10c.normalize();
    local_60[0][0] = local_10c.x;
    local_60[0][1] = local_118.x;
    local_60[0][2] = local_124.x;
    local_60[1][0] = local_10c.y;
    local_60[1][1] = local_118.y;
    local_60[1][2] = local_124.y;
    local_60[2][0] = local_10c.z;
    local_60[2][1] = local_118.z;
    local_60[2][2] = local_124.z;
    f32 fVar3 = (-90.0f / 16384.0f) * param_2->getRotateAngle();
    if (fVar3) {
        Vec local_178 = {1.0f, 1.0f, 1.0f};
        MTXRotAxisRad(auStack_90, &local_178, (M_PI / 180.0f) * fVar3);
        MTXConcat(local_60, auStack_90, local_60);
    }
    param_2->getGlobalPosition(local_100);
    local_60[0][3] = local_100.x;
    local_60[1][3] = local_100.y;
    local_60[2][3] = local_100.z;
    param_1->getGlobalParticleScale(local_130);
    local_130.x *= param_2->getParticleScaleX();
    local_130.y *= param_2->getParticleScaleY();
    local_130.x *= param_2->getWidth(param_1);
    local_130.y *= param_2->getHeight(param_1);
    local_130.x *= 10.0f;
    local_130.y *= 10.0f;
    local_130.z = 0.0f;
    MTXScale(auStack_f0, local_130.x, local_130.y, local_130.z);
    MTXConcat(local_60, auStack_f0, local_60);
    MTXConcat(j3dSys.getViewMtx(), local_60, auStack_c0);
    GXLoadPosMtxImm(auStack_c0, 0);
    JGeometry::TVec3<f32> local_184;
    JGeometry::TVec3<f32> local_190;
    JGeometry::TVec3<f32> local_19c;
    JGeometry::TVec3<f32> local_1a8;
    local_184.set(1.0f, 2.0f, 0.0f);
    local_190.set(1.0f, 0.0f, 0.0f);
    local_19c.set(-1.0f, 0.0f, 0.0f);
    local_1a8.set(-1.0f, 2.0f, 0.0f);
    local_13c.x = 0.0f;
    local_13c.y = 1.0f;
    local_13c.z = 0.0f;
    local_148.x = 0.0f;
    local_148.y = 1.0f;
    local_148.z = 0.0f;
    local_154.x = 0.0f;
    local_154.y = 1.0f;
    local_154.z = 0.0f;
    local_160.x = 0.0f;
    local_160.y = 1.0f;
    local_160.z = 0.0f;
    local_16c.x = 0.0f;
    local_16c.y = 1.0f;
    local_16c.z = 0.0f;
    local_13c.normalize();
    local_148.normalize();
    local_154.normalize();
    local_160.normalize();
    local_16c.normalize();
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 6);
    GXPosition3f32(0.0f, 1.0f, 0.0f);
    GXNormal3f32(local_13c.x, local_13c.y, local_13c.z);
    GXColor4u8(0xff, 0xff, 0xff, globalAlpha);
    GXTexCoord2f32(0.5f, 0.5f);
    GXPosition3f32(local_184.x, local_184.y, local_184.z);
    GXNormal3f32(local_148.x, local_148.y, local_148.z);
    GXColor4u8(0xff, 0xff, 0xff, globalAlpha);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(local_190.x, local_190.y, local_190.z);
    GXNormal3f32(local_154.x, local_154.y, local_154.z);
    GXColor4u8(0xff, 0xff, 0xff, globalAlpha);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(local_19c.x, local_19c.y, local_19c.z);
    GXNormal3f32(local_160.x, local_160.y, local_160.z);
    GXColor4u8(0xff, 0xff, 0xff, globalAlpha);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(local_1a8.x, local_1a8.y, local_1a8.z);
    GXNormal3f32(local_16c.x, local_16c.y, local_16c.z);
    GXColor4u8(0xff, 0xff, 0xff, globalAlpha);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(local_184.x, local_184.y, local_184.z);
    GXNormal3f32(local_148.x, local_148.y, local_148.z);
    GXColor4u8(0xff, 0xff, 0xff, globalAlpha);
    GXTexCoord2f32(1.0f, 0.0f);
    GXEnd();
    param_2->setInvisibleParticleFlag();
}

void dPa_gen_b_light8PcallBack::draw(JPABaseEmitter* param_1, JPABaseParticle* param_2) {
    Mtx local_80;
    JGeometry::TVec3<f32> local_8c;
    JGeometry::TVec3<f32> aTStack_98;
    JGeometry::TVec3<f32> aTStack_a4;
    JGeometry::TVec3<f32> aTStack_b0;
    JGeometry::TVec3<f32> local_bc;
    u8 uVar7 = param_1->getGlobalAlpha();
    JGeometry::TVec3<f32> local_c8;
    JGeometry::TVec3<f32> local_d4;
    JGeometry::TVec3<f32> local_e0;
    JGeometry::TVec3<f32> local_ec;
    JGeometry::TVec3<f32> local_f8;
    param_2->getGlobalPosition(local_8c);
    MTXMultVec(j3dSys.getViewMtx(), &local_8c, &local_8c);
    f32 dVar9 = JMASSin(param_2->getRotateAngle());
    f32 dVar10 = JMASCos(param_2->getRotateAngle());
    param_1->getGlobalParticleScale(local_bc);
    f32 var_f29 = local_bc.x * param_2->getWidth(param_1);
    f32 var_f28 = local_bc.y * param_2->getHeight(param_1);
    local_80[0][0] = dVar10 * var_f29;
    local_80[0][1] = -dVar9 * var_f28;
    local_80[0][3] = local_8c.x;
    local_80[1][0] = dVar9 * var_f29;
    local_80[1][1] = dVar10 * var_f28;
    local_80[1][3] = local_8c.y;
    local_80[2][2] = 1.0f;
    local_80[2][3] = local_8c.z;
    local_80[0][2] = local_80[1][2] = local_80[2][0] = local_80[2][1] = 0.0f;
    GXLoadPosMtxImm(local_80, 0);
    GXLoadNrmMtxImm(local_80, 0);
    JGeometry::TVec3<f32> local_104;
    JGeometry::TVec3<f32> local_110;
    JGeometry::TVec3<f32> local_11c;
    JGeometry::TVec3<f32> local_128;
    local_104.set(12.5f, 12.5f, 0.0f);
    local_110.set(12.5f, -12.5f, 0.0f);
    local_11c.set(-12.5f, -12.5f, 0.0f);
    local_128.set(-12.5f, 12.5f, 0.0f);
    local_c8.x = 0.0f;
    local_c8.y = 0.0f;
    local_c8.z = 0.0f;
    local_d4.x = 1.0f;
    local_d4.y = 1.0f;
    local_d4.z = 0.0f;
    local_e0.x = 1.0f;
    local_e0.y = -1.0f;
    local_e0.z = 0.0f;
    local_ec.x = -1.0f;
    local_ec.y = -1.0f;
    local_ec.z = 0.0f;
    local_f8.x = -1.0f;
    local_f8.y = 1.0f;
    local_f8.z = 0.0f;
    local_c8.normalize();
    local_d4.normalize();
    local_e0.normalize();
    local_ec.normalize();
    local_f8.normalize();
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 6);
    GXPosition3f32(0.0f, 0.0f, 0.0f);
    GXNormal3f32(local_c8.x, local_c8.y, local_c8.z);
    GXColor4u8(0xff, 0xff, 0xff, uVar7);
    GXTexCoord2f32(0.5f, 0.5f);
    GXPosition3f32(local_104.x, local_104.y, local_104.z);
    GXNormal3f32(local_d4.x, local_d4.y, local_d4.z);
    GXColor4u8(0xff, 0xff, 0xff, uVar7);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(local_110.x, local_110.y, local_110.z);
    GXNormal3f32(local_e0.x, local_e0.y, local_e0.z);
    GXColor4u8(0xff, 0xff, 0xff, uVar7);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(local_11c.x, local_11c.y, local_11c.z);
    GXNormal3f32(local_ec.x, local_ec.y, local_ec.z);
    GXColor4u8(0xff, 0xff, 0xff, uVar7);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(local_128.x, local_128.y, local_128.z);
    GXNormal3f32(local_f8.x, local_f8.y, local_f8.z);
    GXColor4u8(0xff, 0xff, 0xff, uVar7);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(local_104.x, local_104.y, local_104.z);
    GXNormal3f32(local_d4.x, local_d4.y, local_d4.z);
    GXColor4u8(0xff, 0xff, 0xff, uVar7);
    GXTexCoord2f32(1.0f, 0.0f);
    GXEnd();
    param_2->setInvisibleParticleFlag();
}

void dPa_gen_d_light8PcallBack::draw(JPABaseEmitter* param_1, JPABaseParticle* param_2) {
    Mtx local_60;
    Mtx auStack_90;
    Mtx auStack_c0;
    Mtx auStack_f0;
    JGeometry::TVec3<f32> local_100;
    JGeometry::TVec3<f32> local_10c;
    JGeometry::TVec3<f32> local_118;
    JGeometry::TVec3<f32> local_124;
    JGeometry::TVec3<f32> local_130;
    u8 uVar6 = param_1->getGlobalAlpha() & 0xff;
    JGeometry::TVec3<f32> local_13c;
    JGeometry::TVec3<f32> local_148;
    JGeometry::TVec3<f32> local_154;
    JGeometry::TVec3<f32> local_160;
    JGeometry::TVec3<f32> local_16c;
    MTXIdentity(local_60);
    MTXIdentity(auStack_90);
    param_2->getBaseAxis(local_10c);
    param_2->getLocalPosition(local_118);
    if (local_118.isZero()) {
        local_118.set(0.0f, 1.0f, 0.0f);
    } else {
        local_118.normalize();
    }
    local_124.cross(local_10c, local_118);
    if (local_124.isZero()) {
        local_124.set(0.0f, 0.0f, 1.0f);
    } else {
        local_124.normalize();
    }
    local_10c.cross(local_118, local_124);
    local_10c.normalize();
    local_60[0][0] = local_10c.x;
    local_60[0][1] = local_118.x;
    local_60[0][2] = local_124.x;
    local_60[1][0] = local_10c.y;
    local_60[1][1] = local_118.y;
    local_60[1][2] = local_124.y;
    local_60[2][0] = local_10c.z;
    local_60[2][1] = local_118.z;
    local_60[2][2] = local_124.z;
    f32 fVar3 = (-90.0f / 16384.0f) * param_2->getRotateAngle();
    if (fVar3) {
        Vec local_178 = {1.0f, 1.0f, 1.0f};
        MTXRotAxisRad(auStack_90, &local_178, (M_PI / 180.0f) * fVar3);
        MTXConcat(local_60, auStack_90, local_60);
    }
    param_2->getGlobalPosition(local_100);
    local_60[0][3] = local_100.x;
    local_60[1][3] = local_100.y;
    local_60[2][3] = local_100.z;
    param_1->getGlobalParticleScale(local_130);
    local_130.x *= param_2->getWidth(param_1);
    local_130.y *= param_2->getHeight(param_1);
    local_130.z = 0.0f;
    MTXScale(auStack_f0, local_130.x, local_130.y, local_130.z);
    MTXConcat(local_60, auStack_f0, local_60);
    MTXConcat(j3dSys.getViewMtx(), local_60, auStack_c0);
    GXLoadPosMtxImm(auStack_c0, 0);
    GXLoadNrmMtxImm(auStack_c0, 0);
    JGeometry::TVec3<f32> local_184;
    JGeometry::TVec3<f32> local_190;
    JGeometry::TVec3<f32> local_19c;
    JGeometry::TVec3<f32> local_1a8;
    local_184.set(12.5f, 12.5f, 0.0f);
    local_190.set(12.5f, -12.5f, 0.0f);
    local_19c.set(-12.5f, -12.5f, 0.0f);
    local_1a8.set(-12.5f, 12.5f, 0.0f);
    local_13c.x = 0.0f;
    local_13c.y = 0.0f;
    local_13c.z = 0.0f;
    local_148.x = 1.0f;
    local_148.y = 1.0f;
    local_148.z = 0.0f;
    local_154.x = 1.0f;
    local_154.y = -1.0f;
    local_154.z = 0.0f;
    local_160.x = -1.0f;
    local_160.y = -1.0f;
    local_160.z = 0.0f;
    local_16c.x = -1.0f;
    local_16c.y = 1.0f;
    local_16c.z = 0.0f;
    local_13c.normalize();
    local_148.normalize();
    local_154.normalize();
    local_160.normalize();
    local_16c.normalize();
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 6);
    GXPosition3f32(0.0f, 0.0f, 0.0f);
    GXNormal3f32(local_13c.x, local_13c.y, local_13c.z);
    GXColor4u8(0xff, 0xff, 0xff, uVar6);
    GXTexCoord2f32(0.5f, 0.5f);
    GXPosition3f32(local_184.x, local_184.y, local_184.z);
    GXNormal3f32(local_148.x, local_148.y, local_148.z);
    GXColor4u8(0xff, 0xff, 0xff, uVar6);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(local_190.x, local_190.y, local_190.z);
    GXNormal3f32(local_154.x, local_154.y, local_154.z);
    GXColor4u8(0xff, 0xff, 0xff, uVar6);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(local_19c.x, local_19c.y, local_19c.z);
    GXNormal3f32(local_160.x, local_160.y, local_160.z);
    GXColor4u8(0xff, 0xff, 0xff, uVar6);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(local_1a8.x, local_1a8.y, local_1a8.z);
    GXNormal3f32(local_16c.x, local_16c.y, local_16c.z);
    GXColor4u8(0xff, 0xff, 0xff, uVar6);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(local_184.x, local_184.y, local_184.z);
    GXNormal3f32(local_148.x, local_148.y, local_148.z);
    GXColor4u8(0xff, 0xff, 0xff, uVar6);
    GXTexCoord2f32(1.0f, 0.0f);
    GXEnd();
    param_2->setInvisibleParticleFlag();
}

void dPa_hermiteEcallBack_c::setOldPosP(cXyz const* param_0, cXyz const* param_1) {
    field_0x10 = param_0;
    field_0x14 = param_1;
}

void dPa_hermiteEcallBack_c::executeAfter(JPABaseEmitter* param_1) {
    JGeometry::TVec3<f32> aTStack_68;
    JGeometry::TVec3<f32> local_74;
    local_74.x = field_0x10->x;
    local_74.y = field_0x10->y;
    local_74.z = field_0x10->z;
    JGeometry::TVec3<f32> local_80;
    local_80.x = field_0xc->x;
    local_80.y = field_0xc->y;
    local_80.z = field_0xc->z;
    JGeometry::TVec3<f32> local_8c;
    JGeometry::TVec3<f32> local_98;
    param_1->setGlobalTranslation(local_80);
    if (param_1->checkStatus(2) == 0) {
        local_98.x = 0.5f * (local_80.x - local_74.x);
        local_98.y = 0.5f * (local_80.y - local_74.y);
        local_98.z = 0.5f * (local_80.z - local_74.z);
        local_8c.x = 0.5f * (local_74.x - field_0x14->x);
        local_8c.y = 0.5f * (local_74.y - field_0x14->y);
        local_8c.z = 0.5f * (local_74.z - field_0x14->z);
        f32 fVar2 = field_0xc->abs(*field_0x10);
        f32 fVar1 = (0.1f * fVar2) * mRate;
        if (mMaxCnt != 0) {
            if (fVar1 > mMaxCnt) {
                fVar1 = mMaxCnt;
            }
        }
        if (fVar1 > 1.0f) {
            f32 dVar9 = 1.0f / fVar1;
            for (f32 dVar8 = dVar9; dVar8 < 1.0f; dVar8 += dVar9) {
                aTStack_68.cubic(local_74, local_8c, local_98, local_80, dVar8);
                JPABaseParticle* particle = param_1->createParticle();
                if (particle != NULL) {
                    particle->setOffsetPosition(aTStack_68);
                }
            }
        }
    }
}

void dPa_hermiteEcallBack_c::setup(JPABaseEmitter* param_0, cXyz const* param_1,
                                       csXyz const* param_2, s8 param_3) {
    field_0xc = param_1;
    mRate = 1.0f;
    mMaxCnt = 0;
}

void dPa_particleTracePcallBack_c::execute(JPABaseEmitter* param_0, JPABaseParticle* param_1) {
    if (param_1->getAge() > 0) {
        Vec* vec = (Vec*)param_0->getUserWork();
        JGeometry::TVec3<f32> local_24;
        param_1->getOffsetPosition(local_24);
        local_24.x += vec->x;
        local_24.y += vec->y;
        local_24.z += vec->z;
        param_1->setOffsetPosition(local_24);
    }
}
