//
// d_a_tag_TWgate
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTAssert.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_tag_TWgate.h"
#include "d/d_com_inf_game.h"
#include "d/d_debug_viewer.h"
#include "d/d_msg_object.h"
#include "f_op/f_op_actor_mng.h"

struct daTagTWGate_zevParam {
    /* 0x00 */ char* mArcName;
    /* 0x04 */ char* mEventName;
    /* 0x08 */ char* mTalkEventName;
    /* 0x0C */ char* mInEventName;
    /* 0x10 */ int mLv;
    /* 0x14 */ char* mStage;
    /* 0x18 */ s16 mPoint;
    /* 0x1A */ s8 mRoomNo;
    /* 0x1B */ s8 mLayer;
};

daTagTWGate_Attr_c const daTagTWGate_c::mAttr = {};

#if DEBUG
daTagTWGate_Hio_c daTagTWGate_c::mHio;
#endif

static daTagTWGate_zevParam const l_zevParamTbl[4] = {
    {
        "TWGate_FL",
        "TW_GATE_FILONE",
        "TW_GATE_FILONE_TALK",
        "TW_GATE_FILONE_IN",
        0,
        "F_SP108",
        23,
        0,
        10,
    },
    {
        "TWGate_OD",
        "TW_GATE_ORDIN",
        "TW_GATE_ORDIN_TALK",
        "TW_GATE_ORDIN_IN",
        1,
        "F_SP121",
        10,
        2,
        14,
    },
    {
        "TWGate_RN",
        "TW_GATE_RANAIL",
        "TW_GATE_RANAIL_TALK",
        "TW_GATE_RANAIL_IN",
        2,
        "F_SP121",
        10,
        9,
        14,
    },
    {
        "TWGate_HY",
        "TW_GATE_HYRAL",
        "TW_GATE_HYRAL_TALK",
        "TW_GATE_HYRAL_IN",
        4,
        "F_SP121",
        10,
        2,
        14,
    },
};

static const char* l_myName = "Gate";

const actionFunc daTagTWGate_c::ActionTable[][2] = {
    {&daTagTWGate_c::initWait, &daTagTWGate_c::executeWait},
    {&daTagTWGate_c::initDemoFilone1, &daTagTWGate_c::executeDemoFilone1},
    {&daTagTWGate_c::initDemoFilone2, &daTagTWGate_c::executeDemoFilone2},
    {&daTagTWGate_c::initDemoFilone3, &daTagTWGate_c::executeDemoFilone3},
    {&daTagTWGate_c::initDemoOrdin1, &daTagTWGate_c::executeDemoOrdin1},
    {&daTagTWGate_c::initDemoOrdin2, &daTagTWGate_c::executeDemoOrdin2},
    {&daTagTWGate_c::initDemoOrdin3, &daTagTWGate_c::executeDemoOrdin3},
    {&daTagTWGate_c::initDemoRanail1, &daTagTWGate_c::executeDemoRanail1},
    {&daTagTWGate_c::initDemoRanail2, &daTagTWGate_c::executeDemoRanail2},
    {&daTagTWGate_c::initDemoRanail3, &daTagTWGate_c::executeDemoRanail3},
    {&daTagTWGate_c::initDemoHyral1, &daTagTWGate_c::executeDemoHyral1},
    {&daTagTWGate_c::initDemoHyral2, &daTagTWGate_c::executeDemoHyral2},
    {&daTagTWGate_c::initDemoHyral3, &daTagTWGate_c::executeDemoHyral3},
};

#if DEBUG
daTagTWGate_Hio_c::daTagTWGate_Hio_c() {
    field_0x8 = 0;
    default_set();
}

void daTagTWGate_Hio_c::ct() {
    if (field_0x8++ == 0) {
        // "Twilight Gate tag"
        entryHIO("トワイライトゲートタグ");
    }
}

void daTagTWGate_Hio_c::dt() {
    if (--field_0x8 == 0) {
        removeHIO();
    }
}

void daTagTWGate_Hio_c::default_set() {
    attr = daTagTWGate_c::mAttr;
}

void daTagTWGate_Hio_c::genMessage(JORMContext* ctx) {
    // "Twilight Gate tag parameter settings"
    ctx->genLabel("§ トワイライトゲートタグ パラメータ設定  §\n", 0);
    // "Draw range"
    ctx->genCheckBox("範囲描画", &attr.show_range, 1);
}
#endif

inline daTagTWGate_c::~daTagTWGate_c() {
    dComIfG_resDelete(&mPhaseZevArc, l_zevParamTbl[mType].mArcName);

    if (mPhaseMdRes.id != cPhs_INIT_e) {
        dComIfG_resDelete(&mPhaseMdRes, "TWGate_Md");
    }

    if (mPhasePyRes.id != cPhs_INIT_e) {
        dComIfG_resDelete(&mPhasePyRes, mIsWolf ? "TWGate_Wf" : "TWGate_Lk");
    }

#if DEBUG
    mHio.dt();
#endif
}

void daTagTWGate_c::initWait() {
    mEventID = -1;
}

void daTagTWGate_c::setAction(Mode_e i_action) {
    JUT_ASSERT(233, i_action < MODE_MAX_e);
    int oldActionID = mActionID;
    mActionID = i_action;
    mAction = ActionTable[mActionID];
    callInit();
}

void daTagTWGate_c::callInit() {
    JUT_ASSERT(289, mAction != NULL);
    (this->*(*mAction))();
}

void daTagTWGate_c::callExecute() {
    JUT_ASSERT(302, mAction != NULL);
    (this->*mAction[1])();
    Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_DARK_GATE, &current.pos,
                                  0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
}

void daTagTWGate_c::executeWait() {
    f32 radius = std::pow(scale.x * 100.0f, 2.0f);

    if (fopAcM_searchPlayerDistanceXZ2(this) < radius) {
        if (field_0x5de == 0) {
            field_0x5de = 1;

            switch (mType) {
            case TYPE_FILONE:
                if (fopAcM_isSwitch(this, getSwitch())) {
                    setAction(ACT_DEMO_FILONE_2);
                } else {
                    setAction(ACT_DEMO_FILONE_1);
                }
                break;
            case TYPE_ORDIN:
                if (fopAcM_isSwitch(this, getSwitch())) {
                    setAction(ACT_DEMO_ORDIN_2);
                } else {
                    setAction(ACT_DEMO_ORDIN_1);
                }
                break;
            case TYPE_RANAIL:
                if (fopAcM_isSwitch(this, getSwitch())) {
                    setAction(ACT_DEMO_RANAIL_2);
                } else {
                    setAction(ACT_DEMO_RANAIL_1);
                }
                break;
            case TYPE_HYRAL:
                if (fopAcM_isSwitch(this, getSwitch())) {
                    setAction(ACT_DEMO_HYRAL_2);
                } else {
                    setAction(ACT_DEMO_HYRAL_1);
                }
                break;
            default:
                // "Twilight Gate demo tag   argument 0 error   Type=%d
                OS_REPORT("\nトワイライトゲートデモタグ 引数0エラー Type=%d\n\n", mType);
                JUT_ASSERT(344, FALSE);
            }
        }
    } else {
        field_0x5de = 0;
    }
}

void daTagTWGate_c::initDemoFilone1() {
    eventInfo.setArchiveName(l_zevParamTbl[mType].mArcName);
    mEventID = dComIfGp_getEventManager().getEventIdx(this, l_zevParamTbl[mType].mEventName, -1);
    fopAcM_orderOtherEventId(this, mEventID, -1, -1, 4, 1);
}

void daTagTWGate_c::executeDemoFilone1() {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int staffId = event_manager.getMyStaffId(l_myName, NULL, 0);

    int* cutName;
    if (staffId != -1) {
        cutName = (int*)event_manager.getMyNowCutName(staffId);

        if (event_manager.getIsAddvance(staffId)) {
            switch (*cutName) {
            case '0001':
                dComIfGp_getEvent()->startCheckSkipEdge(this);
                break;
            default:
                JUT_ASSERT(395, FALSE);
            }
        }

        switch (*cutName) {
        case '0001':
            if (dComIfGp_getEvent()->checkSkipEdge()) {
                dComIfGp_getEvent()->onSkipFade();
                setAction(ACT_DEMO_FILONE_2);
            }
            event_manager.cutEnd(staffId);
            break;
        default:
            JUT_ASSERT(407, FALSE);
        }

        if (eventInfo.checkCommandDemoAccrpt() && mEventID != -1 &&
            event_manager.endCheck(mEventID))
        {
            setAction(ACT_DEMO_FILONE_2);
        }
    }
}

void daTagTWGate_c::initDemoFilone2() {
    fopAcM_onSwitch(this, getSwitch());
    eventInfo.setArchiveName(l_zevParamTbl[mType].mArcName);
    dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
    mEventID = dComIfGp_getEventManager().getEventIdx(this, l_zevParamTbl[mType].mTalkEventName, -1);

    if (dComIfGp_event_runCheck()) {
        dComIfGp_getEvent()->reset(this);
        fopAcM_orderChangeEventId(this, mEventID, 1, -1);
    } else {
        fopAcM_orderOtherEventId(this, mEventID, -1, -1, 4, 1);
    }
}

void daTagTWGate_c::executeDemoFilone2() {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    s32 staffId = eventManager.getMyStaffId(l_myName, NULL, 0);

    if (staffId != -1) {
        int* cutName = (int*)eventManager.getMyNowCutName(staffId);

        if (eventManager.getIsAddvance(staffId)) {
            switch (*cutName) {
            case '0002':
                initTalk(0xBC1, NULL);
                break;
            case '0001':
            case '0003':
                break;
            default:
                JUT_ASSERT(480, FALSE);
            }
        }

        switch (*cutName) {
        case '0001':
        case '0003':
            eventManager.cutEnd(staffId);
            break;
        case '0002':
            if (talkProc(NULL, 1, NULL)) {
                int choiceNo = mMsgFlow.getChoiceNo();
                // "Two-way branch %s" / "Yes" / "No"
                OS_REPORT("二択分岐 %s\n", choiceNo == 0 ? "はい" : "いいえ");
                if (choiceNo == 0) {
                    setAction(ACT_DEMO_FILONE_3);
                }
                eventManager.cutEnd(staffId);
            }
            break;
        default:
            JUT_ASSERT(497, FALSE);
        }

        if (eventInfo.checkCommandDemoAccrpt() && mEventID != -1 &&
            eventManager.endCheck(mEventID)) {
            dComIfGp_event_reset();
            field_0x5e0 = 0;
            setAction(ACT_WAIT);
        }
    }
}

void daTagTWGate_c::initDemoFilone3() {
    field_0x5de = 1;
    mIsWolf = daPy_py_c::checkNowWolf() ? TRUE : FALSE;
    eventInfo.setArchiveName(l_zevParamTbl[mType].mArcName);
    dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
    mEventID =
        dComIfGp_getEventManager().getEventIdx(this, l_zevParamTbl[mType].mInEventName, -1);
    dComIfGp_getEvent()->reset(this);
    fopAcM_orderChangeEventId(this, mEventID, 1, -1);
}

void daTagTWGate_c::executeDemoFilone3() {
    dEvent_manager_c& eventManager = dComIfGp_getEventManager();
    s32 staffId = eventManager.getMyStaffId(l_myName, NULL, 0);

    if (staffId != -1) {
        int* cutName = (int*)eventManager.getMyNowCutName(staffId);

        if (eventManager.getIsAddvance(staffId)) {
            switch (*cutName) {
            case '0001':
                break;
            case '0002':
                dComIfGp_getEvent()->startCheckSkipEdge(this);
                field_0x5e1 = 0;
                break;
            case '0003':
                initBaseMtx();
                mpMorf->setPlaySpeed(1.0f);
                field_0x5e0 = 1;
                break;
            default:
                JUT_ASSERT(560, FALSE);
            }
        }

#ifndef __MWERKS__
        int modelSts;
#endif
        switch (*cutName) {
        case '0001':
#ifndef __MWERKS__
            modelSts = downloadModels();
#else
            int modelSts = downloadModels();
#endif
            if (modelSts == 1) {
                eventManager.cutEnd(staffId);
            } else if (modelSts == -1) {
                dComIfGp_setNextStage(l_zevParamTbl[mType].mStage, l_zevParamTbl[mType].mPoint,
                                      l_zevParamTbl[mType].mRoomNo, l_zevParamTbl[mType].mLayer);
            }
            break;
        case '0002':
            if (dComIfGp_getEvent()->checkSkipEdge()) {
                dComIfGp_getEvent()->onSkipFade();
                dComIfGp_setNextStage(l_zevParamTbl[mType].mStage, l_zevParamTbl[mType].mPoint,
                                      l_zevParamTbl[mType].mRoomNo, l_zevParamTbl[mType].mLayer);
            }

            if (field_0x5e1++ == 43) {
                daPy_py_c* player = daPy_getPlayerActorClass();
                cXyz pos;
                csXyz sxyz(0, fopAcM_GetShapeAngle_p(player)->y + 0x8000, 0);

                mDoMtx_stack_c::transS(fopAcM_GetPosition(player));
                mDoMtx_stack_c::YrotM(sxyz.y);
                mDoMtx_stack_c::transM(0.0f, 240.0f, -710.0f);

                mDoMtx_stack_c::multVecZero(&pos);
                dComIfGp_particle_set(0x86C5, &pos, &sxyz, NULL);
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_DARK_GATE_RIPPLE, &pos,
                                         0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                eventManager.cutEnd(staffId);
            }
            break;
        case '0003':
            if (dComIfGp_getEvent()->checkSkipEdge()) {
                dComIfGp_getEvent()->onSkipFade();
                dComIfGp_setNextStage(l_zevParamTbl[mType].mStage,
                                      l_zevParamTbl[mType].mPoint,
                                      l_zevParamTbl[mType].mRoomNo,
                                      l_zevParamTbl[mType].mLayer);
            }

            if (mpMorf->isStop()) {
                eventManager.cutEnd(staffId);
            }
            break;
        default:
            JUT_ASSERT(621, FALSE);
            break;
        }

        if (eventInfo.checkCommandDemoAccrpt() && mEventID != -1 &&
            eventManager.endCheck(mEventID)) {
            dComIfGp_event_reset();
            field_0x5e0 = 0;
            setAction(ACT_WAIT);
        }
    }
}

void daTagTWGate_c::initDemoOrdin1() {
    eventInfo.setArchiveName(l_zevParamTbl[mType].mArcName);
    mEventID = dComIfGp_getEventManager().getEventIdx(this, l_zevParamTbl[mType].mEventName, -1);
    fopAcM_orderOtherEventId(this, mEventID, -1, -1, 4, 1);
}

void daTagTWGate_c::executeDemoOrdin1() {
    dEvent_manager_c& evtMng = dComIfGp_getEventManager();
    int staffId = evtMng.getMyStaffId(l_myName, NULL, 0);
    int* cutName;
    if (staffId != -1) {
        cutName = (int*)evtMng.getMyNowCutName(staffId);
        if (evtMng.getIsAddvance(staffId)) {
            switch (*cutName) {
            case '0001':
                dComIfGp_getEvent()->startCheckSkipEdge(this);
                break;
            default:
                JUT_ASSERT(674, FALSE);
            }
        }
        switch (*cutName) {
        case '0001':
            if (dComIfGp_getEvent()->checkSkipEdge()) {
                dComIfGp_getEvent()->onSkipFade();
                setAction(ACT_DEMO_ORDIN_2);
            }
            evtMng.cutEnd(staffId);
            break;
        default:
            JUT_ASSERT(686, FALSE);
            break;
        }
        if (eventInfo.checkCommandDemoAccrpt() && mEventID != -1 && evtMng.endCheck(mEventID)) {
            setAction(ACT_DEMO_ORDIN_2);
        }
    }
}

void daTagTWGate_c::initDemoOrdin2() {
    fopAcM_onSwitch(this, getSwitch());
    eventInfo.setArchiveName(l_zevParamTbl[mType].mArcName);
    dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
    mEventID = dComIfGp_getEventManager().getEventIdx(this, l_zevParamTbl[mType].mTalkEventName, 0xff);
    if (dComIfGp_event_runCheck()) {
        dComIfGp_getEvent()->reset(this);
        fopAcM_orderChangeEventId(this, mEventID, 1, 0xffff);
    } else {
        fopAcM_orderOtherEventId(this, mEventID, 0xff, 0xffff, 4, 1);
    }
}

void daTagTWGate_c::executeDemoOrdin2() {
    dEvent_manager_c& evtMng = dComIfGp_getEventManager();
    int staffId = evtMng.getMyStaffId(l_myName, NULL, 0);
    if (staffId == -1) {
        return;
    }
    int* cutName = (int*)evtMng.getMyNowCutName(staffId);
    if (evtMng.getIsAddvance(staffId)) {
        switch(*cutName) {
        case '0001':
            break;
        case '0002':
            initTalk(0xbc1, NULL);
            break;
        case '0003':
            break;
        default:
            JUT_ASSERT(749, FALSE);
            break;
        }
    }
    switch (*cutName) {
    case '0001':
    case '0003':
        evtMng.cutEnd(staffId);
        break;
    case '0002':
        if (talkProc(NULL, 1, NULL)) {
            int choiceNo = mMsgFlow.getChoiceNo();
            // "Two-way branch %s" / "Yes" / "No"
            OS_REPORT("二択分岐 %s\n", choiceNo == 0 ? "はい" : "いいえ");
            if (choiceNo == 0) {
                setAction(ACT_DEMO_ORDIN_3);
            }
            evtMng.cutEnd(staffId);
        }
        break;
    default:
        JUT_ASSERT(766, FALSE);
        break;
    }

    if (eventInfo.checkCommandDemoAccrpt() && mEventID != -1 &&
        evtMng.endCheck(mEventID))
    {
        dComIfGp_event_reset();
        field_0x5e0 = 0;
        setAction(ACT_WAIT);
    }
}

void daTagTWGate_c::initDemoOrdin3() {
    field_0x5de = 1;
    mIsWolf = daPy_py_c::checkNowWolf() ? TRUE : FALSE;
    eventInfo.setArchiveName(l_zevParamTbl[mType].mArcName);
    dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
    mEventID =
        dComIfGp_getEventManager().getEventIdx(this, l_zevParamTbl[mType].mInEventName, -1);
    dComIfGp_getEvent()->reset(this);
    fopAcM_orderChangeEventId(this, mEventID, 1, -1);
}

void daTagTWGate_c::executeDemoOrdin3() {
    dEvent_manager_c& evtMng = dComIfGp_getEventManager();
    int staffId = evtMng.getMyStaffId(l_myName, NULL, 0);
    if (staffId == -1) {
        return;
    }
    int* cutName = (int*)evtMng.getMyNowCutName(staffId);
    if (evtMng.getIsAddvance(staffId)) {
        switch(*cutName) {
        case '0001':
            break;
        case '0002':
            dComIfGp_getEvent()->startCheckSkipEdge(this);
            field_0x5e1 = 0;
            break;
        case '0003':
            initBaseMtx();
           mpMorf->setPlaySpeed(1.0f);
            field_0x5e0 = 1;
            break;
        default:
            JUT_ASSERT(829, FALSE);
            break;
        }

    }
    switch(*cutName) {
    case '0001':
        int downloadResult;
        downloadResult = downloadModels();
        if (downloadResult == 1) {
            evtMng.cutEnd(staffId);
        } else if (downloadResult == -1) {
            dComIfGp_setNextStage(l_zevParamTbl[mType].mStage, l_zevParamTbl[mType].mPoint,
                                  l_zevParamTbl[mType].mRoomNo, l_zevParamTbl[mType].mLayer);
        }
        break;
    case '0002':
        if (dComIfGp_getEvent()->checkSkipEdge()) {
            dComIfGp_getEvent()->onSkipFade();
            dComIfGp_setNextStage(l_zevParamTbl[mType].mStage, l_zevParamTbl[mType].mPoint,
                                  l_zevParamTbl[mType].mRoomNo, l_zevParamTbl[mType].mLayer);
        }
        if (field_0x5e1++ == (daPy_py_c::checkNowWolf() ? 0x2e : 0x4a)) {
            daPy_py_c* player = daPy_getPlayerActorClass();
            cXyz cStack_30;
            csXyz cStack_38(0, fopAcM_GetShapeAngle_p(player)->y + 0x8000, 0);
            mDoMtx_stack_c::transS(fopAcM_GetPosition(player));
            mDoMtx_stack_c::YrotM(cStack_38.y);
            mDoMtx_stack_c::transM(0.0f, 240.0f, -710.0f);
            mDoMtx_stack_c::multVecZero(&cStack_30);
            dComIfGp_particle_set(0x86c5, &cStack_30, &cStack_38, NULL);
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_DARK_GATE_RIPPLE, &cStack_30, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
            evtMng.cutEnd(staffId);
        }
        break;
    case '0003':
        if (dComIfGp_getEvent()->checkSkipEdge()) {
            dComIfGp_getEvent()->onSkipFade();
            dComIfGp_setNextStage(
                l_zevParamTbl[mType].mStage,
                l_zevParamTbl[mType].mPoint,
                l_zevParamTbl[mType].mRoomNo,
                l_zevParamTbl[mType].mLayer);
        }
        if (mpMorf->isStop()) {
            evtMng.cutEnd(staffId);
        }
        break;
    default:
        JUT_ASSERT(890, FALSE);
        break;
    }

    if (eventInfo.checkCommandDemoAccrpt() && mEventID != -1 && evtMng.endCheck(mEventID)) {
        dComIfGp_event_reset();
        field_0x5e0 = 0;
        setAction(ACT_WAIT);
    }
}

void daTagTWGate_c::initDemoRanail1() {
    eventInfo.setArchiveName(l_zevParamTbl[mType].mArcName);
    mEventID = dComIfGp_getEventManager().getEventIdx(this, l_zevParamTbl[mType].mEventName, -1);
    fopAcM_orderOtherEventId(this, mEventID, -1, -1, 4, 1);
}

void daTagTWGate_c::executeDemoRanail1() {
    dEvent_manager_c& evtMng = dComIfGp_getEventManager();
    int staffId = evtMng.getMyStaffId(l_myName, NULL, 0);
    int* cutName;
    if (staffId != -1) {
        cutName = (int*)evtMng.getMyNowCutName(staffId);
        if (evtMng.getIsAddvance(staffId)) {
            switch (*cutName) {
            case '0001':
                dComIfGp_getEvent()->startCheckSkipEdge(this);
                break;
            default:
                JUT_ASSERT(943, FALSE);
                break;
            }
        }
        switch (*cutName) {
        case '0001':
            if (dComIfGp_getEvent()->checkSkipEdge()) {
                dComIfGp_getEvent()->onSkipFade();
                setAction(ACT_DEMO_RANAIL_2);
            }
            evtMng.cutEnd(staffId);
            break;
        default:
            JUT_ASSERT(955, FALSE);
            break;
        }
        if (eventInfo.checkCommandDemoAccrpt() && mEventID != -1 && evtMng.endCheck(mEventID)) {
            setAction(ACT_DEMO_RANAIL_2);
        }
    }
}

void daTagTWGate_c::initDemoRanail2() {
    fopAcM_onSwitch(this, getSwitch());
    eventInfo.setArchiveName(l_zevParamTbl[mType].mArcName);
    dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
    mEventID = dComIfGp_getEventManager().getEventIdx(this, l_zevParamTbl[mType].mTalkEventName, 0xff);
    if (dComIfGp_event_runCheck()) {
        dComIfGp_getEvent()->reset(this);
        fopAcM_orderChangeEventId(this, mEventID, 1, 0xffff);
    } else {
        fopAcM_orderOtherEventId(this, mEventID, 0xff, 0xffff, 4, 1);
    }
}

void daTagTWGate_c::executeDemoRanail2() {
    dEvent_manager_c& evtMng = dComIfGp_getEventManager();
    int staffId = evtMng.getMyStaffId(l_myName, NULL, 0);
    if (staffId == -1) {
        return;
    }
    int* cutName = (int*)evtMng.getMyNowCutName(staffId);
    if (evtMng.getIsAddvance(staffId)) {
        switch(*cutName) {
        case '0001':
            break;
        case '0002':
            initTalk(0xbc1, NULL);
            break;
        case '0003':
            break;
        default:
            JUT_ASSERT(1018, FALSE);
            break;
        }
    }
    switch (*cutName) {
    case '0001':
    case '0003':
        evtMng.cutEnd(staffId);
        break;
    case '0002':
        if (talkProc(NULL, 1, NULL)) {
            int choiceNo = mMsgFlow.getChoiceNo();
            // "Two-way branch" / "Yes" / "No"
            OS_REPORT("二択分岐 %s\n", choiceNo == 0 ? "はい" : "いいえ");
            if (choiceNo == 0) {
                setAction(ACT_DEMO_RANAIL_3);
            }
            evtMng.cutEnd(staffId);
        }
        break;
    default:
        JUT_ASSERT(1035, FALSE);
        break;
    }

    if (eventInfo.checkCommandDemoAccrpt() && mEventID != -1 &&
        evtMng.endCheck(mEventID))
    {
        dComIfGp_event_reset();
        field_0x5e0 = 0;
        setAction(ACT_WAIT);
    }
}

void daTagTWGate_c::initDemoRanail3() {
    field_0x5de = 1;
    mIsWolf = daPy_py_c::checkNowWolf() ? TRUE : FALSE;
    eventInfo.setArchiveName(l_zevParamTbl[mType].mArcName);
    dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
    mEventID =
        dComIfGp_getEventManager().getEventIdx(this, l_zevParamTbl[mType].mInEventName, -1);
    dComIfGp_getEvent()->reset(this);
    fopAcM_orderChangeEventId(this, mEventID, 1, -1);
    /* dSv_event_flag_c::F_0071 - Twilight Hyrule Field - Cannot warp to Lanayru */
    dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[170]);
}

void daTagTWGate_c::executeDemoRanail3() {
    dEvent_manager_c& evtMng = dComIfGp_getEventManager();
    int staffId = evtMng.getMyStaffId(l_myName, NULL, 0);
    if (staffId == -1) {
        return;
    }
    int* cutName = (int*)evtMng.getMyNowCutName(staffId);
    if (evtMng.getIsAddvance(staffId)) {
        switch(*cutName) {
        case '0001':
            break;
        case '0002':
            dComIfGp_getEvent()->startCheckSkipEdge(this);
            field_0x5e1 = 0;
            break;
        case '0003':
            initBaseMtx();
           mpMorf->setPlaySpeed(1.0f);
            field_0x5e0 = 1;
            break;
        default:
            JUT_ASSERT(1099, FALSE);
            break;
        }
    }
    switch(*cutName) {
    case '0001': 
        int downloadResult;
        downloadResult = downloadModels();
        if (downloadResult == 1) {
            evtMng.cutEnd(staffId);
        } else if (downloadResult == -1) {
            dComIfGp_setNextStage(l_zevParamTbl[mType].mStage, l_zevParamTbl[mType].mPoint,
                                  l_zevParamTbl[mType].mRoomNo, l_zevParamTbl[mType].mLayer);
        }
        break;
    case '0002':
        if (dComIfGp_getEvent()->checkSkipEdge()) {
            dComIfGp_getEvent()->onSkipFade();
            dComIfGp_setNextStage(l_zevParamTbl[mType].mStage, l_zevParamTbl[mType].mPoint,
                                  l_zevParamTbl[mType].mRoomNo, l_zevParamTbl[mType].mLayer);
        }
        if (field_0x5e1++ == (daPy_py_c::checkNowWolf() ? 0x2e : 0x4a)) {
            daPy_py_c* player = daPy_getPlayerActorClass();
            cXyz cStack_30;
            csXyz cStack_38(0, fopAcM_GetShapeAngle_p(player)->y + 0x8000, 0);
            mDoMtx_stack_c::transS(fopAcM_GetPosition(player));
            mDoMtx_stack_c::YrotM(cStack_38.y);
            mDoMtx_stack_c::transM(0.0f, 240.0f, -710.0f);
            mDoMtx_stack_c::multVecZero(&cStack_30);
            dComIfGp_particle_set(0x86c5, &cStack_30, &cStack_38, NULL);
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_DARK_GATE_RIPPLE, &cStack_30, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
            evtMng.cutEnd(staffId);
        }
        break;
    case '0003':
        if (dComIfGp_getEvent()->checkSkipEdge()) {
            dComIfGp_getEvent()->onSkipFade();
            dComIfGp_setNextStage(
                l_zevParamTbl[mType].mStage,
                l_zevParamTbl[mType].mPoint,
                l_zevParamTbl[mType].mRoomNo,
                l_zevParamTbl[mType].mLayer);
        }
        if (mpMorf->isStop()) {
            evtMng.cutEnd(staffId);
        }
        break;
    default:
        JUT_ASSERT(1160, FALSE);
        break;
    }

    if (eventInfo.checkCommandDemoAccrpt() && mEventID != -1 && evtMng.endCheck(mEventID)) {
        dComIfGp_event_reset();
        field_0x5e0 = 0;
        setAction(ACT_WAIT);
    }
}

void daTagTWGate_c::initDemoHyral1() {
    eventInfo.setArchiveName(l_zevParamTbl[mType].mArcName);
    mEventID = dComIfGp_getEventManager().getEventIdx(this, l_zevParamTbl[mType].mEventName, -1);
    fopAcM_orderOtherEventId(this, mEventID, -1, -1, 4, 1);
}

void daTagTWGate_c::executeDemoHyral1() {
    dEvent_manager_c& evtMng = dComIfGp_getEventManager();
    int staffId = evtMng.getMyStaffId(l_myName, NULL, 0);
    int* cutName;
    if (staffId != -1) {
        cutName = (int*)evtMng.getMyNowCutName(staffId);
        if (evtMng.getIsAddvance(staffId)) {
            switch (*cutName) {
            case '0001':
                dComIfGp_getEvent()->startCheckSkipEdge(this);
                break;
            default:
                JUT_ASSERT(1213, FALSE);
            }
        }
        switch (*cutName) {
        case '0001':
            if (dComIfGp_getEvent()->checkSkipEdge()) {
                dComIfGp_getEvent()->onSkipFade();
                setAction(ACT_DEMO_HYRAL_2);
            }
            evtMng.cutEnd(staffId);
            break;
        default:
            JUT_ASSERT(1225, FALSE);
            break;
        }
        if (eventInfo.checkCommandDemoAccrpt() && mEventID != -1 && evtMng.endCheck(mEventID)) {
            setAction(ACT_DEMO_HYRAL_2);
        }
    }
}

void daTagTWGate_c::initDemoHyral2() {
    fopAcM_onSwitch(this, getSwitch());
    eventInfo.setArchiveName(l_zevParamTbl[mType].mArcName);
    dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
    mEventID = dComIfGp_getEventManager().getEventIdx(this, l_zevParamTbl[mType].mTalkEventName, 0xff);
    if (dComIfGp_event_runCheck()) {
        dComIfGp_getEvent()->reset(this);
        fopAcM_orderChangeEventId(this, mEventID, 1, 0xffff);
    } else {
        fopAcM_orderOtherEventId(this, mEventID, 0xff, 0xffff, 4, 1);
    }
}

void daTagTWGate_c::executeDemoHyral2() {
    dEvent_manager_c& eventMgr = dComIfGp_getEventManager();
    int staffId = eventMgr.getMyStaffId(l_myName, NULL, 0);
    if (staffId == -1) {
        return;
    }
    int* piVar3 = (int*)eventMgr.getMyNowCutName(staffId);
    if (eventMgr.getIsAddvance(staffId)) {
        switch(*piVar3) {
        case '0001':
        case '0003':
            break;
        case '0002':
            initTalk(0xbc1, NULL);
            break;
        default:
            JUT_ASSERT(1288, FALSE);
        }
    }
    switch (*piVar3) {
    case '0001':
    case '0003':
        eventMgr.cutEnd(staffId);
        break;
    case '0002':
        if (talkProc(NULL, 1, NULL)) {
            int choiceNo = mMsgFlow.getChoiceNo();
            OS_REPORT("二択分岐 %s\n", choiceNo == 0 ? "はい" : "いいえ");
            if (choiceNo == 0) {
                setAction(ACT_DEMO_HYRAL_3);
            }
            eventMgr.cutEnd(staffId);
        }
        break;
    default:
        JUT_ASSERT(1305, FALSE);
        break;
    }
    if (eventInfo.checkCommandDemoAccrpt() && mEventID != -1 &&
        eventMgr.endCheck(mEventID))
    {
        dComIfGp_event_reset();
        field_0x5e0 = 0;
        setAction(ACT_WAIT);
    }
}

void daTagTWGate_c::initDemoHyral3() {
    field_0x5de = 1;
    mIsWolf = daPy_py_c::checkNowWolf() ? TRUE : FALSE;
    eventInfo.setArchiveName(l_zevParamTbl[mType].mArcName);
    dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
    mEventID =
        dComIfGp_getEventManager().getEventIdx(this, l_zevParamTbl[mType].mInEventName, -1);
    dComIfGp_getEvent()->reset(this);
    fopAcM_orderChangeEventId(this, mEventID, 1, -1);
}

void daTagTWGate_c::executeDemoHyral3() {
    dEvent_manager_c& eventMgr = dComIfGp_getEventManager();
    int staffId = eventMgr.getMyStaffId(l_myName, NULL, 0);
    if (staffId == -1) {
        return;
    }
    int* piVar3 = (int*)eventMgr.getMyNowCutName(staffId);
    if (eventMgr.getIsAddvance(staffId)) {
        switch(*piVar3) {
        case '0001':
            break;
        case '0002':
            dComIfGp_getEvent()->startCheckSkipEdge(this);
            field_0x5e1 = 0x0;
            break;
        case '0003':
            initBaseMtx();
            mpMorf->setPlaySpeed(1.0f);
            field_0x5e0 = 1;
            break;
        default:
            JUT_ASSERT(1368, FALSE);
        }
    }
    switch(*piVar3) {
    case '0001':
        int downloadRes;
        downloadRes = downloadModels();
        if (downloadRes == 1) {
            eventMgr.cutEnd(staffId);
        } else if (downloadRes == -1) {
            dComIfGp_setNextStage(l_zevParamTbl[mType].mStage, l_zevParamTbl[mType].mPoint,
                                  l_zevParamTbl[mType].mRoomNo, l_zevParamTbl[mType].mLayer);
        }
        break;
    case '0002':
        if (dComIfGp_getEvent()->checkSkipEdge()) {
            dComIfGp_getEvent()->onSkipFade();
            dComIfGp_setNextStage(l_zevParamTbl[mType].mStage, l_zevParamTbl[mType].mPoint,
                                  l_zevParamTbl[mType].mRoomNo, l_zevParamTbl[mType].mLayer);
        }
        if (field_0x5e1++ == 0x4a) {
            daPy_py_c* player = daPy_getPlayerActorClass();
            cXyz cStack_34;
            csXyz cStack_3c(0, fopAcM_GetShapeAngle_p(player)->y + 0x8000, 0);
            mDoMtx_stack_c::transS(fopAcM_GetPosition(player));
            mDoMtx_stack_c::YrotM(cStack_3c.y);
            mDoMtx_stack_c::transM(0.0f, 240.0f, -710.0f);
            mDoMtx_stack_c::multVecZero(&cStack_34);
            dComIfGp_particle_set(0x86c5, &cStack_34, &cStack_3c, NULL);
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_DARK_GATE_RIPPLE, &cStack_34, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            eventMgr.cutEnd(staffId);
        }
        break;
    case '0003':
        if (dComIfGp_getEvent()->checkSkipEdge()) {
            dComIfGp_getEvent()->onSkipFade();
            dComIfGp_setNextStage(l_zevParamTbl[mType].mStage, l_zevParamTbl[mType].mPoint,
                                  l_zevParamTbl[mType].mRoomNo, l_zevParamTbl[mType].mLayer);
        }
        if (mpMorf->isStop()) {
            eventMgr.cutEnd(staffId);
        }
        break;
    default:
        JUT_ASSERT(1417, FALSE);
        break;
    }
    if (eventInfo.checkCommandDemoAccrpt() && mEventID != -1 &&
        eventMgr.endCheck(mEventID))
    {
        dComIfGp_event_reset();
        field_0x5e0 = 0;
        setAction(ACT_WAIT);
    }
}

void daTagTWGate_c::initBaseMtx() {
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    fopAc_ac_c* player = daPy_getPlayerActorClass();

    shape_angle.y = fopAcM_GetShapeAngle_p(player)->y + 0x8000;
    mDoMtx_stack_c::transS(fopAcM_GetPosition(player));
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::transM(0.0f, 250.0f, -840.0f);

    mDoMtx_stack_c::multVecZero(&current.pos);

    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daTagTWGate_c::downloadModels() {
    int phase_state = dComIfG_resLoad(&mPhaseMdRes, "TWGate_Md");

    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state = dComIfG_resLoad(&mPhasePyRes, mIsWolf ? "TWGate_Wf" : "TWGate_Lk");
    }

    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x3140)) {
            return -1;
        } else {
            return 1;
        }
    }

    if (phase_state == cPhs_ERROR_e) {
        return -1;
    } else {
        return 0;
    }
}

void daTagTWGate_c::initTalk(int param_0, fopAc_ac_c** param_1) {
    mMsgFlow.init(this, param_0, 0, param_1);
}

bool daTagTWGate_c::talkProc(int* param_1, int param_2, fopAc_ac_c** param_3) {
    bool rv = false;
    if (dMsgObject_isMsgSendControl()) {
        if (param_2 != 0) {
            dMsgObject_offMsgSendControl();
        }
    } else {
        if (mMsgFlow.doFlow(this, param_3, 0)) {
            rv = true;
        } else {
            if (mMsgFlow.getMsg() != NULL &&
                (mMsgFlow.getMsg()->mode == 2 || mMsgFlow.getMsg()->mode == 6) && param_1 != NULL)
            {
                for (int i = 0; param_1[i] > 0; i++) {
                    if (param_1[i] == mMsgFlow.getMsgNo()) {
                        rv = true;
                        break;
                    }
                }
            }
        }
    }
    return rv;
}

int daTagTWGate_c::createHeapCallBack(fopAc_ac_c* i_actor) {
    daTagTWGate_c* gate = static_cast<daTagTWGate_c*>(i_actor);
    return gate->CreateHeap();
}

int daTagTWGate_c::CreateHeap() {
    J3DModelData* mdlData_p = NULL;
    int var_r27 = 0;
    mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes("TWGate_Md", 7));
    JUT_ASSERT(1685, NULL != mdlData_p);
    mpMorf =
        new mDoExt_McaMorfSO(mdlData_p, NULL, NULL,
                             static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("TWGate_Md", 4)),
                             0, 0.0f, 0, -1, NULL, 0, 0x11000084);

    if (mpMorf != NULL && mpMorf->getModel() == NULL) {
        mpMorf->stopZelAnime();
        mpMorf = NULL;
    }

    if (mpMorf == NULL) {
        return 0;
    } else {
        return 1;
    }
}

int daTagTWGate_c::create() {
    fopAcM_ct(this, daTagTWGate_c);

    mType = getType();

    int phase_state;
    if (dComIfGs_isDarkClearLV(l_zevParamTbl[mType].mLv)) {
        return cPhs_ERROR_e;
    } else {
        phase_state = dComIfG_resLoad(&mPhaseZevArc, l_zevParamTbl[mType].mArcName);
        if (phase_state == cPhs_COMPLEATE_e) {
            create_init();
        }
    }

    return phase_state;
}

int daTagTWGate_c::draw() {
    if (field_0x5e0 != 0) {
        mpMorf->entryDL();
    }

#if DEBUG
    if (attr().show_range) {
        dDbVw_drawCylinderOpa(current.pos, scale.x * 100.0f, scale.y * 100.0f, (GXColor){0xff, 0xff, 0xff, 0xff}, 1);
    }
#endif

    return 1;
}

void daTagTWGate_c::create_init() {
#if DEBUG
    mHio.ct();
#endif
    field_0x5e0 = 0;
    setAction(ACT_WAIT);
}

static int daTagTWGate_Create(fopAc_ac_c* i_this) {
    daTagTWGate_c* gate = static_cast<daTagTWGate_c*>(i_this);
    fopAcM_RegisterCreateID(i_this, "Tag_TWGate");
    return gate->create();
}

static int daTagTWGate_Delete(daTagTWGate_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "Tag_TWGate");
    i_this->~daTagTWGate_c();
    return 1;
}

int daTagTWGate_c::execute() {
    callExecute();

    if (field_0x5e0 != 0) {
        mpMorf->play(0, 0);
        mpMorf->modelCalc();
    }
    return 1;
}

static int daTagTWGate_Execute(daTagTWGate_c* i_this) {
    return i_this->execute();
}

static int daTagTWGate_Draw(daTagTWGate_c* i_this) {
    return i_this->draw();
}

static int daTagTWGate_IsDelete(daTagTWGate_c* i_this) {
    return 1;
}

static actor_method_class l_daTagTWGate_Method = {
    (process_method_func)daTagTWGate_Create,  (process_method_func)daTagTWGate_Delete,
    (process_method_func)daTagTWGate_Execute, (process_method_func)daTagTWGate_IsDelete,
    (process_method_func)daTagTWGate_Draw,
};

actor_process_profile_definition g_profile_Tag_TWGate = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_TWGate,
    &g_fpcLf_Method.base,
    sizeof(daTagTWGate_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x105,
    &l_daTagTWGate_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
