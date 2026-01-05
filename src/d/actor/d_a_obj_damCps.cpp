/**
 * @file d_a_obj_damCps.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_obj_damCps.h"

#if DEBUG
#include "d/d_debug_viewer.h"

static daObjDamCps_HIO_c l_HIO;

static cull_box l_cull_box = {
    {-20.0f, 0.0f, -20.0f},
    {20.0f, 100.0f, 20.0f},
};

static dCcD_SrcCps l_cps_src = {
    {
        {0x0, {{0x400, 0x0, 0x1d}, {0x0, 0x0}, 0x11}},  // mObj
        {dCcD_SE_SWORD, 0x0, 0x1, 0x0, 0x0},            // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4},             // mGObjTg
        {0x0},                                          // mGObjCo
    },                                                  // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 30.0f},  // mCps
    }  // mCpsAttr
};

daObjDamCps_HIO_c::daObjDamCps_HIO_c() {
    field_0x08 = 0.0f;
    mAdj = false;
    mAngle.setall(0x0);
    mScale.setall(1.0f);
}

void daObjDamCps_HIO_c::genMessage(JORMContext* ctx) {
    // Damage Cylinder
    ctx->genLabel("ダメージ円柱", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Adjustment
    ctx->genCheckBox("調整", &mAdj, 1, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // X Angle
    ctx->genSlider("Ｘ角度", &mAngle.x, -0x7fff, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Y Angle
    ctx->genSlider("Ｙ角度", &mAngle.y, -0x7fff, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // X Scale
    ctx->genSlider("Ｘスケール", &mScale.x, 0.0f, 25.5f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Y Scale
    ctx->genSlider("Ｙスケール", &mScale.y, 0.0f, 25.5f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

void daObjDamCps_c::initBaseMtx() {
    mScale = scale;
    setBaseMtx();
}

void daObjDamCps_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::XrotM(shape_angle.x);
    MTXCopy(mDoMtx_stack_c::get(), mMtx);
}

int daObjDamCps_c::Create() {
    cXyz start = cXyz(0.0f, 0.0f, 0.0f);
    cXyz end = cXyz(0.0f, 100.0f, 0.0f);
    f32 radius = scale.x * 30.0f;
    end.y *= scale.y;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::XrotM(shape_angle.x);
    mDoMtx_stack_c::multVec(&start, &start);
    mDoMtx_stack_c::multVec(&end, &end);

    mCapsule.mStart = start;
    mCapsule.mEnd = end;
    mCapsule.mRadius = radius;

    mCps.cM3dGCps::Set(mCapsule);
    mCps.CalcAtVec();

    initBaseMtx();
    fopAcM_SetMtx(this, mMtx);

    mStts.Init(0xff, 0xff, this);
    mCps.Set(l_cps_src);
    mCps.SetStts(&mStts);

    fopAcM_setCullSizeBox(this, l_cull_box.min.x * scale.x, l_cull_box.min.y * scale.y,
                          l_cull_box.min.z * scale.x, l_cull_box.max.x * scale.x,
                          l_cull_box.max.y * scale.y, l_cull_box.max.z * scale.x);
    return 1;
}

int daObjDamCps_c::create() {
    fopAcM_ct(this, daObjDamCps_c);

    if (!Create()) {
        return cPhs_ERROR_e;
    }

    l_HIO.entryHIO("ダメージ円柱");  // Damage Cylinder

    return cPhs_COMPLEATE_e;
}

int daObjDamCps_c::execute() {
    if (l_HIO.mAdj) {
        shape_angle.x = l_HIO.mAngle.x;
        shape_angle.y = l_HIO.mAngle.y;
        scale.x = l_HIO.mScale.x;
        scale.y = l_HIO.mScale.y;
    } else {
        shape_angle.x = home.angle.x;
        shape_angle.y = home.angle.y;
        scale.x = mScale.x;
        scale.y = mScale.y;
    }

    if (!fopAcM_isSwitch(this, getSwNo())) {
        cXyz start = cXyz(0.0f, 0.0f, 0.0f);
        cXyz end = cXyz(0.0f, 100.0f, 0.0f);
        f32 radius = scale.x * 30.0f;
        end.y *= scale.y;

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::XrotM(shape_angle.x);
        mDoMtx_stack_c::multVec(&start, &start);
        mDoMtx_stack_c::multVec(&end, &end);

        mCapsule.mStart = start;
        mCapsule.mEnd = end;
        mCapsule.mRadius = radius;

        mCps.cM3dGCps::Set(mCapsule);
        dComIfG_Ccsp()->Set(&mCps);
    }

    setBaseMtx();

    return 1;
}

u8 daObjDamCps_c::getSwNo() {
    return fopAcM_GetParamBit(this, 0, 8);
}

int daObjDamCps_c::draw() {
    Mtx mtx;
    GXColor color = (GXColor){0xff, 0x00, 0x00, 0x80};

    if (fopAcM_isSwitch(this, getSwNo())) {
        return 1;
    }

    mDoMtx_stack_c::transS(mCps.GetStart());
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::XrotM(shape_angle.x);
    mDoMtx_stack_c::scaleM(scale.x * 30.0f, scale.y * 50.0f, scale.x * 30.0f);
    mDoMtx_stack_c::transM(0.0f, 1.0f, 0.0f);
    mDoMtx_stack_c::XrotM(0x4000);
    MTXCopy(mDoMtx_stack_c::get(), mtx);
    dDbVw_drawCylinderMXlu(mtx, color, 1);

    return 1;
}

int daObjDamCps_c::_delete() {
    l_HIO.removeHIO();
    return 1;
}

static int daObjDamCps_Draw(daObjDamCps_c* i_this) {
    return i_this->draw();
}

static int daObjDamCps_Execute(daObjDamCps_c* i_this) {
    return i_this->execute();
}

static int daObjDamCps_Delete(daObjDamCps_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "DamCps");
    return i_this->_delete();
}

static int daObjDamCps_Create(fopAc_ac_c* i_this) {
    fopAcM_RegisterCreateID(daObjDamCps_c, i_this, "DamCps");
    return a_this->create();
}

static actor_method_class l_daObjDamCps_Method = {
    (process_method_func)daObjDamCps_Create,  (process_method_func)daObjDamCps_Delete,
    (process_method_func)daObjDamCps_Execute, (process_method_func)NULL,
    (process_method_func)daObjDamCps_Draw,
};
#endif

actor_process_profile_definition g_profile_Obj_DamCps = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Obj_DamCps,        // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daObjDamCps_c),  // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    629,                    // mPriority
#if DEBUG
    &l_daObjDamCps_Method,  // sub_method
#else
    NULL,  // sub_method
#endif
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
