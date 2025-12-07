/**
 * @file d_a_obj_toby.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_toby.h"
#include "d/actor/d_a_e_hz.h"
#include "d/actor/d_a_player.h"
#include "d/d_bg_w.h"
#include "d/d_bomb.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_camera_mng.h"
#include "Z2AudioLib/Z2Instances.h"

#if DEBUG
#include "d/d_debug_viewer.h"
#endif

static s8 r05yuka_image[192] = {
    0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static s8 r11yuka_image[192] = {
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

class daObj_Toby_HIO_c : public JORReflexible{
public:
    daObj_Toby_HIO_c();
    virtual ~daObj_Toby_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 field_0x8;
};

daObj_Toby_HIO_c::daObj_Toby_HIO_c() {
    field_0x4 = -1;
    field_0x8 = 1.0f;
}

static int daObj_Toby_Draw(obj_toby_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mBase;
    g_env_light.settingTevStruct(0x10, &a_this->current.pos, &a_this->tevStr);
    yuka_s* pYuka = i_this->mYukas;
    int roomNo = fopAcM_GetRoomNo(a_this);
    for (int i = 0; i < i_this->mYukaNum; i++, pYuka++) {
        if (pYuka->field_0x34 != 0) {
            if (pYuka->field_0x34 == 1) {
                dComIfGp_entrySimpleModel(pYuka->mModel1, roomNo);
            } else {
                dComIfGp_entrySimpleModel(pYuka->mModel2, roomNo);
            }
        }
    }

    #if DEBUG
    if (mDoCPd_c::getHoldY(PAD_2)) {
        GXColor local_104;
        local_104.r = 100;
        local_104.g = 255;
        local_104.b = 255;
        local_104.a = 255;

        cXyz local_100;
        cXyz acStack_f0[16];
        local_100.x = 0.0f;
        local_100.y = 0.0f;
        local_100.z = i_this->field_0x575 * 100.0f;
        for (int i = 0; i < 16; i++) {
            cMtx_YrotS(*calc_mtx, i << 12);
            MtxPosition(&local_100, acStack_f0 + i);
            acStack_f0[i] += a_this->current.pos;
        }

        for (int i = 0; i < 16; i++) {
            dDbVw_drawLineXlu(acStack_f0[i], acStack_f0[(i + 1) & 0xf], local_104, 1, 12);
        }
    }
    #endif
    return 1;
}

static void* s_bomb_sub(void* param_1, void* param_2) {
    if (fopAcM_IsActor(param_1)) {
        fopAc_ac_c* actor = (fopAc_ac_c*)param_1;
        if (dBomb_c::checkBombActor(actor)) {
            dBomb_c* bomb = (dBomb_c*)actor;
            if (bomb->checkStateExplode()) {
                cXyz acStack_28 = *(cXyz*)((u8*)param_2 + 8) - bomb->current.pos;
                if (acStack_28.abs() < 350.0f) {
                    return param_1;
                }
            }
        }
    }
    return NULL;
}

static void* shot_b_sub(void* param_1, void* param_2) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    if (fopAcM_IsActor(param_1)) {
        if (fopAcM_GetName(param_1) == PROC_BOOMERANG && daPy_py_c::checkBoomerangCharge() &&
                fopAcM_GetParam(param_1) == 1) {
            return param_1;
        }

        if (fopAcM_GetName(param_1) == PROC_E_MK_BO && (((fopAc_ac_c*)param_1)->field_0x560 != 0))
        {
            return param_1;
        }
    }
    return NULL;
}

/* 80D158F4 0001+00 data_80D158F4 @1009 */
/* 80D158F5 0003+00 data_80D158F5 None */
static u8 l_HIOInit;

static daObj_Toby_HIO_c l_HIO;

static f32 hz_check_pos_x;

static f32 hz_check_pos_z;

static void* s_hz_sub(void* param_1, void* param_2) {
    if (fopAcM_IsActor(param_1)) {
        if (fopAcM_GetName(param_1) == PROC_E_HZ) {
            daE_HZ_c* hz = (daE_HZ_c*)param_1;
            if (hz->isWait() == true) {
                f32 xDiff = hz_check_pos_x - hz->current.pos.x;
                f32 zDiff = hz_check_pos_z - hz->current.pos.z;
                if (xDiff * xDiff + zDiff * zDiff < 1600.0f) {
                    return param_1;
                }
            }

        }
    }
    return NULL;
}

static void yuka_ground(obj_toby_class* i_this, yuka_s* pYuka) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mBase;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    if (i_this->field_0x577 == 1) {
        fopAc_ac_c* bomb = (fopAc_ac_c*)fpcM_Search(s_bomb_sub, pYuka);
        if (bomb != 0) {
            cXyz local_78 = pYuka->mPos - bomb->current.pos;
            pYuka->mAngle.y = cM_atan2s(local_78.x, local_78.z);
            pYuka->field_0x39 = 2;
        }
        cXyz cStack_84 = player->current.pos - pYuka->mPos;
        if (cStack_84.abs() < 80.0f) {
            player->onLeafSe();
        }
    } else {
        hz_check_pos_x = pYuka->mPos.x;
        hz_check_pos_z = pYuka->mPos.z;
        if (fpcM_Search(s_hz_sub, i_this) != NULL) {
            pYuka->field_0x34 = 0;
        }
    }

    cXyz local_90;
    f32 var_f31;
    if (pYuka->field_0x3a == 0) {
        if (i_this->field_0x577 == 1 && daPy_getPlayerActorClass()->checkWolfDig()) {
            pYuka->field_0x3a = 20;
        } else {
            fopAc_ac_c* boomerang = (fopAc_ac_c*)fpcM_Search(shot_b_sub, i_this);
            if (boomerang == NULL) {
                return;
            }
            local_90 = boomerang->current.pos - pYuka->mPos;
            var_f31 = 100.0f;
        }
    } else {
        pYuka->field_0x3a--;
        local_90 = daPy_getLinkPlayerActorClass()->getLeftHandPos();
        local_90 -= pYuka->mPos;
        f32 dVar12 = 50.0f;
        if (JMAFastSqrt(local_90.x * local_90.x + local_90.z * local_90.z) < dVar12) {
            if ((pYuka->field_0x3a & 3) == 0) {
                pYuka->mAngle.y = cM_rndF(65536.0f);
            }

            if (pYuka->field_0x3a == 19) {
                f32 scale = pYuka->mScale;
                static cXyz sc(scale, scale, scale);
                dComIfGp_particle_set(0x8272, &pYuka->mPos, &a_this->tevStr, &player->shape_angle, &sc);
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_PEEL_OFF_LEAVES, &pYuka->mPos, 0,
                                        dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)), 1.0f, 1.0f,
                                        -1.0f, -1.0f, 0);
            }
            if (pYuka->field_0x3a == 0) {
                pYuka->field_0x34 = 0;
            }
        }
        return;
    }

    if (local_90.y > -50.0f && local_90.y < 400.0f &&
            JMAFastSqrt(local_90.x * local_90.x + local_90.z * local_90.z) < var_f31)
    {
        if (i_this->field_0x577 == 1) {
            if (i_this->mSwBit != 0xff) {
                dComIfGs_onSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this));
                for (int i = 0; i < i_this->mYukaNum; i++) {
                    if (i_this->mYukas[i].field_0x34 != 0) {
                        i_this->mYukas[i].field_0x39 = 1;
                    }
                }
                i_this->field_0x6350 = 1;
            } else {
                pYuka->field_0x39 = 1;
            }
        } else {
            pYuka->field_0x34 = 2;
            pYuka->field_0x38 = cM_rndF(30.0f) + 60.0f;
            pYuka->mAngleSpeed.y = -(cM_rndF(500.0f) + 4000.0f);
            pYuka->mAngleSpeed.x = cM_rndFX(2000.0f);
            pYuka->mAngleSpeed.z = cM_rndFX(2000.0f);
            pYuka->mSpeed.x = cM_rndFX(2.0f);
            pYuka->mSpeed.y = cM_rndF(10.0f);
            pYuka->mSpeed.z = cM_rndFX(2.0f);

            dComIfG_Bgsp().Release(pYuka->mBgW);
            pYuka->mBgW = NULL;
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_PEEL_OFF_TILE, &pYuka->mPos, 0,
                                        dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)), 1.0f, 1.0f,
                                        -1.0f, -1.0f, 0);
            if (i_this->mSwBit != 0xff) {
                dComIfGs_onSwitch(i_this->mSwBit, fopAcM_GetRoomNo(a_this));
            }
        }
    }
}

static void yuka_fly(obj_toby_class* i_this, yuka_s* pYuka) {
    pYuka->mAngle += pYuka->mAngleSpeed;
    pYuka->mPos += pYuka->mSpeed;
    pYuka->mSpeed.y += 5.0f;
    if (pYuka->mSpeed.y > 50.0f) {
        pYuka->mSpeed.y = 50.0f;
    }
    if (pYuka->field_0x38 == 0) {
        pYuka->field_0x34 = 0;
    }
    pYuka->mParticle = dComIfGp_particle_set(pYuka->mParticle, 0x8343, &pYuka->mPos, 0, 0);
}

static void yuka_mtxset(obj_toby_class* i_this, yuka_s* pYuka) {
    mDoMtx_stack_c::transS(pYuka->mPos.x, pYuka->mPos.y, pYuka->mPos.z);
    mDoMtx_stack_c::YrotM(pYuka->mAngle.y);
    mDoMtx_stack_c::XrotM(pYuka->mAngle.x);
    mDoMtx_stack_c::ZrotM(pYuka->mAngle.z);
    mDoMtx_stack_c::scaleM(pYuka->mScale, 1.0, pYuka->mScale);
    if (i_this->field_0x577 == 0 && pYuka->field_0x34 == 1) {
        pYuka->mModel1->setBaseTRMtx(mDoMtx_stack_c::get());
        cXyz cStack_28 = dComIfGp_getPlayer(0)->current.pos - pYuka->mPos;
        MTXCopy(mDoMtx_stack_c::get(), pYuka->mMtx);
        pYuka->mBgW->Move();
    } else {
        pYuka->mModel2->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

static void yuka_control(obj_toby_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    yuka_s* pYuka = i_this->mYukas;
    for (int i = 0; i < i_this->mYukaNum; i++, pYuka++) {
        if (pYuka->field_0x34 != 0) {
            if (pYuka->field_0x38 != 0) {
                pYuka->field_0x38--;
            }

            if (pYuka->field_0x34 == 1) {
                yuka_ground(i_this, pYuka);
            } else {
                yuka_fly(i_this, pYuka);
            }
            yuka_mtxset(i_this, pYuka);
            if (pYuka->field_0x39 != 0) {
                pYuka->field_0x34 = 0;
                f32 scale = pYuka->mScale;
                static cXyz sc(scale, scale, scale);
                if (pYuka->field_0x39 == 1) {
                    dComIfGp_particle_set(0x8254, &pYuka->mPos, &a_this->tevStr, &pYuka->mAngle,
                                          &sc);
                } else {
                    dComIfGp_particle_set(0x8255, &pYuka->mPos, &a_this->tevStr, &pYuka->mAngle,
                                          &sc);
                }
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_PEEL_OFF_LEAVES, &pYuka->mPos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)), 1.0f, 1.0f, -1.0f, -1.0f,
                                 0);
            }
        }
    }
}

static void demo_camera(obj_toby_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz acStack_2c;
    cXyz cStack_38;
    cXyz cStack_44;
    cXyz cStack_50;

    switch (i_this->field_0x6350) {
    case 0:
        break;
    case 1:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }
        camera->mCamera.Stop();
        i_this->field_0x6350 = 2;
        i_this->field_0x6352 = 0;
        i_this->field_0x636c = 55.0f;
        camera->mCamera.SetTrimSize(3);
        // fallthrough
    case 2:
        i_this->field_0x6360 = a_this->current.pos;
        i_this->field_0x6360.y += 50.0f;
        i_this->field_0x6354 = a_this->current.pos;
        i_this->field_0x6354.x += 800.0f;
        i_this->field_0x6354.y += 200.0f;
        i_this->field_0x6354.z += 600.0f;
        cLib_addCalc2(&i_this->field_0x636c, 30.0f, 0.1f, 0.5f);
        if (i_this->field_0x6352 == 40) {
            mDoAud_seStart(Z2SE_READ_RIDDLE_A, 0, 0, 0);
        }
        if (i_this->field_0x6352 == 110) {
            camera->mCamera.Reset(i_this->field_0x6360, i_this->field_0x6354);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            i_this->field_0x6350 = 0;
        }
        break;
    }

    if (i_this->field_0x6350 != 0) {
        cXyz cStack_5c(i_this->field_0x6360);
        cXyz cStack_68(i_this->field_0x6354);
        camera->mCamera.Set(cStack_5c, cStack_68, i_this->field_0x636c, 0);
        i_this->field_0x6352 = i_this->field_0x6352 + 1;
    }
}

static int daObj_Toby_Execute(obj_toby_class* i_this) {
    i_this->field_0x580++;
    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x586[i] != 0) {
            i_this->field_0x586[i]--;
        }
    }
    yuka_control(i_this);
    demo_camera(i_this);
    return 1;
}

static int daObj_Toby_IsDelete(obj_toby_class* param_0) {
    return 1;
}

static int daObj_Toby_Delete(obj_toby_class* i_this) {
    static int const l_bmdidx[2] = {4, 5};
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAcM_GetID(i_this);
    if (i_this->field_0x6371 != 0) {
        if (i_this->field_0x577 == 0) {
            for (u32 i = 0; i < 2; i++) {
                J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mArcName, l_bmdidx[i]);
                dComIfGp_removeSimpleModel(modelData, fopAcM_GetRoomNo(a_this));
            }
            yuka_s* pYuka = i_this->mYukas;
            for (int i = 0; i < i_this->mYukaNum; i++, pYuka++) {
                if (pYuka->mBgW != NULL) {
                    dComIfG_Bgsp().Release(pYuka->mBgW);
                }
            }
        } else if (i_this->field_0x577 == 1) {
            J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mArcName, 6);
            dComIfGp_removeSimpleModel(modelData, fopAcM_GetRoomNo(a_this));
        }
        i_this->field_0x6371 = 0;
    }
    dComIfG_resDelete(&i_this->mPhase, i_this->mArcName);
    if (i_this->field_0x6370 != 0) {
        l_HIOInit = 0;
        mDoHIO_DELETE_CHILD(l_HIO.field_0x4);
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    obj_toby_class* i_this = (obj_toby_class*)a_this;
    void* modelData;
    void* modelData2;
    yuka_s* pYuka;

    if (i_this->field_0x577 == 0) {
        modelData = dComIfG_getObjectRes(i_this->mArcName, 4);
        JUT_ASSERT(868, modelData != NULL);
        modelData2 = dComIfG_getObjectRes(i_this->mArcName, 5);
        // !@bug assert on wrong variable. not really meaningful.
        JUT_ASSERT(872, modelData != NULL);
        pYuka = i_this->mYukas;
        for (int i = 0; i < i_this->mYukaNum; i++, pYuka++) {
            i_this->mYukas[i].mModel1 = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x20000, 0x11000084);
            if (i_this->mYukas[i].mModel1 == NULL) {
                return 0;
            }

            i_this->mYukas[i].mModel2 = mDoExt_J3DModel__create((J3DModelData*)modelData2, 0x20000, 0x11000084);
            if (i_this->mYukas[i].mModel2 == NULL) {
                return 0;
            }

            pYuka->mBgW = new dBgW();
            if (pYuka->mBgW == NULL) {
                return 0;
            }
            if (pYuka->mBgW->Set((cBgD_t*)dComIfG_getObjectRes(i_this->mArcName, 9), 1, &pYuka->mMtx) == 1) {
                return 0;
            }
        }
    } else if (i_this->field_0x577 == 1) {
        a_this->field_0x567 = 1;
        modelData = dComIfG_getObjectRes(i_this->mArcName, 6);
        JUT_ASSERT(907, modelData != NULL);

        for (int i = 0; i < i_this->mYukaNum; i++) {
            i_this->mYukas[i].mModel1 = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x20000, 0x11000084);
            if (i_this->mYukas[i].mModel1 == NULL) {
                return 0;
            }
            i_this->mYukas[i].mModel2 = i_this->mYukas[i].mModel1;
        }
    }
    return 1;
}

static int set_pos_check(obj_toby_class* i_this, int param_2) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    for (int i = 0; i < param_2; i++) {
        f32 fVar5 = i_this->mYukas[i].mPos.x - i_this->mYukas[param_2].mPos.x;
        f32 fVar6 = i_this->mYukas[i].mPos.z - i_this->mYukas[param_2].mPos.z;
        f32 dVar10 = JMAFastSqrt(fVar5 * fVar5 + fVar6 * fVar6);
        if (dVar10 < i_this->field_0x57c * 0.6f) {
            return 0;
        }

        if (dVar10 < i_this->field_0x57c * 1.0f) {
            i_this->mYukas[param_2].mPos.y = i_this->mYukas[i].mPos.y + 0.1f;
            if (i_this->mYukas[param_2].mPos.y > a_this->current.pos.y + 8.0f) {
                i_this->mYukas[param_2].mPos.y =
                    (a_this->current.pos.y + 2.5f) + cM_rndF(4.0f);
            }
        }
    }
    return 1;
}

static int daObj_Toby_Create(fopAc_ac_c* a_this) {
    static u32 const l_bmdidx[2] = {4, 5};

    fopAcM_ct(a_this, obj_toby_class);
    obj_toby_class* i_this = (obj_toby_class*)a_this;
    if (strcmp(dComIfGp_getStartStageName(), "D_MN07") == 0) {
        i_this->mArcName = "Obj_Tob7";
    } else {
        i_this->mArcName = "Obj_Toby";
    }

    int rv = dComIfG_resLoad(&i_this->mPhase, i_this->mArcName);
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_TOBY PARAM %x\n", fopAcM_GetParam(a_this));
        i_this->field_0x574 = fopAcM_GetParam(a_this) & 0xff;
        i_this->field_0x575 = (fopAcM_GetParam(a_this) & 0xff00) >> 8;
        i_this->field_0x576 = (fopAcM_GetParam(a_this) >> 16) & 0xf;
        if (i_this->field_0x576 == 15) {
            i_this->field_0x576 = 0;
        }
        i_this->field_0x577 = (fopAcM_GetParam(a_this) >> 20) & 0xf;
        if (i_this->field_0x577 == 15) {
            i_this->field_0x577 = 0;
        }
        i_this->mSwBit = (fopAcM_GetParam(a_this) & 0xff000000) >> 24;

        cXyz cStack_38(a_this->home.pos);
        if (i_this->field_0x577 == 3) {
            i_this->mYukaNum = 0;
            s8* local_64;
            if (fopAcM_GetRoomNo(a_this) == 5) {
                local_64 = r05yuka_image;
            } else {
                local_64 = r11yuka_image;
            }
            for (int i = 0; i < 192; i++) {
                if (local_64[i] != 0) {
                    i_this->mYukaNum++;
                }
            }
            i_this->field_0x577 = 0;

            OS_REPORT("OBJ_TOBY//////////////OBJ_TOBY SET 1 !!\n");

            if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x8004b000)) {
                OS_REPORT("//////////////OBJ_TOBY SET NON !!\n");
                return cPhs_ERROR_e;
            }
            OS_REPORT("//////////////OBJ_TOBY SET 2 !!\n");
            yuka_s* pYuka = i_this->mYukas;
            for (int i = 0; i < i_this->mYukaNum; i++, pYuka++)
            {
                if (pYuka->mBgW != NULL) {
                    if (dComIfG_Bgsp().Regist(pYuka->mBgW, a_this) != 0) {
                        return cPhs_ERROR_e;
                    }
                }
            }
            a_this->home.pos.y += 15.0f;
            int yukaIndex = 0;
            cXyz local_44;
            local_44.y = 0.0f;
            for (int i = 0; (int)i < 192; i ++) {
                if (local_64[i] != 0) {
                    local_44.x = (i & 0xf) * 150.0f;
                    local_44.z = ((i >> 4) & 0xf) * 150.0f;
                    i_this->mYukas[yukaIndex].field_0x34 = 1;
                    i_this->mYukas[yukaIndex].mPos = a_this->home.pos + local_44;
                    i_this->mYukas[yukaIndex].field_0x36 = cM_rndF(65536.0f);
                    i_this->mYukas[yukaIndex].mScale = 1.0f;
                    yukaIndex++;
                }
            }
        } else {
            if (i_this->field_0x577 == 0) {
                i_this->mYukaNum = i_this->field_0x574 * i_this->field_0x575;
            } else if (i_this->field_0x576 == 1) {
                i_this->mYukaNum = i_this->field_0x575 * 1.5f;
            } else {
                i_this->mYukaNum = i_this->field_0x575 * 3;
            }
            if (i_this->field_0x576 == 1) {
                i_this->mYukaNum <<= 2;
            }
            if (i_this->mYukaNum > 200) {
                i_this->mYukaNum = 200;
            }

            OS_REPORT("OBJ_TOBY//////////////OBJ_TOBY SET 1 !!\n");

            if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x8004b000)) {
                OS_REPORT("//////////////OBJ_TOBY SET NON !!\n");
                return cPhs_ERROR_e;
            }

            OS_REPORT("//////////////OBJ_TOBY SET 2 !!\n");

            if (!l_HIOInit) {
                i_this->field_0x6370 = 1;
                l_HIOInit = 1;
                l_HIO.field_0x4 = mDoHIO_CREATE_CHILD("飛び床", &l_HIO);
            }

            if (i_this->field_0x577 == 0) {
                yuka_s* pYuka = i_this->mYukas;
                for (int i = 0; i < i_this->mYukaNum; i++, pYuka++)
                {
                    if (pYuka->mBgW != NULL) {
                        if (dComIfG_Bgsp().Regist(pYuka->mBgW, a_this) != 0) {
                            return cPhs_ERROR_e;
                        }
                    }
                }

                int yukaIndex = 0;
                int local_90;
                if (i_this->field_0x576 == 1) {
                    i_this->field_0x57c = 75.0f;
                    a_this->home.pos.x -= 32.5f;
                    a_this->home.pos.z -= 32.5f;
                    local_90 = 2;
                } else {
                    i_this->field_0x57c = 150.0f;
                    local_90 = 1;
                }
                a_this->home.pos.y += 15.0f;
                for (int i = 0; i < i_this->field_0x574 * local_90; i++) {
                    for (int j = 0; j < i_this->field_0x575 * local_90; j++) {
                        i_this->mYukas[yukaIndex].field_0x34 = 1;
                        i_this->mYukas[yukaIndex].mPos.x = a_this->home.pos.x + i * i_this->field_0x57c;
                        i_this->mYukas[yukaIndex].mPos.y = a_this->home.pos.y;
                        i_this->mYukas[yukaIndex].mPos.z = a_this->home.pos.z + j * i_this->field_0x57c;
                        i_this->mYukas[yukaIndex].field_0x36 = cM_rndF(65536.0f);
                        if (i_this->field_0x576 == 1) {
                            i_this->mYukas[yukaIndex].mScale = 0.5f;
                        } else {
                            i_this->mYukas[yukaIndex].mScale = 1.0f;
                        }
                        yukaIndex++;
                    }
                }
            } else {
                OS_REPORT("//////////////OBJ_TOBY SET MAX %d\n", i_this->mYukaNum);
                if (i_this->field_0x576 == 1) {
                    i_this->field_0x57c = 155.0f;
                } else {
                    i_this->field_0x57c = 250.0f;
                }
                cXyz cStack_50(0.0f, 0.2f, 0.0f);
                int local_9c = 0;
                for (int i = 0; i < i_this->mYukaNum; i++) {
                    cMtx_YrotS(*calc_mtx, cM_rndF(65536.0f));
                    MtxPosition(&cStack_50, &i_this->mYukas[i].mPos);
                    i_this->mYukas[i].mPos += a_this->current.pos;
                    if (set_pos_check(i_this, i)) {
                        
                        i_this->mYukas[i].field_0x34 = 1;
                        i_this->mYukas[i].field_0x36 = cM_rndF(65536.0f);
                        if (i_this->field_0x576 == 1) {
                            i_this->mYukas[i].mScale = 0.5f;
                        } else {
                            i_this->mYukas[i].mScale = 1.0f;
                        }
                        i_this->mYukas[i].mAngle.y = cM_rndF(65536.0f);
                        i_this->mYukas[i].mScale *= cM_rndF(0.15f) + 1.0f;
                    } else {
                        i--;
                        local_9c++;
                        if (local_9c > 1000) {
                            // Designated area of leaves is too narrow to place
                            OS_REPORT("      葉っぱの指定範囲が狭すぎて置けません！！！！\n");
                            return cPhs_ERROR_e;
                        }
                    }
                    cStack_50.z = cM_rndF(i_this->field_0x575 * 100.0f);
                }
                OS_REPORT("//////////////OBJ_TOBY SET 4 !!\n");
            }
        }
        i_this->field_0x580 = fopAcM_GetID(a_this);
        daObj_Toby_Execute(i_this);
        if (i_this->field_0x577 == 0) {
            for (u32 i = 0; i < 2; i++) {
                J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mArcName, l_bmdidx[i]);
                JUT_ASSERT(1341, modelData != NULL);
                if (dComIfGp_addSimpleModel(modelData, fopAcM_GetRoomNo(a_this), 1) == -1) {
                    // Skipping floor: simple model registration failed
                    OS_REPORT("\x1B" "[43;30m飛び床：シンプルモデル登録失敗しました。\n" "\x1B" "[m");
                }
                i_this->field_0x6371 = 1;
            }
        } else if (i_this->field_0x577 == 1) {
            J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mArcName, 6);
            JUT_ASSERT(1351, modelData != NULL);
            if (dComIfGp_addSimpleModel(modelData, fopAcM_GetRoomNo(a_this), 1) == -1) {
                //Skipping leaves: simple model registration failed.
                OS_REPORT("\x1B" "[43;30m飛び葉：シンプルモデル登録失敗しました。\n" "\x1B" "[m");
            }
            i_this->field_0x6371 = 1;
        }
    }
    return rv;
}

AUDIO_INSTANCES;

static actor_method_class l_daObj_Toby_Method = {
    (process_method_func)daObj_Toby_Create,
    (process_method_func)daObj_Toby_Delete,
    (process_method_func)daObj_Toby_Execute,
    (process_method_func)daObj_Toby_IsDelete,
    (process_method_func)daObj_Toby_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_TOBY = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_OBJ_TOBY,          // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(obj_toby_class), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  58,                     // mPriority
  &l_daObj_Toby_Method,   // sub_method
  0x00044000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
