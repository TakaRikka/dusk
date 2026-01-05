/**
 * d_a_obj_mirror_table.cpp
 * Mirror Chamber Chains
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_mirror_chain.h"
#include "d/d_com_inf_game.h"

static char const* l_arcName = "MR-Chain";

static u32 l_scissor[4];

#if DEBUG
static u8 l_begin;
#endif

void dScissorBegin_packet_c::draw() {
    GXGetScissor(&l_scissor[0], &l_scissor[1], &l_scissor[2], &l_scissor[3]);

    #if DEBUG
    JUT_ASSERT(76, !l_begin);
    l_begin = TRUE;
    #endif

    f32 var_f31 = FLT_MAX;
    f32 var_f30 = FLT_MAX;
    f32 sp7C = -FLT_MAX;
    f32 sp78 = -FLT_MAX;
    f32 sp74 = -FLT_MAX;

    f32 sp70 = l_scissor[0];
    f32 sp6C = sp70 + l_scissor[2];
    f32 sp68 = l_scissor[1];
    f32 sp64 = sp68 + l_scissor[3];

    cXyz spEC[5];
    int sp60 = 4;
    view_class* view_p = dComIfGd_getView();
    f32 sp58 = -view_p->near;

    cXyz* sp54 = mQuad;
    cXyz* var_r30 = spEC;

    int sp50 = 0;
    int sp4C = 0;

    for (int i = 0; i < 4; i++) {
        cMtx_multVec(view_p->viewMtx, sp54, var_r30);
        if (var_r30->z >= sp58) {
            sp50++;
        } else {
            sp4C = i;
        }

        sp54++;
        var_r30++;
    }

    if (sp50 >= 4) {
        GXSetScissor(FB_WIDTH + 1, FB_HEIGHT + 1, 0, 0);
        return;
    }

    if (sp50 != 0) {
        int sp44 = -1;

        for (int i = 0; i < 4; i++) {
            int sp3C = (sp4C + 1) % 4;
            if (sp44 < 0) {
                if (spEC[sp3C].z >= sp58) {
                    sp44 = sp3C;
                }
            } else if (spEC[sp3C].z < sp58) {
                int sp38 = (sp44 + 3) % 4;
                cXyz spE0 = spEC[sp44] - spEC[sp38];
                f32 sp34 = (sp58 - spEC[sp38].z) / spE0.z;
                spE0 *= sp34;
                spEC[sp60] = spEC[sp38] + spE0;

                sp60++;

                spE0 = spEC[sp4C] - spEC[sp3C];
                sp34 = (sp58 - spEC[sp3C].z) / spE0.z;
                spE0 *= sp34;
                spEC[sp4C] = spEC[sp3C] + spE0;

                for (int sp30 = sp44; sp30 != sp4C; sp30 = (sp30 + 1) % 4) {
                    spEC[sp30] = spEC[sp4C];
                }
                break;
            }

            sp4C = (sp4C + 1) % 4;
        }
    }

    f32 sp2C = view_p->fovy;
    f32 sp28 = view_p->aspect;
    f32 sp24 = std::tan(0.017453292f * (0.5f * sp2C));

    f32 sp20, sp1C, sp18, sp14;
    view_port_class* viewport_p = dComIfGd_getViewport();
    if (0.0f != viewport_p->x_orig) {
        sp20 = (0.5f * ((2.0f * viewport_p->x_orig) + viewport_p->width)) - (FB_WIDTH / 2);
        sp18 = FB_WIDTH;
    } else {
        sp20 = viewport_p->x_orig;
        sp18 = viewport_p->width;
    }

    if (0.0f != viewport_p->y_orig) {
        sp1C = (0.5f * ((2.0f * viewport_p->y_orig) + viewport_p->height)) - (FB_HEIGHT / 2);
        sp14 = FB_HEIGHT;
    } else {
        sp1C = viewport_p->y_orig;
        sp14 = viewport_p->height;
    }

    var_r30 = spEC;
    for (int i = 0; i < sp60; i++) {
        var_r30->y = var_r30->y / (var_r30->z * sp24);
        var_r30->x = var_r30->x / (sp28 * (-var_r30->z * sp24));

        #if WIDESCREEN_SUPPORT
        if (mDoGph_gInf_c::isWideZoom()) {
            var_r30->x *= mDoGph_gInf_c::getScale();
            var_r30->y *= mDoGph_gInf_c::getScale();
        }
        #endif

        var_r30->x = sp20 + ((1.0f + var_r30->x) * (0.5f * sp18));
        var_r30->y = sp1C + ((1.0f + var_r30->y) * (0.5f * sp14));

        if (var_r30->x < var_f31) {
            var_f31 = var_r30->x;
        }
        if (var_r30->x > sp7C) {
            sp7C = var_r30->x;
        }
        if (var_r30->y < var_f30) {
            var_f30 = var_r30->y;
        }
        if (var_r30->y > sp78) {
            sp78 = var_r30->y;
        }
        if (var_r30->z > sp74) {
            sp74 = var_r30->z;
        }

        var_r30++;
    }

    var_r30 = spEC;
    cXyz spD4 = var_r30[1] - var_r30[0];
    cXyz spC8;
    cXyz spBC(0.0f, 0.0f, 1.0f);
    for (int i = 0; i < 2; i++) {
        var_r30++;
        spC8 = var_r30[1] - var_r30[0];
        if (!spD4.isZero()) {
            spBC = spD4.outprod(spC8);
            if (spBC.z < 0.0f) {
                break;
            }
        }

        spD4 = spC8;
    }

    if (spBC.z < 0.0f || var_f31 > sp6C || sp7C < sp70 || var_f30 > sp64 || sp78 < sp68) {
        GXSetScissor(FB_WIDTH + 1, FB_HEIGHT + 1, 0, 0);
    } else {
        var_f31 = cLib_minLimit<f32>(var_f31, sp70);
        sp7C = cLib_maxLimit<f32>(sp7C, sp6C);
        var_f30 = cLib_minLimit<f32>(var_f30, sp68);
        sp78 = cLib_maxLimit<f32>(sp78, sp64);
        GXSetScissor((u32)var_f31, (u32)var_f30, (u32)(sp7C - var_f31), (u32)(sp78 - var_f30));
    }
}

void dScissorEnd_packet_c::draw() {
    #if DEBUG
    JUT_ASSERT(246, l_begin);
    l_begin = FALSE;
    #endif

    GXSetScissor(l_scissor[0], l_scissor[1], l_scissor[2], l_scissor[3]);
}

static int createSolidHeap(fopAc_ac_c* i_this) {
    return ((daObjMirrorChain_c*)i_this)->createHeap();
}

int daObjMirrorChain_c::createHeap() {
    J3DModelData* modelData;

        /* dSv_event_flag_c::F_0354 - Cutscene - [cutscene] Mirror complete */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[354])) {
        modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 16);
        JUT_ASSERT(306, modelData != NULL);
        mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);

        J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 21);
        JUT_ASSERT(316, brk != NULL);
        mpBrkAnm = new mDoExt_brkAnm();
        if (mpBrkAnm == NULL || !mpBrkAnm->init(modelData, brk, 1, 2, 1.0f, 0, -1)) {
            return 0;
        }
    } else {
        modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 15);
        JUT_ASSERT(323, modelData != NULL);
        mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        mpBrkAnm = NULL;
    }

    if (mpModel == NULL) {
        return 0;
    }

        /* dSv_event_flag_c::F_0354 - Cutscene - [cutscene] Mirror complete */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[354])) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 12);
        JUT_ASSERT(336, modelData != NULL);
        mpPortalModel = mDoExt_J3DModel__create(modelData, 0, 0x11000084);
        if (mpPortalModel == NULL) {
            return 0;
        }

        J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 9);
        JUT_ASSERT(344, bck != NULL);
        mpBckAnm = new mDoExt_bckAnm();
        if (mpBckAnm == NULL || !mpBckAnm->init(bck, 1, 2, 1.0f, 0, -1, false)) {
            return 0;
        }
        mpBckAnm->setEndFrame(300);

        J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 20);
        JUT_ASSERT(353, brk != NULL);
        mpPortalBrkAnm = new mDoExt_brkAnm();
        if (mpPortalBrkAnm == NULL || !mpPortalBrkAnm->init(modelData, brk, 1, 0, 1.0f, 0, -1))
        {
            return 0;
        }
        mpPortalBrkAnm->setFrame(brk->getFrameMax());

        cBgD_t* dzb = (cBgD_t*)dComIfG_getObjectRes(l_arcName, 26);
        if (mBgW[1].Set(dzb, 1, &mMtx)) {
            return 0;
        }

        mBgW[0] = mBgW[1];
    } else {
        J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 8);
        JUT_ASSERT(367, bck != NULL);
        mpBckAnm = new mDoExt_bckAnm();

        /* dSv_event_flag_c::F_0361 - Arbiter's Grounds - Spun the spinning pillars */
        f32 rate = (fopAcM_isSwitch(this, getSwitchNo()) || dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[361])) ? 1.0f : 0.0f;
        if (mpBckAnm == NULL || !mpBckAnm->init(bck, 1, 0, rate, 0, -1, false))
        {
            return 0;
        }

        /* dSv_event_flag_c::F_0361 - Arbiter's Grounds - Spun the spinning pillars */
        if (fopAcM_isSwitch(this, getSwitchNo()) || dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[361])) {
            mpBckAnm->setFrame(bck->getFrameMax());
        }

        J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 19);
        JUT_ASSERT(381, brk != NULL);
        mpBrkAnm = new mDoExt_brkAnm();
        if (mpBrkAnm == NULL || !mpBrkAnm->init(modelData, brk, 1, 0, 0.0f, 0, -1))
        {
            return 0;
        }
        mpBrkAnm->setPlaySpeed(0.0f);

        cBgD_t* dzb0 = (cBgD_t*)dComIfG_getObjectRes(l_arcName, 24);
        if (mBgW[0].Set(dzb0, 1, &mMtx)) {
            return 0;
        }

        cBgD_t* dzb1 = (cBgD_t*)dComIfG_getObjectRes(l_arcName, 25);
        if (mBgW[1].Set(dzb1, 1, &mMtx)) {
            return 0;
        }

        mpPortalModel = NULL;
    }

    return 1;
}

void daObjMirrorChain_c::initBaseMtx() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    setBaseMtx();
}

void daObjMirrorChain_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    if (mpPortalModel != NULL) {
        mpPortalModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    MTXCopy(mDoMtx_stack_c::get(), mMtx);
    mpActiveBgW->Move();
}

static int daObjMirrorChain_Draw(daObjMirrorChain_c* i_this) {
    return i_this->draw();
}

int daObjMirrorChain_c::draw() {
    static const f32 SCISSOR_CENTER_X = 1799.2f;
    static const f32 SCISSOR_CENTER_Y = 4779.58f;
    static const f32 SCISSOR_CENTER_Z = -23024.53f;
    static const f32 SCISSOR_SIZE = 984.0f;

    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

        /* dSv_event_flag_c::F_0354 - Cutscene - [cutscene] Mirror complete */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[354])) {
        g_env_light.setLightTevColorType_MAJI(mpPortalModel, &tevStr);

        if (mpBckAnm != NULL) {
            mpBckAnm->entry(mpPortalModel->getModelData());
        }

        if (mpBrkAnm != NULL) {
            mpBrkAnm->entry(mpModel->getModelData());
        }

        if (mpPortalBrkAnm != NULL) {
            mpPortalBrkAnm->entry(mpPortalModel->getModelData());
        }

        dComIfGd_setListBG();
        mDoExt_modelUpdateDL(mpModel);

        static Vec l_offsetScissor[4] = {
            {SCISSOR_CENTER_X - SCISSOR_SIZE / 2, SCISSOR_CENTER_Y + SCISSOR_SIZE, SCISSOR_CENTER_Z},
            {SCISSOR_CENTER_X + SCISSOR_SIZE / 2, SCISSOR_CENTER_Y + SCISSOR_SIZE, SCISSOR_CENTER_Z},
            {SCISSOR_CENTER_X + SCISSOR_SIZE / 2, SCISSOR_CENTER_Y, SCISSOR_CENTER_Z},
            {SCISSOR_CENTER_X - SCISSOR_SIZE / 2, SCISSOR_CENTER_Y, SCISSOR_CENTER_Z},
        };

        cXyz* quad = mScissorBegin.getQuad();
        PSMTXMultVecArray(mpModel->getBaseTRMtx(), l_offsetScissor, quad, 4);
        j3dSys.setDrawBuffer(dComIfGd_getXluListBG(), 0);
        mScissorEnd.entryPacket();
        mDoExt_modelUpdateDL(mpPortalModel);
        mScissorBegin.entryPacket();
        dComIfGd_setList();

        if (mpBckAnm != NULL) {
            mpBckAnm->remove(mpPortalModel->getModelData());
        }

        if (mpBrkAnm != NULL) {
            mpBrkAnm->remove(mpModel->getModelData());
        }

        if (mpPortalBrkAnm != NULL) {
            mpPortalBrkAnm->remove(mpPortalModel->getModelData());
        }
    } else {
        if (mpBckAnm != NULL) {
            mpBckAnm->entry(mpModel->getModelData());
        }

        if (mpBrkAnm != NULL) {
            mpBrkAnm->entry(mpModel->getModelData());
        }

        dComIfGd_setListBG();
        mDoExt_modelUpdateDL(mpModel);
        dComIfGd_setList();

        if (mpBckAnm != NULL) {
            mpBckAnm->remove(mpModel->getModelData());
        }

        if (mpBrkAnm != NULL) {
            mpBrkAnm->remove(mpModel->getModelData());
        }
    }

    return 1;
}

static int daObjMirrorChain_Execute(daObjMirrorChain_c* i_this) {
    return i_this->execute();
}

int daObjMirrorChain_c::execute() {
    if (mpBckAnm != NULL) {
        mpBckAnm->play();
    }

    if (mpBrkAnm != NULL) {
        mpBrkAnm->play();
    }

    if (mpPortalBrkAnm != NULL) {
        mpPortalBrkAnm->play();
    }

        /* dSv_event_flag_c::F_0354 - Cutscene - [cutscene] Mirror complete */
    if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[354]) && mpBckAnm != NULL && mpBckAnm->getPlaySpeed() > 0.0f) {
        if (mpActiveBgW == &mBgW[0]) {
            dComIfG_Bgsp().Release(&mBgW[0]);
            dComIfG_Bgsp().Regist(&mBgW[1], this);
            mpActiveBgW = &mBgW[1];
            mpActiveBgW->Move();
        }

        if (mpEmitter == NULL) {
            mpEmitter = dComIfGp_particle_set(dPa_RM(ID_ZF_S_D32_01_MONO02SAND), &current.pos, &shape_angle, NULL);
            cXyz pos;
            mDoMtx_stack_c::copy(mpModel->getAnmMtx(1));
            mDoMtx_stack_c::multVecZero(&pos);
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_MR_CHIN, &pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        } else {
            cXyz pos;
            mDoMtx_stack_c::copy(mpModel->getAnmMtx(26));
            mDoMtx_stack_c::multVecZero(&pos);
            mpEmitter->setGlobalTranslation(pos.x, pos.y, pos.z);
        }
    }

    return 1;
}

static int daObjMirrorChain_IsDelete(daObjMirrorChain_c* i_this) {
    return 1;
}

daObjMirrorChain_c::~daObjMirrorChain_c() {
    if (mBgW[0].ChkUsed()) {
        dComIfG_Bgsp().Release(&mBgW[0]);
    }

    if (mBgW[1].ChkUsed()) {
        dComIfG_Bgsp().Release(&mBgW[1]);
    }

    dComIfG_resDelete(&mPhaseReq, l_arcName);
}

static int daObjMirrorChain_Delete(daObjMirrorChain_c* i_this) {
    fopAcM_GetID(i_this);
    i_this->~daObjMirrorChain_c();
    return 1;
}

void daObjMirrorChain_c::create_init() {
    /* dSv_event_flag_c::F_0354 - Cutscene - [cutscene] Mirror complete */
    if (mpBckAnm != NULL && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[354])) {
        mpBckAnm->setPlaySpeed(0.0f);
    }

    mpEmitter = NULL;
    mBgW[0].SetCrrFunc(NULL);
    mBgW[0].SetRoomId(fopAcM_GetRoomNo(this));
    mBgW[1].SetCrrFunc(NULL);
    mBgW[1].SetRoomId(fopAcM_GetRoomNo(this));

    /* dSv_event_flag_c::F_0361 - Arbiter's Grounds - Spun the spinning pillars */
    mpActiveBgW = (fopAcM_isSwitch(this, getSwitchNo()) || dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[361])) ? &mBgW[1] : &mBgW[0];
    dComIfG_Bgsp().Regist(mpActiveBgW, this);
    initBaseMtx();
}

cPhs__Step daObjMirrorChain_c::create() {
    fopAcM_ct(this, daObjMirrorChain_c);

    cPhs__Step phase_state = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createSolidHeap, 0x71b0)) {
            return cPhs_ERROR_e;
        }
        create_init();
    }

    return phase_state;
}

static cPhs__Step daObjMirrorChain_Create(fopAc_ac_c* i_this) {
    fopAcM_GetID(i_this);
    return ((daObjMirrorChain_c*)i_this)->create();
}

static actor_method_class l_daObjMirrorChain_Method = {
    (process_method_func)daObjMirrorChain_Create,
    (process_method_func)daObjMirrorChain_Delete,
    (process_method_func)daObjMirrorChain_Execute,
    (process_method_func)daObjMirrorChain_IsDelete,
    (process_method_func)daObjMirrorChain_Draw,
};

actor_process_profile_definition g_profile_Obj_MirrorChain = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_MirrorChain,
    &g_fpcLf_Method.base,
    sizeof(daObjMirrorChain_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x2A9,
    &l_daObjMirrorChain_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
