/**
 * @file d_a_obj_gra2.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_gra2.h"
#include "d/actor/d_a_npc4.h"
#include "d/actor/d_a_tag_gra.h"
#include "d/d_bg_w.h"
#include "d/d_cc_uty.h"
#include "d/d_com_inf_game.h"
#include "d/d_com_inf_actor.h"
#if DEBUG
#include "d/d_debug_viewer.h"
#endif
#include "d/d_msg_object.h"
#include "f_op/f_op_camera_mng.h"
#include "Z2AudioLib/Z2Instances.h"

class daObj_GrA_Param_c {
public:
    virtual ~daObj_GrA_Param_c() {}

    static const daObj_GrA_HIO_Param_c m;
};

#if DEBUG
class daObj_GrA_HIO_c : public fOpAcm_HIO_entry_c {
public:
    daObj_GrA_HIO_c() {
        m = daObj_GrA_Param_c::m;
    }

    void genMessage(JORMContext* ctext) {
        // featured offset
        ctext->genSlider("注目オフセット  ", &m.field_0x00, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // gravity
        ctext->genSlider("重力            ", &m.mGravity, -99.0f, 99.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // scale
        ctext->genSlider("スケ－ル        ", &m.field_0x08, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // interpolated frames
        ctext->genSlider("補間フレ－ム    ", &m.field_0x0c, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // height
        ctext->genSlider("高さ            ", &m.field_0x14, 0.0f, 999.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // knee-length
        ctext->genSlider("ひざ丈          ", &m.mWallR, 0.0f, 999.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // width
        ctext->genSlider("幅              ", &m.mWallH, 0.0f, 999.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // weight
        ctext->genSlider("体重            ", &m.mWeight, 0.0f, 255.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // back upper x angle
        ctext->genSlider("背のＸ角上限    ", &m.field_0x24, -90.0f, 90.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // back lower x angle
        ctext->genSlider("背のＸ角下限    ", &m.field_0x28, -90.0f, 90.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // back upper y angle
        ctext->genSlider("背のＹ角上限    ", &m.field_0x2c, -180.0f, 179.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // --- TODO ---
        ctext->genSlider("", &m.field_0x30, -180.0f, 179.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x34, -90.0f, 90.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x38, -90.0f, 90.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x3c, -180.0f, 179.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x40, -180.0f, 179.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x44, 0.0f, 1.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x48, 0.0f, 999.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x4c, 0.0f, 180.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x50, 0.0f, 999.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x54, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x56, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x58, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x5a, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x5c, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x60, 0.0f, 999.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x64, 0.0f, 999.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x68, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // Goron Soldier Guard
        ctext->genLabel("\n-----------------------【見張りゴロン兵】", 0, 0, NULL, -1, -1, 0x200, 0x18);
        ctext->genSlider("", &m.field_0x70, 0.0f, 1.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x74, 0.0f, 1.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x84, 0.0f, 1.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0xa8, 0.0f, 1.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x78, 0.0f, 1.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x9c, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0x9a, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // Rolling attack start demo parameters
        ctext->genLabel("\n○転がり攻撃開始デモ用パラメータ", 0, 0, NULL, -1, -1, 0x200, 0x18);
        ctext->genSlider("", &m.field_0xac, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0xae, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0xb0, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0xb2, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0xb4, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0xb8, 0.0f, 999.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genSlider("", &m.field_0xbc, 0.0f, 999.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // Goron Regular Soldier
        ctext->genLabel("\n-----------------------【ゴロン一般兵】", 0, 0, NULL, -1, -1, 0x200, 0x18);
        // search radius (10)
        ctext->genSlider("サ－チ半径(10)  ", &m.field_0x6c, 0.0f, 9999.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // turning speed
        ctext->genSlider("旋回速度        ", &m.field_0x88, 0, 0x4000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // walking speed
        ctext->genSlider("歩く速度        ", &m.field_0x90, 0.0f, 999.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // punch start distance
        ctext->genSlider("パンチ開始距離  ", &m.field_0x94, 0.0f, 999.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // post attack wait time
        ctext->genSlider("攻撃後待ち時間  ", &m.field_0x98, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // attack hand radius
        ctext->genSlider("攻撃時の手の半径", &m.field_0x8c, 0.0f, 999.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        // return speed
        ctext->genSlider("戻る速度        ", &m.field_0xa0, 0.0f, 9999.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
        ctext->genLabel("\n", 0, 0, NULL, -1, -1, 0x200, 0x18);
        // debug draw ON
        ctext->genCheckBox("デバグ描画ＯＮ  ", &m.field_0x8a, true, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
        // Export File
        ctext->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    }

    void listenPropertyEvent(const JORPropertyEvent*) {
        // TODO
    }

    daObj_GrA_HIO_Param_c m;
};
#endif

static char* l_resFileNameList[4] = {
    "grA",
    "grA",
    "grA",
    "grA",
};

static int l_exLoadRes_GRAa[4] = {
    1, 2, -1, -1,
};

static int l_exLoadRes_GRA_jump[4] = {
    1, 2, 4, -1,
};

static int l_exLoadRes_GRA_demo[4] = {
    1, 2, 3, -1,
};

static int* l_exloadRes_list[4] = {
    l_exLoadRes_GRAa,
    l_exLoadRes_GRA_jump,
    l_exLoadRes_GRAa,
    l_exLoadRes_GRA_demo,
};

static char* l_resNames[5] = {
    "grA",
    "grA_base",
    "grA_mdl",
    "grA_demo",
    "grA_Look",
};

static int l_bmdGetParamList[2] = {
    3, 2,
};

static int l_bckGetParamList[35][2] = {
    {21, 1}, {18, 0},
    {22, 1}, {11, 0},
    {18, 1}, {19, 1},
    {16, 0}, {-1, 0},
    {15, 1}, {16, 1},
    {6, 0}, {12, 0},
    {13, 0}, {14, 0},
    {17, 0}, {3, 3},
    {5, 3}, {15, 0},
    {6, 3}, {17, 1},
    {20, 1}, {3, 4},
    {14, 1}, {10, 0},
    {9, 1}, {7, 1},
    {8, 0}, {12, 1},
    {13, 1}, {10, 1},
    {8, 1}, {11, 1},
    {6, 0}, {4, 3},
    {9, 0},
};

static int l_btpGetParamList[4][2] = {
    {28, 1}, {32, 1},
    {31, 1}, {21, 0},
};

static u16 l_eventMotionChangeTable[11] = {
    0, 1, 0, 2,
    3, 0, 0, 0,
    0, 0, 0,
};

static u16 l_eventFaceChangeTable[4] = {
    1, 2, 6, 7,
};

static char* l_evtNameList[2] = {
    NULL,
    "GRA_THROWN",
};

static u16 l_entryJntNoList[4] = {
    5, 6, 7, -1,
};

static int jointCtrlCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daObj_GrA_c* i_this = (daObj_GrA_c*)model->getUserArea();
        if (i_this != NULL) {
            i_this->jointCtrl(i_joint, model);
        }
    }

    return 1;
}

void daObj_GrA_c::rideCallBack(dBgW* param_1, fopAc_ac_c* actor_p, fopAc_ac_c* param_3) {
    daObj_GrA_c* aActor_p = (daObj_GrA_c*) actor_p;
    JUT_ASSERT(684, NULL != aActor_p);
    aActor_p->field_0x10c4 = fopAcM_GetProfName(param_3) == PROC_ALINK;
}

daObj_GrA_c::daObj_GrA_c() {}

daObj_GrA_c::~daObj_GrA_c() {
    dComIfG_resDelete(&mPhase, getResName());
    for (int i = 0; l_exloadRes_list[mMode][i] >= 0; i++) {
        dComIfG_resDelete(&mPhases[i], l_resNames[l_exloadRes_list[mMode][i]]);
    }

    if (heap != NULL) {
        mpModelMorf->stopZelAnime();
    }

    if (mpBgw != NULL && mpBgw->ChkUsed()) {
        bool rel_ret = dComIfG_Bgsp().Release(mpBgw);
        if (rel_ret) {
            OS_REPORT("Release Error\n");
        }
    }

#if DEBUG
    if (mpHIO != NULL) {
        mpHIO->removeHIO(this);
    }
#endif
}

cPhs__Step daObj_GrA_c::create() {
    int uVar1 = 0;
    fopAcM_ct(this, daObj_GrA_c);

    mMode = getMode();
    int i;
    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, getResName());
    if (phase == cPhs_ERROR_e || phase == cPhs_UNK3_e) {
        return cPhs_ERROR_e;
    }

    if (phase == cPhs_COMPLEATE_e) {
        uVar1 += 1;
    }

    for (i = 0; l_exloadRes_list[mMode][i] >= 0; i++) {
        phase = (cPhs__Step)dComIfG_resLoad(&mPhases[i], l_resNames[l_exloadRes_list[mMode][i]]);
        if (phase == cPhs_ERROR_e || phase == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }

        if (phase == cPhs_COMPLEATE_e) {
            uVar1++;
        }
    }

    field_0x848 = ((int)home.angle.x == 0xFFFF) ? -1 : home.angle.x;

    field_0x1fe8 = (fopAcM_GetParam(this) & 0xC0000000) >> 30;
    field_0xa7f = home.angle.z & 0xFF;
    field_0x200c = (home.angle.z >> 8) & 0x7F;
    field_0x2030 = (home.angle.z >> 15) & 1;

    if (uVar1 == (i + 1) && (uVar1 == 3 || uVar1 == 4)) {
        int dzb_id = dComIfG_getObjctResName2Index(getResName(), "grA_stone_col.dzb");
        u32 heap_size = 0x3B10;
        if (MoveBGCreate(getResName(), dzb_id, NULL, heap_size) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        mpBgw->SetRideCallback(rideCallBack);
        if (isDelete()) {
            return cPhs_ERROR_e;
        } else {
            init();
        }
    } else {
        return cPhs_INIT_e;
    }

    return phase;
}

daObj_GrA_HIO_Param_c const daObj_GrA_Param_c::m = {
    85.0f,
    -4.0f,
    1.0f,
    12.0f,
    1,
    280.0f,
    40.0f,
    100.0f,
    255.0f,
    0.0f,
    0.0f,
    20.0f,
    -20.0f,
    40.0f,
    0.0f,
    40.0f,
    -40.0f,
    0.4f,
    85.0f,
    85.0f,
    50.0f,
    3,
    16,
    60,
    4,
    30,
    140.0f,
    150.0f,
    100,
    100.0f,
    15.0f,
    20.0f,
    1000.0f,
    3000.0f,
    15.0f,
    40.0f,
    2048,
    0,
    45.0f,
    10.0f,
    230.0f,
    60,
    1,
    0,
    25.0f,
    100,
    0.5f,
    10,
    10,
    30,
    40,
    20,
    60.0f,
    30.0f,
};

static const f32 l_bgcParam[20] = {
    0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 1.0f, -0.5f, 1.0f,
    0.5f, -1.0f, -0.5f, 1.0f,
    0.5f, -1.0f, 0.5f, -1.0f,
    -0.5f, 1.0f, 0.5f, -1.0f,
};

const s16 l_dirToAngleTBL[4] = {0, 0x4000, 0x8000, 0xC000};

int daObj_GrA_c::CreateHeap() {
    J3DModelData* aMdlData_p = NULL;
    field_0x844 = dKy_darkworld_check();

    aMdlData_p = (J3DModelData*)dComIfG_getObjectRes(l_resNames[l_bmdGetParamList[1]], l_bmdGetParamList[0]);
    JUT_ASSERT(854, NULL != aMdlData_p);
    u32 reg_r25 = 0x11020284;
    mpModelMorf = new mDoExt_McaMorfSO(aMdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mSound, 0x80000, reg_r25);
    if (mpModelMorf != NULL && mpModelMorf->getModel() == NULL) {
        mpModelMorf->stopZelAnime();
        mpModelMorf = NULL;
    }

    if (mpModelMorf == NULL) {
        return 0;
    }

    J3DModel* aMdl_p = mpModelMorf->getModel();
    for (u16 i = 0; i < aMdlData_p->getJointNum(); i++) {
        aMdlData_p->getJointNodePointer(i)->setCallBack(jointCtrlCallBack);
    }
    aMdl_p->setUserArea((uintptr_t)this);

    field_0x83c = l_entryJntNoList;
    setFaceAnm(22, false, 0.0f);
    setBaseAnm(17, 0.0f);
    
    return 1;
}

int daObj_GrA_c::Delete() {
    fopAcM_RegisterDeleteID(this, "OBJ_GRA");
    if (dComIfGp_getVibration().CheckQuake()) {
        dComIfGp_getVibration().StopQuake(31);
    }
    this->~daObj_GrA_c();
    return 1;
}

int daObj_GrA_c::Execute(Mtx** param_1) {
    int rv = 1;
    *param_1 = &mBgMtx;
    setParam();
    mainProc();

    if (field_0x109c == 0) {
        fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
        mAcch.CrrPos(dComIfG_Bgsp());
        field_0x9d4 = mAcch.m_gnd;
        mGroundY = mAcch.GetGroundH();

        if (mMode == 0 && !mAcch.ChkGroundHit()) {
            current.pos = old.pos;
        }
    }

    if (mGroundY != -G_CM3D_F_INF) {
        setEnvTevColor();
        setRoomNo();
    }

    allAnmPlay();
    adjustShapeAngle();
    setMtx(0);
    setCollisions();
    setAttnPos();
    setPrtcl();

    if (mpBgw != NULL && field_0x1fbc != field_0x109c) {
        field_0x1fbc = field_0x109c;
        if (field_0x1fbc != 0) {
            if (dComIfG_Bgsp().Regist(mpBgw, this)) {
                rv = 0;
            }
        } else if (dComIfG_Bgsp().Release(mpBgw)) {
            rv = 0;
        }
    }

    return rv;
}

int daObj_GrA_c::Draw() {
    f32 fVar1 = 800.0f;
    if (mMode == 3) {
        fVar1 = 900.0f;
    }

    if (field_0x844 == 0 || dComIfGs_wolfeye_effect_check()) {
        draw(fVar1);
    }

    return 1;
}

int daObj_GrA_c::jointCtrl(J3DJoint* i_joint, J3DModel* param_2) {
    Mtx MStack_78, MStack_a8;
    int jointNo = i_joint->getJntNo();
    cXyz spb4;
    mDoMtx_stack_c::copy(param_2->getAnmMtx(jointNo));

    switch (jointNo) {
        case 1:
        case 3:
        case 4:
            cMtx_copy(mDoMtx_stack_c::get(), MStack_78);
            spb4.set(MStack_78[0][3], MStack_78[1][3], MStack_78[2][3]);
            MStack_78[0][3] = MStack_78[1][3] = MStack_78[2][3] = 0.0f;
            mDoMtx_stack_c::ZXYrotS(shape_angle);
            field_0x9c2.x = field_0x9c8.x * mpHIO->m.field_0x44;
            field_0x9c2.y = field_0x9c8.y * mpHIO->m.field_0x44;
            field_0x9c2.z = field_0x9c8.z * mpHIO->m.field_0x44;

            switch (jointNo) {
                case 1:
                    break;
                    
                case 3:
                    mDoMtx_stack_c::ZXYrotM(field_0x9bc);
                    break;

                case 4:
                    mDoMtx_stack_c::ZXYrotM(field_0x9c2);
            }

            mDoMtx_stack_c::inverse();
            cMtx_copy(mDoMtx_stack_c::get(), MStack_a8);
            mDoMtx_stack_c::transS(spb4);
            mDoMtx_stack_c::ZXYrotM(field_0x91a);

            switch (jointNo) {
                case 1:
                    mDoMtx_stack_c::ZXYrotM(field_0x9bc);
                    break;

                case 3:
                    mDoMtx_stack_c::ZXYrotM(field_0x9c2);
                    break;

                case 4:
                    mDoMtx_stack_c::ZXYrotM(field_0x9c8);
                    break;
            }

            mDoMtx_stack_c::concat(MStack_a8);
            mDoMtx_stack_c::concat(MStack_78);
    }

    param_2->setAnmMtx(jointNo, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    if ((jointNo == 4 || jointNo == 7) && cLib_checkBit<u16>(field_0x840 & 0xFFFF, 0x20)) {
        J3DAnmTransform* anm = mBck.getBckAnm();
        mBck.changeBckOnly(mpModelMorf->getAnm());
        mpModelMorf->changeAnm(anm);
    }

    return 1;
}

const char* daObj_GrA_c::getResName() {
    return l_resFileNameList[mMode];
}

u8 daObj_GrA_c::getMode() {
    u32 uVar1 = fopAcM_GetParam(this) >> 28 & 3;
    strcpy(&field_0x744, "Obj_grA");

    switch (uVar1) {
        case 1:
            return 1;

        case 2:
            return 2;

        case 3:
            return 3;

        case 0:
        default:
            return 0;
    }
}

u8 daObj_GrA_c::getPathNo() {
    return fopAcM_GetParam(this) >> 20 & 0xFF;
}

u16 daObj_GrA_c::getTagNo() {
    return fopAcM_GetParam(this) >> 7 & 0x7F;
}

u16 daObj_GrA_c::getJumpType() {
    return fopAcM_GetParam(this) >> 7 & 0x7F;
}

BOOL daObj_GrA_c::isDelete() {
    if (mMode == 3) {
        if (!dComIfGs_isSwitch(field_0xa7f, fopAcM_GetRoomNo(this))) {
            return FALSE;
        }
    } else if (mMode == 2) {
        if (!isFirstGra() || !daNpcF_chkEvtBit(0x52)) {
            if (daPy_getPlayerActorClass() != NULL) {
                if (daPy_getPlayerActorClass()->current.pos.y < current.pos.y) {
                    return FALSE;
                }
            } else {
                return FALSE;
            }
        }
    } else {
        if (mMode == 1) {
            if (daNpcF_chkEvtBit(0x40)) {
                return FALSE;
            }
        } else {
            return FALSE;
        }
    }

    return TRUE;
}

void daObj_GrA_c::restart() {
    current.angle.set(0, home.angle.y, 0);
    shape_angle = current.angle;
    field_0x91a = current.angle;
    speedF = 0.0f;
    speed.set(0.0f, 0.0f, 0.0f);
    mEvtIdx = -1;
    field_0xa9c = -1;
    field_0xaa0 = 0;
    field_0xa8c = 1;

    if (field_0x844) {
        setLookMode(0);
    } else {
        setLookMode(1);
    }

    setProcess(&daObj_GrA_c::wait);
    field_0xa94 = 0.0f;
    field_0xa98 = 0.0f;
    health = mpHIO->m.field_0x10;
    ppMoveInit();
    field_0x1fbc = ~field_0x109c;
    field_0xaac = 0;
    field_0x1fe9 = 0;
    field_0x1fec = 0;
    field_0x1fe4 = -1;
    field_0x1ff0 = 0;
    field_0x1ff4 = 0;
    field_0x2000 = -1;
    mDemoCamMode = 0;
    field_0x2056 = 0;
    field_0x2004 = 0;
    field_0x2005 = 0;
    field_0x2008 = 0.0f;
    field_0x1fc4 = 0;
    field_0x1fd8 = 0;
    field_0x2031 = 0;
    field_0x1520 = 0;

    for (int i = 0; i < 3; i++) {
        field_0x2020[i] = -1;
    }

    field_0x201d = 0;
    field_0x201c = 0;
    field_0x2014 = 0;
    field_0x1fdc = 0;
    field_0x2032 = 0;
    mGraMode = 0;

    setBaseMotion(0, mpHIO->m.field_0x0c);
    setFaceMotion(0, -1.0f);
    field_0x204c.entry(NULL);
    field_0x209c = 0;
}

static cXyz l_centerOfst(0.0f, 67.0f, 26.0f);

static OBJ_GRA_HIO_CLASS l_HIO;

int daObj_GrA_c::init() {
    fopAcM_SetMtx(this, mpModelMorf->getModel()->getBaseTRMtx());
    mSound.init(&current.pos, &eyePos, 3, 1);
    eventInfo.setArchiveName((char*)getResName());
    mpHIO = &l_HIO;
#if DEBUG
    // "Goron"
    mpHIO->entryHIO("ゴロン");
#endif
    attention_info.distances[fopAc_attn_CARRY_e] = 40;
    attention_info.distances[fopAc_attn_BATTLE_e] = 22;
    mAcchCir.SetWall(mpHIO->m.mWallH, mpHIO->m.mWallR);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, 
              fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    mCcStts.Init(mpHIO->m.mWeight, 0, this);
    field_0xf50.Set(mCcDCyl);
    field_0xf50.SetStts(&mCcStts);
    field_0xce0[0].Set(mCcDSph);
    field_0xce0[0].SetStts(&mCcStts);
    field_0xce0[0].SetAtSe(11);
    field_0xce0[0].SetAtSpl((dCcG_At_Spl)1);
    field_0xce0[0].SetAtType(AT_TYPE_800);
    field_0xce0[0].OnAtSPrmBit(12);
    field_0xce0[0].SetAtAtp(1);
    field_0xce0[0].OffTgNoConHit();
    field_0xce0[0].OffCoSetBit();

    field_0xce0[1].Set(mCcDSph);
    field_0xce0[1].SetStts(&mCcStts);
    field_0xce0[1].SetAtSe(11);
    field_0xce0[1].SetAtSpl((dCcG_At_Spl)1);
    field_0xce0[1].SetAtType(AT_TYPE_800);
    field_0xce0[1].OnAtSPrmBit(12);
    field_0xce0[1].SetAtAtp(1);
    field_0xce0[1].OffTgNoConHit();
    field_0xce0[1].OffCoSetBit();

    mAcch.CrrPos(dComIfG_Bgsp());
    field_0x9d4 = mAcch.m_gnd;
    mGroundY = mAcch.GetGroundH();

    for (int i = 0; i < 6; i++) {
        field_0xab8[i].field_0x24.init(&mAcch, 0.0f, 0.0f);
    }

    setEnvTevColor();
    setRoomNo();

    if (mMode == 2) {
        fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x4000);
    }

    field_0x1528.setPathInfo(getPathNo(), fopAcM_GetRoomNo(this), 1);
    restart();
    return MoveBGExecute();
}

void daObj_GrA_c::setBaseAnm(int param_1, f32 i_morf) {
    J3DAnmTransformKey* transformKey = NULL;
    int i_mode = 2;
    int var_27 = 0;
    (void) var_27;

    if (l_bckGetParamList[param_1][0] >= 0) {
        transformKey = (J3DAnmTransformKey*)getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[param_1][1]], l_bckGetParamList[param_1][0]);
    }

    cLib_offBit<u16>(field_0x840, 0x11);

    switch (param_1) {
        case 0:
        case 1:
        case 2:
        case 7:
        case 9:
        case 18:
        case 19:
        case 21:
            break;

        case 3:
            i_mode = 0;
            break;

        case 4:
        case 5:
            i_mode = 0;
            break;

        case 6:
        case 14:
        case 20:
            i_mode = 0;
            break;

        case 8:
            i_mode = 0;
            break;

        case 10:
        case 11:
        case 12:
        case 13:
            i_mode = 0;
            break;

        case 15:
            i_mode = 0;
            break;

        case 16:
        case 17:
            i_mode = 0;
            break;

        default:
            transformKey = NULL;
    }

    if (transformKey != NULL && setMcaMorfAnm(transformKey, 1.0f, i_morf, i_mode, 0, -1) != 0) {
        field_0x842 = 0;
        cLib_onBit<u16>(field_0x840, 0x11);
        mpModelMorf->setMorf(i_morf);
    }
}

void daObj_GrA_c::setFaceAnm(int param_1, bool i_modify, f32 i_morf) {
    J3DAnmTransform* transform = NULL;
    int i_mode = 0;

    if (l_bckGetParamList[param_1][0] >= 0) {
        transform = getTrnsfrmKeyAnmP(l_resNames[l_bckGetParamList[param_1][1]], l_bckGetParamList[param_1][0]);
    }

    cLib_offBit<u16>(field_0x840, 0x44);
    cLib_offBit<u16>(field_0x840, 0x22);

    switch (param_1) {
        case 22:
        case 23:
        case 24:
        case 25:
            i_mode = 2;
            setFaceBtp(0);
            break;

        case 26:
        case 27:
        case 28:
            setFaceBtp(0);
            break;

        case 29:
            setFaceBtp(1);
            break;

        case 30:
            setFaceBtp(2);
            break;

        case 31:
            setFaceBtp(0);
            break;

        case 32:
            setFaceBtp(3);
            break;

        case 33:
            setFaceBtp(0);
            break;

        case 34:
            setFaceBtp(0);
            break;

        default:
            transform = NULL;
    }

    if (transform != NULL && setBckAnm(transform, 1.0f, i_mode, 0, -1, i_modify) != 0) {
        cLib_onBit<u16>(field_0x840, 0x22);
        mpModelMorf->setMorf(i_morf);
    }
}

void daObj_GrA_c::setFaceBtp(int param_1) {
    J3DAnmTexPattern* texPattern = NULL;
    int i_mode = 0;

    cLib_offBit<u16>(field_0x840, 0x44);

    if (l_btpGetParamList[param_1][0] >= 0) {
        texPattern = (J3DAnmTexPattern*)getTexPtrnAnmP(l_resNames[l_btpGetParamList[param_1][1]], l_btpGetParamList[param_1][0]);
    }

    switch (param_1) {
        case 0:
            i_mode = 2;
            break;

        case 1:
        case 2:
        case 3:
            break;

        default:
            texPattern = NULL;
    }

    if (field_0x844 != false) {
        texPattern = NULL;
    }

    if (texPattern != NULL && setBtpAnm(texPattern, mpModelMorf->getModel()->getModelData(), 1.0f, i_mode) != 0) {
        cLib_onBit<u16>(field_0x840, 0x44);
    }
}

void daObj_GrA_c::setBaseMotion(int param_1, f32 param_2) {
    if (field_0xa90 >= 22) {
        return;
    }

    field_0xa90 = param_1;
    field_0xa94 = param_2;
    field_0xa8d = 1;
}

void daObj_GrA_c::setFaceMotion(int param_1, f32 param_2) {
    if (field_0xa92 >= 14) {
        return;
    }

    field_0xa92 = param_1;
    field_0xa8e = 1;

    if (param_2 < 0.0f) {
        field_0xa98 = mpHIO->m.field_0x0c;
        return;
    }

    field_0xa98 = param_2;
}

void daObj_GrA_c::calcMotion() {
    if (field_0xa90 < 22 && mBaseMotionList[field_0xa90]) {
        field_0xa88 = (this->*mBaseMotionList[field_0xa90])(field_0xa8d);
    }

    if (field_0xa92 < 14 && mFaceMotionList[field_0xa92]) {
        (this->*mFaceMotionList[field_0xa92])(field_0xa8e);
    }

    field_0xa8d = 0;
    field_0xa8e = 0;
}

BOOL daObj_GrA_c::checkProcess(int (daObj_GrA_c::*process)(void*)) {
    return process == field_0xa50;
}

int daObj_GrA_c::setProcess(int (daObj_GrA_c::*process)(void*)) {
    field_0xa7c = 2;

    if (field_0xa50) {
        (this->*field_0xa50)(NULL);
    }

    field_0xa7c = 0;
    field_0xa50 = process;

    if (field_0xa50) {
        (this->*field_0xa50)(NULL);
    }

    field_0xa7c = 1;

    return 1;
}

static u8 getDistTableIdx(int param_1, int param_2) {
    return param_1 + param_2 * 20 + 94;
}

void daObj_GrA_c::setParam() {
    if (mMode == 0) {
        if ((( checkProcess(&daObj_GrA_c::standWait)
            || checkProcess(&daObj_GrA_c::walk))
            && field_0x150c == 0)
            || checkProcess(&daObj_GrA_c::rollReturn)
            || checkProcess(&daObj_GrA_c::toStone)) {
                attnSttsOn(0, 0);
        } else {
            attention_info.flags = (fopAc_AttnFlag_UNK_0x200000 | fopAc_AttnFlag_BATTLE_e);
        }
    } else if (mMode == 1) {
        if (checkProcess(&daObj_GrA_c::standWaitJump)) {
            attnSttsOn(1, 1);
            attention_info.distances[0] = getDistTableIdx(6, 6);
            attention_info.distances[1] = attention_info.distances[0];
            attention_info.distances[3] = getDistTableIdx(4, 6);
        } else {
            attention_info.flags = 0;
        }

        if (field_0x844 == false && daPy_py_c::checkNowWolf() != 0) {
            attention_info.flags = 0;
        }
    } else if (mMode == 2) {
        attention_info.flags &= ~fopAc_AttnFlag_CARRY_e;
        attention_info.flags = fopAc_AttnFlag_UNK_0x200000;
    } else {
        attention_info.flags &= ~fopAc_AttnFlag_CARRY_e;
        if (field_0x1508 == 0) {
            attnSttsOn(0, 0);
        }
    }

    scale.set(mpHIO->m.field_0x08, mpHIO->m.field_0x08, mpHIO->m.field_0x08);
    mCcStts.SetWeight(mpHIO->m.mWeight);
    gravity = mpHIO->m.mGravity;
}

BOOL daObj_GrA_c::checkEvent() {
    BOOL rv = TRUE;
#if VERSION != VERSION_SHIELD_DEBUG
    // TODO: gameInfo fake match to force reuse of pointer
    dComIfG_play_c* play = &g_dComIfG_gameInfo.play;
#endif
    if (dComIfGp_event_runCheck()) {
        rv = FALSE;
        if (eventInfo.checkCommandTalk()) {
            if (checkProcess(&daObj_GrA_c::talk)) {
                rv = (this->*field_0xa50)(NULL);
            } else if (dComIfGp_event_chkTalkXY() == FALSE || dComIfGp_evmng_ChkPresentEnd()) {
                setProcess(&daObj_GrA_c::talk);
            }
        } else if (eventInfo.checkCommandDemoAccrpt()) {
            if (dComIfGp_getEventManager().endCheck(mEvtIdx) != 0) {
#if VERSION != VERSION_SHIELD_DEBUG
                play->getEvent().reset();
#else
                dComIfGp_event_reset();
#endif
                field_0x1520 = 0;
                mEvtIdx = -1;
                rv = TRUE;
            }

            if (field_0x209c == 1) {
                rv = TRUE;
            }
        } else {
            rv = TRUE;
        }
    }

    return rv;
}

void daObj_GrA_c::mainProc() {
    fopAcM_setCullSizeBox(this, -160.0f, -100.0f, -160.0f, 160.0f, 240.0f, 160.0f);

    int chk_event = checkEvent();
    if (chk_event) {
        hitChk();
        if (field_0xa50) {
            (this->*field_0xa50)(NULL);
            if (mMode == 0) {
                if ((home.pos - current.pos).absXZ() > getSrchCircleR()) {
                    cXyz sp3c(0.0f, 0.0f, getSrchCircleR());
                    cLib_offsetPos(&current.pos, &home.pos, 
                                   cLib_targetAngleY(&home.pos, &current.pos), &sp3c);
                }
            }
        }

        if (chk_event) {
            orderEvent(field_0x1508, l_evtNameList[field_0x1520]);
        }
    }

    if (field_0xa8c != 0) {
        field_0xa94 = 0.0f;
        field_0xa98 = 0.0f;
        field_0xa8c = 0;
    }

    calcMotion();
}

void daObj_GrA_c::adjustShapeAngle() {
    /* empty function */
}

void daObj_GrA_c::setMtx(int param_1) {
    static cXyz aTrembleTrans[15] = {
        cXyz(0.0f, 0.0f, 0.0f),
        cXyz(1.154554f, -0.052719f, 1.018953f),
        cXyz(0.722208f, -0.14003f, 1.333208f),
        cXyz(-1.36857f, -0.249424f, 0.091892f),
        cXyz(0.492355f, -0.368397f, -1.149426f),
        cXyz(0.439399, -0.484451f, 1.838846f),
        cXyz(-1.57283f, -0.585079f, 0.435947f),
        cXyz(1.174968f, -0.657772f, -1.155991f),
        cXyz(1.337504f, -0.690033f, 1.076126f),
        cXyz(-0.945907f, -0.669357f, -0.03023f),
        cXyz(0.065675f, -0.583237f, -1.136583f),
        cXyz(1.077258f, -0.43885f, 0.185038f),
        cXyz(-0.006088f, -0.274368f, 1.459019f),
        cXyz(-1.227272f, -0.123749f, 0.182495f),
        cXyz(-0.767045f, -0.020973f, -1.134783f),
    };

    J3DModel* model = mpModelMorf->getModel();
    J3DModelData* modelData = model->getModelData();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        modelData->getJointNodePointer(i)->setCallBack(jointCtrlCallBack);
    }

    if (field_0x109c != 0) {
        cXyz sp30(l_centerOfst.x, l_centerOfst.y, l_centerOfst.z);
        mDoMtx_stack_c::transS(0.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::ZXYrotM(field_0x91a);
        sp30.x += aTrembleTrans[field_0x10a0 % 15].x;
        sp30.y += aTrembleTrans[field_0x10a0 % 15].y;
        sp30.z += aTrembleTrans[field_0x10a0 % 15].z;
        mDoMtx_stack_c::transM(sp30);
        mDoMtx_stack_c::multVecZero(&sp30);
        mDoMtx_stack_c::transS(current.pos.x + sp30.x, current.pos.y + sp30.y, current.pos.z + sp30.z);
        mDoMtx_stack_c::ZXYrotM(field_0x91a);

        cXyz sp3c(scale);
        mDoMtx_stack_c::scaleM(sp3c);
        cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
    }

    if (field_0x109c != 0) {
        cXyz sp48;
        mDoMtx_stack_c::transS(0.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::ZXYrotM(field_0x91a);
        mDoMtx_stack_c::transM(aTrembleTrans[field_0x10a0 % 15].x, aTrembleTrans[field_0x10a0 % 15].y, aTrembleTrans[field_0x10a0 % 15].z);
        mDoMtx_stack_c::multVecZero(&sp48);
        mDoMtx_stack_c::transS(current.pos.x + sp48.x, current.pos.y + sp48.y, current.pos.z + sp48.z);
    } else {
        mDoMtx_stack_c::transS(current.pos);
    }

    mDoMtx_stack_c::ZXYrotM(field_0x91a);
    mDoMtx_stack_c::scaleM(scale);

    model->setBaseTRMtx(mDoMtx_stack_c::get());

    if (param_1 != 0) {
        model->setUserArea((uintptr_t)this);
    } else {
        model->setUserArea(0);
    }

    if (cLib_checkBit<u16>(u16(field_0x840), 0x20) != 0) {
        mBck.getBckAnm()->setFrame(mBck.getFrame());
        mpModelMorf->modelCalc();
    } else {
        mpModelMorf->modelCalc();
    }
}

void daObj_GrA_c::setCollisions() {
    cXyz sp1c;

    if (field_0x1fc4 != 0 || field_0x844 != false) {
        field_0xf50.ClrTgHit();
    } else {
        if (field_0x109c == 0) {
            field_0xf50.SetAtType(AT_TYPE_800);
            if (mMode == 2) {
                field_0xf50.OnAtSetBit();
                field_0xf50.OffAtSPrmBit(12);
                field_0xf50.OnAtSPrmBit(16);
            } else {
                field_0xf50.OnAtSPrmBit(12);
            }

            field_0xf50.SetAtSe(10);
            field_0xf50.SetAtSpl((dCcG_At_Spl)13);
            field_0xf50.SetAtType(AT_TYPE_CSTATUE_SWING);

            if (mMode == 1) {
                field_0xf50.ClrTgHit();
            } else {
                field_0xf50.SetTgType(-1);
                field_0xf50.SetTgHitMark((CcG_Tg_HitMark)2);
                field_0xf50.SetTgSPrm(19);
                field_0xf50.OnTgShield();
                field_0xf50.OnTgNoConHit();
            }

            static cXyz aOfst(0.0f, 0.0f, 20.0f);

            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(field_0x91a.y);
            mDoMtx_stack_c::multVec(&aOfst, &sp1c);
            field_0xf50.SetC(sp1c);

            if (field_0xa90 == 10 || field_0xa90 == 4) {
                field_0xf50.SetH(200.0f);
            } else {
                field_0xf50.SetH(mpHIO->m.field_0x14);
            }

            field_0xf50.SetR(mpHIO->m.mWallH);

            if (field_0xf50.ChkAtSet()) {
                field_0xf50.SetR(mpHIO->m.mWallH + 40.0f);
            }

            if (cLib_calcTimer(&field_0x1fd8) == 0) {
                field_0xf50.OnCoSetBit();
            } else {
                field_0xf50.OffCoSetBit();
            }

            dComIfG_Ccsp()->Set(&field_0xf50);
        }

        if (checkProcess(&daObj_GrA_c::step)) {
            mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(11));
            mDoMtx_stack_c::scaleM(scale);
            mDoMtx_stack_c::multVecZero(&sp1c);
            field_0xce0[1].SetC(sp1c);
            field_0xce0[1].SetR(mpHIO->m.field_0x8c);
            dComIfG_Ccsp()->Set(&field_0xce0[1]);

            mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(17));
            mDoMtx_stack_c::scaleM(scale);
            mDoMtx_stack_c::multVecZero(&sp1c);
            field_0xce0[0].SetC(sp1c);
            field_0xce0[0].SetR(mpHIO->m.field_0x8c);
            dComIfG_Ccsp()->Set(&field_0xce0[0]);
        }
    }
}

void daObj_GrA_c::setAttnPos() {
    lookat();

    static cXyz aEyeOfst(15.0f, 25.0f, 0.0f);

    cXyz sp18;
    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVec(&aEyeOfst, &eyePos);
    mDoMtx_stack_c::multVecZero(&sp18);
    attention_info.position.set(sp18.x, sp18.y + mpHIO->m.field_0x00, sp18.z);
}

#if DEBUG
static s16 dummy_lit_124881(int sel) {
    const s16 arr[2] = {0x00C8, 0x0040};
    return arr[sel];
}

static s16 dummy_lit_124886(int sel) {
    const s16 arr[2] = {0x8000, 0x00A0};
    return arr[sel];
}
#endif

void daObj_GrA_c::drawOtherMdls() {
    /* empty function */
}

bool daObj_GrA_c::dbgDraw() {
    return true;
}

int daObj_GrA_c::ppMoveInit() {
    memset(&field_0x10ac, 0, 16);

    field_0x109c = 0;
    field_0x10a4 = 0;
    field_0x10a8 = 0;
    field_0x10bc = -1;
    field_0x10c4 = 0;
    field_0x14b0 = -1;
    field_0x14f0 = FLT_MAX;
    field_0x14f4 = -1;

    return 1;
}

int daObj_GrA_c::setLookMode(int iNo) {
    JUT_ASSERT(2656, 0 <= iNo && iNo < NUMLOOKMODES_e);
    mLookMode = iNo;
    field_0x1500 = 1;
    return 1;
}

static f32 dummy_lits(int sel) {
    if (sel == 0) {
        return 0.5f;
    } else if (sel == 1) {
        return 149.0f;
    } else if (sel == 2) {
        return 74.0f;
    } else if (sel == 3) {
        return 75.0f;
    } else {
        return 100.0f;
    }
}

int daObj_GrA_c::lookat() {
    J3DModel* model = mpModelMorf->getModel();
    s16 sVar1 = current.angle.y - old.angle.y;
    cXyz* pcVar11 = NULL;

    if (field_0x1500 != 0) {
        field_0x14f8.entry(NULL);
        switch (mLookMode) {
            case 0:
                break;

            case 1:
                field_0x14f8.entry(daPy_getPlayerActorClass());
                break;

            case 2:
                field_0x14f8.entry(field_0x204c.getActor());
                break;

            case 3:
                break;
        }
    }

    field_0x1500 = 0;

    csXyz acStack_60[4];
    csXyz acStack_78[4];
    acStack_60[0].set(cM_deg2s(-mpHIO->m.field_0x28) - 0x4000, cM_deg2s(mpHIO->m.field_0x2c), 0);
    acStack_60[1].set(-0x4000, 0, 0);
    acStack_60[2].set(cM_deg2s(-mpHIO->m.field_0x38), cM_deg2s(mpHIO->m.field_0x3c), 0);
    acStack_60[3].set(0, 0, 0);
    acStack_78[0].set(cM_deg2s(-mpHIO->m.field_0x24) - 0x4000, cM_deg2s(mpHIO->m.field_0x30), 0);
    acStack_78[1].set(-0x4000, 0, 0);
    acStack_78[2].set(cM_deg2s(-mpHIO->m.field_0x34), cM_deg2s(mpHIO->m.field_0x40), 0);
    acStack_78[3].set(0, 0, 0);
    cXyz sp48[4];

    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(1));
    mDoMtx_stack_c::multVecZero(&sp48[0]);

    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(3));
    mDoMtx_stack_c::multVecZero(&sp48[1]);

    mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(4));
    mDoMtx_stack_c::multVecZero(&sp48[2]);

    cXyz sp88(0.0f, 0.0f, 32.0f);
    mDoMtx_stack_c::transS(sp48[2]);
    mDoMtx_stack_c::YrotM(field_0x91a.y);
    mDoMtx_stack_c::multVec(&sp88, &sp48[3]);

    cXyz sp94, spa0;
    fopAc_ac_c* actor_p;
    switch (mLookMode) {
        case 1:
        case 2:
            actor_p = field_0x14f8.getActor();
            if (actor_p != NULL) {
                pcVar11 = &actor_p->eyePos;
            }
            break;

        case 3:
            spa0.set(0.0f, 0.0f, 200.0f);
            mDoMtx_stack_c::YrotS(field_0x91a.y);
            mDoMtx_stack_c::multVec(&spa0, &spa0);
            spa0 += current.pos;
            pcVar11 = &spa0;
            break;

        case 0:
            break;
    }
    mLookat.setAttnPos(pcVar11);

    for (int i = 0; i < 4; i++) {
        mLookat.setMaxJntLmt(acStack_60[i], i);
        mLookat.setMinJntLmt(acStack_78[i], i);
        mLookat.setJntPos(sp48[i], i);
    }
    mLookat.calc(this, model->getBaseTRMtx(), sVar1);

    csXyz* local_ac[3] = { &field_0x9bc, &field_0x9c2, &field_0x9c8 };
    int iVar2 = 0;
    for (int i = 2, iVar1 = 1; i >= 0; i--, iVar1++) {
        if (sVar1 == 0) {
            cLib_addCalcAngleS2(&local_ac[i]->x, mLookat.getRot(i).x, 8, 0x800 / iVar1);
            cLib_addCalcAngleS2(&local_ac[i]->y, mLookat.getRot(i).y, 8, 0x800 / iVar1);
            cLib_addCalcAngleS2(&local_ac[i]->z, mLookat.getRot(i).z, 8, 0x800 / iVar1);
        } else {
            local_ac[i]->x = mLookat.getRot(i).x;
            local_ac[i]->y = mLookat.getRot(i).y;
            local_ac[i]->z = mLookat.getRot(i).z;
        }

        iVar2 = (local_ac[i]->x != 0 || local_ac[i]->y != 0 || local_ac[i]->z != 0);
    }

    if (iVar2 != 0 && field_0x109c == 0) {
        setMtx(1);
    }

    return 1;
}

int daObj_GrA_c::hitChk() {
    if (field_0xaa4 != 0) {
        cLib_calcTimer(&field_0xaa4);
    } else {
        mCcStts.Move();
        if (mMode == 0 && field_0xf50.ChkTgHit() != 0) {
            field_0xab0.entry(field_0xf50.GetTgHitAc());

            if (field_0x1528.field_0xa18 != 0) {
                field_0xaa4 = mpHIO->m.field_0x54;
                def_se_set(&mSound, field_0xf50.GetTgHitObj(), 0x2C, NULL);
                setProcess(&daObj_GrA_c::rollReturn);
                health = 0;
                field_0x1528.field_0xa18 = 0;
            } else {
                def_se_set(&mSound, field_0xf50.GetTgHitObj(), 0x2C, NULL);
                if (health != 0) {
                    setProcess(&daObj_GrA_c::defence);
                }
            }

            field_0xf50.ClrTgHit();
        }
    }

    return 1;
}

int daObj_GrA_c::wait(void* param_1) {
    switch (field_0xa7c) {
        case 0: {
            if (field_0x844) {
                break;
            }

            switch (mMode) {
                case 0:
                    setBaseMotion(0, mpHIO->m.field_0x0c);
                    setFaceMotion(0, -1.0f);
                    setProcess(&daObj_GrA_c::standWait);
                    break;

                case 1:
                    setBaseMotion(0, mpHIO->m.field_0x0c);
                    setFaceMotion(0, -1.0f);
                    setProcess(&daObj_GrA_c::standWaitJump);
                    setLookMode(1);
                    break;

                case 2:
                    setProcess(&daObj_GrA_c::standRoll);
                    break;

                case 3:
                    setProcess(&daObj_GrA_c::waitDemo);
            }
        }

        case 1:
        case 2:
            break;
    }

    return 1;
}

int daObj_GrA_c::talk(void* param_1) {
    int iVar1, iVar2;
    int rv = 0;
    int iVar3;
    int iVar4 = field_0x848;
    s16 sVar1;
    switch (field_0xa7c) {
        case 0:
            field_0x1fcc = 0;
            mMsgFlow.init(this, iVar4, 0, NULL);
            setLookMode(1);
            field_0xaa0 = 0;
            break;

        case 1:
            sVar1 = cLib_targetAngleY(&current.pos, &daPy_getPlayerActorClass()->current.pos);
            if (sVar1 != field_0x91a.y) {
                if (field_0x1fcc < 2) {
                    if (turn_step(sVar1, 0, 19, 20)) {
                        setBaseMotion(0, mpHIO->m.field_0x0c);
                        setFaceMotion(0, -1.0f);
                    }
                    
                    return 0;
                }

                turn_step(sVar1, -1, -1, 20);
            }

            if (mMsgFlow.doFlow(this, NULL, 0) != 0) {
                #if DEBUG
                char* cVar1;
                int choiceNo = mMsgFlow.getChoiceNo();
                if (choiceNo != 0) {
                    cVar1 = "はい"; // Yes
                } else {
                    cVar1 = "いいえ"; // No
                }
                OS_REPORT("二択分岐 %s %d\n", cVar1, choiceNo);
                #endif

                if (mMsgFlow.getChoiceNo() == 0) {
                    setProcess(&daObj_GrA_c::rollReturn);
                } else {
                    setProcess(&daObj_GrA_c::wait);
                }

                rv = 1;
            }

            iVar3 = field_0xaa0;
            if (ctrlMsgAnm(iVar1, iVar2, this) != 0) {
                if (iVar2 != -1) {
                    setBaseMotion(l_eventMotionChangeTable[iVar2], mpHIO->m.field_0x0c);
                }

                if (iVar1 != -1) {
                    setFaceMotion(l_eventFaceChangeTable[iVar1], -1.0f);
                }
            } else if (iVar3 != 0) {
                setFaceTalkAfter();
            }
            break;

        case 2:
            dComIfGp_event_reset();
            rv = 1;
            field_0x1fcc = 0;
    }

    return rv;
}

int daObj_GrA_c::ctrlMsgAnm(int& param_1, int& param_2, fopAc_ac_c* param_3) {
    param_1 = -1;
    param_2 = -1;

    dMsgObject_c* msg_p = (dMsgObject_c*)dMsgObject_c::getActor();
    JUT_ASSERT(3163, NULL != msg_p);

    if (msg_p->mode == 2 || msg_p->mode == 3) {
        field_0xa9c = -1;
    } else if (msg_p->mode == 6) {
        if (msg_p->msg_idx != field_0xa9c) {
            param_1 = dComIfGp_getMesgFaceAnimeAttrInfo();
            param_2 = dComIfGp_getMesgAnimeAttrInfo();
            field_0xa9c = msg_p->msg_idx;
        }

        if (dMsgObject_isMouthCheck()) {
            cLib_offBit<u16>(field_0x840, 0x100);
        } else {
            cLib_onBit<u16>(field_0x840, 0x100);
        }

        field_0xaa0 = 20;
    } else {
        if (cLib_checkBit<u16>(field_0x840, 0x100) != 0) {
            cLib_offBit<u16>(field_0x840, 0x100);
        }

        field_0xa9c = -1;
    }

    cLib_calcTimer(&field_0xaa0);

    return field_0xaa0;
}

static void* s_sub(void* i_actor, void* i_data) {
    daObj_GrA_c* i_this = (daObj_GrA_c*)i_data;
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_TAG_GRA && i_this->checkTagGraSub((fopAc_ac_c*)i_actor)) {
        return i_actor;
    }
    return NULL;
}

bool daObj_GrA_c::checkTagGraSub(fopAc_ac_c* a_this) {
    daTagGra_c* i_this = (daTagGra_c*)a_this;
    if (getTagNo() == i_this->getGraNo()) {
        return 1;
    } else {
        return 0;
    }
}

bool daObj_GrA_c::fallAbyssCheck() {
    if (mGroundY == -G_CM3D_F_INF && field_0x1528.isPath()) {
        field_0x1fc0++;
        if (field_0x1fc0 == 30) {
            current.pos = field_0x1528.getPntPos(0);
            old.pos = current.pos;
            field_0x1528.setIdx(0);
            field_0x1fc0 = 0;

            return true;
        }
    } else {
        field_0x1fc0 = 0;
    }

    return false;
}

static f32 dummy_lits_01(int sel) {
    if (sel == 0) {
        return 0.25f;
    } else {
        return 3.5f;
    }
}

int daObj_GrA_c::setPrtcl() {
    static u16 const l_prticles_id[3] = {
        0x83B5,
        0x83B6,
        0x83B7,
    };

    JPABaseEmitter* centerEmitter = NULL;
    int iVar1 = 0;
    int simply_zero = 0;

    for (int i = 0; i < 6; i++) {
        if (i == 2 || i == 3 || i == 5) {
            iVar1 = 1;
        } else {
            iVar1 = 0;
        }

        field_0xab8[i].field_0x24.setEffectCenter(&tevStr, &field_0xab8[i].field_0x4, iVar1, simply_zero,
                                                  (field_0xab8[i].field_0x0 == true) ? &field_0xab8[i].field_0x4 : NULL,
                                                  (field_0xab8[i].field_0x0 == true) ? &field_0xab8[i].field_0x10 : NULL,
                                                  NULL, fopAcM_GetRoomNo(this), 0.0f, speedF);

        if (field_0xab8[i].field_0x0 != 0) {
            centerEmitter = field_0xab8[i].field_0x24.getCenterEmitter(0, 0);
            if (centerEmitter != NULL) {
                switch (i) {
                    case 0:
                    case 1:
                        field_0xab8[i].field_0x18.set(0.7f, 0.7f, 0.7f);
                        centerEmitter->setAwayFromCenterSpeed(0.0f);
                        centerEmitter->setRandomDirectionSpeed(0.0f);
                        centerEmitter->setSpread(1.0f);
                        centerEmitter->setGlobalParticleScale(field_0xab8[i].field_0x18);
                        break;

                    case 5:
                        field_0xab8[i].field_0x18.set(2.0f, 2.0f, 2.0f);
                        break;

                    case 4:
                        centerEmitter->setVolumeSize(40);
                        centerEmitter->setVolumeMiniRadius(1.0f);
                        centerEmitter->setDirectionalSpeed(0.0f);
                        break;
                }
            }
        }

        field_0xab8[i].field_0x0 = 0;
    }

    if (field_0x201d == 1) {
        JPABaseEmitter* emitter = NULL;
        cXyz sp28(current.pos);
        cXyz sp34(1.0f, 1.0f, 1.0f);

        for (int i = 0; i < 3; i++) {
            if (i == 0) {
                field_0x2020[i] = dComIfGp_particle_set(field_0x2020[i], l_prticles_id[i], &sp28, &tevStr, &current.angle, 
                                                        &sp34, 0xFF, NULL, -1, NULL, NULL, NULL);
            } else {
                field_0x2020[i] = dComIfGp_particle_setPolyColor(field_0x2020[i], l_prticles_id[i], mAcch.m_gnd, &sp28, 
                                                                    &tevStr, &current.angle, &sp34, 0, NULL, -1, NULL);
            }

            emitter = dComIfGp_particle_getEmitter(field_0x2020[i]);
            if (emitter != NULL) {
                emitter->setGlobalTranslation(sp28.x, sp28.y, sp28.z);
            }
        }
    }

    field_0x201d = 0;

    return 1;
}

#include "d/actor/d_a_obj_gra2_soldier.inc"
#include "d/actor/d_a_obj_gra2_base.inc"

int daObj_GrA_c::base000(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(0, field_0xa94);
    }

    return 1;
}

int daObj_GrA_c::base001(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(2, field_0xa94);
    }

    return 1;
}

int daObj_GrA_c::base003(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(4, field_0xa94);
    } else if (mpModelMorf->isStop()) {
        setBaseMotion(0, mpHIO->m.field_0x0c);
        setBaseAnm(0, field_0xa94);
    }

    return 1;
}

int daObj_GrA_c::base004(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(5, field_0xa94);
    } else if (mpModelMorf->isStop()) {
        setBaseMotion(0, mpHIO->m.field_0x0c);
        setBaseAnm(0, field_0xa94);
    }

    return 1;
}

int daObj_GrA_c::base005(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(6, field_0xa94);
    }

    if (mpModelMorf->getFrame() >= 4.5f && mpModelMorf->getFrame() <= 5.5f) {
        mSound.startCreatureVoice(Z2SE_GORON_V_TO_STONE, -1);
    }

    return 1;
}

int daObj_GrA_c::base006(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(7, field_0xa94);
    }

    return 1;
}

int daObj_GrA_c::base007(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(8, field_0xa94);
        mSound.startCreatureVoice(Z2SE_GORON_V_RECOVER, -1);
    }

    return 1;
}

int daObj_GrA_c::base008(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(10, field_0xa94);
    }

    if (mpModelMorf->checkFrame(2.0f)) {
        mSound.startCreatureVoice(Z2SE_GORON_V_GUARD, -1);
    }

    return 1;
}

int daObj_GrA_c::base009(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(1, field_0xa94);
    }

    if (mpModelMorf->checkFrame(12.0f) || mpModelMorf->checkFrame(30.0f)) {
        mSound.startCreatureVoice(Z2SE_GORON_V_FIGHTING_WAIT, -1);
    }

    return 1;
}

int daObj_GrA_c::base010(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(3, field_0xa94);
    }

    if (mpModelMorf->getFrame() == 4.0f || ( mpModelMorf->getFrame() >= 18.5f && mpModelMorf->getFrame() <= 19.5f)) {
        mSound.startCreatureVoice(Z2SE_GORON_V_PUNCH, -1);
    }

    return 1;
}

int daObj_GrA_c::base013(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(9, field_0xa94);
        mpModelMorf->setEndFrame(30.0f);
        field_0x2010 = 0;
    }

    if (mAcch.ChkGroundHit()) {
        if (mpModelMorf->getPlaySpeed() >= 1.0f || mpModelMorf->getPlaySpeed() <= -1.0f) {
            field_0x201d = 1;
        }

        field_0x201c = 0;
    } else if (field_0x201c < 10) {
        field_0x201c++;
    }
    
    if (field_0x2010 != field_0x842 && field_0x201c < 10) {
        mSound.startCreatureSound(Z2SE_GORON_ROLLING, cLib_minMaxLimit(fabsf(mpModelMorf->getPlaySpeed()) * 20.0f, 1.0f, 127.0f), -1);
        field_0x2010 = field_0x842;
    }

    return 1;
}

int daObj_GrA_c::base016(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(11, field_0xa94);
        mSound.startCreatureVoice(Z2SE_GORON_V_HEAD_BACK, -1);
    }

    return 1;
}

int daObj_GrA_c::base017(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(12, field_0xa94);
        mSound.startCreatureVoice(Z2SE_GORON_V_HEAD_BACK, -1);
    }

    return 1;
}

int daObj_GrA_c::base018(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(13, field_0xa94);
        mpModelMorf->setFrame(6.0f);
        mSound.startCreatureVoice(Z2SE_GORON_V_TOTTER, -1);
    }

    return 1;
}

int daObj_GrA_c::base019(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(14, field_0xa94);
        mpModelMorf->setEndFrame(20.0f);
        mSound.startCreatureVoice(Z2SE_GORON_V_HIT_TO_STONE, -1);
    }

    return 1;
}

int daObj_GrA_c::base020(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(15, field_0xa94);
    }

    return 1;
}

int daObj_GrA_c::base021(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(16, field_0xa94);
    }

    return 1;
}

int daObj_GrA_c::base022(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(17, field_0xa94);
    }

    return 1;
}

int daObj_GrA_c::base023(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(18, field_0xa94);
    }

    return 1;
}

int daObj_GrA_c::base024(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(19, field_0xa94);
    }

    return 1;
}

int daObj_GrA_c::base025(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(20, field_0xa94);
    }

    return 1;
}

int daObj_GrA_c::base026(int param_1) {
    if (param_1 != 0) {
        setBaseAnm(21, field_0xa94);
    }

    return 1;
}

int daObj_GrA_c::face000(int param_1) {
    if (param_1 != 0) {
        setFaceAnm(22, true, field_0xa98);
    }

    return 1;
}

int daObj_GrA_c::face003(int param_1) {
    if (param_1 != 0) {
        setFaceAnm(27, true, field_0xa98);
    } else if (mBck.isStop()) {
        setFaceMotion(0, -1.0f);
        setFaceAnm(22, true, field_0xa98);
    }

    return 1;
}

int daObj_GrA_c::face004(int param_1) {
    if (param_1 != 0) {
        setFaceAnm(28, true, field_0xa98);
    } else if (mBck.isStop()) {
        setFaceMotion(0, -1.0f);
        setFaceAnm(22, true, field_0xa98);
    }

    return 1;
}

int daObj_GrA_c::face005(int param_1) {
    if (param_1 != 0) {
        setFaceAnm(31, true, field_0xa98);
    }

    return 1;
}

int daObj_GrA_c::face006(int param_1) {
    if (param_1 != 0) {
        setFaceAnm(24, true, field_0xa98);
    }

    return 1;
}

int daObj_GrA_c::face007(int param_1) {
    if (param_1 != 0) {
        setFaceAnm(25, true, field_0xa98);
    }

    return 1;
}

int daObj_GrA_c::face008(int param_1) {
    if (param_1 != 0) {
        setFaceAnm(29, true, field_0xa98);
    } else if (mBck.isStop()) {
        setFaceMotion(4, -1.0f);
        setFaceAnm(24, true, field_0xa98);
    }

    return 1;
}

int daObj_GrA_c::face009(int param_1) {
    if (param_1 != 0) {
        setFaceAnm(30, true, field_0xa98);
    } else if (mBck.isStop()) {
        setFaceMotion(5, -1.0f);
        setFaceAnm(25, true, field_0xa98);
    }

    return 1;
}

int daObj_GrA_c::face010(int param_1) {
    if (param_1 != 0) {
        setFaceAnm(23, true, field_0xa98);
    }

    return 1;
}

int daObj_GrA_c::face011(int param_1) {
    if (param_1 != 0) {
        setFaceAnm(26, true, field_0xa98);
    }

    return 1;
}

int daObj_GrA_c::face012(int param_1) {
    if (param_1 != 0) {
        setFaceAnm(32, true, field_0xa98);
    }

    return 1;
}

int daObj_GrA_c::face016(int param_1) {
    if (param_1 != 0) {
        setFaceAnm(33, true, field_0xa98);
    }

    return 1;
}

int daObj_GrA_c::face017(int param_1) {
    if (param_1 != 0) {
        setFaceAnm(34, true, field_0xa98);
    }

    return 1;
}

int daObj_GrA_c::face999(int param_1) {
    if (param_1 != 0) {
        int reg_r30 = 0x22;
        cLib_offBit<u16>(field_0x840, reg_r30);
        setFaceBtp(0);
    }

    return 1;
}

int daObj_GrA_c::evtcutTalk(int param_1, int param_2) {
    s32 sVar1 = field_0x848;
    if (param_2 != 0) {
        mMsgFlow.init(this, sVar1, 0, NULL);
        field_0xaa0 = 0;
    }

    if (mMsgFlow.doFlow(this, NULL, 0)) {
        return 1;
    }

    int iVar1, iVar2;
    int iVar3 = field_0xaa0;
    if (ctrlMsgAnm(iVar1, iVar2, this) != 0) {
        if (iVar2 != -1) {
            setBaseMotion(l_eventMotionChangeTable[iVar2], mpHIO->m.field_0x0c);
        }

        if (iVar1 != -1) {
            setFaceMotion(l_eventFaceChangeTable[iVar1], -1.0f);
        }
    } else {
        if (iVar3 != 0) {
            setFaceTalkAfter();
        }
    }

    return 0;
}

void daObj_GrA_c::setFaceTalkAfter() {
    switch (field_0xa92) {
        case 6:
            setFaceMotion(4, mpHIO->m.field_0x0c);
            break;

        case 7:
            setFaceMotion(5, mpHIO->m.field_0x0c);
            break;

        default:
            setFaceMotion(0, mpHIO->m.field_0x0c);
    }
}

static int daObj_GrA_Create(void* i_this) {
    return ((daObj_GrA_c*)i_this)->create();
}

static int daObj_GrA_Delete(void* i_this) {
    return ((daObj_GrA_c*)i_this)->MoveBGDelete();
}

static int daObj_GrA_Execute(void* i_this) {
    return ((daObj_GrA_c*)i_this)->MoveBGExecute();
}

static int daObj_GrA_Draw(void* i_this) {
    return ((daObj_GrA_c*)i_this)->MoveBGDraw();
}

static int daObj_GrA_IsDelete(void* i_this) {
    return 1;
}

AUDIO_INSTANCES;

daObj_GrA_c::MotionFunc daObj_GrA_c::mBaseMotionList[22] = {
    &daObj_GrA_c::base000,
    &daObj_GrA_c::base001,
    &daObj_GrA_c::base003,
    &daObj_GrA_c::base004,
    &daObj_GrA_c::base005,
    &daObj_GrA_c::base006,
    &daObj_GrA_c::base007,
    &daObj_GrA_c::base008,
    &daObj_GrA_c::base009,
    &daObj_GrA_c::base010,
    &daObj_GrA_c::base013,
    &daObj_GrA_c::base016,
    &daObj_GrA_c::base017,
    &daObj_GrA_c::base018,
    &daObj_GrA_c::base019,
    &daObj_GrA_c::base020,
    &daObj_GrA_c::base021,
    &daObj_GrA_c::base022,
    &daObj_GrA_c::base023,
    &daObj_GrA_c::base024,
    &daObj_GrA_c::base025,
    &daObj_GrA_c::base026,
};

daObj_GrA_c::MotionFunc daObj_GrA_c::mFaceMotionList[14] = {
    &daObj_GrA_c::face000,
    &daObj_GrA_c::face003,
    &daObj_GrA_c::face004,
    &daObj_GrA_c::face005,
    &daObj_GrA_c::face006,
    &daObj_GrA_c::face007,
    &daObj_GrA_c::face008,
    &daObj_GrA_c::face009,
    &daObj_GrA_c::face010,
    &daObj_GrA_c::face011,
    &daObj_GrA_c::face012,
    &daObj_GrA_c::face016,
    &daObj_GrA_c::face017,
    &daObj_GrA_c::face999,
};

static actor_method_class daObj_GrA_MethodTable = {
    (process_method_func)daObj_GrA_Create,
    (process_method_func)daObj_GrA_Delete,
    (process_method_func)daObj_GrA_Execute,
    (process_method_func)daObj_GrA_IsDelete,
    (process_method_func)daObj_GrA_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_GRA = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_OBJ_GRA,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObj_GrA_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  19,                     // mPriority
  &daObj_GrA_MethodTable, // sub_method
  0x00040100,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
