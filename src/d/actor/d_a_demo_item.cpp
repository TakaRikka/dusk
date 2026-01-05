/**
 * @file d_a_demo_item.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_demo_item.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_item_data.h"
#include "d/d_a_itembase_static.h"
#include "d/actor/d_a_player.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "Z2AudioLib/Z2Instances.h"

static cXyz l_player_offset = cXyz(0.0f, 115.0f, 54.0f);

static cXyz l_wolf_offset = cXyz(13.0f, 135.0f, 57.0f);

static cXyz l_horse_offset = cXyz(0.0f, 20.0f, 54.0f);

#if DEBUG

class DitemHIO_c 
: public mDoHIO_entry_c 
{
public:
    ~DitemHIO_c() {}
    void genMessage(JORMContext*) {}
    
    /* 0x08 */ cXyz field_0x08;
    /* 0x14 */ u16 field_0x14;
    /* 0x16 */ u16 field_0x16;
    /* 0x18 */ u16 field_0x18;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ u16 field_0x20;
    /* 0x22 */ u8 field_0x22;
    /* 0x23 */ u8 field_0x23;
    /* 0x24 */ u8 field_0x24;

    DitemHIO_c() {
        field_0x20 = 0xfa;
        field_0x23 = 0;
        field_0x08 = l_player_offset;
        field_0x24 = 0;
        field_0x14 = 0x8c;
        field_0x16 = 0x7c;
        field_0x18 = 0x6a;
        field_0x1c = 8.0f;
        field_0x22 = 0;
    }
};

#endif

int daDitem_c::__CreateHeap() {
    return 1;
}

static const GXColor l_light_color = {0x8C, 0x7C, 0x6A, 0xFF};

int daDitem_c::CreateInit() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    
    hide();

    field_0x93d = daDitem_prm::getFlag(this);
    field_0x93e = 1;

    initEffectLight();

    if (chkArgFlag(0x20)) {
        scale.setall(1.0f);
    } else {
        scale.setall(0.0f);
    }

    mSound.init(&current.pos, 1);
    set_mtx();
    return 1;
}

void daDitem_c::action() {
    switch (mAction) {
    case ACTION_START_e:
        actionStart();
        break;
    case ACTION_EVENT_e:
        actionEvent();
        break;
    case ACTION_WAIT_LIGHT_END_e:
        actionWaitLightEnd();
        break;
    case ACTION_END_e:
        actionEnd();
        break;
    }

    followInsectEffect();
}

void daDitem_c::actionStart() {
    if (chkDraw()) {
        if (!chkArgFlag(0x2) && !chkArgFlag(0x4) && !mSetLightEff) {
            mSetLightEff = true;
            settingEffectLight();
            dKy_efplight_set(&mLight);

            // "Get Item: Effect Light set & display start!\n"
            OS_REPORT("ゲットアイテム：エフェクトライトセット＆表示スタート！\n");
        }

        if (m_itemNo == fpcNm_ITEM_DUNGEON_EXIT || m_itemNo == fpcNm_ITEM_DUNGEON_EXIT_2) {
            current.angle.y = dComIfGp_getPlayer(0)->shape_angle.y;
        }

        if (m_itemNo == fpcNm_ITEM_UTAWA_HEART || m_itemNo == fpcNm_ITEM_KAKERA_HEART) {
            JPABaseEmitter* emitter = field_0x96c.getEmitter();
            if (emitter == NULL) {
                dComIfGp_particle_set(0x8DE, &current.pos, NULL, NULL, 0xFF, &field_0x96c, -1, NULL, NULL, NULL);
            }

            emitter = field_0x980.getEmitter();
            if (emitter == NULL) {
                dComIfGp_particle_set(0x8DF, &current.pos, NULL, NULL, 0xFF, &field_0x96c, -1, NULL, NULL, NULL);
            }

            mParticleAlpha = 0xFF;
        }

        if ((m_itemNo == fpcNm_ITEM_ORANGE_RUPEE || m_itemNo == fpcNm_ITEM_SILVER_RUPEE) && field_0x96c.getEmitter() == NULL) {
            cXyz scale(0.55f, 0.55f, 0.55f);
            dComIfGp_particle_set(0xC14, &field_0x99c, NULL, &scale, 0xFF, &field_0x96c, -1, NULL, NULL, NULL);
        }

        if (m_itemNo == fpcNm_ITEM_WALLET_LV3 && field_0x96c.getEmitter() == NULL) {
            cXyz scale(1.2f, 1.2f, 1.2f);
            dComIfGp_particle_set(0xC14, &field_0x99c, NULL, &scale, 0xFF, &field_0x96c, -1, NULL, NULL, NULL);
        }

        if (m_itemNo == fpcNm_ITEM_CHUCHU_RARE && field_0x96c.getEmitter() == NULL) {
            cXyz scale(0.8f, 0.8f, 0.8f);
            dComIfGp_particle_set(0xC14, &field_0x99c, NULL, &scale, 0xFF, &field_0x96c, -1, NULL, NULL, NULL);
        }

        if ((m_itemNo == fpcNm_ITEM_FAIRY_DROP || m_itemNo == fpcNm_ITEM_DROP_BOTTLE) && field_0x96c.getEmitter() == NULL) {
            dComIfGp_particle_set(0x8C15, &field_0x99c, NULL, NULL, 0xFF, &field_0x96c, -1, NULL, NULL, NULL);
        }

        if (isInsect(m_itemNo)) {
            setInsectEffect();
        }

        fopAcM_onDraw(this);
        setAction(ACTION_EVENT_e);
    }
}

void daDitem_c::actionEvent() {
    if (mSetLightEff) {
        cLib_addCalc2(&mLight.mPow, mLightStrength, 0.1f, 1.0f);
    }

    if (m_itemNo == fpcNm_ITEM_POU_SPIRIT) {
        mSound.startLevelSound(Z2SE_EN_PO_SOUL, 0, -1);
    }

    if (chkDead()) {
        if (!chkArgFlag(0x1)) {
            execItemGet(m_itemNo);
        }

        if (m_itemNo == fpcNm_ITEM_KAKERA_HEART) {
            int room_no = dComIfGp_roomControl_getStayNo();

            if (strcmp(dComIfGp_getStartStageName(), "F_SP116") == 0 && room_no == 2) {
                onEventReg(0, 0);
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP118") == 0 && room_no == 1) {
                onEventReg(0, 1);
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0 && room_no == 6) {
                onEventReg(0, 2);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_SB06") == 0 && room_no == 1 && dComIfG_play_c::getLayerNo(0) == 2) {
                onEventReg(0, 3);
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP115") == 0 && room_no == 0) {
                onEventReg(0, 4);
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP108") == 0 && room_no == 14) {
                onEventReg(0, 5);
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP115") == 0 && room_no == 1) {
                onEventReg(0, 6);
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP121") == 0 && room_no == 11) {
                onEventReg(1, 0);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_SB06") == 0 && room_no == 1 && dComIfG_play_c::getLayerNo(0) == 0) {
                onEventReg(1, 1);
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP110") == 0 && room_no == 0) {
                onEventReg(1, 2);
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP109") == 0 && room_no == 0) {
                if (dComIfGs_isTmpBit(0x0604)) {
                    onEventReg(1, 3);
                }
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP126") == 0 && room_no == 0) {
                onEventReg(1, 5);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN05") == 0 && room_no == 5) {
                onEventReg(1, 6);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN05") == 0 && room_no == 3) {
                onEventReg(1, 7);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN04") == 0 && room_no == 6) {
                onEventReg(2, 0);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN04") == 0 && room_no == 4) {
                onEventReg(2, 1);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN01") == 0 && room_no == 3) {
                onEventReg(2, 2);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN01") == 0 && room_no == 8) {
                onEventReg(2, 3);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN10") == 0 && room_no == 13) {
                onEventReg(2, 4);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN10") == 0 && room_no == 2) {
                onEventReg(2, 5);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN11") == 0 && room_no == 7) {
                onEventReg(2, 6);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN11") == 0 && room_no == 0) {
                onEventReg(2, 7);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN06") == 0 && room_no == 4) {
                onEventReg(3, 0);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN06") == 0 && room_no == 3) {
                onEventReg(3, 1);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN07") == 0 && room_no == 11) {
                onEventReg(3, 2);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN07") == 0 && room_no == 12) {
                onEventReg(3, 3);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0 && room_no == 4) {
                onEventReg(3, 4);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0 && room_no == 1) {
                onEventReg(3, 5);
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP128") == 0 && room_no == 0) {
                onEventReg(3, 7);
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP114") == 0 && room_no == 1) {
                onEventReg(4, 0);
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP00") == 0 && room_no == 0) {
                onEventReg(4, 3);
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP108") == 0 && room_no == 5) {
                onEventReg(4, 5);
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0 && room_no == 0) {
                onEventReg(4, 6);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_SB02") == 0 && room_no == 0) {
                onEventReg(4, 7);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_SB03") == 0 && room_no == 0) {
                onEventReg(5, 0);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_SB04") == 0 && room_no == 10) {
                onEventReg(5, 1);
            } else if (strcmp(dComIfGp_getStartStageName(), "D_SB00") == 0 && room_no == 0) {
                onEventReg(5, 2);
            } else if (strcmp(dComIfGp_getStartStageName(), "F_SP117") == 0 && room_no == 2) {
                onEventReg(5, 3);
            }
        }

        endInsectEffect();
        fopAcM_offDraw(this);
        setAction(ACTION_WAIT_LIGHT_END_e);
    }
}

void daDitem_c::actionWaitLightEnd() {
    BOOL particle_hidden = TRUE;

    if (m_itemNo == fpcNm_ITEM_UTAWA_HEART || m_itemNo == fpcNm_ITEM_KAKERA_HEART) {
        particle_hidden = cLib_chaseUC(&mParticleAlpha, 0, 26);

        JPABaseEmitter* emitter = field_0x96c.getEmitter();
        if (emitter != NULL) {
            emitter->setGlobalAlpha(mParticleAlpha);
        }

        emitter = field_0x980.getEmitter();
        if (emitter != NULL) {
            emitter->setGlobalAlpha(mParticleAlpha);
        }
    }

    cLib_addCalc(&mLight.mPow, 0.0f, 0.2f, 3.0f, 1.0f);

    if (mSetLightEff && mLight.mPow == 0.0f && particle_hidden) {
        setAction(ACTION_END_e);
    } else if (chkArgFlag(0x2) || chkArgFlag(0x4)) {
        setAction(ACTION_END_e);
    }
}

void daDitem_c::actionEnd() {
    fopAcM_delete(this);
}

void daDitem_c::setInsectEffect() {
    if (!chkArgFlag(0x2) && !chkArgFlag(0x4)) {
        field_0x994 = dComIfGp_particle_set(0xA1B, &current.pos, &home.angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
    }

    field_0x998 = dComIfGp_particle_set(0xA1C, &current.pos, &home.angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
}

void daDitem_c::followInsectEffect() {
    if (field_0x994 != NULL) {
        field_0x994->setGlobalTranslation(current.pos);
    }

    if (field_0x998 != NULL) {
        field_0x998->setGlobalTranslation(current.pos);
    }
}

void daDitem_c::endInsectEffect() {
    if (field_0x994 != NULL) {
        field_0x994->stopCreateParticle();
        field_0x994->quitImmortalEmitter();
        field_0x994->becomeInvalidEmitter();
        field_0x994 = NULL;
    }

    if (field_0x998 != NULL) {
        field_0x998->stopCreateParticle();
        field_0x998->quitImmortalEmitter();
        field_0x998->becomeInvalidEmitter();
        field_0x998 = NULL;
    }
}

void daDitem_c::onEventReg(int i_regNo, int i_value) {
    static u16 const l_event_reg[] = {
        0xF0FF, 0xEFFF, 0xEEFF, 0xEDFF, 0xECFF, 0xEBFF,
    };

    u8 reg = dComIfGs_getEventReg(l_event_reg[i_regNo]);
    reg |= (1 << i_value);

    dComIfGs_setEventReg(l_event_reg[i_regNo], reg);

    // "Heart Piece: %d %d\n"
    OS_REPORT("ハートの欠片：%d %d\n", i_regNo, i_value);
}

void daDitem_c::set_pos() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    cXyz pos;
    cXyz offset;
    cXyz sp38(player->current.pos);

    static cXyz offset_tbl[] = {
        cXyz(0.0f, 200.0f, 0.0f),
        cXyz(0.0f, 90.0f, 50.0f),
        cXyz(30.0f, 140.0f, 20.0f),
    };

    if (chkArgFlag(0x8)) {
        current.pos = mOffsetPos;
        return;
    } else if (chkArgFlag(0x2)) {
        offset = offset_tbl[1];
    } else if (chkArgFlag(0x4)) {
        offset = offset_tbl[1];
    } else if (chkArgFlag(0x10)) {
        offset = mOffsetPos;
    } else {
        offset = offset_tbl[0];
    }

    if (daPy_py_c::checkNowWolf()) {
        offset = l_wolf_offset;
        sp38.y = player->getLeftFootPosP()->y;
    } else if (player->checkHorseRide()) {
        offset = l_horse_offset;
    } else {
        offset = l_player_offset;
        sp38.y = player->getLeftFootPosP()->y;
    }

    cLib_offsetPos(&pos, &sp38, player->shape_angle.y, &offset);
    current.pos = pos;
}

void daDitem_c::anim_control() {
    animPlay(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void daDitem_c::initEffectLight() {
    mLight.mColor.r = l_light_color.r;
    mLight.mColor.g = l_light_color.g;
    mLight.mColor.b = l_light_color.b;
    mLight.mPow = 0.0f;
    mLight.mFluctuation = 0.0f;
    mSetLightEff = false;
}

void daDitem_c::settingEffectLight() {
    f32 var_f31 = mpModel->getModelData()->getJointNodePointer(0)->getMax()->y;
    mLight.mPosition.set(current.pos.x, current.pos.y + (var_f31 * 0.5f), current.pos.z);
    mLightStrength = 8.0f;
}

void daDitem_c::set_mtx() {
    mpModel->setBaseScale(scale);

    if (m_itemNo != fpcNm_ITEM_DUNGEON_EXIT && m_itemNo != fpcNm_ITEM_DUNGEON_BACK && m_itemNo != fpcNm_ITEM_LV7_DUNGEON_EXIT && m_itemNo != fpcNm_ITEM_DUNGEON_EXIT_2) {
        s16 var_r28 = 0xFFFF / 250;
        fopAcM_addAngleY(this, current.angle.y + var_r28, var_r28);
    }

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daDitem_c::setTevStr() {
    g_env_light.settingTevStruct(14, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
}

void daDitem_c::setListStart() {}

void daDitem_c::draw_WOOD_STICK() {
    static const char nodisp_mat[] = "parts_m";
    
    J3DModelData* modelData = mpModel->getModelData();
    JUTNameTab* nametab = modelData->getMaterialTable().getMaterialName();

    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        if (strcmp(nodisp_mat, nametab->getName(i)) == 0) {
            modelData->getMaterialNodePointer(i)->getShape()->hide();
        }
    }
}

int daDitem_c::Delete() {
    if (!chkArgFlag(0x2) && !chkArgFlag(0x4) && mSetLightEff) {
        dKy_efplight_cut(&mLight);
    }

    if (m_itemNo == fpcNm_ITEM_UTAWA_HEART || m_itemNo == fpcNm_ITEM_KAKERA_HEART) {
        field_0x96c.remove();
        field_0x980.remove();
    }

    if (m_itemNo == fpcNm_ITEM_ORANGE_RUPEE || m_itemNo == fpcNm_ITEM_SILVER_RUPEE || m_itemNo == fpcNm_ITEM_WALLET_LV3 || m_itemNo == fpcNm_ITEM_FAIRY_DROP || m_itemNo == fpcNm_ITEM_DROP_BOTTLE || m_itemNo == fpcNm_ITEM_CHUCHU_RARE) {
        field_0x96c.remove();
    }

    endInsectEffect();
    mSound.deleteObject();
    return DeleteBase(dItem_data::getArcName(m_itemNo));
}

static int daDitem_Delete(daDitem_c* i_this) {
    return i_this->Delete();
}

int daDitem_c::create() {
    fopAcM_ct(this, daDitem_c);

    m_itemNo = daDitem_prm::getNo(this);

    const char* arc_name = dItem_data::getArcName(m_itemNo);
    s16 bmd_name = dItem_data::getBmdName(m_itemNo);

    if (bmd_name < 0 || arc_name == NULL) {
        if (bmd_name == 0) {
            // "It's an item with no model data! [%d]\n"
            OS_REPORT_ERROR("モデルデータのないアイテムゲットです！[%d]\n", m_itemNo);
        }

        if (arc_name == NULL) {
            // "There's no archive name for Get Model! [%d]\n"
            OS_REPORT_ERROR("ゲット用モデルのアーカイブ名がありません！[%d]\n", m_itemNo);
        }

        m_itemNo = fpcNm_ITEM_GREEN_RUPEE;
    }

    int phase_state = dComIfG_resLoad(&mPhase, dItem_data::getArcName(m_itemNo));
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckItemCreateHeap, 0x80003390)) {
            return cPhs_ERROR_e;
        }

        CreateInit();
        OS_REPORT("DEMOITEM PARAM:%x\n", fopAcM_GetParam(this));
    }

    return phase_state;
}

static int daDitem_Create(daDitem_c* i_this) {
    return i_this->create();
}

int daDitem_c::execute() {
    m_timer++;

    action();
    set_pos();
    anim_control();

    if (chkDraw()) {
        f32 target_scale_x = 1.0f;
        if (mMaxScale != 0.0f) {
            target_scale_x = mMaxScale;
        }

        cLib_chaseF(&scale.x, target_scale_x, 0.35f);
        scale.y = scale.x;
        scale.z = scale.x;
    }

    settingEffectLight();
    set_mtx();

    eyePos = current.pos;
    eyePos.y += mpModel->getModelData()->getJointNodePointer(0)->getMax()->y * 0.5f;

    if (m_itemNo == fpcNm_ITEM_ORANGE_RUPEE || m_itemNo == fpcNm_ITEM_SILVER_RUPEE) {
        field_0x99c = current.pos;
        field_0x99c.y += 10.0f;
    } else if (m_itemNo == fpcNm_ITEM_WALLET_LV3 || m_itemNo == fpcNm_ITEM_CHUCHU_RARE) {
        field_0x99c = current.pos;
        field_0x99c.y += 15.0f;
    } else if (m_itemNo == fpcNm_ITEM_FAIRY_DROP || m_itemNo == fpcNm_ITEM_DROP_BOTTLE) {
        field_0x99c = current.pos;
    }

    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    return 1;
}

static int daDitem_Execute(daDitem_c* i_this) {
    return i_this->execute();
}

int daDitem_c::draw() {
    switch (m_itemNo) {
    case fpcNm_ITEM_WOOD_STICK:
        draw_WOOD_STICK();
        break;
    }

    DrawBase();
    return 1;
}

static int daDitem_Draw(daDitem_c* i_this) {
    return i_this->draw();
}

static actor_method_class l_daDitem_Method = {
    (process_method_func)daDitem_Create,
    (process_method_func)daDitem_Delete,
    (process_method_func)daDitem_Execute,
    (process_method_func)NULL,
    (process_method_func)daDitem_Draw,
};

actor_process_profile_definition g_profile_Demo_Item = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Demo_Item,         // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daDitem_c),      // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  241,                    // mPriority
  &l_daDitem_Method,      // sub_method
  0x00060000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
