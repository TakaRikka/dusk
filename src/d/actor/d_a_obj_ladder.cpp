/**
 * @file d_a_obj_ladder.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_ladder.h"
#include "JSystem/J3DGraphAnimator/J3DModelData.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_s.h"
#include "f_op/f_op_actor_mng.h"

namespace daObjLadder {
namespace {
struct Attr_c {
    /* 0x00 */ f32 gravity;
    /* 0x04 */ f32 viscous_resistance;
    /* 0x08 */ f32 inertia_resistance;
    /* 0x0C */ f32 reflectance;
    /* 0x10 */ u8 num_of_bounces;
    /* 0x11 */ u8 bound_vol_0;
    /* 0x12 */ u8 bound_vol_1;
    /* 0x13 */ u8 bound_vol_2;
    /* 0x14 */ u8 bound_vol_3;
    /* 0x16 */ s16 vibration_speed_x;
    /* 0x18 */ s16 vibration_speed_y;
    /* 0x1A */ s16 vibration_timer;
    /* 0x1C */ f32 vibration_amp_x;
    /* 0x20 */ f32 vibration_amp_y;
};

static Attr_c const L_attr = {
    -3.0f,
    0.005f,
    0.0005f,
    0.5f,
    //
    3,
    75,
    50,
    45,
    40,
    //
    20000,
    15555,
    15,
    2.0f,
    1.0f,
};

inline static const Attr_c& attr() {
    return L_attr;
}

struct AttrType {
    /* 0x0 */ s16 field_0x0;
    /* 0x2 */ s16 field_0x2;
    /* 0x4 */ f32 field_0x4;
};

static AttrType L_attr_type[6] = {
    { 0x08, 0x10, 600.0f},
    { 0x09, 0x11, 900.0f},
    { 0x04, 0x0C, 1200.0f},
    { 0x05, 0x0D, 1500.0f},
    { 0x07, 0x0F, 450.0f},
    { 0x06, 0x0E, 300.0f},
};

static inline const AttrType& attr_type(Type_e type) {
    return L_attr_type[type];
}

#if DEBUG
class Hio_c : public JORReflexible {
public:
    Hio_c();
    virtual ~Hio_c() {}

    void default_set();
    void ct();
    void dt();
    void listenPropertyEvent(const JORPropertyEvent*);
    void genMessage(JORMContext*);

    Attr_c attr;
    int mCount;
    bool field_0x2c;
    bool field_0x2d;
};

Hio_c::Hio_c() {
    mCount = 0;
    field_0x2c = false;
    field_0x2d = false;
    default_set();
}

void Hio_c::default_set() {
    attr = L_attr;
}

void Hio_c::ct() {
    if (mCount++ == 0) {
        // ladder
        daObj::HioVarious_c::init(this, "はしご");
    }
}

void Hio_c::dt() {
    if (--mCount == 0) {
        daObj::HioVarious_c::clean(this);
    }
}

void Hio_c::genMessage(JORMContext* ctx) {
    // ladder
    ctx->genLabel("§ はしご  §\n", 0);
    // reset to initial position
    ctx->genButton("初期位置に", 1);
    // attack
    ctx->genButton("アタック", 2);
    // gravity
    ctx->genSlider("重力", &attr.gravity, -30.0f, 0.0f);
    // viscous resistance
    ctx->genSlider("粘性抵抗", &attr.viscous_resistance, 0.0f, 0.1f);
    // inertia resistance
    ctx->genSlider("慣性抵抗", &attr.inertia_resistance, 0.0f, 0.1f);
    // reflectance
    ctx->genSlider("反射率", &attr.reflectance, 0.0f, 1.0f);
    //
    // number of bounces
    ctx->genSlider("バウンド回数", &attr.num_of_bounces, 0, 10);
    // Bound Vol_0, 1, 2, 3
    ctx->genSlider("バウンド Vol_0", &attr.bound_vol_0, 0, 100);
    ctx->genSlider("バウンド Vol_1", &attr.bound_vol_1, 0, 100);
    ctx->genSlider("バウンド Vol_2", &attr.bound_vol_2, 0, 100);
    ctx->genSlider("バウンド Vol_3", &attr.bound_vol_3, 0, 100);
    //
    // vibration speed x, y
    ctx->genSlider("vib 速度 x", &attr.vibration_speed_x, 0, 0x7fff);
    ctx->genSlider("vib 速度 y", &attr.vibration_speed_y, 0, 0x7fff);
    // vibration time
    ctx->genSlider("vib 時間", &attr.vibration_timer, 0, 50);
    // vibration amplitude x, y
    ctx->genSlider("vib 振幅 x", &attr.vibration_amp_x, 0.0f, 5.0f);
    ctx->genSlider("vib 振幅 y", &attr.vibration_amp_y, 0.0f, 5.0f);
}

void Hio_c::listenPropertyEvent(const JORPropertyEvent* property) {
    JORReflexible::listenPropertyEvent(property);

    switch ((u32)property->id) {
        case 1: {
            field_0x2c = true;
            break;
        }

        case 2: {
            field_0x2d = true;
            break;
        }
    }
    // TODO.
}

Hio_c L_hio;
#endif

}

char const Act_c::M_arcname[5] = "Mhsg";

int Act_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(M_arcname, attr_type(mType).field_0x0);
    JUT_ASSERT(382, model_data != NULL);
    mModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    return mModel != NULL;
}

int Act_c::Create() {
    fopAcM_SetMtx(this, mModel->getBaseTRMtx());
    init_mtx();
    fopAcM_setCullSizeBox(this, -55.0f, -1.0f, -10.0f, 55.0f, attr_type(mType).field_0x4 + 41.0f,
                          10.0f);
    cXyz gndVec;
    mDoMtx_stack_c::push();
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::transM(0.0f, 10.0f, 5.0f);
    mDoMtx_stack_c::multVecZero(&gndVec);
    mDoMtx_stack_c::pop();
    mGndChk.SetPos(&gndVec);
    mGndChk.SetActorPid(base.base.id);
    mHeight = dComIfG_Bgsp().GroundCross(&mGndChk);
    mInDemo = false;
    mEventIdx = dComIfGp_getEventManager().getEventIdx(this, prm_get_evId());
    if (prm_get_swSave() != 0xff && fopAcM_isSwitch(this, prm_get_swSave())) {
        current.pos.y = mHeight;
        set_mtx();
        mode_fell_init();
    } else {
        mode_wait_init();
    }

    if (mHeight <= -G_CM3D_F_INF) {
        // "The ladder is falling <%s %d>\n"
        OS_REPORT_ERROR("はしごは落ちる<%s %d>\n", "d_a_obj_ladder.cpp", 459);
    }

    return 1;
}

int Act_c::Mthd_Create() {
    fopAcM_ct(this, Act_c);
    int phase_state = dComIfG_resLoad(&mPhase, M_arcname);
    if (phase_state == cPhs_COMPLEATE_e) {
        mType = prm_get_type();
        phase_state = MoveBGCreate(M_arcname, attr_type(mType).field_0x2,
            dBgS_MoveBGProc_Trans, 0xcb0, NULL);
        JUT_ASSERT(486, (phase_state == cPhs_COMPLEATE_e) || (phase_state == cPhs_ERROR_e));
#if DEBUG
        L_hio.ct();
#endif
    }
    return phase_state;
}

int Act_c::Delete() {
    return 1;
}

int Act_c::Mthd_Delete() {
#if DEBUG
    L_hio.dt();
#endif
    int rv = MoveBGDelete();
    dComIfG_resDelete(&mPhase, M_arcname);
    return rv;
}

void Act_c::demo_end_reset() {
    if (mInDemo && dComIfGp_evmng_endCheck(mEventIdx)) {
        dComIfGp_event_reset();
        mInDemo = false;
    }
}

void Act_c::mode_wait_init() {
    mMode = MODE_WAIT;
}

void Act_c::mode_wait() {
    if (fopAcM_isSwitch(this, prm_get_swSave())) {
        mode_demoreq_init();
    }
}

void Act_c::mode_demoreq_init() {
    mMode = MODE_DEMOREQ;
    mInDemo = false;
}

void Act_c::mode_demoreq() {
    bool isDemo = false;
    if (dComIfGp_evmng_existence(mEventIdx)) {
        if (eventInfo.checkCommandDemoAccrpt()) {
            isDemo = true;
            mInDemo = true;
        } else {
            fopAcM_orderOtherEventId(this, mEventIdx, prm_get_evId(), 0xffff, 0, 1);
            eventInfo.onCondition(2);
        }
    } else {
        isDemo = true;
    }
    if (isDemo) {
        mode_vib_init();
    }
}

void Act_c::mode_vib_init() {
    mVibrationTimer = attr().vibration_timer;
    field_0x610 = 0;
    field_0x612 = 0;
    mMode = MODE_VIB;
}

void Act_c::mode_vib() {
    field_0x610 += attr().vibration_speed_x;
    field_0x612 += attr().vibration_speed_y;
    field_0x614 = cM_scos(field_0x610) * attr().vibration_amp_x;
    field_0x618 = cM_scos(field_0x612) * attr().vibration_amp_y;
    if (--mVibrationTimer <= 0) {
        mode_drop_init();
    }
}

void Act_c::mode_drop_init() {
    gravity = -5.0f;
    speed.set(cXyz::Zero);
    field_0x5b6 = attr().num_of_bounces;
    mMode = MODE_DROP;
}

void Act_c::mode_drop() {
    daObj::posMoveF_stream(this, NULL, &cXyz::Zero, attr().viscous_resistance, attr().inertia_resistance);
    if (current.pos.y < mHeight) {
        if (field_0x5b6 == attr().num_of_bounces) {
            fopAcM_seStart(this, Z2SE_SY_DUMMY, dComIfG_Bgsp().GetMtrlSndId(mGndChk));
            dComIfGp_getVibration().StartShock(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        } else {
            int uVar7;
            int iVar5 = attr().num_of_bounces - field_0x5b6;
            if (iVar5 == 1) {
                uVar7 = attr().bound_vol_0;
            } else if (iVar5 == 2) {
                uVar7 = attr().bound_vol_1;
            } else if (iVar5 == 3) {
                uVar7 = attr().bound_vol_2;
            } else {
                uVar7 = attr().bound_vol_3;
            }
            fopAcM_seStart(this, Z2SE_SY_DUMMY, uVar7);
        }
        if (field_0x5b6 >= 0) {
            field_0x5b6--;
            current.pos.y =
                mHeight - (current.pos.y - mHeight) * 0.5f;
            speed.y *= -0.5f;
        } else {
            current.pos.y = mHeight;
            mode_fell_init();
        }
    }
}

void Act_c::mode_fell_init() {
    mMode = MODE_FELL;
}

void Act_c::mode_fell() {
}

Mtx Act_c::M_tmp_mtx;

void Act_c::set_mtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    cMtx_copy(mDoMtx_stack_c::get(), M_tmp_mtx);
    mDoMtx_stack_c::transM(field_0x614, field_0x618, 0.0f);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void Act_c::init_mtx() {
    mModel->setBaseScale(scale);
    set_mtx();
}

int Act_c::Execute(f32 (**param_1)[3][4]) {
    static const Act_c::modeProc mode_proc[5] = {
        &Act_c::mode_wait, &Act_c::mode_demoreq,
        &Act_c::mode_vib,  &Act_c::mode_drop,
        &Act_c::mode_fell,
    };

    demo_end_reset();
    (this->*(mode_proc[mMode]))();
    eyePos.y = current.pos.y;
    set_mtx();
    *param_1 = &M_tmp_mtx;
    return 1;
}

int Act_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mModel);
    dComIfGd_setList();
    dComIfG_Bgsp().DebugDrawPoly(mpBgW);
    return 1;
}

namespace {
    static int Mthd_Create(void* i_this) {
        return ((Act_c*)i_this)->Mthd_Create();
    }

    static int Mthd_Delete(void* i_this) {
        return ((Act_c*)i_this)->Mthd_Delete();
    }

    static int Mthd_Execute(void* i_this) {
        return ((Act_c*)i_this)->MoveBGExecute();
    }

    static int Mthd_Draw(void* i_this) {
        return ((Act_c*)i_this)->MoveBGDraw();
    }

    static int Mthd_IsDelete(void* i_this) {
        return ((Act_c*)i_this)->MoveBGIsDelete();
    }

    static actor_method_class Mthd_Table = {
        (process_method_func)Mthd_Create, 
        (process_method_func)Mthd_Delete, 
        (process_method_func)Mthd_Execute, 
        (process_method_func)Mthd_IsDelete,
        (process_method_func)Mthd_Draw,
    };
}

}

actor_process_profile_definition g_profile_Obj_Ladder = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Ladder,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjLadder::Act_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  23,                     // mPriority
  &daObjLadder::Mthd_Table,         // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
