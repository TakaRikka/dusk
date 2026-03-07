/*
* Translation Unit: d_a_tag_statue_evt
* Owl Statue Event Tag -- Awarding Sky Characters
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_player.h"
#include "d/actor/d_a_tag_statue_evt.h"
#include "d/d_com_inf_game.h"
#include "d/d_debug_viewer.h"

class daTagStatue_HIO_c : public mDoHIO_entry_c {
public:
    daTagStatue_HIO_c();

    void genMessage(JORMContext*);

    f32 check_range;
    f32 event_trigger_range;
    u8 show_range;
};

#if DEBUG
daTagStatue_HIO_c l_HIO;
#endif

static int CheckCreateHeap(fopAc_ac_c* i_this) {
    daTagStatue_c* tag_statue = static_cast<daTagStatue_c*>(i_this);
    return tag_statue->CreateHeap();
}

// Returns ID of Owl Statue corresponding to tag
static void* s_s_sub(void* i_proc, void* param_1) {
    UNUSED(param_1);
    if (fopAcM_IsActor(i_proc) && fopAcM_GetName(i_proc) == PROC_CSTAF) {
        return i_proc;
    }

    return NULL;
}

void daTagStatue_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

static const u16 l_event_bit[6] = {
    dSv_event_flag_c::F_0791, /* Sky character - Sky character 1 */
    dSv_event_flag_c::F_0792, /* Sky character - Sky character 2 */
    dSv_event_flag_c::F_0793, /* Sky character - Sky character 3 */
    dSv_event_flag_c::F_0794, /* Sky character - Sky character 4 */
    dSv_event_flag_c::F_0795, /* Sky character - Sky character 5 */
    dSv_event_flag_c::F_0812, /* N/A - N/A */
};

static char* l_arcName = "Obj_cs_f";

static u16 l_eff_id[3] = {
    0x8C7E, 0x8C7F, 0x8C84
};

static char* l_eventName[6] = {
    "SKY_LETTERS_00",   // Faron Woods
    "SKY_LETTERS_01",   // Gerudo Desert
    "SKY_LETTERS_02",   // Kakariko Gorge
    "SKY_LETTERS_03",   // Great Eldin Bridge
    "SKY_LETTERS_04",   // Lake Hylia
    "SKY_LETTERS_05"    // Ruined Theatre
};

static char* l_evArcName[6] = {
    NULL,               // Faron Woods
    "cs_ev_01",         // Gerudo Desert
    "cs_ev_02",         // Kakariko Gorge
    "cs_ev_03",         // Great Eldin Bridge
    "cs_ev_04",         // Lake Hylia
    "cs_ev_05"          // Ruined Theatre
};

static cull_box l_cull_box = {
    {-300.0f, -100.0f, -300.0f}, // Min
    {300.0f, 400.0f, 300.0f}     // Max
};

#if DEBUG
daTagStatue_HIO_c::daTagStatue_HIO_c() {
    check_range = 120.0f;
    event_trigger_range = 100.0f;
    show_range = 0;
}

void daTagStatue_HIO_c::genMessage(JORMContext* ctx) {
    // "Sky Character tag"
    ctx->genLabel("天空文字タグ", 0, 0);
    ctx->genCheckBox("範囲表示", &show_range, 1);
    // "Stone statue check range"
    ctx->genSlider("石像チェック範囲", &check_range, 0.0f, 1000.0f);
    // "Event trigger range"
    ctx->genSlider("イベント起動範囲", &event_trigger_range, 0.0f, 1000.0f);
}
#endif

void daTagStatue_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, 1.0f + current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

cPhs_Step daTagStatue_c::Create() {
    if (fopAcM_isSwitch(this, getSwbit2())) {
        // Player already obtained Sky Character from this tag
        setAction(ACTION_DEAD_e);
        mpGlowingSkyCharacterBrk->setFrame(0.0f);
        mpGlowingSkyCharacterBrk->setPlaySpeed(0.0f);
    } else {
        if (fopAcM_isSwitch(this, getSwbit())) {
            setParticle();
            mpGlowingSkyCharacterBrk->setFrame(mpGlowingSkyCharacterBrk->getEndFrame());
        } else {
            mpGlowingSkyCharacterBrk->setFrame(0.0f);
            mpGlowingSkyCharacterBrk->setPlaySpeed(0.0f);
        }

        setAction(ACTION_WAIT_e);
    }

    mpPlatformCircleBtk->setFrame(mSkyCharacterEventBitIdIndex);

    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z,
                          l_cull_box.max.x, l_cull_box.max.y, l_cull_box.max.z);

    if (!l_evArcName[mSkyCharacterEventBitIdIndex]) {
        eventInfo.setArchiveName(l_arcName);
    } else {
        eventInfo.setArchiveName(l_evArcName[mSkyCharacterEventBitIdIndex]);
    }

    mMapToolId = 255;

    mEventId = (int)dComIfGp_getEventManager().
        getEventIdx(this, l_eventName[mSkyCharacterEventBitIdIndex], 0xFF);

    mItemId = -1;
    mOwlStatueId = -1;

    return cPhs_LOADING_e;
}

int daTagStatue_c::CreateHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcName, 6));
    JUT_ASSERT(277, modelData != NULL)
    mpModel = mDoExt_J3DModel__create(modelData, 0x00080000, 0x11000284);

    if (!mpModel) {
        return 0;
    }

    J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, 12);
    JUT_ASSERT(291, pbtk != NULL);
    mpPlatformCircleBtk = new mDoExt_btkAnm();


    if (!mpPlatformCircleBtk || !mpPlatformCircleBtk->init(modelData, pbtk, 1, 0, 1.0f, 0, -1)) {
        return 0;
    }

    J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 9);
    JUT_ASSERT(303, pbrk != NULL);
    mpGlowingSkyCharacterBrk = new mDoExt_brkAnm();

    if (!mpGlowingSkyCharacterBrk ||
       !mpGlowingSkyCharacterBrk->init(modelData, pbrk, 1, 0, 1.0f, 0, -1)) {
        return 0;
    }

    return 1;
}

cPhs_Step daTagStatue_c::create() {
    fopAcM_ct(this, daTagStatue_c);

    mSkyCharacterEventBitIdIndex = getType();

    int phase;
    if (l_evArcName[mSkyCharacterEventBitIdIndex]) {
        phase = dComIfG_resLoad(&mEvArcPhase, l_evArcName[mSkyCharacterEventBitIdIndex]); 

        if (phase != cPhs_COMPLEATE_e) {
            return static_cast<cPhs_Step>(phase);
        }
    }

    phase = dComIfG_resLoad(&mArcPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, 0x1100)) {
            return cPhs_ERROR_e;
        }
        
        if (Create() == cPhs_INIT_e) {
            return cPhs_ERROR_e;
        }

#if DEBUG
        // "Sky Character tag"
        l_HIO.entryHIO("天空文字タグ");
#endif
    }

    return static_cast<cPhs_Step>(phase);
}

int daTagStatue_c::execute() {
    if (static_cast<u32>(mOwlStatueId + (1 << 16)) == 0xFFFF) {
        base_process_class* found_owl_statue = fpcM_Search(s_s_sub, this);
        if (found_owl_statue) {
            mOwlStatueId = fopAcM_GetID(found_owl_statue);
            // "Sky Character tag: a stone statue was discovered!"
            OS_REPORT("天空文字タグ：石像発見しました！ID=%d\n", mOwlStatueId);
        }
    }

    event_proc_call();
    setBaseMtx();

    if (mpGlowingSkyCharacterBrk) {
        mpGlowingSkyCharacterBrk->play();
    }

    return 1;
}

void daTagStatue_c::event_proc_call() {
    typedef void (daTagStatue_c::*actionFunc)(void);
    static actionFunc l_func[4] = {&daTagStatue_c::actionWait, &daTagStatue_c::actionOrderEvent,
                                   &daTagStatue_c::actionEvent, &daTagStatue_c::actionDead};

    (this->*l_func[mCurrentAction])();

    // Form ring of light around platform and glowing Sky Character if Owl Statue has been moved far
    // enough away
    if (checkOnEffect()) {
        setParticle();
        mpGlowingSkyCharacterBrk->setPlaySpeed(1.0f);
    } else {
        endParticle();
        mpGlowingSkyCharacterBrk->setPlaySpeed(-1.0f);
    }
}

void daTagStatue_c::actionWait() {
    if (checkStartDemo()) {
        // Statue is far enough away and player is close enough to the tag for the cutscene to be
        // ordered
        setAction(ACTION_ORDER_EVENT_e);
        fopAcM_orderOtherEventId(this, mEventId, mMapToolId, 0xFFFF, 0, 1);
        eventInfo.onCondition(0x0002);
    }
}

void daTagStatue_c::actionOrderEvent() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        // Cutscene order accepted
        setAction(ACTION_EVENT_e);
        Z2GetAudioMgr()->subBgmStart(Z2BGM_KOMONJO_GET_INTRO);
        demoProc();
    } else {
        // Cutscene order has not been accepted yet, attempt again
        fopAcM_orderOtherEventId(this, mEventId, mMapToolId, 0xFFFF, 0, 1);
        eventInfo.onCondition(0x0002);
    }
}

void daTagStatue_c::actionEvent() {
    if (dComIfGp_evmng_endCheck(mEventId)) {
        setAction(ACTION_DEAD_e);
        dComIfGp_event_reset();
        fopAcM_onSwitch(this, getSwbit2());
        dComIfGs_onEventBit((u16)l_event_bit[mSkyCharacterEventBitIdIndex]);
    } else {
        demoProc();
    }
}

void daTagStatue_c::actionDead() {
    /* empty function */
}

int daTagStatue_c::demoProc() {
    static char* action_table[3] = {
        "WAIT",
        "ST_MOVE",
        "ITEM"
    };

    // Saved positions of Owl Statues for cutscenes
    static Vec l_statue_pos[6] = {
        {-17312.3007812f, -349.100006104f, -20523.1992188f},    // Faron Woods
        {-17312.3007812f, -349.100006104f, -20523.1992188f},    // Gerudo Desert
        {-9675.59960938f, -4800.0f, 39693.8984375f},            // Kakariko Gorge
        {33345.6015625f, -75.5999984741f, -36331.1015625f},     // Great Eldin Bridge
        {-94009.5f, -5440.10009766f, 18534.9003906f},           // Lake Hylia
        {-81451.296875f, -1654.09997559f, 20832.1992188}        // Ruined Theatre
    };

    // Saved y orientations of Owl Statues for cutscenes
    static s16 l_statue_ang[6] = {
        16384,  // Faron Woods
        16384,  // Gerudo Desert
        8485,   // Kakariko Gorge
        16384,  // Great Eldin Bridge
        16384,  // Lake Hylia
        16384   // Ruined Theatre
    };

    daPy_py_c* player = daPy_getPlayerActorClass();

    int act_id =
        dComIfGp_evmng_getMyActIdx(mStaffId, action_table, ARRAY_SIZEU(action_table), 0, 0);

    int item;
    int* timer_p;
    fopAc_ac_c* found_owl_statue;
    if (dComIfGp_evmng_getIsAddvance(mStaffId)) {
        switch(act_id) {
            case DEMO_ACTION_WAIT: {
                // Delay fading to black
                timer_p = dComIfGp_evmng_getMyIntegerP(mStaffId, "Timer");

                if (!timer_p) {
                    mTimer = 1;
                } else {
                    mTimer = *timer_p;
                }

                break;
            }
            case DEMO_ACTION_MOVE_OWL_STATUE:
                // Position and orient Owl Statue behind player
                if (mSkyCharacterEventBitIdIndex != TAG_LOCATION_GERUDO_DESERT &&
                    mSkyCharacterEventBitIdIndex != TAG_LOCATION_LAKE_HYLIA &&
                    mSkyCharacterEventBitIdIndex != TAG_LOCATION_RUINED_THEATRE) {
                    found_owl_statue = reinterpret_cast<fopAc_ac_c*>(fpcM_Search(s_s_sub, this));
                    if (found_owl_statue) {
                        found_owl_statue->current.pos = l_statue_pos[mSkyCharacterEventBitIdIndex];
                        found_owl_statue->shape_angle.y = l_statue_ang[mSkyCharacterEventBitIdIndex];
                        fopAcM_OnStatus(found_owl_statue, 0x800);
                    }
                }
                break;
            case DEMO_ACTION_AWARD_ITEM:
                // If the player already has 5 Sky Characters, reward them with the completed
                // Ancient Sky Book, otherwise, reward another character
                if (getLetterCount() == 5) {
                    item = fpcNm_ITEM_ANCIENT_DOCUMENT2;
                    /* Sky character - Sky character 6 */
                    dComIfGs_onEventBit(dSv_event_flag_c::F_0796);
                } else {
                    item = fpcNm_ITEM_AIR_LETTER;
                }

                mItemId =
                    fopAcM_createItemForTrBoxDemo(&current.pos, item, -1,
                                                  fopAcM_GetRoomNo(this), 0, 0);

                JUT_ASSERT(580, mItemId != fpcM_ERROR_PROCESS_ID_e);

                if (mItemId + (1 << 16) != 0xFFFF) {
                    dComIfGp_event_setItemPartnerId(mItemId);
                }
        }
    }

    switch(act_id) {
        case DEMO_ACTION_WAIT:
            if (!cLib_calcTimer<u8>(&mTimer)) {
                dComIfGp_evmng_cutEnd(mStaffId);
            }
            break;
        case DEMO_ACTION_MOVE_OWL_STATUE:
            dComIfGp_evmng_cutEnd(mStaffId);
            break;
        default:
            dComIfGp_evmng_cutEnd(mStaffId);
            break; 
    }

    return 0;
}

void daTagStatue_c::setParticle() {
    for (int i = 0; i < 3; i++) {
        if (!mpLightRingEmitters[i]) {
            mpLightRingEmitters[i] =
                dComIfGp_particle_set(l_eff_id[i], &current.pos, &home.angle, &scale, 0xff, NULL,
                                      -1, NULL, NULL, NULL);
        }
    }
}

void daTagStatue_c::endParticle() {
    for (int i = 0; i < 3; i++) {
        if (mpLightRingEmitters[i]) {
            mpLightRingEmitters[i]->becomeInvalidEmitter();
            mpLightRingEmitters[i]->quitImmortalEmitter();
            mpLightRingEmitters[i] = NULL;
        }
    }
}

// Check if statue is far enough away from the Sky Character platform and if player is close enough
// to the tag for the cutscene of obtaining a Sky Character to be ordered
BOOL daTagStatue_c::checkStartDemo() {
    const fopAc_ac_c* const player = daPy_getPlayerActorClass();

    if (!checkOnEffect()) {
        // Owl Statue is not far enough away from tag
        return 0;
    }

#if DEBUG
#define TAG_STATUE_EVENT_TRIGGER_RANGE l_HIO.event_trigger_range
#else
#define TAG_STATUE_EVENT_TRIGGER_RANGE 100.0f
#endif

    if (player->current.pos.abs(current.pos) < TAG_STATUE_EVENT_TRIGGER_RANGE) {
        return 1;
    } else {
        return 0;
    }
}

// Return whether the owl statue has moved far enough away (in the horizontal plane) from the
// platform where the Sky Character is obtained in order for its glowing effects to occur and the
// cutscene to be ordered
BOOL daTagStatue_c::checkOnEffect() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (!fopAcM_isSwitch(this, getSwbit2()) &&
        static_cast<u32>(mOwlStatueId + (1 << 16)) != 0xFFFF) {
        fopAc_ac_c* found_owl_statue = fopAcM_SearchByID(mOwlStatueId);
        if (found_owl_statue) {
#if DEBUG
#define TAG_STATUE_CHECK_RANGE l_HIO.check_range
#else
#define TAG_STATUE_CHECK_RANGE 120.0f
#endif

            if (found_owl_statue->current.pos.absXZ(current.pos) > TAG_STATUE_CHECK_RANGE) {
                return TRUE;
            }
        } else {
            // No Owl Statue found, platform cannot be covered
            return TRUE;
        }
    }

    return FALSE;
}

int daTagStatue_c::getLetterCount() {
    int numLetters = 0;
    for (int i = 0; i < 6; i++) {
        if (dComIfGs_isEventBit((u16)l_event_bit[i])) {
            numLetters++;
        }
    }

    return numLetters;
}

int daTagStatue_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();

    mpGlowingSkyCharacterBrk->entry(mpModel->getModelData());
    mpPlatformCircleBtk->entry(mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);
    mDoExt_brkAnmRemove(mpModel->getModelData());
    mDoExt_btkAnmRemove(mpModel->getModelData());

    dComIfGd_setList();

#if DEBUG
    if (l_HIO.show_range) {
        GXColor color1 = (GXColor){0xff, 0, 0, 0xff};
        GXColor color2 = (GXColor){0, 0, 0xff, 0xff};
        dDbVw_drawCircleXlu(current.pos, l_HIO.check_range, color1, 1, 12);
        dDbVw_drawCircleXlu(current.pos, l_HIO.event_trigger_range, color2, 1, 12);
    }
#endif

    return 1;
}

int daTagStatue_c::_delete() {
    endParticle();
    dComIfG_resDelete(&mArcPhase, l_arcName);
    if (l_evArcName[mSkyCharacterEventBitIdIndex]) {
        dComIfG_resDelete(&mEvArcPhase, l_evArcName[mSkyCharacterEventBitIdIndex]);
    }
#if DEBUG
    l_HIO.removeHIO();
#endif

    return 1;
}

static int daTagStatue_Draw(daTagStatue_c* i_this) {
    return i_this->draw();
}

static int daTagStatue_Execute(daTagStatue_c* i_this) {
    return i_this->execute();
}

static int daTagStatue_Delete(daTagStatue_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "Sample");
    return i_this->_delete();
}

static int daTagStatue_Create(daTagStatue_c* i_this) {
    fopAcM_RegisterCreateID(i_this, "Sample");
    return i_this->create();
}

static actor_method_class l_daTagStatue_Method = {
reinterpret_cast<process_method_func>(daTagStatue_Create),
reinterpret_cast<process_method_func>(daTagStatue_Delete),
reinterpret_cast<process_method_func>(daTagStatue_Execute),
NULL,
reinterpret_cast<process_method_func>(daTagStatue_Draw),
};

actor_process_profile_definition g_profile_Tag_Statue = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_Statue,
    &g_fpcLf_Method.base,
    sizeof(daTagStatue_c),
    0,
    0,
    &g_fopAc_Method.base,
    588,
    &l_daTagStatue_Method,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e
};
