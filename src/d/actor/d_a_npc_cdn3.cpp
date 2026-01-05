/**
 * @file d_a_npc_cdn3.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_cdn3.h"
#include "d/d_msg_object.h"
#include "d/d_s_play.h"

const daNpcCdn3_c::ActionPair daNpcCdn3_c::ActionTable[8] = {
    {&daNpcCdn3_c::initWait, &daNpcCdn3_c::executeWait},
    {&daNpcCdn3_c::initPath, &daNpcCdn3_c::executePath},
    {&daNpcCdn3_c::initTalk, &daNpcCdn3_c::executeTalk},
    {&daNpcCdn3_c::initEscape, &daNpcCdn3_c::executeEscape},
    {&daNpcCdn3_c::initFear, &daNpcCdn3_c::executeFear},
    {&daNpcCdn3_c::initFear2, &daNpcCdn3_c::executeFear2},
    {&daNpcCdn3_c::initFight, &daNpcCdn3_c::executeFight},
    {&daNpcCdn3_c::initRelief, &daNpcCdn3_c::executeRelief},
};

int daNpcCdn3_c::createHeap() {
    int rv = NpcCreate(m_type);
    if (rv != 0) {
        if (mObjNum != 0) {
            rv = NULL != (field_0xac8 = ObjCreate(mObjNum));
            if (!rv) {
                mpMorf->stopZelAnime();
            }
        } else {
            field_0xac8 = NULL;
            rv = 1;
        }
        if (isChairStyle()) {
            rv = NULL != (field_0xacc = ChairCreate(Cd2_HIO_objScale(m_type)));
            if (rv == 0) {
                mpMorf->stopZelAnime();
            }
        } else {
            field_0xacc = NULL;
            rv = 1;
        }
    }
    return rv;
}

static int createHeapCallBack(fopAc_ac_c* param_0) {
    daNpcCdn3_c* npc = (daNpcCdn3_c*)param_0;
    return npc->createHeap();
}

void daNpcCdn3_c::setAction(daNpcCdn3_c::Mode_e i_action) {
    JUT_ASSERT(366, i_action < MODE_MAX_e);
    mPrevMode = mMode;
    mMode = i_action;
    mAction = &ActionTable[mMode].mInitFn;
    callInit();
}

void daNpcCdn3_c::callInit() {
    JUT_ASSERT(408, mAction != NULL);
    (this->*(mAction[0]))();
}

void daNpcCdn3_c::callExecute() {
    JUT_ASSERT(421, mAction != NULL);
    (this->*(mAction[1]))();
}

void daNpcCdn3_c::initWait() {
    speedF = 0.0f;
    m_seqStep = 0;
    field_0xb94 = 1;
    (this->*(m_funcTbl[m_seqNum][m_seqStep]))(NULL);
}

void daNpcCdn3_c::executeWait() {
    if ((this->*(m_funcTbl[m_seqNum][m_seqStep]))(NULL) != 0) {
        m_seqStep = (m_funcTbl[m_seqNum][m_seqStep + 1] == NULL) ? 0 : m_seqStep + 1;
        field_0xb94 = 1;
        if (mpMorf->getAnm() == getAnmP(8, mObjNum)) {
            mCitizen.playVoice(3);
        } else if (mpMorf->getAnm() == getAnmP(49, mObjNum)) {
            mCitizen.playVoice(4);
        } else if (mpMorf->getAnm() == getAnmP(20, mObjNum) || mpMorf->getAnm() == getAnmP(21, mObjNum)) {
            mCitizen.playVoice(5);
        }
    }
    checkSchedule();
    if (checkFearSituation()) {
        if (mObjNum == 2 && (m_type == 0 || m_type == 1)) {
            setAction(MODE_FIGHT);
        } else {
            setAction(MODE_FEAR);
        }
    }
}

void daNpcCdn3_c::initPath() {
    setAnm((J3DAnmTransformKey*) getAnmP(2, mObjNum), 1.0f, 12.0f, 2, 0, -1);
    field_0xb68 = 2;
    m_seqStep = 0;
    field_0xb94 = 1;
    field_0xb98 = 0;
    if (mTagSched != NULL) {
        m_seqNum = mTagSched->getSeqNum();
        if (m_seqNum >= 0 && m_seqNum < 44) {
            (this->*(m_funcTbl[m_seqNum][0]))(NULL);
        }
    }
}

void daNpcCdn3_c::executePath() {
    if (m_path.isPath()) {
        switch (field_0xb98) {
        case 0:
            if (m_path.checkPoint(current.pos, speedF)) {
                field_0xb74 = -1;
                if (m_path.getArg0() != -1) {
                    field_0xb74 = getTimeMinute() + getTimeHour() * 60 + m_path.getArg0() * 10;
                }
                m_seqNum = m_path.getArg1();
                if (m_path.checkPathEnd(current.pos, speedF)) {
                    if (getGroupID() != 0xff) {
                        int startEnd = mTagSched->getStartEnd();
                        if (startEnd == 1) {
                            setSchedule(field_0xb88);
                            field_0xb88 = NULL;
                            field_0xb95 = 1;
                        } else {
                            if (m_path.isClose()) {
                                m_path.setNextPoint(current.pos);
                            } else {
                                if (m_seqNum < 0 || m_seqNum >= 44) {
                                    // "Castle Town Townspeople Path Point Argument Setting Value Error
                                    //  Argument 1 Motion Sequence = %d is abnormal"
                                    OSReport_Error("城下町の町人 パス点引数設定値異常 引数1モーションシーケンス=%dが異常です。\n", m_seqNum);
                                    JUT_ASSERT(517, FALSE);
                                }
                                field_0xb68 = 0;
                                setAction(MODE_WAIT);
                            }
                        }
                        checkSchedule();
                    } else {
                        if (!m_path.isClose()) {
                            int time = getTime();
                            field_0xb8c = time + field_0xb90;
                            current.pos.set(m_path.getPoint(0));
                            home.pos.set(current.pos);
                            old.pos.set(current.pos);
                            current.angle.y = (s16)cLib_targetAngleY(m_path.getPoint(0), m_path.getPoint(1));
                            shape_angle.y = current.angle.y;
                            home.angle.y = current.angle.y;
                            old.angle.y = current.angle.y;
                            m_path.setPath(getPathID(), fopAcM_GetRoomNo(this), 1, &current.pos,
                                               false);
                            setAction(MODE_PATH);
                            field_0xb95 = 1;
                        }
                    }
                } else {
                    if (m_seqNum >= 0 && m_seqNum < 44) {
                        field_0xb68 = 0;
                        field_0xb98 = 1;
                    } else {
                        if (mTagSched != NULL) {
                            m_seqNum = mTagSched->getSeqNum();
                            if (m_seqNum >= 0 && m_seqNum < 44) {
                                (this->*(m_funcTbl[m_seqNum][0]))(NULL);
                            }
                        }
                        m_path.setNextPoint(current.pos);
                    }
                }
            }
            break;
        case 1:
            if (field_0xb74 != -1) {
                if ((this->*(m_funcTbl[m_seqNum][m_seqStep]))(NULL)) {
                    m_seqStep = (m_funcTbl[m_seqNum][m_seqStep + 1] == NULL) ? 0 : m_seqStep + 1;
                    field_0xb94 = 1;
                }
                int time = getTime();
                if (field_0xb74 <= time) {
                    if (field_0xb68 == 0) {
                        setAnm((J3DAnmTransformKey*)getAnmP(2, mObjNum), 1.0f, 12.0f, 2, 0, -1);
                        field_0xb68 = 2;
                    }
                    m_seqStep = 0;
                    field_0xb94 = 1;
                    m_path.setNextPoint(current.pos);
                    field_0xb98 = 0;
                }
            } else {
                if ((this->*(m_funcTbl[m_seqNum][m_seqStep]))(NULL)) {
                    if (m_funcTbl[m_seqNum][m_seqStep + 1] == NULL) {
                        if (field_0xb68 == 0) {
                            setAnm((J3DAnmTransformKey*)getAnmP(2, mObjNum), 1.0f, 12.0f, 2, 0, -1);
                            field_0xb68 = 2;
                        }
                        m_seqStep = 0;
                        m_path.setNextPoint(current.pos);
                        field_0xb98 = 0;
                    } else {
                        m_seqStep++;
                    }
                    field_0xb94 = 1;
                }
            }
            break;
        default:
            JUT_ASSERT(598, FALSE);
            break;
        }

        if (field_0xb68 == 2 || field_0xb68 == 1) {
            if (!fopAcM_CheckCondition(this, 4)) {
                m_path.checkObstacle(this);
            }
            cXyz cStack_58;
            m_path.getTargetPoint(&cStack_58);
            cLib_addCalcAngleS2(&shape_angle.y, cLib_targetAngleY(&current.pos, &cStack_58), 0xd, 0x600);
            if (current.pos.y != old.pos.y) {
                s16 sVar10 = 0;
                cM3dGPla acStack_4c;
                bool triPla = dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &acStack_4c);
                if (triPla) {
                    sVar10 = fopAcM_getPolygonAngle(&acStack_4c, shape_angle.y);
                }
                if (sVar10 != 0) {
                    field_0xb64 = current.pos.y;
                }
            }
            if (mAcch.ChkWallHit() && current.pos.abs(old.pos) < 2.0f) {
                field_0xb99++;
                if (field_0xb99 > 30) {
                    setAction(MODE_RELIEF);
                }
            } else {
                field_0xb99 = 0;
            }
        }
        setAngle();
        pathMoveF();
        mAcch.CrrPos(dComIfG_Bgsp());
        if (checkFearSituation()) {
            if (mObjNum == 2 && (m_type == 0 || m_type == 1)) {
                setAction(MODE_FIGHT);
            } else {
                setAction(MODE_FEAR);
            }
        }
    }
}

void daNpcCdn3_c::initTalk() {
    field_0xb68 = 0;
    m_targetAct.entry(dComIfGp_getLinkPlayer());
    field_0xb92 = shape_angle.y;
    field_0xb80 = mpMorf->getAnm();
    field_0xb7c = m_seqNum;
    JUT_ASSERT(658, NULL != m_targetAct.getActorP());
    if (!isInShop() && !isNoTurnTalk()) {
        m_seqNum = isChairStyle() ? 15 : 2;
        m_seqStep = 0;
        AnmNum anmNum = isChairStyle() ? CD2_ANM_14 : CD2_ANM_0;
        setAnm((J3DAnmTransformKey*)getAnmP(anmNum, mObjNum), 1.0f, 12.0f, 2, 0, -1);
    }
    field_0xb98 = 0;
}

void daNpcCdn3_c::executeTalk() {
    if (mpMorf->getAnm() == getAnmP(47,mObjNum)) {
        if (m_type == 17) {
            mCitizen.startCreatureVoiceLevel(Z2SE_WAD_A_MV_DANCE, -1);
        } else if (m_type == 2) {
            mCitizen.startCreatureVoiceLevel(Z2SE_MCN_A_V_DANCE, -1);
        } else if (m_type == 26) {
            mCitizen.startCreatureVoiceLevel(Z2SE_WCN_A_V_DANCE, -1);
        }
    }
    switch (field_0xb98) {
    case 0:
        if (mIsDarkWorld || isInShop() || isNoTurnTalk()) {
            field_0xb98++;
        } else {
            s16 actorAngle = fopAcM_searchActorAngleY(this, m_targetAct.getActorP());
            if (cLib_addCalcAngleS(&shape_angle.y, actorAngle, 4, 0x800, 0x10) == 0) {
                speedF = 0.0f;
                field_0xb98++;
            }
            setAngle();
        }
        break;
    case 1:
        if (dComIfGp_event_chkTalkXY()) {
            if (!dComIfGp_evmng_ChkPresentEnd()) {
                break;
            }
            field_0xb98++;
        } else {
            field_0xb98++;
        }
        mMsgFlow.init(this, mFlowNodeNum, 0, NULL);
    case 2:
        if (mMsgFlow.doFlow(this, NULL, 0)) {
            dComIfGp_event_reset();
            m_targetAct.remove();
            m_seqNum = field_0xb7c;
            if (field_0xb68 != 0) {
                setAction(mPrevMode);
            } else {
                if (!isInShop()) {
                    int iVar8 = 0;
                    if (isChairStyle()) {
                        iVar8 = 14;
                    }
                    setAnm((J3DAnmTransformKey*)getAnmP(iVar8, mObjNum), 1.0f, 12.0f, 2, 0, -1);
                }
                field_0xb98++;
            }
        } else {
            int msgAnm = ctrlMsgAnm();
            if (msgAnm != -1) {
                m_seqNum = msgAnm;
                m_seqStep = 0;
                field_0xb94 = 1;
            }
            JUT_ASSERT(726, m_funcTbl[m_seqNum][m_seqStep] != NULL);
            if ((this->*(m_funcTbl[m_seqNum][m_seqStep]))(NULL) != 0) {
                m_seqStep = (m_funcTbl[m_seqNum][m_seqStep + 1] == NULL) ? 0 : m_seqStep + 1;
                field_0xb94 = 1;
            }
        }
        break;
    case 3:
        if (cLib_addCalcAngleS(&shape_angle.y, field_0xb92, 4, 0x800, 0x10) == 0) {
            setAnm((J3DAnmTransformKey*)field_0xb80, 1.0f, 12.0f, 2, 0, -1);
            setAction(mPrevMode);
        }
        setAngle();
        return;
    default:
        JUT_ASSERT(741, FALSE);
    }
}

void daNpcCdn3_c::initEscape() {
    mEscapeTag = getEscapeTag();
    JUT_ASSERT(816, mEscapeTag != NULL);
    JUT_ASSERT(817, mEscapeTag->getPathID() != 0xff);
    m_path.setPath(mEscapeTag->getPathID(), fopAcM_GetRoomNo(this), 1, &current.pos, true);
    JUT_ASSERT(819, m_path.isPath());
    m_path.setNextPoint(current.pos);
    if (mObjNum == 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(41, mObjNum), 1.0f, 0.0f, 0, 0, -1);
        mCitizen.playVoice(1);
        field_0xb68 = 0;
    } else {
        setAnm((J3DAnmTransformKey*)getAnmP(4, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        mCitizen.playVoice(0);
        field_0xb68 = 1;
    }
    mFlowNodeNum = 0;
    dComIfGs_onSaveDunSwitch(0x3c);
}

daNpcCdn3_c::seqFunc* daNpcCdn3_c::m_funcTbl[44] = {
    m_seq00_funcTbl, m_seq01_funcTbl,
    m_seq02_funcTbl, m_seq03_funcTbl,
    m_seq04_funcTbl, m_seq05_funcTbl,
    m_seq06_funcTbl, m_seq07_funcTbl,
    m_seq08_funcTbl, m_seq09_funcTbl,
    m_seq10_funcTbl, m_seq11_funcTbl,
    m_seq12_funcTbl, m_seq13_funcTbl,
    m_seq14_funcTbl, m_seq15_funcTbl,
    m_seq16_funcTbl, m_seq17_funcTbl,
    m_seq18_funcTbl, m_seq19_funcTbl,
    m_seq20_funcTbl, m_seq21_funcTbl,
    m_seq22_funcTbl, m_seq23_funcTbl,
    m_seq24_funcTbl, m_seq25_funcTbl,
    m_seq26_funcTbl, m_seq27_funcTbl,
    m_seq28_funcTbl, m_seq29_funcTbl,
    m_seq30_funcTbl, m_seq31_funcTbl,
    m_seq32_funcTbl, m_seq33_funcTbl,
    m_seq34_funcTbl, m_seq35_funcTbl,
    m_seq36_funcTbl, m_seq37_funcTbl,
    m_seq38_funcTbl, m_seq39_funcTbl,
    m_seq40_funcTbl, m_seq41_funcTbl,
    m_seq42_funcTbl, m_seq43_funcTbl,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq00_funcTbl[2] = {&daNpcCdn3_c::walka, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq01_funcTbl[2] = {&daNpcCdn3_c::runa, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq02_funcTbl[2] = {&daNpcCdn3_c::waita, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq03_funcTbl[2] = {&daNpcCdn3_c::talka, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq04_funcTbl[2] = {&daNpcCdn3_c::runb, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq05_funcTbl[4] = {&daNpcCdn3_c::turnr, &daNpcCdn3_c::waita,
                                                        &daNpcCdn3_c::talka, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq06_funcTbl[4] = {&daNpcCdn3_c::turnl, &daNpcCdn3_c::waita,
                                                        &daNpcCdn3_c::talka, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq07_funcTbl[2] = {&daNpcCdn3_c::waitwall, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq08_funcTbl[7] = {
    &daNpcCdn3_c::talkwall,
    &daNpcCdn3_c::waitwall,
    &daNpcCdn3_c::talkbwall,
    &daNpcCdn3_c::waitwall,
    &daNpcCdn3_c::talkwall,
    &daNpcCdn3_c::talkbwall,
    NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq09_funcTbl[2] = {&daNpcCdn3_c::talkb, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq10_funcTbl[2] = {&daNpcCdn3_c::talkc, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq11_funcTbl[6] = {&daNpcCdn3_c::talka, &daNpcCdn3_c::waita,
                                                        &daNpcCdn3_c::talkb, &daNpcCdn3_c::waitb,
                                                        &daNpcCdn3_c::talkc, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq12_funcTbl[2] = {&daNpcCdn3_c::waitb, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq13_funcTbl[6] = {
    &daNpcCdn3_c::browsea, &daNpcCdn3_c::browseb, &daNpcCdn3_c::talka,
    &daNpcCdn3_c::talkb,   &daNpcCdn3_c::waita,   NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq14_funcTbl[2] = {&daNpcCdn3_c::walkb, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq15_funcTbl[2] = {&daNpcCdn3_c::sitwaita, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq16_funcTbl[7] = {
    &daNpcCdn3_c::sittalka,
    &daNpcCdn3_c::sitwaita,
    &daNpcCdn3_c::sittalkab,
    &daNpcCdn3_c::sitwaita,
    &daNpcCdn3_c::sittalka,
    &daNpcCdn3_c::sittalkab,
    NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq17_funcTbl[2] = {
    &daNpcCdn3_c::playm,
    NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq18_funcTbl[2] = {&daNpcCdn3_c::sitwaitb, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq19_funcTbl[7] = {
    &daNpcCdn3_c::sittalkb,
    &daNpcCdn3_c::sitwaitb,
    &daNpcCdn3_c::sittalkbb,
    &daNpcCdn3_c::sitwaitb,
    &daNpcCdn3_c::sittalkb,
    &daNpcCdn3_c::sittalkbb,
    NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq20_funcTbl[2] = {&daNpcCdn3_c::looka, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq21_funcTbl[2] = {&daNpcCdn3_c::lookb, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq22_funcTbl[4] = {&daNpcCdn3_c::looka, &daNpcCdn3_c::lookb,
                                                        &daNpcCdn3_c::waita, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq23_funcTbl[7] = {
    &daNpcCdn3_c::talka,
    &daNpcCdn3_c::waita,
    &daNpcCdn3_c::talkb,
    &daNpcCdn3_c::waita,
    &daNpcCdn3_c::talka,
    &daNpcCdn3_c::talkb,
    NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq24_funcTbl[5] = {
    &daNpcCdn3_c::talkb, &daNpcCdn3_c::talka, &daNpcCdn3_c::talkc, &daNpcCdn3_c::waita, NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq25_funcTbl[7] = {
    &daNpcCdn3_c::browsea,
    &daNpcCdn3_c::browseb,
    &daNpcCdn3_c::talka,
    &daNpcCdn3_c::talkb,
    &daNpcCdn3_c::talkc,
    &daNpcCdn3_c::waita,
    NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq26_funcTbl[3] = {
    &daNpcCdn3_c::browsea,
    &daNpcCdn3_c::browseb,
    NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq27_funcTbl[2] = {&daNpcCdn3_c::dance, NULL};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq28_funcTbl[3] = {
    &daNpcCdn3_c::ladytalka,
    &daNpcCdn3_c::normtalka,
    NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq29_funcTbl[3] = {
    &daNpcCdn3_c::ladytalkb,
    &daNpcCdn3_c::normtalkb,
    NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq30_funcTbl[6] = {
    &daNpcCdn3_c::talka, &daNpcCdn3_c::ladytalka, &daNpcCdn3_c::waita,
    &daNpcCdn3_c::talkb, &daNpcCdn3_c::waita,     NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq31_funcTbl[6] = {
    &daNpcCdn3_c::waita, &daNpcCdn3_c::ladytalkb, &daNpcCdn3_c::talka,
    &daNpcCdn3_c::waita, &daNpcCdn3_c::talkb,     NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq32_funcTbl[7] = {
    &daNpcCdn3_c::talka,
    &daNpcCdn3_c::ladytalka,
    &daNpcCdn3_c::waita,
    &daNpcCdn3_c::talkb,
    &daNpcCdn3_c::waita,
    &daNpcCdn3_c::normtalka,
    NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq33_funcTbl[7] = {
    &daNpcCdn3_c::waita,
    &daNpcCdn3_c::ladytalkb,
    &daNpcCdn3_c::talka,
    &daNpcCdn3_c::waita,
    &daNpcCdn3_c::talkb,
    &daNpcCdn3_c::normtalkb,
    NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq34_funcTbl[9] = {
    &daNpcCdn3_c::talka,     &daNpcCdn3_c::ladytalka, &daNpcCdn3_c::ladytalkb,
    &daNpcCdn3_c::waita,     &daNpcCdn3_c::talkb,     &daNpcCdn3_c::waita,
    &daNpcCdn3_c::normtalka, &daNpcCdn3_c::normtalkb, NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq35_funcTbl[2] = {
    &daNpcCdn3_c::sellme,
    NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq36_funcTbl[4] = {
    &daNpcCdn3_c::ladytalkb,
    &daNpcCdn3_c::talka,
    &daNpcCdn3_c::waita,
    NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq37_funcTbl[2] = {
    &daNpcCdn3_c::sing, NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq38_funcTbl[2] = {
    &daNpcCdn3_c::sittalka, NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq39_funcTbl[2] = {
    &daNpcCdn3_c::sittalkab, NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq40_funcTbl[3] = {
    &daNpcCdn3_c::sittalka, &daNpcCdn3_c::sittalkab, NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq41_funcTbl[2] = {
    &daNpcCdn3_c::sittalkb, NULL,
};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq42_funcTbl[2] = {
    &daNpcCdn3_c::sittalkbb, NULL,

};

daNpcCdn3_c::seqFunc daNpcCdn3_c::m_seq43_funcTbl[3] = {
    &daNpcCdn3_c::sittalkb, &daNpcCdn3_c::sittalkbb, NULL,
};

void daNpcCdn3_c::executeEscape() {
    if (field_0xb68 == 0) {
        if (mpMorf->isStop()) {
            setAnm((J3DAnmTransformKey*)getAnmP(4, mObjNum), 1.0f, 12.0f, 2, 0, -1);
            mCitizen.playVoice(0);
            field_0xb68 = 1;
        }
        cLib_addCalcAngleS2(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 3, 0x600);
    } else if (field_0xb68 == 1) {
        if (fopAcM_CheckCondition(this, 4)) {
            fopAcM_delete(this);
        }

        if (m_path.checkPoint(current.pos, speedF)) {
            if (m_path.checkPathEnd(current.pos, speedF)) {
                fopAcM_delete(this);
            }
            m_path.setNextPoint(current.pos);
        }
        m_path.checkObstacle(this);
        cXyz cStack_40;
        m_path.getTargetPoint(&cStack_40);
        cLib_addCalcAngleS2(&shape_angle.y, cLib_targetAngleY(&current.pos, &cStack_40), 3, 0x600);
        if (current.pos.y != old.pos.y) {
            s16 gndAngle = 0;
            cM3dGPla plane;
            bool triPla = dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &plane);
            if (triPla) {
                gndAngle = fopAcM_getPolygonAngle(&plane, shape_angle.y);
            }
            if (gndAngle != 0) {
                field_0xb64 = current.pos.y;
            }
        }
        setAngle();
        pathMoveF();
        mAcch.CrrPos(dComIfG_Bgsp());
        if (mAcch.ChkWallHit() && current.pos.abs(old.pos) < 2.0f) {
            field_0xb99++;
            if (field_0xb99 > 30) {
                setAction(MODE_FEAR2);
            }
        } else {
            field_0xb99 = 0;
        }
    }
}

void daNpcCdn3_c::initFear() {
    int a_anmNum = 0x26;
    if (mpMorf->getAnm() == getAnmP(14, mObjNum) ||
        mpMorf->getAnm() == getAnmP(15, mObjNum) ||
        mpMorf->getAnm() == getAnmP(16, mObjNum))
    {
        a_anmNum = 0x27;
    } else if (mpMorf->getAnm() == getAnmP(17, mObjNum) ||
               mpMorf->getAnm() == getAnmP(18, mObjNum) ||
               mpMorf->getAnm() == getAnmP(19, mObjNum))
    {
        a_anmNum = 0x28;
    }

    m_seqNum = a_anmNum;
    setAnm((J3DAnmTransformKey*)getAnmP(0x29, mObjNum), 1.0f, 0.0f, 0, 0, -1);
    field_0xb92 = fopAcM_searchPlayerAngleY(this);
    mFlowNodeNum = 0;
    speedF = 0.0f;
    dComIfGs_onSaveDunSwitch(0x3c);
    field_0xb68 = 0;
}

void daNpcCdn3_c::executeFear() {
    if (mpMorf->checkFrame(1.0f)) {
        mCitizen.playVoice(2);
    }
    if (mpMorf->isStop()) {
        mObjNum = 0;
        setAnm((J3DAnmTransformKey*)getAnmP(m_seqNum, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xb92 = cLib_targetAngleY(fopAcM_GetPosition_p(daPy_getPlayerActorClass()), &current.pos);
    }
    cLib_addCalcAngleS2(&shape_angle.y, field_0xb92, 3, 0x600);
}

void daNpcCdn3_c::initFear2() {
    mObjNum = 0;
    setAnm((J3DAnmTransformKey*)getAnmP(38, mObjNum), 1.0f, 12.0f, 2, 0, -1);
    field_0xb92 = cLib_targetAngleY(fopAcM_GetPosition_p(daPy_getPlayerActorClass()), &current.pos);
    mFlowNodeNum = 0;
    speedF = 0.0f;
    field_0xb68 = 2;
}

void daNpcCdn3_c::executeFear2() {
    if (mpMorf->checkFrame(1.0f)) {
        mCitizen.playVoice(2);
    }
    if (fopAcM_CheckCondition(this, 4)) {
        fopAcM_delete(this);
    }
    cLib_addCalcAngleS2(&shape_angle.y, field_0xb92, 3, 0x600);
}

void daNpcCdn3_c::initFight() {
    setAnm((J3DAnmTransformKey*)getAnmP(41, mObjNum), 1.0f, 0.0f, 0, 0, -1);
    mCitizen.playVoice(1);
    mFlowNodeNum = 0;
    speedF = 0;
    fopAcM_setCullSizeBox(this, -40.0f, -10.0f, -80.0f, 40.0f, 190.0f, 200.0f);
    dComIfGs_onSaveDunSwitch(0x3c);
    field_0xb68 = 0;
}

void daNpcCdn3_c::executeFight() {
    if (mpMorf->isStop()) {
        setAnm((J3DAnmTransformKey*)getAnmP(42, mObjNum), 1.0f, 12.0f, 2, 0, -1);
    }
    field_0xb92 = fopAcM_searchPlayerAngleY(this);
    if (mpMorf->getAnm() != getAnmP(41, mObjNum)) {
        if (mpMorf->checkFrame(1.0f)) {
            mCitizen.playVoice(2);
        }
        if (cLib_distanceAngleS(shape_angle.y, field_0xb92) > 0x600) {
            setAnm((J3DAnmTransformKey*)getAnmP(44, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        } else {
            setAnm((J3DAnmTransformKey*)getAnmP(42, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        }
    }
    cLib_addCalcAngleS2(&shape_angle.y, field_0xb92, 3, 0x600);
}

void daNpcCdn3_c::initRelief() {
    setAnm((J3DAnmTransformKey*)getAnmP(0, mObjNum), 1.0f, 12.0f, 2, 0, -1);
    speedF = 0.0f;
    field_0xb92 = shape_angle.y + 0x8000;
    field_0xb68 = 2;
}

void daNpcCdn3_c::executeRelief() {
    if (getGroupID() == 0xff && fopAcM_CheckCondition(this, 4)) {
        int time = getTime();
        field_0xb8c = time + field_0xb90;
        current.pos.set(m_path.getPoint(0));
        home.pos.set(current.pos);
        old.pos.set(current.pos);
        current.angle.y = (s16)cLib_targetAngleY(m_path.getPoint(0), m_path.getPoint(1));
        shape_angle.y = current.angle.y;
        home.angle.y = current.angle.y;
        old.angle.y = current.angle.y;
        m_path.setPath(getPathID(), fopAcM_GetRoomNo(this), 1, &current.pos, false);
        setAction(MODE_PATH);
        field_0xb95 = 1;
    }
    if (checkFearSituation()) {
        if (mObjNum == 2 && (m_type == 0 || m_type == 1)) {
            setAction(MODE_FIGHT);
        } else {
            setAction(MODE_FEAR);
        }
    }
    cLib_addCalcAngleS2(&shape_angle.y, field_0xb92, 3 + MREG_S(0), 0x600 + MREG_S(1));
}

void daNpcCdn3_c::setAngle() {
    current.angle.y = shape_angle.y;
}

void daNpcCdn3_c::pathMoveF() {
    f32 walkSpeed = 0.0f;
    f32 anmSpeed = 0.0f;
    cXyz* movePos = mStts.GetCCMoveP();
    if (field_0xb68 == 2) {
        walkSpeed = Cd2_HIO_walkMaxSpeed(m_type);
        anmSpeed = Cd2_HIO_walkAnmPlaySpeed(m_type);

        cLib_chaseF(&field_0xb5c, 0.8f, 0.05f);
        setSpeed(0.7f, walkSpeed, &field_0xb60, 0);
        setSpeed(0.7f, walkSpeed, &speedF, 1);
    } else if (field_0xb68 == 1) {
        walkSpeed = Cd2_HIO_maxSpeed(m_type);
        anmSpeed = Cd2_HIO_anmPlaySpeed(m_type);
        if (mMode == MODE_ESCAPSE) {
            walkSpeed *= 1.5f;
            anmSpeed *= 1.5f;
        }
        cLib_chaseF(&field_0xb5c, 1.0, 0.05f);
        setSpeed(0.7f, walkSpeed, &field_0xb60, 0);
        setSpeed(0.7f, walkSpeed, &speedF, 1);
    } else if (field_0xb68 == 0) {
        cLib_chaseF(&field_0xb5c, 0.0, 0.05f);
        cLib_chaseF(&field_0xb60, 0.0f, 1.5f);
        cLib_chaseF(&speedF, 0.0f, 1.5f);
    }
    cXyz targetPoint;
    m_path.getTargetPoint(&targetPoint);
    int angle = cLib_distanceAngleS(shape_angle.y, cLib_targetAngleY(&current.pos, &targetPoint));
    if (angle >= 0x2000) {
        cLib_chaseF(&field_0xb5c, 0.0f, 0.05f);
        cLib_chaseF(&field_0xb60, 0.0f, 1.2f);
        cLib_chaseF(&speedF, 0.0f, 1.2f);
    }
    fopAcM_posMoveF(this, movePos);
    f32 playSpeed;
    if (field_0xb68 != 0) {
        playSpeed = field_0xb60 / walkSpeed;
        if (playSpeed > 1.0f) {
            playSpeed = 1.0f;
        }
        playSpeed = anmSpeed * playSpeed;
        if (playSpeed < 0.6f) {
            playSpeed = 0.6f;
        }
        mpMorf->setPlaySpeed(playSpeed);
    }
}

int daNpcCdn3_c::ctrlMsgAnm() {
    int rv = -1;
    if (!eventInfo.checkCommandTalk()) {
        return rv;
    }
    if (dComIfGp_event_getTalkPartner() == this) {
        msg_class* msg_p = dMsgObject_c::getActor();
        JUT_ASSERT(1524, NULL != msg_p);
        if (msg_p->mode == 2 || msg_p->mode == 3) {
            mMsgIndex = -1;
        } else if (msg_p->mode == 6) {
            if (msg_p->msg_idx != mMsgIndex) {
                rv = dComIfGp_getMesgAnimeAttrInfo();
                mMsgIndex = msg_p->msg_idx;
            }
        } else {
            mMsgIndex = -1;
        }
    } else {
        mMsgIndex = -1;
    }
    return rv;
}

static void* s_sub1(void* param_1, void* param_2) {
    if (fopAcM_IsActor(param_1) && fopAcM_GetName(param_1) == PROC_Tag_Schedule &&
        static_cast<daNpcCdn3_c*>(param_2)->searchNextScheduleTagSub(
            static_cast<fopAc_ac_c*>(param_1)))
    {
        return param_1;
    }
    return NULL;
}

bool daNpcCdn3_c::searchNextScheduleTagSub(fopAc_ac_c* param_1) {
    daTagSchedule_c* schedule = (daTagSchedule_c*)param_1;
    if (getGroupID() == schedule->getGroupID()) {
        u8 uVar3 = mTagSched->getStartTime();
        int uVar3_2 = (u16)((uVar3 / 10) * 60 + (uVar3 % 10) * 10);
        u8 uVar4 = schedule->getStartTime();
        int uVar4a = (u16)((uVar4 / 10) * 60 + (uVar4 % 10) * 10);
        if (uVar4a <= uVar3_2) {
            uVar4a += 24 * 60;
        }
        if (uVar4a > uVar3_2) {
            if (field_0xb88 == NULL) {
                field_0xb88 = schedule;
            } else {
                u8 uVar5 = field_0xb88->getStartTime();
                int uVar5a = (u16)((uVar5 / 10) * 60 + (uVar5 % 10) * 10);
                if (uVar5a <= uVar3_2) {
                    uVar5a += 24 * 60;
                }
                if (uVar4a < uVar5a) {
                    field_0xb88 = schedule;
                }
            }
        }
    }
    return 0;
}

static void* s_sub2(void* param_1, void* param_2) {
    if (fopAcM_IsActor(param_1) && fopAcM_GetName(param_1) == PROC_Tag_Schedule &&
        static_cast<daNpcCdn3_c*>(param_2)->searchFirstScheduleTagSub(
            static_cast<fopAc_ac_c*>(param_1)))
    {
        return param_1;
    }
    return NULL;
}

bool daNpcCdn3_c::searchFirstScheduleTagSub(fopAc_ac_c* param_1) {
    daTagSchedule_c* schedule = (daTagSchedule_c*)param_1;
    if (getGroupID() == schedule->getGroupID()) {
        u8 uVar3 = schedule->getStartTime();
        int uVar3a = (u16)((uVar3 / 10) * 60 + (uVar3 % 10) * 10);
        int iVar1 = getTime();
        if (mTagSched == NULL) {
            mTagSched = schedule;
        } else {
            u8 uVar4 = mTagSched->getStartTime();
            u16 unusedShort1 = ((uVar4 / 10) * 60) + ((uVar4 % 10) * 10);
            if (uVar3a > iVar1) {
                if (uVar3a < uVar4) {
                    mTagSched = schedule;
                }
            } else if (uVar3a > uVar4) {
                mTagSched = schedule;
            }
        }
    }
    return 0;
}

void daNpcCdn3_c::checkSchedule() {
    if (getGroupID() != 0xff && field_0xb88 != NULL) {
        u8 rawStartTime = field_0xb88->getStartTime();
        int schedStartTime = (u16)((rawStartTime / 10) * 60 + (rawStartTime % 10) * 10);
        int currentTime = getTime();
#if PLATFORM_SHIELD
        if (field_0xb88->getWeekNum() == getDayOfWeek() && schedStartTime <= currentTime)
#else
        int currentDay = getDayOfWeek();
        int schedDay = field_0xb88->getWeekNum();
        if (schedDay == currentDay && schedStartTime <= currentTime)
#endif
        {
            setSchedule(field_0xb88);
            field_0xb88 = NULL;
            setAction(MODE_PATH);
        }
    }
}

void daNpcCdn3_c::setSchedule(daTagSchedule_c* param_1) {
    mTagSched = param_1;
    if (mTagSched->getPathID() != 0xff) {
        int iVar8;
        if (mTagSched->getStartEnd() == 0) {
            iVar8 = 1;
        } else {
            if (mTagSched->getStartEnd() == 1) {
                iVar8 = -1;
            } else {
                iVar8 = 1;
            }
        }
        m_path.setPath(mTagSched->getPathID(), fopAcM_GetRoomNo(this), iVar8,
                             &current.pos, false);
        if (iVar8 == -1) {
            m_path.setLastPointNum();
        }
    }
    u8 startTime = mTagSched->getStartTime();
    field_0xb8c = (startTime / 10) * 60 + (startTime % 10) * 10;
    m_seqNum = mTagSched->getSeqNum();
    mFlowNodeNum = mTagSched->getFlowNodeNum();
    if (m_seqNum < 0 || m_seqNum >= 44) {
        // "Castle town townspeople schedule tag setting value error
        //  Argument 1 motion sequence=%d is abnormal"
        OSReport_Error("城下町の町人 スケジュールタグ設定値異常 引数1モーションシーケンス=%dが異常です。\n", m_seqNum);
        JUT_ASSERT(1825, FALSE);
    }
    if (mTagSched->getStartEnd() == 0) {
        current.pos.set(m_path.getPoint(0));
        home.pos.set(current.pos);
        old.pos.set(current.pos);
        current.angle.y = (int)cLib_targetAngleY(m_path.getPoint(0), m_path.getPoint(1));
        shape_angle.y = current.angle.y;
        home.angle.y = current.angle.y;
        old.angle.y = current.angle.y;
        field_0xb95 = 1;
    }
    if (m_path.isPath()) {
        setAction(MODE_PATH);
    } else {
        setAction(MODE_WAIT);
    }
    const char* dayStr = "";
    switch (mTagSched->getWeekNum()) {
    case 0:
        dayStr = "日";
        break;
    case 1:
        dayStr = "月";
        break;
    case 2:
        dayStr = "火";
        break;
    case 3:
        dayStr = "水";
        break;
    case 4:
        dayStr = "木";
        break;
    case 5:
        dayStr = "金";
        break;
    case 6:
        dayStr = "土";
        break;
    default:
        JUT_ASSERT(1856, FALSE);
        break;
    }
    OS_REPORT("町人スケジュールセット\n");
    OS_REPORT("開始時刻: %s曜日 %d時 %d分\n", dayStr, startTime / 10, (startTime % 10) * 10);
    OS_REPORT("グループID: %d\n", mTagSched->getGroupID());
    OS_REPORT("レールID: %d\n", mTagSched->getPathID());
    OS_REPORT("モーションシーケンス: %d\n", m_seqNum);
    OS_REPORT("スタート、エンド: %d\n", mTagSched->getStartEnd());
    OS_REPORT("メッセージID: %d\n", mFlowNodeNum);
}

inline bool daNpcCdn3_c::isInShop() {
    u8 rv = FALSE;
    if (strcmp(dComIfGp_getStartStageName(), "R_SP160") == 0 && dComIfG_play_c::getLayerNo(0) <= 1) {
        rv = TRUE;
    }
    return rv;
}

bool daNpcCdn3_c::isChairStyle() {
    if (m_seqNum == 15 || m_seqNum == 16 || (m_seqNum == 17 && isM_()) ||
        m_seqNum == 38 || m_seqNum == 39 || m_seqNum == 40)
    {
        return true;
    }
    return false;
}

bool daNpcCdn3_c::isNoTurnTalk() {
    if (m_seqNum == 7 || m_seqNum == 8 || m_seqNum == 18 || m_seqNum == 19 ||
        m_seqNum == 41 || m_seqNum == 42 || m_seqNum == 43)
    {
        return true;
    }
    return false;
}

int daNpcCdn3_c::waita(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::talka(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(6, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::waitwall(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(11, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::talkwall(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(12, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::talkbwall(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(13, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::talkb(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(7, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::talkc(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(8, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::waitb(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(1, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::sitwaita(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(14, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::sittalka(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(15, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::sittalkab(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(16, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::sitwaitb(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(17, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::sittalkb(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(18, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::sittalkbb(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(19, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::looka(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(9, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::lookb(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(10, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::playm(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(34, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }

    if (isM_()) {
        if (getType() == 4) {
            mCitizen.startCreatureVoiceLevel(Z2SE_CSL_TOWN_BAND_GT_1, -1);
        } else {
            if (getType() == 5) {
                mCitizen.startCreatureVoiceLevel(Z2SE_CSL_TOWN_BAND_GT_2, -1);
            } else {
                if (getType() == 6) {
                    mCitizen.startCreatureVoiceLevel(Z2SE_CSL_TOWN_BAND_GT_3, -1);
                }
            }
        }
    } else {
        mCitizen.startCreatureVoiceLevel(Z2SE_CSL_TOWN_BAND_FLUTE, -1);
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::turnr(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xb92 = shape_angle.y - 0x4000;
        field_0xb94 = 0;
    }
    cLib_addCalcAngleS2(&shape_angle.y, field_0xb92, 4, 0x2000);
    if (abs(field_0xb92 - shape_angle.y) <= 4) {
        shape_angle.y = field_0xb92;
        return 1;
    }
    return 0;
}

int daNpcCdn3_c::turnl(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(0, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xb92 = shape_angle.y + 0x4000;
        field_0xb94 = 0;
    }
    cLib_addCalcAngleS2(&shape_angle.y, field_0xb92, 4, 0x2000);
    if (abs(field_0xb92 - shape_angle.y) <= 4) {
        shape_angle.y = field_0xb92;
        return 1;
    }
    return 0;
}

int daNpcCdn3_c::walka(void* param_0) {
    setAnm((J3DAnmTransformKey*)getAnmP(2, mObjNum), 1.0f, 12.0f, 2, 0, -1);
    field_0xb68 = 2;
    return 1;
}

int daNpcCdn3_c::walkb(void* param_0) {
    setAnm((J3DAnmTransformKey*)getAnmP(3, mObjNum), 1.0f, 12.0f, 2, 0, -1);
    field_0xb68 = 2;
    return 1;
}

int daNpcCdn3_c::runa(void* param_0) {
    setAnm((J3DAnmTransformKey*)getAnmP(4, mObjNum), 1.0f, 12.0f, 2, 0, -1);
    field_0xb68 = 1;
    return 1;
}

int daNpcCdn3_c::runb(void* param_0) {
    setAnm((J3DAnmTransformKey*)getAnmP(5, mObjNum), 1.0f, 12.0f, 2, 0, -1);
    field_0xb68 = 1;
    return 1;
}

int daNpcCdn3_c::browsea(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(20, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::browseb(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(21, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::dance(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(47, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    if (m_type == 17) {
        mCitizen.startCreatureVoiceLevel(Z2SE_WAD_A_MV_DANCE, -1);
    } else {
        if (m_type == 2) {
            mCitizen.startCreatureVoiceLevel(Z2SE_MCN_A_V_DANCE, -1);
        } else {
            if (m_type == 26) {
                mCitizen.startCreatureVoiceLevel(Z2SE_WCN_A_V_DANCE, -1);
            }
        }
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::ladytalka(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(48, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::ladytalkb(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(49, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::normtalka(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(50, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::normtalkb(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(51, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::sellme(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(52, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::sing(void* param_0) {
    if (!isStop()) {
        return 0;
    }

    if (field_0xb94 != 0) {
        setAnm((J3DAnmTransformKey*)getAnmP(53, mObjNum), 1.0f, 12.0f, 2, 0, -1);
        field_0xaa0 = 1;
        field_0xb94 = 0;
    }
    mCitizen.startCreatureVoiceLevel(Z2SE_CSL_TOWN_BAND_VOCAL, -1);
    return field_0xaa0 == 0;
}

int daNpcCdn3_c::create() {
    fopAcM_ct(this, daNpcCdn3_c);
    mIsDarkWorld = dKy_darkworld_check();
    m_type = getType();
    mObjNum = getObjNum();
    m_seqNum = getSeqNum();
    mFlowNodeNum = getFlowNodeNum();
    if (m_seqNum < 0 || 44 <= m_seqNum) {
        OSReport_Error("城下町の町人 マップツール設定値異常 引数1モーションシーケンス=%dが異常です。\n", m_seqNum);
        JUT_ASSERT(175, FALSE);
    }
    int rv = loadResrc(m_type, mObjNum);
    if (rv == cPhs_COMPLEATE_e) {
        int msb = 0x80000000;
        int heapSize = 0x2890;
        heapSize |= msb;
        if (fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize) == 0) {
            OS_REPORT("アクター生成失敗しました！\n");
            return cPhs_ERROR_e;
        }
        create_init();
    }
    return rv;
}

void daNpcCdn3_c::create_init() {
#if DEBUG
    l_Cd2_HIO.entryHIO("NPC(C級,D級)HighPoly");
#endif

    gravity = -3.0f;
    initTimeSchedule();
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -40.0f, -10.0f, -40.0f, 40.0f, 190.0f, 40.0f);
    mAcchCir.SetWall(Cd2_HIO_chkWallH(m_type), Cd2_HIO_chkWallR(m_type));
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
              fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    mAcch.SetRoofNone();
    mAcch.SetWaterNone();
    mAcch.CrrPos(dComIfG_Bgsp());
    setEnvTevCol();
    setRoomNo();
    initCollision();
    if (strcmp(dComIfGp_getStartStageName(), "F_SP116") == 0 &&
        dComIfGp_getStartStageRoomNo() == 3 && getType() == 7)
    {
        attention_info.distances[fopAc_attn_SPEAK_e] = 0xda;
        attention_info.distances[fopAc_attn_TALK_e] = 0xda;
    } else {
        attention_info.distances[fopAc_attn_SPEAK_e] = 0xd9;
        attention_info.distances[fopAc_attn_TALK_e] = 0xd9;
    }

    if (isInShop()) {
        fopAcM_OnStatus(this, 0x4000);
    }

    if (m_path.setPath(getPathID(), fopAcM_GetRoomNo(this), 1, &current.pos, false)) {
        setAction(MODE_PATH);
    } else {
        setAction(MODE_WAIT);
    }

    mpMorf->setMorf(0.0f);
    field_0xb64 = current.pos.y = mAcch.GetGroundH();
    setMtx();
}

void daNpcCdn3_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, field_0xb64, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    field_0xa9c = 0;
    mpMorf->modelCalc();
    setAttention(m_type);
    lookat();
    field_0xa9c = 1;
    mpMorf->modelCalc();
    setAttention(m_type);
}

void daNpcCdn3_c::lookat() {
    csXyz vecs1[3] = {
        csXyz(Cd2_HIO_maxLmtX_BACKBONE(m_type), Cd2_HIO_maxLmtY_BACKBONE(m_type), 0),
        csXyz(Cd2_HIO_maxLmtX_NECK(m_type), Cd2_HIO_maxLmtY_NECK(m_type), 0),
        csXyz(Cd2_HIO_maxLmtX_HEAD(m_type), Cd2_HIO_maxLmtY_HEAD(m_type), 0),
    };
    csXyz vecs2[3] = {
        csXyz(Cd2_HIO_minLmtX_BACKBONE(m_type), Cd2_HIO_minLmtY_BACKBONE(m_type), 0),
        csXyz(Cd2_HIO_minLmtX_NECK(m_type), Cd2_HIO_minLmtY_NECK(m_type), 0),
        csXyz(Cd2_HIO_minLmtX_HEAD(m_type), Cd2_HIO_minLmtY_HEAD(m_type), 0),
    };
    int local_68[4];
    cXyz cStack_80(cXyz::Zero);
    if (isM_()) {
        local_68[0] = 1;
        local_68[1] = 2;
        local_68[2] = 3;
        local_68[3] = -1;
    } else {
        local_68[0] = 1;
        local_68[1] = 2;
        local_68[2] = 3;
        local_68[3] = -1;
    }
    mLookat.init(mpMorf->getModel(), local_68, vecs1, vecs2);
    Mtx afStack_38;
    if (m_targetAct.getActorP() != NULL) {
        cMtx_copy(mpMorf->getModel()->getBaseTRMtx(), afStack_38);
        cStack_80 = m_targetAct.getActorP()->eyePos;
    } else {
        cMtx_copy(mpMorf->getModel()->getBaseTRMtx(), afStack_38);
        cStack_80 = cXyz::Zero;
    }
    mLookat.action(cStack_80, eyePos, this, afStack_38, NULL != m_targetAct.getActorP());
}

daNpcCdn3_c::~daNpcCdn3_c() {
    removeResrc(m_type, mObjNum);
    if (heap != NULL) {
        mpMorf->stopZelAnime();
    }

#if DEBUG
    l_Cd2_HIO.removeHIO(this);
#endif
}

int daNpcCdn3_c::execute() {
    mAcchCir.SetWallH(Cd2_HIO_chkWallH(m_type));
    mAcchCir.SetWallR(Cd2_HIO_chkWallR(m_type));
    checkTimeSchedule();
    if (field_0xb95 != 0) {
        fopAcM_OnStatus(this, 0x8000000);
    } else {
        fopAcM_OffStatus(this, 0x8000000);
    }
    if (field_0xb95 != 0) {
        if (!mIsDarkWorld && daPy_py_c::checkNowWolf()) {
            fopAcM_delete(this);
        }
        return 1;
    } else {
        if (getGroupID() == 0xff && mMode == MODE_WAIT && !isInShop() && !isOrchestra() &&
            fopAcM_CheckCondition(this, 4))
        {
            return 1;
        }
        if (!chkEvent()) {
            orderEvent();
        }
        callExecute();
        cLib_addCalc2(&field_0xb64, current.pos.y, 0.4f, 25.0f);
        animation(-1);
        setCollision();
        setMtx();
    }
    return 1;
}

inline bool daNpcCdn3_c::searchFirstScheduleTag() {
    field_0xb88 = NULL;
    base_process_class* unusedSearchResult = fpcM_Search(s_sub2, this);
    if (mTagSched != NULL) {
        mTagSched->setWeekNum(getDayOfWeek());
    }
    return mTagSched != NULL;
}

inline bool daNpcCdn3_c::searchNextScheduleTag() {
    if (mTagSched == NULL) {
        return 0;
    }
    field_0xb88 = 0;
    base_process_class* unusedSearchResult = fpcM_Search(s_sub1, this);
    if (field_0xb88 != 0) {
        u32 uVar2 = mTagSched->getWeekNum();
        if (field_0xb88->getStartTime() <= mTagSched->getStartTime()) {
            uVar2 = (uVar2 + 1) % 7;
        }
        field_0xb88->setWeekNum(uVar2);
    }
    return field_0xb88 != NULL;
}

void daNpcCdn3_c::checkTimeSchedule() {
    int iVar2 = getTime();
    if (getGroupID() == 0xff) {
        if (field_0xb96 == 0 && fopAcM_CheckCondition(this, 4)) {
            if (field_0xb8c <= iVar2 && field_0xb8e > iVar2) {
                field_0xb95 = 0;
            } else {
                field_0xb95 = 1;
            }
        }
    } else {
        if (mTagSched == NULL) {
            if (searchFirstScheduleTag()) {
                setSchedule(mTagSched);
                if (mTagSched->getStartEnd() == 1) {
                    if (field_0xb88 == 0) {
                        searchNextScheduleTag();
                    }
                    setSchedule(field_0xb88);
                    field_0xb88 = NULL;
                    field_0xb95 = 1;
                } else if (field_0xb8c < iVar2) {
                    current.pos.set(m_path.getPoint(m_path.getLastPointNum()));
                    home.pos.set(current.pos);
                    old.pos.set(current.pos);
                    current.angle.y =
                        (s16)cLib_targetAngleY(m_path.getPoint(m_path.getLastPointNum() - 1),
                                          m_path.getPoint(m_path.getLastPointNum()));
                    shape_angle.y = current.angle.y;
                    home.angle.y = current.angle.y;
                    old.angle.y = current.angle.y;
                    m_path.setLastPointNum();
                }
            }
        } else if (field_0xb95 != 0 && mTagSched->getStartEnd() != 1) {
#if PLATFORM_SHIELD
            if (mTagSched->getWeekNum() == getDayOfWeek() && field_0xb8c <= iVar2)
#else
            int day = getDayOfWeek();
            int weekNum = mTagSched->getWeekNum();
            if (weekNum == day && field_0xb8c <= iVar2)
#endif
            {
                field_0xb95 = 0;
            }
        }
        if (field_0xb88 == 0) {
            searchNextScheduleTag();
        }
    }
}

inline int daNpcCdn3_c::draw() {
    if (field_0xb95 != 0) {
        return 1;
    }
    drawNpc();
    drawObj(mObjNum, field_0xac8, Cd2_HIO_objScale(m_type));
    if (field_0xacc != NULL) {
        g_env_light.setLightTevColorType_MAJI(field_0xacc,
            &tevStr);
        dComIfGd_setListBG();
        mDoExt_modelUpdateDL(field_0xacc);
        dComIfGd_setList();
    }
    drawShadow(60.0f);
    mLookat.dbView();
    return 1;
}

static int daNpcCdn3_Create(void* i_this) {
    return static_cast<daNpcCdn3_c*>(i_this)->create();
}

static int daNpcCdn3_Delete(void* i_this) {
    static_cast<daNpcCdn3_c*>(i_this)->~daNpcCdn3_c();
    return 1;
}

static int daNpcCdn3_Execute(void* i_this) {
    return static_cast<daNpcCdn3_c*>(i_this)->execute();
}

static int daNpcCdn3_Draw(void* i_this) {
    return static_cast<daNpcCdn3_c*>(i_this)->draw();
}

static int daNpcCdn3_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daNpcCdn3_METHODS = {
    (process_method_func)daNpcCdn3_Create,
    (process_method_func)daNpcCdn3_Delete,
    (process_method_func)daNpcCdn3_Execute,
    (process_method_func)daNpcCdn3_IsDelete,
    (process_method_func)daNpcCdn3_Draw,
};

actor_process_profile_definition g_profile_NPC_CD3 = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_CD3,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpcCdn3_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  395,                    // mPriority
  &daNpcCdn3_METHODS,     // sub_method
  0x00040107,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
