/**
 * @file d_a_cstaF.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_cstaF.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_crod.h"
#include "d/d_bg_w.h"

enum daCstaF_Action {
    ACTION_WAIT,
    ACTION_WALK,
    ACTION_SWING,
    ACTION_FALL,
};

static void daCstaF_rideCB(dBgW* i_bgw, fopAc_ac_c* i_this, fopAc_ac_c* i_rideActor) {
    if (i_rideActor == (fopAc_ac_c*)daPy_getLinkPlayerActorClass()) {
        ((daCstaF_c*)i_this)->onPlayerRide();
    }
}

static u8 const lit_3765[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const char l_arcNameF[] = "CstaF";

static const char l_arcNameFB[] = "CstaFB";

static const char l_spStageName[8] = "R_SP209";

const daCstaF_c::BckTbl daCstaF_c::m_bckIdxTable[] = {
    {9, 9, 8, 7},
    {9, 9, 8, 7}
};

int daCstaF_c::CreateHeap() {
    struct data {
        u16 bmd_idx;
        u16 bck_idx;
        u16 brk_idx;
        u16 btk_idx;
    };
    static const data dataTbl[] = {
        {12, 9, 16, 19},
        {12, 9, 17, 21},
    };

    const data* data_p = &dataTbl[m_type];

    void* pbck = dComIfG_getObjectRes(m_arcName, data_p->bck_idx);
    mp_modelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes(m_arcName, data_p->bmd_idx), NULL, NULL, (J3DAnmTransform*)pbck, 0, 0.0f, 0, -1, &m_sound, 0x80000, 0x11000284);
    if (mp_modelMorf == NULL || mp_modelMorf->getModel() == NULL) {
        return 0;
    }

    mp_model = mp_modelMorf->getModel();
    if (!initBrk(data_p->brk_idx)) {
        return 0;
    }

    void* pbtk = dComIfG_getObjectRes(m_arcName, data_p->btk_idx);
    if (!m_btk.init(mp_model->getModelData(), (J3DAnmTextureSRTKey*)pbtk, 1, 0, 1.0f, 0, -1)) {
        return 0;
    }

    if (m_arcName == l_arcNameFB && checkCoverModel()) {
        mp_coverModel = mDoExt_J3DModel__create((J3DModelData*)dComIfG_getObjectRes(m_arcName, 0xD), 0x80000, 0x11000284);
        if (mp_coverModel == NULL) {
            return 0;
        }
    }

    return 1;
}

BOOL daCstaF_c::checkCoverModel() {
            /* dSv_event_flag_c::F_0303 - Kakariko Village - Saw Shad's spell 2 */
    return !dComIfGs_isEventBit(0x2540)
           && strcmp(dComIfGp_getStartStageName(), l_spStageName) == 0;
}

static dCcD_SrcCyl l_cylSrc = {
    {
        {0x0, {{AT_TYPE_THROW_OBJ, 0x1, 0x12}, {0xd97afddf, 0x11}, 0x79}}, // mObj
        {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x5, 0x0, 0x0, 0x3}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            125.0f, // mRadius
            390.0f // mHeight
        } // mCyl
    }
};

int daCstaF_c::create() {
    fopAcM_ct(this, daCstaF_c);

    m_type = fopAcM_GetParam(this) & 0xF;
    if (m_type >= 2) {
        m_type = 0;
    }

    m_swbit = (fopAcM_GetParam(this) >> 4);
    m_mapToolID = (fopAcM_GetParam(this) >> 12);
    m_eventId = dComIfGp_getEventManager().getEventIdx(this, m_mapToolID);

    u16 dzb_id;
    u32 heap_size = 0xD40;
    if (m_type == 0) {
        m_arcName = l_arcNameF;
        dzb_id = 22;
    } else {
        m_arcName = l_arcNameFB;
        dzb_id = 25;
        if (checkCoverModel()) {
            heap_size = 0x1700;
        }
    }

    int phase_state = dComIfG_resLoad(&m_phase, m_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state = MoveBGCreate(m_arcName, dzb_id, dBgS_MoveBGProc_TypicalRotY, heap_size, NULL);
        if (phase_state != cPhs_COMPLEATE_e) {
            return phase_state;
        }

        mp_modelMorf->setMorf(1.0f);
        m_action = ACTION_WAIT;
        mp_model->setBaseScale(scale);
        mp_modelMorf->play(0, 0);
    
        m_cc_cyl.Set(l_cylSrc);
        m_cc_cyl.SetStts(&m_cc_stts);
        m_cc_cyl.StartCAt(current.pos);

        fopAcM_SetMtx(this, mp_model->getBaseTRMtx());
        m_lastGroundH = current.pos.y;

        m_acchCir[0].SetWall(30.01f, 80.0f);
        m_acchCir[1].SetWall(160.0f, 80.0f);
        m_acchCir[2].SetWall(280.0f, 80.0f);
        m_acchCir[3].SetWall(400.0f, 80.0f);

        m_cc_stts.Init(255, 0, this);

        fopAcM_SetMax(this, 475.0f, 450.0f, 475.0f);
        fopAcM_SetMin(this, -475.0f, 0.0f, -475.0f);

        gravity = -8.0f;
        maxFallSpeed = -100.0f;
        mpBgW->SetRideCallback(daCstaF_rideCB);
        m_acch.Set(this, 4, m_acchCir);
        model = mp_model;

        initStopBrkBtk();
        m_brk.setFrame(m_brk.getEndFrame());

        m_acch.CrrPos(dComIfG_Bgsp());
        setMatrix();
        setRoomInfo();

        if (mp_coverModel != NULL) {
            J3DModelData* coverModelData = mp_coverModel->getModelData();
            mp_coverModel->setBaseScale(scale);
            mp_coverModel->setBaseTRMtx(mp_model->getBaseTRMtx());

            m_coverBtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(m_arcName, 0x16);
            JUT_ASSERT(367, m_coverBtk);
            m_coverBtk->searchUpdateMaterialID(coverModelData);
            m_coverBtk->setFrame(0.0f);
            coverModelData->entryTexMtxAnimator(m_coverBtk);

            m_coverBrk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(m_arcName, 0x12);
            JUT_ASSERT(376, m_coverBrk);
            m_coverBrk->searchUpdateMaterialID(coverModelData);
            m_coverBrk->setFrame(0.0f);
            coverModelData->entryTevRegAnimator(m_coverBrk);

            m_cc_cyl.SetTgType(0xD87AFDDF);
        }

        m_playerPos = daPy_getLinkPlayerActorClass()->current.pos;
        m_poly_eff.init(&m_acch, 30.0f, m_cc_cyl.GetH());
        m_sound.init(&current.pos, &eyePos, 4, 1);
    }

    return phase_state;
}

static int daCstaF_Create(fopAc_ac_c* i_this) {
    return ((daCstaF_c*)i_this)->create();
}

daCstaF_c::~daCstaF_c() {
    m_sound.deleteObject();

    if (mp_modelMorf != NULL) {
        mp_modelMorf->stopZelAnime();
    }

    dComIfG_resDelete(&m_phase, m_arcName);
}

static int daCstaF_Delete(daCstaF_c* a_this) {
    a_this->MoveBGDelete();
    a_this->~daCstaF_c();
    return 1;
}

void daCstaF_c::setRoomInfo() {
    int room_no;
    if (m_acch.GetGroundH() != -G_CM3D_F_INF) {
        room_no = dComIfG_Bgsp().GetRoomId(m_acch.m_gnd);
        tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(m_acch.m_gnd);
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
    }

    tevStr.room_no = room_no;
    m_reverb = dComIfGp_getReverb(room_no);
    m_cc_stts.SetRoomId(room_no);
    fopAcM_SetRoomNo(this, room_no);
}

void daCstaF_c::setMatrix() {
    static const Vec fieldLocalBallPos = {0.0f, 200.0f, 0.0f};

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mp_model->setBaseTRMtx(mDoMtx_stack_c::get());
    mp_modelMorf->modelCalc();

    attention_info.position.set(current.pos.x, current.pos.y + fieldLocalBallPos.y, current.pos.z);
    mDoMtx_multVec(mp_model->getAnmMtx(0), &fieldLocalBallPos, &m_ballPos);
    eyePos = attention_info.position;
}

void daCstaF_c::posMove() {
    BOOL pre_gnd_hit = m_acch.ChkGroundHit();
    m_acch.OffLineCheck();

    speed.y += gravity;
    if (speed.y < maxFallSpeed) {
        speed.y = maxFallSpeed;
    }
    current.pos.y += speed.y;

    daAlink_c* player = daAlink_getAlinkActorClass();
    if (player->getCopyRodControllActor() == this) {
        if (!m_isStop) {
            shape_angle.y = player->shape_angle.y;
        }

        if (m_acch.ChkGroundHit() && m_action != ACTION_FALL) {
            current.pos.x += speed.x;
            current.pos.z += speed.z;
            speedF = JMAFastSqrt((speed.x * speed.x) + (speed.z * speed.z));

            if (m_action == ACTION_WALK) {
                f32 play_speed = mp_modelMorf->getPlaySpeed();
                f32 speed_target;
                if (speedF >= 1.0f) {
                    current.angle.y = speed.atan2sX_Z();
                    speed_target = speedF / 18.0f;
                    if (speed_target > 1.0f) {
                        speed_target = 1.0f;
                    }
                    speed_target = 0.5f + speed_target;
                } else {
                    current.angle.y = shape_angle.y;
                    speed_target = 0.0f;
                }

                cLib_chaseF(&play_speed, speed_target, 0.2f);
                mp_modelMorf->setPlaySpeed(play_speed);
            }
        }
    } else if (m_action == ACTION_WAIT) {
        mp_modelMorf->setPlaySpeed(0.0f);
        speedF = 0.0f;
    }

    m_playerPos = player->current.pos;
    current.pos += *m_cc_stts.GetCCMoveP();
    m_cc_stts.ClrCcMove();
    m_acch.CrrPos(dComIfG_Bgsp());

    if (pre_gnd_hit && !m_acch.ChkGroundHit() && -G_CM3D_F_INF != m_acch.GetGroundH()) {
        f32 ground_dist = m_acch.GetGroundH() - current.pos.y;
        cM3dGPla tri;
        dComIfG_Bgsp().GetTriPla(m_acch.m_gnd, &tri);
        
        s16 poly_angle = fopAcM_getPolygonAngle(&tri, current.angle.y);
        f32 var_f30 = current.pos.absXZ(old.pos) * cM_ssin(poly_angle);
        if (var_f30 < 0.0f) {
            var_f30 = 0.0f;
        }

        if (ground_dist < 0.0f && ground_dist >= -30.0f - var_f30) {
            current.pos.y = m_acch.GetGroundH();
            m_acch.SetGroundHit();
            speed.y = 0.0f;
        } else {
            m_lastGroundH = old.pos.y;
        }
    }

    if (!pre_gnd_hit && m_acch.ChkGroundHit()) {
        if (m_lastGroundH - current.pos.y >= 100.0f) {
            mp_modelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(m_arcName, m_bckIdxTable[m_type].idx[ACTION_FALL]), -1, 0.0f, 1.0f, 0.0f, -1.0f);
            m_action = ACTION_FALL;
        }
    }
}

void daCstaF_c::setCollision() {
    m_cc_stts.Move();

    if (m_isPlayerRide || mp_coverModel != NULL) {
        m_cc_cyl.SetTgType(0xD87AFDDF);
        
        daAlink_c* player = daAlink_getAlinkActorClass();
        if (player->getCopyRodControllActor() == this) {
            ((daCrod_c*)player->getCopyRodActor())->offControll();
        }
    } else {
        m_cc_cyl.SetTgType(0xD97AFDDF);
    }

    m_cc_cyl.MoveCAt(current.pos);
    dComIfG_Ccsp()->Set(&m_cc_cyl);
}

static f32 l_cancelOffset = JMAFastSqrt(73225.008f) + 100.0f;

void daCstaF_c::setAnime() {
    daAlink_c* player = daAlink_getAlinkActorClass();

    fopAc_ac_c* crod_control_actor = player->getCopyRodControllActor();
    if (crod_control_actor == this) {
        f32 max_control_dist = l_cancelOffset + player->getCopyRodBallDisMax();
        if (player->getCopyRodActor()->current.pos.abs2(player->current.pos) > SQUARE(max_control_dist)) {
            ((daCrod_c*)player->getCopyRodActor())->offControll();
            crod_control_actor = NULL;
        }
    }

    int next_action;
    if (crod_control_actor == this) {
        if (dComIfGp_event_runCheck()) {
            speed.x = 0.0f;
            speed.z = 0.0f;
        } else {
            speed.x = player->speed.x + player->transAnimeMoveX();
            speed.z = player->speed.z + player->transAnimeMoveZ();
        }

        if (player->checkCopyRodSwingMode()) {
            next_action = ACTION_SWING;
            m_isStop = FALSE;
        } else if (m_action == ACTION_FALL && !mp_modelMorf->isStop()) {
            next_action = ACTION_FALL;
            m_isStop = FALSE;
        } else {
            if ((fabsf(speed.x) + fabsf(speed.z)) > 0.1f && m_acch.ChkGroundHit()) {
                next_action = ACTION_WALK;
                m_isStop = FALSE;
            } else {
                next_action = ACTION_WAIT;
            }
        }
    } else if (m_action == ACTION_FALL && !mp_modelMorf->isStop()) {
        next_action = ACTION_FALL;
    } else {
        next_action = ACTION_WAIT;
    }

    if (next_action != m_action) {
        void* pbck = dComIfG_getObjectRes(m_arcName, m_bckIdxTable[m_type].idx[next_action]);
        
        f32 anm_speed;
        if (next_action == ACTION_WAIT) {
            anm_speed = 0.0f;
        } else {
            anm_speed = 1.0f;
        }

        mp_modelMorf->setAnm((J3DAnmTransform*)pbck, -1, 3.0f, anm_speed, 0.0f, -1.0f);
        m_action = next_action;
    }

    if (eventInfo.checkCommandDemoAccrpt()) {
        if (!m_isStartBrkBtkInit) {
            initStartBrkBtk();
        } else if (m_brk.checkFrame(15.0f)) {
            m_sound.startCreatureSound(Z2SE_CSTATUE_L_START, 0, m_reverb);
        } else if (m_brk.getFrame() >= 15.0f) {
            m_sound.startCreatureVoiceLevel(Z2SE_CSTATUE_ACTIVE_LOOP, m_reverb);
        }

        if (dComIfGp_evmng_endCheck(m_eventId)) {
            fopAcM_onSwitch(this, m_swbit);
            dComIfGp_event_reset();
        }
    } else if (crod_control_actor == this) {
        if (m_isStartBrkBtkInit) {
            m_btk.play();

            if (m_brk.checkFrame(15.0f)) {
                m_sound.startCreatureSound(Z2SE_CSTATUE_L_START, 0, m_reverb);
            } else if (m_brk.getFrame() >= 15.0f) {
                m_sound.startCreatureVoiceLevel(Z2SE_CSTATUE_ACTIVE_LOOP, m_reverb);
            }
        } else {
            m_sound.startCreatureSound(Z2SE_CSTATUE_HIT_BALL, 0, m_reverb);
            if (m_swbit != 0xFF && !fopAcM_isSwitch(this, m_swbit) && m_mapToolID != 0xFF) {
                fopAcM_orderOtherEventId(this, m_eventId, m_mapToolID, 0xFFFF, 0, 1);
            } else {
                initStartBrkBtk();
            }
        }
    } else if (m_isStartBrkBtkInit) {
        m_isStartBrkBtkInit = FALSE;
        initStopBrkBtk();
        m_sound.startCreatureSound(Z2SE_CSTATUE_L_STOP, 0, m_reverb);
    }

    m_brk.play();
}

BOOL daCstaF_c::initBrk(u16 i_brkIdx) {
    void* pbrk = dComIfG_getObjectRes(m_arcName, i_brkIdx);
    return m_brk.init(mp_model->getModelData(), (J3DAnmTevRegKey*)pbrk, 1, -1, 1.0f, 0, -1);
}

void daCstaF_c::initStopBrkBtk() {
    static const u16 brkIdx[] = {16, 17};

    m_btk.setFrame(0.0f);
    initBrk(brkIdx[m_type]);
    m_isStop = TRUE;
}

void daCstaF_c::initStartBrkBtk() {
    m_isStartBrkBtkInit = TRUE;
    m_btk.setFrame(0.0f);
    m_btk.reset();

    static const u16 brIdx[] = {15, 16};
    initBrk(brIdx[m_type]);

    cXyz effpos(m_ballPos.x + (40.0f * cM_ssin(shape_angle.y)), m_ballPos.y, m_ballPos.z + (40.0f * cM_scos(shape_angle.y)));
    csXyz effrot(shape_angle);

    for (int i = 0; i < 2; i++) {
        dComIfGp_particle_set(0x88BB, &effpos, &effrot, NULL);
        dComIfGp_particle_set(0x88BC, &effpos, &effrot, NULL);
        effpos.x = (2.0f * m_ballPos.x) - effpos.x;
        effpos.z = (2.0f * m_ballPos.z) - effpos.z;
        effrot.y += 0x8000;
    }
}

int daCstaF_c::Execute(Mtx** param_0) {
    daAlink_c* player = daAlink_getAlinkActorClass();

    setAnime();
    posMove();

    int poly_sound;
    if (m_acch.GetGroundH() != -G_CM3D_F_INF) {
        poly_sound = dKy_pol_sound_get(&m_acch.m_gnd);
    } else {
        poly_sound = 0;
    }

    if (mp_coverModel != NULL) {
        BOOL hide_cover = FALSE;
        if (m_coverVanishFlg) {
            hide_cover = player->simpleAnmPlay(m_coverBtk);
            hide_cover &= player->simpleAnmPlay(m_coverBrk);
            fopAcM_seStartCurrentLevel(this, Z2SE_CSTATUE_SEAL_VANISH, 0);
        }

        if (hide_cover
               /* dSv_event_flag_c::F_0303 - Kakariko Village - Saw Shad's spell 2 */
            || dComIfGs_isEventBit(0x2540)) {
            mp_coverModel = NULL;
            m_coverVanishFlg = FALSE;
        }
    }

    mp_modelMorf->play(poly_sound, m_reverb);

    if (m_action == ACTION_SWING && player->checkCopyRodSwingMode()) {
        if (mp_modelMorf->getEndFrame() > player->getBaseAnimeFrame()) {
            mp_modelMorf->setFrame(player->getBaseAnimeFrame());
        } else {
            mp_modelMorf->setFrame(mp_modelMorf->getEndFrame());
        }
    }

    setRoomInfo();
    setMatrix();

    if (m_cc_cyl.ChkTgHit()) {
        dCcD_GObjInf* hit_gobj = m_cc_cyl.GetTgHitGObj();
        if (hit_gobj != NULL && hit_gobj->GetAtType() != AT_TYPE_1000000) {
            m_sound.startCollisionSE(m_cc_cyl.GetTgHitObjHitSeID(1), 0x2A);
        }
    }

    setCollision();

    u32 var_r27 = 0;
    if (m_acch.ChkGroundLanding()) {
        var_r27 = 7;
    }

    cXyz* effpos_p;
    if (var_r27 != 0) {
        effpos_p = &current.pos;
    } else {
        effpos_p = NULL;
    }

    m_poly_eff.setEffectCenter(&tevStr, &current.pos, var_r27, 0, effpos_p, &current.angle, &scale, fopAcM_GetRoomNo(this), 1.0f, speedF);
    *param_0 = (Mtx*)mp_model->getAnmMtx(0);
    m_isPlayerRide = FALSE;

    if (player->getCopyRodControllActor() == this) {
        m_cc_stts.SetWeight(200);
    } else {
        m_cc_stts.SetWeight(255);
    }

    return 1;
}

static int daCstaF_Execute(daCstaF_c* a_this) {
    return a_this->MoveBGExecute();
}

int daCstaF_c::Draw() {
    g_env_light.settingTevStruct(8, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mp_model, &tevStr);

    m_brk.entry(mp_model->getModelData());
    m_btk.entry(mp_model->getModelData());
    mDoExt_modelEntryDL(mp_model);

    if (mp_coverModel != NULL) {
        mDoExt_modelUpdateDL(mp_coverModel);
    }

    cXyz sp8(current.pos.x, 30.0f + mp_model->getAnmMtx(0)[1][3], current.pos.z);
    m_shadowKey = dComIfGd_setShadow(m_shadowKey, 1, mp_model, &sp8, 1000.0f, 0.0f, current.pos.y, m_acch.GetGroundH(), m_acch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static int daCstaF_Draw(daCstaF_c* a_this) {
    return a_this->MoveBGDraw();
}

static actor_method_class l_daCstaF_Method = {
    (process_method_func)daCstaF_Create,
    (process_method_func)daCstaF_Delete,
    (process_method_func)daCstaF_Execute,
    (process_method_func)NULL,
    (process_method_func)daCstaF_Draw,
};

actor_process_profile_definition g_profile_CSTAF = {
    fpcLy_CURRENT_e,        // mLayerID
    3,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_CSTAF,             // mProcName
    &g_fpcLf_Method.base,  // sub_method
    0x00000B38,             // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    272,                    // mPriority
    &l_daCstaF_Method,      // sub_method
    0x00040520,             // mStatus
    fopAc_ENV_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
