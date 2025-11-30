/**
 * @file d_a_obj_mato.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_mato.h"
#include "d/d_cc_uty.h"
#include "d/d_s_play.h"
#include "f_op/f_op_camera_mng.h"

void daObjMATO_c::setAction(void (daObjMATO_c::*i_action)(int), int i_no) {
    if (mData[i_no].action != NULL) {
        mData[i_no].mode = -1;
        (this->*mData[i_no].action)(i_no);
    }

    mData[i_no].prev_action = mData[i_no].action;
    mData[i_no].action = i_action;
    mData[i_no].mode = 0;
    (this->*mData[i_no].action)(i_no);
}

static const int l_B_ling_bmd_table[] = {
    4,
    3,
    4,
    4,
    4,
    4,
    4,
};

int daObjMATO_c::CreateHeap() {
    int i;
    J3DModelData* a_pModelData[2];

    for (i = 0; i < 2; i++) {
        a_pModelData[i] = (J3DModelData*)dComIfG_getObjectRes("B_ling", l_B_ling_bmd_table[i]);
        JUT_ASSERT(174, a_pModelData[i] != NULL);
    }

    for (i = 0; i < mpPath->m_num; i++) {
        mData[i].model = mDoExt_J3DModel__create(a_pModelData[mData[i].type], J3DMdlFlag_DifferedDLBuffer, BMD_DEFAULT_DIFF_FLAGS);
        if (mData[i].model == NULL) {
            return 0;
        }
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return ((daObjMATO_c*)i_this)->CreateHeap();
}

static int daObjMATO_Create(fopAc_ac_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    return ((daObjMATO_c*)i_this)->create();
}

static int daObjMATO_Delete(daObjMATO_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    i_this->_delete();
    return 1;
}

void daObjMATO_c::action() {
    int i;
    for (i = 0; i < mpPath->m_num; i++) {
        if (field_0x39f6 != 0 && !mData[i].is_hit) {
            mData[i].rot.y = fopCamM_GetAngleY(dComIfGp_getCamera(0));
        }

        (this->*mData[i].action)(i);
    }
}

namespace {
static dCcD_SrcCyl cc_cyl_src = {
    {
        {0, {{0, 0, 0}, {0, 0}, 0x119}},
        {dCcD_SE_13, 0, 0, 0, 0},
        {dCcD_SE_NONE, 0, 0, 0, 2},
        {2},
    },
    {
        {0.0f, 0.0f, 0.0f},
        150.0f,
        300.0f,
    },
};
}

void daObjMATO_c::hit_check() {
    if (field_0x39f6 != 0) {
        for (int i = 0; i < mpPath->m_num; i++) {
            if (!mData[i].is_hit) {
                mData[i].ccStts.Move();
                if (mData[i].ccCyl.ChkCoHit()) {
                    fopAc_ac_c* a_pCoHitAc = dCc_GetAc(mData[i].ccCyl.GetCoHitObj()->GetAc());
                    if (fopAcM_GetName(a_pCoHitAc) == PROC_ALINK) {
                        mData[i].is_hit = TRUE;
                        getRupee(i);
                        setAction(&daObjMATO_c::disappear, i);
                    }
                }
            }
        }
    }
}

void daObjMATO_c::start_wait(int i_no) {
    if (mData[i_no].mode == 0) {
        mData[i_no].is_hit = FALSE;
        mData[i_no].rot.setall(0);
        mData[i_no].scale.setall(0.0f);
        mData[i_no].mode++;
    } else if (mData[i_no].mode != -1 && field_0x39f6 != 0) {
        setAction(&daObjMATO_c::wait, i_no);
    }
}

void daObjMATO_c::wait(int i_no) {
    if (mData[i_no].mode == 0) {
        mData[i_no].mode++;
    } else if (mData[i_no].mode != -1) {
        if (field_0x39f6 != 0) {
            cLib_chaseF(&mData[i_no].scale.x, 1.0f, 0.25f);
            cLib_chaseF(&mData[i_no].scale.y, 1.0f, 0.25f);
            cLib_chaseF(&mData[i_no].scale.z, 1.0f, 0.25f);
        } else {
            setAction(&daObjMATO_c::disappear, i_no);
        }
    }
}

static const int l_rupee_itemno_table[] = {
    fpcNm_ITEM_GREEN_RUPEE,
    fpcNm_ITEM_BLUE_RUPEE,
    fpcNm_ITEM_YELLOW_RUPEE,
    fpcNm_ITEM_RED_RUPEE,
    fpcNm_ITEM_PURPLE_RUPEE,
    fpcNm_ITEM_ORANGE_RUPEE,
    fpcNm_ITEM_SILVER_RUPEE,
};

static const u8 l_rupee_count_table[] = {
    1,
    5,
    10,
    20,
    50,
    100,
    200,
};

static const u32 l_rupee_getse[] = {
    Z2SE_GREEN_LUPY_GET,
    Z2SE_BLUE_LUPY_GET,
    Z2SE_BLUE_LUPY_GET,
    Z2SE_RED_LUPY_GET,
    Z2SE_RED_LUPY_GET,
    Z2SE_RED_LUPY_GET,
    Z2SE_RED_LUPY_GET,
};

void daObjMATO_c::disappear(int i_no) {
    if (mData[i_no].mode == 0) {
        mData[i_no].timer = 20;
        mData[i_no].rot_speed = 0x2000;
        mData[i_no].mode++;
    } else if (mData[i_no].mode != -1) {
        if (mData[i_no].mode == 1) {
            mData[i_no].rot.y += mData[i_no].rot_speed;

            mData[i_no].timer--;
            if (mData[i_no].timer == 0) {
                mData[i_no].mode++;
            }
        } else if (mData[i_no].mode == 2) {
            mData[i_no].rot.y += mData[i_no].rot_speed;

            cLib_chaseF(&mData[i_no].scale.x, 0.0f, 0.1f);
            cLib_chaseF(&mData[i_no].scale.y, 1.5f, 0.05f);
            cLib_chaseF(&mData[i_no].scale.z, 0.0f, 0.1f);
            
            if (!mData[i_no].scale.x && !mData[i_no].scale.z) {
                mData[i_no].mode++;
            }
        } else if (field_0x39f6 == 0) {
            setAction(&daObjMATO_c::start_wait, i_no);
        }
    }
}

void daObjMATO_c::getRupee(int i_no) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz size(1.0f, 1.0f, 1.0f);
    csXyz angle;
    angle.x = angle.z = 0;
    angle.y = player->shape_angle.y;

    fopAcM_createItemForSimpleDemo(&current.pos, l_rupee_itemno_table[mData[i_no].type], fopAcM_GetRoomNo(this), &angle, &size, 0.0f, 0.0f);
    dComIfGp_setItemRupeeCount(l_rupee_count_table[mData[i_no].type]);
    mDoAud_seStart(l_rupee_getse[mData[i_no].type], NULL, 0, 0);
}

int daObjMATO_c::execute() {
    u8 swbit = (fopAcM_GetParam(this) & 0xFF00) >> 8;
    if (swbit != 0xFF) {
        if (field_0x39f6 != 0) {
            if (!dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(this)) && field_0x39f4 != 0) {
                field_0x39f4--;
                if (field_0x39f4 == 0) {
                    field_0x39f6 = 0;
                }
            }
        } else if (dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(this))) {
            field_0x39f6 = 1;
            field_0x39f4 = 1500;
        }
    }

    hit_check();
    action();
    setBaseMtx();
    return 1;
}

int daObjMATO_c::_delete() {
    if (mIsAddSimpleModel) {
        J3DModelData* a_pModelData[2];

        for (int i = 0; i < 2; i++) {
            a_pModelData[i] = (J3DModelData*)dComIfG_getObjectRes("B_ling", l_B_ling_bmd_table[i]);
            dComIfGp_removeSimpleModel(a_pModelData[i], fopAcM_GetRoomNo(this));
        }
    }

    dComIfG_resDelete(&mPhase, "B_ling");
    return 1;
}

void daObjMATO_c::setBaseMtx() {
    cXyz sp8;
    for (int i = 0; i < mpPath->m_num; i++) {
        mDoMtx_stack_c::transS(mData[i].pos);
        mDoMtx_stack_c::ZXYrotM(mData[i].rot);
        mData[i].model->setBaseScale(mData[i].scale);
        mData[i].model->setBaseTRMtx(mDoMtx_stack_c::get());

        sp8 = mData[i].pos;
        sp8.y -= 420.0f + hREG_F(0);

        mData[i].ccCyl.SetC(sp8);
        mData[i].ccCyl.SetR((190.0f + hREG_F(2)) * mData[i].scale.x);
        mData[i].ccCyl.SetH((840.0f + hREG_F(1)) * mData[i].scale.y);
        dComIfG_Ccsp()->Set(&mData[i].ccCyl);
    }
}

int daObjMATO_c::draw() {
    for (int i = 0; i < mpPath->m_num; i++) {
        g_env_light.settingTevStruct(0, &mData[i].pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mData[i].model, &tevStr);
        dComIfGp_entrySimpleModel(mData[i].model, fopAcM_GetRoomNo(this));
    }

    return 1;
}

static int daObjMATO_Draw(daObjMATO_c* i_this) {
    return i_this->draw();
}

static int daObjMATO_Execute(daObjMATO_c* i_this) {
    return i_this->execute();
}

int daObjMATO_c::mato_init() {
    int rail_no = fopAcM_GetParam(this) & 0xFF;
    mpPath = dPath_GetRoomPath(rail_no, fopAcM_GetRoomNo(this));
    if (mpPath == NULL) {
        return 0;
    }

    dPnt* a_pPnt = mpPath->m_points;

    for (int i = 0; i < mpPath->m_num; i++) {
        mData[i].pos = a_pPnt->m_position;
        mData[i].scale.setall(0.0f);
        mData[i].is_hit = FALSE;
        mData[i].timer = 0;
        mData[i].rot.setall(0);
        mData[i].type = a_pPnt->mArg0;

        if (mData[i].type > 6) {
            // "Rupee arg is over upper limit: rail point No:%d"
            OS_REPORT("ルピーの引数が上限オーバー：レール点No:%d\n", i);
            // "Forcing green rupee"
            OS_REPORT("強制的に緑ルピーにします\n");
            mData[i].type = 0;
        }

        mData[i].ccStts.Init(0xFF, 0, this);
        mData[i].ccCyl.Set(cc_cyl_src);
        mData[i].ccCyl.SetStts(&mData[i].ccStts);

        setAction(&daObjMATO_c::start_wait, i);
        a_pPnt++;
    }

    return 1;
}

int daObjMATO_c::create() {
    fopAcM_ct(this, daObjMATO_c);

    if (!mato_init()) {
        OS_REPORT("MATO : rale data not found!\n");
        return cPhs_ERROR_e;
    }

    int phase_state = dComIfG_resLoad(&mPhase, "B_ling");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("MATO PARAM %x\n", fopAcM_GetParam(this));
        OS_REPORT("MATO SET 1\n");

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x4800)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("MATO SET 2\n");

        J3DModelData* a_pModelData[2];
        for (int i = 0; i < 2; i++) {
            a_pModelData[i] = (J3DModelData*)dComIfG_getObjectRes("B_ling", l_B_ling_bmd_table[i]);
            JUT_ASSERT(752, a_pModelData[i] != NULL)

            if (dComIfGp_addSimpleModel(a_pModelData[i], fopAcM_GetRoomNo(this), 0) == -1) {
                // "Cucco Game Target: simple model entry failed. "
                OS_REPORT("\x1B[43;30mコッコゲームの的：シンプルモデル登録失敗しました。\n\x1B[m");
            }

            mIsAddSimpleModel = TRUE;
        }

        fopAcM_OffStatus(this, 0);
        attention_info.flags = 0;

        field_0x39f6 = 0;
        health = 10;
        field_0x560 = 10;
        daObjMATO_Execute(this);
    }

    return phase_state;
}

static int daObjMATO_IsDelete(daObjMATO_c* i_this) {
    return 1;
}

static actor_method_class l_daObjMATO_Method = {
    (process_method_func)daObjMATO_Create,
    (process_method_func)daObjMATO_Delete,
    (process_method_func)daObjMATO_Execute,
    (process_method_func)daObjMATO_IsDelete,
    (process_method_func)daObjMATO_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Mato = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Mato,          // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjMATO_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  441,                    // mPriority
  &l_daObjMATO_Method,    // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
