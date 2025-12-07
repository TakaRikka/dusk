/**
 * @file d_a_obj_swchain.cpp
 *
*/

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#if DEBUG
#include "d/d_debug_viewer.h"
#endif
#include "d/actor/d_a_obj_swchain.h"
#include "d/d_cc_d.h"
#include "d/d_s_play.h"

#if DEBUG
class daObjSwChain_HIO_c : public mDoHIO_entry_c {
public:
    /* 811C8DE0 */ daObjSwChain_HIO_c();
    /* 811C8EE0 */ void genMessage(JORMContext*);

    /* 0x06 */ u8 field_0x06;
    /* 0x08 */ f32 mGravity;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 mTension;
    /* 0x20 */ f32 mTensionBootsEquipped;
    /* 0x24 */ f32 mTensionWolf;
    /* 0x28 */ f32 mSwitchOnTension;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 mReturnSpeed;
    /* 0x34 */ f32 mReturnSpeedHigh;
    /* 0x38 */ f32 mReturnSpeedLow;
    /* 0x3C */ s16 mReactionAngle;
    /* 0x3E */ s16 field_0x3e;
    /* 0x40 */ u8 mSwitchOnTime;
    /* 0x41 */ u8 field_0x41;
    /* 0x42 */ u8 field_0x42;
    /* 0x43 */ u8 mProcessStop;
    /* 0x44 */ u8 field_0x44;
    /* 0x45 */ u8 mReactionAngleDisplay;
};

daObjSwChain_HIO_c::daObjSwChain_HIO_c() {
    mGravity = -6.0f;
    field_0xc = 35.0f;
    field_0x10 = 35.0f;
    field_0x14 = 8.0f;
    field_0x18 = 0.0f;
    mTension = 15.0f;
    mTensionWolf = 13.0f;
    mTensionBootsEquipped = 2.1f;
    mSwitchOnTension = 10.0f;
    field_0x2c = 1.5f;
    mSwitchOnTime = 0;
    mReturnSpeedHigh = 30.0f;
    mReturnSpeedLow = 15.0;
    mReturnSpeed = 2.0;
    mReactionAngle = 0xe74;
    mReactionAngleDisplay = 0;
    field_0x3e = 0;
    field_0x41 = 0;
    field_0x42 = 0;
    mProcessStop = 0;
    field_0x44 = 0;
}

void daObjSwChain_HIO_c::genMessage(JORMContext* ctx) {
    // Chain Switch
    ctx->genLabel("チェーンスイッチ", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Gravity
    ctx->genSlider("重力", &mGravity, -100.0f, -1.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Tension
    ctx->genSlider("張力", &mTension, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Tension (Boots Equipped)
    ctx->genSlider("張力（ブーツ装備）", &mTensionBootsEquipped, 0.0f, 20.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Tension (Wolf)
    ctx->genSlider("張力（狼）", &mTensionWolf, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // SWITCH ON Tension
    ctx->genSlider("ＳＷＯＮの張力", &mSwitchOnTension, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // SWITCH ON Time
    ctx->genSlider("ＳＷＯＮの時間", &mSwitchOnTime, 0, 64.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // -- Normal --
    ctx->genLabel("-- ノーマル --", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Return Speed
    ctx->genSlider("戻る速度", &mReturnSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // -- With Chandelier --
    ctx->genLabel("-- シャンデリア付き --", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Return Speed・High
    ctx->genSlider("戻る速度・高速", &mReturnSpeedHigh, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Return Speed・Low
    ctx->genSlider("戻る速度・低速", &mReturnSpeedLow, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Process Stop
    ctx->genCheckBox("処理停止", &mProcessStop, '\x01', 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Clawshot Related
    ctx->genLabel("-- フックショット関係 --", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Reaction Angle Display
    ctx->genCheckBox("反応角度表示", &mReactionAngleDisplay, '\x01', 0, NULL, 0xffff, 0xffff, 0x200,
                     0x18);
    // Clawshot Reaction Angle
    ctx->genSlider("フックショット反応角度", &mReactionAngle, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

static daObjSwChain_HIO_c l_HIO;
#endif

static int CheckCreateHeap(fopAc_ac_c* a_this) {
    daObjSwChain_c* i_this = (daObjSwChain_c*)a_this;
    return i_this->CreateHeap();
}

static s16 GetActorName(cBgS_PolyInfo& param_0) {
    if (dComIfG_Bgsp().ChkPolySafe(param_0)) {
        if (dComIfG_Bgsp().ChkMoveBG_NoDABg(param_0)) {
            if (dComIfG_Bgsp().GetActorPointer(param_0)) {
                return fopAcM_GetName(dComIfG_Bgsp().GetActorPointer(param_0));
            }
        }
    }

    return PROC_Obj_SwChain;
}

static char* l_arcName = "P_Chain";

static dCcD_SrcSph l_sph_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x4000, 0x11}, 0x58}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},              // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},              // mGObjTg
        {0x0},                                           // mGObjCo
    },                                                   // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 50.0f}  // mSph
    }  // mSphAttr
};

static dCcD_SrcSph l_sph_srcCo = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x10}, 0x49}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},           // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},           // mGObjTg
        {0x0},                                        // mGObjCo
    },                                                // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 50.0f}  // mSph
    }  // mSphAttr
};

int daObjSwChain_c::Create() {
    mStts.Init(0xff, 0xff, this);
    mSph1.Set(l_sph_src);
    mSph1.SetStts(&mStts);
    mSph2.Set(l_sph_srcCo);
    mSph2.SetStts(&mStts);
    mAcchCir.SetWall(0.0f, 40.0f);
    mBgc.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
              fopAcM_GetSpeed_p(this), &current.angle, &shape_angle);
    fopAcM_setCullSizeSphere(this, home.pos.x, home.pos.y, home.pos.z, (mChainNum + 1) * 35.0f);
    cXyz cStack_3c(0.0f, 0.0f, 50.0f);
    cXyz cStack_48;
    cXyz cStack_54;

    mDoMtx_stack_c::YrotS(shape_angle.y);
    mDoMtx_stack_c::multVec(&cStack_3c, &cStack_48);

    getChainBasePos(&mChains->field_0x34);
    cStack_54 = mChains->field_0x34;

    chain_s* p_chain = mChains;

    for (int i = 0; i < mChainNum + 1; i++) {
        if (i < getTopChainNo()) {
            p_chain->field_0x34 = home.pos;
            cStack_54 = p_chain->field_0x34;
        } else {
            p_chain->field_0x34 = cStack_54 + cStack_48;
            cStack_54 = p_chain->field_0x34;
        }

        p_chain++;
    }

    initChainMtx();
    mModel.create(mChainModelData, 0, &tevStr);

    if (fopAcM_GetRoomNo(this) == 4) {
        field_0xa67 = 0x14;
        field_0xa69 = 0x14;
    } else if (fopAcM_GetRoomNo(this) == 6) {
        field_0xa67 = 0x14;
        field_0xa69 = 0x14;
    }

    return 1;
}

int daObjSwChain_c::CreateHeap() {
    mChains = new chain_s[mChainNum + 1];

    if (mChains == NULL) {
        return 0;
    }

    chain_s* p_chain = mChains;
    for (int i = 0; i < mChainNum + 1; i++, p_chain++) {
        p_chain->field_0x34.setall(0.0f);
        p_chain->field_0x40.setall(0);
        p_chain->field_0x48 = 0.0f;
        p_chain->field_0x4c = 0;
    }

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(532, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);

    if (mpModel == NULL) {
        return 0;
    }

    mChainModelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 3);
    JUT_ASSERT(545, mChainModelData != NULL);

    return 1;
}

int daObjSwChain_c::create1st() {
    fopAcM_ct(this, daObjSwChain_c);

    if (field_0xa60 == 0) {
        mHookshotLength = home.angle.x;
        field_0xa5e = home.angle.z;
        home.angle.x = home.angle.z = 0;
        current.angle.x = current.angle.z = 0;
        shape_angle.x = shape_angle.z = 0;
        field_0xa60 = 1;
    }

    u32 uVar1 = getChainNum();
    u8 uVar2 = (uVar1 >> 31);
    if (((uVar1 & 1) ^ uVar2) - uVar2 != 0) {
        uVar1 = getChainNum();
    } else {
        uVar1 = getChainNum() + 1;
    }
    mChainNum = uVar1;

    mHookShotLength = getHookShotLength();
    mOutNum = getOutNum();
    mChainID = getChainID();

#if DEBUG
    if (mChainID != 0 && mChainID != 1 && mChainID != 2 && mChainID != 3) {
        // Chain Switch: Chain ID value is abnormal <%d>
        OS_REPORT_ERROR("チェーンスイッチ：鎖の番号が異常値です<%d>\n", mChainID);
        return cPhs_ERROR_e;
    }
#endif

#if DEBUG
    if (mOutNum > mChainNum) {
        // Chain Switch: Argument 2 > Argument 1 !
        OS_REPORT_ERROR("チェーンスイッチ：引数２＞引数１になっています！\n");
        return cPhs_ERROR_e;
    }
#endif

#if DEBUG
    if (mHookShotLength > mChainNum) {
        // Chain Switch: Argument 3 > Argument 1 !
        OS_REPORT_ERROR("チェーンスイッチ：引数３＞引数１になっています！\n");
        return cPhs_ERROR_e;
    }
#endif

    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        u32 size = 0x4000;
        size |= 0x80000000;

        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, size)) {
            return cPhs_ERROR_e;
        }

        if (!Create()) {
            return cPhs_ERROR_e;
        }
#if DEBUG
        // Chain Switch
        l_HIO.entryHIO("チェーンスイッチ");
#endif
    }

    return phase;
}

int daObjSwChain_c::execute() {
#if DEBUG
    if (l_HIO.mProcessStop) {
        return 1;
    }
#endif

    field_0xa9c = 0;

    if (getSwbit2() != 0xff && !fopAcM_isSwitch(this, getSwbit2())) {
        field_0xa6c = 1;
    } else {
        field_0xa6c = 0;
    }

    chain_count_control();
    chain_s* unused = mChains;
    chain_control();
    chain_s* chains_p = &mChains[mChainNum];

    if (!field_0xa6c && daPy_py_c::setFmChainPos(this, &chains_p->field_0x34, mChainID)) {
        mCarry = 1;
        eyePos = home.pos;
    } else {
        mCarry = 0;
        eyePos = chains_p->field_0x34;
    }

    if (field_0xa9d == 0 && mCarry != 0 || field_0xa9d != 0 && mCarry == 0) {
        daPy_py_c::setPlayerSe(Z2SE_AL_GRAB_CHAIN);
    }

    if (fopAcM_checkHookCarryNow(this)) {
        field_0xa61 = 1;
        if (mOutNum >= mHookShotLength && field_0xa9e == 0) {
            fopAcM_cancelHookCarryNow(this);
        }
    } else {
        field_0xa61 = 0;
        current.pos = chains_p->field_0x34;
    }

    chain_control2();
    setTension();
    setChainMtx();
    field_0xa6b = 0;
    cXyz cStack_68(mChains[getTopChainNo()].field_0x34);
    cXyz cStack_74(mChains[mChainNum].field_0x34);
    f32 fVar15 = cStack_74.abs(cStack_68);
#if DEBUG
    if (fVar15 < (l_HIO.field_0xc * (mOutNum - 2)) || mCarry == 0)
#else
    if (fVar15 < (35.0f * (mOutNum - 2)) || mCarry == 0)
#endif
        if (mOutNum > getOutNum() && mRatio != 0.0f) {
            cXyz cStack_80(mChains[0].field_0x34);
            chains_p = &mChains[getTopChainNo()];
            cXyz cStack_8c = cStack_80 - chains_p->field_0x34;
            cStack_8c.normalizeZP();

            switch (fopAcM_GetRoomNo(this)) {
            case 4:
            case 6:
                if (mOutNum > field_0xa69) {
#if DEBUG
                    chains_p->field_0x34 += cStack_8c * l_HIO.mReturnSpeedHigh;
#else
                    chains_p->field_0x34 += cStack_8c * 30.0f;
#endif
                    // mistake?
                    field_0xa6b = 1;
                    field_0xa6b = 1;
                } else {
#if DEBUG
                    chains_p->field_0x34 += cStack_8c * l_HIO.mReturnSpeedLow;
#else
                    chains_p->field_0x34 += cStack_8c * 15.0f;
#endif
                }
                break;
            default:
#if DEBUG
                chains_p->field_0x34 += cStack_8c * l_HIO.mReturnSpeed;
#else
                chains_p->field_0x34 += cStack_8c * 2.0f;
#endif
            }
        }

    if (field_0xa6c == 0) {
        cXyz pos = (daPy_getPlayerActorClass()->current.pos - home.pos);
        s16 abs_tmp = pos.atan2sX_Z() - shape_angle.y;
#if DEBUG
        if (abs(abs_tmp) < l_HIO.mReactionAngle) {
#else
        if (abs(abs_tmp) < 0xe74) {
#endif
            chains_p = &mChains[mChainNum];
            mSph1.SetC(chains_p->field_0x34);
            dComIfG_Ccsp()->Set(&mSph1);
        }
    }

    if (mCarry != 0) {
        cXyz local_a4;
        cXyz cStack_b0(current.pos);
        mBgc.CrrPos(dComIfG_Bgsp());
        local_a4 = current.pos - cStack_b0;
        current.pos = cStack_b0;

        if (mSph2.ChkCoHit()) {
            cXyz* move_pos = mStts.GetCCMoveP();
            if (move_pos != NULL) {
                local_a4 += *move_pos;
            }
        }

        mStts.ClrCcMove();
        mSph2.ClrCoHit();

        daPy_getPlayerActorClass()->setGrabCollisionOffset(local_a4.x, local_a4.z, 0);
        mSph2.SetC(current.pos);
        dComIfG_Ccsp()->Set(&mSph2);
    }

    field_0xa9d = mCarry;
#if DEBUG
    if (l_HIO.field_0x42 != 0) {
        if (l_HIO.field_0x44 == getChainID()) {
            dDbVw_Report(0x28, 200, "Num<%d>Len<%.2f>", getCurrentChainNum(),
                         getCurrentChainLength());
            if (checkTight() != 0) {
                dDbVw_Report(0x28, 100, "TENSION!");
            }
        }

        dDbVw_Report(0x28, 0xd4, "SeenAngY   <%x>", fopAcM_seenPlayerAngleY(this));
        dDbVw_Report(0x28, 0xe0, "SearchAngY <%x>", fopAcM_searchPlayerAngleY(this));
        dDbVw_Report(0x28, 0xec, "toShapeAngY<%x>", fopAcM_toPlayerShapeAngleY(this));
    }
#endif

    return 1;
}

void daObjSwChain_c::getChainBasePos(cXyz* param_0) {
    *param_0 = home.pos;
}

void daObjSwChain_c::chain_control() {
    f32 fVar1;
    chain_s* chains_p2 = mChains;
    chain_s* chains_p1 = chains_p2;
    cXyz* pcVar2;
    cXyz* pcVar1 = &chains_p1->field_0x34;
    pcVar2 = pcVar1;
    f32 fVar2 = 
    #if DEBUG
        l_HIO.mGravity;
    #else
        -6.0f;
    #endif

    if (mCarry != 0) {
        if (daPy_getPlayerActorClass()->checkBootsOrArmorHeavy()) {
            if (pow > KREG_F(1) + 0.5f) {
                fVar2 *= KREG_F(2) + 0.099999994f;
            } else {
                fVar2 = 
                #if DEBUG
                    l_HIO.mGravity;
                #else
                    -6.0f;
                #endif
            }
        } else if (pow > KREG_F(1) + 5.0f) {
            fVar2 *= KREG_F(2) + 0.099999994f;
        } else {
            fVar2 = 
            #if DEBUG
                l_HIO.mGravity;
            #else
                -6.0f;
            #endif
        }
    }

    cLib_addCalc(&field_0xa94, fVar2, 0.1f, 1.0f, 0.1f);
    gravity = field_0xa94;
    chains_p1 += getTopChainNo();

    for (int topChainNo = getTopChainNo(); topChainNo < mChainNum + 1; chains_p1++, topChainNo++, pcVar1 = pcVar2) {
        pcVar2 = &chains_p1->field_0x34;
        chains_p1->field_0x48 = field_0xa94;
        fVar2 = chains_p1->field_0x34.y;
        chains_p1->field_0x34.y += chains_p1->field_0x48;
        cXyz pos(*pcVar2);
        pos.y += 100.0f;
        chains_p1->field_0x4c = 0;

        if (fopAcM_gc_c::gndCheck(&pos) && GetActorName(*fopAcM_gc_c::getGroundCheck()) != PROC_Obj_Lv4DigSand) {
            fVar1 = fopAcM_gc_c::getGroundY() + 15.0f;
            if (fVar1 > pcVar2->y) {
                pcVar2->y = fVar1;
                chains_p1->field_0x48 = 0.0f;
                chains_p1->field_0x4c = 1;
            }
        } else {
            pcVar2->y = fVar2;
            chains_p1->field_0x48 = 0.0f;
            chains_p1->field_0x4c = 1;
        }

        if (chains_p1->field_0x4c == 0) {
            if (topChainNo != getTopChainNo() && chains_p1[-1].field_0x4c != 0 && pcVar1->y >= pcVar2->y) {
                pcVar2->y = fVar2;
            }

            chain_s* chains_p3 = chains_p1;
            for (int i = topChainNo; i < mChainNum; i++) {
                chains_p3++;
                if (chains_p3->field_0x4c != 0 && chains_p3->field_0x34.y >= pcVar2->y) {
                    pcVar2->y = fVar2;
                    break;
                }
            }
        }

        cXyz sp94 = *pcVar2 - *pcVar1;
        fVar1 = sp94.abs();
        sp94.normalizeZP();

        f32 fVar3 = 
        #if DEBUG
            l_HIO.field_0xc;
        #else
            35.0f;
        #endif
        
        if (topChainNo == mChainNum - 1) {
            fVar3 = 
            #if DEBUG
                l_HIO.field_0x10;
            #else
                35.0f;
            #endif
        }

        if (fVar1 <= fVar3 - 
            #if DEBUG
                l_HIO.field_0x14
            #else
                8.0f
            #endif
            ) 
        {
            fVar1 = fVar3 - 
            #if DEBUG
                l_HIO.field_0x14;
            #else
                8.0f;
            #endif
        }

        if (fVar1 > fVar3) {
            fVar1 = fVar3;
        }

        sp94 = sp94 * fVar1;
        if (topChainNo != getTopChainNo()) {
            *pcVar2 = *pcVar1 + sp94;
        } else {
            pcVar2->y = fVar2;
        }

        chains_p1->field_0x34 = *pcVar2;
        fVar1 = std::fabs(chains_p1->field_0x48);
        fVar2 = std::fabs(fVar2 - pcVar2->y);
        if (fVar2 < fVar1) {
            chains_p1->field_0x48 = 0.0f;
        }
    }
}

void daObjSwChain_c::chain_control2() {
    daPy_py_c* player = (daPy_py_c*)daPy_getPlayerActorClass();
    chain_s* chains_p = &mChains[mChainNum];
    cXyz* pcVar2 = &chains_p->field_0x34;
    cXyz* pcVar1 = pcVar2;

    if (mCarry != 0 || field_0xa61 != 0) {
        cXyz sp78, sp84;

        if (mCarry != 0) {
            if (daPy_py_c::checkNowWolf()) {
                mDoMtx_stack_c::copy(player->getWolfMouthMatrix());
                mDoMtx_stack_c::multVecZero(&sp78);
                cXyz sp90(KREG_F(14) + 12.0f, KREG_F(15) + 7.0f, 0.0f);
                mDoMtx_stack_c::get()[0][3] = 0.0f;
                mDoMtx_stack_c::get()[1][3] = 0.0f;
                mDoMtx_stack_c::get()[2][3] = 0.0f;
                mDoMtx_stack_c::multVec(&sp90, &sp90);
                sp78 += sp90;
                #if DEBUG
                field_0xac0 = sp78;
                #endif

                cXyz sp9c(0.0f, 0.0f, KREG_F(13) + 60.0f);
                mDoMtx_stack_c::YrotS(player->shape_angle.y);
                mDoMtx_stack_c::XrotM(field_0xa6e);
                mDoMtx_stack_c::multVec(&sp9c, &sp9c);
                *pcVar1 = sp9c + sp78;
                field_0xa78 = *pcVar1;
                sp78.set(0.0f, 0.0f, KREG_F(10) + -10.0f);
            } else {
                sp78 = player->getRightHandPos();
                cXyz spa8(KREG_F(10) + 25.0f, KREG_F(11) + 15.0f, KREG_F(12) + 60.0f);
                mDoMtx_stack_c::YrotS(player->shape_angle.y);
                mDoMtx_stack_c::XrotM(field_0xa6e);
                mDoMtx_stack_c::multVec(&spa8, &spa8);
                spa8 += sp78;
                *pcVar1 = spa8;
                field_0xa78 = *pcVar1;
                sp78.set(0.0f, 0.0f, KREG_F(10) + -10.0f);
            }

            mDoMtx_stack_c::YrotS(player->shape_angle.y + 0x7FFF);
            mDoMtx_stack_c::XrotM(-field_0xa6e);
            mDoMtx_stack_c::multVec(&sp78, &sp84);
            sp84 += field_0xa78;
            #if DEBUG
            field_0xac0 = sp84;
            #endif
            *pcVar1 = sp84;
        } else if (field_0xa61 != 0) {
            *pcVar1 = current.pos;
        }

        for (int i = mChainNum - 1; i >= getTopChainNo(); chains_p->field_0x34 = *pcVar2, i--) {
            pcVar1 = pcVar2;
            pcVar2 = &chains_p[-1].field_0x34;
            f32 fVar1 = chains_p[-1].field_0x34.y;
            chains_p--;
            cXyz spb4(*pcVar1 - *pcVar2);
            spb4.z = 0.0f;
            spb4.x = 0.0f;
            spb4.y *=
            #if DEBUG
                l_HIO.field_0x18;
            #else
                0.0f;
            #endif
            pcVar2->y += std::fabs(spb4.y);
            cXyz spc0(*pcVar2);
            spc0.y += 100.0f;

            if (fopAcM_gc_c::gndCheck(&spc0) && GetActorName(*fopAcM_gc_c::getGroundCheck()) != PROC_Obj_Lv4DigSand) {
                f32 fVar2 = fopAcM_gc_c::getGroundY() + 15.0f;
                if (fVar2 > pcVar2->y) {
                    pcVar2->y = fVar2;
                    chains_p->field_0x48 = 0.0f;
                }
            } else {
                pcVar2->y = fVar1;
                chains_p->field_0x48 = 0.0f;
                chains_p->field_0x4c = 1;
            }

            if (chains_p->field_0x4c == 0) {
                if (i != getTopChainNo() && chains_p[-1].field_0x4c != 0 && pcVar1->y >= pcVar2->y) {
                    pcVar2->y = fVar1;
                }

                if (i < mChainNum && chains_p[1].field_0x4c != 0 && chains_p[1].field_0x34.y >= pcVar2->y) {
                    pcVar2->y = fVar1;
                }
            }

            cXyz spcc(*pcVar2 - *pcVar1);
            fVar1 = spcc.abs();
            spcc.normalizeZP();

            f32 fVar3 = 
            #if DEBUG
                l_HIO.field_0xc;
            #else
                35.0f;
            #endif
            
            if (i == mChainNum - 1) {
                fVar3 = 
                #if DEBUG
                    l_HIO.field_0x10;
                #else
                    35.0f;
                #endif
            }

            if (fVar1 <= fVar3 - 
                #if DEBUG
                    l_HIO.field_0x14
                #else
                    8.0f
                #endif
                ) 
            {
                fVar1 = fVar3 - 
                #if DEBUG
                    l_HIO.field_0x14;
                #else
                    8.0f;
                #endif
            }

            if (fVar1 > fVar3) {
                fVar1 = fVar3;
            }

            spcc = spcc * fVar1;
            *pcVar2 = *pcVar1 + spcc;
        }
    }
}

void daObjSwChain_c::initChainMtx() {
    int iVar1 = 0;
    for (int i = 0; i < mChainNum + 1; i++) {
        iVar1 += (int)(cM_rndFX(0.0f) + 16384.0f);

        if (i == mChainNum) {
            mChains[i].field_0x40.z = 0;
        } else {
            mChains[i].field_0x40.z = iVar1;
        }
    }

    setChainMtx();
}

void daObjSwChain_c::setChainMtx() {
    daPy_py_c* player = (daPy_py_c*)daPy_getPlayerActorClass();
    chain_s* chains_p1;
    if (mCarry != 0) {
        chains_p1 = &mChains[mChainNum];    
    } else {
        chains_p1 = &mChains[getTopChainNo()];
    }

    for (int topChainNo = getTopChainNo(); topChainNo < mChainNum + 1; topChainNo++) {
        chain_s* chains_p2 = chains_p1;
        if (mCarry != 0) {
            if (topChainNo != getTopChainNo()) {
                chains_p2 = chains_p1 + 1;
            }
        } else {
            chains_p2 = chains_p1 - 1;
        }

        cXyz sp30(chains_p1->field_0x34 - chains_p2->field_0x34);
        cXyz sp3c;
        sp3c = (chains_p1->field_0x34 + chains_p2->field_0x34) * 0.5f;
        s16 sVar1 = -cM_atan2s(sp30.y, sp30.absXZ());
        s16 sVar2 = cM_atan2s(sp30.x, sp30.z);
        mDoMtx_stack_c::transS(sp3c);
        mDoMtx_stack_c::ZXYrotM(sVar1, sVar2, chains_p1->field_0x40.z);

        if (topChainNo == getTopChainNo() && mCarry != 0) {
            int iVar1 = (s16)(player->shape_angle.y + 0x7FFF);
            mDoMtx_stack_c::transS(field_0xa78);
            mDoMtx_stack_c::YrotM(iVar1);
            mDoMtx_stack_c::XrotM(KREG_S(4) - field_0xa6e);
            mDoMtx_stack_c::ZrotM(0);
        }

        if (mCarry != 0) {
            if (topChainNo == getTopChainNo()) {
                mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
            } else {
                chains_p1->mModel.setMtx(mDoMtx_stack_c::get());
            }
            
            chains_p1--;
        } else {
            if (topChainNo == mChainNum) {
                mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
            } else {
                chains_p1->mModel.setMtx(mDoMtx_stack_c::get());
            }

            chains_p1++;
        }
    }

    cLib_addCalcAngleS2(&field_0xa6e, field_0xa70, 5, 800);
}

void daObjSwChain_c::chain_count_control() {
    daPy_py_c* player = (daPy_py_c*)daPy_getPlayerActorClass();
    chain_s* chains_p = &mChains[getTopChainNo()];
    int iVar1;
    cXyz sp60(mChains->field_0x34);
    int roomNo = fopAcM_GetRoomNo(this);
    f32 fVar1 = sp60.abs(chains_p->field_0x34);
    f32 fVar2 = 
    #if DEBUG
        l_HIO.field_0xc
    #else
        35.0f
    #endif
        * (KREG_F(16) + 3.0f);
    
    if (fVar1 > fVar2) {
        if (mOutNum < mChainNum || ((roomNo == 4 || roomNo == 6) && mOutNum < field_0xa67)) {
            u32 iVar1 = 1;
            int iVar2 = iVar1;
            if (roomNo != 4 && roomNo != 6) {
                for (fVar1 -= fVar2; fVar1 > 0.0f;) {
                    fVar1 -= 
                    #if DEBUG
                        l_HIO.field_0xc;
                    #else
                        35.0f;
                    #endif
                
                    iVar2++;
                }

                if (mOutNum + iVar2 >= mChainNum) {
                    iVar2 = mChainNum - mOutNum;
                }
            }
            mOutNum += iVar2;

            if (mOutNum >= mHookShotLength) {
                field_0xa9e = 1;
            }

            if (field_0xa6a == 0) {
                mDoAud_seStart(Z2SE_OBJ_CHAINSW_PULL, &home.pos, 0, 0);
            } else {
                mDoAud_seStart(Z2SE_OBJ_CHAINSW_PULLHEAVY, &home.pos, 0, 0);
            }

            chain_s* chains_p2 = &mChains[getTopChainNo()];
            chains_p2->field_0x34.x = (chains_p->field_0x34.x + sp60.x) * 0.5f;
            chains_p2->field_0x34.y = (chains_p->field_0x34.y + sp60.y) * 0.5f;
            chains_p2->field_0x34.z = (chains_p->field_0x34.z + sp60.z) * 0.5f;
        }
    } else if (fVar1 < 
            #if DEBUG
                l_HIO.field_0xc
            #else
                35.0f
            #endif
                * (KREG_F(17) + 0.5f) &&
        pow == 0.0f && mOutNum > getOutNum() && (roomNo == 4 || roomNo == 6)
    ) {
        mOutNum--;

        if (field_0xa61 == 0) {
            if (field_0xa6b == 0) {
                mDoAud_seStart(Z2SE_OBJ_CHAINSW_BACK, &home.pos, 0, 0);
            } else {
                mDoAud_seStart(Z2SE_OBJ_CHAINSW_BACKFAST, &home.pos, 0, 0);
            }
        }

        if (roomNo == 4 || roomNo == 6) {
            if (mOutNum < field_0xa69) {
                field_0xa67 = field_0xa69;
            } else {
                field_0xa67 = mOutNum + 1;
            }
        }
    }

    u32 bVar1 = daPy_getPlayerActorClass()->checkBootsOrArmorHeavy();
    if ((roomNo == 4 || roomNo == 6) && mOutNum == field_0xa67) {
        chain_s* topChain_p = &mChains[getTopChainNo()];
        topChain_p->field_0x34 = mChains->field_0x34;
        if (
            (!bVar1 && pow >
            #if DEBUG
                l_HIO.mSwitchOnTension
            #else
                10.0f
            #endif
            ) || (bVar1 && pow >
            #if DEBUG
                l_HIO.field_0x2c
            #else
                1.5f
            #endif
            )
        ) {
            field_0xa68++;
            if (field_0xa68 > KREG_S(7) + 15 && player->getBaseAnimeFrame() > 6.0f && player->getBaseAnimeFrame() < 8.0f) {
                if (field_0xa67 < mChainNum) {
                    field_0xa67 += KREG_S(8) + 1;
                }

                field_0xa68 = 0;
            }
        } else {
            if (pow != 0.0f && std::fabs(player->getSpeedF()) < 10.0f) {
                field_0xa9c = 1;
            }

            field_0xa68 = 0;
        }
    } else if (mOutNum == mChainNum) {
        chain_s* topChain_p = &mChains[getTopChainNo()];
        topChain_p->field_0x34 = mChains->field_0x34;
        if (
            (!daPy_getPlayerActorClass()->checkBootsOrArmorHeavy() && pow > 
            #if DEBUG
                l_HIO.mSwitchOnTension
            #else
                10.0f
            #endif
            ) || (daPy_getPlayerActorClass()->checkBootsOrArmorHeavy() && pow >
            #if DEBUG
                l_HIO.field_0x2c
            #else
                1.5f
            #endif
            )
        ) {
            field_0xa68++;
            
            if (getSwbit() != 0xFF) {
                if (!fopAcM_isSwitch(this, getSwbit()) && field_0xa68 > 
                    #if DEBUG
                        l_HIO.mSwitchOnTime
                    #else
                        0
                    #endif
                ) {
                    if (daPy_py_c::checkNowWolf()) {
                        if (player->getBaseAnimeFrame() > 9.0f && player->getBaseAnimeFrame() < 13.0f) {
                            fopAcM_onSwitch(this, getSwbit());
                            dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));

                            if (fopAcM_GetRoomNo(this) == 0 || fopAcM_GetRoomNo(this) == 2 || fopAcM_GetRoomNo(this) == 5 ||
                                fopAcM_GetRoomNo(this) == 9 || fopAcM_GetRoomNo(this) == 13) {
                                mDoAud_seStart(Z2SE_OBJ_CHAINSW_ON, &home.pos, 0, 0);
                            }
                        }
                    } else if (player->getBaseAnimeFrame() < 20.0f) {
                        player->onChainForcePull();
                        field_0xa6d = KREG_S(0) + 30;
                    }
                }
            }
        } else if (field_0xa6d != 0 && !daPy_py_c::checkNowWolf()) {
            if (cLib_calcTimer<u8>(&field_0xa6d) == 0) {
                fopAcM_onSwitch(this, getSwbit());
                dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));
                
                if (fopAcM_GetRoomNo(this) == 0 || fopAcM_GetRoomNo(this) == 2 || fopAcM_GetRoomNo(this) == 5
                    || fopAcM_GetRoomNo(this) == 9 || fopAcM_GetRoomNo(this) == 13) {
                    mDoAud_seStart(Z2SE_OBJ_CHAINSW_ON, &home.pos, 0, 0);
                }
            }
        } else {
            field_0xa68 = 0;
            field_0xa6d = 0;
            if (fopAcM_GetRoomNo(this) == 4 || fopAcM_GetRoomNo(this) == 6) {
                fopAcM_offSwitch(this, getSwbit());
            }
        }
    }
}

void daObjSwChain_c::setTension() {
    f32 fVar1 = pow;
    field_0xa6a = 0;
    pow = 0.0f;

    if (mCarry == 0 && field_0xa61 == 0) {
        field_0xa6e = field_0xa70 = 0;
    } else {
        int roomNo = fopAcM_GetRoomNo(this);
        daPy_py_c* player = daPy_getPlayerActorClass();
        getTopChainNo();
        cXyz spbc, spc8;
        spbc = home.pos;
        spc8 = mChains[mChainNum].field_0x34;
        f32 len = spc8.abs(spbc);
        
        f32 max =         
        #if DEBUG
            l_HIO.field_0xc
        #else
            35.0f
        #endif
            * (mChainNum - 2);

        if (roomNo == 4 || roomNo == 6) {
            max = (field_0xa67 + AREG_S(0) + -1) *
            #if DEBUG
                l_HIO.field_0xc;
            #else
                35.0f;
            #endif
        }
        
        if (field_0xa61 != 0) {
            if (len > max) {
                fopAcM_cancelHookCarryNow(this);
            }
        } else {
            if (mOutNum == mChainNum || (roomNo == 4 || roomNo == 6) && mOutNum == field_0xa67) {
                if ((fVar1 == 0.0f && len > max + 20.0f + KREG_F(3)) || (fVar1 != 0.0f && len > max + KREG_F(3))) {
                    cXyz spd4;
                    f32 fVar4 = player->getSpeedF();
                    spd4.x = fVar4 * cM_ssin(player->current.angle.y);
                    f32 fVar5 = fVar4 * cM_scos(player->current.angle.y);
                    spd4.x *= KREG_F(6) + 1.0f;
                    spd4.z = fVar5 * (KREG_F(6) + 1.0f);
                    pow = std::sqrt(spd4.x * spd4.x + spd4.z * spd4.z);

                    if (checkPlayerPull() && (roomNo != 4 && roomNo != 6 && mOutNum == mChainNum || (roomNo == 4 || roomNo == 6) && field_0xa67 == mChainNum)) {
                        player->onChainPullEnd();
                    } 
                } else {
                    pow = 0.0f;
                }

                cXyz spe0(mChains->field_0x34 - mChains[mChainNum].field_0x34);
                player->setOutPower(pow, cM_atan2s(spe0.x, spe0.z), 0);
                #if DEBUG
                if (l_HIO.field_0x41 != 0) {
                    dDbVw_Report(40, 260, "len<%.2f>max<%.2f>pow<%.2f>pl_spd<%.2f>", len, max, pow, player->getSpeedF());
                }
                #endif
            } else if ((roomNo == 4 || roomNo == 6) && mOutNum >= field_0xa69 - 1 && checkPlayerPull() && (fVar1 != 0.0f || len > max)) {
                cXyz spec;
                spec.x = player->getSpeedF() * cM_ssin(player->current.angle.y);
                spec.z = player->getSpeedF() * cM_scos(player->current.angle.y);
                spec.x *= (KREG_F(4) + 0.70000005f);
                spec.z *= (KREG_F(4) + 0.70000005f);
                pow = std::sqrt(spec.x * spec.x + spec.z * spec.z);
                player->setOutPower(pow, player->current.angle.y + 0x7FFF, 0);
                field_0xa6a = 1;
            }

            if (daPy_py_c::checkNowWolf()) {
                    fVar1 =
                    #if DEBUG
                        l_HIO.mTensionWolf;
                    #else
                        13.0f;
                    #endif
            } else {
                f32 fVar7;
                if (daPy_getPlayerActorClass()->checkBootsOrArmorHeavy()) {
                    fVar7 = 
                    #if DEBUG
                        l_HIO.mTensionBootsEquipped;
                    #else
                        2.1f;
                    #endif
                } else {
                    fVar7 = 
                    #if DEBUG
                        l_HIO.mTension;
                    #else
                        15.0f;
                    #endif
                }
                fVar1 = fVar7;
            }

            fVar1 = 1.0f - std::fabs(pow / fVar1);
            if (fVar1 < 0.1f) {
                fVar1 = 0.1f;
            }

            field_0xa70 = fVar1 * (KREG_S(1) + 7000);
        }
    }
}

int daObjSwChain_c::getTopChainNo() {
    return mChainNum - mOutNum + 1;
}

BOOL daObjSwChain_c::checkPlayerPull() {
    daPy_py_c* player = (daPy_py_c*)daPy_getPlayerActorClass();
    cXyz sp2c;
    cXyz sp38(mChains->field_0x34 - mChains[mChainNum].field_0x34);

    sp2c.x = player->getSpeedF() * cM_ssin(player->current.angle.y);
    sp2c.z = player->getSpeedF() * cM_scos(player->current.angle.y);

    if (mCarry != 0 && sp38.inprodXZ(sp2c) < 0.0f) {
        return TRUE;
    }

    return FALSE;
}

int daObjSwChain_c::draw() {
    g_env_light.settingTevStruct(0, &home.pos, &tevStr);
    
    chain_s* chains_p = &mChains[getTopChainNo()];
    for (int i = 0; i < mOutNum - 1; i++, chains_p++) {
        mModel.entryObj(&chains_p->mModel);
    }

    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);

    #if DEBUG
    if (l_HIO.field_0x41 != 0) {
        cXyz sp2c;
        mDoMtx_stack_c::YrotS(shape_angle.y);
        mDoMtx_stack_c::multVec(&cXyz::BaseZ, &sp2c);
        sp2c *= 200.0f;
        sp2c += home.pos;
        sp2c.y += 10.0f;
        dDbVw_drawArrowOpa(home.pos, sp2c, (GXColor) {0, 0xFF, 0, 0xFF}, 1, 12);

        int iVar1 = l_HIO.field_0x3e;
        if (iVar1 >= 0) {
            if (iVar1 > mChainNum) {
                iVar1 = mChainNum;
            }

            cXyz& i_pos = mChains[iVar1].field_0x34;
            dDbVw_drawSphereOpa(i_pos, 30.0f, (GXColor) {0xFF, 0xFF, 0xFF, 0xFF}, 1);
        } else if (iVar1 == -2) {
            chains_p = mChains;
            for (int i = 0; i < mChainNum; i++, chains_p++) {
                dDbVw_drawSphereOpa(chains_p[1].field_0x34, 10.0f, (GXColor) {0xFF, 0xFF, 0xFF, 0xFF}, 1);
            }
        }

        dDbVw_drawCircleOpa(home.pos, l_HIO.field_0xc * (mChainNum - 2), (GXColor) {0xFF, 0xFF, 0xFF, 0xFF}, 1, 12);
    }

    cXyz i_start, i_end;
    if (l_HIO.mReactionAngleDisplay != 0) {
        i_start = home.pos;
        i_end.set(0.0f, 0.0f, KREG_F(19) + 1000.0f);
        mDoMtx_stack_c::transS(home.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y + l_HIO.mReactionAngle);
        mDoMtx_stack_c::multVec(&i_end, &i_end);
        dDbVw_drawLineOpa(i_start, i_end, (GXColor) {0xFF, 0, 0, 0xFF}, 1, 12);

        i_end.set(0.0f, 0.0f, KREG_F(19) + 1000.0f);
        mDoMtx_stack_c::transS(home.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y - l_HIO.mReactionAngle);
        mDoMtx_stack_c::multVec(&i_end, &i_end);
        dDbVw_drawLineOpa(i_start, i_end, (GXColor) {0xFF, 0, 0, 0xFF}, 1, 12);
    }
    #endif

    return 1;
}

int daObjSwChain_c::_delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
#if DEBUG
    l_HIO.removeHIO();
#endif
    return 1;
}

static int daObjSwChain_Draw(daObjSwChain_c* i_this) {
    return i_this->draw();
}

static int daObjSwChain_Execute(daObjSwChain_c* i_this) {
    return i_this->execute();
}

static int daObjSwChain_Delete(daObjSwChain_c* i_this) {
    fpc_ProcID unused = fopAcM_GetID(i_this);
    return i_this->_delete();
}

static int daObjSwChain_Create(fopAc_ac_c* a_this) {
    daObjSwChain_c* i_this = (daObjSwChain_c*)a_this;
    fpc_ProcID unused = fopAcM_GetID(a_this);
    return i_this->create1st();
}

static actor_method_class l_daObjSwChain_Method = {
    (process_method_func)daObjSwChain_Create,  (process_method_func)daObjSwChain_Delete,
    (process_method_func)daObjSwChain_Execute, 0,
    (process_method_func)daObjSwChain_Draw,
};

extern actor_process_profile_definition g_profile_Obj_SwChain = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_SwChain,        // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjSwChain_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    615,                     // mPriority
    &l_daObjSwChain_Method,  // sub_method
    0x000C0000,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
