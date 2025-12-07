/**
 * d_a_warp_bug.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_warp_bug.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

void daWarpBug_c::create_init() {}

static char* l_arcName = "WarpBug";

int daWarpBug_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "kisei.bmd");
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);

    J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, "kisei.brk");
    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, "kisei.btk");

    if (pbrk != NULL) {
        mpBrk = new mDoExt_brkAnm();

        if (!mpBrk->init(modelData, pbrk, TRUE, 2, 1.0f, 0, -1)) {
            return 0;
        }
    }

    if (pbtk != NULL) {
        mpBtk = new mDoExt_btkAnm();

        if (!mpBtk->init(modelData, pbtk, TRUE, 2, 1.0f, 0, -1)) {
            return 0;
        }
    }

    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

static int createSolidHeap(fopAc_ac_c* i_this) {
    return static_cast<daWarpBug_c*>(i_this)->createHeap();
}

static int daWarpBug_Draw(daWarpBug_c* i_this) {
    return i_this->draw();
}

int daWarpBug_c::draw() {
#if DEBUG
    daPy_py_c* player_p = (daPy_py_c*)dComIfGp_getPlayer(0);
    if (!player_p->checkMidnaWarp()) {
        field_0x57c = 0;
        return 1;
    }

    cLib_chaseS(&field_0x57c, 255, 25);
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    J3DModelData* modelData = mpModel->getModelData();
    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        GXColorS10* color = modelData->getMaterialNodePointer(i)->getTevColor(1);

        if (field_0x57e != 0) {
            color->r = 0;
            color->g = 255;
            color->b = 0;
        } else {
            color->r = 250;
            color->g = 50;
            color->b = 50;
        }

        color->a = field_0x57c;
    }

    if (field_0x57e != 0) {
        field_0x57e = 0;
    }

    dComIfGd_setListDarkBG();
    mpBrk->entry(modelData);
    mpBtk->entry(modelData);
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
#else
    field_0x57c = 0;
#endif

    return 1;
}

int daWarpBug_c::execute() {
#if DEBUG
    daPy_py_c* player_p = (daPy_py_c*)dComIfGp_getPlayer(0);
#endif

    mDoMtx_stack_c::copy(mpModel->getBaseTRMtx());
    mDoMtx_stack_c::multVecZero(&current.pos);

#if DEBUG
    if (player_p->checkMidnaWarp()) {
        mpBrk->play();
        mpBtk->play();
        Z2GetAudioMgr()->seStartLevel(Z2SE_EN_YM_WAIT, &current.pos, NULL, 0, 1.0f, 1.0f, -1.0f,
                                      -1.0f, 0);
    }
#endif
    return 1;
}

static int daWarpBug_Execute(daWarpBug_c* i_this) {
    return i_this->execute();
}

static int daWarpBug_IsDelete(daWarpBug_c* i_this) {
    return 1;
}

daWarpBug_c::~daWarpBug_c() {
    dComIfG_resDelete(&mPhase, l_arcName);
}

static int daWarpBug_Delete(daWarpBug_c* i_this) {
    i_this->~daWarpBug_c();
    return 1;
}

int daWarpBug_c::create() {
    fopAcM_ct(this, daWarpBug_c);

    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createSolidHeap, 0x4000)) {
            return cPhs_ERROR_e;
        }

        create_init();
    }

    return phase;
}

static int daWarpBug_Create(fopAc_ac_c* i_this) {
    return static_cast<daWarpBug_c*>(i_this)->create();
}

static actor_method_class l_daWarpBug_Method = {
    (process_method_func)daWarpBug_Create,  (process_method_func)daWarpBug_Delete,
    (process_method_func)daWarpBug_Execute, (process_method_func)daWarpBug_IsDelete,
    (process_method_func)daWarpBug_Draw,
};

extern actor_process_profile_definition g_profile_WarpBug = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_WarpBug,
    &g_fpcLf_Method.base,
    sizeof(daWarpBug_c),
    0,
    0,
    &g_fopAc_Method.base,
    726,
    &l_daWarpBug_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
