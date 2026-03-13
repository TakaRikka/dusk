/*
 * d_a_obj_enemy_create.cpp
 * Enemy spawner
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_enemy_create.h"
#include "d/actor/d_a_player.h"
#include "f_op/f_op_actor_mng.h"

static int daObjE_CREATE_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjE_CREATE_c*>(i_this)->create();
}

static int daObjE_CREATE_Delete(daObjE_CREATE_c* i_this) {
    i_this->Delete();
    return 1;
}

void daObjE_CREATE_c::NormalAction() {
    cXyz offsetPos(0.0f, 0.0f, 100.0f);

    switch (mActionMode) {
    case MODE_CREATE:
        if (mActivateSw != 0xFF && dComIfGs_isSwitch(mActivateSw, fopAcM_GetRoomNo(this))) {
            int enemyNum = mEnemyNum;

            for (int i = 0; i < enemyNum; i++) {
                s16 angle = shape_angle.y + (62000.0f / enemyNum) * i;

                // this cLib_offsetPos call is useless since the result is stored in `offsetPos`
                // and `offsetPos` is never used afterward.
                // this was probably supposed to be:
                // "cLib_offsetPos(&current.pos, &current.pos, angle, &offsetPos)"
                cLib_offsetPos(&offsetPos, &current.pos, angle, &offsetPos);

                mActorList[i] =
                    fopAcM_createChild(mEnemyProcName, fopAcM_GetID(this), mEnemyParams,
                                       &current.pos, fopAcM_GetRoomNo(this), &mEnemyAngle,
                                       NULL, -1, NULL);
            }
            mActionMode++;
            mKillCheckDelayTimer = 50;
        }
        break;
    case MODE_WAIT:
        mKillCheckDelayTimer--;
        if (mKillCheckDelayTimer <= 0) {
            mKillCheckDelayTimer = 0;
        }

        if (mKillCheckDelayTimer == 0) {
            mActionMode++;
        }
        break;
    case MODE_KILL_CHECK: {
        int killed_num;
        int enemyNum = mEnemyNum;

        killed_num = 0;
        for (int i = 0; i < enemyNum; i++) {
            if (fopAcM_SearchByID(mActorList[i]) == NULL) {
                mActorList[i] = 0xFFFFFFFF;
                killed_num++;
            }
        }

        if (killed_num == enemyNum) {
            mActionMode++;
            if (mDeactivateSw != 0xFF && !dComIfGs_isSwitch(mDeactivateSw, fopAcM_GetRoomNo(this)))
            {
                dComIfGs_onSwitch(mDeactivateSw, fopAcM_GetRoomNo(this));
            }

            mKillCheckDelayTimer = 100;
        }
        break;
    }
    case MODE_END:
        break;
    }
}

/**
 * Moves the enemy spawner position to player position
 */
void daObjE_CREATE_c::SearchAction() {
    current.pos = fopAcM_GetPosition(daPy_getPlayerActorClass());
    NormalAction();
}

/**
 * Starts the enemy spawn loop over again when all enemies are killed.
 * If a loop end switch is specified and activated, the loop will end
 */
void daObjE_CREATE_c::LoopAction() {
    NormalAction();

    if (mActionMode == MODE_END) {
        mKillCheckDelayTimer--;
        if (mKillCheckDelayTimer <= 0) {
            mKillCheckDelayTimer = 0;
        }

        if (mKillCheckDelayTimer == 0) {
            mActionMode = MODE_CREATE;
        }
    }

    if (mEndLoopSw != 0xFF && dComIfGs_isSwitch(mEndLoopSw, fopAcM_GetRoomNo(this))) {
        mAction = ACT_NORMAL;
        mActionMode = MODE_KILL_CHECK;
    }
}

void daObjE_CREATE_c::Action() {
    switch (mAction) {
    case ACT_NORMAL:
        NormalAction();
        break;
    case ACT_SEARCH:
        SearchAction();
        break;
    case ACT_LOOP:
        LoopAction();
        break;
    }
}

int daObjE_CREATE_c::Execute() {
    Action();
    return 1;
}

int daObjE_CREATE_c::Delete() {
    if (mActorList != NULL) {
        JKR_DELETE_ARRAY(mActorList);
    }

    return 1;
}

static int daObjE_CREATE_Draw(daObjE_CREATE_c* i_this) {
    return 1;
}

static int daObjE_CREATE_Execute(daObjE_CREATE_c* i_this) {
    return i_this->Execute();
}

void daObjE_CREATE_c::NameChk() {
    int enemyType = fopAcM_GetParam(this) & 0xF;
    mEnemyNum = (fopAcM_GetParam(this) >> 4) & 0xF;
    mEnemyParams = 0xFFFFFFFF;
    mEnemyAngle = shape_angle;

    switch (enemyType) {
    case ENEMY_NONE:
        break;
    case ENEMY_KEESE:
        mEnemyProcName = fpcNm_E_BA_e;
        break;
    case ENEMY_RAT:
        mEnemyProcName = fpcNm_E_MS_e;
        break;
    case ENEMY_CHU:
        mEnemyProcName = fpcNm_E_SM2_e;
        mEnemyParams = 0xFFFFFF1F;
        break;
    case ENEMY_CHU2:
        mEnemyProcName = fpcNm_E_SM2_e;
        mEnemyParams = 0xFFFFFF3F;
        break;
    case ENEMY_BABY_GOHMA:
        mEnemyProcName = fpcNm_E_GM_e;
        mEnemyParams = 0x000AFF02;
        break;
    case ENEMY_BUBBLE:
        mEnemyProcName = fpcNm_E_BU_e;
        break;
    case ENEMY_BOKOBLIN:
        mEnemyProcName = fpcNm_E_OC_e;
        break;
    case ENEMY_LIZALFOS:
        mEnemyProcName = fpcNm_E_DN_e;
        break;
    case ENEMY_SHADOW_BEAST:
        mEnemyProcName = fpcNm_E_S1_e;

        if (mEnemyNum == 1) {
            mEnemyParams = 0xFFFFFEF0;
        } else {
            mEnemyParams = (mEnemyNum << 4) | 0xFFFFFE00;
        }
        break;
    case ENEMY_SHADOW_VERMIN:
        mEnemyProcName = fpcNm_E_YG_e;
        mEnemyParams = 0xFFFFFF00;
        break;
    case ENEMY_SHADOW_KEESE:
        mEnemyProcName = fpcNm_E_YK_e;
        mEnemyParams = 0xFFFFFF01;
        break;
    case ENEMY_BOKOBLIN2:
        mEnemyProcName = fpcNm_E_OC_e;
        mEnemyParams = 0xFFFF01FF;
        mEnemyAngle.set(0x100, shape_angle.y, shape_angle.z);
        break;
    }
}

int daObjE_CREATE_c::create() {
    fopAcM_ct(this, daObjE_CREATE_c);

    NameChk();
    u32 enemyType = fopAcM_GetParam(this) & 0xF;

    mAction = current.angle.x & 0xF;
    if (mAction == ACT_DEFAULT) {
        mAction = ACT_NORMAL;
    }

    current.angle.x = 0;

    if (enemyType == 0 || enemyType == 0xFF) {
        return cPhs_ERROR_e;
    }

    mEndLoopSw = fopAcM_GetParam(this) >> 0x18;
    if (dComIfGs_isSwitch(mEndLoopSw, fopAcM_GetRoomNo(this))) {
        return cPhs_ERROR_e;
    }

    mActivateSw = fopAcM_GetParam(this) >> 8;
    mDeactivateSw = fopAcM_GetParam(this) >> 0x10;
    mActorList = JKR_NEW_ARRAY(u32, mEnemyNum);

    if (mActorList == NULL) {
        return cPhs_ERROR_e;
    }

    return cPhs_COMPLEATE_e;
}

static int daObjE_CREATE_IsDelete(daObjE_CREATE_c* i_this) {
    return 1;
}

static actor_method_class l_daObjE_CREATE_Method = {
    (process_method_func)daObjE_CREATE_Create,  (process_method_func)daObjE_CREATE_Delete,
    (process_method_func)daObjE_CREATE_Execute, (process_method_func)daObjE_CREATE_IsDelete,
    (process_method_func)daObjE_CREATE_Draw,
};

actor_process_profile_definition g_profile_Obj_E_CREATE = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 3,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Obj_E_CREATE_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjE_CREATE_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Obj_E_CREATE_e,
    /* Actor SubMtd */ &l_daObjE_CREATE_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
