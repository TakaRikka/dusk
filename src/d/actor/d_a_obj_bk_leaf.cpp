/**
 * d_a_obj_bk_leaf.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_bk_leaf.h"
#include "d/actor/d_a_obj_carry.h"
#include "d/d_com_inf_game.h"

static int CheckCreateHeap(fopAc_ac_c* i_this) {
    return static_cast<daBkLeaf_c*>(i_this)->CreateHeap();
}

void daBkLeaf_c::setBokkuri() {
    u32 sp10 = 0;
    csXyz currentAngle(current.angle);
    daObjCarry_c::make_prm(&sp10, &currentAngle, 6, getItem(), getItemBit(), 1, 2);
    field_0x578 = fopAcM_createChild(PROC_Obj_Carry, fopAcM_GetID(this), 0, &current.pos,
                                     fopAcM_GetRoomNo(this), &currentAngle, &scale, -1, 0);
}

void daBkLeaf_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daBkLeaf_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daBkLeaf_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    field_0x578 = -1;
    field_0x57D = 0;
    if (checkSetDeku()) {
        field_0x57C = 0;
        setBokkuri();
    } else {
        field_0x57C = 2;
    }
    return 1;
}

static const char* l_arcName = "Obj_bkl";

static const char* l_bmdName = "hl.bmd";

static const char* l_bckName = "hl_shake1.bck";

int daBkLeaf_c::CreateHeap() {
    int retVal;
    mpModel = mDoExt_J3DModel__create(
        static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcName, l_bmdName)), 0x80000,
        0x11000084);
    if (mpModel == NULL) {
        retVal = 0;
    } else {
        J3DAnmTransform* transform =
            static_cast<J3DAnmTransform*>(dComIfG_getObjectRes(l_arcName, l_bckName));
        mpBckAnm = new mDoExt_bckAnm;
        if (mpBckAnm == NULL || mpBckAnm->init(transform, 1, 0, 1.0f, 0, -1, false) == 0) {
            retVal = 0;
        } else {
            retVal = 1;
        }
    }
    return retVal;
}

int daBkLeaf_c::create() {
    fopAcM_ct(this, daBkLeaf_c);
    int retVal = dComIfG_resLoad(&mPhase, l_arcName);
    if (retVal == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, 0x980)) {
            return cPhs_ERROR_e;
        } else {
            if (Create() == 0) {
                return cPhs_ERROR_e;
            }
        }
    }
    return retVal;
}

int daBkLeaf_c::execute() {
    switch (field_0x57C) {
    case 0:
        if (field_0x57D != 0 && fopAcM_SearchByID(field_0x578) == NULL) {
            field_0x57E = 0x3C;
            mpBckAnm->setFrame(0.0f);
            mpBckAnm->setPlaySpeed(1.0f);
            field_0x57C = 1;
        }
        break;
    case 1:
        if (field_0x57E > 0) {
            field_0x57E--;
        }
        if (field_0x57E == 0) {
            setBokkuri();
            if (field_0x57D == 0xFF) {
                field_0x57C = 0;
            } else {
                field_0x57D--;
                if (field_0x57D == 0) {
                    field_0x57C = 2;
                } else {
                    field_0x57C = 0;
                }
            }
        }
        break;
    case 2:
        break;
    }
    setBaseMtx();
    return 1;
}

int daBkLeaf_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mpBckAnm->entry(mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);
    mpBckAnm->remove(mpModel->getModelData());
    return 1;
}

int daBkLeaf_c::_delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daBkLeaf_Draw(daBkLeaf_c* i_this) {
    return i_this->draw();
}

static int daBkLeaf_Execute(daBkLeaf_c* i_this) {
    return i_this->execute();
}

static int daBkLeaf_Delete(daBkLeaf_c* i_this) {
    return i_this->_delete();
}

static int daBkLeaf_Create(fopAc_ac_c* i_this) {
    return static_cast<daBkLeaf_c*>(i_this)->create();
}

static actor_method_class l_daBkLeaf_Method = {
    (process_method_func)daBkLeaf_Create,  (process_method_func)daBkLeaf_Delete,
    (process_method_func)daBkLeaf_Execute, NULL,
    (process_method_func)daBkLeaf_Draw,
};

extern actor_process_profile_definition g_profile_Obj_BkLeaf = {fpcLy_CURRENT_e,
                                                                7,
                                                                fpcLy_CURRENT_e,
                                                                PROC_Obj_BkLeaf,
                                                                &g_fpcLf_Method.base,
                                                                sizeof(daBkLeaf_c),
                                                                0,
                                                                0,
                                                                &g_fopAc_Method.base,
                                                                0xFC,
                                                                &l_daBkLeaf_Method,
                                                                0x40100,
                                                                0,
                                                                fopAc_CULLBOX_CUSTOM_e};
