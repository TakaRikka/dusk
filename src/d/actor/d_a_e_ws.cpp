/**
 * @file d_a_e_ws.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_ws.h"
#include "d/actor/d_a_obj_carry.h"
#include <cmath>
#include "f_op/f_op_actor_enemy.h"

#define PLAYER_NOT_FOUND  0
#define PLAYER_TARGET   1
#define PLAYER_NEAR     2

class daE_WS_HIO_c : public JORReflexible {
public:
    daE_WS_HIO_c();
    virtual ~daE_WS_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 base_size;
    /* 0x0C */ f32 move_range;
    /* 0x10 */ f32 search_range;
    /* 0x14 */ f32 search_y_range;
    /* 0x18 */ f32 dist_to_ground;
    /* 0x1C */ f32 search_angle;
    /* 0x20 */ f32 attack_speed;
    /* 0x24 */ u8 debug_ON;
};


namespace {
static dCcD_SrcSph cc_ws_src = {
    {
        {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xD}, {0xD8F9FDF5, 0x3}, 0x75}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

static dCcD_SrcSph cc_bokkuri_src = {
    {
        {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0x0}, {0x20008, 0x3}, 0x0}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};
}

daE_WS_HIO_c::daE_WS_HIO_c() {
    id = -1;
    base_size = 0.8f;
    move_range = 50.0f;
    search_range = 300.0f;
    search_y_range = 200.0f;
    dist_to_ground = 270.0f;
    debug_ON = false;
    search_angle = 18200;
    attack_speed = 10.0f;
}

#if DEBUG
void daE_WS_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("スタルウォーーーーーーーール", 0x80000001);
    ctx->genSlider("基本サイズ", &base_size, 0.0f, 5.0f);
    ctx->genSlider("サーチ角度", &search_angle, 0.0f, 30000.0f);
    ctx->genSlider("攻撃速度", &attack_speed, 0.0f, 100.0f);
    ctx->genSlider("移動範囲", &move_range, 0.0f, 1000.0f);
    ctx->genSlider("サーチ範囲", &search_range, 0.0f, 1000.0f);
    ctx->genSlider("サーチY上下範囲", &search_y_range, 0.0f, 1000.0f);
    ctx->genSlider("地面までの距離", &dist_to_ground, 0.0f, 1000.0f);
    ctx->genCheckBox("デバック表示", &debug_ON, 0x1);
}
#endif

int daE_WS_c::draw() {
    J3DModel* model = mAnm_p->getModel();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);

    if (mDownColor) {
        J3DModelData* modelData_p = model->getModelData();
        for (u16 i = 0; i < modelData_p->getMaterialNum(); i++) {
            J3DMaterial* material_p = modelData_p->getMaterialNodePointer(i);
            material_p->getTevColor(0)->r = mDownColor;
            material_p->getTevColor(0)->g = mDownColor;
            material_p->getTevColor(0)->b = mDownColor;
        }
    }

    mAnm_p->entryDL();

    cXyz sp8;
    sp8.set(current.pos.x, 100.0f + current.pos.y, current.pos.z);
    mShadowId = dComIfGd_setShadow(mShadowId, 1, model, &sp8, 400.0f, 0.0f, current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static int daE_WS_Draw(daE_WS_c* i_this) {
    return i_this->draw();
}

void daE_WS_c::setBck(int i_anm, u8 i_mode, f32 i_morf, f32 i_speed) {
    mAnm_p->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_WS", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f);
}

void daE_WS_c::setFootSound() {
    if (mAnm_p->getAnm() == dComIfG_getObjectRes("E_WS", 7)) {
        if (mAnm_p->checkFrame(0.0f)  ||
            mAnm_p->checkFrame(4.5f)  ||
            mAnm_p->checkFrame(7.5f)  ||
            mAnm_p->checkFrame(9.0f)  ||
            mAnm_p->checkFrame(13.5f) ||
            mAnm_p->checkFrame(16.0f) ||
            mAnm_p->checkFrame(19.0f) ||
            mAnm_p->checkFrame(23.5f) ||
            mAnm_p->checkFrame(25.0f) ||
            mAnm_p->checkFrame(28.0f) ||
            mAnm_p->checkFrame(32.5f) ||
            mAnm_p->checkFrame(36.0f) ||
            mAnm_p->checkFrame(39.5f))
        {
            mSound.startCreatureSound(Z2SE_EN_WS_FOOTNOTE, 0, -1);
        }
    } else if (mAnm_p->getAnm() == dComIfG_getObjectRes("E_WS", 8)) {
        if (mAnm_p->checkFrame(0.5f)  ||
            mAnm_p->checkFrame(6.0f)  ||
            mAnm_p->checkFrame(11.0f) ||
            mAnm_p->checkFrame(16.0f) ||
            mAnm_p->checkFrame(21.0f) ||
            mAnm_p->checkFrame(26.5f) ||
            mAnm_p->checkFrame(31.0f))
        {
            mSound.startCreatureSound(Z2SE_EN_WS_FOOTNOTE, 0, -1);
        }
    }
}

void daE_WS_c::setActionMode(int i_action) {
    if (mAction != i_action) {
        mAction = i_action;
        mMode = 0;
    }
}

f32 daE_WS_c::calcTargetDist(cXyz i_basePos, cXyz i_targetPos) {
    return i_basePos.abs(i_targetPos);
}

s16 daE_WS_c::calcTargetAngle(cXyz i_basePos, cXyz i_targetPos) {
    cXyz mae;
    cXyz ato = i_targetPos - i_basePos;

    mDoMtx_stack_c::XrotS(-mTargetWallAngle.x);
    mDoMtx_stack_c::YrotM(-mTargetWallAngle.y);
    mDoMtx_stack_c::multVec(&ato, &mae);
    return cM_atan2s(mae.x, mae.z);
}

static u8 hio_set;

static daE_WS_HIO_c l_HIO;

bool daE_WS_c::checkInSearchRange(cXyz i_basePos, cXyz i_targetPos) {
    if (calcTargetDist(i_basePos, i_targetPos) < l_HIO.search_range && std::abs(i_basePos.y - i_targetPos.y) < l_HIO.search_y_range) {
        return true;
    }

    return false;
}

int daE_WS_c::checkPlayerPos() {
    cXyz player_pos;
    mDoMtx_stack_c::copy(daPy_getLinkPlayerActorClass()->getModelJointMtx(0));
    mDoMtx_stack_c::multVecZero(&player_pos);

    if ((daPy_getPlayerActorClass()->checkClimbMove() ||
        dComIfGp_checkPlayerStatus1(0, 0x2000000) ||
        dComIfGp_checkPlayerStatus1(0, 0x10000) ||
        calcTargetDist(current.pos, player_pos) < 150.0f) &&
        checkInSearchRange(player_pos, mHomePos) && checkInSearchRange(current.pos, mHomePos))
    {
        dBgS_GndChk gndchk;
        cXyz gndpos;

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(mWallAngle);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::transM(0.0f, 100.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&gndpos);

        gndchk.SetPos(&gndpos);
        gndpos.y = dComIfG_Bgsp().GroundCross(&gndchk);
        if (current.pos.y - gndpos.y > l_HIO.dist_to_ground) {
            // Return 1 if walltula is looking towards player
            if (cLib_distanceAngleS(shape_angle.y, calcTargetAngle(current.pos, player_pos)) < l_HIO.search_angle) {
                return PLAYER_TARGET;
            }

            // otherwise return 2 if player is near the walltula
            if (calcTargetDist(current.pos, player_pos) < 150.0f) {
                return PLAYER_NEAR;
            }
        }
    }

    return PLAYER_NOT_FOUND;
}

bool daE_WS_c::checkAttackEnd() {
    cXyz player_pos;
    mDoMtx_stack_c::copy(daPy_getLinkPlayerActorClass()->getModelJointMtx(0));
    mDoMtx_stack_c::multVecZero(&player_pos);

    BOOL checkPlayerNear = FALSE;
    if (
        daPy_getPlayerActorClass()->checkClimbMove() ||
        dComIfGp_checkPlayerStatus1(0, 0x02000000) ||
        dComIfGp_checkPlayerStatus1(0, 0x10000) ||
        calcTargetDist(current.pos, player_pos) < 200.0f
    ) {
        checkPlayerNear = TRUE;
    }
    if (!checkPlayerNear ||
        !checkInSearchRange(current.pos, mHomePos) ||
        checkBeforeBg(shape_angle.y)
    )
    {
        setActionMode(ACTION_WAIT_e);
        speedF = 0.0f;
        return true;
    }

    return false;
}

void daE_WS_c::executeWait() {
    int playerCheck = checkPlayerPos();
    if (playerCheck == PLAYER_TARGET) {
        setActionMode(ACTION_ATTACK_e);
        return;
    }

    if (playerCheck == PLAYER_NEAR && mMode != 3 && mMode != 4) {
        mMode = 2;
    }

    switch (mMode) {
    case 0:
        mWaitTimer = 50.0f + cM_rndF(50.0f);
        setBck(9, 2, 3.0f, 1.0f);
        mMode = 1;
        /* fallthrough */
    case 1:
        if (mWaitTimer == 0) {
            mMode = 2;
        }
        break;
    case 2:
        speedF = 0.0f;
        mIsReturnHome = 0;
        mTargetAngle = shape_angle.y + cM_rndFX(32768.0f); // random turn up to ±180°

        if (playerCheck == PLAYER_NEAR) {
            mStepAngle = 0x200;
            setBck(8, 2, 3.0f, 2.4f);
        } else {
            if (calcTargetDist(current.pos, mHomePos) >= l_HIO.move_range) {
                mTargetAngle = calcTargetAngle(current.pos, mHomePos);
                mIsReturnHome = 1;
            }
            mStepAngle = 0x100;
            setBck(8, 2, 3.0f, 1.2f);
        }

        mMode = 3;
        /* fallthrough */
    case 3:
        setFootSound();

        if (cLib_chaseAngleS(&shape_angle.y, mTargetAngle, mStepAngle)) {
            mMode = 4;
            mWaitTimer = 10;
            setBck(9, 2, 3.0f, 1.0f);
        }
        break;
    case 4:
        if (mWaitTimer == 0) {
            mMode = 5;
        }
        break;
    case 5:
        mMode = 6;
        speedF = 3.0f;
        mWaitTimer = 20.0f + cM_rndF(10.0f);
        setBck(7, 2, 3.0f, 1.0f);
        /* fallthrough */
    case 6:
        setFootSound();

        if (mIsReturnHome == 0) {
            if (calcTargetDist(current.pos, mHomePos) >= l_HIO.move_range) {
                mWaitTimer = 0;
            }
        }

        if (checkBeforeBg(shape_angle.y)) {
            mWaitTimer = 0;
        }

        if (mWaitTimer == 0) {
            speedF = 0.0f;
            mMode = 0;
        }
        break;
    }

    current.angle.y = shape_angle.y;
}

void daE_WS_c::executeAttack() {
    cXyz player_pos;
    mDoMtx_stack_c::copy(daPy_getLinkPlayerActorClass()->getModelJointMtx(0));
    mDoMtx_stack_c::multVecZero(&player_pos);

    switch (mMode) {
    case 0:
        setBck(8, 2, 3.0f, 3.5f);
        mMode = 1;
        speedF = 0.0f;
        /* fallthrough */
    case 1:
        setFootSound();

        if (checkAttackEnd()) {
            return;
        }

        if (cLib_chaseAngleS(&shape_angle.y, calcTargetAngle(current.pos, player_pos), 0x300)) {
            mMode = 2;
            setBck(10, 2, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_WS_V_YOKOKU, -1);
            mWaitTimer = 10;
        }
        break;
    case 2:
        if (mWaitTimer == 0) {
            speedF = l_HIO.attack_speed * mBodyScale;
            setBck(7, 2, 3.0f, 3.0f);
            mMode = 3;
        }

        if (checkAttackEnd()) {
            return;
        }

        break;
    case 3: {
        setFootSound();
        cLib_chaseAngleS(&shape_angle.y, calcTargetAngle(current.pos, player_pos), 0x400);

        BOOL checkAttackStart = FALSE;
        if (checkBeforeBg(shape_angle.y)) {
            checkAttackStart = TRUE;
        }
        if (mCcSph.ChkAtHit()) {
            cCcD_Obj* hitObj = mCcSph.GetAtHitObj();
            if (fopAcM_GetName(dCc_GetAc(hitObj->GetAc())) == fpcNm_ALINK_e) {
                checkAttackStart = TRUE;
            }
        }
        if (!checkInSearchRange(current.pos, mHomePos)) {
            checkAttackStart = TRUE;
        }
        if (checkAttackStart) {
            mMode = 4;
            speedF = 0.0f;
            setBck(4, 0, 3.0f, 1.0f);
            break;
        }

        if (checkAttackEnd()) {
            return;
        }
        break;
    }
    case 4:
        if (mAnm_p->checkFrame(7.5f)) {
            mSound.startCreatureVoice(Z2SE_EN_WS_V_ATTACK, -1);
        }

        if (mAnm_p->isStop()) {
            setActionMode(ACTION_WAIT_e);
        }
        /* fallthrough */
    }

    current.angle.y = shape_angle.y;
}

void daE_WS_c::executeDown() {
    mInvulnerabilityTimer = 10;

    switch (mMode) {
    case 0:
        mCcSph.OffAtSetBit();
        mCcSph.OffTgSetBit();
        mCcBokkuriSph.OffTgSetBit();

        mMode = 1;
        speed.y = 0.0f;
        speedF = 0.0f;
        setBck(5, 0, 0.0f, 1.0f);

        mSound.startCreatureVoice(Z2SE_EN_WS_V_DAMAGE, -1);
        /* fallthrough */
    case 1:
        if (mAnm_p->isStop()) {
            mBgc.SetWall(0.0f, 4.0f);
            current.angle.y = mTargetWallAngle.y;
            setBck(6, 0, 3.0f, 0.0f);
            mSound.startCreatureVoice(Z2SE_EN_WS_V_DEATH, -1);
            speedF = 5.0f;
            gravity = -3.0f;
            mAcch.SetGroundUpY(25.0f * mBodyScale);
            mMode = 2;
            attention_info.flags = 0;
        }
        return;
    case 2:
        if (abs(shape_angle.y) < 0x4000) {
            cLib_chaseAngleS(&shape_angle.y, 0, 0x200);
            cLib_chaseAngleS(&shape_angle.x, 0x4000, 0x400);
        } else {
            cLib_chaseAngleS(&shape_angle.y, -0x8000, 0x200);
            cLib_chaseAngleS(&shape_angle.x, -0x4000, 0x400);
        }

        if (mAcch.ChkGroundHit()) {
            speedF = 3.0f + cM_rndF(2.0f);
            speed.y = 12.0f;
            mMode = 3;
            mWaitTimer = 30;
            setBck(6, 0, 5.0f, 1.0f);
            mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
        }
        break;
    case 3:
        if (abs(shape_angle.y) < 0x4000) {
            cLib_chaseAngleS(&shape_angle.y, 0, 0x200);
            cLib_chaseAngleS(&shape_angle.x, 0x4000, 0x400);
        } else {
            cLib_chaseAngleS(&shape_angle.y, -0x8000, 0x200);
            cLib_chaseAngleS(&shape_angle.x, -0x4000, 0x400);
        }

        if (mAcch.ChkGroundHit()) {
            mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);

            if (abs(shape_angle.y) < 0x4000) {
                shape_angle.y = 0;
                shape_angle.x = 0x4000;
            } else {
                shape_angle.y = -0x8000;
                shape_angle.x = -0x4000;
            }

            speedF = 0.0f;
            speed.y = 0.0f;
            mMode = 4;
            return;
        }
        break;
    case 4:
        cLib_addCalc2(&mDownColor, -20.0f, 1.0f, 0.4f);

        if (mAnm_p->isStop()) {
            mWaitTimer = 15;
            mMode = 5;
            return;
        }
        break;
    case 5:
        cLib_addCalc2(&mDownColor, -20.0f, 1.0f, 0.4f);

        if (mWaitTimer == 0) {
            fopAcM_delete(this);
            fopAcM_createDisappear(this, &current.pos, 7, 0, 7);

            if (bitSw != 0xFF && !dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(bitSw, fopAcM_GetRoomNo(this));
            }
        }
        break;
    }
}

void daE_WS_c::executeWindDown() {
    mInvulnerabilityTimer = 10;

    switch (mMode) {
    case 0:
        mCcSph.OffAtSetBit();
        mCcSph.OffTgSetBit();
        mCcBokkuriSph.OffTgSetBit();
    
        mBgc.SetWall(0.0f, 4.0f);
        mMode = 1;
        setBck(7, 2, 3.0f, 1.0f);
    
        mSound.startCreatureVoice(Z2SE_EN_WS_V_DAMAGE, -1);
        mWaitTimer = 5;
        mAcch.SetGroundUpY(20.0f);
        attention_info.flags = 0;
        speed.y = 0.0f;
        speedF = 5.0f;
        current.angle.y = mTargetWallAngle.y;
        mStepAngle = -0x800;
        break;
    case 1:
        shape_angle.y += mStepAngle;
        cLib_chaseAngleS(&mWallAngle.y, 0, 0x400);
        cLib_chaseAngleS(&mWallAngle.x, 0, 0x400);
        shape_angle.x += 0x800;
        shape_angle.z += 0x800;
        speed.y = 30.0f;

        if (mWaitTimer == 0) {
            mMode = 2;
            gravity = -3.0f;
        }
        break;
    case 2:
        shape_angle.y += mStepAngle;
        cLib_chaseAngleS(&mWallAngle.y, 0, 0x400);
        cLib_chaseAngleS(&mWallAngle.x, 0, 0x400);
        cLib_chaseAngleS(&shape_angle.x, -0x8000, 0x400);
        cLib_chaseAngleS(&shape_angle.z, 0, 0x400);

        if (mAcch.ChkGroundHit()) {
            speedF = 3.0f + cM_rndF(2.0f);
            speed.y = 12.0f;
            mMode = 3;
            mWaitTimer = 30;
            setBck(6, 0, 5.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_WS_V_DEATH, -1);
            mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
        }
        break;
    case 3:
        shape_angle.y += mStepAngle;
        cLib_chaseAngleS(&mStepAngle, 0, 0x80);
        cLib_chaseAngleS(&shape_angle.x, -0x8000, 0x400);
        cLib_chaseAngleS(&shape_angle.z, 0, 0x400);

        if (mAcch.ChkGroundHit()) {
            mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
            shape_angle.x = -0x8000;
            speedF = 0.0f;
            speed.y = 0.0f;
            mMode = 4;
        }
        break;
    case 4:
        cLib_addCalc2(&mDownColor, -20.0f, 1.0f, 0.4f);
        shape_angle.y += mStepAngle;
        cLib_chaseAngleS(&mStepAngle, 0, 0x80);
        shape_angle.y += mStepAngle;

        if (mAnm_p->isStop()) {
            mWaitTimer = 15;
            mMode = 5;
        }
        break;
    case 5:
        cLib_addCalc2(&mDownColor, -20.0f, 1.0f, 0.4f);
        if (mWaitTimer == 0) {
            fopAcM_delete(this);
            fopAcM_createDisappear(this, &current.pos, 7, 0, 7);

            if (bitSw != 0xFF && !dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(bitSw, fopAcM_GetRoomNo(this));
            }
        }
        break;
    }
}

void daE_WS_c::damage_check() {
    if (mInvulnerabilityTimer == 0) {
        if (mAction == ACTION_DOWN_e || mAction == ACTION_WIND_DOWN_e) {
            return;
        }

        mCcStts.Move();

        if (mCcBokkuriSph.ChkTgHit()) {
            mAtInfo.mpCollider = mCcBokkuriSph.GetTgHitObj();

            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_20000)) {
                dComIfGp_setHitMark(1, this, &eyePos, NULL, NULL, 0);
                dScnPly_c::setPauseTimer(4);
                mSound.startCreatureSound(Z2SE_EN_ST_SHELL_BREAK, 0, -1);
            } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_THROW_OBJ)) {
                fopAc_ac_c* throwobj_p = dCc_GetAc(mAtInfo.mpCollider->GetAc());
                if (fopAcM_GetName(throwobj_p) == fpcNm_Obj_Carry_e && ((daObjCarry_c*)throwobj_p)->getType() == daObjCarry_c::TYPE_BOKKURI) {
                    mSound.startCreatureSound(Z2SE_EN_ST_SHELL_BREAK, 0, -1);
                }
            }
    
            cc_at_check(this, &mAtInfo);
            setActionMode(ACTION_DOWN_e);
            return;
        }

        if (mCcSph.ChkTgHit()) {
            mAtInfo.mpCollider = mCcSph.GetTgHitObj();
            cc_at_check(this, &mAtInfo);

            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_WOLF_ATTACK | AT_TYPE_WOLF_CUT_TURN | AT_TYPE_10000000 | AT_TYPE_MIDNA_LOCK)) {
                mInvulnerabilityTimer = 20;
            } else {
                mInvulnerabilityTimer = 10;
            }

            if (mAtInfo.mAttackPower <= 1) {
                mInvulnerabilityTimer = KREG_S(8) + 10;
            }

            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                setActionMode(ACTION_WIND_DOWN_e);
            } else {
                setActionMode(ACTION_DOWN_e);
            }
        }
    }
}

void daE_WS_c::action() {
    attention_info.distances[fopAc_attn_BATTLE_e] = 46;
    damage_check();
    field_0x566 = 0;

    switch (mAction) {
    case ACTION_WAIT_e:
        executeWait();
        break;
    case ACTION_ATTACK_e:
        field_0x566 = 1;
        executeAttack();
        break;
    case ACTION_DOWN_e:
        executeDown();
        break;
    case ACTION_WIND_DOWN_e:
        executeWindDown();
        break;
    }

    mSound.setLinkSearch(field_0x566);

    if (mAction != ACTION_DOWN_e && mAction != ACTION_WIND_DOWN_e) {
        cXyz ato;
        mDoMtx_stack_c::YrotS(mTargetWallAngle.y);
        mDoMtx_stack_c::XrotM(mTargetWallAngle.x);
        mDoMtx_stack_c::YrotM(current.angle.y);

        cXyz mae(0.0f, 0.0f, speedF);
        mDoMtx_stack_c::multVec(&mae, &ato);
        speed = ato;
        current.pos += speed;
    } else {
        fopAcM_posMoveF(this, NULL);
    }

    mAcch.CrrPos(dComIfG_Bgsp());
    mAnm_p->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

void daE_WS_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mWallAngle);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(mBodyScale, mBodyScale, mBodyScale);

    J3DModel* model_p = mAnm_p->getModel();
    model_p->setBaseTRMtx(mDoMtx_stack_c::get());
    mAnm_p->modelCalc();
}

void daE_WS_c::cc_set() {
    cXyz mae;
    cXyz ato;
    J3DModel* model = mAnm_p->getModel();

    mDoMtx_stack_c::YrotS(mTargetWallAngle.y);
    mDoMtx_stack_c::XrotM(mTargetWallAngle.x);
    mae.set(0.0f, 15.0f + nREG_F(10), 0.0f);
    mDoMtx_stack_c::multVec(&mae, &ato);
    attention_info.position = current.pos + ato;
    attention_info.position.y += 90.0f * mBodyScale;

    MTXCopy(model->getAnmMtx(1), mDoMtx_stack_c::get());
    mae.set(-15.0f + nREG_F(5), -10.0f + nREG_F(6), nREG_F(7));
    mDoMtx_stack_c::multVec(&mae, &eyePos);

    mCcSph.SetC(eyePos);
    mCcSph.SetR(35.0f);
    dComIfG_Ccsp()->Set(&mCcSph);

    mCcBokkuriSph.SetC(eyePos);
    mCcBokkuriSph.SetR(60.0f);
    dComIfG_Ccsp()->Set(&mCcBokkuriSph);
}

int daE_WS_c::execute() {
    if (mWaitTimer != 0) {
        mWaitTimer--;
    }

    if (mInvulnerabilityTimer != 0) {
        mInvulnerabilityTimer--;
    }

    action();
    mtx_set();
    cc_set();

    cXyz effpos(current.pos);
    cXyz effsize(0.5f, 0.5f, 0.5f);
    setMidnaBindEffect(this, &mSound, &effpos, &effsize);
    return 1;
}

static int daE_WS_Execute(daE_WS_c* i_this) {
    return i_this->execute();
}

void daE_WS_c::checkInitialWall() {
    cXyz endpos;
    cXyz offset(0.0f, 0.0f, 200.0f);
    dBgS_LinChk linchk;

    for (int i = 0; i < 4; i++) {
        cLib_offsetPos(&endpos, &current.pos, (shape_angle.y + (i << 14)), &offset);
        linchk.Set(&current.pos, &endpos, NULL);

        if (dComIfG_Bgsp().LineCross(&linchk)) {
            if (arg0 == 0 && dComIfG_Bgsp().GetWallCode(linchk) != 1 && dComIfG_Bgsp().GetWallCode(linchk) != 4) {
                return;
            }

            current.pos = linchk.GetCross();

            cM3dGPla tri;
            dComIfG_Bgsp().GetTriPla(linchk, &tri);

            cXyz* tri_np = tri.GetNP();
            mTargetWallAngle.y = cM_atan2s(tri_np->x, tri_np->z);
            mTargetWallAngle.x = cM_atan2s(tri_np->absXZ(), tri_np->y);
            mWallAngle = mTargetWallAngle;
            return;
        }
    }
}

bool daE_WS_c::checkBeforeBg(s16 i_angle) {
    dBgS_LinChk linchk;
    cXyz mae;
    cXyz ato;
    cXyz endpos;
    cXyz startpos;

    if (!speedF) {
        return false;
    }

    mDoMtx_stack_c::YrotS(mTargetWallAngle.y);
    mDoMtx_stack_c::XrotM(mTargetWallAngle.x);
    mDoMtx_stack_c::YrotM(i_angle);
    mae.set(0.0f, 50.0f * mBodyScale, 0.0f);
    mDoMtx_stack_c::multVec(&mae, &startpos);
    startpos += current.pos;
    
    mDoMtx_stack_c::YrotS(mTargetWallAngle.y);
    mDoMtx_stack_c::XrotM(mTargetWallAngle.x);
    mDoMtx_stack_c::YrotM(i_angle);
    mae.set(0.0f, 0.0f, 50.0f * mBodyScale);
    mDoMtx_stack_c::multVec(&mae, &ato);

    mae.set(ato.x, 0.0f, ato.z);
    endpos = startpos + mae;

    linchk.Set(&startpos, &endpos, NULL);
    if (dComIfG_Bgsp().LineCross(&linchk)) {
        return 1;
    }

    if (ato.y > 0.0f) {
        mae.set(0.0f, 50.0f * mBodyScale, 0.0f);
    } else {
        mae.set(0.0f, -l_HIO.dist_to_ground, 0.0f);
    }

    endpos = startpos + mae;
    linchk.Set(&startpos, &endpos, NULL);
    if (dComIfG_Bgsp().LineCross(&linchk)) {
        return true;
    }

    mDoMtx_stack_c::YrotS(mTargetWallAngle.y);
    mDoMtx_stack_c::XrotM(mTargetWallAngle.x);
    mDoMtx_stack_c::YrotM(i_angle);
    mae.set(0.0f, 50.0f * mBodyScale, 100.0f * mBodyScale);
    mDoMtx_stack_c::multVec(&mae, &ato);
    startpos = current.pos + ato;

    cXyz sp38(0.0f, -40.0f * mBodyScale, 100.0f * mBodyScale);
    mDoMtx_stack_c::multVec(&sp38, &ato);
    endpos = current.pos + ato;

    linchk.Set(&startpos, &endpos, NULL);
    if (!dComIfG_Bgsp().LineCross(&linchk)) {
        return true;
    }

    if (arg0 == 0 && dComIfG_Bgsp().GetWallCode(linchk) != 1 && dComIfG_Bgsp().GetWallCode(linchk) != 4) {
        return true;
    }

    cM3dGPla tri;
    dComIfG_Bgsp().GetTriPla(linchk, &tri);
    
    cXyz* tri_np = tri.GetNP();
    cLib_chaseAngleS(&mWallAngle.y, cM_atan2s(tri_np->x, tri_np->z), 0x100);
    checkWall();
    return false;
}

bool daE_WS_c::checkWall() {
    cXyz startpos;
    cXyz endpos;
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mWallAngle);
    mDoMtx_stack_c::ZXYrotM(shape_angle);

    mDoMtx_stack_c::transM(0.0f, 100.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&startpos);

    mDoMtx_stack_c::transM(0.0f, -150.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&endpos);

    dBgS_LinChk linchk;
    linchk.Set(&startpos, &endpos, NULL);
    if (dComIfG_Bgsp().LineCross(&linchk)) {
        if (arg0 == 0 && dComIfG_Bgsp().GetWallCode(linchk) != 1 && dComIfG_Bgsp().GetWallCode(linchk) != 4) {
            return false;
        }

        current.pos = linchk.GetCross();
    
        cM3dGPla tri;
        dComIfG_Bgsp().GetTriPla(linchk, &tri);

        cXyz* tri_np = tri.GetNP();
        mTargetWallAngle.y = cM_atan2s(tri_np->x, tri_np->z);
        mTargetWallAngle.x = cM_atan2s(tri_np->absXZ(), tri_np->y);
        return true;
    }

    return false;
}

static int daE_WS_IsDelete(daE_WS_c* a_this) {
    return 1;
}

int daE_WS_c::_delete() {
    dComIfG_resDelete(&mPhase, "E_WS");

    if (mHioSet) {
        hio_set = FALSE;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (heap != NULL) {
        mSound.deleteObject();
    }

    return 1;
}

static int daE_WS_Delete(daE_WS_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "E_WS");
    return i_this->_delete();
}

int daE_WS_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_WS", 0xD);
    JUT_ASSERT(1401, modelData != NULL);

    mAnm_p = JKR_NEW mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_WS", 7), 0, 1.0f, 0, -1, &mSound, 0x80000, 0x11000084);
    if (mAnm_p == NULL || mAnm_p->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    return ((daE_WS_c*)actor)->CreateHeap();
}

int daE_WS_c::create() {
    fopAcM_ct(this, daE_WS_c);

    int phase_state = dComIfG_resLoad(&mPhase, "E_WS");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_WS PARAM %x\n", fopAcM_GetParam(this));
    
        bitSw = (fopAcM_GetParam(this) & 0xFF00) >> 8;
        if (bitSw != 0xFF && dComIfGs_isSwitch(bitSw, fopAcM_GetRoomNo(this))) {
            OS_REPORT("E_WS やられ後なので再セットしません\n");
            return cPhs_ERROR_e;
        }
    
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x1B60)) {
            return cPhs_ERROR_e;
        }
    
        if (!hio_set) {
            hio_set = true;
            mHioSet = true;
            l_HIO.id = mDoHIO_CREATE_CHILD("スタルウォール", &l_HIO);
        }
    
        arg0 = fopAcM_GetParam(this);
        if (arg0 == 0xFF) {
            arg0 = 0;
        }
    
        if (((fopAcM_GetParam(this) & 0xFF0000) >> 0x10) == 1) {
            mBodyScale = 0.5f;
        } else {
            mBodyScale = 0.8f;
        }
    
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);
    
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mBgc, fopAcM_GetSpeed_p(this), NULL, NULL);
        mBgc.SetWall(0.0f, 0.0f);
    
        health = 10;
        field_0x560 = 10;
    
        mCcStts.Init(100, 0, this);
        mCcSph.Set(cc_ws_src);
        mCcSph.SetStts(&mCcStts);
        mCcBokkuriSph.Set(cc_bokkuri_src);
        mCcBokkuriSph.SetStts(&mCcStts);
    
        mSound.init(&current.pos, &eyePos, 3, 1);
        mSound.setEnemyName("E_ws");

        mAtInfo.mpSound = &mSound;
        mAtInfo.mPowerType = 1;

        setActionMode(ACTION_WAIT_e);
        checkInitialWall();

        mHomePos = current.pos;
        speed.y = 0.0f;
        gravity = 0.0f;
        mtx_set();
    }

    return phase_state;
}

static int daE_WS_Create(daE_WS_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daE_WS_Method = {
    (process_method_func)daE_WS_Create,
    (process_method_func)daE_WS_Delete,
    (process_method_func)daE_WS_Execute,
    (process_method_func)daE_WS_IsDelete,
    (process_method_func)daE_WS_Draw,
};

actor_process_profile_definition g_profile_E_WS = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_E_WS_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daE_WS_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_E_WS_e,
    /* Actor SubMtd */ &l_daE_WS_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_UNK_0x10000_e | fopAcStts_CULL_e,
    /* Group        */ fopAc_ENEMY_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
