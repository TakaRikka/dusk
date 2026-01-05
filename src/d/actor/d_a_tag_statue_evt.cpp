/*
* Translation Unit: d_a_tag_statue_evt
* Owl Statue Event Tag -- Awarding Sky Characters
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_statue_evt.h"
#include "d/d_com_inf_game.h"

static int CheckCreateHeap(fopAc_ac_c* i_this) {
    daTagStatue_c* tag_statue = static_cast<daTagStatue_c*>(i_this);
    return tag_statue->CreateHeap();
}

// Returns ID of Owl Statue corresponding to tag
static void* s_s_sub(void* i_proc, void* unused) {
    if (fopAc_IsActor(i_proc) && fopAcM_GetName(i_proc) == PROC_CSTAF) {
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

void daTagStatue_c::setBaseMtx() {
    const cXyz& current_pos = fopAcM_GetPosition(this);
    mDoMtx_stack_c::transS(current_pos.x, 1.0f + current_pos.y, current_pos.z);
    const csXyz& shape_angle = *fopAcM_GetShapeAngle_p(this);
    mDoMtx_stack_c::ZXYrotM(shape_angle);

    mpModel->setBaseTRMtx(mDoMtx_stack_c::now);
}

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

cPhs__Step daTagStatue_c::Create() {
    if(fopAcM_isSwitch(this, getSwbit2())) {
        // Player already obtained Sky Character from this tag
        setAction(ACTION_DEAD_e);
        mpGlowingSkyCharacterBrk->setFrame(0.0f);
        mpGlowingSkyCharacterBrk->setPlaySpeed(0.0f);
    }
    else {
        if(fopAcM_isSwitch(this, getSwbit())) {
            setParticle();
            mpGlowingSkyCharacterBrk->setFrame(mpGlowingSkyCharacterBrk->getEndFrame());
        }
        else {
            mpGlowingSkyCharacterBrk->setFrame(0.0f);
            mpGlowingSkyCharacterBrk->setPlaySpeed(0.0f);
        }

        setAction(ACTION_WAIT_e);
    }

    mpPlatformCircleBtk->setFrame(static_cast<f32>(mSkyCharacterEventBitIdIndex));

    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z, l_cull_box.max.x, l_cull_box.max.y, l_cull_box.max.z);

    char* ev_arc_name = l_evArcName[mSkyCharacterEventBitIdIndex]; 

    if(!ev_arc_name) {
        eventInfo.setArchiveName(l_arcName);
    }
    else {
        eventInfo.setArchiveName(ev_arc_name);
    }

    mMapToolId = 255;

    mEventId = dComIfGp_getPEvtManager()->getEventIdx(this, l_eventName[mSkyCharacterEventBitIdIndex], 0xFF);

    mTreasureDemoItemId = -1;
    mOwlStatueId = -1;

    return cPhs_LOADING_e;
}

int daTagStatue_c::CreateHeap() {
    J3DModelData* model_data = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcName, 6));
    mpModel = mDoExt_J3DModel__create(model_data, 0x00080000, 0x11000284);

    if(!mpModel) return 0;

    J3DAnmTextureSRTKey* btk =  reinterpret_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes(l_arcName, 12));
    mpPlatformCircleBtk = new mDoExt_btkAnm();


    if(!mpPlatformCircleBtk || !mpPlatformCircleBtk->init(model_data, btk, 1, 0, 1.0f, 0, -1))
        return 0;

    J3DAnmTevRegKey* brk = reinterpret_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes(l_arcName, 9));
    mpGlowingSkyCharacterBrk = new mDoExt_brkAnm();

    if(!mpGlowingSkyCharacterBrk || !mpGlowingSkyCharacterBrk->init(model_data, brk, 1, 0, 1.0f, 0, -1))
        return 0;

    return 1;
}

cPhs__Step daTagStatue_c::create() {
    fopAcM_ct(this, daTagStatue_c);

    mSkyCharacterEventBitIdIndex = fopAcM_GetParamBit(this, 8, 4);

    int phase;
    if(l_evArcName[mSkyCharacterEventBitIdIndex]) {
        phase = dComIfG_resLoad(&mEvArcPhase, l_evArcName[mSkyCharacterEventBitIdIndex]); 

        if(phase != cPhs_COMPLEATE_e)
            return static_cast<cPhs__Step>(phase);
    }

    phase = dComIfG_resLoad(&mArcPhase, l_arcName);
    if(phase == cPhs_COMPLEATE_e) {
        if(!fopAcM_entrySolidHeap(this, CheckCreateHeap, 0x1100))
            return cPhs_ERROR_e;
        
        if(Create() == cPhs_INIT_e)
            return cPhs_ERROR_e;
    }

    return static_cast<cPhs__Step>(phase);
}

int daTagStatue_c::execute() {
    if(static_cast<u32>(mOwlStatueId + (1 << 16)) == 0xFFFF) {
        base_process_class* found_owl_statue = fpcEx_Search(s_s_sub, this);
        if(found_owl_statue) {
            mOwlStatueId = fopAcM_GetID(found_owl_statue);
        }
    }

    event_proc_call();
    setBaseMtx();

    if(mpGlowingSkyCharacterBrk) {
        mpGlowingSkyCharacterBrk->play();
    }

    return 1;
}

void daTagStatue_c::event_proc_call() {
    typedef void (daTagStatue_c::*actionFunc)(void);
    static actionFunc l_func[4] = {&daTagStatue_c::actionWait, &daTagStatue_c::actionOrderEvent,
                                   &daTagStatue_c::actionEvent, &daTagStatue_c::actionDead};

    (this->*l_func[mCurrentAction])();

    // Form ring of light around platform and glowing Sky Character if Owl Statue has been moved far enough away
    if(checkOnEffect()) {
        setParticle();
        mpGlowingSkyCharacterBrk->setPlaySpeed(1.0f);
    }
    else {
        endParticle();
        mpGlowingSkyCharacterBrk->setPlaySpeed(-1.0f);
    }
}

void daTagStatue_c::actionWait() {
    if(checkStartDemo()) {
        // Statue is far enough away and player is close enough to the tag for the cutscene to be ordered
        setAction(ACTION_ORDER_EVENT_e);
        fopAcM_orderOtherEventId(this, mEventId, mMapToolId, 0xFFFF, 0, 1);
        eventInfo.onCondition(0x0002);
    }
}

void daTagStatue_c::actionOrderEvent() {
    if(eventInfo.checkCommandDemoAccrpt()) {
        // Cutscene order accepted
        setAction(ACTION_EVENT_e);
        mDoAud_subBgmStart(Z2BGM_KOMONJO_GET_INTRO);
        demoProc();
    }
    else {
        // Cutscene order has not been accepted yet, attempt again
        fopAcM_orderOtherEventId(this, mEventId, mMapToolId, 0xFFFF, 0, 1);
        eventInfo.onCondition(0x0002);
    }
}

void daTagStatue_c::actionEvent() {
    if(dComIfGp_evmng_endCheck(mEventId)) {
        setAction(ACTION_DEAD_e);
        dComIfGp_event_reset();
        fopAcM_onSwitch(this, getSwbit2());
        dComIfGs_onEventBit(l_event_bit[mSkyCharacterEventBitIdIndex]);
    }
    else {
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

    int act_id = dComIfGp_evmng_getMyActIdx(mStaffId, action_table, ARRAY_SIZEU(action_table), 0, 0);

    if(dComIfGp_evmng_getIsAddvance(mStaffId)) {
        switch(act_id) {
            case DEMO_ACTION_WAIT: {
                // Delay fading to black
                int* timer_p = dComIfGp_evmng_getMyIntegerP(mStaffId, "Timer");

                if (!timer_p) {
                    mTimer = 1;
                } else {
                    mTimer = *timer_p;
                }

                break;
            }
            case DEMO_ACTION_MOVE_OWL_STATUE:
                // Position and orient Owl Statue behind player
                if(mSkyCharacterEventBitIdIndex != TAG_LOCATION_GERUDO_DESERT && mSkyCharacterEventBitIdIndex != TAG_LOCATION_LAKE_HYLIA && mSkyCharacterEventBitIdIndex != TAG_LOCATION_RUINED_THEATRE) {
                    fopAc_ac_c* found_owl_statue = reinterpret_cast<fopAc_ac_c*>(fpcEx_Search(s_s_sub, this));
                    if(found_owl_statue) {
                        found_owl_statue->current.pos = l_statue_pos[mSkyCharacterEventBitIdIndex];
                        found_owl_statue->shape_angle.y = l_statue_ang[mSkyCharacterEventBitIdIndex];
                        fopAcM_OnStatus(found_owl_statue, 0x800);
                    }
                }
                break;
            case DEMO_ACTION_AWARD_ITEM:
                // If the player already has 5 Sky Characters, reward them with the completed Ancient Sky Book,
                //  otherwise, reward another character
                int item;
                if(getLetterCount() == 5) {
                    item = fpcNm_ITEM_ANCIENT_DOCUMENT2;
                    /* Sky character - Sky character 6 */
                    dComIfGs_onEventBit(dSv_event_flag_c::F_0796);
                }
                else {
                    item = fpcNm_ITEM_AIR_LETTER;
                }

                mTreasureDemoItemId = fopAcM_createItemForTrBoxDemo(&fopAcM_GetPosition(this), item, -1, fopAcM_GetRoomNo(this), 0, 0);

                if(static_cast<u32>(mTreasureDemoItemId + (1 << 16)) != 0xFFFF) {
                    dComIfGp_event_setItemPartnerId(mTreasureDemoItemId);
                }

                break;
        }
    }

    switch(act_id) {
        case DEMO_ACTION_WAIT:
            if(!cLib_calcTimer<u8>(&mTimer)) {
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
    for(int i = 0; i < 3; i++) {
        if(!mpLightRingEmitters[i]) {
            mpLightRingEmitters[i] = dComIfGp_particle_set(l_eff_id[i], &current.pos, &home.angle, &scale);
        }
    }
}

void daTagStatue_c::endParticle() {
    for(int i = 0; i < 3; i++) {
        if(mpLightRingEmitters[i]) {
            mpLightRingEmitters[i]->becomeInvalidEmitter();
            mpLightRingEmitters[i]->quitImmortalEmitter();
            mpLightRingEmitters[i] = NULL;
        }
    }
}

// Check if statue is far enough away from the Sky Character platform and if player is close enough to the tag for the cutscene
//  of obtaining a Sky Character to be ordered
BOOL daTagStatue_c::checkStartDemo() {
    const fopAc_ac_c* const player = dComIfGp_getPlayer(0);

    if(!checkOnEffect()) {
        // Owl Statue is not far enough away from tag
        return 0;
    }

    const f32 tag_distance_to_player = sqrtf(PSVECSquareDistance(&player->current.pos, &current.pos));

    return (tag_distance_to_player < 100.0f);
}

// Return whether the owl statue has moved far enough away (in the horizontal plane) from the platform where the Sky Character is obtained 
//  in order for its glowing effects to occur and the cutscene to be ordered
BOOL daTagStatue_c::checkOnEffect() {
    BOOL is_switch = fopAcM_isSwitch(this, getSwbit2());
    if(!is_switch && static_cast<u32>(mOwlStatueId + (1 << 16)) != 0xFFFF) {
        fopAc_ac_c* found_owl_statue = fopAcM_SearchByID(mOwlStatueId);
        if(found_owl_statue) {
            Vec statue_tag_xy_pos;
            Vec found_owl_statue_xy_pos;

            const f32 found_owl_statue_z = fopAcM_GetPosition(found_owl_statue).z;
            const f32 found_owl_statue_x = fopAcM_GetPosition(found_owl_statue).x;
            found_owl_statue_xy_pos.x = found_owl_statue_x;
            found_owl_statue_xy_pos.y = 0.0f;
            found_owl_statue_xy_pos.z = found_owl_statue_z;

            const f32 statue_tag_z = fopAcM_GetPosition(this).z;
            const f32 statue_tag_x = fopAcM_GetPosition(this).x;
            statue_tag_xy_pos.x = statue_tag_x;
            statue_tag_xy_pos.y = 0.0f;
            statue_tag_xy_pos.z = statue_tag_z;

            const f32 distance_between_statue_and_tag = sqrtf(PSVECSquareDistance(&found_owl_statue_xy_pos, &statue_tag_xy_pos));

            if(distance_between_statue_and_tag > 120.0f) return TRUE;
        }
        else {
            // No Owl Statue found, platform cannot be covered
            return TRUE;
        }
    }

    return FALSE;
}

s32 daTagStatue_c::getLetterCount() {
    s32 numLetters = 0;
    for(int i = 0; i < 6; i++) {
        if(dComIfGs_isEventBit(l_event_bit[i])) {
            numLetters++;
        }
    }

return numLetters;
}

int daTagStatue_c::draw() {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();
    env_light->settingTevStruct(0, &current.pos, &tevStr);
    env_light->setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();

    mpGlowingSkyCharacterBrk->entry(mpModel->getModelData());
    mpPlatformCircleBtk->entry(mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);
    mDoExt_brkAnmRemove(mpModel->getModelData());
    mDoExt_btkAnmRemove(mpModel->getModelData());

    dComIfGd_setList();

    return 1;
}

int daTagStatue_c::_delete() {
    endParticle();
    dComIfG_resDelete(&mArcPhase, l_arcName);
    if(l_evArcName[mSkyCharacterEventBitIdIndex]) {
        dComIfG_resDelete(&mEvArcPhase, l_evArcName[mSkyCharacterEventBitIdIndex]);
    }

    return 1;
}

 static int daTagStatue_Draw(daTagStatue_c* i_this) {
    return i_this->draw();
 }

static int daTagStatue_Execute(daTagStatue_c* i_this) {
    return i_this->execute();
}

static int daTagStatue_Delete(daTagStatue_c* i_this) {
    return i_this->_delete();
}

static int daTagStatue_Create(daTagStatue_c* i_this) {
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
