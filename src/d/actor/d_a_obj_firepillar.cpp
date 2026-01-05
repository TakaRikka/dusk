/**
 * @file d_a_obj_firepillar.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_firepillar.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_debug_viewer.h"
#include "f_op/f_op_actor_mng.h"

void daObjFPillar_c::initBaseMtx() {
    setBaseMtx();
}

void daObjFPillar_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
}

static cull_box l_cull_box = {
    {-30.0f, -10.0f, -30.0f},
    {30.0f, 60.0f, 30.0f},
};

static dCcD_SrcCps l_cps_src = {
    {
        {0x0, {{0x100, 0x1, 0x1d}, {0x0, 0x0}, 0x0}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x1, 0x2}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 100.0f, 0.0f}, 50.0f}, // mCps
    } // mCpsAttr
};

int daObjFPillar_c::Create() {
    initBaseMtx();
    mStts.Init(0, 0xff, this);
    mCps.Set(l_cps_src);
    mCps.SetStts(&mStts);
    f32 maxy = l_cull_box.max.y * scale.y;
    fopAcM_setCullSizeBox(this,
        (l_cull_box.min.x * scale.x),
        (l_cull_box.min.y * scale.y),
        (l_cull_box.min.z * scale.x),
        (l_cull_box.max.x * scale.x),
        maxy,
        maxy);
    return 1;
}

int daObjFPillar_c::create() {
    fopAcM_ct(this, daObjFPillar_c);
    int rv = Create();
    if (rv == 0) {
        return cPhs_ERROR_e;
    }
    return cPhs_COMPLEATE_e;
}

int daObjFPillar_c::execute() {
    action();
    mCylScale.x = scale.x;
    mCylScale.z = scale.z;
    cXyz cStack_1c(0.0f, 1.0f, 0.0f);
    cStack_1c.y *= mCylScale.y * 100.0f;
    mDoMtx_stack_c::ZXYrotS(current.angle.x, current.angle.y, current.angle.z);
    mDoMtx_stack_c::multVec(&cStack_1c, &cStack_1c);
    field_0x954.mStart = current.pos;
    field_0x954.mEnd = cStack_1c + current.pos;
    field_0x954.mRadius = scale.x * 50.0f;
    if (mCylScale.y > scale.y * 0.1f) {
        mCps.cM3dGCps::Set(field_0x954);
        dComIfG_Ccsp()->Set(&mCps);
    }
    return 1;
}

void daObjFPillar_c::action() {
    static daObjFPillar_c::actionFunc l_func[3] = {
        &daObjFPillar_c::actionOff,
        &daObjFPillar_c::actionOnWait,
        &daObjFPillar_c::actionOn,
    };
    (this->*(l_func[mAction]))();
}

void daObjFPillar_c::actionOffInit() {
    u8 bVar1 = daObjFPillar_prm::getArg0(this);
    if (bVar1 == 0xff) {
        bVar1 = 1;
    }
    mActionTimer = bVar1 * 15;
    mAction = ACTION_OFF;
}

void daObjFPillar_c::actionOff() {
    cLib_chaseF(&mCylScale.y, 0.0f, scale.y * 0.1f);
    if (mActionTimer == 0) {
        actionOnWaitInit();
    } else {
        if (daObjFPillar_prm::getSwNo(this) != 0xff) {
            if (fopAcM_isSwitch(this, daObjFPillar_prm::getSwNo(this))) {
                mActionTimer--;
            }
        } else {
            mActionTimer--;
        }
    }
}

void daObjFPillar_c::actionOnWaitInit() {
    u8 bVar1 = daObjFPillar_prm::getArg1(this);
    if (bVar1 == 0xff) {
        bVar1 = 1;
    }
    mActionTimer = bVar1 * 15;
    mActionTimer = 30;
    mAction = ACTION_ON_WAIT;
}

void daObjFPillar_c::actionOnWait() {
    cLib_chaseF(&mCylScale.y, scale.y * 0.1f, scale.y * 0.02f);
    if (mActionTimer == 0) {
        actionOnInit();
    } else {
        if (daObjFPillar_prm::getSwNo(this) != 0xff) {
            if (fopAcM_isSwitch(this, daObjFPillar_prm::getSwNo(this))) {
                mActionTimer--;
            } else {
                actionOffInit();
            }
        } else {
            mActionTimer--;
        }
    }
}

void daObjFPillar_c::actionOnInit() {
    u8 bVar1 = daObjFPillar_prm::getArg2(this);
    if (bVar1 == 0xff) {
        bVar1 = 1;
    }
    mActionTimer = bVar1 * 15;
    mActionTimer = 30;
    mAction = ACTION_ON;
}

void daObjFPillar_c::actionOn() {
    f32 target = scale.y;
    if (mActionTimer == 0) {
        target = 0.0;
    } else {
        if (daObjFPillar_prm::getSwNo(this) != 0xff) {
            if (fopAcM_isSwitch(this, daObjFPillar_prm::getSwNo(this))) {
                mActionTimer--;
            } else {
                target = 0.0;
            }
        } else {
            mActionTimer--;
        }
    }
    int reachedTarget = cLib_chaseF(&mCylScale.y, target, scale.y * 0.1f);
    if (reachedTarget && target == 0.0f) {
        actionOffInit();
    }
}

static void drawCylinder(cXyz* pos, cXyz* scale, csXyz* angle) {
    static GXColor const color = {0xFF,00,00,0x60};

    #if DEBUG
    mDoMtx_stack_c::transS(pos->x, pos->y, pos->z);
    mDoMtx_stack_c::ZXYrotM(angle->x, angle->y, angle->z);
    mDoMtx_stack_c::scaleM(scale->x * 50.0f, scale->y * 50.0f, scale->x * 50.0f);
    mDoMtx_stack_c::transM(0.0f, 1.0f, 0.0f);
    mDoMtx_stack_c::XrotM(0x4000);
    Mtx mtx;
    MTXCopy(mDoMtx_stack_c::get(), mtx);
    dDbVw_drawCylinderMXlu(mtx, color, 1);
    #endif
}

int daObjFPillar_c::draw() {
    drawCylinder(&current.pos, &mCylScale, &current.angle);
    return 1;
}

int daObjFPillar_c::_delete() {
    return 1;
}

static int daObjFPillar_Draw(daObjFPillar_c* i_this) {
    return i_this->draw();
}

static int daObjFPillar_Execute(daObjFPillar_c* i_this) {
    return i_this->execute();
}

static int daObjFPillar_Delete(daObjFPillar_c* i_this) {
    fopAcM_GetID(i_this);
    return i_this->_delete();
}

static int daObjFPillar_Create(fopAc_ac_c* i_this) {
    fopAcM_GetID(i_this);
    return static_cast<daObjFPillar_c*>(i_this)->create();
}

static actor_method_class l_daObjFPillar_Method = {
    (process_method_func)daObjFPillar_Create,
    (process_method_func)daObjFPillar_Delete,
    (process_method_func)daObjFPillar_Execute,
    NULL,
    (process_method_func)daObjFPillar_Draw,
};

actor_process_profile_definition g_profile_Obj_FirePillar = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_FirePillar,    // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjFPillar_c), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  446,                    // mPriority
  &l_daObjFPillar_Method, // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
