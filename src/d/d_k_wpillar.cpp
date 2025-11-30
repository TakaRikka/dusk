#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_k_wpillar.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "f_op/f_op_actor_mng.h"

static const char l_arcName[] = "Always";

int dkWpillar_c::create() {
    new (this) dkWpillar_c();

    int roomNo;
    u8 envrOverride;
    if (fopAcM_gc_c::gndCheck(&pos)) {
        roomNo = fopAcM_gc_c::getRoomId();
        envrOverride = fopAcM_gc_c::getPolyColor();
    } else {
        roomNo = dComIfGp_roomControl_getStayNo();
        envrOverride = 0xFF;
    }

    dKy_tevstr_init(&mTevstr, roomNo, envrOverride);
    g_env_light.settingTevStruct(0x10, &pos, &mTevstr);

    dComIfGp_particle_set(0x1BD, &pos, &mTevstr, NULL, &scale, 255, NULL, roomNo, NULL, NULL,
                          NULL);
    if ((int)parameters != 3) {
        dComIfGp_particle_set(0x1BE, &pos, &mTevstr, NULL, &scale, 255, NULL, roomNo, NULL, NULL,
                              NULL);
    }

    if ((int)parameters == 1) {
        return cPhs_ERROR_e;
    }

    BOOL var_r30;

    mpHeap = mDoExt_createSolidHeapFromGameToCurrent(0x2200, 0x20);
    if (mpHeap != NULL) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 0x1C);
        JUT_ASSERT(96, modelData);

        mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);

        J3DAnmTransform* bckAnm = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 0xB);
        JUT_ASSERT(106, bckAnm);

        var_r30 = mBck.init(bckAnm, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false);

        J3DAnmTextureSRTKey* btkAnm = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 0x3B);
        var_r30 &= mBtk.init(modelData, btkAnm, FALSE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);

        J3DAnmTevRegKey* brkAnm = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 0x2E);
        var_r30 &= mBrk.init(modelData, brkAnm, FALSE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);

        mDoExt_restoreCurrentHeap();
        mDoExt_adjustSolidHeap(mpHeap);
    } else {
        return cPhs_ERROR_e;
    }

    if (mpModel == NULL || !var_r30) {
        return cPhs_ERROR_e;
    }

    if ((int)parameters == 2) {
        scale.set(0.5f, 0.25f, 0.5f);
    }

    mpModel->setBaseScale(scale);
    mDoMtx_stack_c::transS(pos.x, pos.y, pos.z);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    return cPhs_COMPLEATE_e;
}

static int dkWpillar_Create(kankyo_class* i_this) {
    return static_cast<dkWpillar_c*>(i_this)->create();
}

dkWpillar_c::~dkWpillar_c() {
    if (mpHeap != NULL) {
        mDoExt_destroySolidHeap(mpHeap);
    }
}

static int dkWpillar_Delete(dkWpillar_c* i_this) {
    i_this->~dkWpillar_c();
    return 1;
}

int dkWpillar_c::execute() {
    mBck.play();

    if (mBck.isStop()) {
        fopKyM_Delete(this);
    } else {
        f32 sp8;
        if (fopAcM_getWaterY(&pos, &sp8) != 0) {
            mpModel->getBaseTRMtx()[1][3] = sp8;
            pos.y = sp8;
        }
    }

    return 1;
}

static int dkWpillar_Execute(dkWpillar_c* i_this) {
    return i_this->execute();
}

int dkWpillar_c::draw() {
    g_env_light.settingTevStruct(0x10, &pos, &mTevstr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &mTevstr);

    dComIfGd_setListIndScreen();
    mBck.entry(mpModel->getModelData());
    mBtk.entry(mpModel->getModelData(), mBck.getFrame());
    mBrk.entry(mpModel->getModelData(), mBck.getFrame());
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

static int dkWpillar_Draw(dkWpillar_c* i_this) {
    return i_this->draw();
}

static leafdraw_method_class l_dkWpillar_Method = {
    (process_method_func)dkWpillar_Create,  (process_method_func)dkWpillar_Delete,
    (process_method_func)dkWpillar_Execute, (process_method_func)NULL,
    (process_method_func)dkWpillar_Draw,
};

extern kankyo_process_profile_definition g_profile_WPILLAR = {
    fpcLy_CURRENT_e,
    9,
    fpcPi_CURRENT_e,
    PROC_WPILLAR,
    &g_fpcLf_Method.base,
    sizeof(dkWpillar_c),
    0,
    0,
    &g_fopKy_Method,
    759,
    &l_dkWpillar_Method,
};
