/**
 * @file d_a_obj_sakuita.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_sakuita.h"
#include "d/d_a_obj.h"
#include "d/d_com_inf_game.h"

#if DEBUG
class daObjSakuita_Hio_c : public JORReflexible {
public:
    void genMessage(JORMContext* ctx);
    void ct();
    void dt();

    /* 0x04 */ int field_0x04;
    /* 0x08 */ daObjSakuita_Attr_c mAttr;
};

void daObjSakuita_Hio_c::ct() {
    if (field_0x04++ == 0) {
        daObj::HioVarious_c::init(this, "カラカラ板");
    }
}

void daObjSakuita_Hio_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("§ カラカラ板パラメータ設定  §\n", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("重力", &mAttr.mGravity, 0.0f, 200.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("円柱半径", &mAttr.mCylR, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("円柱高さ", &mAttr.mCylH, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("オフセット", &mAttr.mOffset, -300.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("板の重さ", &mAttr.mBoardWeight, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("吊り下げ位置", &mAttr.mHangingPosition, -50.0f, 0.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("windRate", &mAttr.mWindRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("speedRate", &mAttr.mSpeedRate, 0.0f, 5.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("風震動", &mAttr.mWindShake, 0.0f, 30000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("Y震動", &mAttr.mYVibration, 0.0f, 30000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("Y収束", &mAttr.mYConvergence, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("Y周期", &mAttr.mYPeriod, -0x4000, 0x3fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

void daObjSakuita_Hio_c::dt() {
    if (--field_0x04 == 0) {
        daObj::HioVarious_c::clean(this);
    }
}

static daObjSakuita_Hio_c M_hio;
#endif

daObjSakuita_Attr_c const daObjSakuita_c::M_attr = {
    20.0f, 12.0f, 70.0f, -50.0f, 0.8f, 0.5f, -6.0f, 900.0f, 20000.0f, 100.0f, 0x1400, 0x14,
};

static char* l_arcName = "A_Sakuita";

void daObjSakuita_c::create_init() {
    fopAcM_setCullSizeBox(this, -30.0f, -90.0f, -10.0f, 30.0f, 20.0f, 10.0f);
    gravity = attr().mGravity;
    field_0x71c.set(current.pos);
    field_0x71c.y += attr().mOffset;
    field_0x728 = field_0x71c;
    initBaseMtx();
    J3DModelData* modelData = (J3DModelData*) dComIfG_getObjectRes(l_arcName, "A_Sakuita.bmd");
    JUT_ASSERT(210, modelData != NULL);
    if (dComIfGp_addSimpleModel(modelData, fopAcM_GetRoomNo(this), 0) == -1) {
        OS_REPORT("\x1B[43;30mカラカラ板：シンプルモデル登録失敗しました。\n\x1B[m");
    }
    field_0x74c = 1;

    #if DEBUG
    M_hio.ct();
    #endif
}

void daObjSakuita_c::initBaseMtx() {
    setBaseMtx();
}

void daObjSakuita_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZrotM(field_0x744.z);
    mDoMtx_stack_c::XrotM(field_0x744.x);
    mDoMtx_stack_c::YrotM(field_0x744.y);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), mMtx);
}

f32 dummyLiteral() { return 0.5f; }

void daObjSakuita_c::setPlatePos() {
    cXyz cStack_24;
    cXyz cStack_30;
    f32 local_88;
    dKyw_get_AllWind_vec(&current.pos, &cStack_30, &local_88);
    if (cM_rnd() < 0.1f) {
        cStack_30 *= (local_88 * attr().mOffset) * attr().mWindRate;
    } else {
        cStack_30 *= 0.0f;
    }

    cStack_24 = current.pos - field_0x71c;
    cStack_24.y += gravity;
    field_0x74a += attr().mYPeriod;
    if (cLib_chaseF(&field_0x740, 0.0f, attr().mYConvergence) != 0) {
        field_0x74a = 0;
    }
    cStack_24 += field_0x734;
    if (cM_rnd() < 0.4f) {
        cStack_24 += cStack_30;
        if (field_0x740 < 8192.0f) {
            field_0x740 += local_88 * attr().mWindShake;
        }
    }
    cStack_24.normalizeZP();
    field_0x71c = current.pos + (cStack_24 * attr().mOffset);
    field_0x734 = (field_0x734 + (field_0x728 - field_0x71c)) * attr().mSpeedRate;
    field_0x728 = field_0x71c;
}

void daObjSakuita_c::calcAngle() {
    cXyz local_28 = current.pos - field_0x71c;
    field_0x744.x = cM_atan2s(local_28.z, local_28.y);
    field_0x744.y = shape_angle.y + (field_0x740 * cM_scos(field_0x74a));
    field_0x744.z = -cM_atan2s(local_28.x, local_28.y);
}

inline int daObjSakuita_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*) dComIfG_getObjectRes(l_arcName, "A_Sakuita.bmd");
    JUT_ASSERT(567, modelData != NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0x20000, 0x11000084);
    return mModel != NULL ? 1 : 0;
}

static int createSolidHeap(fopAc_ac_c* i_actor) {
    return static_cast<daObjSakuita_c*>(i_actor)->createHeap();
}

inline int daObjSakuita_c::draw() {
    dComIfGp_entrySimpleModel(mModel, fopAcM_GetRoomNo(this));
    return 1;
}

static int daObjSakuita_Draw(daObjSakuita_c* i_this) {
    return i_this->draw();
}

inline int daObjSakuita_c::execute() {
    #if DEBUG
    gravity = attr().mGravity;
    #endif
    current.pos.y += attr().mHangingPosition;
    setPlatePos();
    calcAngle();
    setBaseMtx();
    return 1;
}

static int daObjSakuita_Execute(daObjSakuita_c* i_this) {
    return i_this->execute();
}

static int daObjSakuita_IsDelete(daObjSakuita_c* i_this) {
    return 1;
}

static int daObjSakuita_Delete(daObjSakuita_c* i_this) {
    fopAcM_GetID(i_this);
    i_this->~daObjSakuita_c();
    return 1;
}

daObjSakuita_c::~daObjSakuita_c() {
    #if DEBUG
    M_hio.dt();
    #endif

    if (field_0x74c != 0) {
        J3DModelData* modelData = (J3DModelData*) dComIfG_getObjectRes(l_arcName, "A_Sakuita.bmd");
        JUT_ASSERT(160, modelData != NULL);
        dComIfGp_removeSimpleModel(modelData, fopAcM_GetRoomNo(this));
    }
    dComIfG_resDelete(&mPhase, l_arcName);
}

inline int daObjSakuita_c::create() {
    fopAcM_ct(this, daObjSakuita_c);
    int rv = dComIfG_resLoad(&mPhase, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, createSolidHeap, 0x4000) == 0) {
            return cPhs_ERROR_e;
        }

        create_init();
        fopAcM_SetMtx(this, mMtx);
    }
    return rv;
}

static int daObjSakuita_Create(fopAc_ac_c* i_actor) {
    fopAcM_GetID(i_actor);
    return static_cast<daObjSakuita_c*>(i_actor)->create();
}

static actor_method_class l_daObjSakuita_Method = {
    (process_method_func)daObjSakuita_Create,
    (process_method_func)daObjSakuita_Delete,
    (process_method_func)daObjSakuita_Execute,
    (process_method_func)daObjSakuita_IsDelete,
    (process_method_func)daObjSakuita_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Sakuita = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Sakuita,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjSakuita_c), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  36,                     // mPriority
  &l_daObjSakuita_Method, // sub_method
  0x00040180,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
