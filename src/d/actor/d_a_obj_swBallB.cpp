/**
 * @file d_a_obj_swBallB.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_swBallB.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_obj_carry.h"
#include "d/d_com_inf_game.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "d/d_lib.h"

static daObjCarry_c* l_target_info[2];

static int l_target_info_count;

static void* s_ball_sub(void* param_1, void* param_2) {
    if (fopAcM_IsActor(param_1) && fopAcM_GetName(param_1) == PROC_Obj_Carry &&
        ((daObjCarry_c*)param_1)->prm_chk_type_lightball())
    {
        if (l_target_info_count < 2) {
            l_target_info[l_target_info_count] = (daObjCarry_c*)param_1;
            l_target_info_count++;
        }
    }
    return NULL;
}

static int CheckCreateHeap(fopAc_ac_c* param_0) {
    return ((daObjSwBallB_c*)param_0)->CreateHeap();
}

static GXColor const l_color = {0x03, 0x96, 0xFF, 0xFF};

static u16 const l_event_bitA[6] = {
    0x4304, /* dSv_event_flag_c::F_0551 - Palace of Twilight - Palace of Twilight control use 1 */
    0x4302, /* dSv_event_flag_c::F_0552 - Palace of Twilight - Palace of Twilight control use 2 */
    0x4301, /* dSv_event_flag_c::F_0553 - Palace of Twilight - Palace of Twilight control use 3 */
    0x4480, /* dSv_event_flag_c::F_0554 - Palace of Twilight - Palace of Twilight control use 4 */
    0x4440, /* dSv_event_flag_c::F_0555 - Palace of Twilight - Palace of Twilight control use 5 */
    0x4420, /* dSv_event_flag_c::F_0556 - Palace of Twilight - Palace of Twilight control use 6 */
};

static u16 const l_event_bitB[6] = {
    0x4410, /* dSv_event_flag_c::F_0557 - Palace of Twilight - Palace of Twilight control use 7 */
    0x4408, /* dSv_event_flag_c::F_0558 - Palace of Twilight - Palace of Twilight control use 8 */
    0x4404, /* dSv_event_flag_c::F_0559 - Palace of Twilight - Palace of Twilight control use 9 */
    0x4402, /* dSv_event_flag_c::F_0560 - Palace of Twilight - Palace of Twilight control use 10 */
    0x4401, /* dSv_event_flag_c::F_0561 - Palace of Twilight - Palace of Twilight control use 11 */
    0x4580, /* dSv_event_flag_c::F_0562 - Palace of Twilight - Palace of Twilight control use 12 */
};

int daObjSwBallB_c::checkArea_sub(fopAc_ac_c* param_1) {
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

void daObjSwBallB_c::search_ball() {
    l_target_info_count = 0;
    for (int i = 0; i < 2; i++) {
        l_target_info[i] = NULL;
    }
    fpcM_Search(s_ball_sub, this);
    for (int i = 0; i < l_target_info_count; i++) {
        if (l_target_info[i] != 0) {
            fpc_ProcID id = fopAcM_GetID(l_target_info[i]);
            if (id != -1) {
                int idIndex = -1;
                for (int j = 0; j < 2; j++) {
                    if (id == field_0x58c[j]) {
                        idIndex = j;
                        break;
                    }
                }
                if (idIndex < 0) {
                    for (int j = 0; j < 2; j++) {
                        if (field_0x58c[j] == -1) {
                            field_0x58c[j] = id;
                            break;
                        }
                    }
                }
            }
        }
    }

    for (int j = 0; j < 2; j++) {
        if (fopAcM_SearchByID(field_0x58c[j]) == NULL) {
            field_0x58c[j] = -1;
        }
    }
    if (field_0x59c != -1 && fopAcM_SearchByID(field_0x59c) == NULL) {
        field_0x59c = -1;
    }
}

void daObjSwBallB_c::initBaseMtx() {
    setBaseMtx();
}

void daObjSwBallB_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daObjSwBallB_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mModel->getBaseTRMtx());
    J3DJoint* joint = mModel->getModelData()->getJointNodePointer(0);
    fopAcM_setCullSizeBox(this,
        joint->getMin()->x, joint->getMin()->y, joint->getMin()->z,
        joint->getMax()->x, joint->getMax()->y, joint->getMax()->z);
    for (int i = 0; i < 2; i++) {
        field_0x58c[i] = -1;
    }
    field_0x59c = -1;

    if (fopAcM_GetRoomNo(this) == 0 && fopAcM_isSwitch(this, 0x3f)) {
        field_0x588->setFrame(field_0x588->getEndFrame());
        if (getArg1() == 1) {
            fopAcM_onSwitch(this, getSwbit2());
        }
        fopAcM_onSwitch(this, getSwbit());
        field_0x596 = 5;
    }
    GXColor* color = mModel->getModelData()->getMaterialNodePointer(0)->getTevKColor(1);
    color->r = l_color.r;
    color->g = l_color.g;
    color->b = l_color.b;
    execute();
    return 1;
}

static char* l_arcName = "P_LBswBC";

int daObjSwBallB_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 5);
    JUT_ASSERT(427, modelData != NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (mModel == 0) {
        return 0;
    }
    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 9);
    JUT_ASSERT(441, pbtk != NULL);
    field_0x588 = new mDoExt_btkAnm();
    if (field_0x588 == NULL || field_0x588->init(modelData, pbtk, 1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }
    field_0x588->setPlaySpeed(0.0f);
    return 1;
}

int daObjSwBallB_c::create() {
    fopAcM_ct(this, daObjSwBallB_c);
    if (field_0x5a0 == 0) {
        field_0x5a0 = 1;
        field_0x5a2 = home.angle.x;
        field_0x5a4 = home.angle.z;
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

int daObjSwBallB_c::execute() {
    typedef void (daObjSwBallB_c::*ballfunc)();
    static ballfunc l_func[2] = {&daObjSwBallB_c::actionRun, &daObjSwBallB_c::actionStop};
    (this->*l_func[field_0x595])();
    eventUpdate();
    field_0x588->play();
    return 1;
}

void daObjSwBallB_c::actionRun() {
    u32 iVar4 = field_0x59c;
    search_ball();
    PutCrrPos();
    switch (field_0x596) {
    case 0:
        if (iVar4 == -1 && field_0x59c != -1) {
            fopAcM_seStart(this, Z2SE_OBJ_L8_L_BALL_SW_ON, 0);
            field_0x588->setPlaySpeed(1.0f);
            field_0x596 = 1;
        } else if (field_0x5ac != 0) {
            fopAcM_seStart(this, Z2SE_OBJ_L8_L_BALL_SW_ON, 0);
            field_0x588->setPlaySpeed(1.0f);
            field_0x596 = 3;
        }
        break;
    case 1:
        if (field_0x59c == -1) {
            field_0x588->setPlaySpeed(-1.0f);
            if (dComIfGp_roomControl_getStayNo() != 0) {
                fopAcM_offSwitch(this, getSwbit());
            }
            fopAcM_seStart(this, Z2SE_OBJ_L8_L_BALL_SW_OFF, 0);
            field_0x5a8 = -1;
            field_0x596 = 0;
        } else {
            if (field_0x588->play() != 0) {
                if (dComIfGp_roomControl_getStayNo() == 0) {
                    daObjCarry_c* carryObj = (daObjCarry_c*)fopAcM_SearchByID(field_0x59c);
                    if (carryObj != NULL) {
                        if (getArg1() == 1) {
                            fopAcM_onSwitch(this, getSwbit2());
                        }
                        if (carryObj->getType() == 8) {
                            fopAcM_onSwitch(this, getSwbit());
                            dComIfGs_onEventBit(l_event_bitA[getID()]);
                            field_0x5a8 = 0;
                        } else {
                            dComIfGs_onEventBit(l_event_bitB[getID()]);
                            field_0x5a8 = 1;
                        }
                    }
                } else {
                    fopAcM_onSwitch(this, getSwbit());
                }
                if (getEvent() != 0xff) {
                    orderEvent(getEvent(), 0xff, 1);
                }
            }
            if (field_0x5ad != 0) {
                fopAcM_seStart(this, Z2SE_OBJ_L8_L_BALL_SW_OFF, 0);
                field_0x588->setPlaySpeed(-1.0f);
                field_0x596 = 4;
            }
            fopAcM_seStartLevel(this, Z2SE_OBJ_L8_L_BALL_SW, 0);
        }
        break;
    case 2:
        break;
    case 3:
        field_0x588->play();
        fopAcM_seStartLevel(this, Z2SE_OBJ_L8_L_BALL_SW, 0);
        break;
    case 4:
        if (field_0x588->play() != 0 && field_0x5ac != 0) {
            fopAcM_seStart(this, Z2SE_OBJ_L8_L_BALL_SW_ON, 0);
            field_0x588->setPlaySpeed(1.0f);
            field_0x596 = 3;
        }
        break;
    case 5:
        break;
    }
}

void daObjSwBallB_c::actionStop() {
    /* empty function */
}

void daObjSwBallB_c::PutCrrPos() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    s16 sVar5 = cLib_targetAngleY(&player->current.pos, &current.pos) - player->shape_angle.y;
    if (fopAcM_searchPlayerDistanceXZ(this) < 150.0f && sVar5 < 0x4000 && sVar5 > -0x4000) {
        daObjCarry_c* carryObj = (daObjCarry_c*)fopAcM_SearchByID(player->getGrabActorID());
        if (carryObj != NULL && fopAcM_IsActor(carryObj) &&
            fopAcM_GetName(carryObj) == PROC_Obj_Carry && carryObj->prm_chk_type_lightball())
        {
            player->setForcePutPos(current.pos);
            if (!dComIfGp_event_runCheck()) {
                dComIfGp_setDoStatus(0x14, 2);
            }
        }
    }
    daObjCarry_c* carryObj;
    for (u8 i = 0; i < 2; i++) {
        bool isArea = false;
        u8 carryNow = 0;
        u8 hookCarryNow = 0;
        if (field_0x58c[i] != -1) {
            carryObj = (daObjCarry_c*)fopAcM_SearchByID(field_0x58c[i]);
            isArea = checkArea_sub(carryObj);
            carryNow = fopAcM_checkCarryNow(carryObj) >> 13;
            hookCarryNow = fopAcM_checkHookCarryNow(carryObj) >> 20;
            if (field_0x59c == -1 || field_0x59c == field_0x58c[i]) {
                switch(field_0x599[i]) {
                case 0:
                    if ((isArea && ((carryNow == 0 && checkFlag(i, 1)) ||
                                        (hookCarryNow == 0 && checkFlag(i, 4)))) ||
                        (isArea && !checkFlag(i, 2) && carryNow == 0 && hookCarryNow == 0))
                    {
                        carryObj->startCtrl();
                        field_0x59c = field_0x58c[i];
                        field_0x599[i] = 1;
                    } else {
                        if (!isArea) {
                            field_0x59c = 0xffffffff;
                        }
                    }
                    break;
                case 1:
                    if (isArea != 0) {
                        cLib_chaseF(&carryObj->current.pos.x, current.pos.x,
                                                 10.0f);
                        cLib_chaseF(&carryObj->current.pos.z, current.pos.z,
                                                 10.0f);
                        if (fopAcM_searchActorDistanceXZ(this, carryObj) == 0.0f) {
                            carryObj->endCtrl();
                            field_0x599[i] = 0;
                        }
                    } else {
                        carryObj->endCtrl();
                        field_0x599[i] = 0;
                        field_0x59c = 0xffffffff;
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

int daObjSwBallB_c::draw() {
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

int daObjSwBallB_c::_delete() {
    dComIfG_resDelete(&field_0x57c, l_arcName);
    return 1;
}

static int daObjSwBallB_Draw(daObjSwBallB_c* param_0) {
    return param_0->draw();
}

static int daObjSwBallB_Execute(daObjSwBallB_c* param_0) {
    return param_0->execute();
}

static int daObjSwBallB_Delete(daObjSwBallB_c* param_0) {
    return param_0->_delete();
}

static int daObjSwBallB_Create(fopAc_ac_c* param_0) {
    return ((daObjSwBallB_c*)param_0)->create();
}

static actor_method_class l_daObjSwBallB_Method = {
    (process_method_func)daObjSwBallB_Create,
    (process_method_func)daObjSwBallB_Delete,
    (process_method_func)daObjSwBallB_Execute,
    NULL,
    (process_method_func)daObjSwBallB_Draw,
};

extern actor_process_profile_definition g_profile_Obj_SwBallB = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_SwBallB,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjSwBallB_c), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  572,                    // mPriority
  &l_daObjSwBallB_Method, // sub_method
  0x00044100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
