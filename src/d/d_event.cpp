#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_event.h"
#include "d/actor/d_a_alink.h"
#include "d/d_demo.h"
#include "d/d_msg_object.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "d/actor/d_a_midna.h"
#include "d/actor/d_a_tag_mhint.h"
#include "d/actor/d_a_tag_mstop.h"
#include "d/d_event_debug.h"
#include "SSystem/SComponent/c_counter.h"

namespace {
static u8 event_debug_evnt() {
#if DEBUG
    dEvM_HIO_c& hio = dComIfGp_getEventManager().getEventHIO();
    return hio.m_eve_debug;
#else
    return false;
#endif
}

static void clear_tmpflag_for_message() {
    dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[11]);
    dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[12]);
    dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[13]);
    dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[14]);
    dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[15]);
    dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[51]);
    dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[52]);
    dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[53]);
    dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[54]);
    dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[55]);
}
};  // namespace

dEvt_control_c::dEvt_control_c() {
    remove();
}

dEvt_order_c::dEvt_order_c() {}

s32 dEvt_control_c::orderOld(u16 type, u16 prio, u16 flag, u16 hind, void* requestActor,
                             void* targetActor, const void* eventName) {
    #if DEBUG
    if (event_debug_evnt()) {
        if (type == dEvt_type_OTHER_e && eventName != NULL) {
            // "requested event %s!!"
            OS_REPORT("要求イベント %s!!", eventName);
        }
        OS_REPORT("\n");
    }
    #endif

    return order(type, prio, flag, hind, requestActor, targetActor, (s16)dComIfGp_getEventManager().getEventIdx((char*)eventName, -1, -1), 0xFF);
}

s32 dEvt_control_c::order(u16 type, u16 prio, u16 flag, u16 hind, void* requestActor,
                          void* targetActor, s16 id, u8 mapToolId) {
    if (!(flag & 0x400) && mapToolId != 0xFF) {
        int roomNo = dComIfGp_roomControl_getStayNo();
        dStage_MapEvent_dt_c* data = searchMapEventData(mapToolId, roomNo);

        if (data != NULL && data->switch_no != 0xFF) {
            if (dComIfGs_isSwitch(data->switch_no, roomNo)) {
                return 0;
            }
        }
    }

    #if DEBUG
    if (event_debug_evnt()) {
        OS_REPORT("%06d: event: type = %d !!", g_Counter.mCounter0, type);

        if (requestActor != NULL) {
            // "Request actor %s!!"
            OS_REPORT("要求者 %s!!", dStage_getName(fopAcM_GetProfName(requestActor), -1));
        }

        if (targetActor != NULL) {
            // "Target actor %s!!"
            OS_REPORT("対象者 %s!!", dStage_getName(fopAcM_GetProfName(targetActor), -1));
        }

        // "Request id = %d prio = %d "
        OS_REPORT(" 要求 id = %d prio = %d ", id, prio);
        OS_REPORT("\n");
    }
    #endif

    if (mNum >= ARRAY_SIZE(mOrder)) {
        // "%06d: event: order failed"
        OS_REPORT("\x1b[34m%06d: event: 注文失敗\n\x1b[m", g_Counter.mCounter0);
        return 0;
    }

    dEvt_order_c* new_order = &mOrder[mNum];
    new_order->mEventType = type;
    new_order->mPriority = prio;
    new_order->mFlag = flag;
    new_order->mpRequestActor = (fopAc_ac_c*)requestActor;
    new_order->mpTargetActor = (fopAc_ac_c*)targetActor;
    new_order->mEventId = id;
    new_order->mHindFlag = hind;
    new_order->mMapToolId = mapToolId;

    if (new_order->mPriority == 0) {
        new_order->mPriority = 1;
    }

    if (mNum == 0) {
        mOrderIdx = 0;
        new_order->mNextOrderIdx = -1;
    } else {
        dEvt_order_c* curOrder;
        int curOrderIdx = mOrderIdx;
        curOrder = &mOrder[curOrderIdx];

        if (new_order->mPriority < curOrder->mPriority) {
            mOrderIdx = mNum;
            new_order->mNextOrderIdx = curOrderIdx;
        } else {
            while (curOrder->mNextOrderIdx >= 0) {
                dEvt_order_c* next_order = &mOrder[curOrder->mNextOrderIdx];
                if (new_order->mPriority < next_order->mPriority) {
                    break;
                }

                int nextOrderIdx = curOrder->mNextOrderIdx;
                curOrder = &mOrder[nextOrderIdx];
            }

            new_order->mNextOrderIdx = curOrder->mNextOrderIdx;
            curOrder->mNextOrderIdx = mNum;
        }
    }

    mNum++;

    #if DEBUG
    if (event_debug_evnt()) {
        // "%06d: event: order success  mNum = %d"
        OS_REPORT("\x1B[34m%06d: event: 注文成功　mNum = %d\n\x1B[m", g_Counter.mCounter0, mNum);
    }
    #endif

    return 1;
}

void dEvt_control_c::setParam(dEvt_order_c* order) {
    setPt1(order->mpRequestActor);
    setPt2(order->mpTargetActor);

    mEventId = order->mEventId;
    mHindFlag = order->mHindFlag;

    if (dComIfGp_getPlayer(0) != order->mpRequestActor) {
        setPtT(order->mpRequestActor);
        setPtI(order->mpRequestActor);
    } else {
        setPtT(order->mpTargetActor);
        setPtI(order->mpTargetActor);
    }

    mMapToolId = order->mMapToolId;
    field_0xea = 0xFF;
    field_0xec = 0xFF;

    int roomNo = dComIfGp_roomControl_getStayNo();
    dStage_MapEvent_dt_c* mapdata = searchMapEventData(mMapToolId, roomNo);
    mStageEventDt = mapdata;

    if (mapdata != NULL) {
        field_0xea = mapdata->data.maptool.field_0x16;

        if (mapdata->switch_no != 0xFF) {
            dComIfGs_onSwitch(mapdata->switch_no, roomNo);
        }

        if (order->mFlag & 0x300) {
            field_0xec = mapdata->field_0x5;
        }
    }

    if (!(order->mFlag & 0x200)) {
        mUnkEventId = order->mEventId;
    }

    mCullRate = 1.0f;
    mEventFlag = 0;
}

s32 dEvt_control_c::beforeFlagProc(dEvt_order_c* order) {
    fopAc_ac_c* actor = order->mpTargetActor;

    if ((order->mFlag & 4) && !actor->eventInfo.chkCondition(dEvtCnd_CANTALK_e)) {
        return 0;
    }

    return 1;
}

static char const* dummy1() {
    return "？？？";
}

void dEvt_control_c::afterFlagProc(dEvt_order_c* order) {
    if (order->mFlag & 2) {
        dComIfGp_getEventManager().issueStaff("ALL");
    }
}

int dEvt_control_c::commonCheck(dEvt_order_c* order, u16 condition, u16 command) {
    fopAc_ac_c* actor1 = order->mpRequestActor;
    fopAc_ac_c* actor2 = order->mpTargetActor;

    if ((actor1 != NULL && actor1->eventInfo.chkCondition(condition)) &&
        (actor2 != NULL && actor2->eventInfo.chkCondition(condition))) {
        actor1->eventInfo.setCommand(command);
        actor2->eventInfo.setCommand(command);
        setParam(order);
        return 1;
    }

    return 0;
}

int dEvt_control_c::talkCheck(dEvt_order_c* order) {
    char* eventname = "DEFAULT_TALK";
    fopAc_ac_c* actor = order->mpTargetActor;
    if ((fopAcM_GetName(actor) == PROC_Tag_Mhint && ((daTagMhint_c*)actor)->checkNoAttention()) ||
        (fopAcM_GetName(actor) == PROC_Tag_Mstop && ((daTagMstop_c*)actor)->checkNoAttention()) ||
        fopAcM_GetName(actor) == PROC_MIDNA)
    {
        daMidna_c* midna = (daMidna_c*)daPy_py_c::getMidnaActor();
        if (!daPy_py_c::checkNowWolf() || midna->checkNoDraw()) {
            eventname = "MHINT_TALK";
        }
    }

    if (commonCheck(order, dEvtCnd_CANTALK_e, dEvtCmd_INTALK_e)) {
        #if DEBUG
        if (event_debug_evnt()) {
            // "conversation  actor1 = %x  actor2 = %x"
            OS_REPORT("会話へ actor1 = %x  actor2 = %x\n", order->mpRequestActor, order->mpTargetActor);
            // "conversation partner %s"
            OS_REPORT("会話相手 %s\n", dStage_getName(fopAcM_GetProfName(actor), -1));
        }
        #endif

        mMode = dEvt_mode_TALK_e;

        if (mEventId == -1) {
            if (actor != NULL && actor->eventInfo.getEventName() != NULL) {
                mEventId = actor->eventInfo.getEventId();
            } else {
                mEventId = dComIfGp_getEventManager().getEventIdx(eventname, 0xFF, -1);
            }
        }

        if (!dComIfGp_getEventManager().order(mEventId)) {
            // "specified event %d not ordered!!"
            OS_REPORT("指定イベント %d が無い!!\n", mEventId);
            JUT_ASSERT(429, FALSE);
        }

        return 1;
    }

    return 0;
}

int dEvt_control_c::talkXyCheck(dEvt_order_c* order) {
    const char* eventname = "DEFAULT_TALK_XY";
    fopAc_ac_c* targetActor = order->mpTargetActor;

    int itemIndex;
    switch (order->mEventType) {
    case dEvt_type_SHOWITEM_X_e:
        itemIndex = SELECT_ITEM_X;
        mTalkXyType = 1;
        break;
    case dEvt_type_SHOWITEM_Y_e:
        mTalkXyType = 2;
        itemIndex = SELECT_ITEM_Y;
        break;
    #if PLATFORM_WII || PLATFORM_SHIELD
    case 8:
        mTalkXyType = 3;
        itemIndex = 2;
        break;
    case 9:
        mTalkXyType = 4;
        itemIndex = 3;
        break;
    #endif
    default:
        OS_REPORT_ERROR("event XY talk unknown type %d\n", order->mEventType);
        mTalkXyType = 0;
        return 0;
    }

    if (dComIfGp_getSelectItem(itemIndex) == fpcNm_ITEM_NONE) {
        return 0;
    }

    if (targetActor == NULL || !targetActor->eventInfo.chkCondition(dEvtCnd_CANTALKITEM_e)) {
        return 0;
    }

    if (commonCheck(order, dEvtCnd_CANTALK_e, dEvtCmd_INTALK_e)) {
        #if DEBUG
        if (event_debug_evnt()) {
            // "conversation  actor1 = %x  actor2 = %x"
            OS_REPORT("会話へ actor1 = %x  actor2 = %x\n", order->mpRequestActor, order->mpTargetActor);
            // "conversation partner"
            OS_REPORT("会話相手 %s\n", dStage_getName(fopAcM_GetProfName(targetActor), -1));
        }
        #endif

        mMode = dEvt_mode_TALK_e;
        mPreItemNo = dComIfGp_getSelectItem(itemIndex);
        mEventId = dComIfGp_getEventManager().getEventIdx(eventname, 0xFF, -1);
        
        if (!dComIfGp_getEventManager().order(mEventId)) {
            // "specified event %d not ordered!!"
            OS_REPORT("指定イベント %d が無い!!\n", mEventId);
            JUT_ASSERT(512, FALSE);
        }
        return 1;
    }

    return 0;
}

int dEvt_control_c::catchCheck(dEvt_order_c* order) {
    fopAc_ac_c* actor2 = order->mpTargetActor;
    fopAc_ac_c* actor1 = order->mpRequestActor;

    if (actor1 == NULL || (actor2 != NULL && !actor2->eventInfo.chkCondition(dEvtCnd_40_e))) {
        return 0;
    }

    actor1->eventInfo.setCommand(dEvtCmd_INCATCH_e);
    if (actor2 != NULL) {
        actor2->eventInfo.setCommand(dEvtCmd_INCATCH_e);
    }

    setParam(order);
    mPreItemNo = dComIfGp_att_getCatchChgItem();
    mMode = dEvt_mode_DEMO_e;

    if (mEventId != -1) {
        if (!dComIfGp_getEventManager().order(mEventId)) {
            // "specified event %d not ordered!!"
            OS_REPORT("指定イベント %d が無い!!\n", mEventId);
            JUT_ASSERT(551, FALSE);
        }
    }

    return 1;
}

int dEvt_control_c::talkEnd() {
    fopAc_ac_c* actor = getPt1();
    if (actor != NULL) {
        actor->eventInfo.setCommand(dEvtCmd_NONE_e);
    }

    actor = getPt2();
    if (actor != NULL) {
        actor->eventInfo.setCommand(dEvtCmd_NONE_e);
    }

    if (mEventId != -1) {
        dComIfGp_getEventManager().endProc(mEventId, TRUE);
        mEventId = -1;
    }

    daItemBase_c* item = (daItemBase_c*)fopAcM_getItemEventPartner(NULL);
    if (item != NULL && fopAcM_GetName(item) == PROC_ITEM) {
        item->dead();
    }

    return 1;
}

int dEvt_control_c::demoCheck(dEvt_order_c* order) {
    fopAc_ac_c* actor1 = order->mpRequestActor;
    fopAc_ac_c* actor2 = order->mpTargetActor;
    int eventId = order->mEventId;

    if (actor2 == NULL) {
        JUT_ASSERT(608, FALSE);
        return 0;
    }

    if (!(order->mFlag & 0x10)) {
        if (actor1 != NULL && !actor1->eventInfo.chkCondition(dEvtCnd_CANDEMO_e)) {
            return 0;
        }

        if (actor2 != NULL && !actor2->eventInfo.chkCondition(dEvtCnd_CANDEMO_e)) {
            return 0;
        }
    }

    if (!beforeFlagProc(order)) {
        return 0;
    }

    if (actor1 != NULL) {
        dComIfGp_getEventManager().setObjectArchive(actor1->eventInfo.mArchiveName);
    }

    if (!dComIfGp_getEventManager().order(eventId)) {
        return 0;
    }

    if (actor1 != NULL) {
        actor1->eventInfo.setCommand(dEvtCmd_INDEMO_e);
    }

    if (actor2 != NULL) {
        actor2->eventInfo.setCommand(dEvtCmd_INDEMO_e);
    }

    mMode = dEvt_mode_DEMO_e;
    setParam(order);
    afterFlagProc(order);
    return 1;
}

int dEvt_control_c::demoEnd() {
    fopAc_ac_c* actor = getPt1();
    if (actor != NULL) {
        actor->eventInfo.setCommand(dEvtCmd_NONE_e);
    }

    actor = getPt2();
    if (actor != NULL) {
        actor->eventInfo.setCommand(dEvtCmd_NONE_e);
    }

    if (mEventId != -1) {
        dComIfGp_getEventManager().endProc(mEventId, TRUE);
        mEventId = -1;
    }

    return 1;
}

int dEvt_control_c::potentialCheck(dEvt_order_c* order) {
    fopAc_ac_c* actor1 = order->mpRequestActor;
    fopAc_ac_c* actor2 = order->mpTargetActor;
    const char* actorname;

    #if DEBUG
    if (actor1 == NULL || actor2 == NULL) {
        JUT_ASSERT(704, FALSE);
    }
    #endif

    if (!beforeFlagProc(order)) {
        return 0;
    }

    actor1->eventInfo.setCommand(dEvtCmd_INDEMO_e);
    mMode = dEvt_mode_DEMO_e;
    setParam(order);
    afterFlagProc(order);

    #if DEBUG
    if (event_debug_evnt()) {
        if (actor1 != NULL) {
            actorname = dStage_getName(fopAcM_GetProfName(actor1), -1);
        } else {
            actorname = "???";
        }

        // "%06d: event: potential demo start (%s) "
        OS_REPORT("\x1b[34m%06d: event: 仮想デモ開始 (%s) \n\x1b[m", g_Counter.mCounter0, actorname);
    }
    #endif
    return 1;
}

int dEvt_control_c::doorCheck(dEvt_order_c* order) {
    // "door demo check start!!"
    OS_REPORT("ドアデモのチェック開始!!\n");

    if (commonCheck(order, dEvtCnd_CANDOOR_e, dEvtCmd_INDOOR_e)) {
        #if DEBUG
        if (event_debug_evnt()) {
            // "door demo  actor1 = %x  actor2 = %x"
            OS_REPORT("ドアデモへ actor1 = %x  actor2 = %x\n", order->mpRequestActor, order->mpTargetActor);
        }
        #endif

        mMode = dEvt_mode_DEMO_e;

        fopAc_ac_c* actor2 = getPt2();
        if (actor2 != NULL) {
            dComIfGp_getEventManager().setObjectArchive(actor2->eventInfo.mArchiveName);
        }

        if (mEventId == -1 && actor2 != NULL && actor2->eventInfo.getEventId() != -1) {
            mEventId = actor2->eventInfo.getEventId();
        }

        if (mEventId != -1 && dComIfGp_getEventManager().getEventData(mEventId) != NULL) {
            if (!dComIfGp_getEventManager().order(mEventId)) {
                JUT_ASSERT(773, FALSE);
            }
        } else {
            mEventId = -1;
            reset();
        }

        if (order->mEventType == dEvt_type_TREASURE_e) {
            onEventFlag(4);
        } else {
            onEventFlag(0x40);
        }

        return 1;
    }

    return 0;
}

int dEvt_control_c::itemCheck(dEvt_order_c* order) {
    const char* eventname = "DEFAULT_GETITEM";

    if (commonCheck(order, dEvtCnd_CANGETITEM_e, dEvtCmd_INGETITEM_e)) {
        #if DEBUG
        if (event_debug_evnt()) {
            OS_REPORT("アイテムデモへ actor1 = %x  actor2 = %x\n", order->mpRequestActor, order->mpTargetActor);
        }
        #endif

        mMode = dEvt_mode_DEMO_e;
        mEventId = dComIfGp_getEventManager().getEventIdx(eventname, 0xFF, -1);
        if (!dComIfGp_getEventManager().order(mEventId)) {
            // "specified event %d not ordered!!"
            OS_REPORT("指定イベント %s が無い!!\n", eventname);
            JUT_ASSERT(817, FALSE);
        }
        return 1;
    }

    return 0;
}

int dEvt_control_c::endProc() {
    #if DEBUG
    if (event_debug_evnt()) {
        OS_REPORT("\x1b[34m%06d: event: イベントが終了しました mMode = %d \n\x1b[m", g_Counter.mCounter0, mMode);
    }
    #endif

    switch (mMode) {
    case dEvt_mode_TALK_e:
        talkEnd();
        break;
    case dEvt_mode_DEMO_e:
        demoEnd();
        break;
    case dEvt_mode_COMPULSORY_e:
        JUT_ASSERT(858, FALSE);
    }

    mMode = dEvt_mode_WAIT_e;
    field_0xea = 0xFF;
    mMapToolId = 0xFF;
    mStageEventDt = NULL;
    field_0xec = 0xFF;
    mTalkXyType = 0;
    mUnkEventId = 255;
    mPreItemNo = fpcNm_ITEM_NONE;
    dComIfGp_getEventManager().setStartDemo(-2);
    return 1;
}

int dEvt_control_c::change() {
    dEvt_order_c* order;

    if (mNum != 0) {
        s8 curOrderIdx = mOrderIdx;

        do {
            order = &mOrder[curOrderIdx];

            if ((order->mFlag & 0xE00) && order->mpRequestActor == mChangeActor) {
                mOrderIdx = curOrderIdx;
                mNum = 1;
                order->mNextOrderIdx = -1;
                return curOrderIdx;
            }

            curOrderIdx = order->mNextOrderIdx;
        } while (curOrderIdx >= 0);
    }

    return -1;
}

int dEvt_control_c::entry() {
    if (mNum != 0) {
        // "%06d: event: %d event requests!!"
        OS_REPORT("\x1b[34m%06d: event: %d個のイベント要求!!\n\x1b[m", g_Counter.mCounter0, mNum);

        s8 orderIdx = mOrderIdx;
        mNum = 0;

        do {
            dEvt_order_c* order = &mOrder[orderIdx];
            orderIdx = order->mNextOrderIdx;

            switch (order->mEventType) {
            case dEvt_type_TALK_e:
                if (dComIfGs_getLife() != 0 && talkCheck(order)) {
                    return 1;
                }
                break;
            case dEvt_type_OTHER_e:
                if (dComIfGs_getLife() != 0 && demoCheck(order)) {
                    return 1;
                }
                break;
            case dEvt_type_DOOR_e:
            case dEvt_type_TREASURE_e:
                if (dComIfGs_getLife() != 0 && doorCheck(order)) {
                    return 1;
                }
                break;
            case dEvt_type_COMPULSORY_e:
                mMode = dEvt_mode_COMPULSORY_e;

                #if DEBUG
                if (event_debug_evnt()) {
                    // "%06d: event: player initiated demo start"
                    OS_REPORT("\x1b[34m%06d: event: プレーヤ主導デモ開始\n\x1b[m", g_Counter.mCounter0);
                }
                #endif

                setParam(order);
                return 1;
            case dEvt_type_POTENTIAL_e:
                if (dComIfGs_getLife() != 0 && potentialCheck(order)) {
                    return 1;
                }
                break;
            case dEvt_type_ITEM_e:
                if (dComIfGs_getLife() != 0 && itemCheck(order)) {
                    return 1;
                }
                break;
            case dEvt_type_SHOWITEM_X_e:
            case dEvt_type_SHOWITEM_Y_e:
                if (dComIfGs_getLife() != 0 && talkXyCheck(order)) {
                    return 1;
                }
                break;
            case dEvt_type_CATCH_e:
                if (dComIfGs_getLife() != 0 && catchCheck(order)) {
                    return 1;
                }
                break;
            case 12:
                JUT_ASSERT(980, FALSE);
                break;
            case 13:
                break;
            }
        } while (orderIdx >= 0);

        #if DEBUG
        if (event_debug_evnt()) {
            // "%06d: event: check failed or change"
            OS_REPORT("\x1b[34m%06d: event: チェック失敗 or チェンジ\n\x1b[m", g_Counter.mCounter0);
        }
        #endif
    }

    return 0;
}

void dEvt_control_c::reset() {
    if (field_0xec != 0xFF) {
        fopAc_ac_c* pt1 = getPt1();
        fopAc_ac_c* var_r27 = pt1;
        s16 eventIdx = dComIfGp_getEventManager().getEventIdx(NULL, field_0xec, -1);

        order(dEvt_type_OTHER_e, 3, 0x201, mHindFlag, pt1, getPt2(), eventIdx, field_0xec);
        mChangeActor = pt1;
    }

    onEventFlag(8);
}

void dEvt_control_c::reset(void* param_0) {
    if (field_0xec != 0xFF) {
        fopAc_ac_c* var_r29 = (fopAc_ac_c*)param_0;
        s16 eventIdx = dComIfGp_getEventManager().getEventIdx(var_r29, field_0xec);
        order(dEvt_type_OTHER_e, 3, 0x201, mHindFlag, param_0, getPt2(), eventIdx, field_0xec);
    }

    mChangeActor = param_0;
    onEventFlag(8);
}

void dEvt_control_c::clearSkipSystem() {
    mSkipFunc = NULL;
    mSkipActorId = fpcM_ERROR_PROCESS_ID_e;
    mSkipParameter = 0;
    mSkipTimer = 0;

    if (!chkFlag2(3)) {
        mIsSkipFade = FALSE;
    }

    mSkipEventName[0] = 0;
}

int dEv_defaultSkipProc(void* actor, int parameter) {
    // "%06d: event: skip event!!!"
    OS_REPORT("\x1b[34m%06d: event: スキップ event!!!\n\x1b[m", g_Counter.mCounter0);
    dEvt_control_c* event = dComIfGp_getEvent();

    event->reset(actor);
    return 1;
}

int dEv_defaultSkipZev(void* actor, int parameter) {
    // "%06d: event: skip ZEV event!!!"
    OS_REPORT("\x1b[34m%06d: event: スキップ ZEV event!!!\n\x1b[m", g_Counter.mCounter0);
    dEvt_control_c* event = dComIfGp_getEvent();

    s16 eventID = -1;
    dStage_MapEvent_dt_c* data = event->getStageEventDt();

    bool var_r27 = true;
    if (data != NULL) {
        if (data->field_0x9 != 0xFF) {
            var_r27 = false;
        } else if (data->field_0x7 != 0xFF) {
            var_r27 = false;
        }
    }

    char eventName[28];
    char* skipName;
    switch (parameter) {
    case 0:
        strcpy(eventName, data->data.event_name);
        strcat(eventName, "$0");
        eventID = dComIfGp_getEventManager().getEventIdx(eventName, 0xFF, -1);
        OS_REPORT("%06d: event:   [%d] %s!\n", g_Counter.mCounter0, eventID, eventName);
        break;
    case 1:
        skipName = dComIfGp_getEvent()->getSkipEventName();
        eventID = dComIfGp_getEventManager().getEventIdx(skipName, 0xFF, -1);
        OS_REPORT("%06d: event:   [%d] %s!\n", g_Counter.mCounter0, eventID, dComIfGp_getEvent()->getSkipEventName());
        break;
    default:
        OS_REPORT("%06d: event: ERROR: illegal prameter!\n", g_Counter.mCounter0);
    }

    if (eventID != -1) {
        event->reset(actor);
        fopAcM_orderOtherEventId((fopAc_ac_c*)actor, eventID, event->getMapToolId(), -1, 3,
                                 0x400);
        return 2;
    } else if (var_r27) {
        event->reset(actor);
        return 1;
    } else {
        event->reset(actor);
        return 1;
    }
}

int dEv_defaultSkipStb(void* actor, int parameter) {
    OS_REPORT("\x1b[34m%06d: event: スキップ STB event!!!\n\x1b[m", g_Counter.mCounter0);
    dEvt_control_c* event = dComIfGp_getEvent();

    s16 eventID = -1;
    dStage_MapEvent_dt_c* data = event->getStageEventDt();

    bool var_r27 = true;
    if (data != NULL) {
        if (data->field_0x9 != 0xFF) {
            var_r27 = false;
        } else if (data->field_0x7 != 0xFF) {
            var_r27 = false;
        }
    }

    char eventName[28];
    char* skipName;
    switch (parameter) {
    case 0:
        strcpy(eventName, data->data.event_name);
        strcat(eventName, "$0");
        eventID = dComIfGp_getEventManager().getEventIdx(eventName, 0xFF, -1);
        OS_REPORT("%06d: event:   [%d] %s!\n", g_Counter.mCounter0, eventID, eventName);
        break;
    case 1:
        skipName = dComIfGp_getEvent()->getSkipEventName();
        eventID = dComIfGp_getEventManager().getEventIdx(skipName, 0xFF, -1);
        OS_REPORT("%06d: event:   [%d] %s!\n", g_Counter.mCounter0, eventID, dComIfGp_getEvent()->getSkipEventName());
        break;
    default:
        OS_REPORT("%06d: event: ERROR: illegal prameter!\n", g_Counter.mCounter0);
    }

    if (eventID != -1) {
        event->reset(actor);
        dDemo_c::end();
        fopAcM_orderOtherEventId((fopAc_ac_c*)actor, eventID, event->getMapToolId(), -1, 3,
                                 0x400);
        return 2;
    } else if (var_r27) {
        event->reset(actor);
        dDemo_c::end();
        return 1;
    } else {
        event->reset(actor);
        return 1;
    }
}

int dEv_noFinishSkipProc(void* actor, int parameter) {
    dComIfGp_getEvent()->offSkipFade();
    return 0;
}

char* dEvt_control_c::getSkipEventName() {
    return mSkipEventName;
}

void dEvt_control_c::setSkipProc(void* skipActor, dEvt_SkipCb skipCb, int skipParameter) {
    mSkipFunc = skipCb;
    mSkipActorId = getPId(skipActor);
    mSkipParameter = skipParameter;
}

void dEvt_control_c::setSkipZev(void* skipActor, char* eventName) {
    setSkipProc(skipActor, dEv_defaultSkipZev, 1);
    strcpy(mSkipEventName, eventName);
}

void dEvt_control_c::onSkipFade() {
    mIsSkipFade = TRUE;
}

void dEvt_control_c::offSkipFade() {
    if (mIsSkipFade) {
        mIsSkipFade = FALSE;
    }

    mIsSkipFade = FALSE;
}

bool dEvt_control_c::skipper() {
    bool doSkip = false;
    bool canSkip = false;

    offFlag2(8);

    if (mEventStatus == 1) {
        if (mSkipFunc != NULL) {
            canSkip = true;
        }

        bool is_trig_skipbtn = mDoCPd_c::getTrigStart(PAD_1);
        if (is_trig_skipbtn) {
            if (mSkipTimer > 0) {
                mSkipTimer = -1;

                if (canSkip && mIsSkipFade) {
                    mDoGph_gInf_c::fadeOut(0.1f);
                }
            } else if (mSkipTimer == 0) {
                mSkipTimer = 1;
            }
        }

        if (mSkipTimer > 0) {
            if (canSkip) {
                dComIfGp_setSButtonStatusForce(0x43, 1);
            } else {
                dComIfGp_setSButtonStatusForce(0x4D, 1);
            }

            if (mSkipTimer++ > 45) {
                mSkipTimer = 0;
            }
        } else if (mSkipTimer != 0) {
            if (canSkip && mIsSkipFade) {
                if (mSkipTimer-- < -20) {
                    doSkip = true;
                    mSkipTimer = 0;
                }
            } else {
                if (canSkip) {
                    doSkip = true;
                }
                mSkipTimer = 0;
            }
        }

        if (doSkip) {
            dMsgObject_onKillMessageFlag();

            fopAc_ac_c* skipActor = convPId(mSkipActorId);
            if (skipActor == NULL) {
                OS_REPORT("\x1b[31m%06d: event: Skip ordered actor DEAD!! (%d) \n\x1b[m", g_Counter.mCounter0, mSkipActorId);
                skipActor = dComIfGp_getPlayer(0);
            }

            int skipRet = mSkipFunc(skipActor, mSkipParameter);
            onFlag2(8);

            if (skipRet != 0) {
                mSkipFunc = NULL;

                if (skipRet == 2) {
                    onFlag2(1);
                } else {
                    onFlag2(2);
                }
            }
        }
    }

    return doSkip;
}

int dEvt_control_c::Step() {
    dEvent_manager_c* evtMng = &dComIfGp_getEventManager();

    #if DEBUG
    static int prevEvId = -99;
    static char* basename[] = {
        "NULL",
        "KEEP",
        "STAGE",
        "ROOM0",
        "ROOM1",
        "ROOM2",
        "ROOM3",
        "ROOM4",
        "ROOM5",
        "DEMO",
        "DEBUG",
    };

    evtMng->debugBfProc();
    #endif

    field_0xe7 = 0;
    field_0xe8 = mNum;

    skipper();

    if (chkEventFlag(8)) {
        if (mEventStatus == 1) {
            mEventStatus = 5;

            if (dMsgObject_getMsgObjectClass() != NULL) {
                dMsgObject_endFlowGroup();
            }
        }

        dEvDtEvent_c* eventDt = evtMng->getEventData(mEventId);
        if (eventDt != NULL) {
            eventDt->forceFinish();
        }

        offEventFlag(8);
    }

    if (mEventStatus != 0) {
        evtMng->Sequencer();
    }

    offEventFlag(0x300);

    if (mEventStatus == 2) {
        // "%06d: event: end (%d)"
        OS_REPORT("\x1b[36m%06d: event: 終了 (%d)\n\x1b[m", g_Counter.mCounter0, prevEvId);

        evtMng->cancelStaff("ALL");
        evtMng->setCameraPlay(0);
        mEventFlag = 0;
        onEventFlag(0x200);
        mEventStatus = 0;
    } else if (mEventStatus == 0) {
        mEventFlag = 0;
    }

    if (mEventStatus == 5 && !dComIfGp_isEnableNextStage()) {
        #if DEBUG
        prevEvId = mEventId;
        #endif

        if (chkFlag2(2) || mSkipTimer < 0) {
            if (mIsSkipFade) {
                mDoGph_gInf_c::fadeOut(-0.05f);
            }
            offFlag2(2);
            clear_tmpflag_for_message();
        }

        endProc();

        if (!chkFlag2(3)) {
            mIsSkipFade = false;
            evtMng->setObjectArchive(NULL);
        }

        mEventStatus = 2;

        // "%06d: event: order info (%d)"
        OS_REPORT("\x1b[36m%06d: event: オーダー受付 (%d)\n\x1b[m", g_Counter.mCounter0, prevEvId);
    } else if (mEventStatus == 5 && (chkFlag2(2) || mSkipTimer < 0)) {
        clear_tmpflag_for_message();
    }

    if (change() != -1) {
        #if DEBUG
        if (event_debug_evnt()) {
            // "%06d: event: event change"
            OS_REPORT("\x1b[34m%06d: event: イベント変更\n\x1b[m", g_Counter.mCounter0);
        }
        #endif

        evtMng->cancelStaff("ALL");
        evtMng->setCameraPlay(0);
        onEventFlag(0x200);
        mEventStatus = 0;

        if (chkFlag2(1)) {
            offFlag2(1);
            onFlag2(2);
        }
    }

    mCompulsory = 0;

    int roomNo = dComIfGp_roomControl_getStayNo();
    if (mRoomNo != roomNo) {
        mRoomInfoSet = 0;
        mRoomNo = roomNo;
    }

    if (mEventStatus == 0 && entry()) {
        if (dMsgObject_getMsgObjectClass() != NULL) {
            dMsgObject_setKillMessageFlag();
        }

        onEventFlag(0x100);

        dComIfGp_getVibration().StopQuake(31);
        daAlink_c* player = daAlink_getAlinkActorClass();

        if (!dCam_getBody()->Active() && player->checkFishingCastMode()) {
            dCam_getBody()->QuickStart();
        }

        mEventStatus = 1;
        clearSkipSystem();

        #if PLATFORM_SHIELD || PLATFORM_WII
        field_0x130 = 0;
        #endif

        #if DEBUG
        const char* eventname;
        if (mEventId == -1) {
            eventname = "POTENTIAL?";
        } else {
            dEvDtEvent_c* data = evtMng->getEventData(mEventId);
            if (data == NULL) {
                eventname = "UNKOWN";
            } else {
                eventname = data->getName();
            }
        }

        // "%06d: event: start (%d=%s[%d]) %s"
        OS_REPORT("\x1b[36m%06d: event: 開始 (%d=%s[%d]) %s\n\x1b[m", g_Counter.mCounter0, mEventId,
                  mEventId == -1 ? "PROGRAM" : basename[mEventId >> 8], mEventId & 0xFF, eventname);
        prevEvId = -99;
        #endif
    }

    mNum = 0;
    mChangeActor = NULL;

    if (chkEventFlag(0x200)) {
       Z2GetAudioMgr()->setDemoName(NULL);
    }

    if (mEventStatus != 0) {
        evtMng->Experts();

        #if PLATFORM_WII || PLATFORM_SHIELD
        if (field_0x130) {
            mDoGph_gInf_c::onWideZoom();
        }
        #endif
        return 1;
    } else {
        if (!mRoomInfoSet) {
            char lastStageName[8];
            strncpy(lastStageName, dComIfGp_getStartStageName(), sizeof(lastStageName) - 1);
            lastStageName[7] = 0;

            dComIfGp_setLastPlayStageName(lastStageName);
            dComIfGs_onVisitedRoom(mRoomNo);
        }
        mRoomInfoSet = true;
    }

    return 0;
}

int dEvt_control_c::moveApproval(void* param_0) {
    fopAc_ac_c* actor = (fopAc_ac_c*)param_0;
    if (fopAcM_CheckStatus(actor, 0x20000)) {
        return 2;
    }

    if (!runCheck()) {
        return 1;
    }

    switch (mMode) {
    case dEvt_mode_TALK_e:
        if (getPt1() == actor || getPt2() == actor) {
            return 2;
        }
        break;
    case dEvt_mode_DEMO_e:
        if (getPt1() == actor || getPt2() == actor) {
            return 2;
        }

        if (actor->demoActorID != 0) {
            return 2;
        }
        break;
    case dEvt_mode_COMPULSORY_e:
        if (getPt1() == actor) {
            return 2;
        }
        break;
    }

    if (fopAcM_CheckStatus(actor, 0x8000)) {
        return 2;
    }

    if (fopAcM_CheckStatus(actor, 0x800)) {
        return 1;
    }

    if (dComIfGp_checkPlayerStatus0(0, 0x20000000) ||
        dComIfGp_checkPlayerStatus1(0, 0x4002000)) {
        return 0;
    }

    if (mMode == dEvt_mode_TALK_e && fopAcM_CheckStatus(actor, 0x40)) {
        return 1;
    }

    if (dDemo_c::getMode() == 1) {
        return 1;
    }

    if ((getMode() == 3 || getMode() == dEvt_mode_TALK_e) && fopAcM_CheckStatus(actor, 0x4000000)) {
        return 0;
    }

    if (fopAcM_CheckStatus(actor, 0x4000)) {
        return 1;
    }

    if (fopAcM_CheckStatus(actor, 0x2000)) {
        return 1;
    }

    return 0;
}

BOOL dEvt_control_c::compulsory(void* param_0, const char* eventName, u16 hind) {
    if (!isOrderOK() || mCompulsory) {
        return 0;
    }

    mCompulsory = 1;
    return orderOld(dEvt_type_COMPULSORY_e, 2, 0, hind, param_0, NULL, eventName);
}

void dEvt_control_c::remove() {
    mMode = dEvt_mode_WAIT_e;
    mEventStatus = 0;
    mNum = 0;
    mDebugStb = 0;
    field_0xe6 = 0;
    field_0xe7 = 0;
    mCullRate = 0.0f;
    field_0xea = 0xFF;
    mMapToolId = 0xFF;
    field_0xec = 0xFF;
    mStageEventDt = NULL;
    mUnkEventId = 255;
    mTalkXyType = 0;
    mPreItemNo = fpcNm_ITEM_NONE;
    mEventFlag = 0;
    mFlag2 = 0;
    mChangeActor = NULL;
    clearSkipSystem();
    mCompulsory = 0;
    mRoomInfoSet = false;
    mRoomNo = -1;
}

dStage_MapEvent_dt_c* dEvt_control_c::getStageEventDt() {
    return mStageEventDt;
}

void dEvt_control_c::sceneChange(int exitId) {
    dStage_changeScene4Event(exitId, mRoomNo, -1, mRoomInfoSet, 0.0f, 0, 0, -1);
}

fpc_ProcID dEvt_control_c::getPId(void* actor) {
    if (actor == NULL) {
        return fpcM_ERROR_PROCESS_ID_e;
    } else {
        return fopAcM_GetID(actor);
    }
}

fopAc_ac_c* dEvt_control_c::convPId(fpc_ProcID id) {
    return fopAcM_SearchByID(id);
}

void* dEvt_control_c::getStbDemoData(char* resName) {
    void* res = NULL;
    char* demoArcName = dStage_roomControl_c::getDemoArcName();
    
    if (*demoArcName != 0) {
        res = dComIfG_getObjectRes(demoArcName, resName);
        if (res != NULL) {
            if (event_debug_evnt()) {
                OS_REPORT("%06d: event: stb data %s found in %s.arc\n", g_Counter.mCounter0, resName, demoArcName);
            }
            return res;
        }

        if (event_debug_evnt()) {
            OS_REPORT("%06d: event: stb data %s not found in %s.arc\n", g_Counter.mCounter0, resName, demoArcName);
        }
    } else {
        if (event_debug_evnt()) {
            OS_REPORT("%06d: event: stb data %s unkown archive name\n", g_Counter.mCounter0, resName);
        }
    }

    char* roomArcName = dComIfG_getRoomArcName(dComIfGp_roomControl_getStayNo());
    res = dComIfG_getStageRes(roomArcName, resName);
    if (res != NULL) {
        if (event_debug_evnt()) {
            OS_REPORT("%06d: event: stb data %s found in %s.arc\n", g_Counter.mCounter0, resName, roomArcName);
        }
        return res;
    }

    res = dComIfG_getStageRes(resName);
    if (res != NULL) {
        if (event_debug_evnt()) {
            OS_REPORT("%06d: event: stb %s data found in %s.arc\n", g_Counter.mCounter0, resName, "Stage");
        }
        return res;
    } else {
        OS_REPORT("\x1b[43;30m%06d: event: stb data %s NOT found\n\x1b[m", g_Counter.mCounter0, resName);
    }

    return NULL;
}

dEvt_info_c::dEvt_info_c() {
    setCommand(dEvtCmd_NONE_e);
    setCondition(dEvtCnd_CANDEMO_e);
    setEventId(-1);
    setMapToolId(0xFF);
    mArchiveName = NULL;
    mIndex = 0;
}

void dEvt_info_c::setEventName(char* name) {
    if (name == NULL) {
        mEventId = -1;
    } else {
        mEventId = (s16)dComIfGp_getEventManager().getEventIdx(name, 0xFF, -1);
    }
}

char* dEvt_info_c::getEventName() {
    if (mEventId == -1) {
        return NULL;
    }

    dEvDtEvent_c* data = dComIfGp_getEventManager().getEventData(mEventId);
    if (data == NULL) {
        return NULL;
    } else {
        return data->getName();
    }
}

void dEvt_info_c::beforeProc() {
    mCondition = dEvtCnd_CANDEMO_e;
}

dStage_MapEvent_dt_c* dEvt_control_c::searchMapEventData(u8 mapToolID) {
    s32 roomNo = dComIfGp_roomControl_getStayNo();
    return searchMapEventData(mapToolID, roomNo);
}

dStage_MapEvent_dt_c* dEvt_control_c::searchMapEventData(u8 mapToolID, s32 roomNo) {
    int i;
    if (mapToolID == 0xFF) {
        return NULL;
    }

    s32 var_r28 = roomNo;
    dStage_roomDt_c* room_dt = dComIfGp_roomControl_getStatusRoomDt(var_r28);
    dStage_MapEventInfo_c* roomInfo;
    if (room_dt != NULL) {
        roomInfo = room_dt->getMapEventInfo();

        if (roomInfo != NULL) {
            for (i = 0; i < roomInfo->num; i++) {
                if (mapToolID == roomInfo->m_entries[i].field_0x4) {
                    return &roomInfo->m_entries[i];
                }
            }
        }
    }

    roomInfo = dComIfGp_getStage()->getMapEventInfo();
    if (roomInfo != NULL) {
        for (i = 0; i < roomInfo->num; i++) {
            if (mapToolID == roomInfo->m_entries[i].field_0x4) {
                return &roomInfo->m_entries[i];
            }
        }
    }

    return NULL;
}

s16 dEvt_control_c::runningEventID(s16 eventID) {
    if (eventID == mUnkEventId) {
        return mEventId;
    } else {
        return eventID;
    }
}

void dEvt_control_c::setPt1(void* actor) {
    mPt1 = getPId(actor);
}

void dEvt_control_c::setPt2(void* actor) {
    mPt2 = getPId(actor);
}

void dEvt_control_c::setPtT(void* actor) {
    mPtT = getPId(actor);
}

void dEvt_control_c::setPtI(void* actor) {
    mPtI = getPId(actor);
}

void dEvt_control_c::setPtI_Id(fpc_ProcID id) {
    mPtI = id;
}

void dEvt_control_c::setPtD(void* actor) {
    mPtd = getPId(actor);
}

void dEvt_control_c::setGtItm(u8 itemNo) {
    mGtItm = itemNo;
}

static char const* dummy2() {
    return "GIVEMAN";
}
