/**
 * d_a_obj_table.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_table.h"

#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"

static char* l_arcName = "Table";

void daObjTable_c::initBaseMtx() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    setBaseMtx();
}

void daObjTable_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daObjTable_c::create() {
    fopAcM_ct(this, daObjTable_c);

    int phase_state = dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state = MoveBGCreate(l_arcName, 8, NULL, 0x4000, NULL);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }
    }

    return phase_state;
}

static int daObjTable_Draw(daObjTable_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daObjTable_Execute(daObjTable_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjTable_IsDelete(daObjTable_c* i_this) {
    return 1;
}

static int daObjTable_Delete(daObjTable_c* i_this) {
    i_this->MoveBGDelete();
    return 1;
}

static int daObjTable_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjTable_c*>(i_this)->create();
}

int daObjTable_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 5);
    JUT_ASSERT(76, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);

    if (mpModel == NULL) {
        return false;
    }
    return true;
}

int daObjTable_c::Create() {
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    if (dKy_darkworld_check() || dComIfGs_isStageSwitch(0x18, 0x4b)) {
        mpModel->getModelData()->getMaterialNodePointer(0)->getShape()->hide();
        mpModel->getModelData()->getMaterialNodePointer(1)->getShape()->hide();
        mpModel->getModelData()->getMaterialNodePointer(2)->getShape()->hide();
    } else {
        if (dComIfG_play_c::getLayerNo(0) == 4) {
                 /* dSv_event_flag_c::F_0361 - Arbiter's Grounds - Spun the spinning pillars */
            if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[361])) {
                mpModel->getModelData()->getMaterialNodePointer(0)->getShape()->hide();
            }
                /* dSv_event_flag_c::F_0361 - Arbiter's Grounds - Spun the spinning pillars */
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[361])
                   /* dSv_event_flag_c::F_0266 - Snowpeak Ruins - Snowpeak Ruins clear */
                && dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[266])
                    /* dSv_event_flag_c::F_0267 - Temple of Time - Temple of Time clear */
                && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[267]))
            {
                mpModel->getModelData()->getMaterialNodePointer(1)->getShape()->hide();
            }
                /* dSv_event_flag_c::F_0361 - Arbiter's Grounds - Spun the spinning pillars */
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[361])
                   /* dSv_event_flag_c::F_0266 - Snowpeak Ruins - Snowpeak Ruins clear */
                && dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[266])
                   /* dSv_event_flag_c::F_0267 - Temple of Time - Temple of Time clear */
                && dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[267])
                    /* dSv_event_flag_c::F_0268 - City in the Sky - City in the Sky clear */
                && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[268]))
            {
                mpModel->getModelData()->getMaterialNodePointer(2)->getShape()->hide();
            }
        } else {
            mpModel->getModelData()->getMaterialNodePointer(1)->getShape()->hide();
        }
    }

        /* dSv_event_flag_c::M_070 - Cutscene - [cutscene: 18] Lanayru spirit restored */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[103])
              /* dSv_event_flag_c::F_0268 - City in the Sky - City in the Sky clear */
        && !(!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[268])
                /* dSv_event_flag_c::F_0264 - Cutscene - Get master sword */
             && dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[264])))
    {
        mpModel->getModelData()->getMaterialNodePointer(4)->getShape()->hide();
        attention_info.flags = 0;
    } else {
        attention_info.flags = (fopAc_AttnFlag_TALKCHECK_e | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);

        attention_info.distances[fopAc_attn_LOCK_e] = dKy_darkworld_check() ? 218 : 98;
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    }

    attention_info.position.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);

    mMsgFlow.init(this, getMessageNo(), 0, NULL);
    dMsgObject_endFlowGroup();

    initBaseMtx();
    mEventID = -1;

    return cPhs_COMPLEATE_e;
}

int daObjTable_c::Execute(Mtx** i_mtx) {
    eventInfo.onCondition(1);

    if (dComIfGp_event_runCheck()) {
        if (eventInfo.checkCommandTalk()) {
            if (mMsgFlow.doFlow(this, NULL, 0) != 0) {
                dComIfGp_getEvent()->reset(this);
                eventInfo.setArchiveName("Table");
                dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
                mEventID = dComIfGp_getEventManager().getEventIdx(this, "TABLE_MAP", -1);

                int r27 = fopAcM_orderChangeEventId(this, mEventID, 1, -1);
            }
        } else {
            dEvent_manager_c& evtMgr = dComIfGp_getEventManager();
            int staffId = evtMgr.getMyStaffId(l_arcName, NULL, 0);
            if (staffId != -1 &&
                eventInfo.checkCommandDemoAccrpt() && mEventID != -1 &&
                evtMgr.endCheck(mEventID))
            {
                mEventID = -1;
                dMeter2Info_setPauseStatus(7);
            }
        }
    } else {
        mMsgFlow.init(this, getMessageNo(), 0, NULL);
        dMsgObject_endFlowGroup();
    }

    *i_mtx = &mBgMtx;
    return 1;
}

int daObjTable_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    cXyz v(current.pos.x, current.pos.y + 100.0f, current.pos.z);
    mShadowKey =
        dComIfGd_setRealShadow(mShadowKey, 1, mpModel, &v, 500.0f, current.pos.y, &tevStr);

    return 1;
}

int daObjTable_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return 1;
}

static actor_method_class l_daObjTable_Method = {
    (process_method_func)daObjTable_Create,  (process_method_func)daObjTable_Delete,
    (process_method_func)daObjTable_Execute, (process_method_func)daObjTable_IsDelete,
    (process_method_func)daObjTable_Draw,
};

actor_process_profile_definition g_profile_Obj_Table = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Table,
    &g_fpcLf_Method.base,
    sizeof(daObjTable_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x02AB,
    &l_daObjTable_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
