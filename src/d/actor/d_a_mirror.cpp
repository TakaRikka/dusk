/**
 * d_a_mirror.cpp
 * Mirror of Twilight
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_mirror.h"
#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "dolphin/gf/GFGeometry.h"
#include "dolphin/gf/GFLight.h"
#include "m_Do/m_Do_lib.h"

static BOOL daMirror_c_createHeap(fopAc_ac_c* i_this) {
    return ((daMirror_c*)i_this)->createHeap();
}

static char* l_arcName = "Mirror";

static char* l_arcName2 = "MR-Table";

dMirror_packet_c::dMirror_packet_c() {
    reset();
}

void dMirror_packet_c::reset() {
    mModelCount = 0;
}

void dMirror_packet_c::calcMinMax() {
    mMinVal.set(FLT_MAX, FLT_MAX, FLT_MAX);
    mMaxVal.set(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    cXyz* quad = mQuad;
    for (int i = 0; i < 4; i++, quad++) {
        f32 val = quad->x;
        if (val < mMinVal.x) {
            mMinVal.x = val;
        }

        val = quad->x;
        if (val > mMaxVal.x) {
            mMaxVal.x = val;
        }

        val = quad->y;
        if (val < mMinVal.y) {
            mMinVal.y = val;
        }

        val = quad->y;
        if (val > mMaxVal.y) {
            mMaxVal.y = val;
        }

        val = quad->z;
        if (val < mMinVal.z) {
            mMinVal.z = val;
        }

        val = quad->z;
        if (val > mMaxVal.z) {
            mMaxVal.z = val;
        }
    }
}

int dMirror_packet_c::entryModel(J3DModel* i_model) {
    if (mModelCount >= 0x40) {
        return 0;
    }

    mModels[mModelCount++] = i_model;
    return 1;
}

void dMirror_packet_c::mirrorZdraw(f32* param_0, f32* param_1, f32 param_2, f32 param_3,
                                   f32 param_4, f32 param_5, f32 param_6, f32 param_7) {
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXColor color = {0, 0, 0, 0xFF};
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetZCompLoc(GX_TRUE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
    GXSetFogRangeAdj(0, 0, NULL);
    GXSetCullMode(GX_CULL_BACK);
    GXSetNumIndStages(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetZMode(GX_ENABLE, GX_GEQUAL, GX_ENABLE);
    GXLoadPosMtxImm(j3dSys.getViewMtx(), 0);
    GXSetCurrentMtx(0);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    for (int i = 0; i < 4; i++) {
        GXPosition3f32(mQuad[i].x, mQuad[i].y, mQuad[i].z);
    }
    GXEnd();

    if (mViewScale.y > 0.0f) {
        GXSetZMode(GX_ENABLE, GX_ALWAYS, GX_ENABLE);
        GXSetColorUpdate(GX_DISABLE);
        GXSetAlphaUpdate(GX_DISABLE);
        Mtx44 mtx;
        C_MTXOrtho(mtx, param_1[1], param_1[1] + param_1[3], param_1[0], param_1[0] + param_1[2], 0,
                   100.0f);
        GXSetProjection(mtx, GX_ORTHOGRAPHIC);
        GXLoadPosMtxImm(mDoMtx_getIdentity(), 0);

        param_3 -= 1.0f;
        param_4 -= 1.0f;
        param_5 += 1.0f;
        param_6 += 1.0f;

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(param_3, param_4, -100.0f);
        GXPosition3f32(param_5, param_4, -100.0f);
        GXPosition3f32(param_5, param_6, -100.0f);
        GXPosition3f32(param_3, param_6, -100.0f);
        GXEnd();

        GXSetProjectionv(param_0);
        GXSetZMode(1, GX_ALWAYS, 1);
        GXLoadPosMtxImm(j3dSys.getViewMtx(), 0);

        if (GXGetTexObjWidth(&mTexObj)) {
            GXLoadTexObj(&mTexObj, GX_TEXMAP0);
            GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
                            GX_TEVPREV);
            GXSetNumChans(0);
            GXSetNumTexGens(1);
            GXSetZCompLoc(GX_FALSE);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_S8, 0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);

            static s8 const l_texCoord[4][2] = {
                {0x00, 0x00},
                {0x01, 0x00},
                {0x01, 0x01},
                {0x00, 0x01},
            };

            cXyz* quad = mQuad;
            const s8* texPtr = &l_texCoord[0][0];
            for (int i = 0; i < 4; quad++, texPtr += 2, i++) {
                GXPosition3f32(quad->x, quad->y, quad->z);
                GXTexCoord2s8(texPtr[0], texPtr[1]);
            }
        } else {
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            cXyz* quad = mQuad;
            for (int i = 0; i < 4; quad++, i++) {
                GXPosition3f32(quad->x, quad->y, quad->z);
            }
        }

        GXEnd();
    } else {
        GXSetProjectionv(param_0);
    }

    GXSetColorUpdate(GX_ENABLE);
    GXSetAlphaUpdate(GX_DISABLE);
}

void dMirror_packet_c::modelDraw(J3DModel* i_model, Mtx param_1) {
    Mtx& model_mtx = i_model->getBaseTRMtx();
    cXyz sp28(model_mtx[0][3], model_mtx[1][3], model_mtx[2][3]);

    cXyz sp1C;
    cMtx_multVec(param_1, &sp28, &sp1C);

    cXyz sp10;
    cMtx_multVec(j3dSys.getViewMtx(), &sp28, &sp10);

    if (mViewScale.y > 0.0f && sp1C.z > sp10.z) {
        return;
    }

    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    JUT_ASSERT(0, kankyo != NULL);

    GXColor color = {0};
    color.r = kankyo->bg_amb_col[0].r;
    color.g = kankyo->bg_amb_col[0].g;
    color.b = kankyo->bg_amb_col[0].b;
    color.a = kankyo->bg_amb_col[0].a;

    J3DModelData* modelData = i_model->getModelData();
    u16 materialNum = modelData->getMaterialNum();
    for (u16 i = 0; i < materialNum; i++) {
        J3DMatPacket* matPacket = i_model->getMatPacket(i);
        J3DShapePacket* shapePacket = matPacket->getShapePacket();
        J3DShape* shape = shapePacket->getShape();

        if (!shape->checkFlag(1)) {
            J3DMaterial* material = modelData->getMaterialNodePointer(i);
            u32 texGenNum = material->getTexGenBlock()->getTexGenNum();
            u8 colorChanNum = material->getColorBlock()->getColorChanNum();
            u8 tevStageNum = material->getTevBlock()->getTevStageNum();
            u8 indTexStageNum = material->getIndBlock()->getIndTexStageNum();

            material->load();
            matPacket->callDL();
            shape->loadPreDrawSetting();
            if (shapePacket->getDisplayListObj() != NULL) {
                shapePacket->getDisplayListObj()->callDL();
            }

            GFSetGenMode2(texGenNum, (GXChannelID)colorChanNum, (GXTevStageID)tevStageNum,
                          indTexStageNum, GX_CULL_FRONT);
            GFSetChanAmbColor(GX_COLOR0, color);
            shapePacket->setBaseMtxPtr((Mtx*)param_1);
            shapePacket->drawFast();
            shapePacket->setBaseMtxPtr((Mtx*)j3dSys.getViewMtx());
        }
        shape->resetVcdVatCache();
    }
}

void dMirror_packet_c::mainDraw() {
    j3dSys.reinitGX();

    cXyz sp19C[5];

    Mtx sp16C;

    f32 sp150[7];
    GXGetProjectionv(sp150);

    f32 sp138[6];
    GXGetViewportv(sp138);

    u32 scissor[4];
    GXGetScissor(&scissor[0], &scissor[1], &scissor[2], &scissor[3]);

    f32 var_f31 = FLT_MAX;
    f32 var_f30 = FLT_MAX;
    f32 temp_f0 = -FLT_MAX;
    f32 var_f29 = temp_f0;
    f32 var_f28 = temp_f0;
    f32 var_f27 = temp_f0;

    f32 temp_f26 = scissor[0];
    f32 temp_f25 = temp_f26 + scissor[2];
    f32 temp_f24 = scissor[1];
    f32 temp_f23 = temp_f24 + scissor[3];

    int prjPosNum = 4;

    view_class* view = dComIfGd_getView();
    f32 temp_f22 = -view->near;
    cXyz* var_r21 = mQuad;
    cXyz* var_r22 = sp19C;
    int var_r23 = 0;
    int var_r28 = 0;
    for (int i = 0; i < 4; i++) {
        cMtx_multVec(view->viewMtx, var_r21, var_r22);
        if (var_r22->z >= temp_f22) {
            var_r23++;
        } else {
            var_r28 = i;
        }

        var_r21++;
        var_r22++;
    }

    if (var_r23 < 4) {
        if (var_r23 != 0) {
            int var_r27 = -1;

            for (int i = 0; i < 4; i++) {
                int temp_r5 = (var_r28 + 1) % 4;
                if (var_r27 < 0) {
                    if (sp19C[temp_r5].z >= temp_f22) {
                        var_r27 = temp_r5;
                    }
                } else if (sp19C[temp_r5].z < temp_f22) {
                    int temp_r29 = (var_r27 + 3) % 4;
                    cXyz sp11C = sp19C[var_r27] - sp19C[temp_r29];
                    f32 temp_f1 = sp11C.z;
                    temp_f1 = (temp_f22 - sp19C[temp_r29].z) / temp_f1;
                    sp11C *= temp_f1;

                    JUT_ASSERT(0, 0 <= prjPosNum && prjPosNum < (4 + 1));

                    sp19C[4] = sp19C[temp_r29] + sp11C;

                    prjPosNum++;

                    sp11C = sp19C[var_r28] - sp19C[temp_r5];
                    sp11C *= (temp_f22 - sp19C[temp_r5].z) / sp11C.z;
                    sp19C[var_r28] = sp19C[temp_r5] + sp11C;

                    for (int j = var_r27; j != var_r28; j = (j + 1) % 4) {
                        sp19C[j] = sp19C[var_r28];
                        temp_r29++;
                    }
                    break;
                }

                var_r28 = temp_r5;
            }
        }

        f32 aspect = view->aspect;
        f32 temp_f0_2 = tanf(MTXDegToRad(view->fovy * 0.5f));
        view_port_class* view_port = dComIfGd_getViewport();

        f32 var_f3;
        f32 var_f4;
        f32 var_f5;
        f32 var_f6;
        if (view_port->x_orig != 0.0f) {
            var_f3 = (((view_port->x_orig * 2.0f) + view_port->width) * 0.5f) - (FB_WIDTH / 2);
            var_f5 = FB_WIDTH;
        } else {
            var_f3 = view_port->x_orig;
            var_f5 = view_port->width;
        }

        if (view_port->y_orig != 0.0f) {
            var_f4 = (((view_port->y_orig * 2.0f) + view_port->height) * 0.5f) - (FB_HEIGHT / 2);
            var_f6 = FB_HEIGHT;
        } else {
            var_f4 = view_port->y_orig;
            var_f6 = view_port->height;
        }

        Vec* var_r3 = sp19C;
        for (int i = 0; i < prjPosNum; i++) {
            var_r3->y = (var_r3->y / (var_r3->z * temp_f0_2));
            var_r3->x = (var_r3->x / (aspect * (-var_r3->z * temp_f0_2)));
            var_r3->x = (var_f3 + ((1.0f + var_r3->x) * (var_f5 * 0.5f)));
            var_r3->y = (var_f4 + ((1.0f + var_r3->y) * (var_f6 * 0.5f)));

            var_r3->x = cLib_minMaxLimit<f32>(var_r3->x, temp_f26, temp_f25);
            var_r3->y = cLib_minMaxLimit<f32>(var_r3->y, temp_f24, temp_f23);

            if (var_r3->x < var_f31) {
                var_f31 = var_r3->x;
            }

            if (var_r3->x > var_f29) {
                var_f29 = var_r3->x;
            }

            if (var_r3->y < var_f30) {
                var_f30 = var_r3->y;
            }

            if (var_r3->y > var_f28) {
                var_f28 = var_r3->y;
            }

            if (var_r3->z > var_f27) {
                var_f27 = var_r3->z;
            }

            var_r3++;
        }

        if (fabsf(var_f29 - var_f31) < 8.0f || fabsf(var_f28 - var_f30) < 8.0f) {
            return;
        }

        GXSetScissor((u32)var_f31, (u32)var_f30, (u32)(var_f29 - var_f31),
                     (u32)(var_f28 - var_f30));

        cXyz sp110 = mQuad[1] - mQuad[0];
        cXyz sp104 = mQuad[2] - mQuad[1];
        cXyz spF8 = sp110.outprod(sp104);
        spF8.normalizeZP();

        f32 temp_f7 = (spF8.x * mQuad[0].x) + (spF8.y * mQuad[0].y) + (spF8.z * mQuad[0].z);
        f32 temp_f22_4 =
            ((spF8.z * view->lookat.center.z) +
             ((spF8.x * view->lookat.center.x) + (spF8.y * view->lookat.center.y))) -
            temp_f7;

        cXyz spEC =
            view->lookat.eye -
            (spF8 * (2.0f * (((spF8.z * view->lookat.eye.z) +
                              ((spF8.x * view->lookat.eye.x) + (spF8.y * view->lookat.eye.y))) -
                             temp_f7)));
        cXyz spE0 = view->lookat.center - (spF8 * (2.0f * temp_f22_4));

        cXyz spD4(0.0f, 1.0f, 0.0f);
        if (mViewScale.y > 0.0f) {
            sp110 = spEC - view->lookat.eye;
            sp104 = sp110.outprod(view->lookat.up);
            spD4 = sp110.outprod(sp104);
            spD4.normalizeZP();
            spD4 *= cXyz(-1.0f, -1.0f, -1.0f);
        }

        mDoMtx_lookAt(sp16C, &spEC, &spE0, &spD4, view->bank);
        mDoMtx_stack_c::scaleS(mViewScale);
        mDoMtx_stack_c::concat(sp16C);
        J3DShape::resetVcdVatCache();

        for (int i = 0; i < mModelCount; i++) {
            modelDraw(mModels[i], mDoMtx_stack_c::get());
        }

        j3dSys.reinitGX();
        GXSetScissor(scissor[0], scissor[1], scissor[2], scissor[3]);
        mirrorZdraw(sp150, sp138, view->far, var_f31, var_f30, var_f29, var_f28, var_f27);
    }
}

void dMirror_packet_c::draw() {
    mDoLib_clipper::changeFar(dComIfGd_getView()->far);
    if (!mDoLib_clipper::clip(j3dSys.getViewMtx(), &mMaxVal, &mMinVal)) {
        mainDraw();
    }

    mDoLib_clipper::resetFar();
    reset();
}

daMirror_c::daMirror_c() {
    m_entryModel = &daMirror_c::entryModel;
}

dMirror_packet_c::~dMirror_packet_c() {}

BOOL daMirror_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return false;
    }

    return true;
}

void daMirror_c::setModelMtx() {
    mpModel->setBaseScale(scale);

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::XrotM(shape_angle.x);
    mDoMtx_stack_c::ZrotM(0x2000);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static int daMirror_create(daMirror_c* i_this) {
    fopAcM_ct(i_this, daMirror_c);
    return i_this->daMirror_c::create();
}

int daMirror_c::create() {
    if (getSw() != 0xFF && !fopAcM_isSwitch(this, getSw())) {
        return cPhs_COMPLEATE_e;
    }

    if (m_myObj != NULL) {
        return cPhs_INIT_e;
    }

    int type = getType();
    if (type == 2) {
        int phase_state = dComIfG_resLoad(this, l_arcName);
        switch (phase_state) {
        default:
            return phase_state;
        case cPhs_COMPLEATE_e:
            if (!fopAcM_entrySolidHeap(this, daMirror_c_createHeap, 0x1540)) {
                return cPhs_ERROR_e;
            }
            break;
        }

        setModelMtx();
        mDoLib_setResTimgObj((ResTIMG*)dComIfG_getObjectRes(l_arcName, 7), &mPacket.getTexObj(), 0,
                             NULL);
        Vec src[4] = {
            {-72.5f, 145.0f, 0.0f},
            {72.5f, 145.0f, 0.0f},
            {72.5f, 0.0f, 0.0f},
            {-72.5f, 0.0f, 0.0f},
        };
        mDoMtx_stack_c::scaleS(scale);
        mDoMtx_stack_c::revConcat(mpModel->getBaseTRMtx());
        cMtx_multVecArray(mDoMtx_stack_c::get(), src, mPacket.getQuad(), 4);
        mPacket.getViewScale().set(-1.0f, 1.0f, 1.0f);
    } else {
        if (type == 1) {
            scale *= 10.0f;
            mPacket.getViewScale().set(1.0, -1.0, 1.0);
        } else {
            if (type == 3) {
                int phase_state = dComIfG_resLoad(this, l_arcName2);
                switch (phase_state) {
                default:
                    return phase_state;
                case cPhs_COMPLEATE_e:
                    mDoLib_setResTimgObj((ResTIMG*)dComIfG_getObjectRes(l_arcName2, 0x25),
                                         &mPacket.getTexObj(), 0, NULL);
                    break;
                }
            }

            mPacket.getViewScale().set(-1.0f, 1.0f, 1.0f);
        }

        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::XrotM(shape_angle.x);
        mDoMtx_stack_c::scaleM(scale.x, scale.y, scale.z);
        static Vec const l_mirrorQuad[4] = {
            {-50.0f, 100.0f, 0.0f},
            {50.0f, 100.0f, 0.0f},
            {50.0f, 0.0f, 0.0f},
            {-50.0f, 0.0f, 0.0f},
        };
        mDoMtx_stack_c::multVecArray(l_mirrorQuad, mPacket.getQuad(), 4);
    }

    m_myObj = this;
    mPacket.calcMinMax();
    static const Vec l_mirrorLook = {0.0f, 50.0f, 0.0f};
    mDoMtx_stack_c::multVec(&l_mirrorLook, &eyePos);
    return cPhs_COMPLEATE_e;
}

int daMirror_c::Delete() {
    int type = getType();
    if (type == 2) {
        dComIfG_resDelete(this, l_arcName);
    } else if (type == 3) {
        dComIfG_resDelete(this, l_arcName2);
    }

    m_myObj = NULL;
    return 1;
}

static int daMirror_Delete(daMirror_c* i_this) {
    return i_this->Delete();
}

static int daMirror_execute(daMirror_c* i_this) {
    return i_this->execute();
}

int daMirror_c::execute() {
    if (this != m_myObj) {
        if (m_myObj == NULL) {
            if (create() == cPhs_ERROR_e) {
                fopAcM_delete(this);
            }
        }

        return 1;
    }

    daPy_py_c* player = daPy_getLinkPlayerActorClass();
    JUT_ASSERT(0, player != NULL);

    if (mPacket.getViewScale().y > 0.0f && player->getKandelaarFlamePos() &&
        fopAcM_searchActorDistance2(this, player) < 40000.0f)
    {
        if (fopAcM_seenActorAngleY(this, player) < 0x4000) {
            daPy_py_c::setLookPos(&eyePos);
        }
    }

    return 1;
}

int daMirror_c::draw() {
    if (this != daMirror_c::m_myObj) {
        return 1;
    }

    if (mpModel != NULL) {
        g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
        g_env_light.setLightTevColorType(mpModel, &tevStr);
        mDoExt_modelUpdateDL(mpModel);
    }

    dComIfGd_getOpaListBG()->entryImm(&mPacket, 0);
    return 1;
}

static int daMirror_draw(daMirror_c* i_this) {
    return i_this->draw();
}

int daMirror_c::entryModel(J3DModel* i_model) {
    return mPacket.entryModel(i_model);
}

static actor_method_class daMirror_METHODS = {
    (process_method_func)daMirror_create,  (process_method_func)daMirror_Delete,
    (process_method_func)daMirror_execute, (process_method_func)NULL,
    (process_method_func)daMirror_draw,
};

actor_process_profile_definition g_profile_MIRROR = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_MIRROR,
    &g_fpcLf_Method.base,
    sizeof(daMirror_c),
    0,
    0,
    &g_fopAc_Method.base,
    757,
    &daMirror_METHODS,
    0x60000,
    fopAc_UNK_GROUP_5_e,
    fopAc_CULLBOX_0_e,
};
