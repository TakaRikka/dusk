/**
 * @file d_a_obj_tombo.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_tombo.h"
#include "d/actor/d_a_player.h"
#include "d/d_menu_insect.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_camera_mng.h"
#include "f_pc/f_pc_name.h"
#include "m_Do/m_Do_lib.h"

class daObj_TomHIO_c : public JORReflexible {
public:
    daObj_TomHIO_c();
    virtual ~daObj_TomHIO_c() {}

    #if DEBUG
    void genMessage(JORMContext* context) {
        // Golden dragonfly
        context->genLabel("黄金蟲(トンボ)", 0x80000001, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
        // Model Scale (male)
        context->genSlider("モデルスケール(オス)", &mFemaleModelScale, 0.1f, 4.0f, 0, NULL, 0xffff, 0xffff,
                           0x200, 0x18);
        // Model Scale (female)
        context->genSlider("モデルスケール(メス)", &mMaleModelScale, 0.1f, 4.0f, 0, NULL, 0xffff, 0xffff,
                           0x200, 0x18);
        // btk speed
        context->genSlider("btkスピード", &mBtkSpeed, 0.0f, 4.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                           0x18);
    }
    #endif

    /* 0x04 */ s8 mId;
    /* 0x08 */ f32 mFemaleModelScale;
    /* 0x0C */ f32 mMaleModelScale;
    /* 0x10 */ f32 mBtkSpeed;
};

daObj_TomHIO_c::daObj_TomHIO_c() {
    mId = -1;
    mMaleModelScale = 0.9f;
    mFemaleModelScale = 0.9f;
    mBtkSpeed = 1.0f;
}

static u8 const l_tom_itemno[2] = {
    fpcNm_ITEM_M_DRAGONFLY, fpcNm_ITEM_F_DRAGONFLY,
};

void daObjTOMBO_c::InitCcSph() {
    const static dCcD_SrcSph ccSphSrc = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0x14002, 0x11}, 0x0}},  // mObj
            {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0},             // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},              // mGObjTg
            {0x0},                                           // mGObjCo
        },                                                   // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
        }  // mSphAttr
    };

    mStts.Init(1, 0, this);
    mSph.Set(ccSphSrc);
    mSph.SetStts(&mStts);
    mSph.SetC(current.pos);
    mSph.OnTgNoHitMark();
}

void daObjTOMBO_c::SetCcSph() {
    mSph.SetC(current.pos);
    mSph.SetR(20.0f);
    dComIfG_Ccsp()->Set(&mSph);
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return ((daObjTOMBO_c*)i_this)->CreateHeap();
}

int daObjTOMBO_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Tombo", 9);
    JUT_ASSERT(259, modelData != NULL);
    mpMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                  (J3DAnmTransform*)dComIfG_getObjectRes("Tombo", 6), 2, 1.0, 0, -1,
                                  &mCreature, 0, 0x11000284);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* morfModel = mpMorf->getModel();
    mBrk = new mDoExt_brkAnm();
    if (mBrk == NULL) {
        return 0;
    }

    if (mSex == SEX_FEMALE) {
        if (!mBrk->init(morfModel->getModelData(), (J3DAnmTevRegKey*)dComIfG_getObjectRes("Tombo", 12), 1, 2, 1.0f, 0, -1)) {
            return 0;
        }
    } else {
        if (!mBrk->init(morfModel->getModelData(), (J3DAnmTevRegKey*)dComIfG_getObjectRes("Tombo", 13), 1, 2, 1.0f, 0, -1)) {
            return 0;
        }
    }
    mBtk = new mDoExt_btkAnm();
    if (mBtk == NULL) {
        return 0;
    }

    if (mSex == SEX_FEMALE) {
        if (!mBtk->init(morfModel->getModelData(), (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Tombo", 16), 1, 2, 1.0f, 0, -1)) {
            return 0;
        }
    } else {
        if (!mBtk->init(morfModel->getModelData(), (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Tombo", 17), 1, 2, 1.0f, 0, -1)) {
            return 0;
        }
    }
    return 1;
}

static int daObjTOMBO_Create(fopAc_ac_c* i_this) {
    return ((daObjTOMBO_c*)i_this)->create();
}

static int daObjTOMBO_Delete(daObjTOMBO_c* i_this) {
    i_this->Delete();
    return 1;
}

void daObjTOMBO_c::WaitAction() {
    cXyz cStack_28;
    switch (field_0x712) {
    case 0:
        field_0x71c = field_0x71c * 0.2f;
        field_0x720 = 1.0f * field_0x720;
        field_0x714[0] = cM_rndF(10.0f) + 20.0f;
        field_0x712++;
        break;
    case 1:
        if (field_0x714[0] == 0) {
            mAction = ACTION_MOVE;
            field_0x712 = 0;
            field_0x714[0] = cM_rndF(10.0f) + 10.0f;
            if (field_0x714[1] == 0) {
                SearchLink();
            }
        }
        break;
    }
    SpeedSet();
}

void daObjTOMBO_c::SpeedSet() {
    current.pos.y += speed.y;
    cXyz cStack_1c(0.0f, 0.0f, speedF);
    cXyz local_28(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::YrotS(current.angle.y);
    mDoMtx_stack_c::multVec(&cStack_1c, &local_28);
    current.pos.x += local_28.x;
    current.pos.z += local_28.z;
    cLib_addCalc2(&speedF, field_0x71c, 0.1f, 10.0f);
    cLib_addCalc2(&speed.y, field_0x720, 0.1f, 10.0f);
    cLib_addCalcAngleS2(&current.angle.y, field_0x724, 0x10, 0x1000);
    shape_angle.y = current.angle.y;
}

bool daObjTOMBO_c::CheckWater() {
    dBgS_ObjGndChk_All adStack_6c;
    cXyz gndPos(current.pos.x, current.pos.y + 100.0f, current.pos.z);
    adStack_6c.SetPos(&gndPos);
    gndPos.y = dComIfG_Bgsp().GroundCross(&adStack_6c);
    if (gndPos.y != -G_CM3D_F_INF) {
        if (dComIfG_Bgsp().GetPolyAtt0(adStack_6c) == 7 && current.pos.y - gndPos.y < 50.0f) {
            return TRUE;
        }
    }
    return FALSE;
}

void daObjTOMBO_c::CheckGround() {
    dBgS_ObjGndChk_All adStack_6c;
    cXyz gndPos(current.pos.x, current.pos.y + 100.0f, current.pos.z);
    adStack_6c.SetPos(&gndPos);
    gndPos.y = dComIfG_Bgsp().GroundCross(&adStack_6c);
    if (gndPos.y != -G_CM3D_F_INF) {
        if (current.pos.y - gndPos.y < 100.0f) {
            field_0x720 = 3.0f;
        } else if (current.pos.y - gndPos.y > 300.0f) {
            field_0x720 = -3.0f;
        }

        if (current.pos.y - gndPos.y < 10.0f) {
            speed.y = 0.0f;
        }
    }
}

void daObjTOMBO_c::CheckWall() {
    dBgS_LinChk linChk;
    cXyz cStack_8c(0.0f, 0.0f, 200.0f);
    cXyz cStack_98;
    cMtx_YrotS(*calc_mtx, current.angle.y);
    MtxPosition(&cStack_8c, &cStack_98);
    cStack_98 += current.pos;
    linChk.Set(&current.pos, &cStack_98, this);
    if (dComIfG_Bgsp().LineCross(&linChk)) {
        field_0x724 += 0x1000;
        field_0x714[0] = cM_rndF(10.0f) + 10.0f;
    }
}

void daObjTOMBO_c::SearchLink() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz& playerPos = fopAcM_GetPosition(player);
    if (current.pos.absXZ(playerPos) < 400.0f && current.pos.y - playerPos.y < 150.0f &&
        fopAcM_GetSpeedF(player) == 0.0f && player->checkItemSwordEquip())
    {
        mAction = ACTION_LINK;
        field_0x712 = 0;
        cXyz cStack_28(30.0f, 70.0f, 0.0f);
        MTXCopy(daPy_getLinkPlayerActorClass()->getModelJointMtx(9), *calc_mtx);
        MtxPosition(&cStack_28, &field_0x73c);
    }
}

void daObjTOMBO_c::LinkAction() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    fopAcM_GetPosition(player);
    cXyz cStack_1c(30.0f, 70.0f, 0.0f);
    cXyz cStack_28;
    MTXCopy(daPy_getLinkPlayerActorClass()->getModelJointMtx(9), *calc_mtx);
    MtxPosition(&cStack_1c, &cStack_28);
    switch (field_0x712) {
    case 0:
        if (cLib_addCalcPos(&current.pos, cStack_28, 0.05f, 5.0f, 0.0f) < 10.0f) {
            field_0x712++;
            field_0x714[0] = 200;
            mpMorf->setPlaySpeed(0.0f);
        }
        field_0x724 = cLib_targetAngleY(&current.pos, &cStack_28);
        cLib_addCalcAngleS2(&current.angle.y, field_0x724, 8, 0x1000);
        shape_angle.y = current.angle.y;
        break;
    case 1:
        cLib_chasePos(&current.pos, cStack_28, 3.0f);
        if (field_0x714[0] == 0) {
            mAction = ACTION_WAIT;
            field_0x712 = 1;
            field_0x714[1] = 100;
            speedF = 0.0f;
            speed.y = 0.0f;
            field_0x714[0] = 0;
            mpMorf->setPlaySpeed(1.0f);
        }
        break;
    }

    if ((!player->checkItemSwordEquip() || cStack_28.abs(field_0x73c) > 2.0f) &&
        !player->checkCutCharge())
    {
        mAction = ACTION_WAIT;
        field_0x712 = 1;
        field_0x714[1] = 100;
        speedF = 0.0f;
        speed.y = 0.0f;
        field_0x714[0] = 0;
        mpMorf->setPlaySpeed(1.0f);
    }
    field_0x73c = cStack_28;
}

void daObjTOMBO_c::MoveAction() {
    cXyz local_1c;
    daPy_py_c* player = daPy_getPlayerActorClass();
    switch (field_0x712) {
    case 0: {
        field_0x71c = cM_rndF(10.0f) + 4.0f;
        field_0x720 = cM_rndFX(3.0f);
        speedF = 4.0f;
        cXyz local_1c = home.pos - current.pos;
        local_1c.x += cM_rndFX(1000.0f);
        local_1c.z += cM_rndFX(1000.0f);
        field_0x724 = cM_atan2s(local_1c.x, local_1c.z);
        CheckGround();
        field_0x712++;
        break;
    }
    case 1:
        SpeedSet();
        cXyz* flamePos = player->getKandelaarFlamePos();
        if (flamePos != NULL) {
            field_0x724 = cLib_targetAngleY(&current.pos, flamePos);
        }
        if (field_0x714[0] == 0) {
            mAction = ACTION_WAIT;
            field_0x712 = 0;
            CheckWall();
        }
        break;
    }
}

void daObjTOMBO_c::Action() {
    if (fopAcM_checkHookCarryNow(this)) {
        if (CheckWater()) {
            fopAcM_cancelHookCarryNow(this);
        }
        home.pos = current.pos;
    }
    for (int i = 0; i < 3; i++) {
        field_0x714[i]--;
        if (field_0x714[i] <= 0) {
            field_0x714[i] = 0;
        }
    }

    switch (mAction) {
    case ACTION_WAIT:
        WaitAction();
        break;
    case ACTION_MOVE: {
        MoveAction();
        cLib_addCalcAngleS2(
            &shape_angle.x,
            cM_atan2s(field_0x720, JMAFastSqrt(speed.x * speed.x + speed.z * speed.z)) * -0.001f, 2,
            0x600);
        break;
    }
    case ACTION_LINK:
        LinkAction();
        break;
    }

    Insect_GetDemoMain();
}

void daObjTOMBO_c::ShopAction() {
    for (int i = 0; i < 3; i++) {
        field_0x714[i]--;
        if (field_0x714[i] <= 0) {
            field_0x714[i] = 0;
        }
    }
    switch (mAction) {
    case ACTION_WAIT:
        WaitAction();
        break;
    case ACTION_MOVE: {
        MoveAction();
        cLib_addCalcAngleS2(
            &shape_angle.x,
            cM_atan2s(field_0x720, JMAFastSqrt(speed.x * speed.x + speed.z * speed.z)) * -0.001f, 2,
            0x600);
        break;
    }
    case ACTION_LINK:
        LinkAction();
        break;
    }
}

void daObjTOMBO_c::Insect_Release() {
    field_0x56c = 1;
    mAction = ACTION_MOVE;
}

static f32 dummyLiteral1() { return 3.5f; }
static f32 dummyLiteral2() { return 12000.0f; }
static f32 dummyLiteral3() { return 4096.0f; }
static f32 dummyLiteral4() { return 0.5f; }
static f32 dummyLiteral5() { return 0.7f; }
static f32 dummyLiteral6() { return 0.25f; }

void daObjTOMBO_c::ParticleSet() {
    if (field_0x750 > field_0x754) {
        cLib_addCalc2(&mParticleScale, 0.0f, 1.0f, 1.0f);
    } else {
        cLib_addCalc2(&mParticleScale, 1.0f, 1.0f, 1.0f);
    }
    mParticleKey1 = dComIfGp_particle_set(mParticleKey1, 0xa1b, &current.pos, &tevStr, &shape_angle,
                                          0, 0xff, 0, -1, 0, 0, 0);
    f32 scale = mParticleScale;
    JPABaseEmitter* pEmitter = dComIfGp_particle_getEmitter(mParticleKey1);
    if (pEmitter != NULL) {
        pEmitter->setGlobalParticleScale(scale, scale);
    }
}

void daObjTOMBO_c::BoomChk() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (mIsHitByBoomerang) {
        cXyz& playerPos = fopAcM_GetPosition(player);
        cXyz cStack_a8(playerPos);
        cStack_a8.y += 100.0f;
        if (mBoomerangMove.posMove(&current.pos, &shape_angle.y, NULL, 0x1c00)) {
            mBoomerangMove.bgCheckAfterOffset(&current.pos);
            current.pos.y += 0.5f;
        } else {
            dBgS_LinChk linChk;
            linChk.Set(&cStack_a8, &current.pos, NULL);
            if (dComIfG_Bgsp().LineCross(&linChk)) {
                cM3dGPla pla;
                dComIfG_Bgsp().GetTriPla(linChk, &pla);
                current.pos = old.pos = linChk.GetCross();
                mAction = ACTION_MOVE;
                field_0x712 = 0;
                speedF = 5.0f;
                field_0x71c = 5.0f;
                mIsHitByBoomerang = false;
                field_0x718 = 100;
                mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Tombo", 6), 2, 5.0f, 1.0f,
                               0.0f, -1.0f);
                home.pos = current.pos;
            } else {
                home.pos = current.pos;
                mAction = ACTION_MOVE;
                field_0x712 = 0;
                mIsHitByBoomerang = false;
                speedF = 5.0f;
                field_0x71c = 5.0f;
                field_0x718 = 100;
                mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Tombo", 6), 2, 5.0f, 1.0f,
                                         0.0f, -1.0f);
                current.pos.y = old.pos.y = playerPos.y + 100.0f;
            }
        }
    }
}

static bool l_HIOInit;

static daObj_TomHIO_c l_HIO;

int daObjTOMBO_c::Execute() {
    if (ChkGetDemo()) {
        field_0x750 = field_0x754 + 10000.0f;
        mCreature.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
        Insect_GetDemoMain();
        ParticleSet();
        return 1;
    }

    attention_info.position = current.pos;
    eyePos = current.pos;
    eyePos.y += 10.0f;
    switch (field_0x75c) {
    case 0:
        Action();
        mParticleKey2 = dComIfGp_particle_set(mParticleKey2, 0xa1c,
        &current.pos, &tevStr, &shape_angle, 0,
        0xff, 0, -1, 0, 0, 0);
        SetCcSph();
        ObjHit();
        BoomChk();
        ParticleSet();
        break;
    case 1:
        ShopAction();
        break;
    }

    mCreature.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
    mAcch.CrrPos(dComIfG_Bgsp());
    mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mBtk->setPlaySpeed(l_HIO.mBtkSpeed);
    mBtk->play();
    mBrk->play();
    setBaseMtx();
    return 1;
}

void daObjTOMBO_c::ObjHit() {
    if (mSph.ChkTgHit()) {
        cCcD_Obj* tgHitObj = mSph.GetTgHitObj();
        if (tgHitObj->ChkAtType(AT_TYPE_BOOMERANG)) {
            if (!mIsHitByBoomerang) {
                mIsHitByBoomerang = true;
                dCcD_GObjInf* tgHitObj = (dCcD_GObjInf*)mSph.GetTgHitGObj();
                mBoomerangMove.initOffset(&current.pos, tgHitObj);
            }
        } else if (!tgHitObj->ChkAtType(AT_TYPE_NORMAL_SWORD) &&
                   tgHitObj->ChkAtType(AT_TYPE_HOOKSHOT))
        {
            cXyz cStack_28(0.0f, 0.0f, 0.0f);
            daPy_getPlayerActorClass()->setHookshotCarryOffset(fopAcM_GetID(this), &cStack_28);
            mAction = ACTION_MOVE;
            field_0x712 = 0;
            mIsHitByBoomerang = false;
            mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Tombo", 6), 2, 5.0f, 1.0f, 0.0f,
                           -1.0f);
            speedF = 8.0f;
            field_0x71c = cM_rndF(5.0f) + 8.0f;
        }
        mSph.ClrTgHit();
    }
}

void daObjTOMBO_c::Z_BufferChk() {
    cXyz local_5c;
    cXyz cStack_68;
    cStack_68 = current.pos;
    cStack_68.y += 20.0f;
    mDoLib_project(&cStack_68, &local_5c);
    camera_class* pCamera = dComIfGp_getCamera(0);
    f32 trimHeight;
    if (pCamera != NULL) {
        trimHeight = pCamera->mCamera.TrimHeight();
    } else {
        trimHeight = 0.0f;
    }
    if (local_5c.x > 0.0f && local_5c.x < FB_WIDTH && local_5c.y > trimHeight &&
        local_5c.y < FB_HEIGHT - trimHeight)
    {
        dComIfGd_peekZ(local_5c.x, local_5c.y, &field_0x754);
    }
    f32 near = dComIfGd_getView()->near;
    f32 far = dComIfGd_getView()->far;
    mDoLib_pos2camera(&cStack_68, &local_5c);
    local_5c.z += 50.0f;
    if (local_5c.z > 0.0f) {
        local_5c.z = 0.0f;
    }
    field_0x750 =
        ((near + ((far * near) / local_5c.z)) / (far - near) + 1.0f) * 16777215.0f;
}

int daObjTOMBO_c::Delete() {
    dComIfG_resDelete(&mPhase, "Tombo");
    if (mIsHIOOwner) {
        l_HIOInit = false;
        mDoHIO_DELETE_CHILD(l_HIO.mId);
    }
    if (heap != NULL) {
        mpMorf->stopZelAnime();
    }
    return 1;
}

void daObjTOMBO_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
}

inline int daObjTOMBO_c::Draw() {
    if (mDraw) {
        Z_BufferChk();
        J3DModel* morfModel = mpMorf->getModel();
        g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(morfModel, &tevStr);
        mBtk->entry(morfModel->getModelData());
        mBrk->entry(morfModel->getModelData());
        mpMorf->entryDL();
        if (!field_0x75c) {
            dComIfGd_setSimpleShadow(&current.pos, mAcch.GetGroundH(), 15.0f, mAcch.m_gnd, 0, -0.6f,
                                     dDlst_shadowControl_c::getSimpleTex());
        }
    }
    return 1;
}

static int daObjTOMBO_Draw(daObjTOMBO_c* i_this) {
    return i_this->Draw();
}

static int daObjTOMBO_Execute(daObjTOMBO_c* i_this) {
    return i_this->Execute();
}

static u16 const l_musiya_num[2] = {
    0x01A3, /* dSv_event_flag_c::F_0419 - Misc. - Dragonfly (M) */
    0x01A4, /* dSv_event_flag_c::F_0420 - Misc. - Dragonfly (F) */
};

bool daObjTOMBO_c::CreateChk() {
    u8 uVar4 = (fopAcM_GetParam(this) & 0xf00) >> 8;
    if (uVar4 == 0xf) {
        uVar4 = 0;
    }

    if (field_0x75c != 2) {
        if (uVar4 != 0) {
            if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[0]]) || !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[1]])) {
                return 0;
            }
        } else if (strcmp("R_SP160", dComIfGp_getStartStageName()) == 0 &&
            dComIfGp_getStartStageRoomNo() == 3)
        {
            if (dMenu_Insect_c::isCatchNotGiveInsect(l_tom_itemno[mSex])) {
                return 0;
            }
            
            if ((mSex == SEX_MALE &&
                    !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[mSex]])) ||
                (mSex == SEX_FEMALE &&
                    !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[mSex]])))
            {
                return 0;
            }
        }
    }
    return 1;
}

int daObjTOMBO_c::create() {
    fopAcM_ct(this, daObjTOMBO_c);
    int phase = dComIfG_resLoad(&mPhase, "Tombo");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("TOMBO PARAM %x\n", fopAcM_GetParam(this));
        field_0x75c = fopAcM_GetParam(this) & 0xf;
        if (field_0x75c == 2) {
            field_0x56c = 0;
            shape_angle.x -= 0x2000;
            fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
        } else {
            mDraw = true;
        }
        attention_info.distances[fopAc_attn_CARRY_e] = 0x5d;
        mSex = (fopAcM_GetParam(this) & 0x10) >> 4;
        setItemNo(l_tom_itemno[mSex]);
        setSaveBitNo(l_musiya_num[mSex]);
        if (mSex == SEX_FEMALE) {
            scale.set(l_HIO.mFemaleModelScale, l_HIO.mFemaleModelScale, l_HIO.mFemaleModelScale);
        } else if (mSex == SEX_MALE) {
            scale.set(l_HIO.mMaleModelScale, l_HIO.mMaleModelScale, l_HIO.mMaleModelScale);
        }
        if (!CreateChk()) {
            return cPhs_ERROR_e;
        }
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0xaa0)) {
            return cPhs_ERROR_e;
        }
        if (!l_HIOInit) {
            l_HIOInit = true;
            mIsHIOOwner = true;
            // Golden Dragonfly
            l_HIO.mId = mDoHIO_CREATE_CHILD("黄金蟲(トンボ)", &l_HIO);
        }
        mAcchCir.SetWall(30.0f, 50.0f);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), NULL, NULL);
        mAcch.ClrWaterNone();
        if (field_0x75c == 0) {
            attention_info.flags = 0;
            InitCcSph();
        }
        if (field_0x75c == 2) {
            if (strcmp("R_SP160", dComIfGp_getStartStageName()) == 0 &&
                dComIfGp_getStartStageRoomNo() == 3)
            {
                field_0x75c = 3;
            }
        }
        mAction = ACTION_WAIT;
        field_0x712 = 0;
        gravity = 0.0f;
        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -50.0f, -50.0f, -50.0f);
        fopAcM_SetMax(this, 50.0f, 50.0f, 50.0f);
        mCreature.init(&current.pos, &eyePos, 3, 1);
        daObjTOMBO_Execute(this);
    }
    return phase;
}

static int daObjTOMBO_IsDelete(daObjTOMBO_c* param_0) {
    return 1;
}

static actor_method_class l_daObjTOMBO_Method = {
    (process_method_func)daObjTOMBO_Create,
    (process_method_func)daObjTOMBO_Delete,
    (process_method_func)daObjTOMBO_Execute,
    (process_method_func)daObjTOMBO_IsDelete,
    (process_method_func)daObjTOMBO_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Tombo = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Tombo,         // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjTOMBO_c),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  487,                    // mPriority
  &l_daObjTOMBO_Method,   // sub_method
  0x000C0100,             // mStatus
  fopAc_ENV_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
