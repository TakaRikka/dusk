/**
* @file d_a_obj_glowSphere.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_glowSphere.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"

daGlwSph_HIO_c::daGlwSph_HIO_c() {
    speed = 50.0f;
    speed2 = 4.0f;
}

static daGlwSph_HIO_c l_HIO;

static cull_box l_cull_box = {
    {-30.0f, -10.0f, -30.0f},
    {30.0f, 60.0f, 30.0f}
};

const dCcD_SrcGObjInf daGlwSph_c::mCcDObjInfo = {
    {0, {{0, 0, 0}, {0x10000, 0x11}, {0x19}}},
    {dCcD_SE_SWORD, 0, 0, 0, 0x0},
    {dCcD_SE_STONE, 0, 0, 0, 0x2},
    {0},
};

dCcD_SrcSph daGlwSph_c::mCcDSph = {
    daGlwSph_c::mCcDObjInfo,
    {
        {{0.0f, 0.0f, 0.0f}, 0.0f}  // mSph
    }  // mSphAttr
};

static GXColor l_prmColor = {0xFF, 0xFF, 0xFF, 0xFF};

static GXColor l_envColor[] = {
    /* Gray   */ {0x3C, 0x1E, 0x3C, 0xFF},
    /* Red    */ {0xFF, 0x00, 0x32, 0xFF},
    /* Yellow */ {0xC8, 0x78, 0x00, 0xFF},
    /* Blue   */ {0x00, 0x32, 0xFF, 0xFF},
};

_GlSph_Mng_c daGlwSph_c::mSphMng;

_GlSph_Mng_c& daGlwSph_c::getSphMng() {
    return mSphMng;
}

void daGlwSph_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daGlwSph_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return ((daGlwSph_c*)i_this)->CreateHeap();
}

int daGlwSph_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("glwSphere", 5);
    JUT_ASSERT(0xF4, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (mpModel == NULL) {
        return 0;
    }

    J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("glwSphere", 8);
    int res = mBrk.init(modelData, brk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);
    JUT_ASSERT(0x106, res == 1);

    J3DAnmTextureSRTKey* btk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("glwSphere", 11);
    res = mBtk.init(modelData, btk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);
    JUT_ASSERT(0x10F, res == 1);

    return 1;
}

int daGlwSph_c::create() {
    fopAcM_ct(this, daGlwSph_c);

    int phase_state = dComIfG_resLoad(&mPhase, "glwSphere");
    if (phase_state != cPhs_COMPLEATE_e) {
        return phase_state;
    }

    int sw = getSw();

    u8 arg0 = getArg0();
    if (arg0 == 0xFF) {
        arg0 = 0;
    }

    if ((fopAcM_isSwitch(this, sw) == TRUE && arg0 == 1) || (!fopAcM_isSwitch(this, sw) && arg0 == 0)) {
        return cPhs_ERROR_e;
    }

    if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x9E0)) {
        return cPhs_ERROR_e;
    }

    mColliderStts.Init(10, 0xFF, this);
    mSphCollider.Set(mCcDSph);
    mSphCollider.SetStts(&mColliderStts);

    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z, l_cull_box.max.x, l_cull_box.max.y, l_cull_box.max.z);
    
    mAcchCir.SetWall(25.0f, 25.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);

    mColor = getColor();
    if (mColor == COLOR_DEFAULT_e) {
        mColor = COLOR_GRAY_e;
    }

    mMoveTimer = 0;
    mIsNoMoveHome = false;
    field_0x968 = current.pos;
    mEmitterIDs[0] = fpcM_ERROR_PROCESS_ID_e;
    mEmitterIDs[1] = fpcM_ERROR_PROCESS_ID_e;

    offGetFlag();
    saveGetFlag();

    mSphMng.entry(this);
    field_0x980 = -1;

    setBaseMtx();
    actionWaitInit();

    return phase_state;
}

int daGlwSph_c::execute() {
    if (is_getted()) {
        saveGetFlag();
        return 1;
    }

    mBrk.play();
    mBtk.play();
    
    actionMain();

    mColliderStts.Move();
    mSphCollider.SetR(50.0f);
    mSphCollider.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mSphCollider);

    setBaseMtx();
    saveGetFlag();
    return 1;
}

void daGlwSph_c::actionMain() {
    static void (daGlwSph_c::*l_func[])() = {
        &daGlwSph_c::actionWait,
        &daGlwSph_c::actionGet,
        &daGlwSph_c::actionMove,
    };

    if (mSphCollider.ChkTgHit()) {
        OS_REPORT("");
        fopAc_ac_c* hit_actor = mSphCollider.GetTgHitAc();
        cXyz vec = current.pos - hit_actor->current.pos;
        mMoveVec = vec.normalizeZP();
        actionMoveInit();
    }

    if (mSphCollider.ChkCoHit()) {
        mSphCollider.OffCoSetBit();
        mSphCollider.ClrCoHit();
        actionGetInit();
    } else {
        effectSet();
        mLight.mPosition = current.pos;
        mLight.mColor.r = l_envColor[mColor].r;
        mLight.mColor.g = l_envColor[mColor].g;
        mLight.mColor.b = l_envColor[mColor].b;
        mLight.mPow = 400.0f;
        dKy_mock_light_every_set(&mLight);
    }

    if (mMoveTimer != 0) {
        mMoveTimer--;
        if (mMoveTimer == 0) {
            mIsNoMoveHome = false;
        }
    }

    if (!mIsNoMoveHome) {
        cLib_addCalcPos(&current.pos, home.pos, 0.5f, l_HIO.speed2, 0.5f);
    }

    (this->*l_func[mAction])();
}

void daGlwSph_c::effectSet() {
    field_0x974 = current.pos - field_0x968;

    for (int i = 0; i < 2; i++) {
        static u16 l_eff[] = {0x874F, 0x8750};
        mEmitterIDs[i] = dComIfGp_particle_setColor(mEmitterIDs[i], l_eff[i], &current.pos, &tevStr, &l_prmColor, &l_envColor[mColor], 0.0f, 0xFF, NULL, NULL, NULL, -1, NULL);

        JPABaseEmitter* emitterp = dComIfGp_particle_getEmitter(mEmitterIDs[i]);
        if (emitterp != NULL) {
            emitterp->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
            emitterp->setUserWork((uintptr_t)&field_0x974);
        }
    }

    field_0x968 = current.pos;
}

void daGlwSph_c::getSE() {
    static JAISoundID sSeId[] = {
        Z2SE_OBJ_STAR_GAME_HIT_1,
        Z2SE_OBJ_STAR_GAME_HIT_2,
        Z2SE_OBJ_STAR_GAME_HIT_3,
        Z2SE_OBJ_STAR_GAME_HIT_4,
        Z2SE_OBJ_STAR_GAME_HIT_5
    };

    u16 sph_seid = _GlSph_Mng_c::getSphSe();
    cXyz pos(current.pos);
    
    OS_REPORT("---------- sph seid = %d\n", sph_seid);
    Z2GetAudioMgr()->seStart(sSeId[sph_seid], &pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
}

void daGlwSph_c::getVibe() {
    dComIfGp_getVibration().StartShock(VIBMODE_S_POWER4, 1, cXyz(0.0f, 1.0f, 0.0f));
}

void daGlwSph_c::actionWaitInit() {
    mAction = ACTION_WAIT_e;
}

void daGlwSph_c::actionWait() {}

void daGlwSph_c::actionGetInit() {
    field_0x980 = 1;
    dComIfGp_particle_setColor(0x8751, &current.pos, &tevStr, &l_prmColor, &l_envColor[mColor], 0.0f, 0xFF);
    getSE();
    getVibe();

    _GlSph_Mng_c::incSphSe();
    mAction = ACTION_GET_e;
}

void daGlwSph_c::actionGet() {
    if (field_0x980 > 0) {
        field_0x980--;
        if (field_0x980 == 0) {
            for (int i = 0; i < 2; i++) {
                JPABaseEmitter* emitterp = dComIfGp_particle_getEmitter(mEmitterIDs[i]);
                if (emitterp != NULL) {
                    emitterp->deleteAllParticle();
                    dComIfGp_particle_levelEmitterOnEventMove(mEmitterIDs[i]);
                }
            }

            onGetFlag();
            field_0x980 = -1;
        }
    }
}

void daGlwSph_c::actionMoveInit() {
    mIsNoMoveHome = true;
    mMoveSpeed = l_HIO.speed;
    mAction = ACTION_MOVE_e;
}

void daGlwSph_c::actionMove() {
    f32 dist_to_target = cLib_addCalc(&mMoveSpeed, 0.0f, 0.5f, 10.0f, 1.0f);
    current.pos += mMoveVec * mMoveSpeed;

    if (dist_to_target == 0.0f) {
        mMoveTimer = 10;
        actionWaitInit();
    }
}

int daGlwSph_c::draw() {
    if (is_getted()) {
        return 1;
    }

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    J3DModelData* modelData = mpModel->getModelData();
    mBrk.entry(modelData);
    mBtk.entry(modelData);

    J3DMaterial* materialp = modelData->getMaterialNodePointer(0);
    
    static u8 l_colorKR[4] = {0x3C, 0x50, 0x50, 0x00};
    static u8 l_colorKG[4] = {0x32, 0x00, 0x23, 0x14};
    static u8 l_colorKB[4] = {0x3C, 0x23, 0x00, 0x50};

    J3DGXColor* kcolor = materialp->getTevKColor(1);
    kcolor->r = l_colorKR[mColor];
    kcolor->g = l_colorKG[mColor];
    kcolor->b = l_colorKB[mColor];

    static u8 l_colorCR[4] = {0x96, 0xFF, 0xFF, 0x00};
    static u8 l_colorCG[4] = {0x96, 0x64, 0xFF, 0x96};
    static u8 l_colorCB[4] = {0x96, 0x64, 0x00, 0xFF};

    J3DGXColorS10* ccolor = materialp->getTevColor(1);
    ccolor->r = l_colorCR[mColor];
    ccolor->g = l_colorCG[mColor];
    ccolor->b = l_colorCB[mColor];

    mDoExt_modelUpdateDL(mpModel);
    return 1;
}

int daGlwSph_c::_delete() {
    dComIfG_resDelete(&mPhase, "glwSphere");
    mSphMng.remove(this);
    return 1;
}

static int daGlwSph_Draw(daGlwSph_c* i_this) {
    return i_this->draw();
}

static int daGlwSph_Execute(daGlwSph_c* i_this) {
    return i_this->execute();
}

static int daGlwSph_Delete(daGlwSph_c* i_this) {
    return i_this->_delete();
}

static int daGlwSph_Create(fopAc_ac_c* i_this) {
    return ((daGlwSph_c*)i_this)->create();
}

void _GlSph_Mng_c::_clrLstBuf() {
    _GlSph_LstInfo_c* info = mListBuf;

    for (int i = 0; i < 120; i++) {
        info->Remove();
        info++;
    }
}

int _GlSph_Mng_c::entry(daGlwSph_c* i_sph) {
    _GlSph_LstInfo_c* info = mListBuf;

    for (int i = 0; i < 120; i++) {
        if (!info->isSet()) {
            info->Set(i_sph);
            return 1;
        }

        info++;
    }

    return 0;
}

void _GlSph_Mng_c::remove(daGlwSph_c* i_sph) {
    _GlSph_LstInfo_c* info = mListBuf;

    for (int i = 0; i < 120; i++) {
        if (i_sph == info->getpSph()) {
            info->Remove();
            break;
        }

        info++;
    }
}

int _GlSph_Mng_c::_setting_main() {
    _GlSph_LstInfo_c* info = mListBuf;

    for (int i = 0; i < 120; i++) {
        if (info->isSet()) {
            daGlwSph_c* psph = info->getpSph();
            if (psph != NULL) {
                psph->reset();
            }
        }

        info++;
    }

    return 1;
}

BOOL _GlSph_Mng_c::_chkAllGet_main() {
    _GlSph_LstInfo_c* info = mListBuf;

    for (int i = 0; i < 120; i++) {
        if (info->isSet()) {
            daGlwSph_c* psph = info->getpSph();
            if (psph != NULL && !psph->is_getted()) {
                return FALSE;
            }
        }

        info++;
    }

    return TRUE;
}

/* 80BFAFE8 0002+00 data_80BFAFE8 mSphSe__12_GlSph_Mng_c */
u16 _GlSph_Mng_c::mSphSe;

/* 80BFAFEA 0002+00 data_80BFAFEA mSeClrTmr__12_GlSph_Mng_c */
s16 _GlSph_Mng_c::mSeClrTmr;

void _GlSph_Mng_c::SphSeProc() {
    if (mSeClrTmr > 0) {
        mSeClrTmr--;
        OS_REPORT("-------- se timer = %d\n", mSeClrTmr);

        if (mSeClrTmr == 0) {
            OS_REPORT("------------ se clr ----------------\n");
            clrSphSe();
        }
    } else if (mSphSe != 0) {
        if (dComIfGp_checkPlayerStatus1(0, 0x10) || daPy_getPlayerActorClass()->checkPlayerFly()) {
            if (dComIfGp_checkPlayerStatus1(0, 0x2000000) || dComIfGp_checkPlayerStatus1(0, 0x10000)) {
                mSeClrTmr = 16;
            }
        } else {
            mSeClrTmr = 16;
        }
    } else {
        mSeClrTmr = 0;
    }
}

void _GlSph_Mng_c::clrSphSe() {
    mSphSe = 0;
}

u16 _GlSph_Mng_c::getSphSe() {
    return mSphSe;
}

void _GlSph_Mng_c::incSphSe() {
    mSphSe++;
    if (mSphSe > 4) {
        mSphSe = 4;
    }
}

static actor_method_class l_daGlwSph_Method = {
    (process_method_func)daGlwSph_Create,  (process_method_func)daGlwSph_Delete,
    (process_method_func)daGlwSph_Execute, (process_method_func)NULL,
    (process_method_func)daGlwSph_Draw,
};

extern actor_process_profile_definition g_profile_Obj_glowSphere = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_glowSphere,     // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daGlwSph_c),      // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    651,                     // mPriority
    &l_daGlwSph_Method,      // sub_method
    0x00040000,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};

AUDIO_INSTANCES
