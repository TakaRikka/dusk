/**
 * d_k_wmark.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_k_wmark.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_lib.h"

int dkWmark_c::m_nowID;

int dkWmark_c::create() {
    static cXyz wolfFootScale(1.1f, 1.0f, 0.6f);

    new (this) dkWmark_c();

    field_0x48a = this->parameters >> 0x10;
    this->parameters &= 0xFFFF;

    if ((int)this->parameters >= 4) {
        return cPhs_ERROR_e;
    }

    if ((int)this->parameters == 3) {
        mColorType = 2;
    } else {
        mColorType = this->parameters;
    }

    mpHeap = mDoExt_createSolidHeapFromGameToCurrent(0x880, 0x20);
    if (mpHeap != NULL) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Alink", 0x23);
        JUT_ASSERT(60, modelData != NULL);

        mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        mDoExt_restoreCurrentHeap();
        mDoExt_adjustSolidHeap(mpHeap);
    } else {
        return cPhs_ERROR_e;
    }

    if (mpModel == NULL) {
        return cPhs_ERROR_e;
    }

    if ((int)this->parameters != 3 && daPy_py_c::checkNowWolf()) {
        mpModel->setBaseScale(wolfFootScale);
    }

    if (!setMatrix()) {
        return cPhs_ERROR_e;
    }

    if ((int)this->parameters == 1 || (int)this->parameters == 3 || (int)this->parameters == 2) {
        field_0x494 = m_nowID;
        m_nowID++;
    } else {
        field_0x488 = 1;
    }

    mAlphaFactor = 1.0f;
    dKy_tevstr_init(&mTevstr, dComIfGp_roomControl_getStayNo(), 0xFF);
    return cPhs_COMPLEATE_e;
}

static int dkWmark_Create(kankyo_class* i_this) {
    return static_cast<dkWmark_c*>(i_this)->create();
}

dkWmark_c::~dkWmark_c() {
    if (mpHeap != NULL) {
        mDoExt_destroySolidHeap(mpHeap);
    }
}

static int dkWmark_Delete(dkWmark_c* i_this) {
    i_this->~dkWmark_c();
    return 1;
}

BOOL dkWmark_c::setMatrix() {
    cXyz chk_pos(pos.x, pos.y + 100.0f, pos.z);

    if (fopAcM_gc_c::gndCheck(&chk_pos)) {
        pos.y = fopAcM_gc_c::getGroundY();

        cM3dGPla poly;
        fopAcM_gc_c::getTriPla(&poly);

        s16 temp_r26 = poly.mNormal.atan2sX_Z() - field_0x48a;
        f32 var_f31 = poly.mNormal.absXZ();

        mDoMtx_stack_c::transS(pos.x, pos.y + 1.0f, pos.z);
        mDoMtx_stack_c::ZXYrotM(cM_atan2s(var_f31 * cM_scos(temp_r26), poly.mNormal.y), field_0x48a,
                                cM_atan2s(var_f31 * -cM_ssin(temp_r26), poly.mNormal.y));
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

        if (dComIfG_Bgsp().ChkMoveBG(*fopAcM_gc_c::getGroundCheck())) {
            field_0x48c = 1;
        } else {
            field_0x48c = 0;
        }
    } else {
        return 0;
    }

    return 1;
}

int dkWmark_c::execute() {
    if (field_0x488 == 0 && abs(field_0x494 - m_nowID) > 20) {
        field_0x488 = 1;
    }

    if (field_0x488 != 0) {
        mAlphaFactor -= 0.02f;
    }

    if (mAlphaFactor <= 0.0f || (field_0x48c == 1 && !setMatrix())) {
        fopKyM_Delete(this);
        mAlphaFactor = 0.0f;
    }

    return 1;
}

static int dkWmark_Execute(dkWmark_c* i_this) {
    return i_this->execute();
}

int dkWmark_c::draw() {
    J3DGXColor color;

    if (mDoLib_clipper::clip(j3dSys.getViewMtx(), pos, scale.x * 50.0f)) {
        return 1;
    }

    g_env_light.settingTevStruct(0, &pos, &mTevstr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &mTevstr);

    static const GXColor kColor[] = {{0, 0, 0, 130}, {95, 75, 50, 60}, {100, 110, 120, 40}};
    const GXColor* color_p = &kColor[mColorType];

    color.r = color_p->r;
    color.g = color_p->g;
    color.b = color_p->b;
    color.a = color_p->a * mAlphaFactor;

    mpModel->getModelData()->getMaterialNodePointer(0)->setTevKColor(1, &color);
    mDoExt_modelUpdateDL(mpModel);

    return 1;
}

static int dkWmark_Draw(dkWmark_c* i_this) {
    return i_this->draw();
}

static leafdraw_method_class l_dkWmark_Method = {
    (process_method_func)dkWmark_Create,  (process_method_func)dkWmark_Delete,
    (process_method_func)dkWmark_Execute, (process_method_func)NULL,
    (process_method_func)dkWmark_Draw,
};

kankyo_process_profile_definition g_profile_WMARK = {
    fpcLy_CURRENT_e,
    9,
    fpcPi_CURRENT_e,
    PROC_WMARK,
    &g_fpcLf_Method.base,
    sizeof(dkWmark_c),
    0,
    0,
    &g_fopKy_Method,
    760,
    &l_dkWmark_Method,
};
