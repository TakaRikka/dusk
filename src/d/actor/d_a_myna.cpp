/**
 * @file d_a_myna.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_myna.h"
#include "d/actor/d_a_player.h"
#include "d/d_kankyo.h"
#include "d/d_msg_object.h"
#include "d/actor/d_a_obj_ss_item.h"
#include "d/actor/d_a_tag_myna_light.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/d_s_play.h"

static daMyna_c::ProcFunc init_proc[] = {
    &daMyna_c::attack_wait_init,
    &daMyna_c::attack_before_talk_init,
    &daMyna_c::attack_fly_init,
    &daMyna_c::attack_init,
    &daMyna_c::attack_after_talk_init,
    &daMyna_c::greet_wait_init,
    &daMyna_c::greet_talk_init,
    &daMyna_c::shopping_wait_init,
    &daMyna_c::shopping_talk_init,
    &daMyna_c::thanks_talk_init,
    &daMyna_c::byebye_talk_init,
    &daMyna_c::turn_on_start_init,
    &daMyna_c::turn_on_init,
    &daMyna_c::turn_on_end1_init,
    &daMyna_c::turn_on_end2_init,
    &daMyna_c::turn_on_end3_init,
    &daMyna_c::wolf_talk_init,
    &daMyna_c::attack_wait2_init,
    &daMyna_c::attack_before_talk2_init,
    &daMyna_c::attack_fly2_init,
    &daMyna_c::attack2_init,
    &daMyna_c::attack2_talk_init,
};

static daMyna_c::ProcFunc move_proc[] = {
    &daMyna_c::attack_wait_move,
    &daMyna_c::attack_before_talk_move,
    &daMyna_c::attack_fly_move,
    &daMyna_c::attack_move,
    &daMyna_c::attack_after_talk_move,
    &daMyna_c::greet_wait_move,
    &daMyna_c::greet_talk_move,
    &daMyna_c::shopping_wait_move,
    &daMyna_c::shopping_talk_move,
    &daMyna_c::thanks_talk_move,
    &daMyna_c::byebye_talk_move,
    &daMyna_c::turn_on_start_move,
    &daMyna_c::turn_on_move,
    &daMyna_c::turn_on_end1_move,
    &daMyna_c::turn_on_end2_move,
    &daMyna_c::turn_on_end3_move,
    &daMyna_c::wolf_talk_move,
    &daMyna_c::attack_wait2_move,
    &daMyna_c::attack_before_talk2_move,
    &daMyna_c::attack_fly2_move,
    &daMyna_c::attack2_move,
    &daMyna_c::attack2_talk_move,
};

static char* l_bckFileNameTBL[] = {
    "MYNA_hovering.bck", "MYNA_wait_a.bck",    "MYNA_pick_a.bck",    "MYNA_pick_b.bck",
    "MYNA_jump.bck",     "MYNA_sidestepL.bck", "MYNA_sidestepR.bck", "MYNA_lightup.bck",
    "MYNA_attack.bck",   "MYNA_talk_a.bck",    "MYNA_talk_b.bck",    "MYNA_wait_b.bck",
    "MYNA_wait_c.bck",   "MYNA_talk_c.bck",    "MYNA_wait_d.bck",
};

static char* l_btpFileNameTBL[] = {"MYNA.btp"};

static void createHeapCallBack(fopAc_ac_c* i_this) {
    daMyna_c* a_this = static_cast<daMyna_c*>(i_this);
    a_this->createHeap();
}

static int jntNodeCallBack(J3DJoint* i_jnt, int param_1) {
    if (param_1 == 0) {
        J3DModel* model_p = j3dSys.getModel();
        daMyna_c* myna_p = (daMyna_c*) model_p->getUserArea();
        if (myna_p != NULL) {
            myna_p->jntNodeCB(i_jnt, model_p);
        }
    }
    return 1;
}

class daMyna_HIO_c : public mDoHIO_entry_c {
public:
    virtual ~daMyna_HIO_c() {}

    daMyna_HIO_c() {
        static struct {
            /* 0x04 */ f32 scale;
            /* 0x08 */ f32 jump_anime;
            /* 0x0C */ f32 jump_speed;
            /* 0x10 */ f32 side_walking_speed;
            /* 0x14 */ f32 link_attack_height;
            /* 0x18 */ f32 link_approach_dist;
            /* 0x1C */ f32 streetlight_approach_dist;
            /* 0x20 */ f32 return_approach_dist;
            /* 0x24 */ s16 annihilation_return_wait;
            /* 0x26 */ s16 turn_angle_add_val;
            /* 0x28 */ s16 max_vibration_angle;
            /* 0x2A */ s16 min_vibration_angle;
            /* 0x2C */ s16 up_and_down_movement;
            /* 0x2E */ s16 heaving_speed;
            /* 0x30 */ s16 damage_timer;
        } aParam = {
            1.0f,
            1.0f,
            0.08f,
            0.4f,
            30.0f,
            45.0f,
            42.0f,
            2.0f,
            75,
            4,
            0x7fff,
            1500,
            5,
            0x2000,
            30,
        };
        memcpy(&scale, &aParam, sizeof(aParam));
    }

#if DEBUG
    void genMessage(JORMContext* ctext) {
        // scale:
        ctext->genSlider("スケール", &scale, 0.0f, 2.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genLabel("**********", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // turn angle add value
        ctext->genSlider("振向角加算値", &turn_angle_add_val, -0x8000, 0x7fff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // max vibration angle
        ctext->genSlider("振向角最大", &max_vibration_angle, -0x8000, 0x7fff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // min vibration angle
        ctext->genSlider("振向角最小", &min_vibration_angle, -0x8000, 0x7fff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genLabel("**********", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // jump anime
        ctext->genSlider("Jumpアニメ", &jump_anime, 0.0f, 5.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // jump speed
        ctext->genSlider("Jumpスピード", &jump_speed, 0.0f, 2.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // side walking speed
        ctext->genSlider("側歩スピード", &side_walking_speed, 0.0f, 5.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genLabel("**********", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // approach dist to streetlights?
        ctext->genSlider("外灯接近距離 ", &streetlight_approach_dist, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // return approach dist
        ctext->genSlider("戻り接近距離", &return_approach_dist, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // annihilation return wait
        ctext->genSlider("全滅戻りウエイト", &annihilation_return_wait, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genLabel("**********", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // link attack height
        ctext->genSlider("リンク攻撃高さ", &link_attack_height, -100.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // link approach dist
        ctext->genSlider("リンク接近距離", &link_approach_dist, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // up and down movement
        ctext->genSlider("上下揺れ具合", &up_and_down_movement, 0, 100, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // heaving speed
        ctext->genSlider("上下揺れ速度", &heaving_speed, 0, 0x7fff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // damage timer
        ctext->genSlider("ダメージタイマー", &damage_timer, 10, 500, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    }
#endif

    /* 0x04 */ f32 scale;
    /* 0x08 */ f32 jump_anime;
    /* 0x0C */ f32 jump_speed;
    /* 0x10 */ f32 side_walking_speed;
    /* 0x14 */ f32 link_attack_height;
    /* 0x18 */ f32 link_approach_dist;
    /* 0x1C */ f32 streetlight_approach_dist;
    /* 0x20 */ f32 return_approach_dist;
    /* 0x24 */ s16 annihilation_return_wait;
    /* 0x26 */ s16 turn_angle_add_val;
    /* 0x28 */ s16 max_vibration_angle;
    /* 0x2A */ s16 min_vibration_angle;
    /* 0x2C */ s16 up_and_down_movement;
    /* 0x2E */ s16 heaving_speed;
    /* 0x30 */ s16 damage_timer;
};

STATIC_ASSERT(sizeof(daMyna_HIO_c) == 0x34);

static daMyna_HIO_c l_HOSTIO;

static fopAc_ac_c* daMyna_targetActor[10];

static fopAc_ac_c* daMyna_subActor[10];

static daTag_MynaLight_c* daMyna_LightActor;

static int daMyna_actor_count;

static void* daMyna_searchSSItem(void* param_0, void* param_1) {
    if (fopAcM_IsActor(param_0) && (fopAcM_GetName(param_0) == PROC_OBJ_SSITEM ||
                                    fopAcM_GetName(param_0) == PROC_OBJ_SSDRINK ||
                                    fopAcM_GetName(param_0) == PROC_TAG_SSDRINK))
    {
        if ((fopAcM_GetParam(param_1) & 0xF0000000) == (fopAcM_GetParam(param_0) & 0xF0000000) &&
            daMyna_actor_count < 10)
        {
            fopAc_ac_c* pfVar4 = static_cast<fopAc_ac_c*>(param_0);
            if (fopAcM_GetName(param_0) == PROC_OBJ_SSITEM) {
                pfVar4 = static_cast<daObj_SSItem_c*>(param_0)->getExchangeItemPtr();
            }
            if (pfVar4 != NULL) {
                daMyna_targetActor[daMyna_actor_count] = pfVar4;
                daMyna_subActor[daMyna_actor_count] = static_cast<fopAc_ac_c*>(param_0);
                daMyna_actor_count++;
            }
        }
    }
    return 0;
}

static void* daMyna_searchLight(void* i_mynaLightTagActor, void* param_1) {
    if (fopAcM_IsActor(i_mynaLightTagActor) &&
        fopAcM_GetName(i_mynaLightTagActor) == PROC_TAG_MNLIGHT)
    {
        if ((fopAcM_GetParam(param_1) & 0xF0000000) ==
            (fopAcM_GetParam(i_mynaLightTagActor) & 0xF0000000))
        {
            daMyna_LightActor = static_cast<daTag_MynaLight_c*>(i_mynaLightTagActor);
        }
    }
    return 0;
}

static fopAc_ac_c* daMyna_evtTagActor0;

static fopAc_ac_c* daMyna_evtTagActor1;

static void* daMyna_searchEvtTag(void* i_evtTagActor, void* param_1) {
    if (fopAcM_IsActor(i_evtTagActor) && fopAcM_GetName(i_evtTagActor) == PROC_TAG_EVT) {
        if (((fopAcM_GetParam(i_evtTagActor) & 0xF000000) >> 24) == 0) {
            // Event range tag 0 search
            daMyna_evtTagActor0 = static_cast<fopAc_ac_c*>(i_evtTagActor);
            OS_REPORT("イベント範囲タグ０サーチ\n");
        } else if (((fopAcM_GetParam(i_evtTagActor) & 0xF000000) >> 24) == 1) {
            // Event range tag 1 search
            daMyna_evtTagActor1 = static_cast<fopAc_ac_c*>(i_evtTagActor);
            OS_REPORT("イベント範囲タグ１サーチ\n");
        }
        return 0;
    }
    return 0;
}

dCcD_SrcSph const daMyna_c::mCcDSph = {
    0, 0x400, 1, 0x1F, 0, 0, 0x79, 0xA, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 15.0f,
};

int daMyna_c::baseMotion00(int param_0) {
    if (param_0 != 0) {
        field_0x916 = 0;
        setAnimeType(1, 0.0f);
    }
    return 1;
}

int daMyna_c::baseMotion01(int param_0) {
    if (param_0 != 0) {
        field_0x916 = 1;
        setAnimeType(11, 0.0f);
    }
    return 1;
}

int daMyna_c::baseMotion02(int param_0) {
    if (param_0 != 0) {
        field_0x916 = 2;
        setAnimeType(12, 0.0f);
    }
    return 1;
}

int daMyna_c::baseMotion03(int param_0) {
    if (param_0 != 0) {
        field_0x916 = 3;
        setAnimeType(9, 0.0f);
    } else {
        if (field_0x91E != 0) {
            setAnimeType(11, 0.0f);
        }
    }
    return 1;
}

int daMyna_c::baseMotion04(int param_0) {
    if (param_0 != 0) {
        field_0x916 = 4;
        setAnimeType(10, 0.0f);
    } else {
        if (field_0x91E != 0) {
            setAnimeType(12, 0.0f);
        }
    }
    return 1;
}

int daMyna_c::baseMotion05(int param_0) {
    if (param_0 != 0) {
        field_0x916 = 5;
        setAnimeType(13, 0.0f);
    } else {
        if (field_0x91E != 0) {
            setAnimeType(14, 0.0f);
        }
    }
    return 1;
}

int daMyna_c::baseMotion06(int param_0) {
    if (param_0 != 0) {
        field_0x916 = 6;
        setAnimeType(14, 0.0f);
    }
    return 1;
}

daMyna_c::BaseMotionFunc daMyna_c::mBaseMotionTBL[] = {
    &daMyna_c::baseMotion00, &daMyna_c::baseMotion01, &daMyna_c::baseMotion02,
    &daMyna_c::baseMotion03, &daMyna_c::baseMotion04, &daMyna_c::baseMotion05,
    &daMyna_c::baseMotion06,
};

int daMyna_c::create() {
    fopAcM_ct(this, daMyna_c);
    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, "Npc_myna");
    if (phase != cPhs_COMPLEATE_e) {
        return phase;
    }

    if (!fopAcM_entrySolidHeap(this, (heapCallbackFunc)&createHeapCallBack, 0x21F0)) {
        return cPhs_ERROR_e;
    }

#if DEBUG
    // myna bird
    l_HOSTIO.entryHIO("九官鳥");
#endif
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    mCreature.init(&current.pos, &eyePos, 3, 1);
    initiate();
    setRoomNo();
    execute();
    field_0x844.set(field_0x82C);
    field_0x850.set(field_0x838);
    return phase;
}

int daMyna_c::destroy() {
    dComIfG_resDelete(&mPhase, "Npc_myna");
    if (heap != NULL) {
        mpMorf->stopZelAnime();
    }

#if DEBUG
    l_HOSTIO.removeHIO();
#endif
    return 1;
}

int daMyna_c::draw() {
    J3DModel* model = mpMorf->getModel();
    J3DModelData* modelData = model->getModelData();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    if (cLib_checkBit<u16>(u16(field_0x914), 0x40)) {
        mBtpAnm.entry(modelData);
    }
    fopAcM_setEffectMtx(this, modelData);
    mpMorf->entryDL();
    if (cLib_checkBit<u16>(u16(field_0x914), 0x40)) {
        mBtpAnm.remove(modelData);
    }
    return 1;
}

int daMyna_c::execute() {
    u8 uVar1 = field_0x92C;
    u8 uVar2 = field_0x935;
    bool isTalkNow = dMsgObject_isTalkNowCheck();
    setItemInfo();
    int iVar1 = chkEvent();
    if (daMyna_LightActor == NULL) {
        fpcM_Search(daMyna_searchLight, this);
    }

    setRoomNo();
    attention_info.position.set(current.pos.x, current.pos.y + 40.0f, current.pos.z);
    eyePos.set(attention_info.position);
    if (mDamageTimer > 0) {
        mDamageTimer--;
    }

    if (isTalkNow) {
        talkAnime(mMsgFlow.getMsg());
    } else if (field_0x824 != 0) {
        field_0x824 = 0;
    }

    if (iVar1 != 0) {
        (this->*move_proc[field_0x92C])();
        orderEvent();
    }

    if (!isTalkNow) {
        playDefaultWaitAnime();
        setDefaultWaitAnime(uVar2);
    }

    if (uVar1 != field_0x92C) {
        (this->*init_proc[field_0x92C])();
    }

    if (uVar2 != field_0x935) {
        if (field_0x935 == 1) {
            field_0x924 = cM_rndF(100.0f) + 100.0f;
        }

        animeControl();
    }

    animePlay();
    set_mtx();
    shape_angle = current.angle;
    shape_angle.y = -current.angle.y;
    setCollision();
    checkDead();
    return 1;
}

int daMyna_c::createHeap() {
    J3DModelData* aMdlData_p = NULL;
    aMdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes("Npc_myna", "MYNA.bmd"));
    JUT_ASSERT(803, NULL != aMdlData_p);
    mpMorf = new mDoExt_McaMorfSO(aMdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mCreature, 0x80000,
                                  0x11020084);
    if (mpMorf != NULL && mpMorf->getModel() == NULL) {
        mpMorf->stopZelAnime();
        mpMorf = NULL;
    }

    if (mpMorf == NULL) {
        return 0;
    }

    J3DModel* model_p = mpMorf->getModel();
    for (u16 i = 0; i < aMdlData_p->getJointNum(); i++) {
        aMdlData_p->getJointNodePointer(i)->setCallBack(jntNodeCallBack);
    }

    field_0x916 = 7;
    field_0x914 = 0;
    field_0x918 = 0;
    field_0x91A = 0;
    field_0x936 = 0;
    int reg_r24 = 2;
    J3DAnmTexPattern* anmTexPattern = getTexPtrnAnm(l_btpFileNameTBL[field_0x936]);
    if (anmTexPattern != NULL) {
        setBtpAnm(anmTexPattern, mpMorf->getModel()->getModelData(), 1.0f, reg_r24);
        cLib_onBit<u16>(field_0x914, 0x44);
    }

    return 1;
}

int daMyna_c::jntNodeCB(J3DJoint* i_jnt, J3DModel* i_model) {
    J3DJoint* my_joint = i_jnt;
    int jntNo = my_joint->getJntNo();
    cXyz spC;
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));
    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

void daMyna_c::attack_wait_init() {
    setAnimeType(1, 0.0f);
    field_0x92A = 0;
}

void daMyna_c::attack_wait_move() {
    if (current.angle.y != home.angle.y) {
        cLib_addCalcAngleS(&current.angle.y, home.angle.y, 4, 32767, 1500);
        cLib_addCalcAngleS(&current.angle.x, home.angle.x, 4, 32767, 1500);
        if (abs(s16(current.angle.y - home.angle.y)) < 128) {
            current.angle = home.angle;
        }
    }
    if (!daPy_py_c::checkNowWolf() && daMyna_evtTagActor0 != NULL) {
        cXyz var1(1.0f, 1.0f, 1.0f);
        if (!dComIfGs_isSaveSwitch(0x3C)) {
            if (chkPlayerInEvtArea(daMyna_evtTagActor0, var1)) {
                if (!isEventFlag(1)) {
                    eventInfo.onCondition(1);
                    fopAcM_orderSpeakEvent(this, 0, 0);
                    if (eventInfo.checkCommandTalk()) {
                        field_0x92C = 1;
                    }
                } else {
                    field_0x92C = 2;
                }
            }
        } else if (field_0x92A < l_HOSTIO.annihilation_return_wait) {
            field_0x92A++;
        } else {
            eventInfo.onCondition(1);
            fopAcM_orderSpeakEvent(this, 0, 0);
            if (eventInfo.checkCommandTalk()) {
                field_0x92C = 4;
            }
        }
    }
}

void daMyna_c::attack_before_talk_init() {
    mMsgFlow.init(this, field_0x91C, 0, NULL);
    setAnimeType(10, 0.0f);
}

void daMyna_c::attack_before_talk_move() {
    if (eventInfo.checkCommandTalk() && mMsgFlow.doFlow(this, NULL, 0) != 0) {
        dComIfGp_event_reset();
        onEventFlag(1);
        field_0x92C = 2;
    }
}

void daMyna_c::attack_fly_init() {
    setAnimeType(0, 0.0f);
    field_0x91A = 0;
}

void daMyna_c::attack_fly_move() {
    if (daMyna_evtTagActor0 != NULL) {
        cXyz var1(1.0f, 1.0f, 1.0f);
        fly_body_wave();
        if (mDamageTimer > 0 && dComIfGs_isSaveSwitch(0x3C)) {
            mDamageTimer = 0;
        }
        if (chkPlayerInEvtArea(daMyna_evtTagActor0, var1) && !daPy_py_c::checkNowWolf() &&
            !dComIfGs_isSaveSwitch(0x3C) && mDamageTimer == 0)
        {
            mStatus.Move();
            if (fly_attack_move()) {
                field_0x92C = 3;
            }
        } else if (fly_return_move()) {
            field_0x92C = 0;
        }
    }
}

void daMyna_c::attack_init() {
    setAnimeType(8, 0.0f);
    mSph.OnAtVsPlayerBit();
}

void daMyna_c::attack_move() {
    mStatus.Move();
    if (field_0x91E != 0) {
        field_0x92C = 2;
    }
    if (mSph.ChkAtHit()) {
        mDamageTimer = l_HOSTIO.damage_timer;
        mSph.OffAtVsPlayerBit();
    }
}

void daMyna_c::attack_after_talk_init() {
    mMsgFlow.init(this, field_0x91C, 0, NULL);
}

void daMyna_c::attack_after_talk_move() {
    if (eventInfo.checkCommandTalk() && mMsgFlow.doFlow(this, NULL, 0) != 0) {
        dComIfGp_event_reset();
        onEventFlag(2);
        field_0x91C = 0x139;
        field_0x92C = 5;
    }
}

void daMyna_c::greet_wait_init() {
    setAnimeType(1, 0.0f);
}

void daMyna_c::greet_wait_move() {
    if (!daPy_py_c::checkNowWolf()) {
        if (daMyna_evtTagActor0 != NULL) {
            cXyz sp8(1.0f, 1.0f, 1.0f);
            if (chkPlayerInEvtArea(daMyna_evtTagActor0, sp8)) {
                eventInfo.onCondition(1);
                fopAcM_orderSpeakEvent(this, 0, 0);
                if (eventInfo.checkCommandTalk()) {
                    field_0x92C = 6;
                }
            } else {
                if (isEventFlag(0)) {
                    eventInfo.onCondition(1);
                    fopAcM_orderSpeakEvent(this, 0, 0);
                    if (eventInfo.checkCommandTalk()) {
                        field_0x92C = 10;
                    }
                }
            }
        } else {
            if (fopAcM_searchPlayerDistanceXZ(this) < 350.0f) {
                eventInfo.onCondition(1);
                fopAcM_orderSpeakEvent(this, 0, 0);
                if (eventInfo.checkCommandTalk()) {
                    field_0x92C = 6;
                }
            } else {
                if (isEventFlag(0)) {
                    eventInfo.onCondition(1);
                    fopAcM_orderSpeakEvent(this, 0, 0);
                    if (eventInfo.checkCommandTalk()) {
                        field_0x92C = 10;
                    }
                }
            }
        }
    }
}

void daMyna_c::greet_talk_init() {
    dMsgObject_setTotalPayment(0);
    dMsgObject_setTotalPrice(0);
    field_0x922 = dMsgObject_getTotalPayment();
    field_0x920 = dMsgObject_getTotalPrice();
    dMsgObject_getMsgObjectClass()->setNowTotalPrice(0);
    dMsgObject_getMsgObjectClass()->setNowTotalPayment(0);

    switch (field_0x932) {
    case 0:
        field_0x91C = getFlowNodeNum();
        break;
    case 1:
        field_0x91C = getFlowNodeNum() + 1;
        break;
    case 2:
        field_0x91C = getFlowNodeNum() + 2;
        break;
    }

    setAnimeType(9, 0.0f);
    mMsgFlow.init(this, field_0x91C, 0, NULL);
}

void daMyna_c::greet_talk_move() {
    if (eventInfo.checkCommandTalk() && mMsgFlow.doFlow(this, NULL, 0) != 0) {
        dComIfGp_event_reset();
        /* dSv_event_flag_c::F_0802 - Faron Woods - Trill attacks when stealing */
        if (dComIfGs_isEventBit(u16(dSv_event_flag_c::saveBitLabels[802]))) {
            field_0x937 = 20;
            field_0x92C = 19;
        } else {
            field_0x92C = 7;
        }
    }
}

void daMyna_c::shopping_wait_init() {
    setAnimeType(1, 0.0f);
}

void daMyna_c::shopping_wait_move() {
    if (!daPy_py_c::checkNowWolf()) {
        /* dSv_event_flag_c::F_0802 - Faron Woods - Trill attacks when stealing */
        if (dComIfGs_isEventBit(u16(dSv_event_flag_c::saveBitLabels[802]))) {
            field_0x92C = 17;
            field_0x937 = 20;
            return;
        }

        daPy_py_c* player = daPy_getPlayerActorClass();
        bool bVar1 = false;
        s16 now_total_payment = dMsgObject_getMsgObjectClass()->getNowTotalPayment();
        s16 nowTotalPrice = dMsgObject_getMsgObjectClass()->getNowTotalPrice();
        if (dMsgObject_getMsgObjectClass()->isPaymentFlag()) {
            eventInfo.onCondition(1);
            fopAcM_orderSpeakEvent(this, 0, 0);
            if (eventInfo.checkCommandTalk()) {
                field_0x92C = 9;
                dMsgObject_getMsgObjectClass()->offPaymentFlag();
            }

            return;
        }

        if (player->getGrabUpEnd()) {
            for (int i = 0; i < 10; i++) {
                if (mShopItems[i].mTargetActorID == player->getGrabActorID() &&
                    mShopItems[i].mItemStatus == 2)
                {
                    dMsgObject_addTotalPrice(mShopItems[i].mValueNum);
                    field_0x931 = i;
                    if (mShopItems[i].mFlowNodeNum != 0xFFFF) {
                        eventInfo.onCondition(1);
                        fopAcM_orderSpeakEvent(this, 0, 0);
                    }
                }
            }
        }

        if (field_0x931 != 0xFF) {
            if (mShopItems[field_0x931].mTargetActorID != player->getGrabActorID() &&
                mShopItems[field_0x931].mItemStatus == 2)
            {
                dMsgObject_addTotalPrice(-mShopItems[field_0x931].mValueNum);
                field_0x931 = -1;
            }
        }

        /* dSv_event_flag_c::F_0203 - Shop - First tried to steal from unnmaned shop (Havent checked donation box) */
        if (!dComIfGs_isEventBit(u16(dSv_event_flag_c::saveBitLabels[203]))
            && daMyna_evtTagActor1 != NULL)
        {
            cXyz stack_2c(1.0f, 1.0f, 1.0f);
            if (chkPlayerInEvtArea(daMyna_evtTagActor1, stack_2c)) {
                if (isEventFlag(0)) {
                    eventInfo.onCondition(1);
                    fopAcM_orderSpeakEvent(this, 0, 0);
                    bVar1 = true;
                    field_0x933 = 1;
                } else if (field_0x931 != 0xFF && mShopItems[field_0x931].mItemStatus == 2)
                {
                    eventInfo.onCondition(1);
                    fopAcM_orderSpeakEvent(this, 0, 0);
                    bVar1 = true;
                    field_0x933 = 1;
                }
            }
        }

        if (daMyna_evtTagActor0 != NULL) {
            cXyz stack_38(1.0f, 1.0f, 1.0f);
            if (!chkPlayerInEvtArea(daMyna_evtTagActor0, stack_38)) {
                if (nowTotalPrice > 0) {
                    eventInfo.onCondition(1);
                    fopAcM_orderSpeakEvent(this, 0, 0);
                    bVar1 = true;
                }
            }
        } else if (fopAcM_searchPlayerDistanceXZ(this) > 450.0f) {
            if (isEventFlag(0)) {
                eventInfo.onCondition(1);
                fopAcM_orderSpeakEvent(this, 0, 0);
                bVar1 = true;
            } else if (field_0x931 != 0xFF && mShopItems[field_0x931].mItemStatus == 2) {
                eventInfo.onCondition(1);
                fopAcM_orderSpeakEvent(this, 0, 0);
                bVar1 = true;
            }
        }

        if (eventInfo.checkCommandTalk()) {
            if (!bVar1) {
                field_0x92C = 8;
            } else {
                field_0x92C = 10;
            }
        }
    }
}

void daMyna_c::shopping_talk_init() {
    field_0x828 = fopAcM_SearchByID(mShopItems[field_0x931].mTargetActorID);
    field_0x91C = mShopItems[field_0x931].mFlowNodeNum;
    mMsgFlow.init(this, field_0x91C, 0, NULL);
}

void daMyna_c::shopping_talk_move() {
    if (eventInfo.checkCommandTalk() && mMsgFlow.doFlow(this, NULL, 0) != 0) {
        dComIfGp_event_reset();
        field_0x828 = NULL;
        field_0x92C = 7;
    }
}

void daMyna_c::thanks_talk_init() {
    s16 nowTotalPayment = dMsgObject_getMsgObjectClass()->getNowTotalPayment();
    s16 nowTotalPrice = dMsgObject_getMsgObjectClass()->getNowTotalPrice();
    if (nowTotalPayment == nowTotalPrice) {
        field_0x91C = getFlowNodeNum() + 6;
    } else if (nowTotalPayment > nowTotalPrice) {
        field_0x91C = getFlowNodeNum() + 7;
    } else {
        field_0x91C = getFlowNodeNum() + 8;
    }
    mMsgFlow.init(this, field_0x91C, 0, NULL);
}

void daMyna_c::thanks_talk_move() {
    if (eventInfo.checkCommandTalk() && mMsgFlow.doFlow(this, NULL, 0) != 0) {
        dComIfGp_event_reset();
        field_0x92C = 7;
    }
}

void daMyna_c::byebye_talk_init() {
    int payment_diff = dMsgObject_getTotalPayment() - field_0x922;
    int price_diff = dMsgObject_getTotalPrice() - field_0x920;
    if (payment_diff >= price_diff) {
        field_0x932 = 0;
    } else if (payment_diff > 0) {
        field_0x932 = 1;
    } else {
        field_0x932 = 2;
    }

    switch (field_0x932) {
    case 0:
        field_0x91C = getFlowNodeNum() + 3;
        break;
    case 1:
        field_0x91C = getFlowNodeNum() + 4;
        break;
    case 2:
        field_0x91C = getFlowNodeNum() + 5;
        break;
    }

    if (field_0x933 == 0) {
        // To force stack reg to use r31 in dbg asm:
        int unused;
        field_0x922 = dMsgObject_getTotalPayment();
        field_0x920 = dMsgObject_getTotalPrice();
        dMsgObject_getMsgObjectClass()->setNowTotalPrice(0);
        dMsgObject_getMsgObjectClass()->setNowTotalPayment(0);
        offEventFlag(0);
    }

    mMsgFlow.init(this, field_0x91C, 0, NULL);
}

void daMyna_c::byebye_talk_move() {
    bool bVar1 = true;
    if (eventInfo.checkCommandTalk() && mMsgFlow.doFlow(this, NULL, 0) != 0) {
        if (field_0x933 == 0) {
            if (field_0x931 != 0xFF) {
                mShopItems[field_0x931].mItemStatus = 3;
                field_0x931 = -1;
            }
            for (int i = 0; i < mNumShopItems; i++) {
                if (mShopItems[i].mItemStatus == 2) {
                    bVar1 = false;
                }
            }
        } else {
            field_0x933 = 0;
            bVar1 = false;
        }

        dComIfGp_event_reset();
        /* dSv_event_flag_c::F_0802 - Faron Woods - Trill attacks when stealing */
        if (dComIfGs_isEventBit(u16(dSv_event_flag_c::saveBitLabels[802]))) {
            field_0x92C = 0x11;
            field_0x937 = 0x14;
        } else if (bVar1) {
            field_0x92C = 7;
        } else {
            field_0x92C = 7;
        }
    }
}

void daMyna_c::turn_on_start_init() {
    setAnimeType(4, 0.0f);
    if (daMyna_LightActor->getTurnOnFlag()) {
        mCreature.startCreatureVoice(JAISoundID(Z2SE_MYNA_V_LIGHT_ON), -1);
    } else {
        mCreature.startCreatureVoice(JAISoundID(Z2SE_MYNA_V_LIGHT_OFF), -1);
    }
}

void daMyna_c::turn_on_start_move() {
    cXyz stack_28;
    cXyz local_34;
    cXyz local_40;

    local_40.x = daMyna_LightActor->current.pos.x;
    local_40.y = current.pos.y;
    local_40.z = daMyna_LightActor->current.pos.z;

    cXyz local_4c = local_40 - current.pos;

    s16 sVar2 = cM_atan2s(local_4c.x, local_4c.z);
    f32 fVar1 = 0.5f * ((field_0x82C.y - field_0x844.y) + (field_0x838.y - field_0x850.y));

    local_34.x = 0.0f;
    local_34.y = 0.0f;
    local_34.z = l_HOSTIO.jump_speed * (fVar1 * fVar1);

    cLib_offsetPos(&stack_28, &current.pos, sVar2, &local_34);
    current.pos = stack_28;

    cXyz streetlight_to_curpos = local_40 - current.pos;
    if (streetlight_to_curpos.abs() < l_HOSTIO.streetlight_approach_dist) {
        field_0x92C = 12;
    }
}

void daMyna_c::turn_on_init() {
    setAnimeType(7, 0.0f);
}

void daMyna_c::turn_on_move() {
    if (field_0x91E != 0) {
        daMyna_LightActor->setTurnOnOffChange();
        field_0x92C = 13;
    }
}

void daMyna_c::turn_on_end1_init() {
    setAnimeType(4, 0.0f);
}

void daMyna_c::turn_on_end1_move() {
    cXyz local_18 = home.pos - current.pos;
    s16 sVar2 = cM_atan2s(local_18.x, local_18.z);
    cLib_addCalcAngleS(&current.angle.y, sVar2, l_HOSTIO.turn_angle_add_val, l_HOSTIO.max_vibration_angle,
                       l_HOSTIO.min_vibration_angle);

    if (abs(current.angle.y - sVar2) <= 16) {
        current.angle.y = sVar2;
        field_0x92C = 14;
    }
}

void daMyna_c::turn_on_end2_init() {
    if (field_0x935 != 4) {
        setAnimeType(4, 0.0f);
    }
}

void daMyna_c::turn_on_end2_move() {
    cXyz stack_24;
    cXyz local_30;
    cXyz local_3c = home.pos - current.pos;

    s16 sVar2 = cM_atan2s(local_3c.x, local_3c.z);
    f32 fVar1 = ((field_0x82C.y - field_0x844.y) + (field_0x838.y - field_0x850.y)) * 0.5f;

    local_30.x = 0.0f;
    local_30.y = 0.0f;
    local_30.z = l_HOSTIO.jump_speed * (fVar1 * fVar1);

    cLib_offsetPos(&stack_24, &current.pos, sVar2, &local_30);
    current.pos = stack_24;

    cXyz home_to_curpos = home.pos - current.pos;
    if (home_to_curpos.abs() < l_HOSTIO.return_approach_dist) {
        current.pos.set(home.pos);
        field_0x92C = 15;
    }
}

void daMyna_c::turn_on_end3_init() {
    /* empty function */
}

void daMyna_c::turn_on_end3_move() {
    cLib_addCalcAngleS(&current.angle.y, home.angle.y, l_HOSTIO.turn_angle_add_val, l_HOSTIO.max_vibration_angle,
                       l_HOSTIO.min_vibration_angle);

    if (abs(current.angle.y - home.angle.y) <= 16) {
        current.angle.y = home.angle.y;
        field_0x92C = field_0x92D;
    }
}

void daMyna_c::wolf_talk_init() {
    mMsgFlow.init(this, 0xC9, 0, NULL);
}

void daMyna_c::wolf_talk_move() {
    if (mMsgFlow.doFlow(this, NULL, 0) != 0) {
        dComIfGp_event_reset();
        field_0x92C = field_0x92E;
    }
}

void daMyna_c::attack_wait2_init() {
    setAnimeType(1, 0.0f);
    field_0x92A = 0;
}


void daMyna_c::attack_wait2_move() {
    if (daPy_py_c::checkNowWolf()) {
        field_0x92C = 7;
    } else {
        if (current.angle.y != home.angle.y) {
            cLib_addCalcAngleS(&current.angle.y, home.angle.y, 4, 0x7FFF, 0x5DC);
            cLib_addCalcAngleS(&current.angle.x, home.angle.x, 4, 0x7FFF, 0x5DC);
            if (abs(s16(current.angle.y - home.angle.y)) < 128) {
                current.angle = home.angle;
            }
        }

        if (daMyna_evtTagActor0 != NULL) {
            cXyz stack_1c(1.0f, 1.0f, 1.0f);
            /* dSv_event_flag_c::F_0802 - Faron Woods - Trill attacks when stealing */
            if (dComIfGs_isEventBit(u16(dSv_event_flag_c::saveBitLabels[802]))) {
                if (chkPlayerInEvtArea(daMyna_evtTagActor0, stack_1c)) {
                    field_0x937 = 20;
                    field_0x92C = 19;
                }
            } else {
                if (field_0x939 == 1) {
                    current.angle = home.angle;
                    eventInfo.onCondition(1);
                    fopAcM_orderSpeakEvent(this, 0, 0);
                    if (eventInfo.checkCommandTalk()) {
                        field_0x92C = 21;
                    }
                } else if (field_0x92A < l_HOSTIO.annihilation_return_wait) {
                    field_0x92A++;
                } else {
                    field_0x92C = 7;
                }
            }
        }
    }
}


void daMyna_c::attack_before_talk2_init() {
    mMsgFlow.init(this, field_0x91C, 0, NULL);
    setAnimeType(10, 0.0f);
}

void daMyna_c::attack_before_talk2_move() {
    if (eventInfo.checkCommandTalk() && mMsgFlow.doFlow(this, NULL, 0) != 0) {
        dComIfGp_event_reset();
        field_0x92C = 19;
    }
}

void daMyna_c::attack_fly2_init() {
    setAnimeType(0, 0.0f);
    field_0x91A = 0;
}

void daMyna_c::attack_fly2_move() {
    if (daMyna_evtTagActor0 != NULL) {
        cXyz stack_2c(1.0f, 1.0f, 1.0f);
        fly_body_wave();

        if (mDamageTimer > 0 && field_0x937 == 0) {
            mDamageTimer = 0;
        }

        if (dMsgObject_getTotalPayment() > field_0x922) {
            field_0x937 = 0;
            /* dSv_event_flag_c::F_0802 - Faron Woods - Trill attacks when stealing */
            dComIfGs_onEventBit(u16(dSv_event_flag_c::saveBitLabels[802]));
            field_0x939 = 1;
        }

        if (chkPlayerInEvtArea(daMyna_evtTagActor0, stack_2c) && !daPy_py_c::checkNowWolf() &&
            field_0x937 != 0 && mDamageTimer == 0)
        {
            mStatus.Move();
            if (fly_attack_move()) {
                field_0x92C = 20;
            }
        } else if (fly_return_move()) {
            if (field_0x937 == 0) {
                /* dSv_event_flag_c::F_0802 - Faron Woods - Trill attacks when stealing */
                dComIfGs_offEventBit(u16(dSv_event_flag_c::saveBitLabels[802]));
            }
            field_0x92C = 17;
        }
    }
}

void daMyna_c::attack2_init() {
    setAnimeType(8, 0.0f);
    mSph.OnAtVsPlayerBit();
}

void daMyna_c::attack2_move() {
    mStatus.Move();
    if (field_0x91E != 0) {
        field_0x92C = 19;
    }

    if (mSph.ChkAtHit()) {
        mDamageTimer = l_HOSTIO.damage_timer;
        mSph.OffAtVsPlayerBit();
        if (field_0x937 != 0) {
            field_0x937--;
            if (dComIfGs_getLife() <= 1) {
                /* dSv_event_flag_c::F_0802 - Faron Woods - Trill attacks when stealing */
                dComIfGs_offEventBit(u16(dSv_event_flag_c::saveBitLabels[802]));
                field_0x937 = 0;
            }
        }
    }
}

void daMyna_c::attack2_talk_init() {
    field_0x91C = 214;
    mMsgFlow.init(this, field_0x91C, 0, NULL);
}

void daMyna_c::attack2_talk_move() {
    if (eventInfo.checkCommandTalk() && mMsgFlow.doFlow(this, NULL, 0) != 0) {
        dComIfGp_event_reset();
        field_0x828 = NULL;
        field_0x92C = 7;
        dMsgObject_setTotalPayment(0);
        dMsgObject_setTotalPrice(0);
        field_0x922 = dMsgObject_getTotalPayment();
        field_0x920 = dMsgObject_getTotalPrice();
        dMsgObject_getMsgObjectClass()->setNowTotalPrice(0);
        dMsgObject_getMsgObjectClass()->setNowTotalPayment(0);
        dMsgObject_getMsgObjectClass()->offPaymentFlag();
        field_0x939 = 0;
    }
}

void daMyna_c::soldoutItem(fpc_ProcID i_itemId) {
    for (int i = 0; i < mNumShopItems; i++) {
        if (i_itemId == mShopItems[i].mTargetActorID) {
            mShopItems[i].mItemStatus = 3;
        }
    }
}

bool daMyna_c::fly_attack_move() {
    daPy_py_c* playerActorClass = daPy_getPlayerActorClass();
    cXyz vec_0x68;
    cXyz plyr_head_top_pos;
    cXyz vec_0x50;
    cXyz vec_0x44;

    plyr_head_top_pos.set(playerActorClass->getHeadTopPos());
    plyr_head_top_pos.y += l_HOSTIO.link_attack_height;
    vec_0x50.set(field_0x85C);
    vec_0x44 = current.pos - vec_0x50;
    vec_0x68 = plyr_head_top_pos - vec_0x50;

    s16 sVar1 = -cM_atan2s(vec_0x68.y, JMAFastSqrt(vec_0x68.x * vec_0x68.x + vec_0x68.z * vec_0x68.z));
    s16 sVar2 = cM_atan2s(vec_0x68.x, vec_0x68.z);
    cLib_addCalcAngleS(&current.angle.y, sVar2, 4, 0x7FFF, 1500);
    cLib_addCalcAngleS(&current.angle.x, sVar1, 4, 0x7FFF, 1500);
    cLib_addCalcPos2(&vec_0x50, plyr_head_top_pos, 0.5f + YREG_F(11), 15.0f + YREG_F(12));
    cXyz vec_0x38 = plyr_head_top_pos - vec_0x50;
    current.pos = vec_0x50 + vec_0x44;
    if (vec_0x38.abs() < l_HOSTIO.link_approach_dist) {
        return true;
    } else {
        return false;
    }
}

bool daMyna_c::fly_return_move() {
    cXyz local_30;
    if (mDamageTimer > 0) {
        cXyz stack_3c;
        daPy_py_c* playerActorClass = daPy_getPlayerActorClass();
        stack_3c.set(playerActorClass->getHeadTopPos());

        local_30 = stack_3c - current.pos;
        s16 sVar1 = -cM_atan2s(local_30.y, JMAFastSqrt(local_30.x * local_30.x + local_30.z * local_30.z));
        s16 sVar2 = cM_atan2s(local_30.x, local_30.z);

        cLib_addCalcAngleS(&current.angle.y, sVar2, 4, 0x7FFF, 0x5DC);
        cLib_addCalcAngleS(&current.angle.x, sVar1, 4, 0x7FFF, 0x5DC);
        cLib_addCalcPos2(&current.pos, home.pos, 0.001f + YREG_F(9), 100.0f + YREG_F(10));
    } else {
        cXyz vec_0x38;
        vec_0x38 = home.pos;
        local_30 = vec_0x38 - current.pos;
        s16 sVar1 = -cM_atan2s(local_30.y, JMAFastSqrt(local_30.x * local_30.x + local_30.z * local_30.z));
        s16 sVar2 = cM_atan2s(local_30.x, local_30.z);

        cLib_addCalcAngleS(&current.angle.y, sVar2, 4, 0x7FFF, 0x5DC);
        cLib_addCalcAngleS(&current.angle.x, sVar1, 4, 0x7FFF, 0x5DC);
        cLib_addCalcPos2(&current.pos, vec_0x38, 0.5f, 25.0f);

        cXyz local_44 = vec_0x38 - current.pos;
        if (local_44.abs() < 3.0f) {
            current.pos.set(home.pos);
            return true;
        }
    }
    return false;
}

void daMyna_c::fly_body_wave() {
    if (field_0x935 == 0) {
        field_0x91A += l_HOSTIO.heaving_speed;
        f32 fVar1 = cM_scos(field_0x91A) * l_HOSTIO.up_and_down_movement;
        current.pos.y += fVar1;
        field_0x85C.y += fVar1;
    }
}

void daMyna_c::talkAnime(msg_class* param_0) {
    u32 uVar4 = 0;
    if (param_0 != NULL && this == dComIfGp_event_getTalkPartner()) {
        switch (param_0->mode) {
        case 2:
            field_0x81C = 1;
            break;
        default:
            if (param_0->msg_idx != field_0x824) {
                field_0x81C = 1;
            }
        }

        if (param_0->mode == 6 && field_0x81C != 0) {
            field_0x81C = 0;
            field_0x820 = dComIfGp_getMesgAnimeAttrInfo();
            field_0x824 = param_0->msg_idx;
            uVar4 = 1;
        }
    }

    if (field_0x824 != 0 && field_0x820 >= 0 && field_0x820 < 7 &&
        mBaseMotionTBL[field_0x820] != NULL)
    {
        (this->*mBaseMotionTBL[field_0x820])(uVar4);
    }
}

int daMyna_c::chkEvent() {
    int retVal = 1;
    if (daPy_py_c::checkNowWolf()) {
        if (!dComIfGp_getEvent()->isOrderOK()) {
            retVal = 0;
            if (eventInfo.checkCommandTalk()) {
                if (field_0x92C == 0x10) {
                    (this->*move_proc[field_0x92C])();
                    retVal = 0;
                } else {
                    if (dComIfGp_event_chkTalkXY() == 0 || dComIfGp_evmng_ChkPresentEnd()) {
                        field_0x92E = field_0x92C;
                        field_0x92C = 0x10;
                        retVal = 0;
                    }
                }
                return retVal;
            }
        }
    }
    return retVal;
}

int daMyna_c::orderEvent() {
    if (daPy_py_c::checkNowWolf()) {
        switch (field_0x92C) {
        case 0:
        case 5:
        case 7:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;
        }
        if (attention_info.flags == (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e)) {
            attention_info.distances[fopAc_attn_TALK_e] = 0x8B;
            attention_info.distances[fopAc_attn_SPEAK_e] = 0x8B;
            eventInfo.onCondition(1);
        }
    }
    return 1;
}

void daMyna_c::deleteItem(int i_itemIndex) {
    mShopItems[i_itemIndex].mTargetActorID = -1;
    mShopItems[i_itemIndex].mItemStatus = 4;
}

// For some reason, this function needs to be force active or the linker strips it out.
#pragma push
#pragma force_active on

void daMyna_c::deleteItem(fpc_ProcID i_itemId) {
    for (int i = 0; i < mNumShopItems; i++) {
        if (i_itemId == mShopItems[i].mTargetActorID) {
            mShopItems[i_itemId].mTargetActorID = -1;
            mShopItems[i_itemId].mItemStatus = 4;
        }
    }
}

#pragma pop

void daMyna_c::initiate() {
    for (int i = 0; i < 10; i++) {
        mShopItems[i].mTargetActorID = -1;
        mShopItems[i].mSubActorID = -1;
        mShopItems[i].mFlowNodeNum = 0;
        mShopItems[i].mValueNum = 0;
        mShopItems[i].mItemStatus = 0;
        mShopItems[i].mItemType = 0xF;
    }
    if (dComIfGs_isSaveSwitch(0x3C)) {
        field_0x91C = 0x139;
        field_0x92C = 5;
    } else {
        field_0x91C = 200;
        field_0x92C = 0;
    }
    field_0x92D = field_0x92C;
    field_0x92E = field_0x92C;
    field_0x920 = 0;
    field_0x922 = 0;

         /* dSv_event_flag_c::F_0802 - Faron Woods - Trill attacks when stealing */
    if (!dComIfGs_isEventBit(u16(dSv_event_flag_c::saveBitLabels[802]))) {
        field_0x932 = 0;
        field_0x938 = 0;
    } else {
        field_0x932 = 2;
        field_0x938 = 1;
    }

    field_0x939 = 0;
    field_0x937 = 0;
    field_0x935 = 1;
    field_0x934 = 1;
    field_0x90C = 0.0f;
    field_0x910 = 0.0f;
    field_0x924 = cM_rndF(200.0f) + 200.0f;
    field_0x933 = 0;
    field_0x908 = -1;
    field_0x91E = 0;
    field_0x92F = 0;
    mNumShopItems = getItemNumMax();
    field_0x931 = -1;
    field_0x828 = NULL;
    mDamageTimer = 0;
    field_0x92A = 0;
    field_0x82C.set(0.0f, 0.0f, 0.0f);
    field_0x838.set(0.0f, 0.0f, 0.0f);
    field_0x85C.set(0.0f, 0.0f, 0.0f);
    field_0x844.set(0.0f, 0.0f, 0.0f);
    field_0x850.set(0.0f, 0.0f, 0.0f);
    (this->*init_proc[field_0x92C])();
    field_0x928 = current.angle.y;
    current.angle.x = 0;
    current.angle.z = 0;
    mStatus.Init(100, 0, this);
    mSph.Set(mCcDSph);
    mSph.SetStts(&mStatus);
    mSph.OffAtVsPlayerBit();
    J3DAnmTransformKey* key_anm = getTrnsfrmKeyAnm(l_bckFileNameTBL[1]);
    setMcaMorfAnm(key_anm, 1.0f, 0.0f, 2, 0, -1);
}

void daMyna_c::setItemInfo() {
    if (daMyna_actor_count != -1 && mNumShopItems > daMyna_actor_count) {
        daMyna_actor_count = 0;
        fpcM_Search(daMyna_searchSSItem, this);
    }
    if (daMyna_evtTagActor0 == NULL) {
        fpcM_Search(daMyna_searchEvtTag, this);
    }
    if (daMyna_actor_count != -1 && mNumShopItems <= daMyna_actor_count) {
        for (int i = 0; i < mNumShopItems; i++) {
            daObj_SSBase_c* subActor = static_cast<daObj_SSBase_c*>(daMyna_subActor[i]);
            subActor->setParentPtr(this);
            mShopItems[i].mTargetActorID = fopAcM_GetID(daMyna_targetActor[i]);
            mShopItems[i].mSubActorID = fopAcM_GetID(daMyna_subActor[i]);
            mShopItems[i].mFlowNodeNum = subActor->getFlowNodeNumber();
            mShopItems[i].mValueNum = subActor->getValueNumber();
            mShopItems[i].mItemType = getItemType(subActor);
            mShopItems[i].mItemStatus = 2;
        }
        daMyna_actor_count = -1;
    }
}

void daMyna_c::setRoomNo() {
    s8 roomId = dComIfG_Bgsp().GetRoomId(mGndChk);
    fopAcM_SetRoomNo(this, roomId);
    mStatus.SetRoomId(roomId);
}

void daMyna_c::setCollision() {
    mSph.SetC(field_0x85C);
    mSph.SetR(15.0f);
    dComIfG_Ccsp()->Set(&mSph);
}

void daMyna_c::set_mtx() {
    J3DModel* model = mpMorf->getModel();
    cXyz local_18(current.pos);
    scale.set(l_HOSTIO.scale, l_HOSTIO.scale, l_HOSTIO.scale);
    mDoMtx_stack_c::transS(local_18);
    mDoMtx_stack_c::ZXYrotM(current.angle);
    mDoMtx_stack_c::scaleM(scale);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&field_0x82C);
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&field_0x838);
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(0x10));
    mDoMtx_stack_c::multVecZero(&field_0x85C);
    field_0x82C -= current.pos;
    field_0x838 -= current.pos;
}

void daMyna_c::animePlay() {
    int reg_r29 = 15;
    f32 speed = mpMorf->getPlaySpeed();
    mpMorf->play(0, 0);
    if (checkEndAnm(speed) != 0) {
        field_0x91E += u16(1);
    }

    if (cLib_checkBit<u16>(u16(field_0x914), 0x40) != 0 && cLib_checkBit<u16>(u16(field_0x914), 4) == 0) {
        f32 reg_f30 = mBtpAnm.getFrame();
        mBtpAnm.play();
        if (mBtpAnm.getFrameCtrl()->getAttribute() == 2) {
            if (checkEndAnm(mBtpAnm.getFrameCtrl()) != 0) {
                field_0x918 = cLib_getRndValue(0x3C, 100);
                mBtpAnm.setPlaySpeed(0.0f);
            }
        } else {
            field_0x918 = 0;
        }

        if (cLib_calcTimer(&field_0x918) == 0) {
            mBtpAnm.setPlaySpeed(1.0f);
        }
    }

    cLib_offBit<u16>(field_0x914, reg_r29);
}

int daMyna_c::setMcaMorfAnm(J3DAnmTransformKey* i_anm, f32 i_rate, f32 i_morf, int i_attr,
                            int i_start, int i_end) {
    f32 f_start = i_start;
    f32 f_end = i_end;
    mpMorf->setAnm(i_anm, i_attr, i_morf, i_rate, f_start, f_end);
    field_0x91E = 0;
    return 1;
}


void daMyna_c::setBtpAnm(J3DAnmTexPattern* i_btk, J3DModelData* param_1, f32 i_rate,
                         int i_attribute) {
    mBtpAnm.init(param_1, i_btk, 1, i_attribute, i_rate, 0, -1);
}

J3DAnmTransformKey* daMyna_c::getTrnsfrmKeyAnm(char* i_resName) {
    if (i_resName != NULL) {
        return static_cast<J3DAnmTransformKey*>(dComIfG_getObjectRes("Npc_myna", i_resName));
    } else {
        return NULL;
    }
}

J3DAnmTexPattern* daMyna_c::getTexPtrnAnm(char* i_resName) {
    if (i_resName != NULL) {
        return static_cast<J3DAnmTexPattern*>(dComIfG_getObjectRes("Npc_myna", i_resName));
    } else {
        return NULL;
    }
}

int daMyna_c::checkEndAnm(f32 param_0) {
    bool retVal;
    switch ((u8)mpMorf->getPlayMode()) {
    case J3DFrameCtrl::EMode_LOOP:
        return mpMorf->isLoop();
    case J3DFrameCtrl::EMode_NONE:
    case J3DFrameCtrl::EMode_RESET:
        return mpMorf->isStop() && mpMorf->getPlaySpeed() != param_0;
    case J3DFrameCtrl::EMode_REVERSE:
    case J3DFrameCtrl::EMode_LOOP_REVERSE:
        return mpMorf->isStop() && mpMorf->getPlaySpeed() != param_0;
    default:
        return false;
    }
}

int daMyna_c::checkEndAnm(J3DFrameCtrl* param_0) {
    switch (param_0->getAttribute()) {
    case 2:
        return param_0->checkState(2);
    case 0:
    case 1: {
        return (param_0->checkState(1) || param_0->getRate() == 0.0f);
    }
    case 3:
    case 4:
    default:
        return 0;
    }
}

u8 daMyna_c::getItemNumMax() {
    u8 param_val = (fopAcM_GetParam(this) & 0xF000000) >> 24;
    return param_val;
}

u8 daMyna_c::getItemType(void* param_0) {
    u8 itemType = 0xF;
    u8 uVar1 = fopAcM_GetParam(param_0) & 0xFF;

    if (fopAcM_GetName(param_0) == PROC_OBJ_SSITEM) {
        switch (uVar1) {
        case 0:
            itemType = 1;
            break;
        case 1:
            itemType = 2;
            break;
        case 2:
            itemType = 0;
            break;
        }
    } else if (fopAcM_GetName(param_0) == PROC_OBJ_SSDRINK) {
        switch (uVar1) {
        case 0:
            itemType = 3;
            break;
        case 1:
            itemType = 4;
            break;
        case 2:
            itemType = 5;
            break;
        case 3:
            itemType = 6;
            break;
        case 4:
            itemType = 7;
            break;
        case 5:
            itemType = 8;
            break;
        }
    } else if (fopAcM_GetName(param_0) == PROC_TAG_SSDRINK) {
        switch (uVar1) {
        case 0:
            itemType = 9;
            break;
        case 1:
            itemType = 10;
            break;
        case 2:
            itemType = 11;
            break;
        case 3:
            itemType = 12;
            break;
        case 4:
            itemType = 13;
            break;
        case 5:
            itemType = 14;
            break;
        }
    }

    return itemType;
}

u16 daMyna_c::getFlowNodeNum() {
    u16 num = home.angle.x;
    return (num == 0xFFFF || num == 0) ? -1 : num;
}

void daMyna_c::checkDead() {
    for (int i = 0; i < 10; i++) {
        if (mShopItems[i].mItemStatus == 2 && !fopAcM_IsExecuting(mShopItems[i].mTargetActorID)) {
            deleteItem(i);
            onEventFlag(0);
            dMsgObject_addTotalPrice(mShopItems[i].mValueNum);
            fopAcM_delete(mShopItems[i].mSubActorID);
            cXyz var1(1.0f, 1.0f, 1.0f);
            if (daMyna_evtTagActor0 == NULL ||
                (daMyna_evtTagActor0 != NULL && !chkPlayerInEvtArea(daMyna_evtTagActor0, var1)))
            {
                /* dSv_event_flag_c::F_0203 - Shop - First tried to steal from unnmaned shop (Havent checked donation box) */
                if (!dComIfGs_isEventBit(u16(dSv_event_flag_c::saveBitLabels[203]))) {
                    dComIfGs_onEventBit(u16(dSv_event_flag_c::saveBitLabels[203]));
                }
            }

            // Actor Dead!!!
            OS_REPORT("アクター死亡!!!\n");
        }
    }
}

int daMyna_c::chkPlayerInEvtArea(fopAc_ac_c* param_0, cXyz param_1) {
    cXyz local_60;
    cXyz local_6c;

    int retVal = 0;
    f32 fVar3, fVar4, fVar1, fVar2;
    if (param_0 != NULL) {
        mDoMtx_stack_c::YrotS(-param_0->current.angle.y);
        mDoMtx_stack_c::transM(-param_0->current.pos.x, -param_0->current.pos.y,
                               -param_0->current.pos.z);
        mDoMtx_stack_c::multVec(&daPy_getPlayerActorClass()->current.pos, &local_6c);
        fVar1 = fabsf(param_0->scale.x + param_1.x);
        fVar2 = fabsf(param_0->scale.z + param_1.z);
        fVar3 = fabsf(local_6c.x);
        fVar4 = fabsf(local_6c.z);
        local_60 = param_0->current.pos - daPy_getPlayerActorClass()->current.pos;
        if ((fVar3 * fVar3) / (fVar1 * fVar1) + (fVar4 * fVar4) / (fVar2 * fVar2) <= 1.0f &&
            -(param_0->scale.y + param_1.y) < local_60.y &&
            local_60.y < (param_0->scale.y + param_1.y))
        {
            retVal = 1;
        }
    }

    return retVal;
}

void daMyna_c::animeControl() {
    f32 rate = 1.0f;
    int attr = J3DFrameCtrl::EMode_LOOP;
    f32 morf = field_0x90C;
    switch (field_0x935) {
    case 1:
        if (field_0x934 == 4) {
            morf = 6.0f;
        }
        break;
    case 2:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case 3:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case 4:
        rate = l_HOSTIO.jump_anime;
        break;
    case 7:
        attr = J3DFrameCtrl::EMode_NONE;
        morf = 6.0f;
        break;
    case 8:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case 13:
        attr = J3DFrameCtrl::EMode_NONE;
        break;
    case 14:
        break;
    }

    J3DAnmTransformKey* anm = getTrnsfrmKeyAnm(l_bckFileNameTBL[field_0x935]);
    setMcaMorfAnm(anm, rate, morf, attr, 0, -1);
    if (cLib_checkBit<u16>((u16)field_0x914, 0x40) == 0) {
        int attribute = 2;
        J3DAnmTexPattern* btp = getTexPtrnAnm(l_btpFileNameTBL[field_0x936]);
        if (btp != NULL) {
            setBtpAnm(btp, mpMorf->getModel()->getModelData(), 1.0f, attribute);
            cLib_onBit<u16>(field_0x914, 0x44);
        }
    }
}

void daMyna_c::setAnimeType(u8 param_0, f32 param_1) {
    if (param_0 == 13 || param_0 == 1 || (param_0 == 14 && field_0x935 != 13)) {
        param_1 = 6.0f;
    }
    field_0x934 = field_0x935;
    field_0x935 = param_0;
    field_0x90C = param_1;
}


void daMyna_c::playDefaultWaitAnime() {
    switch (field_0x935) {
    case 2:
    case 3:
        if (field_0x91E != 0) {
            setAnimeType(1, 0.0f);
        }
        break;
    case 6: {
        cXyz local_58;
        cXyz local_64;
        cXyz stack_70;

        local_58.x = daMyna_LightActor->current.pos.x;
        local_58.y = home.pos.y;
        local_58.z = daMyna_LightActor->current.pos.z;

        cXyz stack_7c = local_58 - home.pos;

        s16 sVar42 = cM_atan2s(stack_7c.x, stack_7c.z);
        f32 fVar7 = field_0x910 * stack_7c.abs();
        f32 fVar3 = ((field_0x82C.y - field_0x844.y) + (field_0x838.y - field_0x850.y)) * 0.5f;

        local_64.x = 0.0f;
        local_64.y = 0.0f;
        local_64.z = l_HOSTIO.side_walking_speed * (fVar3 * fVar3);

        cLib_offsetPos(&stack_70, &current.pos, sVar42, &local_64);
        current.pos = stack_70;

        cXyz stack_88 = current.pos - home.pos;
        if (stack_88.abs() > fVar7) {
            setAnimeType(1, 6.0f);
        }
        return;
    }
    case 5:
        cXyz local_94;
        cXyz stack_a0;
        cXyz local_ac = home.pos - current.pos;

        s16 sVar4 = cM_atan2s(local_ac.x, local_ac.z);
        f32 fVar2 = ((field_0x82C.y - field_0x844.y) + (field_0x838.y - field_0x850.y)) * 0.5f;

        local_94.x = 0.0f;
        local_94.y = 0.0f;
        local_94.z = l_HOSTIO.side_walking_speed * (fVar2 * fVar2);

        cLib_offsetPos(&stack_a0, &current.pos, sVar4, &local_94);
        current.pos = stack_a0;

        cXyz home_to_curpos = home.pos - current.pos;
        if (home_to_curpos.abs() < l_HOSTIO.return_approach_dist) {
            current.pos.set(home.pos);
            setAnimeType(1, 6.0f);
        }
    }
}

#if DEBUG
char* dummy_lit_121495() {
    return "rebirth ===> %d\n";
}
#endif

void daMyna_c::setDefaultWaitAnime(u8 param_0) {
    if (daMyna_LightActor != NULL && field_0x935 == 1 &&
        (field_0x92C == 0 || field_0x92C == 5 || field_0x92C == 7) &&
        daMyna_LightActor->checkTurnOnOffChange())
    {
        field_0x92D = field_0x92C;
        field_0x92C = 11;
    } else {
        if (field_0x935 == param_0) {
            switch (field_0x935) {
            case 1:
                field_0x924--;
                if (field_0x924 <= 0) {
                    switch (int(cM_rndF(100.0f)) % 3) {
                    case 0:
                        setAnimeType(2, 0.0f);
                        break;
                    case 1:
                        setAnimeType(3, 0.0f);
                        break;
                    default:
                        cXyz home_to_curpos = home.pos - current.pos;
                        if (home_to_curpos.abs() < l_HOSTIO.return_approach_dist) {
                            field_0x910 = cM_rnd() * 0.25f + 0.25f;
                            setAnimeType(6, 0.0f);
                        } else {
                            setAnimeType(5, 0.0f);
                        }
                    }
                }
            }
        }
    }
}

static int daMyna_Create(void* i_this) {
    return static_cast<daMyna_c*>(i_this)->create();
}

static int daMyna_Delete(void* i_this) {
    return static_cast<daMyna_c*>(i_this)->destroy();
}

static int daMyna_Execute(void* i_this) {
    return static_cast<daMyna_c*>(i_this)->execute();
}

static int daMyna_Draw(void* i_this) {
    return static_cast<daMyna_c*>(i_this)->draw();
}

static int daMyna_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daMyna_MethodTable = {
    (process_method_func)daMyna_Create,  (process_method_func)daMyna_Delete,
    (process_method_func)daMyna_Execute, (process_method_func)daMyna_IsDelete,
    (process_method_func)daMyna_Draw,
};

actor_process_profile_definition g_profile_MYNA = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_MYNA,
    &g_fpcLf_Method.base,
    sizeof(daMyna_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x0130,
    &daMyna_MethodTable,
    0x40000,
    0,
    fopAc_CULLBOX_0_e
};

AUDIO_INSTANCES;
