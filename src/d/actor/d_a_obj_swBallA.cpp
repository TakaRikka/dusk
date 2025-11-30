/**
 * @file d_a_obj_swBallA.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_swBallA.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_obj_carry.h"
#include "d/d_com_inf_game.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "d/d_lib.h"

static daObjCarry_c* l_target_info[8];

static int l_target_info_count;

static void* s_ball_sub(void* param_1, void* param_2) {
    if (fopAcM_IsActor(param_1) && fopAcM_GetName(param_1) == PROC_Obj_Carry &&
        ((daObjCarry_c*)param_1)->getType() == daObjCarry_c::TYPE_LV8_BALL)
    {
        if (l_target_info_count < 8) {
            l_target_info[l_target_info_count] = (daObjCarry_c*)param_1;
            l_target_info_count++;
        }
    }
    return NULL;
}

static int CheckCreateHeap(fopAc_ac_c* param_0) {
    return ((daObjSwBallA_c*)param_0)->CreateHeap();
}

static GXColor const l_color = {0x3,0x96,0xFF,0xFF};

int daObjSwBallA_c::checkArea_sub(fopAc_ac_c* param_1) {
    if (param_1 == NULL) {
        return 0;
    }
    if (getType() == 0) {
        if (param_1->current.pos.y >= current.pos.y - 10.0f &&
            param_1->current.pos.y < current.pos.y + scale.y * 100.0f &&
            fopAcM_searchActorDistanceXZ(this, param_1) < scale.x * 100.0f)
        {
            return 1;
        }
    } else {
        if (getType() == 1) {
            cXyz cStack_1c(scale.x * -100.0f, 0.0f, scale.z * -100.0f);
            cXyz cStack_28(scale.x * 100.0f, scale.y * 100.0f, scale.z * 100.0f);
            cStack_1c.y += current.pos.y - 10.0f;
            cStack_28.y += current.pos.y;
            if (param_1->current.pos.y >= cStack_1c.y && param_1->current.pos.y < cStack_28.y &&
                dLib_checkActorInRectangle(param_1, this, &cStack_1c, &cStack_28))
            {
                return 1;
            }
        }
    }
    return 0;
}


void daObjSwBallA_c::search_ball() {
    l_target_info_count = 0;
    for (int i = 0; i < 8; i++) {
        l_target_info[i] = NULL;
    }
    fpcM_Search(s_ball_sub, this);
    for (int i = 0; i < l_target_info_count; i++) {
        if (l_target_info[i] != 0) {
            fpc_ProcID id = fopAcM_GetID(l_target_info[i]);
            if (id != -1) {
                int idIndex = -1;
                for (int j = 0; j < 8; j++) {
                    if (id == field_0x58c[j]) {
                        idIndex = j;
                        break;
                    }
                }
                if (idIndex < 0) {
                    for (int j = 0; j < 8; j++) {
                        if (field_0x58c[j] == -1) {
                            field_0x58c[j] = id;
                            break;
                        }
                    }
                }
            }
        } 
    }
}

void daObjSwBallA_c::initBaseMtx() {
    setBaseMtx();
}

void daObjSwBallA_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daObjSwBallA_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mModel->getBaseTRMtx());
    J3DJoint* joint = mModel->getModelData()->getJointNodePointer(0);
    fopAcM_setCullSizeBox(this,
        joint->getMin()->x, joint->getMin()->y, joint->getMin()->z,
        joint->getMax()->x, joint->getMax()->y, joint->getMax()->z);
    for (int i = 0; i < 8; i++) {
        field_0x58c[i] = -1;
    }
    field_0x5c0 = -1;
    if (fopAcM_isSwitch(this, getSwbit2())) {
        field_0x588->setFrame(field_0x588->getEndFrame());
        field_0x5ae = 2;
    } else {
        fopAcM_offSwitch(this, getSwbit());
        field_0x588->setFrame(0.0f);
        field_0x5ae = 0;
    }
    GXColor* color = mModel->getModelData()->getMaterialNodePointer(0)->getTevKColor(1);
    color->r = l_color.r;
    color->g = l_color.g;
    color->b = l_color.b;
    return 1;
}

static char* l_arcName = "P_LBswA";

int daObjSwBallA_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(381, modelData != NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (mModel == 0) {
        return 0;
    }
    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 7);
    JUT_ASSERT(381, pbtk != NULL);
    field_0x588 = new mDoExt_btkAnm();
    if (field_0x588 == NULL || field_0x588->init(modelData, pbtk, 1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }
    field_0x588->setPlaySpeed(0.0f);
    return 1;
}

int daObjSwBallA_c::create() {
    fopAcM_ct(this, daObjSwBallA_c);
    if (field_0x5c4 == 0) {
        field_0x5c4 = 1;
        field_0x5c6 = home.angle.x;
        field_0x5c8 = home.angle.z;
        shape_angle.x = 0;
        current.angle.x = 0;
        home.angle.x = 0;
        shape_angle.z = 0;
        current.angle.z = 0;
        home.angle.z = 0;
    }
    int res = dComIfG_resLoad(&field_0x57c, l_arcName);
    if (res == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, CheckCreateHeap, 0x940) == 0) {
            return cPhs_ERROR_e;
        } else if (Create() == 0) {
            return cPhs_ERROR_e;
        }
    }
    return res;
}

int daObjSwBallA_c::execute() {
    typedef void (daObjSwBallA_c::*ballfunc)();
    static ballfunc l_func[2] = {&daObjSwBallA_c::actionRun, &daObjSwBallA_c::actionStop};
    (this->*l_func[field_0x5ad])();
    eventUpdate();
    field_0x588->play();
    return 1;
}

void daObjSwBallA_c::actionRun() {
    u32 iVar4 = field_0x5c0;
    search_ball();
    PutCrrPos();
    switch (field_0x5ae) {
    case 0:
        if (iVar4 == -1 && field_0x5c0 != -1) {
            fopAcM_seStart(this, Z2SE_OBJ_L8_L_BALL_SW_ON, 0);
            field_0x588->setPlaySpeed(1.0f);
            field_0x5ae = 1;
        }
        break;
    case 1:
        if (field_0x5c0 == -1) {
            fopAcM_seStart(this, Z2SE_OBJ_L8_L_BALL_SW_OFF, 0);
            field_0x588->setPlaySpeed(-1.0f);
            fopAcM_offSwitch(this, getSwbit());
            field_0x5ae = 0;
        } else {
            if (field_0x588->play() != 0) {
                fopAcM_onSwitch(this, getSwbit());
                if (getEvent() != 0xff) {
                    orderEvent(getEvent(), 0xff, 1);
                }
                if (getArg0() == 1) {
                    field_0x5ae = 2;
                }
            }
            fopAcM_seStartLevel(this, Z2SE_OBJ_L8_L_BALL_SW, 0);
        }
        break;
    case 2:
        break;
    }
}


void daObjSwBallA_c::actionStop() {
    /* empty function */
}

void daObjSwBallA_c::PutCrrPos() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    s16 sVar5 = cLib_targetAngleY(&player->current.pos, &current.pos) - player->shape_angle.y;
    if (fopAcM_searchPlayerDistanceXZ(this) < 300.0f && sVar5 < 0x4000) {
        daObjCarry_c* carryObj = (daObjCarry_c*)fopAcM_SearchByID(player->getGrabActorID());
        if (carryObj != NULL &&
            fopAcM_IsActor(carryObj)
                && fopAcM_GetName(carryObj) == PROC_Obj_Carry &&
              carryObj->getType() == daObjCarry_c::TYPE_LV8_BALL)
        {
            player->setForcePutPos(current.pos);
        }
    }
    daObjCarry_c* carryObj;
    for (u8 i = 0; i < 8; i++) {
        bool isArea = false;
        u8 carryNow = 0;
        u8 hookCarryNow = 0;
        if (field_0x58c[i] != -1) {
            carryObj = (daObjCarry_c*)fopAcM_SearchByID(field_0x58c[i]);
            isArea = checkArea_sub(carryObj);
            carryNow = fopAcM_checkCarryNow(carryObj) >> 13;
            hookCarryNow = fopAcM_checkHookCarryNow(carryObj) >> 20;
            if (field_0x5c0 == -1 || field_0x5c0 == field_0x58c[i]) {
                switch(field_0x5b7[i]) {
                case 0:
                    if ((isArea && ((carryNow == 0 && checkFlag(i, 1)) ||
                                        (hookCarryNow == 0 && checkFlag(i, 4)))) ||
                        (isArea && !checkFlag(i, 2) && carryNow == 0 && hookCarryNow == 0))
                    {
                        carryObj->startCtrl();
                        field_0x5c0 = field_0x58c[i];
                        field_0x5b7[i] = 1;
                    } else {
                        if (!isArea) {
                            field_0x5c0 = 0xffffffff;
                        }
                    }
                    break;
                case 1:
                    if (isArea != 0 && carryNow == 0 && hookCarryNow == 0) {
                        cLib_chaseF(&carryObj->current.pos.x, current.pos.x,
                                                 10.0f);
                        cLib_chaseF(&carryObj->current.pos.z, current.pos.z,
                                                 10.0f);
                        if (fopAcM_searchActorDistanceXZ(this, carryObj) == 0.0f) {
                            carryObj->endCtrl();
                            field_0x5b7[i] = 0;
                        }
                    } else {
                        carryObj->endCtrl();
                        field_0x5b7[i] = 0;
                        field_0x5c0 = 0xffffffff;
                    }
                    break;
                }
            }
        }
        clrFlag(i);
        if (isArea) {
            onFlag(i, 2);
        }
        if (carryNow) {
            onFlag(i, 1);
        }
        if (hookCarryNow) {
            onFlag(i, 4);
        }
    }
}

int daObjSwBallA_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    dComIfGd_setListDarkBG();
    J3DMaterial* material = mModel->getModelData()->getMaterialNodePointer(0);
    if (material->getTexGenBlock()->getTexMtx(1) != NULL) {
        J3DTexMtxInfo* texMtxInfo = &material->getTexGenBlock()->getTexMtx(1)->getTexMtxInfo();
        if (texMtxInfo != NULL) {
            Mtx auStack_48;
            C_MTXLightOrtho(auStack_48, 100.0f, -100.0f, -100.0f, 100.0f, 1.0f, 1.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::XrotS(0x4000);
            mDoMtx_stack_c::transM(-current.pos.x, -current.pos.y, -current.pos.z);
            cMtx_concat(auStack_48, mDoMtx_stack_c::get(), texMtxInfo->mEffectMtx);
        }
    }
    field_0x588->entry(mModel->getModelData());
    mDoExt_modelUpdateDL(mModel);
    mDoExt_btkAnmRemove(mModel->getModelData());
    dComIfGd_setList();
    return 1;
}


int daObjSwBallA_c::_delete() {
    if (!fopAcM_isSwitch(this, getSwbit2())) {
        fopAcM_offSwitch(this, getSwbit());
    }
    dComIfG_resDelete(&field_0x57c, l_arcName);
    return 1;
}

static int daObjSwBallA_Draw(daObjSwBallA_c* param_0) {
    return param_0->draw();
}

static int daObjSwBallA_Execute(daObjSwBallA_c* param_0) {
    return param_0->execute();
}

static int daObjSwBallA_Delete(daObjSwBallA_c* param_0) {
    return param_0->_delete();
}

static int daObjSwBallA_Create(fopAc_ac_c* param_0) {
    return ((daObjSwBallA_c*)param_0)->create();
}

static actor_method_class l_daObjSwBallA_Method = {
    (process_method_func)daObjSwBallA_Create,
    (process_method_func)daObjSwBallA_Delete,
    (process_method_func)daObjSwBallA_Execute,
    NULL,
    (process_method_func)daObjSwBallA_Draw,
};

extern actor_process_profile_definition g_profile_Obj_SwBallA = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_SwBallA,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjSwBallA_c), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  571,                    // mPriority
  &l_daObjSwBallA_Method, // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
