//
// d_a_e_oct_bg
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "dolphin/types.h"

#include "d/actor/d_a_e_oct_bg.h"
#include "f_op/f_op_actor_mng.h"
#include "c/c_damagereaction.h"
#include "d/d_particle.h"
#include "d/d_cc_uty.h"
#include "d/d_com_inf_game.h"
#include "d/d_drawlist.h"
#include "d/d_resorce.h"
#include "d/d_bomb.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"
#include "SSystem/SComponent/c_math.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "Z2AudioLib/Z2Instances.h"

static dCcD_SrcSph cc_obg_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}},  // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0},                 // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                 // mGObjTg
        {0x0},                                              // mGObjCo
    },                                                      // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
    }                                // mSphAttr
};

static dCcD_SrcSph cc_obg_at_src = {
    {
        {0x0, {{0x400, 0x1, 0xD}, {0x0, 0x0}, 0x0}},  // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0},          // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},          // mGObjTg
        {0x0},                                       // mGObjCo
    },                                               // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
    }                                // mSphAttr
};

static u16 enemyBombID[5] = {
    0x0A0D,
    0x0A0E,
    0x0A0F,
    0x0A10,
    0x0A11,
};

daE_OctBg_HIO_c::daE_OctBg_HIO_c() {
    field_0x04 = -1;
    field_0x08 = 1200.0f;
    field_0x0c = 1400.0f;
    field_0x10 = 500.0f;
    field_0x14 = 100.0f;
    field_0x18 = 1.5f;
    field_0x1c = 2000.0f;
    field_0x21 = 0;
    field_0x20 = 1;
}

void daE_OctBg_c::setAction(actionFunc param_0) {
    if (field_0xb58 != NULL) {
        field_0xb70 = -1;
        (this->*field_0xb58)();
    }
    field_0xb64 = field_0xb58;
    field_0xb58 = param_0;
    field_0xb70 = 0;
    (this->*field_0xb58)();
}

void daE_OctBg_c::setSparkEffect() {
    mDoMtx_stack_c::copy(field_0x5b4->getModel()->getAnmMtx(1));
    for (int i = 0; i < 5; i++) {
        field_0xb98[i] = dComIfGp_particle_set(field_0xb98[i], enemyBombID[i], &current.pos, &tevStr);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(field_0xb98[i]);
        if (emitter != NULL) {
            emitter->setGlobalRTMatrix(mDoMtx_stack_c::get());
        }
    }
}

bool daE_OctBg_c::checkExplode() {
    if (field_0xb90 != 0) {
        setSparkEffect();
        field_0xb90--;
        if (field_0xb90 == 0) {
            dBomb_c::createWaterBombExplode(&current.pos);
            fopAcM_delete(this);
            return true;
        }
        if (field_0xb90 < 30) {
            field_0xb94 += 0x1000;
        } else if (field_0xb90 < 45) {
            field_0xb94 += 0x800;
        } else {
            field_0xb94 += 0x300;
        }
        field_0x5bc.startCreatureSoundLevel(Z2SE_OBJ_BOMB_IGNITION, 0, dComIfGp_getReverb( fopAcM_GetRoomNo(this)));
    }
    return false;
}

void daE_OctBg_c::action() {
    if (field_0xbae != 0) {
        field_0xbae--;
    }
    if (field_0xbb0 != 0) {
        field_0xbb0--;
    }
    if (field_0xbaf != 0) {
        field_0xbaf--;
    }
    if (field_0xbad != 0) {
        field_0xbad--;
    }
    if (!checkExplode()) {
        damage_check();
        (this->*field_0xb58)();
        shape_angle = current.angle;
        fopAcM_posMoveF(this, field_0x884.GetCCMoveP());
        field_0x6ac.CrrPos(dComIfG_Bgsp());
        field_0x5bc.setLinkSearch(field_0xb74);
        if (field_0xb8c > 0x2000) {
            field_0xb8c = 0x2000;
        } else if (field_0xb8c < -0x2000) {
            field_0xb8c = -0x2000;
        }
        cLib_addCalcAngleS(&field_0xb84[3], field_0xb8c, 0x10, 0x100, 0x10);
        f32 dVar9 = field_0xb80 / 4096.0f;
        if (dVar9 > 1.0f) {
            dVar9 = 1.0f;
        }
        if (field_0xb7c < 0 && field_0xb7e >= 0) {
            field_0x5bc.startCreatureSound(Z2SE_EN_BG_SWIM, 0, -1);
        }
        if (field_0xb75 != 0 && field_0xb7c > 0 && field_0xb7e <= 0) {
            field_0x5bc.startCreatureSound(Z2SE_EN_BG_SWIM, 0, -1);
        }
        field_0xb7e = field_0xb7c;
        field_0xb84[0] = (field_0xb82 * dVar9) * cM_ssin(field_0xb7c);
        field_0xb84[1] = 1.5f * ((field_0xb82 * dVar9) * cM_ssin(field_0xb7c + 0x4000));
        field_0xb84[2] = 3.0f * ((field_0xb82 * dVar9) * cM_ssin(field_0xb7c + 0x8000));
        field_0x5b4->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        mtx_set();
        cc_set();
    }
}

static u8 struct_8073A13D;

int daE_OctBg_c::_delete() {
    dComIfG_resDelete(&mPhase, "E_bg");
    if (field_0xbb3 != 0) {
        struct_8073A13D = 0;
    }
    if (heap != NULL) {
        field_0x5b4->stopZelAnime();
    }
    return 1;
}

static int daE_OctBg_Delete(daE_OctBg_c* i_this) {
    return i_this->_delete();
}

static daE_OctBg_HIO_c l_HIO;

u8 unk_bss_78;
u8 unk_bss_79;
u8 unk_bss_7A;
u8 unk_bss_7B;

void daE_OctBg_c::cc_set() {
    cXyz cStack_1c;
    cXyz cStack_28;
    J3DModel* model = field_0x5b4->getModel();
    mDoMtx_stack_c::copy(model->getAnmMtx(0));
    cStack_1c.set(0.0f, l_HIO.field_0x18 * 25.0f, 0.0f);
    mDoMtx_stack_c::multVec(&cStack_1c, &eyePos);
    attention_info.position = eyePos;
    attention_info.position.y += l_HIO.field_0x18 * 25.0f;
    mDoMtx_stack_c::copy(model->getAnmMtx(0));
    cStack_1c.set(0.0f, l_HIO.field_0x18 * 25.0f, 0.0f);
    mDoMtx_stack_c::multVec(&cStack_1c, &cStack_28);
    field_0x8c0.SetC(cStack_28);
    field_0x8c0.SetR(l_HIO.field_0x18 * 35.0f);
    dComIfG_Ccsp()->Set(&field_0x8c0);
    mDoMtx_stack_c::copy(model->getAnmMtx(0));
    cStack_1c.set(0.0f, l_HIO.field_0x18 * 25.0f, l_HIO.field_0x18 * 20.0f);
    mDoMtx_stack_c::multVec(&cStack_1c, &cStack_28);
    field_0x9f8.SetC(cStack_28);
    field_0x9f8.SetR(l_HIO.field_0x18 * 20.0f);
    dComIfG_Ccsp()->Set(&field_0x9f8);
}

void daE_OctBg_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::transM(0.0f, l_HIO.field_0x18 * 25.0f, 0.0f);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, l_HIO.field_0x18 * -25.0f, 0.0f);
    mDoMtx_stack_c::scaleM(l_HIO.field_0x18, l_HIO.field_0x18, l_HIO.field_0x18);
    J3DModel* model = field_0x5b4->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    for (u16 i = 1; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(&daE_OctBg_c::JointCallBack);
    }
    field_0x5b4->modelCalc();
    for (u16 i = 1; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(NULL);
    }
}

bool daE_OctBg_c::checkNormalAttack() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (dComIfGp_event_runCheck()) {
        return false;
    }
    if (unk_bss_7A < l_HIO.field_0x20 &&
         home.pos.absXZ(player->current.pos) < l_HIO.field_0x08 &&
        player->current.pos.y < l_HIO.field_0x0c &&
          !fopAcM_otherBgCheck(this, player))
    {
        s16 targetAngle = cLib_targetAngleY(&current.pos, &player->current.pos);
        if (abs(current.angle.y - targetAngle) < 0x2000) {
            return true;
        }
    }
    return false;
}

bool daE_OctBg_c::checkChase() {
    if (field_0xbac == 0) {
        return false;
    }

    fopAc_ac_c* actor;
    if (fopAcM_SearchByID(fopAcM_GetLinkId(this), &actor) == 0 || actor == NULL) {
        field_0xbac = 0;
        unk_bss_79--;
        return false;
    }
    return true;
}

void daE_OctBg_c::born_swim() {
    if (field_0xb70 == 0) {
        field_0x8c0.SetTgType(0);
        field_0xb8c = 0;
        field_0xb82 = 0xc00;
        field_0xb80 = 0x2400;
        current.angle.x = cM_rndFX(4096.0f) + -16384.0f;
        current.angle.y = cM_rndFX(32768.0f);
        attention_info.flags = 0;
        speedF = 3.0f;
        speed.y = 10.0f;
        field_0xbae = cM_rndF(18.0f);
        field_0xb70++;
    } else if (field_0xb70 == -1) {
        field_0x8c0.SetTgType(0xd8fbfdff);
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else if (field_0xbae == 0) {
        cLib_addCalcAngleS(&current.angle.x, 0, 0x10, 0xc00, 0x200);
        cLib_chaseF(&speedF, 0.0f, 0.05f);
        cLib_chaseF(&speed.y, 0.0f, 0.2f);
        if (cLib_addCalcAngleS(&field_0xb80, 0x400, 0x20, 0x400, 0x80) < 0x80 && !speedF &&
            !speed.y)
        {
            if (checkChase()) {
                setAction(&daE_OctBg_c::chase_core);
            } else {
                setAction(&daE_OctBg_c::swim);
            }
        }
        field_0xb7c += field_0xb80;
    }
}

void daE_OctBg_c::swim() {
    if (field_0xb70 == 0) {
        field_0xb8c = 0;
        field_0xb82 = 0xc00;
        if (current.pos.absXZ(home.pos) < (l_HIO.field_0x08 - 100.0f)) {
            field_0xb96 = current.angle.y - 0x8000 + cM_rndFX(16384.0f);
        } else {
            field_0xb96 = cLib_targetAngleY(&current.pos, &home.pos) + cM_rndFX(8192.0f);
        }
        field_0x660.y = current.pos.y + cM_rndFX(2000.0f);
        field_0x660.x = cM_ssin(field_0xb96) * 500.0f;
        field_0x660.z = cM_scos(field_0xb96) * 500.0f;
        field_0xbae = cM_rndFX(30.0f) + 100.0f;
        attention_info.distances[fopAc_attn_BATTLE_e] = 3;
        field_0xb70++;
    } else if (field_0xb70 == -1) {
        attention_info.distances[fopAc_attn_BATTLE_e] = 4;
    } else {
        if (checkNormalAttack()) {
            setAction(&daE_OctBg_c::normal_attack);
        } else {
            if (field_0x660.y > (home.pos.y + l_HIO.field_0x0c) - 100.0f) {
                field_0x660.y = current.pos.y - cM_rndF(2000.0f);
            }
            cXyz cStack_58(current.pos);
            cStack_58.y += 200.0f;
            if (fopAcM_gc_c::gndCheck(&cStack_58) &&
                field_0x660.y < fopAcM_gc_c::getGroundY() + 200.0f)
            {
                field_0x660.y = current.pos.y + cM_rndF(2000.0f);
            }
            cLib_chaseAngleS(&current.angle.y, field_0xb96, 0x200);
            cLib_chaseAngleS(&current.angle.x, -cLib_targetAngleX(&current.pos, &field_0x660),
                             0x200);
            cLib_chaseF(&speedF, cM_scos(current.angle.x) * 4.0f, 0.2f);
            cLib_chaseF(&speed.y, cM_ssin(current.angle.x) * -4.0f, 0.2f);
            field_0xb8c = field_0xb96 - current.angle.y;
            cLib_addCalcAngleS(&field_0xb80, 0x1000, 0x20, 0x400, 0x80);
            field_0xb7c += field_0xb80;
            if (unk_bss_7B != 0 &&
                unk_bss_79 < unk_bss_78 >> 1)
            {
                field_0xbac = 1;
                unk_bss_79++;
                setAction(&daE_OctBg_c::chase_core);
                if (unk_bss_79 >= unk_bss_78 >> 1) {
                    unk_bss_7B = 0;
                }
            } else if (field_0xbac != 0 && !dComIfGp_event_runCheck()) {
                setAction(&daE_OctBg_c::chase_core);
            } else if (field_0xbae == 0) {
                setAction(&daE_OctBg_c::swim);
            }
        }
    }
}

void daE_OctBg_c::setChasePos(cXyz& param_1, f32 param_2) {
    mDoMtx_stack_c::YrotS(cM_rndFX(32768.0f));
    mDoMtx_stack_c::XrotM(cM_rndFX(32768.0f));
    mDoMtx_stack_c::transM(0.0f, 0.0f, param_2);
    mDoMtx_stack_c::multVecZero(&param_1);
}

bool daE_OctBg_c::checkCoreFishAttack() {
    fopAc_ac_c* actor;
    if (fopAcM_SearchByID(fopAcM_GetLinkId(this), &actor) == 0 || actor == NULL) {
        return false;
    }
    if (fopAcM_CheckStatus(actor, 0x100000) == 0) {
        return false;
    }
    return true;
}

void daE_OctBg_c::chase_core() {
    if (field_0xb70 == 0) {
        setChasePos(field_0x660, cM_rndF(l_HIO.field_0x10));
        field_0xbaf = cM_rndFX(20.0f) + 100.0f;
        field_0xb70++;
    } else if (field_0xb70 != -1) {
        cXyz acStack_2c;
        fopAc_ac_c* local_6c;
        if (fopAcM_SearchByID(fopAcM_GetLinkId(this), &local_6c) == 0 || local_6c == NULL) {
            setAction(&daE_OctBg_c::swim);
        } else {
            if (local_6c->current.pos.y < home.pos.y + l_HIO.field_0x14) {
                acStack_2c = local_6c->current.pos + field_0x660;
                acStack_2c.y = (home.pos.y + l_HIO.field_0x14) + cM_rndF(200.0f);
            } else {
                acStack_2c = local_6c->current.pos + field_0x660;
            }
            field_0xb96 = cLib_targetAngleY(&current.pos, &acStack_2c);
            field_0xb8c = field_0xb96 - current.angle.y;
            if (abs(current.angle.y - field_0xb96) > 0x4000) {
                cLib_chaseAngleS(&current.angle.y, field_0xb96, 0x400);
                cLib_chaseAngleS(&current.angle.x, -cLib_targetAngleX(&current.pos, &acStack_2c), 0x400);
                if (current.pos.abs(acStack_2c) < l_HIO.field_0x10 - 100.0f) {
                    cLib_chaseF(&speedF, cM_scos(current.angle.x) * 8.0f, 0.2f);
                    cLib_chaseF(&speed.y, cM_ssin(current.angle.x) * -8.0f, 0.2f);
                    cLib_addCalcAngleS(&field_0xb80, 0xc00, 0x20, 0x400, 0x80);
                }
            } else {
                cLib_chaseAngleS(&current.angle.y, (int)field_0xb96, 0x200);
                cLib_chaseAngleS(&current.angle.x, -cLib_targetAngleX(&current.pos, &acStack_2c),
                                 0x200);
                ;
                if (current.pos.abs(acStack_2c) > l_HIO.field_0x10 - 100.0f) {
                    cLib_chaseF(&speedF, 18.0f * cM_scos(current.angle.x), 0.4f);
                    cLib_chaseF(&speed.y, (-18.0f * cM_ssin(current.angle.x)), 0.4f);
                    cLib_addCalcAngleS(&field_0xb80, 0x2400, 0x20, 0x400, 0x80);
                }
            }
            field_0xb7c += field_0xb80;
            if (checkCoreFishAttack()) {
                setAction(&daE_OctBg_c::core_fish_attack);
            } else if (dComIfGp_event_runCheck()) {
                setAction(&daE_OctBg_c::swim);
            } else if (field_0xbaf == 0) {
                setChasePos(field_0x660, cM_rndF(l_HIO.field_0x10));
                field_0xbaf = cM_rndFX(20.0f) + 100.0f;
            }
        }
    }
}

void daE_OctBg_c::normal_attack() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (field_0xb70 == 0) {
        field_0xb74 = true;
        unk_bss_7A++;
        field_0xbae = cM_rndFX(20.0f) + 60.0f;
        field_0xb70++;
    } else if (field_0xb70 == -1) {
        field_0xb74 = false;
        unk_bss_7A--;
    } else if (field_0xb70 == 1) {
        cXyz acStack_34;
        acStack_34 = player->current.pos;
        if (dComIfGp_checkPlayerStatus0(0, 0x100000)) {
            acStack_34.y -= 60.0f;
        } else {
            acStack_34.y += 80.0f;
        }
        field_0xb96 = cLib_targetAngleY(&current.pos, &acStack_34);
        field_0xb8c = field_0xb96 - current.angle.y;
        cLib_chaseAngleS(&current.angle.y, field_0xb96, 0x400);
        cLib_chaseAngleS(&current.angle.x, -cLib_targetAngleX(&current.pos, &acStack_34), 0x400);
        cLib_chaseF(&speedF, cM_scos(current.angle.x) * 0.1f, 0.2f);
        cLib_chaseF(&speed.y, cM_ssin(current.angle.x) * -0.1f, 0.2f);
        field_0xb8c = fopAcM_searchPlayerAngleY(this) - current.angle.y;
        cLib_addCalcAngleS(&field_0xb80, 0x400, 0x20, 0x400, 0x80);
        field_0xb7c += field_0xb80;
        if (field_0xbae == 0) {
            field_0xbae = 120;
            field_0xb70++;
        }
    } else if (field_0xb70 == 2) {
        cXyz cStack_40;
        cStack_40 = player->current.pos;
        if (dComIfGp_checkPlayerStatus0(0, 0x100000)) {
            cStack_40.y -= 60.0f;
        } else {
            cStack_40.y += 80.0f;
        }
        field_0xb96 = cLib_targetAngleY(&current.pos, &cStack_40);
        field_0xb8c = field_0xb96 - current.angle.y;
        cLib_chaseAngleS(&current.angle.y, field_0xb96, 0x400);
        cLib_chaseAngleS(&current.angle.x, -cLib_targetAngleX(&current.pos, &cStack_40), 0x400);
        cLib_chaseF(&speedF, cM_scos(current.angle.x) * 8.0f,
                                 0.2f);
        cLib_chaseF(&speed.y, (cM_ssin(current.angle.x) * -8.0f),
                                 0.2f);
        field_0xb8c = fopAcM_searchPlayerAngleY(this) - current.angle.y;
        cLib_addCalcAngleS(&field_0xb80, 0x2400, 0x20, 0x400, 0x80);
        field_0xb7c = field_0xb7c + field_0xb80;
        if (home.pos.absXZ(player->current.pos) >= l_HIO.field_0x08 && player->current.pos.y >= l_HIO.field_0x0c) {
            setAction(&daE_OctBg_c::swim);
        } else {
            if (dComIfGp_event_runCheck() || field_0xbae == 0) {
                setAction(&daE_OctBg_c::swim);
            }
        }
    }
}

void daE_OctBg_c::core_fish_attack() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (field_0xb70 == 0) {
        field_0xb74 = true;
        field_0xbb0 = cM_rndF(50.0f) + 230.0f;
        setChasePos(field_0x660, cM_rndF(800.0f));
        field_0xbaf = cM_rndFX(20.0f) + 40.0f;
        field_0xb70++;
    } else if (field_0xb70 == -1) {
        field_0xb74 = false;
    } else if (field_0xb70 == 1) {
        cXyz cStack_50;
        cXyz cStack_5c;
        cStack_5c = player->current.pos;
        if (dComIfGp_checkPlayerStatus0(0, 0x100000)) {
            cStack_5c.y -= 60.0f;
        } else {
            cStack_5c.y += 80.0f;
        }
        cStack_50 = cStack_5c + field_0x660;
        field_0xb96 = cLib_targetAngleY(&current.pos, &cStack_50);
        field_0xb8c = field_0xb96 - current.angle.y;
        cLib_chaseAngleS(&current.angle.y, field_0xb96, 0x200);
        cLib_chaseAngleS(&current.angle.x, -cLib_targetAngleX(&current.pos, &cStack_50), 0x200);
        cLib_chaseF(&speedF, 14.0f * cM_scos(current.angle.x), 0.4f);
        cLib_chaseF(&speed.y, -14.0f * cM_ssin(current.angle.x), 0.4f);
        cLib_addCalcAngleS(&field_0xb80, 0x3000, 0x20, 0x400, 0x80);
        field_0xb7c += field_0xb80;
        if (field_0xbb0 == 0) {
            setAction(&daE_OctBg_c::chase_core);
        } else if (field_0xbaf == 0) {
            cXyz cStack_68(current.pos);
            f32 in_f31;
            cStack_68.y += 200.0f;
            if (fopAcM_gc_c::gndCheck(&cStack_68)) {
                f32 groundY = fopAcM_gc_c::getGroundY();
                if (current.pos.y < groundY + 200.0f) {
                    if (cStack_5c.y < groundY + 200.0f) {
                        in_f31 = cM_rndF(50.0f) + 20.0f;
                        field_0xbaf = cM_rndFX(20.0f) + 40.0f;
                    } else if (current.pos.abs(cStack_5c) < 400.0f) {
                        in_f31 = cM_rndF(50.0f) + 20.0f;
                        field_0xbaf = cM_rndFX(20.0f) + 40.0f;
                    } else {
                        in_f31 = cM_rndF(400.0f) + 80.0f;
                        field_0xbaf = cM_rndFX(80.0f) + 100.0f;
                    }
                }
            } else if (current.pos.abs(cStack_5c) < 400.0f) {
                in_f31 = cM_rndF(50.0f) + 20.0f;
                field_0xbaf = cM_rndFX(20.0f) + 40.0f;
            } else {
                in_f31 = cM_rndF(400.0f) + 80.0f;
                field_0xbaf = cM_rndFX(80.0f) + 100.0f;
            }
            setChasePos(field_0x660, in_f31);
        }
    }
}

void daE_OctBg_c::back_swim() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (field_0xb70 == 0) {
        field_0xbae = 48;
        field_0xb80 = 0x4000;
        field_0xb82 = 0x1000;
        speedF = -10.0f;
        speed.y = 10.0f;
        field_0xb70++;
    } else if (field_0xb70 != -1) {
        cXyz cStack_30;
        cLib_addCalcAngleS(&field_0xb80, 0x1000, 8, 0x400, 0x100);
        cLib_addCalcAngleS(&current.angle.y, fopAcM_searchPlayerAngleY(this), 0x10, 0x400, 0x100);
        cStack_30 = player->current.pos;
        cStack_30.y += cLib_addCalcAngleS(&current.angle.x,
                                          -cLib_targetAngleX(&current.pos, &player->current.pos),
                                          0x10, 0x400, 0x100);
        cLib_chaseF(&speedF, cM_scos(current.angle.x) * 0.1f, 0.4f);
        cLib_chaseF(&speed.y, cM_ssin(current.angle.x) * 0.1f, 0.4f);
        if (field_0xbae == 0) {
            if (field_0xbac != 0) {
                setAction(&daE_OctBg_c::chase_core);
            } else {
                setAction(&daE_OctBg_c::swim);
            }
        }
    }
}

bool daE_OctBg_c::setBombCarry(int param_1) {
    fopAc_ac_c* actor = NULL;
    if (param_1 == 0) {
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
        if (fopAcM_checkCarryNow(this) != 0) {
            current.angle.x = 0;
            actor =
                dBomb_c::createEnemyWaterBomb(&current.pos, &current.angle, fopAcM_GetRoomNo(this));
        }
    } else {
        current.angle.x = 0;
        actor = dBomb_c::createEnemyWaterBombHookshot(&current.pos, &current.angle,
                                                      fopAcM_GetRoomNo(this));
    }
    if (actor != 0) {
        fopAcM_delete(this);
        return true;
    }
    return false;
}

void daE_OctBg_c::hook() {
    if (field_0xb70 == 0) {
        field_0x9f8.OffAtSetBit();
        field_0xb70++;
    } else if (field_0xb70 == -1) {
        field_0x9f8.OnAtSetBit();
    } else {
        setBombCarry(1);
    }
}

void daE_OctBg_c::damage() {
    field_0xb90 = 120;
    if (field_0xb70 == 0) {
        J3DAnmTransform* transform = (J3DAnmTransform*)dComIfG_getObjectRes("E_bg", 6);
        field_0x5b4->setAnm(transform, 0, 3.0f, 1.0f, 0.0f, -1.0f);
        field_0xb82 = 0;
        field_0xb8c = 0;
        field_0x8c0.OffTgSetBit();
        field_0x9f8.OffAtSetBit();
        attention_info.flags = 0;
        current.angle.y = field_0xb30.mHitDirection.y + 0x8000;
        speedF = 10.0f;
        field_0xb96 = 0x1000;
        speed.y = 0.0f;
        field_0xb70++;
    } else if (field_0xb70 != -1) {
        current.angle.y += field_0xb96;
        current.angle.x += field_0xb96;
        if (field_0x6ac.ChkGroundHit()) {
            cLib_chaseAngleS(&field_0xb96, 0, 0x80);
        } else {
            cLib_chaseAngleS(&field_0xb96, 0x100, 0x60);
        }
        cLib_chaseF(&speedF, 0.0f, 0.2f);
        if (speedF < 5.0f) {
            gravity = -0.1f;
            maxFallSpeed = -3.0f;
        }
        if (field_0x5b4->isStop()) {
            setAction(&daE_OctBg_c::bomb_wait);
        } else {
            field_0x5b8->play();
        }
    }
}

void daE_OctBg_c::bomb_wait() {
    if (field_0xb70 == 0) {
        J3DAnmTransform* transform = (J3DAnmTransform*)dComIfG_getObjectRes("E_bg", 5);
        field_0x5b4->setAnm(transform, 2, 3.0f, 1.0f, 0.0f,
                                 -1.0f);
        field_0xb82 = 0;
        field_0xb8c = 0;
        field_0xb70++;
    } else if (field_0xb70 != -1) {
        if (field_0xb70 == 1) {
            field_0xb90 = 120;
            current.angle.y += field_0xb96;
            current.angle.x += field_0xb96;
            if (field_0x6ac.ChkGroundHit()) {
                field_0xb70++;
                speed.y = 1.0f;
            } else {
                cLib_chaseAngleS(&field_0xb96, 0x100, 0x80);
            }
            if (speedF < 5.0f) {
                gravity = -0.1f;
                maxFallSpeed = -3.0f;
            }
            cLib_chaseF(&speedF, 0.0f, 0.2f);
        } else if (field_0xb70 == 2) {
            current.angle.y += field_0xb96;
            if (field_0x6ac.ChkGroundHit()) {
                if (field_0xbae != 0) {
                    cLib_chaseAngleS(&current.angle.x, -0x8000, 0x80);
                }
                cLib_chaseAngleS(&field_0xb96, 0, 0x80);
                if (setBombCarry(0) != 0) {
                    return;
                }
            } else {
                field_0xbae = 10;
                cLib_chaseAngleS(&field_0xb96, 0x100, 0x80);
                cLib_chaseAngleS(&current.angle.x, -0x8000, 0x80);
            }
            if (speedF < 5.0f) {
                gravity = -0.1f;
                maxFallSpeed = -3.0f;
            }
            cLib_chaseF(&speedF, 0.0, 0.2f);
        }
    }
}

void daE_OctBg_c::damage_check() {
    bool bVar1;
    field_0x884.Move();
    if (field_0xbad == 0) {
        bVar1 = true;
        if ((checkAction(&daE_OctBg_c::core_fish_attack) ||
             checkAction(&daE_OctBg_c::normal_attack)) &&
            field_0x9f8.ChkAtHit())
        {
            if (field_0x9f8.ChkAtShieldHit()) {
                if (daPy_getPlayerActorClass()->getCutType() == 41) {
                    setAction(&daE_OctBg_c::damage);
                    return;
                }
            }
            setAction(&daE_OctBg_c::back_swim);
        } else {
            if (field_0x8c0.ChkTgHit()) {
                field_0xb30.mpCollider = field_0x8c0.GetTgHitObj();
                cc_at_check(this, &field_0xb30);
                if (field_0xb30.mpCollider->ChkAtType(0xd8000000)) {
                    field_0xbad = 20;
                } else {
                    field_0xbad = 10;
                }
                if (field_0xb30.mHitStatus != 0) {
                    field_0xbad = 10;
                }
                dScnPly_c::setPauseTimer(0);
                field_0x8c0.ClrTgHit();
                field_0x5bc.startCreatureVoice(Z2SE_EN_BG_V_DAMAGE, -1);
                if (field_0xb30.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
                    setAction(&daE_OctBg_c::hook);
                } else {
                    setAction(&daE_OctBg_c::damage);
                }
            }
        }
    }
}

int daE_OctBg_c::execute() {
    if (cDmrNowMidnaTalk()) {
        return 1;
    }
    action();
    return 1;
}

static int daE_OctBg_Execute(daE_OctBg_c* i_this) {
    return i_this->execute();
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daE_OctBg_c*>(i_this)->CreateHeap();
}

int daE_OctBg_c::ctrlJoint(J3DJoint* param_1, J3DModel* param_2) {
    int jntNo = param_1->getJntNo();
    mDoMtx_stack_c::copy(param_2->getAnmMtx(jntNo));
    if (jntNo != 0) {
        if (jntNo == 1) {
            if (field_0xb75 == 0) {
                mDoMtx_stack_c::YrotM(field_0xb84[3]);
            } else {
                mDoMtx_stack_c::YrotM(field_0xb84[3] - field_0xb84[0]);
            }
        } else {
            mDoMtx_stack_c::YrotM(field_0xb84[jntNo - 2]);
        }
    }
    param_2->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

int daE_OctBg_c::JointCallBack(J3DJoint* param_0, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daE_OctBg_c* octBg = (daE_OctBg_c*)model->getUserArea();
        if (octBg != NULL) {
            octBg->ctrlJoint(param_0, model);
        }
    }
    return 1;
}

int daE_OctBg_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_bg", 10);
    JUT_ASSERT(1341, modelData != NULL);
    field_0x5b4 = new mDoExt_McaMorfSO(
            modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_bg", 7), 0, 1.0f, 0, -1, &field_0x5bc,
            0x80000, 0x11000084);
    if (field_0x5b4 == NULL || field_0x5b4->getModel() == NULL) {
        return 0;
    }
    J3DModel* model = field_0x5b4->getModel();
    model->setUserArea((uintptr_t)this);
    for (u16 i = 1; i <  model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
    }
    field_0x5b8 = new mDoExt_brkAnm();
    if (field_0x5b8 == NULL) {
        return 0;
    }
    if (!field_0x5b8->init(model->getModelData(), (J3DAnmTevRegKey*)dComIfG_getObjectRes("E_bg", 13), 1, 0, 1.0f, 0, -1)) {
        return 0;
    }
    return 1;
}

int daE_OctBg_c::create() {
    fopAcM_ct(this, daE_OctBg_c);

    int rv = dComIfG_resLoad(&mPhase,  "E_bg");
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("///////////// E_OctBg ////////////////\n");
        if (fopAcM_entrySolidHeap(this, useHeapInit, 0x10e0) == 0) {
            return cPhs_ERROR_e;
        }
        OS_REPORT("//////////// E_OctBg SET OK //////////\n");
        if (struct_8073A13D == 0) {
            struct_8073A13D = 1;
            field_0xbb3 = 1;
            l_HIO.field_0x04 = -1;
        }
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        onWolfNoLock();
        fopAcM_SetMtx(this, field_0x5b4->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -50.0f, -50.0f, -50.0f);
        fopAcM_SetMax(this, 50.0f, 50.0f, 50.0f);
        field_0x6ac.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                            &field_0x66c, fopAcM_GetSpeed_p(this), NULL, NULL);
        field_0x66c.SetWall(l_HIO.field_0x18 * 10.0f,
                                l_HIO.field_0x18 * 20.0f);
        health = 10;
        field_0x560 = 10;
        field_0x884.Init(100, 0, this);
        field_0x8c0.Set(cc_obg_src);
        field_0x8c0.SetStts(&field_0x884);
        field_0x9f8.Set(cc_obg_at_src);
        field_0x9f8.SetStts(&field_0x884);
        field_0x5bc.init(&current.pos, &eyePos, 3, 1);
        field_0x5bc.setEnemyName("E_octbg");
        field_0xb30.mpSound = &field_0x5bc;
        field_0xb30.mPowerType = 1;
        gravity = 0.0f;
        attention_info.distances[fopAc_attn_BATTLE_e] = 4;
        field_0xb74 = false;
        field_0xbac = fopAcM_GetID(this) & 1;
        unk_bss_78++;
        if (field_0xbac != 0) {
            unk_bss_79++;
        }
        setAction(&daE_OctBg_c::born_swim);
        daE_OctBg_Execute(this);

    }
    return rv;
}

static int daE_OctBg_Create(daE_OctBg_c* i_this) {
    return i_this->create();
}

int daE_OctBg_c::draw() {
    J3DModel* model = field_0x5b4->getModel();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    field_0x5b8->entry(model->getModelData());
    if ((checkAction(&daE_OctBg_c::damage) || checkAction(&daE_OctBg_c::bomb_wait)) && field_0xb90 != 0) {
        J3DGXColorS10 local_68;
        local_68.r = 32.0f - fabsf(cM_scos(field_0xb94) * 30.0f);
        local_68.g = 0;
        local_68.b = 0;
        local_68.a = 0;
        model->getModelData()->getMaterialNodePointer(0)->setTevColor(0, &local_68);
        model->getModelData()->getMaterialNodePointer(1)->setTevColor(0, &local_68);
    }
    field_0x5b4->entryDL();
    cXyz cStack_48;
    cStack_48.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
    field_0xb78 = dComIfGd_setShadow(field_0xb78, 1, model, &cStack_48, 200.0f, 0.0f, current.pos.y, field_0x6ac.GetGroundH(), field_0x6ac.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static int daE_OctBg_Draw(daE_OctBg_c* i_this) {
    return i_this->draw();
}

static int daE_OctBg_IsDelete(daE_OctBg_c* i_this) {
    unk_bss_78--;
    if (i_this->getChaseFlag() != 0) {
        unk_bss_79--;
        if (unk_bss_79 < unk_bss_78 >> 2) {
            unk_bss_7B = 1;
        }
    }
    if (i_this->checkNormalAttk()) {
        unk_bss_7A--;
    }
    return 1;
}

static actor_method_class l_daE_OctBg_Method = {
    (process_method_func)daE_OctBg_Create,
    (process_method_func)daE_OctBg_Delete,
    (process_method_func)daE_OctBg_Execute,
    (process_method_func)daE_OctBg_IsDelete,
    (process_method_func)daE_OctBg_Draw,
};

actor_process_profile_definition g_profile_E_OctBg = {
    fpcLy_CURRENT_e,          // mLayerID
    7,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_E_OctBg,             // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daE_OctBg_c),      // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    0xCD,                     // mPriority
    &l_daE_OctBg_Method,      // sub_method
    0xC4100,                  // mStatus
    fopAc_ENEMY_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e,   // cullType
};

AUDIO_INSTANCES;
