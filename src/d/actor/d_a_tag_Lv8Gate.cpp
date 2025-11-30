#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_Lv8Gate.h"

#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/actor/d_a_obj_mirror_table.h"

static char* l_arcName = "Lv8Gate";

static int createSolidHeap(fopAc_ac_c* i_this) {
    return static_cast<daTagLv8Gate_c*>(i_this)->createHeap();
}

daTagLv8Gate_c::~daTagLv8Gate_c() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
}

inline void daTagLv8Gate_c::initBaseMtx() {
    if (mpModel != NULL) {
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y + 0x8000);
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

inline void daTagLv8Gate_c::create_init() {
    fopAcM_setCullSizeBox(this, -100.0f, -50.0f, -100.0f, 100.0f, 220.0f, 100.0f);
    fopAcM_OnCarryType(this, fopAcM_CARRY_LIGHT);

    attention_info.flags = fopAc_AttnFlag_CARRY_e;
    attention_info.distances[fopAc_attn_CARRY_e] = 90;

    mEventID = -1;

    initBaseMtx();
}

inline int daTagLv8Gate_c::create() {
    fopAcM_ct(this, daTagLv8Gate_c);

    cPhs__Step step;
         /* dSv_event_flag_c::F_0354 - Cutscene - [cutscene] Mirror complete */
    if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[354])) {
        step = cPhs_ERROR_e;
    } else {
        step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, l_arcName);
        if (step == cPhs_COMPLEATE_e) {
            if (!fopAcM_entrySolidHeap(this, createSolidHeap, 0x1600)) {
                step = cPhs_ERROR_e;
            } else {
                create_init();
            }
        }
    }

    return step;
}

inline int daTagLv8Gate_c::draw() {
    if (mpModel != NULL) {
        for (u16 index = 0; index < 3; index++) {
            J3DMaterial* material = mpModel->getModelData()->getMaterialNodePointer(index);
            material->getTevKColor(1)->r = 128;
            material->getTevKColor(1)->g = 120;
            material->getTevKColor(1)->b = 100;
        }

        g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

        if (mpBck != NULL) {
            mpBck->entry(mpModel->getModelData());
        }

        dComIfGd_setListBG();
        mDoExt_modelUpdateDL(mpModel);
        dComIfGd_setList();

        if (mpBck != NULL) {
            mpModel->getModelData()->getJointNodePointer(0)->setMtxCalc(NULL);
        }
    }
    return TRUE;
}

int daTagLv8Gate_c::createHeap() {
    if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0) {
        J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 8);
        mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);

        J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 5);
        mpBck = new mDoExt_bckAnm();
        if (mpBck == NULL ||
            !mpBck->init(bck, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false))
        {
            return 0;
        }
    }

    return 1;
}

static int daTagLv8Gate_Create(fopAc_ac_c* i_this) {
    daTagLv8Gate_c* a_this = static_cast<daTagLv8Gate_c*>(i_this);
    return a_this->create();
}

static int daTagLv8Gate_Execute(daTagLv8Gate_c* i_this) {
    return i_this->execute();
}

inline int daTagLv8Gate_c::execute() {
    #if VERSION != VERSION_SHIELD_DEBUG
    // TODO: gameInfo fake match to force reuse of pointer
    dComIfG_play_c* play = &g_dComIfG_gameInfo.play;
    if (play->getEvent().runCheck() && !eventInfo.checkCommandTalk()) {
    #else
    if (dComIfGp_event_runCheck() && !eventInfo.checkCommandTalk()) {
    #endif
        dEvent_manager_c& eventManager = dComIfGp_getEventManager();
        s32 cut_index = eventManager.getMyStaffId(l_arcName, NULL, 0);

        if (cut_index != -1) {
            int* cut_name = (int*)eventManager.getMyNowCutName(cut_index);

            if (eventManager.getIsAddvance(cut_index)) {
                switch (*cut_name) {
                case '0001': {
                    dComIfGp_getEvent().startCheckSkipEdge(this);
                    daPy_getPlayerActorClass()->setPlayerPosAndAngle(&current.pos, shape_angle.y,
                                                                     0);

                    fopAc_ac_c* mirror_table = fopAcM_SearchByName(PROC_Obj_MirrorTable);
                    if (mirror_table != NULL) {
                        static_cast<daObjMirrorTable_c*>(mirror_table)->setEffect();
                    }
                    break;
                }
                case '0002':
                    dStage_changeScene(getSceneNo(), 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
                    break;

                default:
                    JUT_ASSERT(139, FALSE);
                }
            }

            #if VERSION != VERSION_SHIELD_DEBUG
            dEvt_control_c& eventControl = play->getEvent();
            #endif
            if (dComIfGp_getEvent().checkSkipEdge()) {
                #if VERSION != VERSION_SHIELD_DEBUG
                dComIfGp_getEvent().reset();
                #else
                dComIfGp_event_reset();
                #endif
                dStage_changeScene(getSceneNo(), 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
            }

            switch (*cut_name) {
            case '0001':
            case '0002':
                eventManager.cutEnd(cut_index);
                break;
            default:
                JUT_ASSERT(154, FALSE);
                break;
            }

            if (eventInfo.checkCommandDemoAccrpt() && mEventID != -1) {
                if (eventManager.endCheck(mEventID)) {
                    mEventID = -1;
                }
            }
        }
    } else {
        if (daPy_getPlayerActorClass()->checkPriActorOwn(this)) {
            JUT_ASSERT(169, NULL != dComIfGp_getAttention());
            for (int i = 0; i < dComIfGp_getAttention()->GetActionCount(); i++) {
                if (dComIfGp_getAttention()->ActionTarget(i) == this) {
                    if (dComIfGp_getAttention()->getActionBtnB() != NULL &&
                        dComIfGp_getAttention()->getActionBtnB()->mType == fopAc_attn_CARRY_e)
                    {
                        dComIfGp_setDoStatusForce(7, 0);
                    }
                }
            }
        }

        if (fopAcM_checkCarryNow(this)) {
            fopAcM_cancelCarryNow(this);
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            eventInfo.setArchiveName(l_arcName);
            dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
            mEventID = dComIfGp_getEventManager().getEventIdx(this, "LV8_GATE_ENTRY", -1);
            fopAcM_orderOtherEventId(this, mEventID, -1, -1, 0, 1);
        }
    }

    if (mpBck != NULL) {
        mpBck->play();
    }

    cXyz pos(0.0f, 400.0f, 1400.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&pos, &pos);

    Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_MONOLIS_HOLE, &pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);

    return 1;
}

static int daTagLv8Gate_Draw(daTagLv8Gate_c* i_this) {
    return i_this->draw();
}

static int daTagLv8Gate_IsDelete(daTagLv8Gate_c* i_this) {
    return TRUE;
}

static int daTagLv8Gate_Delete(daTagLv8Gate_c* i_this) {
    i_this->~daTagLv8Gate_c();
    return TRUE;
}

static actor_method_class l_daTagLv8Gate_Method = {
    (process_method_func)daTagLv8Gate_Create,  (process_method_func)daTagLv8Gate_Delete,
    (process_method_func)daTagLv8Gate_Execute, (process_method_func)daTagLv8Gate_IsDelete,
    (process_method_func)daTagLv8Gate_Draw,
};

extern actor_process_profile_definition g_profile_Tag_Lv8Gate = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Tag_Lv8Gate,        // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daTagLv8Gate_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    0x108,                   // mPriority
    &l_daTagLv8Gate_Method,  // sub_method
    0x40000,                 // mStatus
    0,                       // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
