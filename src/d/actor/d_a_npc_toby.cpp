/**
 * @file d_a_npc_toby.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_toby.h"
#include "Z2AudioLib/Z2Instances.h"
#if DEBUG
#include "JSystem/JHostIO/JORFile.h"
#endif
#include "d/actor/d_a_obj_automata.h"
#include "d/d_msg_object.h"
#include "d/actor/d_a_obj_scannon.h"

const daNpc_Toby_HIOParam daNpc_Toby_Param_c::m = {
    160.0f,
    -3.0f,
    1.0f,
    400.0f,
    255.0f,
    150.0f,
    35.0f,
    40.0f,
    0.0f,
    0.0f,
    10.0f,
    -10.0f,
    30.0f,
    -10.0f,
    45.0f,
    -45.0f,
    0.6f,
    12.0f,
    3,
    6,
    5,
    6,
    110.0f,
    0.0f,
    0.0f,
    0.0f,
    60,
    8,
    0,
    0,
    0,
    0,
    0,
    4.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    // actor-specific:
    4.0f,
    2.0f,
    2.0f,
    4.0f,
};

#if DEBUG
daNpc_Toby_HIO_c::daNpc_Toby_HIO_c() {
    m = daNpc_Toby_Param_c::m;
}

void daNpc_Toby_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    char msg_buffer[1988];

    JORReflexible::listenPropertyEvent(event);

    JORFile aJStack_910;
    int len;
    switch ((u32)event->id) {
    case 0x40000002:
        if (aJStack_910.open(6, "すべてのファイル(*.*)\0*.*\0", NULL, NULL, NULL) != 0) {
            memset(msg_buffer, 0, 2000);
            len = 0;
            daNpcT_cmnListenPropertyEvent(msg_buffer, &len, &m.common);
            // performance speed:
            sprintf(msg_buffer + len, "%d,   \t//  演奏速度\n", m.performance_speed);
            len = strlen(msg_buffer);
            // performance speed up:
            sprintf(msg_buffer + len, "%d,   \t//  演奏加速\n", m.speedup_performance);
            len = strlen(msg_buffer);
            // initial speed of performance:
            sprintf(msg_buffer + len, "%d,   \t//  演奏初速\n", m.init_play_speed);
            len = strlen(msg_buffer);
            // switching speed:
            sprintf(msg_buffer + len, "%d,   \t//  切り替え速度\n", m.switching_speed);
            len = strlen(msg_buffer);
            aJStack_910.writeData(msg_buffer, len);
            aJStack_910.close();
            OS_REPORT("write append success!::%6d\n", len);
        } else {
            OS_REPORT("write append failure!\n");
        }
        break;
    }
}

void daNpc_Toby_HIO_c::genMessage(JORMContext* ctext) {
    daNpcT_cmnGenMessage(ctext, &m.common);
    // Performance speed:
    ctext->genSlider("演奏速度        ", &m.performance_speed, 1.0f, 16.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // playing speed up:
    ctext->genSlider("演奏加速        ", &m.speedup_performance, 0.0f, 16.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // playing initial velocity:
    ctext->genSlider("演奏初速        ", &m.init_play_speed, 1.0f, 16.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // switching speed:
    ctext->genSlider("切り替え速度    ", &m.switching_speed, 0.0f, 16.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // export file:
    ctext->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

static int l_bmdData[3][2] = {
    {3, 1},
    {3, 2},
    {9, 6},
};

static daNpcT_evtData_c l_evtList[7] = {
    {"", 0},
    {"NO_RESPONSE", 0},
    {"TALKTO_ONESELF", 5},
    {"REPAIR_SCANNON", 4},
    {"SCANNON", 4},
    {"CONVERSATION_ABOUT_SCANNON", 4},
    {"CONVERSATION_ABOUT_ZRA", 4},
};

static char* l_resNameList[9] = {
    "",
    "Toby",
    "Toby_TW",
    "Toby0",
    "Toby1",
    "Toby2",
    "Toby3",
    "Toby4",
    "Toby5",
};

static s8 l_loadResPtrn0[6] = {1, 3, 4, 7, 8, -1};

static s8 l_loadResPtrn1[6] = {2, 3, 5, 7, 8, -1};

static s8 l_loadResPtrn2[5] = {1, 3, 6, 7, -1};

static s8 l_loadResPtrn3[8] = {1, 2, 3, 4, 5, 7, 8, -1};

static s8* l_loadResPtrnList[7] = {
    l_loadResPtrn0, l_loadResPtrn1, l_loadResPtrn1, l_loadResPtrn2,
    l_loadResPtrn2, l_loadResPtrn0, l_loadResPtrn3,
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[16] = {
    {-1, 0, 0, 13, 2, 3, 1},
    {5, 0, 3, 14, 0, 3, 0},
    {12, 0, 4, 29, 0, 4, 0},
    {14, 0, 4, 31, 0, 4, 0},
    {13, 0, 4, 30, 0, 4, 0},
    {6, 0, 4, 26, 0, 4, 0},
    {7, 2, 4, 27, 2, 4, 0},
    {8, 2, 4, 28, 2, 4, 0},
    {7, 0, 5, 21, 0, 5, 0},
    {5, 0, 7, 19, 0, 7, 0},
    {5, 0, 8, 18, 0, 8, 0},
    {6, 0, 5, 20, 0, 5, 0},
    {9, 2, 5, 13, 2, 3, 1},
    {6, 2, 7, 13, 2, 3, 1},
    {6, 2, 8, 19, 2, 8, 0},
    {8, 2, 5, 22, 2, 5, 0},
};

static daNpcT_motionAnmData_c l_motionAnmData[25] = {
    {7, 2, 3, 10, 0, 3, 1, 0},
    {15, 2, 4, 10, 0, 3, 1, 0},
    {20, 2, 4, 10, 0, 3, 1, 0},
    {16, 0, 4, 10, 0, 3, 1, 0},
    {17, 0, 4, 10, 0, 3, 1, 0},
    {19, 0, 4, 10, 0, 3, 1, 0},
    {18, 0, 4, 23, 0, 4, 0, 0},
    {9, 0, 4, 10, 0, 3, 1, 0},
    {10, 2, 4, 10, 0, 3, 1, 0},
    {11, 2, 4, 10, 0, 3, 1, 0},
    {10, 0, 5, 15, 0, 5, 0, 0},
    {12, 2, 5, 17, 2, 5, 0, 0},
    {10, 2, 7, 16, 2, 7, 0, 0},
    {9, 2, 7, 15, 2, 7, 0, 0},
    {7, 0, 7, 13, 0, 7, 0, 0},
    {8, 2, 7, 14, 2, 7, 0, 0},
    {11, 0, 5, 16, 0, 5, 0, 0},
    {9, 2, 8, 15, 2, 8, 0, 0},
    {7, 0, 8, 13, 0, 8, 0, 0},
    {8, 2, 8, 14, 2, 8, 0, 0},
    {6, 0, 3, 10, 0, 3, 1, 0},
    {4, 2, 6, 10, 0, 3, 0, 0},
    {5, 2, 6, 10, 0, 3, 0, 0},
    {6, 2, 6, 10, 0, 3, 0, 0},
    {10, 0, 8, 10, 0, 3, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[68] = {
    {1, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {8, -1, 1}, {15, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 1}, {13, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {10, -1, 1}, {12, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {11, -1, 1}, {14, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {2, -1, 1}, {3, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {6, 0, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, 0, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {4, 0, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {13, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {14, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {15, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {10, -1, 1}, {14, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[100] = {
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {1, -1, 1}, {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {12, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {13, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {14, -1, 1}, {15, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {16, -1, 1}, {17, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {18, -1, 1}, {19, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {15, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {17, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {19, -9, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {4, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {0, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {8, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {6, 0, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {20, 4, 1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {21, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {22, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {23, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {24, 2, 1}, {17, -1, 1}, {18, -1, 1}, {19, 0, 0},
};

enum NPC_TOBY_CUTS {
    CUT_NONE = 0,
    CUT_TOBY_HOUSE_FIRE = 1,
    CUT_TALKTO_ONESELF = 2,
    CUT_REPAIR_SCANNON = 3,
    CUT_SCANNON = 4,
    CUT_CONVERSATION_ABOUT_SCANNON = 5,
    CUT_CONVERSATION_ABOUT_ZRA = 6,
};

char* daNpc_Toby_c::mCutNameList[7] = {
    "",
    "TOBY_HOUSE_FIRE",
    "TALKTO_ONESELF",
    "REPAIR_SCANNON",
    "SCANNON",
    "CONVERSATION_ABOUT_SCANNON",
    "CONVERSATION_ABOUT_ZRA",
};

daNpc_Toby_c::cutFunc daNpc_Toby_c::mCutList[7] = {
    NULL,
    &daNpc_Toby_c::cutTobyHouseFire,
    &daNpc_Toby_c::cutTalkToOneself,
    &daNpc_Toby_c::cutRepairSCannon,
    &daNpc_Toby_c::cutSCannon,
    &daNpc_Toby_c::cutConversationAboutSCannon,
    &daNpc_Toby_c::cutConversationAboutZra,
};

NPC_TOBY_HIO_CLASS l_HIO;

daNpc_Toby_c::~daNpc_Toby_c() {
    OS_REPORT("|%06d:%x|daNpc_Toby_c -> デストラクト\n", g_Counter.mCounter0, this);
    if (mpMorf[0] != NULL) {
        mpMorf[0]->stopZelAnime();
    }

#if DEBUG
    if (mpHIO != NULL) {
        mpHIO->removeHIO();
    }
#endif

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

int daNpc_Toby_c::create() {
    static int const heapSize[7] = {
        0x3BC0, 0x3B10, 0x3B10, 0x43C0,
        0x0, 0x3BD0, 0x0,
    };
    daNpcT_ct(this, daNpc_Toby_c, l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4,
                       l_evtList, l_resNameList
    );

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = dKy_darkworld_check();
    int rv = loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) flowNo:%d, PathID:%02x, BitSW:%02x<%08x> ",
                  fopAcM_getProcNameString(this), mType, mFlowNodeNo, getPathID(),
                  getBitSW(), fopAcM_GetParam(this));
        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heapSize[mType])) {
            return cPhs_ERROR_e;
        }

        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 300.0f, 200.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        field_0x9c0.init(&mAcch, 0.0f, 0.0f);
#if DEBUG
        mpHIO = &l_HIO;
        // Toby:
        mpHIO->entryHIO("トビ－");
#endif
        reset();
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                        &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this),
                        fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mpHIO->m.common.weight, 0, this);
        mCyl1.Set(mCcDCyl);
        mCyl1.SetStts(&mCcStts);
        mCyl1.SetTgHitCallback(tgHitCallBack);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        if (mGroundH != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }

        mCreating = 1;
        Execute();
        mCreating = 0;
    }

    return rv;
}

int daNpc_Toby_c::CreateHeap() {
    static int const bmdTypeList[1] = {2};

    J3DModelData* modelData = NULL;
    J3DModel* model = NULL;
    int bmdIdx = mTwilight == true ? TRUE : FALSE;
    int res_name_idx = l_bmdData[bmdIdx][1];
    int my_bmd = l_bmdData[bmdIdx][0];
    modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_resNameList[res_name_idx], my_bmd));
    if (modelData == NULL) {
        return 0;
    }

    u32 sp_0x24 = 0x11020284;
    mpMorf[0] = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, sp_0x24);
    if (mpMorf[0] == NULL || mpMorf[0]->getModel() == NULL) {
        return 0;
    }

    model = mpMorf[0]->getModel();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    model->setUserArea((uintptr_t)this);
    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    if (mType == 3 || mType == 4) {
        for (int idx = 0; idx < 1; ++idx) {
            if (l_bmdData[bmdTypeList[idx]][0] >= 0) {
                modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[bmdTypeList[idx]][1]],
                                                                l_bmdData[bmdTypeList[idx]][0]);
            } else {
                modelData = NULL;
            }

            if (modelData != NULL) {
                mpTobyModels[idx] = mDoExt_J3DModel__create(modelData,  0x80000, 0x11000084);
            } else {
                mpTobyModels[idx] = NULL;
            }
        }
    }

    if (setFaceMotionAnm(1, false) && setMotionAnm(0, 0.0f, FALSE)) {
        return 1;
    }

    return 0;
}

int daNpc_Toby_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_Toby_c -> Delete\n", g_Counter.mCounter0, this);
    fopAcM_RegisterDeleteID(this, "NPC_TOBY");
    this->~daNpc_Toby_c();
    return 1;
}

int daNpc_Toby_c::Execute() {
    return daNpcT_c::execute();
}

int daNpc_Toby_c::Draw() {
    if (mpMatAnm[0] != NULL) {
        J3DModelData* modelData = mpMorf[0]->getModel()->getModelData();
        modelData->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    return daNpcT_c::draw(
#if DEBUG
        chkAction(&daNpc_Toby_c::test),
#else
        FALSE,
#endif
        FALSE, mRealShadowSize, NULL, 100.0f, 0, 0, 0
    );
}

int daNpc_Toby_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daNpc_Toby_c* a_this = static_cast<daNpc_Toby_c*>(i_this);
    return a_this->CreateHeap();
}

int daNpc_Toby_c::ctrlJointCallBack(J3DJoint* i_joint, int i_int) {
    if (i_int == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Toby_c* i_this = reinterpret_cast<daNpc_Toby_c*>(model->getUserArea());
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

void* daNpc_Toby_c::srchToby(void* i_actor, void* i_data) {
    if (mFindCount < 50) {
        fopAc_ac_c* actor_p = (fopAc_ac_c*)i_actor;
        if (actor_p != NULL && actor_p != i_data) {
            if (fopAcM_IsExecuting(fopAcM_GetID(actor_p)) && fopAcM_GetName(actor_p) == PROC_NPC_TOBY) {
                mFindActorPtrs[mFindCount] = actor_p;
                mFindCount++;
            }
        }
    }

    return NULL;
}

fopAc_ac_c* daNpc_Toby_c::getOtherTobyP(int i_int) {
    fopAc_ac_c* actor = NULL;
    f32 fVar1 = G_CM3D_F_INF;
    mFindCount = 0;

    fopAcM_Search(srchToby, this);

    for (int i = 0; i < mFindCount; i++) {
        if (i_int == (((daNpc_Toby_c*)mFindActorPtrs[i])->getType())
                      && fopAcM_searchActorDistance(this, mFindActorPtrs[i]) < fVar1) {
            fVar1 = fopAcM_searchActorDistance(this, mFindActorPtrs[i]);
            actor = mFindActorPtrs[i];
        }
    }

    return actor;
}

u8 daNpc_Toby_c::getType() {
    u8 prm = fopAcM_GetParam(this);
    switch (prm & 0xFF) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    case 4:
        return 4;
    case 5:
        return 5;
    default:
        return 6;
    }
}

int daNpc_Toby_c::isDelete() {
    // 0x054 -  M_051 - Fought the Shadow Bulbin riding the Shadow Kargorok (?)
    // 0x136 - F_0310 - Handed Auru's note to Fyer
    // 0x190 - F_0400 - Warped sky cannon to Lake Hylia
    // 0x1E4 - F_0484 - Completed sky canon repairs!
    // 0x1E5 - F_0485 - Heard Fyer talk about repairs 1 time
    switch (mType) {
        case 0:
            return FALSE;
        case 1:
            return daNpcT_chkEvtBit(0x54);
        case 2:
            return !daNpcT_chkEvtBit(0x54);
        case 3:
        case 4:
            return !daNpcT_chkEvtBit(0x136) || !daNpcT_chkEvtBit(0x190)
                 || daNpcT_chkEvtBit(0x1E4);
        case 5:
            return !daNpcT_chkEvtBit(0x136) || !daNpcT_chkEvtBit(0x190)
                 || daNpcT_chkEvtBit(0x1E4) || daNpcT_chkEvtBit(0x1E5);
        default:
            return FALSE;
    }
}

void daNpc_Toby_c::reset() {
    csXyz sp_0x10;
    int num_bytes = (u8*)&field_0x1004 - (u8*)&mActFunc_0;
    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    initialize();
    for (int i = 0; i < 4; i++) {
        mActorMngr[i].initialize();
    }

    memset(&mActFunc_0, 0, num_bytes);
    if (getPathID() != 0xFF) {
        mPath.initialize();
        mPath.setPathInfo(getPathID(), fopAcM_GetRoomNo(this), 0);
    }

    sp_0x10.setall(0);
    sp_0x10.y = home.angle.y;
    switch (mType) {
    case 0:
        eventInfo.setIdx(0);
        break;
    case 1:
        eventInfo.setIdx(0);
        break;
    case 2:
        eventInfo.setIdx(0);
        break;
    case 3:
        mHide = 1;
        eventInfo.setIdx(1);
        break;
    case 4:
        eventInfo.setIdx(2);
        break;
    case 5:
        mHide = 1;
        eventInfo.setIdx(3);
        break;
    }

    setAngle(sp_0x10);
}

void daNpc_Toby_c::afterJntAnm(int arg0) {
    if (arg0 == 1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (arg0 == 4) {
        mDoMtx_stack_c::YrotM(-mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(mStagger.getAngleX(0));
    }
}

void daNpc_Toby_c::setParam() {
    selectAction();
    srchActors();
    u32 uVar7 = (fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    s16 talk_dist = mpHIO->m.common.talk_distance;
    s16 talk_ang = mpHIO->m.common.talk_angle;
    s16 att_dist = mpHIO->m.common.attention_distance;
    s16 att_ang = mpHIO->m.common.attention_angle;

    if (mType == 0) {
        talk_dist = 2;
        talk_ang = 3;
        att_dist = 3;
        att_ang = 3;
    }

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(att_dist, att_ang);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_dist, talk_ang);

    attention_info.flags = uVar7;
    scale.set(mpHIO->m.common.scale, mpHIO->m.common.scale,
            mpHIO->m.common.scale);
    mCcStts.SetWeight(mpHIO->m.common.weight);
    mCylH = mpHIO->m.common.height;
    mWallR = mpHIO->m.common.width;
    mAttnFovY = mpHIO->m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    mRealShadowSize = mpHIO->m.common.real_shadow_size;
    if (&daNpc_Toby_c::play == mActFunc_0) {
        mRealShadowSize = 600.0f;
    }

    mExpressionMorfFrame = mpHIO->m.common.expression_morf_frame;
    mMorfFrames = mpHIO->m.common.morf_frame;
    gravity = mpHIO->m.common.gravity;
}

int daNpc_Toby_c::checkChangeEvt() {
    if (!chkAction(&daNpc_Toby_c::talk)) {
        mPreItemNo = 0;
        if (dComIfGp_event_chkTalkXY()) {
            if (dComIfGp_evmng_ChkPresentEnd()) {
                mPreItemNo = dComIfGp_event_getPreItemNo();
                if (mPreItemNo == fpcNm_ITEM_RAFRELS_MEMO) {
                    return FALSE;
                }

                mEvtNo = CUT_TOBY_HOUSE_FIRE;
                evtChange();
            }

            return TRUE;
        }

        switch (mType) {
        case 0:
            if (daNpcT_chkEvtBit(0x136)) {
                // Gave note to Fyer.
                if (daNpcT_chkEvtBit(0x190) && !daNpcT_chkEvtBit(0x1E5) && !daNpcT_chkEvtBit(0x1E4)) {
                    // Warped cannon but did not talk to Fyer.
                    mEvtNo = CUT_CONVERSATION_ABOUT_SCANNON;
                    evtChange();
                    return TRUE;
                }

                break;
            }

#if VERSION == VERSION_SHIELD_DEBUG
            if (chkAttnZra() && !dComIfGs_isSaveSwitch(0x51))
#else
            // 0x31E - F_0798 - Heard about Zora from Fyer
            if (chkAttnZra() && !daNpcT_chkEvtBit(0x31E) && !dComIfGs_isSaveSwitch(0x51))
#endif
            {
                mEvtNo = CUT_CONVERSATION_ABOUT_ZRA;
                evtChange();
                return TRUE;
            }

            break;

        case 1:
            // 0x0EF - T_0239 - Spoke with Fyer (start dark carge?)
            if (!daNpcT_chkEvtBit(0xEF)) {
                mEvtNo = CUT_TALKTO_ONESELF;
                evtChange();
                return TRUE;
            }

            break;

        case 2:
        case 3:
        case 4:
        case 5:
            break;
        }
    }

    return FALSE;
}

void daNpc_Toby_c::setAfterTalkMotion() {
    int iVar2 = 16;
    switch(mFaceMotionSeqMngr.getNo()) {
        case 1:
            iVar2 = 13;
            break;
        case 2:
            iVar2 = 10;
            break;
        case 3:
            iVar2 = 11;
            break;
        case 4:
            iVar2 = 12;
            break;
        case 5:
            iVar2 = 14;
            break;
    }

    mFaceMotionSeqMngr.setNo(iVar2, -1.0f, 0, 0);
}

void daNpc_Toby_c::srchActors() {
    switch (mType) {
    case 0:
        if (mActorMngr[0].getActorP() == NULL) {
            mActorMngr[0].entry(getNearestActorP(PROC_Obj_TobyHouse));
        }

        if (mActorMngr[1].getActorP() == NULL) {
            // Fyer's music box that he plays when Link gets shot from the cannon.
            mActorMngr[1].entry(getNearestActorP(PROC_OBJ_AUTOMATA));
        }

        if (mActorMngr[2].getActorP() == NULL) {
            mActorMngr[2].entry(getNearestActorP(PROC_Obj_SCannon));
        }

        break;

    case 1:
        if (mActorMngr[3].getActorP() == NULL) {
            // Shadow Bulbin (drained Lake Hylia Kargorok fight):
            mActorMngr[3].entry(getNearestActorP(PROC_E_RDY));
        }

        break;

    case 2:
        break;

    case 3:
        if (mActorMngr[2].getActorP() == NULL) {
            mActorMngr[2].entry(getNearestActorP(PROC_Obj_SCannon));
        }

        break;

    case 4:
    case 5:
        break;
    }
}

int daNpc_Toby_c::evtTalk() {
    if (chkAction(&daNpc_Toby_c::talk)) {
        (this->*mActFunc_1)(NULL);
    } else {
        setAction(&daNpc_Toby_c::talk);
    }

    return 1;
}

int daNpc_Toby_c::evtCutProc() {
    int staffId = dComIfGp_getEventManager().getMyStaffId("Toby", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int actIdx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 7, 0, 0);
        if ((this->*(mCutList[actIdx]))(mStaffId) != 0) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        return TRUE;
    } 

    return FALSE;
}

void daNpc_Toby_c::action() {
    fopAc_ac_c* actor_p = NULL;
    if (!mTwilight) {
        actor_p = hitChk(&mCyl1, -1);
    }

    if (actor_p != NULL && mCyl1.GetTgHitObj()->ChkAtType(AT_TYPE_THROW_OBJ)) {
        mStagger.setParam(this, actor_p, mCurAngle.y);
        setDamage(0, 16, 0);
        mDamageTimerStart = 0;
        mJntAnm.lookNone(1);
    }

    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = 1;
    }

    if (mActFunc_0 != NULL) {
        if (mActFunc_1 == mActFunc_0) {
            (this->*mActFunc_1)(NULL);
        } else {
            setAction(mActFunc_0);
        }
    }

    if (dComIfGp_event_runCheck() == FALSE && field_0x1001) {
        fopAcM_delete(this);
    }
}

void daNpc_Toby_c::beforeMove() {
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x8000000);
    if (checkHide()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x8000000);
    }

    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Toby_c::setAttnPos() {
    cXyz cStack_3c(0.0f, 30.0f, 0.0f);
    mStagger.calc(0);
    f32 dVar8 = cM_s2rad(mCurAngle.y - field_0xd7e.y);
    mJntAnm.setParam(this, mpMorf[0]->getModel(), &cStack_3c, getBackboneJointNo(), getNeckJointNo(),
        getHeadJointNo(), mpHIO->m.common.body_angleX_min, mpHIO->m.common.body_angleX_max,
        mpHIO->m.common.body_angleY_min, mpHIO->m.common.body_angleY_max,
        mpHIO->m.common.head_angleX_min, mpHIO->m.common.head_angleX_max,
        mpHIO->m.common.head_angleY_min, mpHIO->m.common.head_angleY_max,
        mpHIO->m.common.neck_rotation_ratio, dVar8, NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, dVar8);
    setMtx();
    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&cStack_3c, &eyePos);
    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, 1, 1.0f, 0);
    cStack_3c.set(0.0f, 0.0f, 20.0f);
    cStack_3c.y = mpHIO->m.common.attention_offset;
    if (chkAttnZra()) {
        cStack_3c.set(0.0f, 140.0f, 20.0f);
    } else if (mType == 1 && mMotionSeqMngr.getNo() == 11) {
        cStack_3c.set(0.0f, 120.0f, 40.0f);
    }

    mDoMtx_stack_c::YrotS(mCurAngle.y);
    mDoMtx_stack_c::multVec(&cStack_3c, &cStack_3c);
    attention_info.position = cStack_3c + current.pos;
    static cXyz prtclScl(1.0f, 1.0f, 1.0f);
    setFootPos();
    if (3.0f < speedF) {
        setFootPrtcl(&prtclScl, 10.0f, 0);
    }
}

void daNpc_Toby_c::setCollision() {
    cXyz cyl_center;
    if (mHide == 0) {
        int reg_r26 = 0x79;
        u32 tgType = 0xD8FBFDFF;
        u32 tgSPrm = 0x1f;
        if (mTwilight) {
            tgType = 0;
            tgSPrm = 0;
        } else if (mStagger.checkStagger()) {
            tgType = 0;
            tgSPrm = 0;
        }

        mCyl1.SetCoSPrm(reg_r26);
        mCyl1.SetTgType(tgType);
        mCyl1.SetTgSPrm(tgSPrm);
        mCyl1.OnTgNoHitMark();
        cyl_center.set(0.0f, 0.0f, 10.0f);
        f32 cylHeight = mCylH;
        f32 cylRadius = 20.0f + mWallR;
        if (chkAttnZra()) {
            cyl_center.set(0.0f, 0.0f, 30.0f);
            cylHeight = 110.0f;
            cylRadius = 64.0f;
        }

        mDoMtx_stack_c::YrotS(mCurAngle.y);
        mDoMtx_stack_c::multVec(&cyl_center, &cyl_center);
        cyl_center += current.pos;
        mCyl1.SetH(cylHeight);
        mCyl1.SetR(cylRadius);
        mCyl1.SetC(cyl_center);
        if (&daNpc_Toby_c::play != mActFunc_0) {
            dComIfG_Ccsp()->Set(&mCyl1);
        }
    }

    mCyl1.ClrCoHit();
    mCyl1.ClrTgHit();
}

int daNpc_Toby_c::drawDbgInfo() {
    return FALSE;
}

#if DEBUG
// I have 0 clue what these are about, but I saw them in npc_moir as well.
static s16 dummy_lit_122993(int sel) {
    const s16 arr[2] = {0x00C8, 0x00FF};
    return arr[sel];
}

static s16 dummy_lit_122996(int sel) {
    const s16 arr[2] = {0xC800, 0x00FF};
    return arr[sel];
}

static s16 dummy_lit_122999(int sel) {
    const s16 arr[2] = {0x8080, 0x80A0};
    return arr[sel];
}

static s16 dummy_lit_123002(int sel) {
    const s16 arr[2] = {0x8080, 0x80A0};
    return arr[sel];
}

static s16 dummy_lit_123016(int sel) {
    const s16 arr[2] = {0x4190, 0x0000};
    return arr[sel];
}

static s16 dummy_lit_123017(int sel) {
    const s16 arr[2] = {0x4110, 0x0000};
    return arr[sel];
}
#endif

void daNpc_Toby_c::drawOtherMdl() {
    J3DModel* model = mpMorf[0]->getModel();
    for (int i = 0; i < 1; i++) {
        if (mpTobyModels[i] != NULL && i == 0 && field_0x1002) {
            g_env_light.setLightTevColorType_MAJI(mpTobyModels[i], &tevStr);
            static int const jointNo[1] = {18};
            mDoMtx_stack_c::copy(model->getAnmMtx(jointNo[i]));
            Mtx jointAnmMtx;
            cMtx_copy(mDoMtx_stack_c::get(), jointAnmMtx);
            mpTobyModels[i]->setBaseTRMtx(jointAnmMtx);
            mDoExt_modelUpdateDL(mpTobyModels[i]);
            dComIfGd_addRealShadow(mShadowKey, mpTobyModels[i]);
        }
    }
}

bool daNpc_Toby_c::afterSetFaceMotionAnm(int arg0, int, f32, int) {
    // Amongst all the daNpcTs, seems like this is the only one that creates a
    // non-trivial definition for this func?
    switch (arg0) {
    case 4:
    case 5:
        mBckAnm.setPlaySpeed(0.0f);
        mBtpAnm.setPlaySpeed(0.0f);
        break;
    case 6:
        break;
    case 7:
        mAnmFlags &= ~(ANM_PAUSE_BCK | ANM_PAUSE_BTP);
        mBckAnm.setPlaySpeed(mPlaySpeed);
        mBtpAnm.setPlaySpeed(mPlaySpeed);
        break;
    }

    return true;
}

bool daNpc_Toby_c::afterSetMotionAnm(int arg0, int, f32, int) {
    switch (arg0) {
    case 6:
        mpMorf[0]->setPlaySpeed(0.0f);
        mBtkAnm.setPlaySpeed(0.0f);
        break;
    case 7:
        mpMorf[0]->setPlaySpeed(0.0f);
        break;
    case 8:
        mpMorf[0]->setPlaySpeed(mPlaySpeed);
        break;
    case 9:
        mAnmFlags &= 0xFFFFFFFE;
        mpMorf[0]->setPlaySpeed(mPlaySpeed);
        break;
    }

    field_0x0FFC = mMorfLoops;
    return true;
}

int daNpc_Toby_c::selectAction() {
    mActFunc_0 = NULL;
#if DEBUG
    if (mpHIO->m.common.debug_mode_ON) {
        mActFunc_0 = &daNpc_Toby_c::test;
        return 1;
    }
#endif

    switch (mType) {
    case 0:
        if (field_0x0FFF) {
            mActFunc_0 = &daNpc_Toby_c::play;
        } else if (field_0x0FFE == 0 && getBitSW() != 0xFF
                && dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
            mActFunc_0 = &daNpc_Toby_c::walk;
        } else {
            mActFunc_0 = &daNpc_Toby_c::wait;
        }

        break;

    default:
        mActFunc_0 = &daNpc_Toby_c::wait;
        break;
    }

    return 1;
}

int daNpc_Toby_c::chkAction(actionFunc i_func) {
    return mActFunc_1 == i_func;
}

int daNpc_Toby_c::setAction(actionFunc i_func) {
    mMode = MODE_EXIT;
    if (mActFunc_1 != NULL) {
        (this->*(mActFunc_1))(NULL);
    }

    mMode = MODE_ENTER;
    mActFunc_1 = i_func;
    if (mActFunc_1 != NULL) {
        (this->*(mActFunc_1))(NULL);
    }

    return 1;
}

int daNpc_Toby_c::cutTobyHouseFire(int arg0) {
    fopAc_ac_c* atmt_p = mActorMngr[1].getActorP();
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    int timer_val = 0;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer");
    if (param_p != NULL) {
        timer_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
        case 0:
            break;

        case 1:
            JUT_ASSERT(2331, NULL != atmt_p);
            field_0x0FFF = 1;
            setParam();
            setPos(atmt_p->current.pos);
            setAngle(atmt_p->shape_angle.y);
            speedF = 0.0f;
            speed.setall(0.0f);
            break;

        case 2:
            setPos(home.pos);
            setAngle(home.angle.y);
            if (getBitSW() != 0xFF) {
                dComIfGs_offSwitch(getBitSW(), fopAcM_GetRoomNo(this));
            }

            field_0x0FFE = 0;
            field_0x0FFF = 0;
            break;

        case 3:
            mpMorf[0]->setPlaySpeed(mPlaySpeed);
            mBtkAnm.setPlaySpeed(mPlaySpeed);
            mBckAnm.setPlaySpeed(mPlaySpeed);
            mBtpAnm.setPlaySpeed(mPlaySpeed);
            break;

        case 4:
            mEventTimer = timer_val;
            break;

        case 99:
            break;
        }
    }

    switch (prm_val) {
    case 0:
    case 1:
    case 2:
        action();
        retval = 1;
        break;

    case 3: 
        if (mMotionSeqMngr.getNo() != 19) {
            mpMorf[0]->setPlaySpeed(mPlaySpeed);
            mBckAnm.setPlaySpeed(mPlaySpeed);
            mBtpAnm.setPlaySpeed(mPlaySpeed);
            ((daObj_AutoMata_c*)atmt_p)->setAnmPlaySpeed(mpMorf[0]->getPlaySpeed());
            retval = 1;
        }

        action();
        break;

    case 4: 
        if (cLib_calcTimer(&mEventTimer) == 0) {
            retval = 1;
        }

        action();
        break;

    case 99:
        action();
        break;
    }

    mCurAngle.y = shape_angle.y;
    return retval;
}

int daNpc_Toby_c::cutTalkToOneself(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    int msgNo_val = 0;
    int timer_val = 0;
    int sp_0x10 = 0;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "msgNo");
    if (param_p != NULL) {
        msgNo_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer");
    if (param_p != NULL) {
        timer_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {                          /* switch 1 */
        case 0:
            daNpcT_offTmpBit(0xB);
            daNpcT_offTmpBit(0xC);
            initTalk(mFlowNodeNo, NULL);
            break;
        case 1:
            mFaceMotionSeqMngr.setNo(16, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            mEventTimer = timer_val;
            break;
        case 2:
            dComIfGs_sense_type_change_Set(1);
            initTalk(mFlowNodeNo, NULL);
            break;
        case 3:
            mEventTimer = timer_val;
            break;
        case 5:
        case 12:
            mEventTimer = timer_val;
            break;
        case 6:
            mEventTimer = timer_val;
            initTalk(mFlowNodeNo, NULL);
            break;
        case 7:
            field_0x0FF4 = 0;
            break;
        case 11:
            mFaceMotionSeqMngr.setNo(0xA, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(6, -1.0f, 0, 0);
            mEventTimer = timer_val;
            break;
        case 13:
            dComIfGs_sense_type_change_Set(-1);
            break;
        }
    }

    int sp18[2] = {-1, -1};
    switch (prm_val) {
    case 0:
        if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
            daNpcT_offEvtBit(0xEF);
            retval = 1;
        }

        break;

    case 1:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            retval = 1;
        }

        break;

    case 2:
        sp18[0] = msgNo_val;
        if (talkProc(sp18, 0, NULL, 0)) {
            if (msgNo_val) {
                retval = 1;
            } else if (mFlow.checkEndFlow()) {
                retval = 1;
            }
        }

        break;

    case 3:
    case 4:
    case 8:
        sp18[0] = msgNo_val;
        if (prm_val == 4 || prm_val == 8) {
            sp_0x10 = 1;
        }

        if (talkProc(sp18, sp_0x10, NULL, 0)) {
            if (msgNo_val) {
                retval = 1;
            } else if (mFlow.checkEndFlow()) {
                retval = 1;
            }
        }

        switch (dMsgObject_getMsgObjectClass()->getStatus()) {
        case 15:
        case 16:
            if (dMsgObject_isMsgSendControl() && prm_val == 3) {
                retval = 1;
            }
        }

        break;

    case 5:
    case 11:
    case 12:
        if (mEventTimer == 0) {
            if (prm_val == 5) {
                mDoAud_seStart(Z2SE_EN_RD_V_LAKE_APPEAR, &current.pos, 0, 0);
            }

            retval = 1;
        } else {
            cLib_calcTimer(&mEventTimer);
        }

        break;

    case 6:
    case 7:
        if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
            retval = 1;
        }

        switch (dMsgObject_getMsgObjectClass()->getStatus()) {
        case 15:
        case 16:
            if (dMsgObject_isMsgSendControl()) {
                if (prm_val == 6 && cLib_calcTimer(&mEventTimer) == 0) {
                    retval = 1;
                }

                if (prm_val == 7 && dComIfGp_getEventManager().getIsAddvance(arg0)) {
                    dMsgObject_onMsgSend();
                }
            }
        }

        if (!daNpcT_chkEvtBit(0xEF)) {
            daNpcT_onEvtBit(0xEF);
        }

        break;

    case 9:
        if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
            retval = 1;
        }

        break;

    case 10:
        if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
            retval = 1;
        }

        retval = 1;
        break;

    case 13:
        retval = 1;
        break;
    }

    return retval;
}

int daNpc_Toby_c::cutRepairSCannon(int arg0) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();
    fopAc_ac_c* scannon_p = mActorMngr[2].getActorP();
    cXyz work;
    csXyz angle;
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    int timer_val = 0;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer");
    if (param_p != NULL) {
        timer_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
        case 0:
            break;

        case 1:
            mFaceMotionSeqMngr.setNo(16, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 0, 0);
            mJntAnm.lookNone(1);
            setPos(home.pos);
            setAngle(home.angle.y);
            speedF = 0.0f;
            speed.setall(0.0f);
            mEventTimer = timer_val;
            if (daNpcT_getPlayerInfoFromPlayerList(1, fopAcM_GetRoomNo(this), &work, &angle)) {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&work, angle.y, 0);
            }

            JUT_ASSERT(2703, NULL != scannon_p);
            ((daSCannon_c*)scannon_p)->setCannonRepair();
            if (mEventTimer) {
                Z2GetAudioMgr()->bgmStreamPrepare(0x02000003);
                Z2GetAudioMgr()->bgmStreamPlay();
            }

            break;

        case 2:
            mEventTimer = timer_val;
            break;

        case 10:
            break;

        case 11:
            field_0x1002 = 1;
            break;
        }
    }

    int sp10[2] = {-1, -1};
    switch (prm_val) {                              /* switch 1; irregular */
    case 0:
        action();
        retval = 1;
        break;

    case 1:
        if (mEventTimer && cLib_calcTimer(&mEventTimer) == 0) {
            Z2GetAudioMgr()->bgmStreamStop(0x1E);
            dStage_changeScene(8, 0.0f, 0, fopAcM_GetRoomNo(this), 0, dComIfG_play_c::getLayerNo(0));
        }

        break;

    case 2:
        action();
        if (cLib_calcTimer(&mEventTimer) == 0) {
            retval = 1;
        }

        break;

    case 10:
        retval = 1;
        break;

    case 11:
        if (mPath.getPathInfo() != NULL) {
            if (cLib_calcTimer(&mEventTimer) == 0) {
                if (!dComIfGp_getEventManager().getIsAddvance(arg0)) {
                    mPath.setNextIdx();
                }

                switch (mPath.getArg1()) {
                case 0:
                    mHide = 0;
                    mMotionSeqMngr.setNo(0x15, 0.0f, 0, 0);
                    break;
                case 1:   
                    mHide = 0;
                    mMotionSeqMngr.setNo(0x16, 0.0f, 0, 0);
                    break;
                case 2:   
                    mHide = 0;
                    mMotionSeqMngr.setNo(0x17, 0.0f, 0, 0);
                    break;
                default:  
                    mHide = 1;
                    break;
                }

                current.pos = mPath.getPntPos(mPath.getIdx());
                old.pos = current.pos;
                setAngle(cM_deg2s(5.0f * f32(mPath.getArg0())));
                mEventTimer = mPath.getArg2();
            }
        } else if (!mHide) {
            mHide = 1;
        }

        mAcch.SetGrndNone();
        mAcch.SetWallNone();
        gravity = 0.0f;
        break;
    }

    if (prm_val == 1) {
        env_light->daytime = dComIfGs_getTime();
        env_light->daytime += 4.0f;
        if (360.0f <= u32(env_light->daytime)) {
            env_light->daytime = 0.0f;
        }

        dComIfGs_setTime(env_light->daytime);
    }

    return retval;
}

int daNpc_Toby_c::cutSCannon(int arg0) {
    cXyz work;
    csXyz my_svec;
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    int timer_val = 0;
    f32 reg_f31 = 0.0f;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer");
    if (param_p != NULL) {
        timer_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
        case 0:
            if (daNpcT_getPlayerInfoFromPlayerList(40, fopAcM_GetRoomNo(this), &work, &my_svec)) {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&work, my_svec.y, 0);
                dComIfGp_evmng_setGoal(&work);
            }

            break;

        case 1:
            initTalk(mFlowNodeNo, NULL);
            break;

        case 2:
            mMotionSeqMngr.setNo(15, 4.0f, 0, 0);
            reg_f31 = 1000.0f;
            work.set(0.0f, 100.0f, reg_f31);
            mDoMtx_stack_c::YrotS(home.angle.y + 0x1000);
            mDoMtx_stack_c::multVec(&work, &work);
            field_0x0FE8 = home.pos + work;
            mEventTimer = timer_val;
            break;

        case 3:
            mHide = 1;
            field_0x1001 = 1;
            break;

        case 4:
            mEventTimer = 0;
            break;

        case 5:
            mEventTimer = timer_val;
            break;
        }
    }

    int sp10[2] = {-1, -1};
    int sp8 = 0;
    switch (prm_val) {
    case 0:
        mJntAnm.lookPlayer(0);
        retval = 1;
        break;

    case 1:
        mJntAnm.lookPlayer(0);
        if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
            retval = 1;
        }

        break;

    case 2:
    case 4:
        mJntAnm.lookPlayer(0);
        if (prm_val == 4) {
            mJntAnm.lookNone(0);
        }

        if (1600.0f < current.pos.abs2(field_0x0FE8)) {
            cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &field_0x0FE8), 4, 0x400);
            shape_angle.y = current.angle.y;
            mCurAngle.y = shape_angle.y;
            cLib_chaseF(&speedF, 5.0f, 0.55f);
        } else {
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            speedF = 0.0f;
            speed.setall(0.0f);
            retval = 1;
        }

        if (cLib_calcTimer(&mEventTimer) == 0) {
            retval = 1;
        }

        break;

    case 5:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            retval = 1;
        }

        break;

    case 3:
        retval = 1;
        break;
    }

    return retval;
}

int daNpc_Toby_c::cutConversationAboutSCannon(int arg0) {
    fopAc_ac_c* actor_p = NULL;
    cXyz work;
    cXyz sp_0x5C;
    csXyz my_svec;
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    int timer_val = 0;
    int msgNo_val = 0;
    int send_val = 0;
    f32 reg_f31 = 0.0f;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer");
    if (param_p != NULL) {
        timer_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "msgNo");
    if (param_p != NULL) {
        msgNo_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "send");
    if (param_p != NULL) {
        send_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
        case 0:
            daNpcT_offTmpBit(0xB);
            daNpcT_offTmpBit(0xC);
            initTalk(mFlowNodeNo, NULL);
            break;

        case 1:
            mEventTimer = timer_val;
            break;

        case 2:
            initTalk(mFlowNodeNo, NULL);
            break;

        case 4:
            mFaceMotionSeqMngr.setNo(16, 0.0f, TRUE, 0);
            mMotionSeqMngr.setNo(15, 0.0f, TRUE, 0);
            reg_f31 = -300.0f;
            actor_p = getOtherTobyP(5);
            JUT_ASSERT(3042, NULL != actor_p);
            field_0x0FE8 = actor_p->current.pos;
            setAngle(actor_p->shape_angle.y);
            work.set(0.0f, 100.0f, reg_f31);
            mDoMtx_stack_c::YrotS(actor_p->shape_angle.y);
            mDoMtx_stack_c::multVec(&work, &work);
            work += actor_p->current.pos;
            setPos(work);
            if (daNpcT_getPlayerInfoFromPlayerList(55, fopAcM_GetRoomNo(this), &work, &my_svec)) {
                dComIfGp_evmng_setGoal(&work);
                sp_0x5C.set(0.0f, 100.0f, reg_f31);
                mDoMtx_stack_c::YrotS(my_svec.y);
                mDoMtx_stack_c::multVec(&sp_0x5C, &sp_0x5C);
                work += sp_0x5C;
                mGndChk.SetPos(&work);
                work.y = dComIfG_Bgsp().GroundCross(&mGndChk);
                JUT_ASSERT(3064, -G_CM3D_F_INF != work.y);
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&work, my_svec.y, 0);
            }

            initTalk(mFlowNodeNo, NULL);
            break;

        case 7:
            mPlayerAngle = fopAcM_searchPlayerAngleY(this);
            break;

        case 8:
            mMotionSeqMngr.setNo(15, 4.0f, 0, 0);
            reg_f31 = -1000.0f;
            actor_p = getOtherTobyP(5);
            JUT_ASSERT(3088, NULL != actor_p);
            work.set(0.0f, 100.0f, reg_f31);
            mDoMtx_stack_c::YrotS(actor_p->shape_angle.y);
            mDoMtx_stack_c::multVec(&work, &work);
            field_0x0FE8 = actor_p->current.pos + work;
            mEventTimer = timer_val;
            break;

        case 9:
            mFaceMotionSeqMngr.setNo(16, 0.0f, 1, 0);
            mMotionSeqMngr.setNo(0, 0.0f, 1, 0);
            setPos(home.pos);
            setAngle(home.angle.y);
            speedF = 0.0f;
            speed.setall(0.0f);
            break;

        case 10:
            mEventTimer = 0;
            break;

        case 20:
            mEventTimer = timer_val;
            break;
        }
    }

    int sp28[2] = {-1, -1};
    int sp10 = 0;

    switch (prm_val) {
    case 0:
        mJntAnm.lookPlayer(0);
        if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
            retval = 1;
        }

        break;

    case 1:
        actor_p = mActorMngr[2].getActorP();
        if (actor_p != NULL) {
            mJntAnm.lookPos(&actor_p->current.pos, 0);
            dComIfGp_getEvent().setPt2(actor_p);
        } else {
            mJntAnm.lookNone(0);
        }

        if (cLib_calcTimer(&mEventTimer) == 0) {
            retval = 1;
        }

        break;

    case 2:
        mJntAnm.lookPlayer(0);
        sp28[0] = msgNo_val;
        if (talkProc(sp28, 0, NULL, 0)) {
            if (msgNo_val) {
                retval = 1;
            } else if (mFlow.checkEndFlow()) {
                retval = 1;
            }
        }

        break;

    case 3:
        mJntAnm.lookPlayer(0);
        if (talkProc(NULL, 0, NULL, 0) && mFlow.checkEndFlow()) {
            retval = 1;
        }

        break;

    case 4:
        actor_p = mActorMngr[2].getActorP();
        if (actor_p != NULL) {
            field_0xd6c = actor_p->current.pos;
            field_0xd6c.y += 600.0f;
            mJntAnm.lookPos(&field_0xd6c, 0);
            dComIfGp_getEvent().setPt2(actor_p);
        } else {
            mJntAnm.lookNone(0);
        }
        /* fallthrough */
    case 8:
    case 10:
        mJntAnm.lookPlayer(0);
        if (prm_val == 10) {
            mJntAnm.lookNone(0);
        }

        if (1600.0f < current.pos.abs2(field_0x0FE8)) {
            cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &field_0x0FE8), 4, 0x400);
            shape_angle.y = current.angle.y;
            mCurAngle.y = shape_angle.y;
            cLib_chaseF(&speedF, 5.0f, 0.55f);
        } else {
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            speedF = 0.0f;
            speed.setall(0.0f);
            retval = 1;
        }

        if ((prm_val == 8 || prm_val == 10) && cLib_calcTimer(&mEventTimer) == 0) {
            retval = 1;
        }

        break;

    case 5:
    case 6:
        sp28[0] = msgNo_val;
        field_0xe26 = 0;
        if (talkProc(sp28, send_val, NULL, 0)) {
            if (msgNo_val) {
                retval = 1;
            } else if (mFlow.checkEndFlow()) {
                retval = 1;
            }
        }

        break;

    case 7:
        mJntAnm.lookPlayer(0);
        if (mPlayerAngle != mCurAngle.y) {
            step(mPlayerAngle, -1, -1, 15, 0);
        }

        shape_angle.y = mPlayerAngle;
        if (talkProc(NULL, 0, NULL, 0)) {
            int itemNo;
            int eventId = mFlow.getEventId(&itemNo);
            switch (eventId) {
            case 19:
                mEvtNo = CUT_REPAIR_SCANNON;
                evtChange();
                break;
            default:
                if (mFlow.checkEndFlow()) {
                    retval = 1;
                }
            }
        }

        break;

    case 20:
        if (cLib_calcTimer(&mEventTimer) == 0) {
            retval = 1;
        }

        break;
    }

    return retval;
}

int daNpc_Toby_c::cutConversationAboutZra(int arg0) {
    int retval = 0;
    int* param_p = NULL;
    int prm_val = -1;
    int timer_val = 0; // unused.
    int msgNo_val = 0;
    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm");
    if (param_p != NULL) {
        prm_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "timer");
    if (param_p != NULL) {
        timer_val = *param_p;
    }

    param_p = dComIfGp_evmng_getMyIntegerP(arg0, "msgNo");
    if (param_p != NULL) {
        msgNo_val = *param_p;
    }

    if (dComIfGp_getEventManager().getIsAddvance(arg0)) {
        switch (prm_val) {
        case 0:
            initTalk(mFlowNodeNo, NULL);
            break;
        case 1:
            break;
        }
    }

    int sp8[2] = {-1, -1};
    switch (prm_val) {
    case 0:
    case 1:
        mJntAnm.lookPlayer(0);
        if (prm_val == 0) {
            mJntAnm.lookNone(0);
        }

        sp8[0] = msgNo_val;
        if (talkProc(sp8, 0, NULL, 0)) {
            if (msgNo_val) {
                retval = 1;
            } else if (mFlow.checkEndFlow()) {
                retval = 1;
            }
        }

        break;
    }

    return retval;
}

int daNpc_Toby_c::chkAttnZra() {
    // In practice, get Bit SW returns 0xEC for all mType == 0, otherwise 0xFF.
    // 0x0A1 - F_0063 - Lake Hylia - Used Fyer's cannon for first time
    return (mType == 0 && !daNpcT_chkEvtBit(0xA1) && getBitSW() != 0xFF
            && !dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this)));
}

int daNpc_Toby_c::wait(void*) {
    switch (mMode) {
    case 0:
    case 1:
        if (mStagger.checkStagger() == 0) {
            if (field_0x0FF4 == 0) {
                if (mType == 1 && daNpcT_chkEvtBit(0xEF)) {
                    mFaceMotionSeqMngr.setNo(12, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(11, -1.0f, 0, 0);
                } else if (chkAttnZra()) {
                    mFaceMotionSeqMngr.setNo(16, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(6, -1.0f, 0, 0);
                } else {
                    mFaceMotionSeqMngr.setNo(16, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                }
            }

            mMode = 2;
        }
        /* fallthrough */
    case 2:
        if (mType == 4 && mHide == 0) {
            mEvtNo = CUT_SCANNON;
            field_0xe33 = true;
        }

        if (mStagger.checkStagger()) {
            break;
        }

        if (field_0x0FF4) {
            if (cLib_calcTimer(&field_0x0FF4) == 0) {
                mMode = 1;
            }
        }

        if (mTwilight == false && mHide == 0 && daPy_py_c::checkNowWolf()) {
            cXyz att_pos = getAttnPos(daPy_getPlayerActorClass());
            if (field_0x1000) {
                if (chkPointInArea(att_pos, attention_info.position,
                                   2000.0f, 300.0f, -300.0f, mCurAngle.y) == FALSE) {
                    mFaceMotionSeqMngr.setNo(16, -1.0f, 0, 0);
                    mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
                    field_0x1000 = 0;
                } else if (mMotionSeqMngr.getNo() == 24) {
                    if (mMotionSeqMngr.getStepNo() < 2) {
                        cLib_chaseS(&current.angle.y, fopAcM_searchPlayerAngleY(this), 0x1000);
                        shape_angle.y = current.angle.y;
                        mCurAngle.y = shape_angle.y;
                    } else if (mMotionSeqMngr.getStepNo() > 2) {
                        mSound.startCreatureVoiceLevel(Z2SE_TOBY_V_BURUBURU, -1);
                    }
                }
            } else {
                if (chkPointInArea(att_pos, attention_info.position, 1000.0f, 300.0f, -300.0f, mCurAngle.y)) {
                    if (chkActorInSight(daPy_getPlayerActorClass(), 90.0f, mCurAngle.y)) {
                        mFaceMotionSeqMngr.setNo(15, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(24, -1.0f, 0, 0);
                        mSound.startCreatureVoice(Z2SE_TOBY_V_SURPRISE, -1);
                        field_0x1000 = 1;
                    }
                }
            }
        } else if (field_0x1000) {
            mFaceMotionSeqMngr.setNo(16, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0, -1.0f, 0, 0);
            field_0x1000 = 0;
        }

        if (field_0x1000) {
            attention_info.flags = 0;
        } else {
            if (chkAttnZra()) {
                mPlayerActorMngr.remove();
            }

            if (mPlayerActorMngr.getActorP() != NULL && mTwilight == false) {
                int reg_r28 = 1;
                mJntAnm.lookPlayer(0);
                if (chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y) == FALSE) {
                    mJntAnm.lookNone(0);
                    reg_r28 = 0;
                }

                if (mType == 0 && reg_r28 == 0) {
                    mPlayerActorMngr.remove();
                    if (home.angle.y == mCurAngle.y) {
                        mMode = 1;
                    }
                } else if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                    mMode = 1;
                }
            } else {
                mJntAnm.lookNone(0);
                if (home.angle.y != mCurAngle.y) {
                    if (field_0xe34) {
                        if (step(home.angle.y, 0x10, 0x14, 0xF, 0)) {
                            mMode = 1;
                        }
                    } else {
                        setAngle(home.angle.y);
                        mMode = 1;
                    }

                    attention_info.flags = 0;
                } else {
                    srchPlayerActor();
                }
            }

            switch (mJntAnm.getMode()) {
            case 0:
                break;
            }
        }

        break;

    case 3:
        break;
    }

    return 1;
}

int daNpc_Toby_c::walk(void*) {
    switch (mMode) {
    case 0:
    case 1:
        if (mStagger.checkStagger() == 0) {
            mFaceMotionSeqMngr.setNo(0x10, -1.0f, 0, 0);
            mMotionSeqMngr.setNo(0xF, -1.0f, 0, 0);
            mAcch.SetWallNone();
            mMode = 2;
        }
        /* fallthrough */
    case 2:
        if (mStagger.checkStagger() == 0) {
            cXyz work;
            JUT_ASSERT(3620, NULL != mPath.getPathInfo());
            int dst_pos_h = mPath.getDstPosH(current.pos, &work, mPath.getNumPnts(), 2);
            calcSpeedAndAngle(work, dst_pos_h, 4, 0x800);
            if (dst_pos_h && cM3d_IsZero(speedF)) {
                home.angle.y = 0x4000;
                if (home.angle.y == mCurAngle.y) {
                    mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                    field_0x0FFE = 1;
                } else {
                    step(home.angle.y, 16, 20, 15, 0);
                }
            }

            mJntAnm.lookNone(0);
        }

        attention_info.flags = 0;
        break;

    case 3:
        mAcch.ClrWallNone();
        break;
    }

    return 1;
}

int daNpc_Toby_c::play(void* param_0) {
    daObj_AutoMata_c* actor_p = (daObj_AutoMata_c*) mActorMngr[1].getActorP();
    int reg_r29 = 1;
    if (actor_p != NULL) {
        actor_p->setCoNone();
    }

    switch (mMode) {
    case 0:
    case 1:
        if (mStagger.checkStagger() == 0) {
            mFaceMotionSeqMngr.setNo(9, 0.0f, 0, 0);
            mMotionSeqMngr.setNo(19, 0.0f, 0, 0);
            mSound.startCreatureVoice(Z2SE_TOBY_V_BOOT, -1);
            mPlaySpeed = 1.0f;
            if (actor_p != NULL) {
                actor_p->setAnm(0, 1);
                actor_p->setAnmPlaySpeed(0.0f);
                reg_r29 = 0;
            }
            mMode = 2;
        }

    case 2:
        if (mStagger.checkStagger() == 0) {
            if (mMotionSeqMngr.getNo() == 19) {
                if (mMotionSeqMngr.checkEndSequence()) {
                    mFaceMotionSeqMngr.setNo(6, 0.0f, 0, 0);
                    mMotionSeqMngr.setNo(16, 0.0f, 0, 0);
                    field_0x0FFC = mMorfLoops;
                    mPlaySpeed = 1.0f;
                }

                reg_r29 = 0;
            } else if (mMotionSeqMngr.getNo() == 16) {
                if (mMotionSeqMngr.checkEndSequence()) {
                    mFaceMotionSeqMngr.setNo(7, 0.0f, 0, 0);
                    mMotionSeqMngr.setNo(17, 0.0f, 0, 0);
                    field_0x0FFC = mMorfLoops;
                    mPlaySpeed = mpHIO->m.init_play_speed;
                    if (actor_p != NULL) {
                        actor_p->setAnm(1, 1);
                        actor_p->setAnmPlaySpeed(0.0f);
                        reg_r29 = 0;
                    }
                }
            } else if (field_0x0FFC != mMorfLoops) {
                cLib_chaseF(&mPlaySpeed, mpHIO->m.performance_speed, mpHIO->m.speedup_performance);
                if (mMotionSeqMngr.getNo() != 0x12) {
                    if (mpHIO->m.switching_speed <= mPlaySpeed) {
                        mFaceMotionSeqMngr.setNo(8, -1.0f, 0, 0);
                        mMotionSeqMngr.setNo(18, -1.0f, 0, 0);
                    }
                }

                mpMorf[0]->setPlaySpeed(mPlaySpeed);
                mBckAnm.setPlaySpeed(mPlaySpeed);
                mBtpAnm.setPlaySpeed(mPlaySpeed);
            }

            if (actor_p != NULL && reg_r29) {
                actor_p->setAnmPlaySpeed(mpMorf[0]->getPlaySpeed());
            }

            field_0x0FFC = mMorfLoops;
            mJntAnm.lookNone(0);
        }

        attention_info.flags = 0;
        break;

    case 3:
        break;
    }

    return 1;
}

int daNpc_Toby_c::talk(void*) {
    switch (mMode) {
    case 0:
    case 1:
        if (mStagger.checkStagger() == 0) {
#if VERSION != VERSION_SHIELD_DEBUG
            if (chkAttnZra()) {
                mFaceMotionSeqMngr.setNo(16, -1.0f, TRUE, mFaceMotionSeqMngr.getOffset());
                mMotionSeqMngr.setNo(20, -1.0f, TRUE, mMotionSeqMngr.getOffset());
            }
#endif

            if (mPreItemNo == fpcNm_ITEM_RAFRELS_MEMO) {
                initTalk(2, NULL);
            } else {
                initTalk(mFlowNodeNo, NULL);
            }

            field_0x0FF4 = 0;
            mMode = 2;
        }
        /* fallthrough */
    case 2:
        if (mStagger.checkStagger() == 0) {
            if (mTwilight || mPlayerAngle == mCurAngle.y) {
                int spC[3] = {-1, -1, -1};
                if (mPreItemNo == fpcNm_ITEM_RAFRELS_MEMO) {
                    spC[0] = 0x1420;
                    spC[1] = 0x142D;
                }

                if (talkProc(spC, FALSE, NULL, FALSE)) {
                    int sp8;
                    int evt_id = mFlow.getEventId(&sp8);
                    switch (evt_id) {
                    case 19:
                        mEvtNo = CUT_REPAIR_SCANNON;
                        evtChange();
                        break;

                    default:
                        if (mFlow.checkEndFlow()) {
                            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
                            dComIfGp_event_reset();
                            mMode = 3;
                        } else {
                            daPy_getPlayerActorClass()->changeOriginalDemo();
                            daPy_getPlayerActorClass()->changeDemoMode(6, 0, 1, 0);
                        }
                    }
                }

                mJntAnm.lookPlayer(0);
                if (mTwilight) {
                    mJntAnm.lookNone(0);
                }
            } else {
                mJntAnm.lookPlayer(0);
                step(mPlayerAngle, 16, 20, 15, 0);
            }
        }

        break;

    case 3:
        break;
    }

    return 0;
}

int daNpc_Toby_c::test(void* param_0) {
    // TODO
}

static int daNpc_Toby_Create(void* i_this) {
    return static_cast<daNpc_Toby_c*>(i_this)->create();
}

static int daNpc_Toby_Delete(void* i_this) {
    return static_cast<daNpc_Toby_c*>(i_this)->Delete();
}

static int daNpc_Toby_Execute(void* i_this) {
    return static_cast<daNpc_Toby_c*>(i_this)->Execute();
}

static int daNpc_Toby_Draw(void* i_this) {
    return static_cast<daNpc_Toby_c*>(i_this)->Draw();
}

static int daNpc_Toby_IsDelete(void* i_this) {
    return true;
}

static actor_method_class daNpc_Toby_MethodTable = {
    (process_method_func)daNpc_Toby_Create,
    (process_method_func)daNpc_Toby_Delete,
    (process_method_func)daNpc_Toby_Execute,
    (process_method_func)daNpc_Toby_IsDelete,
    (process_method_func)daNpc_Toby_Draw,
};

extern actor_process_profile_definition g_profile_NPC_TOBY = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_NPC_TOBY,           // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daNpc_Toby_c),    // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  377,                     // mPriority
  &daNpc_Toby_MethodTable, // sub_method
  0x00040107,              // mStatus
  fopAc_NPC_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES;
