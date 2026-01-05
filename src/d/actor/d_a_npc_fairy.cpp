/**
 * @file d_a_npc_fairy.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_fairy.h"
#include "d/actor/d_a_npc4.h"
#include "d/d_s_play.h"
#include "JSystem/JHostIO/JORFile.h"

enum fairy_RES_File_ID {
    /* BCK */
    /* 0x09 */ BCK_FAIRY_F_SAD = 0x9,
    /* 0x0A */ BCK_FAIRY_F_SMILE,
    /* 0x0B */ BCK_FAIRY_F_TALK,
    /* 0x0C */ BCK_FAIRY_FH_SMILE,
    /* 0x0D */ BCK_FAIRY_GO,
    /* 0x0E */ BCK_FAIRY_GOWAIT,

    /* Wing BCKs | "Hane" = Feather */
    /* 0x0F */ BCK_FAIRY_HANE_GETUP,
    /* 0x10 */ BCK_FAIRY_HANE_NON,
    /* 0x11 */ BCK_FAIRY_HANE_SITWAIT,
    /* 0x12 */ BCK_FAIRY_HANE_SYUTU,
    /* 0x13 */ BCK_FAIRY_HANE_WAIT,
    /* 0x14 */ BCK_FAIRY_HANE_WALK,
    /* ---------------------------- */

    /* 0x15 */ BCK_FAIRY_HOPPE,
    /* 0x16 */ BCK_FAIRY_HOPPE_WAIT,
    /* 0x17 */ BCK_FAIRY_SIT_A,
    /* 0x18 */ BCK_FAIRY_SIT_B,
    /* 0x19 */ BCK_FAIRY_SIT_GO,
    /* 0x1A */ BCK_FAIRY_SIT_GOWAIT,
    /* 0x1B */ BCK_FAIRY_SITTALK,
    /* 0x1C */ BCK_FAIRY_STANDUP,
    /* 0x1D */ BCK_FAIRY_WAIT,
    /* 0x1E */ BCK_FAIRY_WALK,

    /* BMDV */
    /* 0x21 */ BMDV_FAIRY = 0x21,
    /* 0x22 */ BMDV_FAIRY_FEATHER_GT,

    /* BPK */
    /* 0x25 */ BPK_FAIRY = 0x25,

    /* BRK */
    /* 0x28 */ BRK_FAIRY = 0x28,
    /* 0x29 */ BRK_FAIRY_FEATHER_GT,

    /* BTK */
    /* 0x2C */ BTK_FAIRY = 0x2C,
    /* 0x2D */ BTK_FAIRY_FEATHER_GT,

    /* BTP */
    /* 0x30 */ BTP_FAIRY = 0x30,
    /* 0x31 */ BTP_FAIRY_F_SAD,
    /* 0x32 */ BTP_FAIRY_F_SMILE,
    /* 0x33 */ BTP_FAIRY_F_TALK,

    /* EVT */
    /* 0x36 */ EVT_FAIRY_EVENT_LIST = 0x36,
};

enum RES_Name {
    /* 0x0 */ NONE,
    /* 0x1 */ FAIRY,
};

enum Animation {
    /* 0x0 */ ANM_WAIT,
    /* 0x1 */ ANM_GO,
    /* 0x2 */ ANM_GOWAIT,
    /* 0x3 */ ANM_SIT_A,
    /* 0x4 */ ANM_SIT_B,
    /* 0x5 */ ANM_SIT_GO,
    /* 0x6 */ ANM_SIT_GOWAIT,
    /* 0x7 */ ANM_SITTALK,
    /* 0x8 */ ANM_STANDUP,
    /* 0x9 */ ANM_WALK,
    /* 0xA */ ANM_HOPPE,
    /* 0xB */ ANM_HOPPE_WAIT,
};

enum Feather_Anm {
    /* 0x0 */ ANM_FEATHER_NON,
    /* 0x1 */ ANM_FEATHER_SYUTU,
    /* 0x2 */ ANM_FEATHER_SITWAIT,
    /* 0x3 */ ANM_FEATHER_WAIT,
    /* 0x4 */ ANM_FEATHER_GETUP,
    /* 0x5 */ ANM_FEATHER_WALK,
};

enum Face_Motion {
    /* 0x0 */ FACE_MOT_TALK,
    /* 0x1 */ FACE_MOT_SMILE,
    /* 0x2 */ FACE_MOT_SAD,
    /* 0x3 */ FACE_MOT_H_SMILE,
    /* 0x4 */ FACE_MOT_NONE,
};

enum Motion {
    /* 0x0 */ MOT_SIT_A,
    /* 0x1 */ MOT_SITTALK,
    /* 0x2 */ MOT_SIT_GO,
    /* 0x3 */ MOT_WAIT,
    /* 0x4 */ MOT_WALK,
    /* 0x5 */ MOT_HOPPE,
    /* 0x6 */ MOT_GO,
    /* 0x7 */ MOT_HOPPE_WAIT,
    /* 0x8 */ MOT_SIT_B,
    /* 0x9 */ MOT_SIT_A_2,
    /* 0xA */ MOT_STANDUP,
};

enum Event {
    /* 0x00 */ EVT_NONE,
    /* 0x01 */ EVT_APPEAR_10F_01,
    /* 0x02 */ EVT_APPEAR_10F_02,
    /* 0x03 */ EVT_APPEAR_20F_01,
    /* 0x04 */ EVT_APPEAR_20F_02,
    /* 0x05 */ EVT_APPEAR_30F_01,
    /* 0x06 */ EVT_APPEAR_30F_02,
    /* 0x07 */ EVT_APPEAR_40F_01,
    /* 0x08 */ EVT_APPEAR_40F_02,
    /* 0x09 */ EVT_APPEAR_50F_01,
    /* 0x0A */ EVT_APPEAR_50F_02,
    /* 0x0B */ EVT_APPEAR_50F_03,
    /* 0x0C */ EVT_APPEAR_50F_04,
    /* 0x0D */ EVT_APPEAR_50F_05,
    /* 0x0E */ EVT_SELECT_RETURN1,
    /* 0x0F */ EVT_SELECT_RETURN2,
    /* 0x10 */ EVT_SELECT_RETURN3,
    /* 0x11 */ EVT_RETURN_CANCEL,
};

enum Type {
    /* 0x0 */ TYPE_10F,
    /* 0x1 */ TYPE_20F,
    /* 0x2 */ TYPE_30F,
    /* 0x3 */ TYPE_40F,
    /* 0x4 */ TYPE_50F,
};

const daNpc_Fairy_HIOParam daNpc_Fairy_Param_c::m = {
    190.0f,
    -3.0f,
    1.0f,
    700.0f,
    255.0f,
    170.0f,
    35.0f,
    30.0f,
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
    60.0f,
    200.0f,
};

#if DEBUG
daNpc_Fairy_HIO_c::daNpc_Fairy_HIO_c() {
    m = daNpc_Fairy_Param_c::m;
}

void daNpc_Fairy_HIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    char buffer[2000];
    JORReflexible::listenPropertyEvent(event);
    JORFile jorFile;
    int len;

    switch ((u32)event->id) {
        case 0x40000002:
            if (jorFile.open(6, "", NULL, NULL, NULL)) {
                memset(buffer, 0, sizeof(buffer));
                len = 0;
                daNpcT_cmnListenPropertyEvent(buffer, &len, &m.common);
                jorFile.writeData(buffer, len);
                jorFile.close();
                OS_REPORT("write append success!::%6d\n", len);
            } else {
                OS_REPORT("write append failure!\n");
            }
            break;
    }
}

void daNpc_Fairy_HIO_c::genMessage(JORMContext* ctx) {
    daNpcT_cmnGenMessage(ctx, &m.common);
    ctx->genSlider("残留思念の幅", &m.spirit_width, 0.0f, 1000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("残留思念の高さ", &m.spirit_height, 0.0f, 1000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

BOOL _Fairy_Feather_c::create() {
    if (!loadModel()) {
        return FALSE;
    }

    if (!setAnm(ANM_FEATHER_WAIT)) {
        OS_REPORT("--------------------------ANIMATION SET FAILED!!\n");
        return FALSE;
    }

    if (!setBrk(0)) {
        OS_REPORT("--------------------------BRK SET FAILED!!\n");
        return FALSE;
    }

    if (!setBtk(0)) {
        OS_REPORT("--------------------------BTK SET FAILED!!\n");
        return FALSE;
    }

    playAnm();

    return TRUE;
}

static char* l_resNameList[2] = {
    "",
    "fairy",
};

static s8 l_loadResPtrn0[2] = {
    1, -1,
};

static s8* l_loadResPtrnList[5] = {
    l_loadResPtrn0, l_loadResPtrn0, l_loadResPtrn0,
    l_loadResPtrn0, l_loadResPtrn0,
};

static int l_bmdData[1][2] = {
    {BMDV_FAIRY, FAIRY},
};

static daNpcT_faceMotionAnmData_c l_faceMotionAnmData[5] = {
    {-1, J3DFrameCtrl::EMode_NONE, NONE,BTP_FAIRY, J3DFrameCtrl::EMode_LOOP, FAIRY, 1},
    {BCK_FAIRY_F_SAD, J3DFrameCtrl::EMode_LOOP, FAIRY, BTP_FAIRY_F_SAD, J3DFrameCtrl::EMode_LOOP, FAIRY, 1},
    {BCK_FAIRY_F_SMILE, J3DFrameCtrl::EMode_NONE, FAIRY, BTP_FAIRY_F_SMILE, J3DFrameCtrl::EMode_NONE, FAIRY, 1},
    {BCK_FAIRY_F_TALK, J3DFrameCtrl::EMode_NONE, FAIRY, BTP_FAIRY_F_TALK, J3DFrameCtrl::EMode_NONE, FAIRY, 1},
    {BCK_FAIRY_FH_SMILE, J3DFrameCtrl::EMode_LOOP, FAIRY, BTP_FAIRY, J3DFrameCtrl::EMode_LOOP, FAIRY, 1},
};

static daNpcT_motionAnmData_c l_motionAnmData[12] = {
    {BCK_FAIRY_WAIT, J3DFrameCtrl::EMode_LOOP, FAIRY, BTK_FAIRY, J3DFrameCtrl::EMode_LOOP, FAIRY, 1, 0},
    {BCK_FAIRY_GO, J3DFrameCtrl::EMode_NONE, FAIRY, BTK_FAIRY, J3DFrameCtrl::EMode_LOOP, FAIRY, 1, 0},
    {BCK_FAIRY_GOWAIT, J3DFrameCtrl::EMode_LOOP, FAIRY, BTK_FAIRY, J3DFrameCtrl::EMode_LOOP, FAIRY, 1, 0},
    {BCK_FAIRY_SIT_A, J3DFrameCtrl::EMode_LOOP, FAIRY, BTK_FAIRY, J3DFrameCtrl::EMode_LOOP, FAIRY, 1, 0},
    {BCK_FAIRY_SIT_B, J3DFrameCtrl::EMode_LOOP, FAIRY, BTK_FAIRY, J3DFrameCtrl::EMode_LOOP, FAIRY, 1, 0},
    {BCK_FAIRY_SIT_GO, J3DFrameCtrl::EMode_NONE, FAIRY, BTK_FAIRY, J3DFrameCtrl::EMode_LOOP, FAIRY, 1, 0},
    {BCK_FAIRY_SIT_GOWAIT, J3DFrameCtrl::EMode_LOOP, FAIRY, BTK_FAIRY, J3DFrameCtrl::EMode_LOOP, FAIRY, 1, 0},
    {BCK_FAIRY_SITTALK, J3DFrameCtrl::EMode_NONE, FAIRY, BTK_FAIRY, J3DFrameCtrl::EMode_LOOP, FAIRY, 1, 0},
    {BCK_FAIRY_STANDUP, J3DFrameCtrl::EMode_NONE, FAIRY, BTK_FAIRY, J3DFrameCtrl::EMode_LOOP, FAIRY, 1, 0},
    {BCK_FAIRY_WALK, J3DFrameCtrl::EMode_LOOP, FAIRY, BTK_FAIRY, J3DFrameCtrl::EMode_LOOP, FAIRY, 1, 0},
    {BCK_FAIRY_HOPPE, J3DFrameCtrl::EMode_NONE, FAIRY, BTK_FAIRY, J3DFrameCtrl::EMode_LOOP, FAIRY, 1, 0},
    {BCK_FAIRY_HOPPE_WAIT, J3DFrameCtrl::EMode_LOOP, FAIRY, BTK_FAIRY, J3DFrameCtrl::EMode_LOOP, FAIRY, 1, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_faceMotionSequenceData[20] = {
    {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {2, -1, 1}, {4, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

static daNpcT_MotionSeqMngr_c::sequenceStepData_c l_motionSequenceData[44] = {
    {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {7, -1, 1}, {3, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {5, -1, 1}, {6, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {0, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {9, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {10, -1, 1}, {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {1, -1, 1}, {2, -1, 0}, {-1, 0, 0}, {-1, 0, 0},
    {11, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {4, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {3, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
    {8, -1, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
};

BOOL _Fairy_Feather_c::loadModel() {
    // ----------------Loading wing model.\n
    OS_REPORT("---------------- 羽モデルを読み込みます。\n");
    J3DModelData* mdlData_p = NULL;
    mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[1], BMDV_FAIRY_FEATHER_GT);
    OS_REPORT("----------- RESOURCE NAME = %s\n", l_resNameList[1]);
    JUT_ASSERT(674, NULL != mdlData_p);

    u32 uVar1 = 0x11000284;
    mpMorf = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, J3DMdlFlag_None, uVar1);
    if (mpMorf != NULL && mpMorf->getModel() == NULL) {
        mpMorf->stopZelAnime();
        mpMorf = NULL;
    }

    if (mpMorf == NULL) {
        // ----------------Failed to load wing model\n
        OS_REPORT("---------------- 羽モデル読み込み失敗\n");
        return 0;
    }

    J3DModel* mdl_p = mpMorf->getModel();
    for (u16 i = 0; i < 26; i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    // ---------------- Wing model generation complete\n
    OS_REPORT("---------------- 羽モデル生成完了\n");

    return 1;
}

BOOL _Fairy_Feather_c::setAnm(int i_idx) {
    if (i_idx == mPrevAnm) {
        return TRUE;
    }

    static daNpc_GetParam2 const sBckPrm[6] = {
        {BCK_FAIRY_HANE_NON, J3DFrameCtrl::EMode_LOOP, FAIRY},
        {BCK_FAIRY_HANE_SYUTU, J3DFrameCtrl::EMode_NONE, FAIRY},
        {BCK_FAIRY_HANE_SITWAIT, J3DFrameCtrl::EMode_LOOP, FAIRY},
        {BCK_FAIRY_HANE_WAIT, J3DFrameCtrl::EMode_LOOP, FAIRY},
        {BCK_FAIRY_HANE_GETUP, J3DFrameCtrl::EMode_NONE, FAIRY},
        {BCK_FAIRY_HANE_WALK, J3DFrameCtrl::EMode_LOOP, FAIRY},
    };

    J3DAnmTransform* anm = NULL;
    anm = static_cast<J3DAnmTransform*>(dComIfG_getObjectRes(l_resNameList[sBckPrm[i_idx].arcIdx], sBckPrm[i_idx].fileIdx));
    if (anm == NULL) {
        return FALSE;
    }

    f32 i_morf = 0.0f;
    if (i_idx == ANM_FEATHER_WAIT) {
        if (mPrevAnm == ANM_FEATHER_WALK) {
            i_morf = 12.0f;
        }
    } else if (i_idx == ANM_FEATHER_WALK && mPrevAnm == ANM_FEATHER_WAIT) {
        i_morf = 12.0f;
    }

    mpMorf->setAnm(anm, sBckPrm[i_idx].attr, i_morf, 1.0f, 0.0f, -1.0f);
    mpMorf->setPlaySpeed(1.0f);
    mPrevAnm = i_idx;

    return TRUE;
}

BOOL _Fairy_Feather_c::setBrk(int i_idx) {
    static daNpc_GetParam2 const sBrkPrm[1] = {
        {BRK_FAIRY_FEATHER_GT, J3DFrameCtrl::EMode_LOOP, FAIRY},
    };

    J3DAnmTevRegKey* brk = NULL;
    brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_resNameList[sBrkPrm[i_idx].arcIdx], sBrkPrm[i_idx].fileIdx);
    if (brk == NULL) {
        return FALSE;
    }

    return mBrkAnm.init(mpMorf->getModel()->getModelData(), brk, 1, sBrkPrm[i_idx].attr, 1.0f, 0, -1);
}

BOOL _Fairy_Feather_c::setBtk(int i_idx) {
    static daNpc_GetParam2 const sBtkPrm[1] = {
        {BTK_FAIRY_FEATHER_GT, J3DFrameCtrl::EMode_LOOP, FAIRY},
    };

    J3DAnmTextureSRTKey* btk = NULL;
    btk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_resNameList[sBtkPrm[i_idx].arcIdx], sBtkPrm[i_idx].fileIdx);
    if (btk == NULL) {
        return FALSE;
    }

    return mBtkAnm.init(mpMorf->getModel()->getModelData(), btk, 1, sBtkPrm[i_idx].attr, 1.0f, 0, -1);
}

int _Fairy_Feather_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    return 1;
}

void _Fairy_Feather_c::connect(daNpc_Fairy_c* i_this) {
    J3DModel* mdl_p = i_this->mpMorf[0]->getModel();
    mDoMtx_stack_c::copy(mdl_p->getAnmMtx(JNT_FEATHER1AL1));
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
}

void _Fairy_Feather_c::draw(daNpc_Fairy_c* i_this) {
    cXyz pos(fopAcM_GetPosition(i_this));
    dKy_tevstr_c tevStr = i_this->tevStr;
    J3DModel* mdl_p = mpMorf->getModel();

    g_env_light.settingTevStruct(0, &pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mdl_p, &tevStr);
    
    J3DModelData* mdlData_p = mdl_p->getModelData();
    mBtkAnm.entry(mdlData_p);
    mBrkAnm.entry(mdlData_p);
    mpMorf->entryDL();

    mBrkAnm.remove(mdlData_p);
    mBtkAnm.remove(mdlData_p);
}

static daNpcT_evtData_c l_evtList[18] = {
    {"", 0},
    {"APPEAR_10F_01", 1},
    {"APPEAR_10F_02", 1},
    {"APPEAR_20F_01", 1},
    {"APPEAR_20F_02", 1},
    {"APPEAR_30F_01", 1},
    {"APPEAR_30F_02", 1},
    {"APPEAR_40F_01", 1},
    {"APPEAR_40F_02", 1},
    {"APPEAR_50F_01", 1},
    {"APPEAR_50F_02", 1},
    {"APPEAR_50F_03", 1},
    {"APPEAR_50F_04", 1},
    {"APPEAR_50F_05", 1},
    {"SELECT_RETURN1", 1},
    {"SELECT_RETURN2", 1},
    {"SELECT_RETURN3", 1},
    {"RETURN_CANCEL", 1},
};

char* daNpc_Fairy_c::mCutNameList[18] = {
    "",
    "APPEAR_10F_01",
    "APPEAR_10F_02",
    "APPEAR_20F_01",
    "APPEAR_20F_02",
    "APPEAR_30F_01",
    "APPEAR_30F_02",
    "APPEAR_40F_01",
    "APPEAR_40F_02",
    "APPEAR_50F_01",
    "APPEAR_50F_02",
    "APPEAR_50F_03",
    "APPEAR_50F_04",
    "APPEAR_50F_05",
    "SELECT_RETURN1",
    "SELECT_RETURN2",
    "SELECT_RETURN3",
    "RETURN_CANCEL",
};

daNpc_Fairy_c::cutFunc daNpc_Fairy_c::mCutList[18] = {
    NULL,
    &daNpc_Fairy_c::cutAppear_10F_01,
    &daNpc_Fairy_c::cutAppear_10F_02,
    &daNpc_Fairy_c::cutAppear_20F_01,
    &daNpc_Fairy_c::cutAppear_20F_02,
    &daNpc_Fairy_c::cutAppear_30F_01,
    &daNpc_Fairy_c::cutAppear_30F_02,
    &daNpc_Fairy_c::cutAppear_40F_01,
    &daNpc_Fairy_c::cutAppear_40F_02,
    &daNpc_Fairy_c::cutAppear_50F_01,
    &daNpc_Fairy_c::cutAppear_50F_02,
    &daNpc_Fairy_c::cutAppear_50F_03,
    &daNpc_Fairy_c::cutAppear_50F_04,
    &daNpc_Fairy_c::cutAppear_50F_05,
    &daNpc_Fairy_c::cutSelect_Return1,
    &daNpc_Fairy_c::cutSelect_Return2,
    &daNpc_Fairy_c::cutSelect_Return3,
    &daNpc_Fairy_c::cutReturnCancel,
};

daNpc_Fairy_c::~daNpc_Fairy_c() {
    OS_REPORT("|%06d:%x|daNpc_Fairy_c -> デストラクト\n", g_Counter.mCounter0, this);

    if (heap != NULL) {
        mpMorf[0]->stopZelAnime();
    }

    #if DEBUG
    if (mHIO != NULL) {
        mHIO->removeHIO();
    }
    #endif

    deleteRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
}

static NPC_FAIRY_HIO_CLASS l_HIO;

cPhs__Step daNpc_Fairy_c::Create() {
    daNpcT_ct(this, daNpc_Fairy_c, l_faceMotionAnmData, l_motionAnmData,
                       l_faceMotionSequenceData, 4, l_motionSequenceData, 4, l_evtList, l_resNameList);

    mType = getType();
    mFlowNodeNo = getFlowNodeNo();
    mTwilight = false;

    cPhs__Step phase = (cPhs__Step)loadRes(l_loadResPtrnList[mType], (const char**)l_resNameList);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x8000)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("\t(%s:%d) flowNo:%d<%08x> ", fopAcM_getProcNameString(this), mType, mFlowNodeNo, fopAcM_GetParam(this));

        if (isDelete()) {
            OS_REPORT("===>isDelete:TRUE\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("\n");

        J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf[0]->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -200.0f, -400.0f, 300.0f, 200.0f, 200.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);

        #if DEBUG
        mHIO = &l_HIO;
        mHIO->entryHIO("大妖精");
        #endif
        
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this),
                  fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mCcStts.Init(mHIO->m.common.weight, 0, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mCcStts);
        mCyl.SetTgHitCallback(tgHitCallBack);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();

        if (mGroundH != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }

        reset();
        mCreating = true;
        Execute();
        mCreating = false;
    }

    return phase;
}

int daNpc_Fairy_c::CreateHeap() {
    int arc_idx = 0;
    int file_idx = 0;
    J3DModelData* mdlData_p = NULL;

    arc_idx = l_bmdData[0][1];
    file_idx = l_bmdData[0][0];
    mdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[arc_idx], file_idx);
    JUT_ASSERT(1113, NULL != mdlData_p);

    u32 uVar1 = 0x11020285;
    mpMorf[0] = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, J3DMdlFlag_None, uVar1);
    if (mpMorf[0] != NULL && mpMorf[0]->getModel() == NULL) {
        mpMorf[0]->stopZelAnime();
        mpMorf[0] = NULL;
    }

    if (mpMorf[0] == NULL) {
        return 0;
    }

    J3DModel* mdl_p = mpMorf[0]->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }

    if (!mFairyFeather.create()) {
        return 0;
    }

    mdl_p->setUserArea((uintptr_t)this);
    
    mpMatAnm[0] = new daNpcT_MatAnm_c();
    if (mpMatAnm[0] == NULL) {
        return 0;
    }

    setAnmData();

    if (setFaceMotionAnm(1, false) && setMotionAnm(ANM_WAIT, 0.0f, 0)) {
        return 1;
    }

    return 1;
}

void daNpc_Fairy_c::setAnmData() {
    J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
    J3DAnmColor* bpk = getColorAnmP(l_resNameList[1], BPK_FAIRY);
    if (bpk != NULL) {
        setBpkAnm(bpk, mdlData_p, 1.0f, J3DFrameCtrl::EMode_LOOP);
    }

    J3DAnmTevRegKey* brk = getTevRegKeyAnmP(l_resNameList[1], BRK_FAIRY);
    if (brk != NULL) {
        setBrkAnm(brk, mdlData_p, 1.0f, J3DFrameCtrl::EMode_LOOP);
    }

    J3DAnmTextureSRTKey* btk = getTexSRTKeyAnmP(l_resNameList[1], BTK_FAIRY);
    if (btk != NULL) {
        setBtkAnm(btk, mdlData_p, 1.0f, J3DFrameCtrl::EMode_LOOP);
    }

    mAnmFlags |= ANM_PLAY_BPK | ANM_PLAY_BRK | ANM_PLAY_BTK;
}

bool daNpc_Fairy_c::setMotionAnm(int i_idx, f32 i_morf, int param_3) {
    J3DAnmTransformKey* anm = NULL;
    J3DAnmTextureSRTKey* btk = NULL;
    int unused = 0;

    mAnmFlags &= ~(ANM_PLAY_BPK | ANM_PLAY_BRK | ANM_PLAY_BTK | ANM_PLAY_MORF | ANM_PAUSE_BPK | ANM_PAUSE_BRK | ANM_PAUSE_BTK | ANM_PAUSE_MORF);

    daNpcT_motionAnmData_c anmData = getMotionAnm(mpMotionAnmData[i_idx]);
    if (anmData.mBckFileIdx != -1) {
        anm = getTrnsfrmKeyAnmP(mpArcNames[anmData.mBckArcIdx], anmData.mBckFileIdx);
    }

    if (param_3 != 0 && anm == mpMorf[0]->getAnm()) {
        mAnmFlags |= ANM_PLAY_MORF;
        mMorfLoops = 0;
        unused = 1;
    } else if (setMcaMorfAnm(anm, 1.0f, i_morf, anmData.mBckAttr, 0, -1) != 0) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMorfLoops = 0;
    } else {
        anm = NULL;
    }

    if (anm == NULL && anmData.mBckFileIdx != -1) {
        return false;
    }

    field_0xe29 = 0;
    field_0xe2a = 0;

    if (anmData.mBtkFileIdx != -1) {
        int arc_idx = anmData.mBtkArcIdx;
        int file_idx = anmData.mBtkFileIdx;
        changeBtk(&file_idx, &arc_idx);
        field_0xe29 = anmData.field_0x18;
        field_0xe2a = anmData.field_0x1a;
        btk = getTexSRTKeyAnmP(mpArcNames[arc_idx], file_idx);
    }

    mAnmFlags |= ANM_PLAY_BTK;

    if (btk == NULL && anmData.mBtkFileIdx != -1) {
        return false;
    }

    mAnmFlags |= ANM_PLAY_BPK | ANM_PLAY_BRK | ANM_PLAY_BTK;

    return daNpc_Fairy_c::afterSetMotionAnm(i_idx, anmData.mBckAttr, i_morf, anmData.mBtkAttr);
}

bool daNpc_Fairy_c::afterSetMotionAnm(int i_idx, int i_bck_attr, f32 i_morf, int i_btk_attr) {
    int feather_bck;

    if (mUnkFlag == 0) {
        return true;
    }

    switch (i_idx) {
        case ANM_SIT_A:
        case ANM_SIT_B:
        case ANM_SIT_GO:
        case ANM_SIT_GOWAIT:
        case ANM_SITTALK:
            feather_bck = ANM_FEATHER_SITWAIT;
            break;
        
        case ANM_STANDUP:
            feather_bck = ANM_FEATHER_GETUP;
            break;

        case ANM_WALK:
            feather_bck = ANM_FEATHER_WALK;
            break;

        default:
            feather_bck = ANM_FEATHER_WAIT;
            break;
    }

    OS_REPORT("feather_bck=%d\n", feather_bck);
    return mFairyFeather.setAnm(feather_bck);
}

int daNpc_Fairy_c::Delete() {
    OS_REPORT("|%06d:%x|daNpc_Fairy_c -> Delete\n", g_Counter.mCounter0, this);
    fpc_ProcID id = fopAcM_GetID(this);
    this->~daNpc_Fairy_c();
    return 1;
}

int daNpc_Fairy_c::Execute() {
    int rv = execute();
    mFairyFeather.playAnm();
    mFairyFeather.output_anmframe();
    return rv;
}

int daNpc_Fairy_c::Draw() {
    if (mStatus == 1) {
        return 1;
    }

    if (mpMatAnm[0] != NULL) {
        J3DModelData* mdlData_p = mpMorf[0]->getModel()->getModelData();
        mdlData_p->getMaterialNodePointer(getEyeballMaterialNo())->setMaterialAnm(mpMatAnm[0]);
    }

    #if DEBUG
    return draw(chkAction(&daNpc_Fairy_c::test), TRUE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE);
    #else
    return draw(FALSE, TRUE, mRealShadowSize, NULL, 100.0f, FALSE, FALSE, FALSE);
    #endif
}

void daNpc_Fairy_c::drawOtherMdl() {
    mFairyFeather.connect(this);
    mFairyFeather.draw(this);
}

int daNpc_Fairy_c::createHeapCallBack(fopAc_ac_c* actor) {
    daNpc_Fairy_c* i_this = (daNpc_Fairy_c*)actor;
    return i_this->CreateHeap();
}

int daNpc_Fairy_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* mdl_p = j3dSys.getModel();
        daNpc_Fairy_c* actor = (daNpc_Fairy_c*)mdl_p->getUserArea();
        if (actor != NULL) {
            actor->ctrlJoint(i_joint, mdl_p);
        }
    }

    return 1;
}

BOOL daNpc_Fairy_c::isDelete() {
    return FALSE;
}

void daNpc_Fairy_c::reset() {
    initialize();

    if (mpMatAnm[0] != NULL) {
        mpMatAnm[0]->initialize();
    }

    mPrtclTimer = 10;
    mPaPo.init(&mAcch, mHIO->m.common.height, mHIO->m.common.height);
    setAngle(home.angle.y);
    initStatus();
    mSwitchBit = getSwitchBitNo();
    mTempBit = -1;

    if (mType == TYPE_50F) {
        mTempBit = chk_tempbit();
        OS_REPORT("-------------daNpc_Fairy_c::chk temp bit  mTempBit=%d\n", mTempBit);

        if (mTempBit >= 0) {
            mFlowNodeNo = 0xBC8;
        }
    }

    mUnkFlag = 0;
    mAction = NULL;
    setAction(&daNpc_Fairy_c::wait, 0);
}

void daNpc_Fairy_c::initStatus() {
    mStatus = 1;
}

void daNpc_Fairy_c::afterJntAnm(int i_joint) {
    if (i_joint == JNT_BACKBONE1) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(1));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(1));
    } else if (i_joint == JNT_HEAD) {
        mDoMtx_stack_c::YrotM(mStagger.getAngleZ(0));
        mDoMtx_stack_c::ZrotM(-mStagger.getAngleX(0));
    }
}

void daNpc_Fairy_c::setParam() {
    srchActors();
    s16 talk_distance = mHIO->m.common.talk_distance;
    s16 talk_angle = mHIO->m.common.talk_angle;
    s16 attention_distance = mHIO->m.common.attention_distance;
    s16 attention_angle = mHIO->m.common.attention_angle;

    if (mStatus == 1) {
        talk_distance = 7;
        attention_distance = 9;
    }

    attention_info.distances[fopAc_attn_LOCK_e] = daNpcT_getDistTableIdx(attention_distance, attention_angle);
    attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
    attention_info.distances[fopAc_attn_SPEAK_e] = daNpcT_getDistTableIdx(talk_distance, talk_angle);
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    scale.set(mHIO->m.common.scale, mHIO->m.common.scale, mHIO->m.common.scale);
    mCcStts.SetWeight(mHIO->m.common.weight);

    if (mStatus == 0) {
        mCylH = mHIO->m.common.height;
        mWallR = mHIO->m.common.width;
    } else if (mStatus == 1) {
        mCylH = mHIO->m.spirit_height;
        mWallR = mHIO->m.spirit_width;
    }

    mAttnFovY = mHIO->m.common.fov;
    mAcchCir.SetWallR(mWallR);
    mAcchCir.SetWallH(mHIO->m.common.knee_length);
    mRealShadowSize = mHIO->m.common.real_shadow_size;
    gravity = mHIO->m.common.gravity;
    mExpressionMorfFrame = mHIO->m.common.expression_morf_frame;
    mMorfFrames = mHIO->m.common.morf_frame;
}

BOOL daNpc_Fairy_c::checkChangeEvt() {
    return FALSE;
}

void daNpc_Fairy_c::setAfterTalkMotion() {
    int idx;
    switch (mFaceMotionSeqMngr.getNo()) {
        case FACE_MOT_SMILE:
            idx = FACE_MOT_H_SMILE;
            break;

        default:
            idx = FACE_MOT_NONE;
            break;

    }

    mFaceMotionSeqMngr.setNo(idx, -1.0f, FALSE, 0);
}

void daNpc_Fairy_c::srchActors() {
    /* empty function */
}

BOOL daNpc_Fairy_c::evtTalk() {
    if (chkAction(&daNpc_Fairy_c::talk)) {
        (this->*mAction)(NULL);
    } else {
        mPreItemNo = 0;

        if (dComIfGp_event_chkTalkXY()) {
            if (!dComIfGp_evmng_ChkPresentEnd()) {
                return TRUE;
            }

            dComIfGp_event_reset();
            return TRUE;
        }

        setAction(&daNpc_Fairy_c::talk, 0);
    }

    return TRUE;
}

BOOL daNpc_Fairy_c::evtCutProc() {
    BOOL rv = FALSE;
    s32 staffId = dComIfGp_getEventManager().getMyStaffId("Fairy", this, -1);
    if (staffId != -1) {
        mStaffId = staffId;
        int act_idx = dComIfGp_getEventManager().getMyActIdx(mStaffId, mCutNameList, 0x12, FALSE, FALSE);

        if ((this->*mCutList[act_idx])(mStaffId)) {
            dComIfGp_getEventManager().cutEnd(mStaffId);
        }

        rv = TRUE;
    }

    return rv;
}

BOOL daNpc_Fairy_c::evtEndProc() {
    OS_REPORT("------------------evt end proc!!\n");

    switch (field_0xff4) {
        case EVT_SELECT_RETURN1:
        case EVT_SELECT_RETURN3:
            if (mFlow.getChoiceNo() == 0) {
                mEvtNo = EVT_SELECT_RETURN2;
            } else {
                mEvtNo = EVT_RETURN_CANCEL;
                Z2GetAudioMgr()->bgmStreamStop(0x2D);
            }

            field_0xff4 = mEvtNo;
            evtChange();
            break;

        case EVT_SELECT_RETURN2:
        default:
            dKy_change_colpat(0);
            break;
    }

    return TRUE;
}

void daNpc_Fairy_c::action() {
    if (mStagger.checkRebirth()) {
        mStagger.initialize();
        mMode = 1;
    }

    if (mAction) {
        (this->*mAction)(NULL);
    }
}

void daNpc_Fairy_c::ptcl_water() {
    u32 uVar1 = 0x40002;
    cXyz scale(1.1f, 1.1f, 1.1f);

    mPrtclTimer--;
    if (mPrtclTimer <= 0) {
        mPrtclTimer = 10;
    } else {
        uVar1 |= 0x20000;
    }

    mPaPo.setEffectCenter(&tevStr, &current.pos, 1, uVar1, NULL, NULL, &scale, fopAcM_GetRoomNo(this), 1.0f, speedF);
}

void daNpc_Fairy_c::ptcl_sprit() {
    cXyz pos;
    cXyz scale(1.0f, 1.0f, 1.0f);

    static u16 const id[3] = {
        dPa_RM(ID_ZI_S_FAIRY_SPRIT_A),
        dPa_RM(ID_ZI_S_FAIRY_SPRIT_B),
        dPa_RM(ID_ZI_S_FAIRY_SPRIT_C),
    };

    for (int i = 0; i < 3; i++) {
        pos = current.pos;
        mPrtcls[i] = dComIfGp_particle_set(mPrtcls[i], id[i], &pos, &mCurAngle, &scale);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mPrtcls[i]);
        if (emitter != NULL) {
            emitter->setGlobalTranslation(pos.x, pos.y, pos.z);
        }
    }
}

void daNpc_Fairy_c::_to_FairyField() {
    int iVar1 = 4;
    if (mTempBit < 0 || mTempBit >= iVar1) {
        return;
    }

    Z2GetAudioMgr()->seStart(Z2SE_SY_WARP_FADE_FAIRY, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);

    static u8 const sFairyField[4] = {
        0x00,
        0x01,
        0x02,
        0x03,
    };

    dStage_changeScene(sFairyField[mTempBit], 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
}

int daNpc_Fairy_c::chk_tempbit() {
    static u32 const sTempBit[4] = {
        121,   // dSv_event_tmp_flag_c::T_0121 - Program use - Fairy use 1
        122,   // dSv_event_tmp_flag_c::T_0122 - Program use - Fairy use 2
        123,   // dSv_event_tmp_flag_c::T_0123 - Program use - Fairy use 3
        124,   // dSv_event_tmp_flag_c::T_0124 - Program use - Fairy use 4
    };

    u32 i = 0;
    for (; i < 4; i++) {
        OS_REPORT("----------chk temp bit no =%d\n ", sTempBit[i]);
        if (daNpcT_chkTmpBit(sTempBit[i])) {
            OS_REPORT("----------on temp bit no =%d\n ", sTempBit[i]);
            daNpcT_offTmpBit(sTempBit[i]);
            return i;
        }
    }

    return -1;
}

void daNpc_Fairy_c::beforeMove() {
    if (checkHide() || mNoDraw) {
        attention_info.flags = 0;
    }
}

void daNpc_Fairy_c::afterMoved() {
    ptcl_water();

    switch (mStatus) {
        case 0:
            if (mUnkFlag) {
                feather_se();
            }
            break;
        
        case 1:
            ptcl_sprit();

            if (!dComIfGp_event_runCheck()) {
                sprit_se();
            }
            break;
    }
}

void daNpc_Fairy_c::setAttnPos() {
    cXyz pos(0.0f, -30.0f, 0.0f);

    mStagger.calc(FALSE);
    f32 rad = cM_s2rad((s16)(mCurAngle.y - field_0xd7e.y));
    
    mJntAnm.setParam(this, mpMorf[0]->getModel(), &pos, getBackboneJointNo(), getNeckJointNo(), getHeadJointNo(),
                     mHIO->m.common.body_angleX_min, mHIO->m.common.body_angleX_max, mHIO->m.common.body_angleY_min, mHIO->m.common.body_angleY_max,
                     mHIO->m.common.head_angleX_min, mHIO->m.common.head_angleX_max, mHIO->m.common.head_angleY_min, mHIO->m.common.head_angleY_max,
                     mHIO->m.common.neck_rotation_ratio, rad, NULL);
    mJntAnm.calcJntRad(0.2f, 1.0f, rad);
    setMtx();

    mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getHeadJointNo()));
    mDoMtx_stack_c::multVec(&pos, &eyePos);
    eyePos.y += 15.0f;

    mJntAnm.setEyeAngleX(eyePos, 1.0f, 0);
    mJntAnm.setEyeAngleY(eyePos, mCurAngle.y, TRUE, 1.0f, 0);

    attention_info.position = current.pos;
    attention_info.position.y += mHIO->m.common.attention_offset;
}

void daNpc_Fairy_c::setCollision() {
    cXyz pos;
    u32 co_s_prm = 0x79;
    u32 tg_type = 0xD8FBFDFF;
    u32 tg_s_prm = 0x1F;

    if (!mHide) {
        if (dComIfGp_event_runCheck()) {
            tg_type = 0;
            tg_s_prm = 0;
            co_s_prm = 0;
        } else if (mTwilight) {
            tg_type = 0;
            tg_s_prm = 0;
        } else if (mStagger.checkStagger()) {
            tg_type = 0;
            tg_s_prm = 0;
        }

        f32 cyl_h = mCylH;
        f32 wall_r = mWallR;
        pos = current.pos;

        mCyl.SetCoSPrm(co_s_prm);
        mCyl.SetTgType(tg_type);
        mCyl.SetTgSPrm(tg_s_prm);
        mCyl.OnTgNoHitMark();
        mCyl.SetH(cyl_h);
        mCyl.SetR(wall_r);
        mCyl.SetC(pos);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    mCyl.ClrTgHit();
}

int daNpc_Fairy_c::drawDbgInfo() {
    return 0;
}

BOOL daNpc_Fairy_c::setAction(actionFunc action, int param_2) {
    mMode = 0xFFFF;

    if (mAction) {
        (this->*mAction)(NULL);
    }

    mMode = 0;
    mAction = action;

    if (mAction) {
        (this->*mAction)(param_2);
    }

    return TRUE;
}

int daNpc_Fairy_c::wait(int param_1) {
    switch (mMode) {
        case -1:
            break;
        
        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                if (mType == TYPE_50F && mTempBit >= 0) {
                    mUnkFlag = 1;
                    mEvtNo = EVT_APPEAR_50F_03;
                    field_0xff4 = mEvtNo;
                    evtChange();
                } else {
                    mFaceMotionSeqMngr.setNo(FACE_MOT_NONE, -1.0f, FALSE, 0);
                    mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
                    mMode = 2;
                }
            }
            break;
        
        case 2:
            if (mStatus == 1) {
                if (daNpcT_chkDoBtnIsSpeak(this)) {
                    dComIfGp_setDoStatus(8, 0);
                }
            } else if (mStatus == 0 && !mStagger.checkStagger()) {
                if (mPlayerActorMngr.getActorP() != NULL && !mTwilight) {
                    mJntAnm.lookNone(0);

                    if (chkActorInSight(mPlayerActorMngr.getActorP(), mAttnFovY, mCurAngle.y)) {
                        mJntAnm.lookPlayer(0);
                    }

                    if (!srchPlayerActor() && home.angle.y == mCurAngle.y) {
                        mMode = 1;
                    }
                } else {
                    mJntAnm.lookNone(0);

                    if (home.angle.y != mCurAngle.y) {
                        if (field_0xe34 == 0) {
                            setAngle(home.angle.y);
                            mMode = 1;
                        } else if (step(home.angle.y, -1, -1, 15, 0)) {
                            mMode = 1;
                        }

                        attention_info.flags = 0;
                    } else {
                        srchPlayerActor();
                    }
                }
            }
            break;
    }

    return 1;
}

int daNpc_Fairy_c::talk(int param_1) {
    switch (mMode) {
        case -1:
            break;

        case 0:
        case 1:
            if (!mStagger.checkStagger()) {
                if (mStatus == 1) {
                    initTalk(-1, NULL);
                } else {
                    initTalk(mFlowNodeNo, NULL);
                }

                if (checkStep()) {
                    mStepMode = 0;
                }

                mMode = 2;
            }
            break;
        
        case 2:
            if (mStatus == 1) {
                if (!fopAcM_isSwitch(this, mSwitchBit)) {
                    AppearDemoCall();
                } else {
                    ReturnDemoCall();
                }

                evtChange();

                return 1;
            }
    }

    return 1;
}

int daNpc_Fairy_c::test(int param_1) {
    int rv = 0;

    switch (mMode) {
        case -1:
            break;
        
        case 0:
        case 1:
            speedF = 0.0f;
            speed.setall(0.0f);
            mMode = 2;
            // fallthrough
        case 2:
            mFaceMotionSeqMngr.setNo(mHIO->m.common.face_expression, -1.0f, FALSE, 0);
            mMotionSeqMngr.setNo(mHIO->m.common.motion, -1.0f, FALSE, 0);
            mJntAnm.lookNone(0);
            attention_info.flags = 0;
            break;
    }

    return rv;
}

void daNpc_Fairy_c::AppearDemoCall() {
    if (mType == TYPE_50F) {
        // dSv_event_flag_c::F_0505 - sub-dungeon - Cave of Ordeals - B50 first arrival (clear)
        if (daNpcT_chkEvtBit(505)) {
            if (dComIfGs_checkEmptyBottle()) {
                mEvtNo = EVT_APPEAR_50F_05;
            } else {
                mEvtNo = EVT_APPEAR_50F_02;
            }
        } else if (dComIfGs_checkEmptyBottle()) {
            mEvtNo = EVT_APPEAR_50F_01;
        } else {
            mEvtNo = EVT_APPEAR_50F_04;
        }
    } else {
        static u16 const sEvtType1[5] = {
            EVT_APPEAR_10F_01,
            EVT_APPEAR_20F_01,
            EVT_APPEAR_30F_01,
            EVT_APPEAR_40F_01,
            EVT_APPEAR_50F_01,
        };

        static u16 const sEvtType2[5] = {
            EVT_APPEAR_10F_02,
            EVT_APPEAR_20F_02,
            EVT_APPEAR_30F_02,
            EVT_APPEAR_40F_02,
            EVT_APPEAR_50F_02,
        };

        static s16 const sEvtBitNo[5] = {
            501,
            502,
            503,
            504,
            505,
        };

        mEvtNo = sEvtType1[mType];

        // dSv_event_flag_c::F_0505 - sub-dungeon - Cave of Ordeals - B50 first arrival (clear)
        if (daNpcT_chkEvtBit(505)) {
            mEvtNo = sEvtType2[mType];
        } else if (daNpcT_chkEvtBit(sEvtBitNo[mType])) {
            mEvtNo = sEvtType2[mType];
        }

        if (mEvtNo != EVT_APPEAR_10F_01) {
            mUnkFlag = 1;
        }
    }

    field_0xff4 = mEvtNo;
}

void daNpc_Fairy_c::ReturnDemoCall() {
    if (mType == TYPE_10F) {
        mEvtNo = EVT_SELECT_RETURN1;
    } else {
        mEvtNo = EVT_SELECT_RETURN3;
    }

    field_0xff4 = mEvtNo;
    mUnkFlag = 1;
}

void daNpc_Fairy_c::PresentDemoCall() {
    int item_no = 0;
    if (mFlow.getEventId(&item_no) != 1) {
        item_no = 0;
    }

    fpc_ProcID id = fopAcM_createItemForPresentDemo(&current.pos, item_no, 0, -1, -1, NULL, NULL);
    if (id != fpcM_ERROR_PROCESS_ID_e) {
        dComIfGp_event_setItemPartnerId(id);
    }
}

int daNpc_Fairy_c::cutAppear_10F_01(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutAppear_10F_01_Init(*cutId);
    }

    return _cutAppear_10F_01_Main(*cutId);
}

int daNpc_Fairy_c::_cutAppear_10F_01_Init(int const& i_cutId) {
    int rv = 0;
    
    switch (i_cutId) {
        case 0xA:
            mMotionSeqMngr.setNo(MOT_SIT_B, -1.0f, FALSE, 0);
            mTimer = 61;
            dKy_change_colpat(10);
            Z2GetAudioMgr()->bgmStreamPrepare(0x200003F);
            break;

        case 0x14:
            mStatus = 0;
            mTimer = 120;
            Z2GetAudioMgr()->bgmStreamPlay();
            dKy_change_colpat(12);
            break;
        
        case 0x15:
            mMotionSeqMngr.setNo(MOT_SIT_A_2, -1.0f, FALSE, 0);
            break;

        case 0x1E:
            feather_appear_se();
            mFairyFeather.setAnm(ANM_FEATHER_SYUTU);
            break;

        case 0x28:
            break;

        case 0x32:
            mFairyFeather.setAnmFrame(220.0f);
            mFairyFeather.setAnmPlaySpeed(1.0f);
            break;
            
        case 0x3C:
            mUnkFlag = 1;
            initTalk(mFlowNodeNo, NULL);
            break;

        case 0x46:
            Z2GetAudioMgr()->bgmStreamStop(45);
    }

    return rv;
}

int daNpc_Fairy_c::_cutAppear_10F_01_Main(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        case 0xA:
            if (--mTimer <= 0) {
                mFairyFeather.setAnm(ANM_FEATHER_NON);
                dKy_change_colpat(11);
                rv = 1;
            }
            break;

        case 0x14:
            if (--mTimer <= 0) {
                rv = 1;
            }
            break;
        
        case 0x15:
            rv = 1;
            break;

        case 0x1E:
            if (mFairyFeather.getAnmFrame() >= 140.0f) {
                rv = 1;
            }
            break;
        
        case 0x28:
            if (mFairyFeather.getAnmFrame() >= 220.0f) {
                rv = 1;
            }
            break;

        case 0x32:
            if (mFairyFeather.getAnmFrame() >= 298.0f) {
                mFairyFeather.setAnm(ANM_FEATHER_SITWAIT);
                rv = 1;
            }
            break;

        case 0x3C:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x46:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Fairy_c::cutAppear_10F_02(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutAppear_10F_02_Init(*cutId);
    }

    return _cutAppear_10F_02_Main(*cutId);
}

int daNpc_Fairy_c::_cutAppear_10F_02_Init(int const& i_cutId) {
    int rv = 0;
    
    switch (i_cutId) {
        case 0xA:
            mMotionSeqMngr.setNo(MOT_SIT_A, -1.0f, FALSE, 0);
            mTimer = 61;
            dKy_change_colpat(10);
            Z2GetAudioMgr()->bgmStreamPrepare(0x200003F);
            break;

        case 0x14:
            mStatus = 0;
            mTimer = 120;
            dKy_change_colpat(12);
            Z2GetAudioMgr()->bgmStreamPlay();
            break;

        case 0x1E:
            initTalk(mFlowNodeNo, NULL);
            break;

        case 0x28:
            Z2GetAudioMgr()->bgmStreamStop(45);
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutAppear_10F_02_Main(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        case 0xA:
            if (--mTimer <= 0) {
                dKy_change_colpat(11);
                rv = 1;
            }
            break;

        case 0x14:
            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1E:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;
        
        case 0x28:
                rv = 1;
            break;
    }

    return rv;
}

int daNpc_Fairy_c::cutAppear_20F_01(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutAppear_20F_01_Init(*cutId);
    }

    return _cutAppear_20F_01_Main(*cutId);
}

int daNpc_Fairy_c::_cutAppear_20F_01_Init(int const& i_cutId) {
    int rv = 0;
    
    switch (i_cutId) {
        case 0xA:
            mMotionSeqMngr.setNo(MOT_SIT_A, -1.0f, FALSE, 0);
            mTimer = 61;
            dKy_change_colpat(10);
            Z2GetAudioMgr()->bgmStreamPrepare(0x200003F);
            break;

        case 0x14:
            mStatus = 0;
            mTimer = 120;
            dKy_change_colpat(12);
            Z2GetAudioMgr()->bgmStreamPlay();
            break;

        case 0x1E:
            mMotionSeqMngr.setNo(MOT_STANDUP, -1.0f, FALSE, 0);
            break;

        case 0x28:
            initTalk(mFlowNodeNo, NULL);
            break;

        case 0x3C:
            Z2GetAudioMgr()->bgmStreamStop(45);
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutAppear_20F_01_Main(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        case 0xA:
            if (--mTimer <= 0) {
                dKy_change_colpat(11);
                rv = 1;
            }
            break;

        case 0x14:
            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1E:
            if (mpMorf[0]->isStop()) {
                mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
                rv = 1;
            }
            break;
        
        case 0x28:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x3C:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Fairy_c::cutAppear_20F_02(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutAppear_20F_02_Init(*cutId);
    }

    return _cutAppear_20F_02_Main(*cutId);
}

int daNpc_Fairy_c::_cutAppear_20F_02_Init(int const& i_cutId) {
    int rv = 0;
    
    switch (i_cutId) {
        case 0xA:
            mMotionSeqMngr.setNo(MOT_SIT_A, -1.0f, FALSE, 0);
            mTimer = 61;
            dKy_change_colpat(10);
            Z2GetAudioMgr()->bgmStreamPrepare(0x200003F);
            break;

        case 0x14:
            mStatus = 0;
            mTimer = 120;
            dKy_change_colpat(12);
            Z2GetAudioMgr()->bgmStreamPlay();
            break;

        case 0x1E:
            mMotionSeqMngr.setNo(MOT_STANDUP, -1.0f, FALSE, 0);
            break;

        case 0x28:
            initTalk(mFlowNodeNo, NULL);
            break;

        case 0x32:
            Z2GetAudioMgr()->bgmStreamStop(45);
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutAppear_20F_02_Main(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        case 0xA:
            if (--mTimer <= 0) {
                dKy_change_colpat(11);
                rv = 1;
            }
            break;

        case 0x14:
            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1E:
            if (mpMorf[0]->isStop()) {
                mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
                rv = 1;
            }
            break;
        
        case 0x28:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x32:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Fairy_c::cutAppear_30F_01(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutAppear_30F_01_Init(*cutId);
    }

    return _cutAppear_30F_01_Main(*cutId);
}

int daNpc_Fairy_c::_cutAppear_30F_01_Init(int const& i_cutId) {
    int rv = 0;
    
    switch (i_cutId) {
        case 0xA:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            mTimer = 61;
            dKy_change_colpat(10);
            Z2GetAudioMgr()->bgmStreamPrepare(0x200003F);
            break;

        case 0x14:
            mStatus = 0;
            mTimer = 100;
            dKy_change_colpat(12);
            Z2GetAudioMgr()->bgmStreamPlay();
            break;

        case 0x1E:
            initTalk(mFlowNodeNo, NULL);
            break;

        case 0x32:
            Z2GetAudioMgr()->bgmStreamStop(45);
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutAppear_30F_01_Main(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        case 0xA:
            if (--mTimer <= 0) {
                dKy_change_colpat(11);
                rv = 1;
            }
            break;

        case 0x14:
            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1E:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x32:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Fairy_c::cutAppear_30F_02(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutAppear_30F_02_Init(*cutId);
    }

    return _cutAppear_30F_02_Main(*cutId);
}

int daNpc_Fairy_c::_cutAppear_30F_02_Init(int const& i_cutId) {
    int rv = 0;
    
    switch (i_cutId) {
        case 0xA:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            mTimer = 61;
            dKy_change_colpat(10);
            Z2GetAudioMgr()->bgmStreamPrepare(0x200003F);
            break;

        case 0x14:
            mStatus = 0;
            mTimer = 100;
            dKy_change_colpat(12);
            Z2GetAudioMgr()->bgmStreamPlay();
            break;

        case 0x1E:
            initTalk(mFlowNodeNo, NULL);
            break;

        case 0x28:
            Z2GetAudioMgr()->bgmStreamStop(0x2D);
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutAppear_30F_02_Main(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        case 0xA:
            if (--mTimer <= 0) {
                dKy_change_colpat(11);
                rv = 1;
            }
            break;

        case 0x14:
            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1E:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;
        
        case 0x28:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Fairy_c::cutAppear_40F_01(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutAppear_40F_01_Init(*cutId);
    }

    return _cutAppear_40F_01_Main(*cutId);
}

int daNpc_Fairy_c::_cutAppear_40F_01_Init(int const& i_cutId) {
    int rv = 0;
    
    switch (i_cutId) {
        case 0xA:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            mTimer = 61;
            dKy_change_colpat(10);
            Z2GetAudioMgr()->bgmStreamPrepare(0x200003F);
            break;

        case 0x14:
            mStatus = 0;
            mTimer = 100;
            Z2GetAudioMgr()->bgmStreamPlay();
            dKy_change_colpat(12);
            break;

        case 0x1E:
            initTalk(mFlowNodeNo, NULL);
            break;

        case 0x32:
            Z2GetAudioMgr()->bgmStreamStop(45);
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutAppear_40F_01_Main(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        case 0xA:
            if (--mTimer <= 0) {
                dKy_change_colpat(11);
                rv = 1;
            }
            break;

        case 0x14:
            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1E:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;
        
        case 0x32:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Fairy_c::cutAppear_40F_02(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutAppear_40F_02_Init(*cutId);
    }

    return _cutAppear_40F_02_Main(*cutId);
}

int daNpc_Fairy_c::_cutAppear_40F_02_Init(int const& i_cutId) {
    int rv = 0;
    
    switch (i_cutId) {
        case 0xA:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            mTimer = 61;
            dKy_change_colpat(10);
            Z2GetAudioMgr()->bgmStreamPrepare(0x200003F);
            break;

        case 0x14:
            mStatus = 0;
            mTimer = 100;
            Z2GetAudioMgr()->bgmStreamPlay();
            dKy_change_colpat(12);
            break;

        case 0x1E:
            initTalk(mFlowNodeNo, NULL);
            break;

        case 0x28:
            Z2GetAudioMgr()->bgmStreamStop(45);
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutAppear_40F_02_Main(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        case 0xA:
            if (--mTimer <= 0) {
                dKy_change_colpat(11);
                rv = 1;
            }
            break;

        case 0x14:
            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1E:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;
        
        case 0x28:
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Fairy_c::cutAppear_50F_01(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    int* piVar2 = NULL;
    int msgNo = 0;
    piVar2 = dComIfGp_evmng_getMyIntegerP(i_staffId, "msgNo");
    if (piVar2 != NULL) {
        msgNo = *piVar2;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutAppear_50F_01_Init(*cutId, msgNo);
    }

    return _cutAppear_50F_01_Main(*cutId, msgNo);
}

int daNpc_Fairy_c::_cutAppear_50F_01_Init(int const& i_cutId, int const& i_msgNo) {
    int rv = 0;
    
    switch (i_cutId) {
        case 0xA:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            mTimer = 61;
            dKy_change_colpat(10);
            Z2GetAudioMgr()->bgmStreamPrepare(0x200003F);
            break;

        case 0x14:
            mStatus = 0;
            mTimer = 100;
            Z2GetAudioMgr()->bgmStreamPlay();
            dKy_change_colpat(12);
            break;

        case 0x1E:
            daNpcF_offTmpBit(11);
            initTalk(mFlowNodeNo, NULL);
            mTimer = 143;
            fopAcM_SetSpeedF(this, 2.2f);
            mMotionSeqMngr.setNo(MOT_WALK, -1.0f, FALSE, 0);
            mFaceMotionSeqMngr.setNo(FACE_MOT_TALK, -1.0f, FALSE, 0);
            break;

        case 0x1F:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            fopAcM_SetSpeedF(this, 0.0f);
            break;

        case 0x28:
            initTalk(mFlowNodeNo, NULL);
            break;

        case 0x2A:
            mCurrentPosCopy = current.pos;
            mTimer = 30;
            break;

        case 0x32: {
            cXyz pos(0.309993f, -61250.0f, -3734.8193f);
            fopAcM_SetPosition(this, pos.x, pos.y, pos.z);
            fopAcM_SetSpeedF(this, 0.0f);
            daPy_getPlayerActorClass()->setAnimeFrame(tREG_F(1) + 32.0f);
            initTalk(mFlowNodeNo, NULL);
            break;
        }

        case 0x33:
            mpMorf[0]->setFrame(0.0f);
            daPy_getPlayerActorClass()->setAnimeFrame(tREG_F(2));
            break;

        case 0x3C:
            Z2GetAudioMgr()->bgmStreamStop(45);
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutAppear_50F_01_Main(int const& i_cutId, int const& i_msgNo) {
    int rv = 0;
    int msgNos[2] = {-1, -1};

    switch (i_cutId) {
        case 0xA:
            if (--mTimer <= 0) {
                dKy_change_colpat(11);
                rv = 1;
            }
            break;

        case 0x14:
            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1E:
            msgNos[0] = i_msgNo;
            talkProc(msgNos, FALSE, NULL, FALSE);

            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1F:
            msgNos[0] = i_msgNo;

            if (talkProc(msgNos, TRUE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x28:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                PresentDemoCall();
                rv = 1;
            }
            break;

        case 0x29:
            rv = 1;
            break;

        case 0x2A:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);

            mTimer--;
            if (mTimer <= 0) {
                fopAcM_SetSpeedF(this, 0.0f);
                rv = 1;
            } else if (mTimer == 10) {
                fopAcM_SetSpeedF(this, -4.0f);
            }
            break;

        case 0x32:
            msgNos[0] = i_msgNo;

            if (talkProc(msgNos, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x33:
            msgNos[0] = i_msgNo;

            if (talkProc(msgNos, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x34:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x3C:
            Z2GetAudioMgr()->seStart(Z2SE_SY_WARP_FADE_FAIRY, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dStage_changeScene(getSceneNo2(), 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Fairy_c::cutAppear_50F_02(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    int* piVar2 = NULL;
    int msgNo = 0;
    piVar2 = dComIfGp_evmng_getMyIntegerP(i_staffId, "msgNo");
    if (piVar2 != NULL) {
        msgNo = *piVar2;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutAppear_50F_02_Init(*cutId, msgNo);
    }

    return _cutAppear_50F_02_Main(*cutId, msgNo);
}

int daNpc_Fairy_c::_cutAppear_50F_02_Init(int const& i_cutId, int const& i_msgNo) {
    int rv = 0;
    
    switch (i_cutId) {
        case 0xA:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            mTimer = 61;
            dKy_change_colpat(10);
            Z2GetAudioMgr()->bgmStreamPrepare(0x200003F);
            break;

        case 0x14:
            mStatus = 0;
            mTimer = 100;
            Z2GetAudioMgr()->bgmStreamPlay();
            dKy_change_colpat(12);
            break;

        case 0x1E:
            daNpcF_offTmpBit(11);
            initTalk(mFlowNodeNo, NULL);
            mTimer = 143;
            fopAcM_SetSpeedF(this, 2.2f);
            mMotionSeqMngr.setNo(MOT_WALK, -1.0f, FALSE, 0);
            mFaceMotionSeqMngr.setNo(FACE_MOT_TALK, -1.0f, FALSE, 0);
            break;

        case 0x1F:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            fopAcM_SetSpeedF(this, 0.0f);
            break;

        case 0x20: {
            cXyz pos(0.309993f, -61250.0f, -3734.8193f);
            fopAcM_SetPosition(this, pos.x, pos.y, pos.z);
            fopAcM_SetSpeedF(this, 0.0f);
            daPy_getPlayerActorClass()->setAnimeFrame(1.0f);
            break;
        }

        case 0x21:
            mpMorf[0]->setFrameF(0.0f);
            daPy_getPlayerActorClass()->setAnimeFrame(tREG_F(2));
            break;
            
        case 0x28:
            Z2GetAudioMgr()->bgmStreamStop(45);
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutAppear_50F_02_Main(int const& i_cutId, int const& i_msgNo) {
    int rv = 0;
    int msgNos[2] = {-1, -1};

    switch (i_cutId) {
        case 0xA:
            if (--mTimer <= 0) {
                dKy_change_colpat(11);
                rv = 1;
            }
            break;

        case 0x14:
            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1E:
            msgNos[0] = i_msgNo;
            talkProc(msgNos, FALSE, NULL, FALSE);

            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1F:
            msgNos[0] = i_msgNo;

            if (talkProc(msgNos, TRUE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x20:
            msgNos[0] = i_msgNo;

            if (talkProc(msgNos, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x21:
            msgNos[0] = i_msgNo;

            if (talkProc(msgNos, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x22:
            msgNos[0] = i_msgNo;

            if (talkProc(msgNos, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x28:
            Z2GetAudioMgr()->seStart(Z2SE_SY_WARP_FADE_FAIRY, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dStage_changeScene(getSceneNo2(), 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Fairy_c::cutAppear_50F_03(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    int* piVar2 = NULL;
    int msgNo = 0;
    piVar2 = dComIfGp_evmng_getMyIntegerP(i_staffId, "msgNo");
    if (piVar2 != NULL) {
        msgNo = *piVar2;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutAppear_50F_03_Init(*cutId, msgNo);
    }

    return _cutAppear_50F_03_Main(*cutId, msgNo);
}

int daNpc_Fairy_c::_cutAppear_50F_03_Init(int const& i_cutId, int const& i_msgNo) {
    int rv = 0;
    
    switch (i_cutId) {
        case 0xA:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            mTimer = 31;
            dKy_change_colpat(10);
            Z2GetAudioMgr()->bgmStreamPrepare(0x200003F);
            break;

        case 0x14:
            mStatus = 0;
            mTimer = 100;
            Z2GetAudioMgr()->bgmStreamPlay();
            dKy_change_colpat(12);
            break;

        case 0x1E:
            initTalk(mFlowNodeNo, NULL);
            break;

        case 0x20:
            mTimer = 10;
            break;

        case 0x28: {
            cXyz pos(0.309993f, -61250.0f, -3734.8193f);
            fopAcM_SetPosition(this, pos.x, pos.y, pos.z);
            fopAcM_SetSpeedF(this, 0.0f);
            mMotionSeqMngr.setNo(MOT_HOPPE, -1.0f, FALSE, 0);
            mFaceMotionSeqMngr.setNo(FACE_MOT_SMILE, -1.0f, FALSE, 0);
            daPy_getPlayerActorClass()->setAnimeFrame(0.0f);
            initTalk(mFlowNodeNo, NULL);
            break;
        }
            
        case 0x32:
            Z2GetAudioMgr()->bgmStreamStop(45);
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutAppear_50F_03_Main(int const& i_cutId, int const& i_msgNo) {
    int rv = 0;
    int msgNos[2] = {-1, -1};

    switch (i_cutId) {
        case 0xA:
            if (--mTimer <= 0) {
                dKy_change_colpat(11);
                rv = 1;
            }
            break;

        case 0x14:
            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1E:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                PresentDemoCall();
                rv = 1;
            }
            break;

        case 0x1F:
            rv = 1;
            break;

        case 0x20:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);

            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x28:
            field_0xe26 = false;

            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x32:
            _to_FairyField();
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Fairy_c::cutAppear_50F_04(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    int* piVar2 = NULL;
    int msgNo = 0;
    piVar2 = dComIfGp_evmng_getMyIntegerP(i_staffId, "msgNo");
    if (piVar2 != NULL) {
        msgNo = *piVar2;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutAppear_50F_04_Init(*cutId, msgNo);
    }

    return _cutAppear_50F_04_Main(*cutId, msgNo);
}

int daNpc_Fairy_c::_cutAppear_50F_04_Init(int const& i_cutId, int const& i_msgNo) {
    int rv = 0;
    
    switch (i_cutId) {
        case 0xA:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            mTimer = 61;
            dKy_change_colpat(10);
            Z2GetAudioMgr()->bgmStreamPrepare(0x200003F);
            break;

        case 0x14:
            mStatus = 0;
            mTimer = 100;
            Z2GetAudioMgr()->bgmStreamPlay();
            dKy_change_colpat(12);
            break;

        case 0x1E:
            daNpcF_offTmpBit(11);
            initTalk(mFlowNodeNo, NULL);
            mTimer = 143;
            fopAcM_SetSpeedF(this, 2.2f);
            mMotionSeqMngr.setNo(MOT_WALK, -1.0f, FALSE, 0);
            mFaceMotionSeqMngr.setNo(FACE_MOT_TALK, -1.0f, FALSE, 0);
            break;

        case 0x1F:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            fopAcM_SetSpeedF(this, 0.0f);
            break;

        case 0x29: {
            cXyz pos(0.309993f, -61250.0f, -3734.8193f);
            fopAcM_SetPosition(this, pos.x, pos.y, pos.z);
            fopAcM_SetSpeedF(this, 0.0f);
            daPy_getPlayerActorClass()->setAnimeFrame(tREG_F(1) + 1.0f);
            break;
        }

        case 0x2A:
            mpMorf[0]->setFrameF(0.0f);
            daPy_getPlayerActorClass()->setAnimeFrame(tREG_F(2));
            break;

        case 0x32:
            Z2GetAudioMgr()->bgmStreamStop(45);
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutAppear_50F_04_Main(int const& i_cutId, int const& i_msgNo) {
    int rv = 0;
    int msgNos[2] = {-1, -1};

    switch (i_cutId) {
        case 0xA:
            if (--mTimer <= 0) {
                dKy_change_colpat(11);
                rv = 1;
            }
            break;

        case 0x14:
            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1E:
            msgNos[0] = i_msgNo;
            talkProc(msgNos, FALSE, NULL, FALSE);

            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1F:
            msgNos[0] = i_msgNo;
            
            if (talkProc(msgNos, TRUE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x28:
            msgNos[0] = i_msgNo;

            if (talkProc(msgNos, TRUE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x29:
            msgNos[0] = i_msgNo;

            if (talkProc(msgNos, TRUE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x2A:
            msgNos[0] = i_msgNo;

            if (talkProc(msgNos, TRUE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x2B:
            if (talkProc(NULL, TRUE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x32:
            Z2GetAudioMgr()->seStart(Z2SE_SY_WARP_FADE_FAIRY, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dStage_changeScene(getSceneNo2(), 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Fairy_c::cutAppear_50F_05(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    int* piVar2 = NULL;
    int msgNo = 0;
    piVar2 = dComIfGp_evmng_getMyIntegerP(i_staffId, "msgNo");
    if (piVar2 != NULL) {
        msgNo = *piVar2;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutAppear_50F_05_Init(*cutId, msgNo);
    }

    return _cutAppear_50F_05_Main(*cutId, msgNo);
}

int daNpc_Fairy_c::_cutAppear_50F_05_Init(int const& i_cutId, int const& i_msgNo) {
    int rv = 0;
    
    switch (i_cutId) {
        case 0xA:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            mTimer = 61;
            dKy_change_colpat(10);
            Z2GetAudioMgr()->bgmStreamPrepare(0x200003F);
            break;

        case 0x14:
            mStatus = 0;
            mTimer = 100;
            Z2GetAudioMgr()->bgmStreamPlay();
            dKy_change_colpat(12);
            break;

        case 0x1E:
            daNpcF_offTmpBit(11);
            initTalk(mFlowNodeNo, NULL);
            mTimer = 143;
            fopAcM_SetSpeedF(this, 2.2f);
            mMotionSeqMngr.setNo(MOT_WALK, -1.0f, FALSE, 0);
            mFaceMotionSeqMngr.setNo(FACE_MOT_TALK, -1.0f, FALSE, 0);
            break;

        case 0x1F:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            fopAcM_SetSpeedF(this, 0.0f);
            break;

        case 0x28:
            break;

        case 0x2A:
            mCurrentPosCopy = current.pos;
            mTimer = 30;
            break;

        case 0x32: {
            cXyz pos(0.309993f, -61250.0f, -3734.8193f);
            fopAcM_SetPosition(this, pos.x, pos.y, pos.z);
            fopAcM_SetSpeedF(this, 0.0f);
            daPy_getPlayerActorClass()->setAnimeFrame(tREG_F(1) + 32.0f);
            initTalk(mFlowNodeNo, NULL);
            break;
        }

        case 0x33:
            mpMorf[0]->setFrameF(0.0f);
            daPy_getPlayerActorClass()->setAnimeFrame(tREG_F(2));
            break;

        case 0x3C:
            Z2GetAudioMgr()->bgmStreamStop(45);
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutAppear_50F_05_Main(int const& i_cutId, int const& i_msgNo) {
    int rv = 0;
    int msgNos[2] = {-1, -1};

    switch (i_cutId) {
        case 0xA:
            if (--mTimer <= 0) {
                dKy_change_colpat(11);
                rv = 1;
            }
            break;

        case 0x14:
            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1E:
            msgNos[0] = i_msgNo;
            talkProc(msgNos, FALSE, NULL, FALSE);

            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1F:
            msgNos[0] = i_msgNo;
            
            if (talkProc(msgNos, TRUE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x28:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                PresentDemoCall();
                rv = 1;
            }
            break;

        case 0x29:
            rv = 1;
            break;

        case 0x2A:
            mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            mTimer--;

            if (mTimer <= 0) {
                fopAcM_SetSpeedF(this, 0.0f);
                rv = 1;
            } else if (mTimer == 10) {
                fopAcM_SetSpeedF(this, -4.0f);
            }
            break;

        case 0x32:
            msgNos[0] = i_msgNo;

            if (talkProc(msgNos, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x33:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;

        case 0x3C:
            Z2GetAudioMgr()->seStart(Z2SE_SY_WARP_FADE_FAIRY, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dStage_changeScene(getSceneNo2(), 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
            rv = 1;
            break;
    }

    return rv;
}

int daNpc_Fairy_c::cutSelect_Return1(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutSelect_Return1_Init(*cutId);
    }

    return _cutSelect_Return1_Main(*cutId);
}

int daNpc_Fairy_c::_cutSelect_Return1_Init(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        case 0xA:
            if (mType == TYPE_10F) {
                mMotionSeqMngr.setNo(MOT_SIT_A, -1.0f, FALSE, 0);
            } else {
                mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            }

            mTimer = 61;
            dKy_change_colpat(10);
            Z2GetAudioMgr()->bgmStreamPrepare(0x200003F);
            break;
        
        case 0x14:
            mStatus = 0;
            mTimer = 120;
            dKy_change_colpat(12);
            Z2GetAudioMgr()->bgmStreamPlay();
            break;

        case 0x1E:
            daNpcF_offTmpBit(11);
            initTalk(mFlowNodeNo, NULL);
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutSelect_Return1_Main(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        case 0xA:
            if (--mTimer <= 0) {
                dKy_change_colpat(11);
                rv = 1;
            }
            break;
        
        case 0x14:
            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1E:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;
    }

    return rv;
}

int daNpc_Fairy_c::cutSelect_Return2(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutSelect_Return2_Init(*cutId);
    }

    return _cutSelect_Return2_Main(*cutId);
}

int daNpc_Fairy_c::_cutSelect_Return2_Init(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        case 0xA:
            initTalk(mFlowNodeNo, NULL);
            break;
        
        case 0x14:
            Z2GetAudioMgr()->bgmStreamStop(45);
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutSelect_Return2_Main(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        case 0xA:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;
        
        case 0x14:
            u8 i_exitId;
            if (mFlow.getChoiceNo() == 0) {
                i_exitId = getSceneNo2();
                Z2GetAudioMgr()->seStart(Z2SE_SY_WARP_FADE_FAIRY, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            } else {
                i_exitId = getSceneNo1();
            }

            dStage_changeScene(i_exitId, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
            rv = 1;
    }

    return rv;
}

int daNpc_Fairy_c::cutSelect_Return3(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutSelect_Return3_Init(*cutId);
    }

    return _cutSelect_Return3_Main(*cutId);
}

int daNpc_Fairy_c::_cutSelect_Return3_Init(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        case 0xA:
            if (mType == TYPE_10F) {
                mMotionSeqMngr.setNo(MOT_SIT_A, -1.0f, FALSE, 0);
            } else {
                mMotionSeqMngr.setNo(MOT_WAIT, -1.0f, FALSE, 0);
            }

            mTimer = 61;
            dKy_change_colpat(10);
            Z2GetAudioMgr()->bgmStreamPrepare(0x200003F);
            break;
        
        case 0x14:
            mStatus = 0;
            mTimer = 120;
            dKy_change_colpat(12);
            Z2GetAudioMgr()->bgmStreamPlay();
            break;

        case 0x1E:
            daNpcF_offTmpBit(12);
            initTalk(mFlowNodeNo, NULL);
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutSelect_Return3_Main(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        case 0xA:
            if (--mTimer <= 0) {
                dKy_change_colpat(11);
                rv = 1;
            }
            break;
        
        case 0x14:
            if (--mTimer <= 0) {
                rv = 1;
            }
            break;

        case 0x1E:
            if (talkProc(NULL, FALSE, NULL, FALSE)) {
                rv = 1;
            }
            break;
    }

    return rv;
}

int daNpc_Fairy_c::cutReturnCancel(int i_staffId) {
    dEvent_manager_c& event_manager = dComIfGp_getEventManager();
    int* cutId = NULL;
    cutId = dComIfGp_evmng_getMyIntegerP(i_staffId, "cutId");
    if (cutId == NULL) {
        OS_REPORT("---------------CUT FALSE\n");
        return 1;
    }

    if (event_manager.getIsAddvance(i_staffId)) {
        _cutReturnCancel_Init(*cutId);
    }

    return _cutReturnCancel_Main(*cutId);
}

int daNpc_Fairy_c::_cutReturnCancel_Init(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        #if DEBUG
        case 0xA:
        #endif
        default:
            break;
    }

    return rv;
}

int daNpc_Fairy_c::_cutReturnCancel_Main(int const& i_cutId) {
    int rv = 0;

    switch (i_cutId) {
        #if DEBUG
        case 0xA:
        #endif
        default:
            break;
    }

    return rv;
}

static int daNpc_Fairy_Create(void* i_this) {
    return static_cast<daNpc_Fairy_c*>(i_this)->Create();
}

static int daNpc_Fairy_Delete(void* i_this) {
    return static_cast<daNpc_Fairy_c*>(i_this)->Delete();
}

static int daNpc_Fairy_Execute(void* i_this) {
    return static_cast<daNpc_Fairy_c*>(i_this)->Execute();
}

static int daNpc_Fairy_Draw(void* i_this) {
    return static_cast<daNpc_Fairy_c*>(i_this)->Draw();
}

static int daNpc_Fairy_IsDelete(void* i_this) {
    return true;
}

static actor_method_class daNpc_Fairy_MethodTable = {
    (process_method_func)daNpc_Fairy_Create,
    (process_method_func)daNpc_Fairy_Delete,
    (process_method_func)daNpc_Fairy_Execute,
    (process_method_func)daNpc_Fairy_IsDelete,
    (process_method_func)daNpc_Fairy_Draw,
};

actor_process_profile_definition g_profile_NPC_FAIRY = {
  fpcLy_CURRENT_e,          // mLayerID
  7,                        // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_NPC_FAIRY,           // mProcName
  &g_fpcLf_Method.base,    // sub_method
  sizeof(daNpc_Fairy_c),    // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  340,                      // mPriority
  &daNpc_Fairy_MethodTable, // sub_method
  0x08044108,               // mStatus
  fopAc_NPC_e,              // mActorType
  fopAc_CULLBOX_CUSTOM_e,   // cullType
};
