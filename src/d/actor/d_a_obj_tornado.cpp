/**
 * d_a_obj_tornado.cpp
 * Wind Column
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_tornado.h"
#include "d/d_kankyo_rain.h"
#include "d/d_s_play.h"

void daObjTrnd_c::setPntWind() {
    mWindInfluence.position = current.pos;
    mWindInfluence.mDirection.x = 0.0f;
    mWindInfluence.mDirection.y = 1.0f;
    mWindInfluence.mDirection.z = 0.0f;
    mWindInfluence.mRadius = 300.0f * scale.x;
    mWindInfluence.field_0x20 = 0.0f;
    mWindInfluence.mStrength = 0.2f;
    dKyw_pntwind_set(&mWindInfluence);
}

void daObjTrnd_c::cutPntWind() {
    dKyw_pntwind_cut(&mWindInfluence);
}

void daObjTrnd_c::movePntWind() {
    cXyz start;
    cXyz end;
    cXyz direction;
    start.x = mWindCps.mStart.x;
    start.y = mWindCps.mStart.y;
    start.z = mWindCps.mStart.z;
    end.x = mWindCps.mEnd.x;
    end.y = mWindCps.mEnd.y;
    end.z = mWindCps.mEnd.z;
    dKyr_get_vectle_calc(&start, &end, &direction);

    mWindInfluence.mDirection = direction;
    mWindInfluence.mRadius = mWindCps.mRadius * 2.0f;
    mWindInfluence.field_0x20 = 0.0f;

    cLib_addCalc(&mWindInfluence.position.x, mWindCps.mEnd.x, 0.1f, mWindInfluence.mRadius,
                 mWindInfluence.mRadius * 0.5f);
    cLib_addCalc(&mWindInfluence.position.y, mWindCps.mEnd.y, 0.1f, mWindInfluence.mRadius,
                 mWindInfluence.mRadius * 0.5f);
    cLib_addCalc(&mWindInfluence.position.z, mWindCps.mEnd.z, 0.1f, mWindInfluence.mRadius,
                 mWindInfluence.mRadius * 0.5f);

    if (mWindInfluence.position.abs(mWindCps.mEnd) < mWindInfluence.mRadius) {
        mWindInfluence.position = mWindCps.mStart;
    }
}

void daObjTrnd_c::setCpsInfo() {
    cLib_chaseF(&mNowLength, mTargetLength, scale.y * 3000.0f * 0.05f);
    mWindCps.mEnd.x = 0;
    mWindCps.mEnd.y = mNowLength;
    mWindCps.mEnd.z = 0;

    mDoMtx_stack_c::transS(mWindCps.mStart);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::multVec(&mWindCps.mEnd, &mWindCps.mEnd);
    mCps.cM3dGCps::Set(mWindCps);

    cXyz push_vec;
    push_vec.x = mWindCps.mEnd.x - mWindCps.mStart.x;
    push_vec.y = mWindCps.mEnd.y - mWindCps.mStart.y;
    push_vec.z = mWindCps.mEnd.z - mWindCps.mStart.z;

    if (!push_vec.isZero()) {
        push_vec.normalize();
    } else {
        push_vec = cXyz::Zero;
    }

    push_vec *= scale.z * 30.0f;
    mCps.SetAtVec(push_vec);
    movePntWind();

    if (!checkSE()) {
        mDoAud_seStartLevel(Z2SE_ENV_WIND_SARUDAN, 0, (mNowLength / (scale.y * 3000.0f)) * 127.0f,
                            0);
    }
}

void daObjTrnd_c::initBaseMtx() {
    setBaseMtx();
}

void daObjTrnd_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    MTXCopy(mDoMtx_stack_c::get(), mMtx);
}

static dCcD_SrcCps l_cps_src = {
    {
        {0x0, {{AT_TYPE_LANTERN_SWING, 0x0, 0x11}, {0x0, 0x0}, 0x0}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, dCcD_MTRL_WIND, 0x2},                 // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, dCcD_MTRL_NONE, 0x4},                 // mGObjTg
        {0x0},                                                         // mGObjCo
    },                                                                 // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 3000.0f, 0.0f}, 300.0f},  // mCps
    }  // mCpsAttr
};

int daObjTrnd_c::Create() {
    current.angle.z = 0;
    shape_angle.z = 0;
    home.angle.z = 0;

    initBaseMtx();
    fopAcM_SetMtx(this, mMtx);

    mStts.Init(0xFF, 0xFF, this);
    mCps.Set(l_cps_src);
    mCps.SetStts(&mStts);

    cXyz cull_min;
    cXyz cull_max;
    cull_min.x = scale.x * -300.0f;
    cull_min.y = 0.0;
    cull_min.z = scale.x * -300.0f;
    cull_max.x = scale.x * 300.0f;
    cull_max.y = scale.y * 3000.0f;
    cull_max.z = scale.x * 300.0f;
    fopAcM_setCullSizeBox(this, cull_min.x, 0, cull_min.z, cull_max.x, cull_max.y, cull_max.z);

    u32 timer = getTimer();
    if (timer == 0xFF || timer == 0) {
        mTimer = 0xFFFF;
    } else {
        mTimer = timer * 15;
    }

    mWindCps.mRadius = scale.x * 300.0f;
    mWindCps.mStart = current.pos;
    mTargetLength = scale.y * 3000.0f;
    mNowLength = mTargetLength;
    setPntWind();
    return 1;
}

int daObjTrnd_c::create() {
    fopAcM_ct(this, daObjTrnd_c);
    if (!Create()) {
        return cPhs_ERROR_e;
    }

    return cPhs_COMPLEATE_e;
}

int daObjTrnd_c::execute() {
    if (getSwbit() != 0xFF && !fopAcM_isSwitch(this, getSwbit())) {
        if (mTargetLength != 0.0f) {
            mTargetLength = 0;
            cutPntWind();
        }
        cLib_addCalc(&mWindPower, 0.3f, 0.2f, 0.1f, 0.001f);
    } else if (mTimer > 0) {
        mTimer--;
        if (mTargetLength == 0.0f) {
            cLib_addCalc(&mWindPower, 0.3f, 0.2f, 0.1f, 0.001f);
        } else {
            cLib_addCalc(&mWindPower, 0.9f, 0.5f, 0.1f, 0.001f);
        }
    } else if (mTimer == 0) {
        mTimer = 15 * getTimer();
        if (mTargetLength == 0.0f) {
            mTargetLength = scale.y * 3000.0f;
            setPntWind();
        } else {
            mTargetLength = 0.0f;
            cutPntWind();
        }
    }

    dKyw_evt_wind_set(0, home.angle.y + KREG_S(0));
    dKyw_custom_windpower(mWindPower);
    setCpsInfo();
    setBaseMtx();

    if (mNowLength != 0.0f) {
        mCps.cM3dGCps::Set(mWindCps);
        dComIfG_Ccsp()->Set(&mCps);
    }

    return 1;
}

int daObjTrnd_c::draw() {
    return 1;
}

int daObjTrnd_c::_delete() {
    dKy_getEnvlight()->evt_wind_go = 0;
    cutPntWind();
    return 1;
}

static int daObjTrnd_Draw(daObjTrnd_c* i_this) {
    return i_this->draw();
}

static int daObjTrnd_Execute(daObjTrnd_c* i_this) {
    return i_this->execute();
}

static int daObjTrnd_Delete(daObjTrnd_c* i_this) {
    return i_this->_delete();
}

static int daObjTrnd_Create(daObjTrnd_c* i_this) {
    return i_this->create();
}

void dummyString() {
    DEAD_STRING("");
}

static actor_method_class l_daObjTrnd_Method = {
    (process_method_func)daObjTrnd_Create,  (process_method_func)daObjTrnd_Delete,
    (process_method_func)daObjTrnd_Execute, (process_method_func)NULL,
    (process_method_func)daObjTrnd_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Tornado = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Tornado,        // mProcName
    &g_fpcLf_Method.base,   // sub_method
    0x00000770,              // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    444,                     // mPriority
    &l_daObjTrnd_Method,     // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
