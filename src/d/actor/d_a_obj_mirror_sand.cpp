/**
 * d_a_obj_mirror_sand.cpp
 * Mirror Chamber Sand Floor
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_mirror_sand.h"
#include "d/d_procname.h"

static char* l_arcName = "MR-Sand";

int daObjMirrorSand_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 8);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 5);
    mpBck = new mDoExt_bckAnm();

    f32 speed;
    if (isSwitch()) {
        speed = 1.0f;
    } else {
        speed = 0.0f;
    }

    if (mpBck == NULL || !mpBck->init(bck, FALSE, J3DFrameCtrl::EMode_NONE, speed, 0, -1, false)) {
        return 0;
    }

    if (isSwitch()) {
        mpBck->setFrame(bck->getFrameMax());
    }

    int i = 0;  // probably fake match, needed to avoid saving mBgMtx to a register
    cBgD_t* dzb = (cBgD_t*)dComIfG_getObjectRes(l_arcName, 11);
    if (mBgw[0].Set(dzb, dBgW::MOVE_BG_e, (Mtx*)&mBgMtx[i])) {
        return 0;
    }

    cBgD_t* dzb2 = (cBgD_t*)dComIfG_getObjectRes(l_arcName, 12);
    if (mBgw[1].Set(dzb2, dBgW::MOVE_BG_e, &mBgMtx)) {
        return 0;
    }

    return 1;
}

static int createSolidHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjMirrorSand_c*>(i_this)->createHeap();
}

void daObjMirrorSand_c::initBaseMtx() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    setBaseMtx();
}

void daObjMirrorSand_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
    mpCurrentBgw->Move();
}

int daObjMirrorSand_c::draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    mpBck->entry(mpModel->getModelData());

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    mpBck->remove(mpModel->getModelData());
    return 1;
}

static int daObjMirrorSand_Draw(daObjMirrorSand_c* i_this) {
    return i_this->draw();
}

int daObjMirrorSand_c::execute() {
    mpBck->play();

    if (mpCurrentBgw == &mBgw[0] && mpBck->getPlaySpeed() > 0.0f) {
        dComIfG_Bgsp().Release(&mBgw[0]);
        dComIfG_Bgsp().Regist(&mBgw[1], this);
        mpCurrentBgw = &mBgw[1];
        mpCurrentBgw->Move();
    }

    return 1;
}

static int daObjMirrorSand_Execute(daObjMirrorSand_c* i_this) {
    return i_this->execute();
}

static int daObjMirrorSand_IsDelete(daObjMirrorSand_c* i_this) {
    return 1;
}

daObjMirrorSand_c::~daObjMirrorSand_c() {
    if (mBgw[0].ChkUsed()) {
        dComIfG_Bgsp().Release(&mBgw[0]);
    }

    if (mBgw[1].ChkUsed()) {
        dComIfG_Bgsp().Release(&mBgw[1]);
    }

    dComIfG_resDelete(&mPhase, l_arcName);
}

static int daObjMirrorSand_Delete(daObjMirrorSand_c* i_this) {
    i_this->~daObjMirrorSand_c();
    return 1;
}

void daObjMirrorSand_c::create_init() {
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    mpBck->setPlaySpeed(0.0f);

    mBgw[0].SetCrrFunc(NULL);
    mBgw[0].SetRoomId(fopAcM_GetRoomNo(this));
    mBgw[1].SetCrrFunc(NULL);
    mBgw[1].SetRoomId(fopAcM_GetRoomNo(this));

    dBgW* current_bgw;
    if (isSwitch()) {
        current_bgw = &mBgw[1];
    } else {
        current_bgw = &mBgw[0];
    }

    mpCurrentBgw = current_bgw;
    dComIfG_Bgsp().Regist(mpCurrentBgw, this);
    initBaseMtx();
}

int daObjMirrorSand_c::create() {
    fopAcM_ct(this, daObjMirrorSand_c);

    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createSolidHeap, 0x1AD0)) {
            return cPhs_ERROR_e;
        }

        create_init();
    }

    return phase;
}

static int daObjMirrorSand_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjMirrorSand_c*>(i_this)->create();
}

static actor_method_class l_daObjMirrorSand_Method = {
    (process_method_func)daObjMirrorSand_Create,  (process_method_func)daObjMirrorSand_Delete,
    (process_method_func)daObjMirrorSand_Execute, (process_method_func)daObjMirrorSand_IsDelete,
    (process_method_func)daObjMirrorSand_Draw,
};

extern actor_process_profile_definition g_profile_Obj_MirrorSand = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_MirrorSand,
    &g_fpcLf_Method.base,
    sizeof(daObjMirrorSand_c),
    0,
    0,
    &g_fopAc_Method.base,
    679,
    &l_daObjMirrorSand_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
