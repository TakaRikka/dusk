/**
 * @file d_a_npc_shad.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_shad.h"
#include "d/actor/d_a_cstaF.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_tag_evtarea.h"
#include "d/d_msg_object.h"

const daNpcShad_HIOParam daNpcShad_Param_c::m = {
    35.0f,      // attention_offset 
    -3.0f,      // gravity
    1.0f,       // scale 
    400.0f,     // real_shadow_size
    255.0f,     // weight
    200.0f,     // height
    35.0f,      // knee_length
    30.0f,      // width
    0.0f,       // body_angleX_max 
    0.0f,       // body_angleX_min 
    45.0f,       // body_angleY_max
    -45.0f,       // body_angleY_min
    30.0f,      // head_angleX_max
    -10.0f,       // head_angleX_min
    40.0f,      // head_angleY_max
    -40.0f,     // head_angleY_min
    0.6f,       // neck_rotation_ratio
    12.0f,      // morf_frame
    3,          // talk_distance
    6,          // talk_angle
    5,          // attention_distance
    6,          // attention_angle
    80.0f,     // fov
    500.0f,     // search_distance
    300.0f,     // search_height
    -300.0f,    // search_depth
    60,         // attention_time
    8,          // damage_time
    0,          // face_expression
    0,          // motion
    0,          // look_mode
    false,          // debug_mode_ON
    false,          // debug_info_ON
    12.0f, // traveling_speed
};

#if DEBUG
daNpcShad_HIO_c::daNpcShad_HIO_c() {
    m = daNpcShad_Param_c::m;
}

void daNpcShad_HIO_c::genMessage(JORMContext* ctext) {
    // TODO
}
#endif

enum Shad_RES_File_ID {
    /* BCK */
    /* 0x07 */ BCK_SHAD_F_DISCOURAGED = 0x7,
    /* 0x08 */ BCK_SHAD_F_HAPPY,
    /* 0x09 */ BCK_SHAD_F_TALK_A,
    /* 0x0A */ BCK_SHAD_FH_DISCOURAGED,
    /* 0x0B */ BCK_SHAD_FH_HAPPY,
    /* 0x0C */ BCK_SHAD_RUN_A,

    /* BMDR */
    /* 0x0F */ BMDR_SHAD = 0xF,
    /* 0x10 */ BMDR_SHAD_BOOK_KNIFE,

    /* BTK */
    /* 0x13 */ BTK_SHAD = 0x13,

    /* BTP */
    /* 0x16 */ BTP_SHAD = 0x16,
    /* 0x17 */ BTP_SHAD_F_DISCOURAGED,
    /* 0x18 */ BTP_SHAD_F_HAPPY,
    /* 0x19 */ BTP_SHAD_FH_DISCOURAGED,
    /* 0x1A */ BTP_SHAD_FH_HAPPY,

    /* EVT */
    /* 0x1D */ EVT_EVENT_LIST = 0x1D,
};

enum Shad1_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_SHAD_BOOK_TALK_A = 0x6,
    /* 0x07 */ BCK_SHAD_BOOK_WAIT,
    /* 0x08 */ BCK_SHAD_CHANT,
    /* 0x09 */ BCK_SHAD_DISCOURAGED,
    /* 0x0A */ BCK_SHAD_DISCOURAGED_WAIT,
    /* 0x0B */ BCK_SHAD_F_ANGER,
    /* 0x0C */ BCK_SHAD_F_CHANT,
    /* 0x0D */ BCK_SHAD_F_KOMON,
    /* 0x0E */ BCK_SHAD_F_SURPRISE,
    /* 0x0F */ BCK_SHAD_FH_ANGER,
    /* 0x10 */ BCK_SHAD_FH_KOMON,
    /* 0x11 */ BCK_SHAD_FH_SURPRISE,
    /* 0x12 */ BCK_SHAD_HI,
    /* 0x13 */ BCK_SHAD_LOOK,
    /* 0x14 */ BCK_SHAD_LOOK_WAIT,
    /* 0x15 */ BCK_SHAD_LOOKBACK,
    /* 0x16 */ BCK_SHAD_READ_KOMON,
    /* 0x17 */ BCK_SHAD_SAD_WALK,
    /* 0x18 */ BCK_SHAD_STEP,
    /* 0x19 */ BCK_SHAD_STEPB,
    /* 0x1A */ BCK_SHAD_SURPRISE,
    /* 0x1B */ BCK_SHAD_SURPRISE_WAIT,
    /* 0x1C */ BCK_SHAD_TAKE_KOMON,
    /* 0x1D */ BCK_SHAD_TALK_A,
    /* 0x1E */ BCK_SHAD_WAIT_A,
    /* 0x1F */ BCK_SHAD_WALK_A,

    /* BMDR */
    /* 0x22 */ BMDR_SHAD_KOMONSHO = 0x22,

    /* BTK */
    /* 0x25 */ BTK_SHAD_DISCOURAGED = 0x25,
    /* 0x26 */ BTK_SHAD_DISCOURAGED_WAIT,
    /* 0x27 */ BTK_SHAD_READ_KOMON,
    /* 0x28 */ BTK_SHAD_TAKE_KOMON,

    /* BTP */
    /* 0x2B */ BTP_SHAD_F_ANGER = 0x2B,
    /* 0x2C */ BTP_SHAD_F_CHANT,
    /* 0x2D */ BTP_SHAD_F_KOMON,
    /* 0x2E */ BTP_SHAD_F_SURPRISE,
    /* 0x2F */ BTP_SHAD_FH_ANGER,
    /* 0x30 */ BTP_SHAD_FH_KOMON,
    /* 0x31 */ BTP_SHAD_FH_SURPRISE,
};

enum Shad2_RES_File_ID {
    /* BCK */
    /* 0x3 */ BCK_SHAD_SIT_A = 0x3,
    /* 0x4 */ BCK_SHAD_SITTALK_A,
};

enum Shad3_RES_File_ID {
    /* BCK */
    /* 0x3 */ BCK_SHAD_LOOKUP = 0x3,
};

enum arcIdx {
    /* 0x0 */ SHAD,
    /* 0x1 */ SHAD1,
    /* 0x2 */ SHAD2,
    /* 0x3 */ SHAD3,
};

enum Joint {
    /* 0x00 */ JNT_CENTER,
    /* 0x01 */ JNT_BACKBONE1,
    /* 0x02 */ JNT_BACKBONE2,
    /* 0x03 */ JNT_COLLOR,
    /* 0x04 */ JNT_NECK,
    /* 0x05 */ JNT_HEAD,
    /* 0x06 */ JNT_CHIN,
    /* 0x07 */ JNT_MAYUL,
    /* 0x08 */ JNT_MAYUR,
    /* 0x09 */ JNT_MOUTH,
    /* 0x0A */ JNT_NECKTIE,
    /* 0x0B */ JNT_SHOULDERL,
    /* 0x0C */ JNT_ARML1,
    /* 0x0D */ JNT_ARML2,
    /* 0x0E */ JNT_HANDL,
    /* 0x0F */ JNT_FINGERL,
    /* 0x10 */ JNT_THUMBL,
    /* 0x11 */ JNT_SHOULDERR,
    /* 0x12 */ JNT_ARMR1,
    /* 0x13 */ JNT_ARMR2,
    /* 0x14 */ JNT_HANDR,
    /* 0x15 */ JNT_FINGERR,
    /* 0x16 */ JNT_THUMBR,
    /* 0x17 */ JNT_WAIST,
    /* 0x18 */ JNT_BOOK,
    /* 0x19 */ JNT_LEGL1,
    /* 0x1A */ JNT_LEGL2,
    /* 0x1B */ JNT_FOOTL,
    /* 0x1C */ JNT_LEGR1,
    /* 0x1D */ JNT_LEGR2,
    /* 0x1E */ JNT_FOOTR,
};

static daNpc_GetParam2 l_bckGetParamList[36] = {
    {-1, J3DFrameCtrl::EMode_LOOP, SHAD},
    {BCK_SHAD_F_TALK_A, J3DFrameCtrl::EMode_NONE, SHAD},
    {BCK_SHAD_F_HAPPY, J3DFrameCtrl::EMode_NONE, SHAD},
    {BCK_SHAD_F_ANGER, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BCK_SHAD_F_DISCOURAGED, J3DFrameCtrl::EMode_NONE, SHAD},
    {BCK_SHAD_F_CHANT, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BCK_SHAD_F_KOMON, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BCK_SHAD_F_SURPRISE, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BCK_SHAD_FH_SURPRISE, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BCK_SHAD_FH_KOMON, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BCK_SHAD_FH_HAPPY, J3DFrameCtrl::EMode_LOOP, SHAD},
    {BCK_SHAD_FH_ANGER, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BCK_SHAD_FH_DISCOURAGED, J3DFrameCtrl::EMode_LOOP, SHAD},
    {BCK_SHAD_WAIT_A, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BCK_SHAD_TALK_A, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BCK_SHAD_STEP, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BCK_SHAD_STEPB, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BCK_SHAD_BOOK_WAIT, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BCK_SHAD_BOOK_TALK_A, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BCK_SHAD_HI, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BCK_SHAD_RUN_A, J3DFrameCtrl::EMode_LOOP, SHAD},
    {BCK_SHAD_WALK_A, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BCK_SHAD_SIT_A, J3DFrameCtrl::EMode_LOOP, SHAD2},
    {BCK_SHAD_LOOKBACK, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BCK_SHAD_CHANT, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BCK_SHAD_DISCOURAGED, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BCK_SHAD_DISCOURAGED_WAIT, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BCK_SHAD_SITTALK_A, J3DFrameCtrl::EMode_NONE, SHAD2},
    {BCK_SHAD_LOOK, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BCK_SHAD_LOOK_WAIT, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BCK_SHAD_SAD_WALK, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BCK_SHAD_LOOKUP, J3DFrameCtrl::EMode_LOOP, SHAD3},
    {BCK_SHAD_TAKE_KOMON, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BCK_SHAD_READ_KOMON, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BCK_SHAD_SURPRISE, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BCK_SHAD_SURPRISE_WAIT, J3DFrameCtrl::EMode_LOOP, SHAD1},
};

static daNpc_GetParam2 l_btpGetParamList[11] = {
    {BTP_SHAD, J3DFrameCtrl::EMode_LOOP, SHAD},
    {BTP_SHAD_F_ANGER, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BTP_SHAD_F_DISCOURAGED, J3DFrameCtrl::EMode_NONE, SHAD},
    {BTP_SHAD_F_HAPPY, J3DFrameCtrl::EMode_NONE, SHAD},
    {BTP_SHAD_F_KOMON, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BTP_SHAD_F_SURPRISE, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BTP_SHAD_FH_SURPRISE, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BTP_SHAD_FH_KOMON, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BTP_SHAD_FH_ANGER, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BTP_SHAD_FH_DISCOURAGED, J3DFrameCtrl::EMode_LOOP, SHAD},
    {BTP_SHAD_FH_HAPPY, J3DFrameCtrl::EMode_LOOP, SHAD},
};

static daNpc_GetParam2 l_btkGetParamList[5] = {
    {BTK_SHAD, J3DFrameCtrl::EMode_LOOP, SHAD},
    {BTK_SHAD_DISCOURAGED, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BTK_SHAD_DISCOURAGED_WAIT, J3DFrameCtrl::EMode_LOOP, SHAD1},
    {BTK_SHAD_TAKE_KOMON, J3DFrameCtrl::EMode_NONE, SHAD1},
    {BTK_SHAD_READ_KOMON, J3DFrameCtrl::EMode_LOOP, SHAD1},
};

static int l_loadRes_SHAD0[3] = {
    0, 2, -1,
};

static int l_loadRes_SHAD1[3] = {
    0, 1, -1,
};

static int l_loadRes_SHAD2[3] = {
    0, 3, -1,
};

static int* l_loadRes_list[3] = {
    l_loadRes_SHAD0,
    l_loadRes_SHAD1,
    l_loadRes_SHAD2,
};

static char* l_arcNames[4] = {
    "Shad",
    "Shad1",
    "Shad2",
    "Shad3",
};

static char* l_evtNames[14] = {
    "",
    "THE_INTRODUCTION",
    "RESIST_MEETING",
    "SHAD_TO_CHANT_SPELL1",
    "SHAD_TO_CHANT_SPELL2",
    "SHAD_TO_CHANT_SPELL2_SKIP",
    "SHAD_DISCOVERY_CANNON",
    "SHAD_DISCOVERY_CANNON_SKIP",
    "SHAD_DISAPPEAR",
    "SHAD_DISAPPEAR_SKIP",
    "SHAD_CALLBACK",
    "RESIST_WIRETAP_ASH",
    "RESIST_WIRETAP_SHAD",
    "RESIST_WIRETAP_RAFREL"
};

static char* l_evtArcs[14] = {
    NULL,
    NULL,
    NULL,
    l_arcNames[0],
    l_arcNames[0],
    l_arcNames[0],
    l_arcNames[0],
    l_arcNames[0],
    l_arcNames[0],
    l_arcNames[0],
    l_arcNames[0],
    NULL,
    l_arcNames[0],
    NULL,
};

static char* l_myName = "Shad";

NPC_SHAD_HIO_CLASS l_HIO;

daNpcShad_c::EventFn daNpcShad_c::mEvtSeqList[14] = {
    NULL,
    &daNpcShad_c::EvCut_Introduction,
    &daNpcShad_c::EvCut_Meeting,
    &daNpcShad_c::EvCut_ToChantSpell1,
    &daNpcShad_c::EvCut_ToChantSpell2,
    &daNpcShad_c::EvCut_ToChantSpell2Skip,
    &daNpcShad_c::EvCut_DiscoveryCannon,
    &daNpcShad_c::EvCut_DiscoveryCannonSkip,
    &daNpcShad_c::EvCut_Disappear,
    &daNpcShad_c::EvCut_DisappearSkip,
    &daNpcShad_c::EvCut_CallBack,
    &daNpcShad_c::EvCut_WiretapEntrant,
    &daNpcShad_c::EvCut_WiretapSponsor,
    &daNpcShad_c::EvCut_WiretapEntrant,
};

daNpcShad_c::daNpcShad_c() {
    // empty function
}

daNpcShad_c::~daNpcShad_c() {
    for (int i = 0; l_loadRes_list[mMode][i] >= 0; i++) {
        dComIfG_resDelete(&mPhases[i], l_arcNames[l_loadRes_list[mMode][i]]);
    }

    if (heap != NULL) {
        mAnm_p->stopZelAnime();
    }
}

cPhs__Step daNpcShad_c::Create() {
    fopAcM_ct(this, daNpcShad_c);

    if (strcmp(dComIfGp_getStartStageName(), "R_SP116") == 0 && dComIfG_play_c::getLayerNo(0) == 4) {
        if (daNpcF_chkEvtBit(0x169) && daNpcF_chkEvtBit(0x10A) && daNpcF_chkEvtBit(0x10B) && !daNpcF_chkEvtBit(0x10C)) {
            return cPhs_ERROR_e;
        }

        if (dComIfGs_isStageSwitch(0x18, 0x4B)) {
            return cPhs_ERROR_e;
        }

        mMode = 0;
    } else {
        if (strcmp(dComIfGp_getStartStageName(), "R_SP209") == 0) {
            if (daNpcF_chkEvtBit(0x311)) {
                return cPhs_ERROR_e;
            }

            if (!daNpcF_chkEvtBit(0x10B) || (daNpcF_chkEvtBit(0x12E) && !daNpcF_chkEvtBit(0x31C))) {
                return cPhs_ERROR_e;
            }

            if (daNpcF_chkEvtBit(0x12F)) {
                if (!daNpcF_chkEvtBit(0x312)) {
                    if (getPathPoint(getPathID(), 1, &home.pos)) {
                        current.pos = home.pos;
                        old.pos = current.pos;
                    }
                } else {
                    home.pos.set(4342.7183f, -1518.5f, -3942.3232f);
                    current.pos = home.pos;
                    old.pos = current.pos;
                    setAngle(-0x2581);
                }
            }

            mMode = 1;
        } else {
            if (strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0) {
                if (dComIfGs_isSaveSwitch(20)) {
                    return cPhs_ERROR_e;
                }

                fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
                mMode = 2;
            }
        }
    }

    bool bVar1 = false;
    if (mMode == 1 && daNpcF_chkEvtBit(0x31C)) {
        bVar1 = true;
    }

    s16 sVar1;
    if (bVar1) {
        sVar1 = 0x42;
    } else {
        sVar1 = getMessageNo();
    }

    field_0xe14 = sVar1;
    field_0xe16 = field_0xe14;

    cPhs__Step phase = cPhs_ERROR_e;
    for (int i = 0; l_loadRes_list[mMode][i] >= 0; i++) {
        phase = (cPhs__Step)dComIfG_resLoad(&mPhases[i], l_arcNames[l_loadRes_list[mMode][i]]);

        if (phase != cPhs_COMPLEATE_e) {
            return phase;
        }
    }

    if (phase == cPhs_COMPLEATE_e) {
        int i_size = 0;
        switch (mMode) {
            case 0:
                i_size = 0x3610;
                break;

            case 1:
                i_size = 0x4CA0;
                break;

            case 2:
                i_size = 0x44A0;
                break;

            default:
                JUT_ASSERT(423, FALSE);
                break;
        }

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, i_size)) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -100.0f, -50.0f, -100.0f, 100.0f, 220.0f, 100.0f);
        mSound.init(&current.pos, &eyePos, 3, 1);
        mAcchCir.SetWall(mpHIO->m.common.width, mpHIO->m.common.knee_length);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                    fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mAcch.SetRoofNone();
        mAcch.SetWaterNone();
        mAcch.CrrPos(dComIfG_Bgsp());
        mCcStts.Init(mpHIO->m.common.weight, 0, this);

        field_0xcb0.Set(mCcDCyl);
        field_0xcb0.SetStts(&mCcStts);
        field_0xcb0.SetTgType(0);
        field_0xcb0.SetTgSPrm(0);

        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        setEnvTevColor();
        setRoomNo();
        mAnm_p->modelCalc();
        reset();
        Execute();
    }

    return phase;
}

int daNpcShad_c::CreateHeap() {
    J3DModelData* mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcNames[0], BMDR_SHAD));
    JUT_ASSERT(483, NULL != mdlData_p);
    mAnm_p = new mDoExt_McaMorfSO(mdlData_p, NULL, NULL, NULL, -1, 1.0f, 0, -1,
                                  &mSound, 0x80000, 0x11020284);
    if (mAnm_p != NULL && mAnm_p->getModel() == NULL) {
        mAnm_p->stopZelAnime();
        mAnm_p = NULL;
    }

    if (mAnm_p == NULL) {
        return 0;
    }

    J3DModel* model = mAnm_p->getModel();
    for (u16 i = 0; i < mdlData_p->getJointNum(); i++) {
        mdlData_p->getJointNodePointer(i)->setCallBack(ctrlJointCallBack);
    }
    model->setUserArea((uintptr_t)this);

    mpMatAnm = new daNpcF_MatAnm_c();
    if (mpMatAnm == NULL) {
        return 0;
    }

    mBookKnifeModel = NULL;
    if (mMode == 1 || mMode == 2) {
        mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcNames[0], BMDR_SHAD_BOOK_KNIFE));
        JUT_ASSERT(518, mdlData_p != NULL);
        mBookKnifeModel = mDoExt_J3DModel__create(mdlData_p, 0x80000, 0x11000084);
        if (mBookKnifeModel == NULL) {
            return 0;
        }

        if (mMode == 2) {
            mdlData_p = mBookKnifeModel->getModelData();
            mdlData_p->getMaterialNodePointer(1)->getShape()->hide();
        }
    }

    mKomonshoModel = NULL;
    if (mMode == 1) {
        mdlData_p = static_cast<J3DModelData*>(dComIfG_getObjectRes(l_arcNames[1], BMDR_SHAD_KOMONSHO));
        JUT_ASSERT(530, mdlData_p != NULL);
        mKomonshoModel = mDoExt_J3DModel__create(mdlData_p, 0x80000, 0x11000084);
        if (mKomonshoModel == NULL) {
            return 0;
        }
    }

    if (!setExpressionAnm(ANM_F_TALK_A, false)) {
        return 0;
    }

    switch (mMode) {
        case 0:
            setMotionAnm(ANM_SIT_A, 0.0f);
            break;

        case 1:
            setMotionAnm(ANM_WAIT_A, 0.0f);
            break;

        case 2:
            setMotionAnm(ANM_LOOKUP, 0.0f);
            break;

        default:
            JUT_ASSERT(545, FALSE);
            break;
    }

    return 1;
}

int daNpcShad_c::Delete() {
    this->~daNpcShad_c();
    return 1;
}

int daNpcShad_c::Execute() {
    execute();
    return 1;
}

int daNpcShad_c::Draw() {
    if (field_0xe1e == 0) {
        return 1;
    }

    mAnm_p->getModel()->getModelData()->getMaterialNodePointer(1)->setMaterialAnm(mpMatAnm);
    draw(FALSE, FALSE, mpHIO->m.common.real_shadow_size, NULL, FALSE);

    if (mBookKnifeModel != NULL) {
        g_env_light.setLightTevColorType_MAJI(mBookKnifeModel, &tevStr);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_FINGERL));
        mBookKnifeModel->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoExt_modelUpdateDL(mBookKnifeModel);
    }

    if (mKomonshoModel != NULL && (mMotion == MOT_TAKE_KOMON || mMotion == MOT_READ_KOMON)) {
        g_env_light.setLightTevColorType_MAJI(mKomonshoModel, &tevStr);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_FINGERR));
        mKomonshoModel->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoExt_modelUpdateDL(mKomonshoModel);
    }

    return 1;
}

int daNpcShad_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jntNo = i_joint->getJntNo();
    int lookatJoints[3] = {JNT_BACKBONE1, JNT_NECK, JNT_HEAD};

    if (jntNo == JNT_CENTER) {
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_BACKBONE1));
        mDoMtx_stack_c::multVecZero(&mLookatPos[0]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_NECK));
        mDoMtx_stack_c::multVecZero(&mLookatPos[1]);
        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
        mDoMtx_stack_c::multVecZero(&mLookatPos[2]);
    }

    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));

    switch (jntNo) {
        case JNT_BACKBONE1:
        case JNT_NECK:
        case JNT_HEAD:
            setLookatMtx(jntNo, lookatJoints, mpHIO->m.common.neck_rotation_ratio);
            break;
    }

    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    if ((jntNo == JNT_HEAD || jntNo == JNT_MOUTH) && (mAnmFlags & ANM_PLAY_BCK) != 0) {
        J3DAnmTransform* anmTransform = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mAnm_p->getAnm());
        mAnm_p->changeAnm(anmTransform);
    }

    return 1;
}

int daNpcShad_c::createHeapCallBack(fopAc_ac_c* a_this) {
    daNpcShad_c* i_this = (daNpcShad_c*)a_this;
    return i_this->CreateHeap();
}

int daNpcShad_c::ctrlJointCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        J3DModel* model = j3dSys.getModel();
        daNpcShad_c* i_this = (daNpcShad_c*)model->getUserArea();
        
        if (i_this != NULL) {
            i_this->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

bool daNpcShad_c::setExpressionAnm(int i_idx, bool i_modify) {
    J3DAnmTransform* anmTransform;
    mAnmFlags &= 0xFFFFE03F;

    if (l_bckGetParamList[i_idx].fileIdx >= 0) {
        anmTransform = (J3DAnmTransform*)getTrnsfrmKeyAnmP(l_arcNames[l_bckGetParamList[i_idx].arcIdx], l_bckGetParamList[i_idx].fileIdx);
    } else {
        anmTransform = NULL;
    }

    int i_attr = l_bckGetParamList[i_idx].attr;
    bool bVar1 = false;

    switch (i_idx) {
        case ANM_NONE:
            bVar1 = setExpressionBtp(EXPR_BTP_SHAD);
            break;

        case ANM_F_TALK_A:
            bVar1 = setExpressionBtp(EXPR_BTP_SHAD);
            break;

        case ANM_F_HAPPY:
            bVar1 = setExpressionBtp(EXPR_BTP_F_HAPPY);
            break;

        case ANM_F_ANGER:
            bVar1 = setExpressionBtp(EXPR_BTP_F_ANGER);
            break;

        case ANM_F_DISCOURAGED:
            bVar1 = setExpressionBtp(EXPR_BTP_F_DISCOURAGED);
            break;

        case ANM_F_CHANT:
            bVar1 = setExpressionBtp(EXPR_BTP_SHAD);
            break;

        case ANM_F_KOMON:
            bVar1 = setExpressionBtp(EXPR_BTP_F_KOMON);
            break;

        case ANM_F_SURPRISE:
            bVar1 = setExpressionBtp(EXPR_BTP_F_SURPRISE);
            break;

        case ANM_FH_SURPRISE:
            bVar1 = setExpressionBtp(EXPR_BTP_FH_SURPRISE);
            break;

        case ANM_FH_KOMON:
            bVar1 = setExpressionBtp(EXPR_BTP_FH_KOMON);
            break;

        case ANM_FH_HAPPY:
            bVar1 = setExpressionBtp(EXPR_BTP_FH_HAPPY);
            break;

        case ANM_FH_ANGER:
            bVar1 = setExpressionBtp(EXPR_BTP_FH_ANGER);
            break;

        case ANM_FH_DISCOURAGED:
            bVar1 = setExpressionBtp(EXPR_BTP_FH_DISCOURAGED);
            break;

        default:
            anmTransform = NULL;
    }

    if (!bVar1) {
        return false;
    }

    if (anmTransform == NULL) {
        return true;
    }

    if (setBckAnm(anmTransform, 1.0f, i_attr, 0, -1, i_modify)) {
        mAnmFlags |= ANM_PLAY_BCK | ANM_PAUSE_BCK;
        mExpressionLoops = 0;
        return true;
    }

    OS_REPORT("%s: 表情Bckアニメーションの登録に失敗しました！\n", "d_a_npc_shad.cpp"); // %s: Failed to register expression Bck animation!
    return false;
}

bool daNpcShad_c::setExpressionBtp(int i_idx) {
    J3DAnmTexPattern* anmTexPattern = getTexPtrnAnmP(l_arcNames[l_btpGetParamList[i_idx].arcIdx], l_btpGetParamList[i_idx].fileIdx);
    int i_attr = l_btpGetParamList[i_idx].attr;

    mAnmFlags &= 0xFFFFF57F;

    if (anmTexPattern == NULL) {
        return true;
    }

    if (setBtpAnm(anmTexPattern, mAnm_p->getModel()->getModelData(), 1.0f, i_attr)) {
        mAnmFlags |= ANM_PLAY_BTP | ANM_PAUSE_BTP;

        if (i_idx == 0) {
            mAnmFlags |= ANM_FLAG_800;
        }
            
        return true;
    }

    OS_REPORT("%s: 表情Btpアニメーションの登録に失敗しました！\n", "d_a_npc_shad.cpp"); // %s: Failed to register expression Btp animation!
    return false;
}

void daNpcShad_c::setMotionAnm(int i_idx, f32 i_morf) {
    J3DAnmTransformKey* anmTransformKey = getTrnsfrmKeyAnmP(l_arcNames[l_bckGetParamList[i_idx].arcIdx], l_bckGetParamList[i_idx].fileIdx);
    int i_attr = l_bckGetParamList[i_idx].attr;

    mAnmFlags &= 0xFFFFFFC0;

    if (anmTransformKey != NULL && setMcaMorfAnm(anmTransformKey, 1.0f, i_morf, i_attr, 0, -1)) {
        mAnmFlags |= ANM_PLAY_MORF | ANM_PAUSE_MORF;
        mMotionLoops = 0;
    }

    int i_index = 0;
    switch (i_idx) {
        case ANM_DISCOURAGED:
            i_index = 1;
            break;

        case ANM_DISCOURAGED_WAIT:
            i_index = 2;
            break;

        case ANM_TAKE_KOMON:
            i_index = 3;
            break;

        case ANM_READ_KOMON:
            i_index = 4;
            break;
    }

    J3DAnmTextureSRTKey* anmTextureSRTKey = getTexSRTKeyAnmP(l_arcNames[l_btkGetParamList[i_index].arcIdx], l_btkGetParamList[i_index].fileIdx);
    i_attr = l_btkGetParamList[i_index].attr;

    if (anmTextureSRTKey != NULL) {
        if (setBtkAnm(anmTextureSRTKey, mAnm_p->getModel()->getModelData(), 1.0f, i_attr)) {
            mAnmFlags |= ANM_PLAY_BTK | ANM_PAUSE_BTK;
        }
    }
}

void daNpcShad_c::reset() {
    initialize();
    mpMatAnm->initialize();
    mLookat.initialize();

    for (int i = 0; i < 5; i++) {
        mActorMngr[i].initialize();
    }

    field_0xe0c = 0;
    field_0xe10 = 0;
    mActionFn = NULL;
    mLookMode = -1;
    field_0xe1a = 0;
    field_0xe1e = 1;
    field_0xe1f = 0;

    if (isSneaking()) {
        attention_info.distances[fopAc_attn_LOCK_e] = 78;
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = 77;
        attention_info.flags |= fopAc_AttnFlag_UNK_0x800000;
    } else if (mMode == 0) {
        attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(3, 3);
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(2, 3);
    } else {
        attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(mpHIO->m.common.attention_distance, mpHIO->m.common.attention_angle);
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(mpHIO->m.common.talk_distance, mpHIO->m.common.talk_angle);
    }

    current.pos = home.pos;
    old.pos = current.pos;
    current.angle.set(0, home.angle.y, 0);
    old.angle = current.angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    mOldAngle = mCurAngle;
    speedF = 0.0f;
    speed.setall(0.0f);
    mItemPartnerId = fpcM_ERROR_PROCESS_ID_e;
    mOrderEvtNo = 0;
    mExpressionMorfOverride = 0.0f;
    mMotionMorfOverride = 0.0f;

    if (isSneaking()) {
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
    }

    setWaitAction();
}

inline void daNpcShad_c::setWaitAction() {
    switch (mMode) {
        case 0:
            setAction(&daNpcShad_c::wait_type0);
            break;

        case 1:
            setAction(&daNpcShad_c::wait_type1);
            break;

        case 2:
            setAction(&daNpcShad_c::wait_type2);
            break;

        default:
            JUT_ASSERT(1379, FALSE);
    }
}

inline bool daNpcShad_c::getPathPoint(u8 i_pathIndex, int i_pntIndex, Vec* param_3) {
    dPath* path = dPath_GetRoomPath(i_pathIndex, fopAcM_GetRoomNo(this));
    if (path == NULL) {
        return false;
    }

    dPnt* pnt = dPath_GetPnt(path, i_pntIndex);
    if (pnt == NULL) {
        return false;
    }

    *param_3 = pnt->m_position;
    return true;
}

inline bool daNpcShad_c::isSneaking() {
    if (strcmp(dComIfGp_getStartStageName(), "R_SP116") == 0 && dComIfGp_getStartStageRoomNo() == 5 && dComIfG_play_c::getLayerNo(0) == 2) {
        return true;
    }

    return false;
}

inline void daNpcShad_c::playExpression() {
    daNpcF_anmPlayData dat0 = {ANM_F_TALK_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1a = {ANM_F_HAPPY, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_FH_HAPPY, 0.0f, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2a = {ANM_F_ANGER, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat2b = {ANM_FH_ANGER, 0.0f, 0};
    daNpcF_anmPlayData* pDat2[2] = {&dat2a, &dat2b};
    daNpcF_anmPlayData dat3a = {ANM_F_DISCOURAGED, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat3b = {ANM_FH_DISCOURAGED, 0.0f, 0};
    daNpcF_anmPlayData* pDat3[2] = {&dat3a, &dat3b};
    daNpcF_anmPlayData dat4 = {ANM_F_KOMON, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};
    daNpcF_anmPlayData dat5 = {ANM_F_SURPRISE, 0.0f, 0};
    daNpcF_anmPlayData* pDat5[1] = {&dat5};
    daNpcF_anmPlayData dat6 = {ANM_FH_SURPRISE, 0.0f, 0};
    daNpcF_anmPlayData* pDat6[1] = {&dat6};
    daNpcF_anmPlayData dat7 = {ANM_FH_KOMON, 0.0f, 0};
    daNpcF_anmPlayData* pDat7[1] = {&dat7};
    daNpcF_anmPlayData dat8 = {ANM_FH_HAPPY, 0.0f, 0};
    daNpcF_anmPlayData* pDat8[1] = {&dat8};
    daNpcF_anmPlayData dat9 = {ANM_FH_ANGER, 0.0f, 0};
    daNpcF_anmPlayData* pDat9[1] = {&dat9};
    daNpcF_anmPlayData dat10 = {ANM_FH_DISCOURAGED, 0.0f, 0};
    daNpcF_anmPlayData* pDat10[1] = {&dat10};
    daNpcF_anmPlayData dat11 = {ANM_F_CHANT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat11[1] = {&dat11};
    daNpcF_anmPlayData dat12 = {ANM_NONE, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat12[1] = {&dat12};
    daNpcF_anmPlayData** ppDat[13] = {
        pDat0, pDat1, pDat2, pDat3, pDat4,
        pDat5, pDat6, pDat7, pDat8, pDat9,
        pDat10, pDat11, pDat12,
    };

    if (mExpression >= 0 && mExpression < 0xD) {
        playExpressionAnm(ppDat);
    }
}

inline void daNpcShad_c::playMotion() {
    daNpcF_anmPlayData dat0 = {ANM_WAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat0[1] = {&dat0};
    daNpcF_anmPlayData dat1a = {ANM_BOOK_TALK_A, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat1b = {ANM_BOOK_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat1[2] = {&dat1a, &dat1b};
    daNpcF_anmPlayData dat2 = {ANM_BOOK_WAIT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat2[1] = {&dat2};
    daNpcF_anmPlayData dat3a = {ANM_HI, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat3b = {ANM_WAIT_A, 0.0f, 0};
    daNpcF_anmPlayData* pDat3[2] = {&dat3a, &dat3b};
    daNpcF_anmPlayData dat4 = {ANM_SIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat4[1] = {&dat4};
    daNpcF_anmPlayData dat5a = {ANM_LOOKBACK, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat5b = {ANM_WAIT_A, 0.0f, 0};
    daNpcF_anmPlayData* pDat5[2] = {&dat5a, &dat5b};
    daNpcF_anmPlayData dat6a = {ANM_DISCOURAGED, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat6b = {ANM_DISCOURAGED_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat6[2] = {&dat6a, &dat6b};
    daNpcF_anmPlayData dat7 = {ANM_DISCOURAGED_WAIT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat7[1] = {&dat7};
    daNpcF_anmPlayData dat8a = {ANM_TALK_A, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat8b = {ANM_WAIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat8[2] = {&dat8a, &dat8b};
    daNpcF_anmPlayData dat9a = {ANM_SITTALK_A, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat9b = {ANM_SIT_A, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat9[2] = {&dat9a, &dat9b};
    daNpcF_anmPlayData dat10a = {ANM_LOOK, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat10b = {ANM_LOOK_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat10[2] = {&dat10a, &dat10b};
    daNpcF_anmPlayData dat11a = {ANM_TAKE_KOMON, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat11b = {ANM_READ_KOMON, 0.0f, 0};
    daNpcF_anmPlayData* pDat11[2] = {&dat11a, &dat11b};
    daNpcF_anmPlayData dat12 = {ANM_READ_KOMON, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat12[1] = {&dat12};
    daNpcF_anmPlayData dat13 = {ANM_LOOK_WAIT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat13[1] = {&dat13};
    daNpcF_anmPlayData dat14a = {ANM_SURPRISE, mpHIO->m.common.morf_frame, 1};
    daNpcF_anmPlayData dat14b = {ANM_SURPRISE_WAIT, 0.0f, 0};
    daNpcF_anmPlayData* pDat14[2] = {&dat14a, &dat14b};
    daNpcF_anmPlayData dat15 = {ANM_SURPRISE_WAIT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat15[1] = {&dat15};
    daNpcF_anmPlayData dat16 = {ANM_LOOKUP, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat16[1] = {&dat16};
    daNpcF_anmPlayData dat17 = {ANM_SAD_WALK, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat17[1] = {&dat17};
    daNpcF_anmPlayData dat18 = {ANM_CHANT, mpHIO->m.common.morf_frame, 0};
    daNpcF_anmPlayData* pDat18[1] = {&dat18};
    daNpcF_anmPlayData dat19 = {ANM_RUN_A, 4.0f, 0};
    daNpcF_anmPlayData* pDat19[1] = {&dat19};
    daNpcF_anmPlayData dat20 = {ANM_WALK_A, 4.0f, 0};
    daNpcF_anmPlayData* pDat20[1] = {&dat20};
    daNpcF_anmPlayData dat21 = {ANM_STEP, 4.0f, 0};
    daNpcF_anmPlayData* pDat21[1] = {&dat21};
    daNpcF_anmPlayData dat22 = {ANM_STEPB, 0.0f, 0};
    daNpcF_anmPlayData* pDat22[1] = {&dat22};
    daNpcF_anmPlayData** ppDat[23] = {
        pDat0, pDat1, pDat2, pDat3, pDat4,
        pDat5, pDat6, pDat7, pDat8, pDat9,
        pDat10, pDat11, pDat12, pDat13, pDat14,
        pDat15, pDat16, pDat17, pDat18, pDat19,
        pDat20, pDat21, pDat22,
    };

    if (mMotion >= 0 && mMotion < 23) {
        playMotionAnm(ppDat);
    }
}

inline void daNpcShad_c::lookat() {
    daPy_py_c* player = NULL;
    J3DModel* model = mAnm_p->getModel();
    int iVar1 = 0;
    f32 body_angleX_min = mpHIO->m.common.body_angleX_min;
    f32 body_angleX_max = mpHIO->m.common.body_angleX_max;

    f32 body_angleY_min;
    if (mMode == 1) {
        body_angleY_min = -30.0f;
    } else {
        body_angleY_min = mpHIO->m.common.body_angleY_min;
    }

    f32 body_angleY_max;
    if (mMode == 1) {
        body_angleY_max = 30.0f;
    } else {
        body_angleY_max = mpHIO->m.common.body_angleY_max;
    }

    f32 head_angleX_min = mpHIO->m.common.head_angleX_min;
    f32 head_angleX_max = mpHIO->m.common.head_angleX_max;
    f32 head_angleY_min = mpHIO->m.common.head_angleY_min;
    f32 head_angleY_max = mpHIO->m.common.head_angleY_max;

    s16 sVar1 = mCurAngle.y - mOldAngle.y;
    cXyz spf0[3] = {mLookatPos[0], mLookatPos[1], mLookatPos[2]};
    csXyz* local_fc[3] = {&mLookatAngle[0], &mLookatAngle[1], &mLookatAngle[2]};
    cXyz sp108;

    switch (mLookMode) {
        case LOOK_RESET:
            iVar1 = 1;
            break;

        case LOOK_PLAYER:
        case LOOK_PLAYER_TALK:
            player = daPy_getPlayerActorClass();
            break;

        case LOOK_ACTOR:
            player = field_0xca8;
            break;

        case LOOK_ATTN:
            break;
    }

    if (mLookMode == LOOK_ATTN) {
        mLookat.setAttnPos(&mLookPos);
    } else if (player != NULL) {
        mLookPos = player->attention_info.position;

        if (mLookMode != LOOK_PLAYER && mLookMode != LOOK_PLAYER_TALK) {
            mLookPos.y -= 40.0f;
        }

        mLookat.setAttnPos(&mLookPos);
    } else {
        mLookat.setAttnPos(NULL);
    }

    mLookat.setParam(body_angleX_min, body_angleX_max, body_angleY_min, body_angleY_max, 0.0f, 0.0f, 0.0f, 0.0f, head_angleX_min, head_angleX_max, 
                     head_angleY_min, head_angleY_max, mCurAngle.y, spf0);
    mLookat.calc(this, model->getBaseTRMtx(), (csXyz**)local_fc, iVar1, sVar1, FALSE);
}

inline BOOL daNpcShad_c::chkFindPlayer() {
    f32 fVar1;
    if (mMode == 0) {
        fVar1 = 135.0f;
    } else {
        fVar1 = mpHIO->m.common.fov;
    }

    if (!chkActorInSight(daPy_getPlayerActorClass(), fVar1)) {
        mActorMngr[0].remove();
        return FALSE;
    }

    BOOL rv;
    if (mActorMngr[0].getActorP() == NULL) {
        rv = chkPlayerInSpeakArea(this);
    } else {
        rv = chkPlayerInTalkArea(this);
    }

    if (rv) {
        mActorMngr[0].entry(daPy_getPlayerActorClass());
    } else {
        mActorMngr[0].remove();
    }

    return rv;
}

inline void daNpcShad_c::setLookMode(int i_lookMode, fopAc_ac_c* param_2, cXyz* param_3) {
    switch (i_lookMode) {
        case LOOK_NONE:
        case LOOK_RESET:
        case LOOK_PLAYER:
        case LOOK_PLAYER_TALK:
        case LOOK_ACTOR:
            if (mLookMode != i_lookMode || (daPy_py_c*)param_2 != field_0xca8) {
                mLookMode = i_lookMode;
                field_0xca8 = (daPy_py_c*)param_2;
            }
            break;

        case LOOK_ATTN:
            mLookMode = i_lookMode;
            field_0xca8 = NULL;

            if (param_3 != NULL) {
                mLookPos.set(*param_3);
            }
            break;
    }
}

inline BOOL daNpcShad_c::step(s16 i_targetAngle, int i_motion, f32 i_rate) {
    if (mTurnMode == 0) {
        if (i_motion != -1) {
            if ((int)fabsf(cM_sht2d((s16)(i_targetAngle - mCurAngle.y))) > 40) {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(i_motion, -1.0f, FALSE);
            }
        }

        mTurnTargetAngle = i_targetAngle;
        mTurnAmount = 0;

        if (mCurAngle.y == mTurnTargetAngle) {
            mTurnMode++;
        }

        current.angle.y = mCurAngle.y;
        shape_angle.y = current.angle.y;
        mTurnMode++;
    } else if (mTurnMode == 1) {
        if (turn(mTurnTargetAngle, i_rate, 0)) {
            shape_angle.y = current.angle.y;
            mCurAngle.y = current.angle.y;
            mOldAngle.y = current.angle.y;
            mTurnMode++;
        } else {
            shape_angle.y = current.angle.y;
            mCurAngle.y = current.angle.y;
        }
    }

    return mTurnMode > 1;
}

bool daNpcShad_c::wait_type0(void* param_1) {
    switch (field_0xe1a) {
        case 0:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_SIT_A, -1.0f, FALSE);
            mTurnMode = 0;
            speedF = 0.0f;
            field_0xe1a = 2;
            break;

        case 2:
            if (isSneaking()) {
                setLookMode(LOOK_NONE, NULL, NULL);
            } else {
                if (mActorMngr[0].getActorP() != NULL) {
                    if (!chkFindPlayer()) {
                        mTurnMode = 0;
                    }
                } else {
                    if (chkFindPlayer()) {
                        mTurnMode = 0;
                    }
                }

                if (mActorMngr[0].getActorP() != NULL) {
                    setLookMode(LOOK_PLAYER, NULL, NULL);
                } else {
                    setLookMode(LOOK_NONE, NULL, NULL);

                    if (home.angle.y != mCurAngle.y && step(home.angle.y, MOT_STEP, 15.0f)) {
                        setExpression(EXPR_NONE, -1.0f);
                        setMotion(MOT_SIT_A, -1.0f, FALSE);
                        mTurnMode = 0;
                    }
                }
            }

            if (dComIfGp_event_runCheck()) {
                if (eventInfo.checkCommandTalk()) {
                    if (dComIfGp_event_chkTalkXY() == 0 || dComIfGp_evmng_ChkPresentEnd()) {
                        if (isSneaking()) {
                            mOrderEvtNo = 12;
                            changeEvent(l_evtArcs[mOrderEvtNo], l_evtNames[mOrderEvtNo], 1, 0xFFFF);
                        } else {
                            setAction(&daNpcShad_c::talk);
                        }
                    }
                } else {
                    if (dComIfGp_getEventManager().getMyStaffId(l_myName, NULL, 0) != -1) {
                        setAction(&daNpcShad_c::demo);
                    }
                }
            } else {
                if (mOrderEvtNo != 0) {
                    eventInfo.setArchiveName(l_evtArcs[mOrderEvtNo]);
                }

                char* event;
                if (mOrderEvtNo != 0) {
                    event = l_evtNames[mOrderEvtNo];
                } else {
                    event = NULL;
                }
                orderEvent(mOrderSpeakEvt, event, 0xFFFF, 40, 0xFF, 1);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1747, FALSE);
    }

    return true;
}

void daNpcShad_c::setMotion(int i_motion, f32 i_morf, BOOL i_restart) {
    s16 motion = i_motion;
    if ((i_restart || mMotion != motion) && i_motion >= 0 && i_motion < 0x17) {
        mMotion = motion;
        mMotionMorfOverride = i_morf;
        mMotionPrevPhase = -1;
        mMotionPhase = 0;
    }
}

void daNpcShad_c::setExpression(int i_expression, f32 i_morf) {
    if (i_expression >= 0 && i_expression < 0xD) {
        mExpression = i_expression;
        mExpressionMorfOverride = i_morf;
        mExpressionPrevPhase = -1;
        mExpressionPhase = 0;
    }
}

bool daNpcShad_c::wait_type1(void* param_1) {
    switch (field_0xe1a) {
        case 0:
            if (daNpcF_chkEvtBit(0x12F) && !daNpcF_chkEvtBit(0x312)) {
                setExpression(EXPR_H_DISCOURAGED, -1.0f);
                setMotion(MOT_DISCOURAGED_WAIT, -1.0f, FALSE);
            } else {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
            }

            mActorMngr[0].remove();
            setLookMode(LOOK_NONE, NULL, NULL);
            mTurnMode = 0;
            speedF = 0.0f;
            field_0xe1c = fopAcM_searchPlayerAngleY(this);
            field_0xe1a = 2;
            break;

        case 2:
            if (daNpcF_chkEvtBit(0x312)) {
                setLookMode(LOOK_PLAYER, NULL, NULL);

                if (mCurAngle.y == field_0xe1c) {
                    if (fopAcM_seenPlayerAngleY(this) > cM_deg2s(mpHIO->m.common.body_angleY_max + mpHIO->m.common.head_angleY_max)) {
                        field_0xe1c = fopAcM_searchPlayerAngleY(this);
                    }
                } else if (step(field_0xe1c, MOT_STEPB, 30.0f)) {
                    setExpression(EXPR_NONE, -1.0f);
                    setMotion(MOT_WAIT_A, -1.0f, FALSE);
                    mTurnMode = 0;
                }

                if (fopAcM_GetPosition_p(daPy_getPlayerActorClass())->x <= 3750.0f) {
                    mOrderEvtNo = 10;
                }
            } else if (daNpcF_chkEvtBit(0x12F)) {
                daTag_EvtArea_c* tag = (daTag_EvtArea_c*)mActorMngr[4].getActorP();
                if (tag != NULL) {
                    if (tag->chkPointInArea(*fopAcM_GetPosition_p(daPy_getPlayerActorClass()))) {
                        field_0xe14 = 67;
                        daNpcF_offTmpBit(0xB);
                        daNpcF_offTmpBit(0xC);
                        daNpcF_offTmpBit(0xD);
                        mOrderEvtNo = 6;
                        fopAcM_delete(tag);
                    }
                }
            } else if (field_0xe1f != 0) {
                setLookMode(LOOK_PLAYER, NULL, NULL);

                if (mCurAngle.y == field_0xe1c) {
                    if (fopAcM_seenPlayerAngleY(this) > cM_deg2s(mpHIO->m.common.body_angleY_max + mpHIO->m.common.head_angleY_max)) {
                        field_0xe1c = fopAcM_searchPlayerAngleY(this);
                    }
                } else if (step(field_0xe1c, MOT_STEPB, 30.0f)) {
                    setExpression(EXPR_NONE, -1.0f);
                    setMotion(MOT_WAIT_A, -1.0f, FALSE);
                    mTurnMode = 0;
                }

                if (fopAcM_searchPlayerDistanceXZ(this) > 400.0f) {
                    field_0xe1f = 0;
                }
            } else {
                if (mActorMngr[0].getActorP() != NULL) {
                    if (!chkFindPlayer()) {
                        mTurnMode = 0;
                    }
                } else if (chkFindPlayer()) {
                    mTurnMode = 0;
                }

                if (mActorMngr[0].getActorP() != NULL) {
                    setLookMode(LOOK_PLAYER, NULL, NULL);
                } else {
                    setLookMode(LOOK_NONE, NULL, NULL);
                    if (home.angle.y != mCurAngle.y && step(home.angle.y, MOT_STEP, 15.0f)) {
                        setExpression(EXPR_NONE, -1.0f);
                        setMotion(MOT_WAIT_A, -1.0f, FALSE);
                        mTurnMode = 0;
                    }
                }
            }

            if (dComIfGp_event_runCheck() != 0) {
                if (eventInfo.checkCommandTalk()) {
                    if (dComIfGp_event_chkTalkXY()) {
                        if (!dComIfGp_evmng_ChkPresentEnd()) {
                            return true;
                        }

                        u8 sVar1 = dComIfGp_event_getPreItemNo();
                        if (sVar1 == 0xE9) {
                            field_0xe14 = 64;
                            daNpcF_offTmpBit(0xB);
                            daNpcF_offTmpBit(0xC);
                            daNpcF_offTmpBit(0xD);
                            daNpcF_offTmpBit(0xE);
                            mOrderEvtNo = 3;
                            changeEvent(l_evtArcs[mOrderEvtNo], l_evtNames[mOrderEvtNo], 1, 0xFFFF);
                        } else if (sVar1 == 0xEB) {
                            field_0xe14 = 65;
                            daNpcF_offTmpBit(0xB);
                            daNpcF_offTmpBit(0xC);
                            daNpcF_offTmpBit(0xD);
                            mOrderEvtNo = 4;
                            changeEvent(l_evtArcs[mOrderEvtNo], l_evtNames[mOrderEvtNo], 1, 0xFFFF);
                        } else {
                            s16 eventIdx = dComIfGp_getEventManager().getEventIdx(this, "NO_RESPONSE", 0xFF);
                            dComIfGp_getEvent().reset(this);
                            fopAcM_orderChangeEventId(this, eventIdx, 1, 0xFFFF);
                        }
                    } else if (daNpcF_chkEvtBit(0x313)) {
                        field_0xe14 = 68;
                        daNpcF_offTmpBit(0xB);
                        mOrderEvtNo = 8;
                        changeEvent(l_evtArcs[mOrderEvtNo], l_evtNames[mOrderEvtNo], 1, 0xFFFF);
                    } else {
                        setAction(&daNpcShad_c::talk);
                    }
                } else if (dComIfGp_getEventManager().getMyStaffId(l_myName, NULL, 0) != -1) {
                    setAction(&daNpcShad_c::demo);
                }
            } else {
                if (!daNpcF_chkEvtBit(0x12F)) {
                    eventInfo.onCondition(0x20);
                }

                if (mOrderEvtNo != 0) {
                    eventInfo.setArchiveName(l_evtArcs[mOrderEvtNo]);
                }

                char* event;
                if (mOrderEvtNo != 0) {
                    event = l_evtNames[mOrderEvtNo];
                } else {
                    event = NULL;
                }
                orderEvent(mOrderSpeakEvt, event, 0xFFFF, 40, 0xFF, 1);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1915, FALSE);
    }

    return true;
}

bool daNpcShad_c::wait_type2(void* param_1) {
    switch (field_0xe1a) {
        case 0:
            setExpression(EXPR_H_HAPPY, -1.0f);
            setMotion(MOT_LOOKUP, -1.0f, FALSE);
            mTurnMode = 0;
            speedF = 0.0f;
            mActorMngr[0].entry(daPy_getPlayerActorClass());
            setLookMode(LOOK_NONE, NULL, NULL);
            field_0xe1a = 2;
            break;

        case 2:
            switch (field_0xe10) {
                case 0:
                    if (daNpcF_chkTmpBit(0x76)) {
                        current.pos.set(-4532.0845f, 0.0f, 8555.381f);
                        old.pos = current.pos;
                        setAngle(-0x6000);
                        field_0xe10++;
                    }
                    break;

                case 1:
                    if (daNpcF_chkTmpBit(0x74)) {
                        setAction(&daNpcShad_c::leave);
                    }
                    break;

                default:
                    JUT_ASSERT(1957, FALSE);
                    break;
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(1962, FALSE);
            break;
    }

    return true;
}

inline void daNpcShad_c::setExpressionTalkAfter() {
    switch (mExpression) {
        case 1:
            setExpression(EXPR_H_HAPPY, -1.0f);
            break;

        case 2:
            setExpression(EXPR_H_ANGER, -1.0f);
            break;

        case 3:
            setExpression(EXPR_H_DISCOURAGED, -1.0f);
            break;

        case 4:
            setExpression(EXPR_H_KOMON, -1.0f);
            break;

        default:
            setExpression(EXPR_NONE, -1.0f);
    }
}

bool daNpcShad_c::talk(void* param_1) {
    int itemNo, i_expression, i_motion, i_msgTimer;
    u16 eventID;
    s16 eventIdx;
    bool rv = false;

    switch (field_0xe1a) {
        case 0:
            initTalk(field_0xe14, NULL);
            mTurnMode = 0;
            mMsgTimer = 0;
            speedF = 0.0f;
            setLookMode(LOOK_PLAYER_TALK, NULL, NULL);
            field_0xe1a = 2;
            break;

        case 2:
            if (strcmp(dComIfGp_getStartStageName(), "R_SP116") == 0 || mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                if (talkProc(NULL, TRUE, NULL)) {
                    mActorMngr[0].entry(daPy_getPlayerActorClass());
                    itemNo = NULL;
                    eventID = mFlow.getEventId(&itemNo);
                    OS_REPORT("会話終了時 イベントID=%d アイテムNo=%d\n", eventID, itemNo); // At the end of the conversation, Event ID=%d Item No=%d

                    if (eventID == 1) {
                        mItemPartnerId = fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1, -1, NULL, NULL);

                        if (mItemPartnerId != fpcM_ERROR_PROCESS_ID_e) {
                            eventIdx = dComIfGp_getEventManager().getEventIdx(this, "DEFAULT_GETITEM", 0xFF);
                            dComIfGp_getEvent().reset(this);
                            fopAcM_orderChangeEventId(this, eventIdx, 1, 0xFFFF);
                            field_0x9ec = true;
                            setWaitAction();
                        }
                    } else {
                        setWaitAction();
                    }

                    rv = true;
                } else {
                    i_msgTimer = mMsgTimer;
                    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE)) {
                        setExpression(i_expression, -1.0f);
                        setMotion(i_motion, -1.0f, FALSE);
                    } else if (i_msgTimer != 0 && mMsgTimer == 0) {
                        setExpressionTalkAfter();
                    }
                }
            } else if (step(fopAcM_searchPlayerAngleY(this), MOT_STEP, 15.0f)) {
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                mTurnMode = 0;
            }
            break;

        case 3:
            field_0xe1f = 1;
            field_0xe14 = field_0xe16;
            setExpression(EXPR_NONE, -1.0f);

            if (!field_0x9ec) {
                dComIfGp_event_reset();
            }
            break;

        default:
            JUT_ASSERT(2042, FALSE);
            break;
    }
    
    return rv;
}

bool daNpcShad_c::demo(void* param_1) {
    int iVar2;

    switch (field_0xe1a) {
        case 0:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, FALSE);
            field_0xe1a = 2;
            // fallthrough
        case 2:
            if (dComIfGp_event_runCheck() != 0 && !eventInfo.checkCommandTalk()) {
                dEvent_manager_c& eventManager = dComIfGp_getEventManager();
                s32 iVar1 = eventManager.getMyStaffId(l_myName, NULL, 0);
                if (iVar1 != -1) {
                    mStaffID = iVar1;
                    iVar2 = eventManager.getMyActIdx(iVar1, l_evtNames, 14, 0, 0);

                    if (iVar2 > 0 && iVar2 < 14) {
                        mOrderEvtNo = iVar2;
                    }

                    JUT_ASSERT(2082, NULL != mEvtSeqList[mOrderEvtNo]);

                    if ((this->*mEvtSeqList[mOrderEvtNo])(iVar1)) {
                        eventManager.cutEnd(iVar1);
                    }
                }

                if (eventInfo.checkCommandDemoAccrpt() && mEventIdx != -1 && eventManager.endCheck(mEventIdx)) {
                    dComIfGp_event_reset();
                    mOrderEvtNo = 0;
                    mEventIdx = -1;
                    setWaitAction();
                }
            } else {
                mOrderEvtNo = 0;
                mEventIdx = -1;
                setWaitAction();
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(2114, FALSE);
            break;
    }

    return true;
}

bool daNpcShad_c::leave(void* param_1) {
    cXyz sp28(-6253.387f, 0.0f, 6251.7603f);

    switch (field_0xe1a) {
        case 0:
            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_RUN_A, -1.0f, FALSE);
            mTurnMode = 0;
            speedF = mpHIO->m.traveling_speed;
            field_0xe1a = 2;
            break;

        case 2:
            step(cLib_targetAngleY(&current.pos, &sp28), MOT_STEP, 15.0f);

            if (mAcch.ChkWallHit() || current.pos.abs(sp28) < speedF) {
                fopAcM_delete(this);
            }
            break;

        case 3:
            break;

        default:
            JUT_ASSERT(2149, FALSE);
            break;
    }

    return true;
}

BOOL daNpcShad_c::EvCut_Introduction(int i_cutIndex) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* piVar1 = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (piVar1 == NULL) {
        return FALSE;
    }

    if (eventManager->getIsAddvance(i_cutIndex)) {
        switch (*piVar1) {
            case 0:
            case 2:
                setLookMode(LOOK_NONE, NULL, NULL);
                return TRUE;

            case 1:
                setLookMode(LOOK_PLAYER, NULL, NULL);
                return TRUE;

            default:
                JUT_ASSERT(2184, FALSE);
                break;
        }
    }

    return FALSE;
}

BOOL daNpcShad_c::EvCut_Meeting(int i_cutIndex) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* prm = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");
    if (prm == NULL) {
        return FALSE;
    }

    if (eventManager->getIsAddvance(i_cutIndex)) {
        switch (*prm) {
            case 0:
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_SIT_A, -1.0f, FALSE);
                break;

            case 1:
                setLookMode(LOOK_PLAYER, NULL, NULL);
                break;

            default:
                JUT_ASSERT(2221, FALSE);
                break;
        }
    }

    int i_expression, i_motion, i_msgTimer;
    i_msgTimer = mMsgTimer;
    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE)) {
        setExpression(i_expression, -1.0f);
        setMotion(i_motion, -1.0f, FALSE);
    } else if (i_msgTimer != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    fopAc_ac_c* actor;
    switch (*prm) {
        case 0:
            actor = dComIfGp_event_getTalkPartner();
            if (actor == this) {
                actor = NULL;
            }

            setLookMode(LOOK_ACTOR, actor, NULL);
            return TRUE;

        case 1:
            return TRUE;

        default:
            JUT_ASSERT(2245, FALSE);
            break;
    }

    return FALSE;
}

BOOL daNpcShad_c::EvCut_ToChantSpell1(int i_cutIndex) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* cutName = (int*)eventManager->getMyNowCutName(i_cutIndex);

    if (eventManager->getIsAddvance(i_cutIndex)) {
        switch (*cutName) {
            case '0001':
            case '0002':
                setLookMode(LOOK_NONE, NULL, NULL);
                initTalk(field_0xe14, NULL);
                break;

            case '0005':
                initTalk(field_0xe14, NULL);
                setExpression(EXPR_H_DISCOURAGED, -1.0f);
                setMotion(MOT_DISCOURAGED_WAIT, -1.0f, FALSE);
                break;

            case '0006':
                mTurnMode = 0;
                setMotion(MOT_WALK_A, -1.0f, FALSE);
                break;

            case '0007':
                current.pos = home.pos;
                old.pos = current.pos;
                speedF = 0.0f;
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                field_0xe1e = 0;
                break;

            case '0008':
                initTalk(field_0xe14, NULL);
                Z2GetAudioMgr()->subBgmStart(Z2BGM_ITEM_GET);
                break;

            case '0009':
                setAngle(home.angle.y - 0x2000);
                // fallthrough
            case '0010':
                initTalk(field_0xe14, NULL);
                break;

            default:
                JUT_ASSERT(2298, FALSE);
                break;
        }

    }

    int i_expression, i_motion, i_msgTimer;
    i_msgTimer = mMsgTimer;
    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE)) {
        setExpression(i_expression, -1.0f);
        setMotion(i_motion, -1.0f, FALSE);
    } else if (i_msgTimer != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    cXyz sp30;
    switch (*cutName) {
        case '0001':
            if (!step(fopAcM_searchPlayerAngleY(this), MOT_STEP, 15.0f)) {
                break;
            }

            setExpression(EXPR_NONE, -1.0f);
            setMotion(MOT_WAIT_A, -1.0f, FALSE);
            mTurnMode = 0;
            return TRUE;

        case '0002':
        case '0005':
            if (mCurAngle.y == fopAcM_searchPlayerAngleY(this)) {
                if (talkProc(NULL, TRUE, NULL)) {
                    return TRUE;
                }
            } else if (step(fopAcM_searchPlayerAngleY(this), MOT_STEP, 15.0f)) {
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                mTurnMode = 0;
            }
            break;

        case '0006':
            sp30.set(-429.52515f, -770.0f, -2582.8027f);
            step(cLib_targetAngleY(fopAcM_GetPosition_p(this), &sp30), -1, 60.0f);
            speedF = 4.7f;
            return TRUE;

        case '0007':
            Z2GetAudioMgr()->seStartLevel(Z2SE_AL_COPYROD_COMEBACK, fopAcM_GetPosition_p(daPy_getPlayerActorClass()), 0,
                                            0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            return TRUE;

        case '0008':
            Z2GetAudioMgr()->seStartLevel(Z2SE_AL_COPYROD_COMEBACK, fopAcM_GetPosition_p(daPy_getPlayerActorClass()), 0,
                                            0, 1.0f, 1.0f, -1.0f, -1.0f, 0);

            if (talkProc(NULL, TRUE, NULL)) {
                fopAcM_delete(this);
                return TRUE;
            }
            break;

        case '0009':
            if (home.angle.y == mCurAngle.y) {
                if (talkProc(NULL, TRUE, NULL)) {
                    if (mMotion != MOT_CHANT) {
                        Z2GetAudioMgr()->muteSceneBgm(190, 0.0f);
                    }

                    mSound.startCreatureVoiceLevel(Z2SE_SHAD_V_INCANTATION, -1);
                    setExpression(EXPR_CHANT, -1.0f);
                    setMotion(MOT_CHANT, -1.0f, FALSE);
                    return TRUE;
                }
            } else if (step(home.angle.y, MOT_STEP, 15.0f)) {
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                mTurnMode = 0;
            }
            break;

        case '0010':
            if (talkProc(NULL, TRUE, NULL)) {
                Z2GetAudioMgr()->unMuteSceneBgm(70);
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                setAngle(fopAcM_searchPlayerAngleY(this));
                return TRUE;
            }
            break;

        default:
            JUT_ASSERT(2382, FALSE);
            break;
    }

    return FALSE;
}

BOOL daNpcShad_c::EvCut_ToChantSpell2(int i_cutIndex) {
    dEvent_manager_c* eventManager = dComIfGp_getPEvtManager();
    int* cutName = (int*)eventManager->getMyNowCutName(i_cutIndex);
    fopAc_ac_c* actor;
    daCstaF_c* statue;

    if (eventManager->getIsAddvance(i_cutIndex) != 0) {
        switch (*cutName) {
            case '0002':
                dComIfGs_setItem(SLOT_22, 0xFF);
                mTurnMode = 0;

                actor = NULL;
                fopAcM_SearchByName(PROC_CSTAF, (fopAc_ac_c**)&actor);
                if (actor != NULL) {
                    fopAcM_OnStatus(actor, fopAcM_STATUS_UNK_0x800);
                }

                dComIfGp_getEvent().startCheckSkipEdge(this);
                dComIfGp_getEvent().onSkipFade();
                // fallthrough
            case '0003':
                setLookMode(LOOK_NONE, NULL, NULL);
                // falllthrough
            case '0005':
                initTalk(field_0xe14, NULL);
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                break;

            case '0006':
                Z2GetAudioMgr()->unMuteSceneBgm(70);
                initTalk(field_0xe14, NULL);
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                break;

            case '0007':
                mTurnMode = 0;
                setMotion(MOT_SAD_WALK, 15.0f, FALSE);
                break;

            case '0008':
                if (getPathPoint(getPathID(), 1, &home.pos)) {
                    current.pos = home.pos;
                    old.pos = current.pos;
                    speedF = 0.0f;
                    field_0xe14 = 0x42;
                    setExpression(EXPR_H_DISCOURAGED, -1.0f);
                    setMotion(MOT_DISCOURAGED_WAIT, -1.0f, FALSE);
                }
                break;

            case '0009':
                setAngle(home.angle.y - 0x2000);
                initTalk(field_0xe14, NULL);
                break;

            case '0010':
                statue = NULL;
                fopAcM_SearchByName(PROC_CSTAF, (fopAc_ac_c**)&statue);
                if (statue != NULL) {
                    statue->onCoverVanishFlg();
                }
                break;

            default:
                JUT_ASSERT(2451, FALSE);
                break;
        }
    }

    if (dComIfGp_getEvent().checkSkipEdge()) {
        Z2GetAudioMgr()->unMuteSceneBgm(70);
        daNpcF_clearMessageTmpBit();
        daNpcF_onEvtBit(0x12F);
        field_0xe14 = 0x42;
        mOrderEvtNo = 5;
        dMsgObject_onKillMessageFlag();
        changeEvent(l_evtArcs[mOrderEvtNo], l_evtNames[mOrderEvtNo], 1, 0xFFFF);
    }

    int i_expression, i_motion, i_msgTimer;
    i_msgTimer = mMsgTimer;
    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE) != 0) {
        setExpression(i_expression, -1.0f);
        setMotion(i_motion, -1.0f, FALSE);
    } else if (i_msgTimer != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    cXyz sp30;
    switch (*cutName) {
        case '0002':
            if (step(fopAcM_searchPlayerAngleY(this), MOT_STEP, 15.0f)) {
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                mTurnMode = 0;
                return TRUE;
            }
            break;

        case '0003':
            if (fopAcM_searchPlayerAngleY(this) == mCurAngle.y) {
                if (talkProc(NULL, TRUE, NULL)) {
                    return TRUE;
                }
            } else if (step(fopAcM_searchPlayerAngleY(this), MOT_STEP, 15.0f)) {
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                mTurnMode = 0;
            }
            break;

        case '0005':
        case '0006':
            if (talkProc(NULL, TRUE, NULL)) {
                return TRUE;
            }
            break;

        case '0007':
            if (getPathPoint(getPathID(), 1, &sp30)) {
                step(cLib_targetAngleY(fopAcM_GetPosition_p(this), &sp30), -1, 60.0f);
                speedF = 3.0f;
            }
            return TRUE;

        case '0008':
            return TRUE;

        case '0009':
            if (home.angle.y == mCurAngle.y) {
                if (talkProc(NULL, TRUE, NULL)) {
                    if (mMotion != MOT_CHANT) {
                        Z2GetAudioMgr()->muteSceneBgm(190, 0.0f);
                    }

                    mSound.startCreatureVoiceLevel(Z2SE_SHAD_V_INCANTATION_L, -1);
                    setExpression(EXPR_CHANT, -1.0f);
                    setMotion(MOT_CHANT, -1.0f, FALSE);
                    return TRUE;
                }
            } else if (step(home.angle.y, MOT_STEP, 15.0f)) {
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                mTurnMode = 0;
            }

            break;

        case '0010':
            return TRUE;
            
        default:
            JUT_ASSERT(2540, FALSE);
            break;
    }

    return FALSE;
}

BOOL daNpcShad_c::EvCut_ToChantSpell2Skip(int i_cutIndex) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* cutName = (int*)eventManager->getMyNowCutName(i_cutIndex);

    if (eventManager->getIsAddvance(i_cutIndex)) {
        switch (*cutName) {
            case '0001':
                return TRUE;

            case '0002':
                if (getPathPoint(getPathID(), 1, &home.pos)) {
                    current.pos = home.pos;
                    old.pos = current.pos;
                    speedF = 0.0f;
                    setExpression(EXPR_H_DISCOURAGED, -1.0f);
                    setMotion(MOT_DISCOURAGED_WAIT, -1.0f, FALSE);
                }
                return TRUE;

            default:
                JUT_ASSERT(2574, FALSE);
                break;
        }
    }

    return FALSE;
}

inline void daNpcShad_c::setLookObliquenessUp() {
    cXyz sp18(0.0f, 100.0f, 100.0f);
    mDoMtx_stack_c::transS(mLookatPos[2]);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::multVec(&sp18, &sp18);
    setLookMode(LOOK_ATTN, NULL, &sp18);
}

BOOL daNpcShad_c::EvCut_DiscoveryCannon(int i_cutIndex) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* cutName = (int*)eventManager->getMyNowCutName(i_cutIndex);
    cXyz* pos;
    int* angle;

    if (eventManager->getIsAddvance(i_cutIndex)) {
        switch (*cutName) {
            case '0001':
                dComIfGp_getEvent().startCheckSkipEdge(this);
                Z2GetAudioMgr()->bgmStreamPrepare(0x200007C);
                Z2GetAudioMgr()->bgmStreamPlay();
                setExpression(EXPR_NONE, 0.0f);
                setMotion(MOT_WAIT_A, 0.0f, FALSE);
                setLookMode(LOOK_PLAYER, NULL, NULL);
                break;

            case '0003':
                pos = dComIfGp_evmng_getMyXyzP(i_cutIndex, "POS");
                angle = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "ANGLE");

                if (pos != NULL) {
                    current.pos = *pos;
                    old.pos = current.pos;
                }

                if (angle != NULL) {
                    setAngle(*angle);
                }

                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                mTurnMode = 0;
                speedF = 0.0f;
                break;

            case '0005':
            case '0008':
                mTurnMode = 0;
                setExpression(EXPR_H_SURPRISE, -1.0f);
                setMotion(MOT_WALK_A, -1.0f, FALSE);
                speedF = 5.0f;
                break;

            case '0004':
            case '0006':
            case '0007':
                setLookMode(LOOK_NONE, NULL, NULL);
                // fallthrough
            case '0002':
                dComIfGs_onSaveSwitch(79);
                initTalk(field_0xe14, NULL);
                break;

            default:
                JUT_ASSERT(2636, FALSE);
                break;
        }
    }

    if (dComIfGp_getEvent().checkSkipEdge()) {
        Z2GetAudioMgr()->bgmStreamStop(30);
        daNpcF_clearMessageTmpBit();
        daNpcF_onEvtBit(0x312);
        mOrderEvtNo = 7;
        dMsgObject_onKillMessageFlag();
        changeEvent(l_evtArcs[mOrderEvtNo], l_evtNames[mOrderEvtNo], 1, 0xFFFF);
    }

    cXyz* pos2;
    int i_expression, i_motion, i_msgTimer;
    i_msgTimer = mMsgTimer;
    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE) != 0) {
        setExpression(i_expression, -1.0f);
        setMotion(i_motion, -1.0f, FALSE);
    } else if (i_msgTimer != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    switch (*cutName) {
        case '0001':
        case '0003':
            return TRUE;

        case '0005':
        case '0008':
            setLookObliquenessUp();
            pos2 = dComIfGp_evmng_getMyXyzP(i_cutIndex, "POS");

            if (pos2 != NULL) {
                step(cLib_targetAngleY(&current.pos, pos2), -1, 15.0f);

                if (pos2->absXZ(current.pos) < 15.0f) {
                    speedF = 0.0f;
                    setMotion(MOT_WAIT_A, -1.0f, FALSE);
                    return TRUE;
                }
            }
            break;

        case '0002':
        case '0004':
        case '0006':
        case '0007':
            if (talkProc(NULL, TRUE, NULL)) {
                return TRUE;
            }
            break;

        default:
            JUT_ASSERT(2693, FALSE);
            break;
    }

    return FALSE;
}

BOOL daNpcShad_c::EvCut_DiscoveryCannonSkip(int i_cutIndex) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* cutName = (int*)eventManager->getMyNowCutName(i_cutIndex);
    cXyz* pos;
    int* angle;

    if (eventManager->getIsAddvance(i_cutIndex)) {
        switch (*cutName) {
            case '0001':
                return TRUE;

            case '0002':
                pos = dComIfGp_evmng_getMyXyzP(i_cutIndex, "POS");
                angle = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "ANGLE");

                if (pos != NULL) {
                    current.pos = *pos;
                    old.pos = current.pos;
                }

                if (angle != NULL) {
                    setAngle(*angle);
                }

                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                speedF = 0.0f;
                return TRUE;

            default:
                JUT_ASSERT(2729, FALSE);
                break;
        }
    }

    return FALSE;
}

BOOL daNpcShad_c::EvCut_Disappear(int i_cutIndex) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* cutName = (int*)eventManager->getMyNowCutName(i_cutIndex);
    cXyz* pos;
    int* angle;

    if (eventManager->getIsAddvance(i_cutIndex)) {
        switch (*cutName) {
            case '0001':
                dComIfGp_getEvent().startCheckSkipEdge(this);
                // fallthrough
            case '0004':
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                setLookMode(LOOK_PLAYER, NULL, NULL);
                initTalk(68, NULL);
                break;

            case '0002':
                pos = dComIfGp_evmng_getMyXyzP(i_cutIndex, "POS");
                angle = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "ANGLE");

                if (pos != NULL) {
                    current.pos = *pos;
                    old.pos = current.pos;
                }

                if (angle != NULL) {
                    setAngle(*angle);
                }

                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                setLookMode(LOOK_RESET, NULL, NULL);
                speedF = 0.0f;
                break;

            case '0005':
            case '0003':
                setExpression(EXPR_H_SURPRISE, -1.0f);
                setMotion(MOT_RUN_A, -1.0f, FALSE);
                mTurnMode = 0;
                speedF = daNpcShad_Param_c::m.traveling_speed;
                break;

            case '0006':
                fopAcM_delete(this);
                break;

            default:
                JUT_ASSERT(2786, FALSE);
        }
    }

    int i_expression, i_motion, i_msgTimer;
    i_msgTimer = mMsgTimer;
    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE) != 0) {
        setExpression(i_expression, -1.0f);
        setMotion(i_motion, -1.0f, FALSE);
    } else if (i_msgTimer != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    int i_expression2, i_motion2, i_msgTimer2;
    i_msgTimer2 = mMsgTimer;
    if (ctrlMsgAnm(i_expression2, i_motion2, this, FALSE) != 0) {
        setExpression(i_expression2, -1.0f);
        setMotion(i_motion2, -1.0f, FALSE);
    } else if (i_msgTimer2 != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    if (dComIfGp_getEvent().checkSkipEdge()) {
        daNpcF_clearMessageTmpBit();
        daNpcF_onEvtBit(0x311);
        mOrderEvtNo = 9;
        dMsgObject_onKillMessageFlag();
        changeEvent(l_evtArcs[mOrderEvtNo], l_evtNames[mOrderEvtNo], 1, 0xFFFF);
    }

    cXyz* pos2;
    switch (*cutName) {
        case '0001':
            if (fopAcM_searchPlayerAngleY(this) == mCurAngle.y) {
                if (talkProc(NULL, TRUE, NULL)) {
                    setLookMode(LOOK_NONE, NULL, NULL);
                    return TRUE;
                }
            } else if (step(fopAcM_searchPlayerAngleY(this), MOT_STEP, 15.0f)) {
                setMotion(MOT_WAIT_A, -1.0f, FALSE);
                mTurnMode = 0;
            }
            break;

        case '0002':
            return TRUE;

        case '0004':
            if (talkProc(NULL, TRUE, NULL)) {
                setLookMode(LOOK_NONE, NULL, NULL);
                return TRUE;
            }
            break;

        case '0005':
        case '0003':
            pos2 = dComIfGp_evmng_getMyXyzP(i_cutIndex, "POS");

            if (pos2 != NULL) {
                step(cLib_targetAngleY(&current.pos, pos2), -1, 15.0f);
                
                if (pos2->x > current.pos.x) {
                    speedF = 0.0f;
                    setMotion(MOT_WAIT_A, -1.0f, FALSE);
                    return TRUE;
                }
            }
            break;

        case '0006':
            return TRUE;

        default:
            JUT_ASSERT(2873, FALSE);
            break;
    }

    return FALSE;
}

BOOL daNpcShad_c::EvCut_DisappearSkip(int i_cutIndex) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* cutName = (int*)eventManager->getMyNowCutName(i_cutIndex);

    if (eventManager->getIsAddvance(i_cutIndex)) {
        switch (*cutName) {
            case '0001':
                return TRUE;

            case '0002':
                field_0xe1e = 0;
                return TRUE;

            case '0003':
                dComIfGp_event_reset();
                fopAcM_delete(this);
                return TRUE;

            default:
                JUT_ASSERT(2905, FALSE);
                break;
        }
    }

    return FALSE;
}

BOOL daNpcShad_c::EvCut_CallBack(int i_cutIndex) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* cutName = (int*)eventManager->getMyNowCutName(i_cutIndex);
    cXyz sp30;

    if (eventManager->getIsAddvance(i_cutIndex)) {
        switch (*cutName) {
            case '0001':
                sp30.set(4100.0f, -1520.0f, -3855.8125f);
                setAngle(cLib_targetAngleY(&current.pos, &sp30));
                setLookMode(LOOK_RESET, NULL, NULL);
                break;

            case '0002':
                initTalk(69, NULL);
                break;

            default:
                JUT_ASSERT(2939, FALSE);
                break;
        }
    }

    int i_expression, i_motion, i_msgTimer;
    i_msgTimer = mMsgTimer;
    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE) != 0) {
        setExpression(i_expression, -1.0f);
        setMotion(i_motion, -1.0f, FALSE);
    } else if (i_msgTimer != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    switch (*cutName) {
        case '0001':
            return TRUE;

        case '0002':
            if (talkProc(NULL, TRUE, NULL)) {
                return TRUE;
            }
            break;

        default:
            JUT_ASSERT(2965, FALSE);
            break;
    }

    return FALSE;
}

BOOL daNpcShad_c::EvCut_WiretapSponsor(int i_cutIndex) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    fopAc_ac_c* actors[4] = {mActorMngr[2].getActorP(), mActorMngr[1].getActorP(), this, mActorMngr[3].getActorP()};
    int* cutName = (int*)eventManager->getMyNowCutName(i_cutIndex);

    if (eventManager->getIsAddvance(i_cutIndex)) {
        switch (*cutName) {
            case '0001':
                dComIfGp_setMesgCameraInfoActor(actors[0], actors[1], actors[2], actors[3], NULL, NULL, NULL, NULL, NULL, NULL);
                initTalk(field_0xe14, (fopAc_ac_c**)&actors[0]);
                break;

            default:
                JUT_ASSERT(3002, FALSE);
                break;
        }
    }

    int i_expression, i_motion, i_msgTimer;
    i_msgTimer = mMsgTimer;
    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE) != 0) {
        setExpression(i_expression, -1.0f);
        setMotion(i_motion, -1.0f, FALSE);
    } else if (i_msgTimer != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    fopAc_ac_c* talkPartner;
    switch (*cutName) {
        case '0001':
            if (talkProc(NULL, TRUE, (fopAc_ac_c**)&actors[0])) {
                return TRUE;
            }

            talkPartner = dComIfGp_event_getTalkPartner();
            if (talkPartner == this) {
                talkPartner = mActorMngr[2].getActorP();
            }
            setLookMode(LOOK_ACTOR, talkPartner, NULL);
            break;

        default:
            JUT_ASSERT(3032, FALSE);
            break;
    }
    return FALSE;
}

BOOL daNpcShad_c::EvCut_WiretapEntrant(int i_cutIndex) {
    dEvent_manager_c* eventManager = &dComIfGp_getEventManager();
    int* prm = dComIfGp_evmng_getMyIntegerP(i_cutIndex, "prm");

    if (prm == NULL) {
        return FALSE;
    }

    if (eventManager->getIsAddvance(i_cutIndex)) {
        switch (*prm) {
            case 0:
                setExpression(EXPR_NONE, -1.0f);
                setMotion(MOT_SIT_A, -1.0f, FALSE);
                break;

            default:
                JUT_ASSERT(3065, FALSE);
                break;
        }
    }

    int i_expression, i_motion, i_msgTimer;
    i_msgTimer = mMsgTimer;
    if (ctrlMsgAnm(i_expression, i_motion, this, FALSE) != 0) {
        setExpression(i_expression, -1.0f);
        setMotion(i_motion, -1.0f, FALSE);
    } else if (i_msgTimer != 0 && mMsgTimer == 0) {
        setExpressionTalkAfter();
    }

    fopAc_ac_c* talkPartner;
    switch (*prm) {
        case 0:
            talkPartner = dComIfGp_event_getTalkPartner();
            if (talkPartner == this) {
                talkPartner = mActorMngr[2].getActorP();
            }
            setLookMode(LOOK_ACTOR, talkPartner, NULL);
            return TRUE;

        default:
            JUT_ASSERT(3088, FALSE);
            break;
    }

    return FALSE;
}

static int daNpcShad_Create(void* a_this) {
    daNpcShad_c* i_this = (daNpcShad_c*)a_this;
    return i_this->Create();
}

static int daNpcShad_Delete(void* a_this) {
    daNpcShad_c* i_this = (daNpcShad_c*)a_this;
    return i_this->Delete();
}

static int daNpcShad_Execute(void* a_this) {
    daNpcShad_c* i_this = (daNpcShad_c*)a_this;
    return i_this->Execute();
}

static int daNpcShad_Draw(void* a_this) {
    daNpcShad_c* i_this = (daNpcShad_c*)a_this;
    return i_this->Draw();
}

static int daNpcShad_IsDelete(void* a_this) {
    return 1;
}

// force J3DTexNoAnm::calc to be emitted earlier than it otherwise would be
static void dummy() {
    ((J3DTexNoAnm*)NULL)->calc(NULL);
}

void daNpcShad_c::setParam() {
    searchActors();
    attention_info.flags = fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e;

    if (isSneaking()) {
        attention_info.distances[fopAc_attn_LOCK_e] = 78;
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = 77;
        attention_info.flags |= fopAc_AttnFlag_UNK_0x800000;
    } else if (mMode == 0) {
        attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(3, 5);
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(2, 5);
    } else {
        attention_info.distances[fopAc_attn_LOCK_e] = getDistTableIdx(mpHIO->m.common.attention_distance, mpHIO->m.common.attention_angle);
        attention_info.distances[fopAc_attn_TALK_e] = attention_info.distances[fopAc_attn_LOCK_e];
        attention_info.distances[fopAc_attn_SPEAK_e] = getDistTableIdx(mpHIO->m.common.talk_distance, mpHIO->m.common.talk_angle);
    }

    mAcchCir.SetWallR(mpHIO->m.common.width);
    mAcchCir.SetWallH(mpHIO->m.common.knee_length);
    gravity = mpHIO->m.common.gravity;
}

BOOL daNpcShad_c::main() {
    if (mActionFn) {
        (this->*mActionFn)(NULL);
    }

    if (dComIfGp_event_runCheck() != 0 && !eventInfo.checkCommandTalk() && mItemPartnerId != fpcM_ERROR_PROCESS_ID_e) {
        dComIfGp_event_setItemPartnerId(mItemPartnerId);
        mItemPartnerId = fpcM_ERROR_PROCESS_ID_e;
    }

    playExpression();
    playMotion();
    return TRUE;
}

inline BOOL daNpcShad_c::ctrlBtk() {
    if (mpMatAnm != NULL) {
        J3DAnmTextureSRTKey* btkAnm = NULL;
        btkAnm = getTexSRTKeyAnmP(l_arcNames[l_btkGetParamList[0].arcIdx], l_btkGetParamList[0].fileIdx);

        if (btkAnm == mBtkAnm.getBtkAnm()) {
            mpMatAnm->setNowOffsetX(cM_ssin(mEyeAngle.y) * 0.2f * -1.0f);
            mpMatAnm->setNowOffsetY(cM_ssin(mEyeAngle.x) * 0.2f);
            mpMatAnm->onEyeMoveFlag();
            return TRUE;
        }

        mpMatAnm->offEyeMoveFlag();
    }

    return FALSE;
}

inline void daNpcShad_c::setAttnPos() {
    if (mLookMode == 1) {
        for (int i = 0; i < 3; i++) {
            mLookatAngle[i].setall(0);
        }
    }

    setMtx();
    lookat();

    cXyz sp40(10.0f, 10.0f, 0.0f);
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_HEAD));
    mDoMtx_stack_c::multVecZero(&mHeadPos);
    mDoMtx_stack_c::multVec(&sp40, &eyePos);
    sp40.x = 0.0f;
    mDoMtx_stack_c::multVec(&sp40, &sp40);
    mHeadAngle.x = cLib_targetAngleX(&mHeadPos, &sp40);
    mHeadAngle.y = cLib_targetAngleY(&mHeadPos, &sp40);

    cXyz* attnPos = mLookat.getAttnPos();
    if (attnPos != NULL) {
        cXyz sp4c(*attnPos - eyePos);
        mEyeAngle.y = -mLookatAngle[2].y - mCurAngle.y;
        mEyeAngle.y += cM_atan2s(sp4c.x, sp4c.z);
        mEyeAngle.x = mHeadAngle.x - cM_atan2s(sp4c.y, sp4c.absXZ());
    } else {
        mEyeAngle.y = 0;
        mEyeAngle.x = 0;
    }

    attention_info.position.set(mHeadPos.x, mHeadPos.y + mpHIO->m.common.attention_offset, mHeadPos.z);
    cXyz sp58;
    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(JNT_BACKBONE2));
    mDoMtx_stack_c::multVecZero(&sp58);
    sp58.y = current.pos.y;
    
    field_0xcb0.SetC(sp58);
    field_0xcb0.SetH(mpHIO->m.common.height);

    f32 fVar1;
    if (mMode == 0) {
        fVar1 = 45.0f;
    } else {
        fVar1 = mpHIO->m.common.width;
    }
    field_0xcb0.SetR(fVar1);

    if (field_0xe1e != 0) {
        dComIfG_Ccsp()->Set(&field_0xcb0);
    }
}

inline BOOL daNpcShad_c::drawDbgInfo() {
    return FALSE;
}

static actor_method_class daNpcShad_MethodTable = {
    (process_method_func)daNpcShad_Create,
    (process_method_func)daNpcShad_Delete,
    (process_method_func)daNpcShad_Execute,
    (process_method_func)daNpcShad_IsDelete,
    (process_method_func)daNpcShad_Draw,
};

extern actor_process_profile_definition g_profile_NPC_SHAD = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_SHAD,          // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daNpcShad_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  413,                    // mPriority
  &daNpcShad_MethodTable, // sub_method
  0x00040100,             // mStatus
  fopAc_NPC_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
