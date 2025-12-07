/**
 * @file d_a_obj_swLight.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_swLight.h"
#include "d/d_cc_d.h"
#include "d/d_s_play.h"

#if DEBUG
class daObjSwLight_HIO_c : public mDoHIO_entry_c {
public:
    /* 811CF8C0 */ daObjSwLight_HIO_c();
    /* 811CF920 */ void genMessage(JORMContext*);

    /* 0x06 */ u8 field_0x06;
    /* 0x08 */ f32 mLightMaxRadius;
    /* 0x0C */ u8 mNoLightHit;
    /* 0x0D */ u8 mCheckRendering;
};

daObjSwLight_HIO_c::daObjSwLight_HIO_c() {
    mLightMaxRadius = 500.0f;
    mCheckRendering = 0;
    mNoLightHit = 0;
}

void daObjSwLight_HIO_c::genMessage(JORMContext* ctx) {
    // Light Candlestick Switch
    ctx->genLabel("光燭台スイッチ", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Light Maximum Radius
    ctx->genSlider("光最大半径", &mLightMaxRadius, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Check Rendering
    ctx->genCheckBox("チェック描画", &mCheckRendering, 1, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // No Light Hit
    ctx->genCheckBox("光当たり無し", &mNoLightHit, 1, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

static daObjSwLight_HIO_c l_HIO;
#endif

void daObjSwLight_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjSwLight_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

const static dCcD_SrcSph l_sph_src_at = {
    {
        {0x0, {{AT_TYPE_LANTERN_SWING, 0x0, 0x11}, {0x0, 0x0}, 0x78}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x4, 0x0},                             // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                             // mGObjTg
        {0x0},                                                          // mGObjCo
    },                                                                  // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 100.0f}  // mSph
    }  // mSphAttr
};

const static dCcD_SrcSph l_sph_src_tg = {
    {
        {0x0, {{0x0, 0x0, 0x10}, {0x202, 0x11}, 0x78}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},              // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x4, 0x0},              // mGObjTg
        {0x0},                                           // mGObjCo
    },                                                   // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 90.0f}  // mSph
    }  // mSphAttr
};

static char* l_arcName = "Obj_l8sw0";

static cull_box l_cull_box = {{
                                -500.0f,
                                0.0f,
                                -500.0f,
                            },
                            {
                                500.0f,
                                500.0f,
                                500.0f,
                            }};

static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x1e}, {0xd8fafdff, 0x11}, 0x79}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                   // mGObjAt
        {dCcD_SE_METAL, 0x2, 0x0, 0x7, 0x3},                  // mGObjTg
        {0x0},                                                // mGObjCo
    },                                                        // mObjInf
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        90.0f,               // mRadius
        170.0f               // mHeight
    }  // mCyl
};

int daObjSwLight_c::Create() {
    if (fopAcM_isSwitch(this, getSwbit())) {
        init_modeSwOffWait();
#if DEBUG
        field_0xadc = l_HIO.mLightMaxRadius;
#else
        field_0xadc = 500.0f;
#endif
    } else {
        init_modeSwOnWait();
        field_0xadc = 0.0f;
    }

    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    mStts.Init(0xff, 0xff, this);
    mAtSph.Set(l_sph_src_at);
    mAtSph.SetStts(&mStts);
    for (int i = 0; i < 2; i++) {
        mTgSph[i].Set(l_sph_src_tg);
        mTgSph[i].SetStts(&mStts);
    }

    mTgSph[0].SetTgType(2);
    mTgSph[1].SetTgType(0x200);
    mCyl.Set(l_cyl_src);
    mCyl.SetStts(&mStts);
    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z, l_cull_box.max.x,
                          l_cull_box.max.y, l_cull_box.max.z);
    field_0xae8 = 0xffff;

    JUTNameTab* name = mpModel->getModelData()->getMaterialTable().getMaterialName();
    for (u16 i = 0; i < mpModel->getModelData()->getMaterialNum(); i++) {
        if (!strcmp(name->getName(i), "cc_LB_v")) {
            field_0xae8 = i;
        }
    }

#if DEBUG
    if (field_0xae8 == 0xffff) {
        // Light Candlestick Switch: Material "cc_LB_v" is missing.
        OSReport_Error("光燭台スイッチ：マテリアル\"cc_LB_v\"がありません\n");
        return 0;
    }
#endif

    mSound.init(&current.pos, 1);
    return 1;
}

int daObjSwLight_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(385, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mpModel == NULL ? 0 : 1;
}

int daObjSwLight_c::create1st() {
    if (field_0xb20 == 0) {
        field_0xb20 = 1;
        field_0xb22 = home.angle.x;
        field_0xb24 = home.angle.z;
        home.angle.x = current.angle.x = shape_angle.x = 0;
        home.angle.z = current.angle.z = shape_angle.z = 0;
    }

    int phase = dComIfG_resLoad(&mPhase, l_arcName);

    if (phase == cPhs_COMPLEATE_e) {
        phase = MoveBGCreate(l_arcName, -1, NULL, 0xec0, NULL);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
#if DEBUG
        // Light Candlestick Switch
        l_HIO.entryHIO("光燭台スイッチ");
#endif
    }

    return phase;
}

int daObjSwLight_c::Execute(Mtx** param_0) {
    action();
    eventUpdate();
    *param_0 = &mBgMtx;
    setBaseMtx();

    if (mCyl.ChkTgHit()) {
        cCcD_Obj* hit_obj = mCyl.GetTgHitObj();
        if (hit_obj != NULL) {
            hit_obj->ChkAtType(AT_TYPE_BOOMERANG);
            u32 se_id = mCyl.GetTgHitObjHitSeID(1);
            mSound.startCollisionSE(se_id, 9, NULL);
        }
    }

    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    return 1;
}

void daObjSwLight_c::action() {
    static modeFunc l_func[] = {
        &daObjSwLight_c::modeSwOnWait,
        &daObjSwLight_c::modeSwOffWait,
        &daObjSwLight_c::modeOnLight,
        &daObjSwLight_c::modeDead,
    };

    f32 fVar2 = field_0xadc;
    (this->*l_func[field_0xae0])();
    cXyz cStack_48(current.pos.x, current.pos.y + 90.0f + KREG_F(1), current.pos.z);

    for (int i = 0; i < 2; i++) {
        mTgSph[i].SetC(cStack_48);
        dComIfG_Ccsp()->Set(&mTgSph[i]);
    }
#if DEBUG
    u8 a = 1;
    u8 light_hit = l_HIO.mNoLightHit == 0;
#endif

    mCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCyl);
    mInfluence.mPos = cStack_48;
    mInfluence.field_0xc = field_0xadc;

    if (field_0xadc != 0.0f && fVar2 == 0.0f) {
        dKy_dalkmist_inf_set(&mInfluence);
    } else if (field_0xadc == 0.0f) {
        if (fVar2 != 0.0f) {
            dKy_dalkmist_inf_cut(&mInfluence);
        }
    }

#if DEBUG
    u8 tmp = field_0xadc / l_HIO.mLightMaxRadius * 255.0f;
#else
    u8 tmp = field_0xadc / 500.0f * 255.0f;
#endif

    if (tmp != 0) {
        fopAcM_seStartLevel(this, Z2SE_OBJ_L8_L_TORCH_SW, tmp);
    }
}

void daObjSwLight_c::init_modeSwOnWait() {
    // Light Candlestick Switch: Switch <%d> was turned off.
    OS_REPORT("光燭台スイッチ：スイッチ<%d>オフしました\n", getSwbit());
    fopAcM_offSwitch(this, getSwbit());

    field_0xae1 = 0;
    field_0xae0 = 0;
}

void daObjSwLight_c::modeSwOnWait() {
    checkHit();
    u8 tmp = field_0xae6;

    if (cLib_checkBit<u8>(tmp, 1)) {
        init_modeOnLight();
    } else {
        u8 tmp2 = field_0xae6;
        if (cLib_checkBit<u8>(tmp2, 2)) {
            if (field_0xae1 <= 60) {
                field_0xae1++;
            }

            if (field_0xae1 < 5) {
                field_0xadc = 0.0f;
            } else if (field_0xae1 == 5) {
#if DEBUG
                field_0xadc = l_HIO.mLightMaxRadius * 0.25f;
#else
                field_0xadc = 500.0f * 0.25f;
#endif
            } else if (field_0xae1 < 50) {
#if DEBUG
                field_0xadc =
                    l_HIO.mLightMaxRadius * ((field_0xae1 - 5) * 0.25f / 45.0f + 0.25f);
#else
                field_0xadc = 500.0f * ((field_0xae1 - 5) * 0.25f / 45.0f + 0.25f);
#endif

            } else if (field_0xae1 == 50) {
#if DEBUG
                field_0xadc = l_HIO.mLightMaxRadius;
#else
                field_0xadc = 500.0f;
#endif
            } else if (field_0xae1 == 60) {
                init_modeSwOffWait();
            }
        } else {
            cLib_calcTimer(&field_0xae1);
        }
    }
}

void daObjSwLight_c::init_modeOnLight() {
    field_0xae0 = 2;
}

void daObjSwLight_c::modeOnLight() {
#if DEBUG
    if (cLib_chaseF(&field_0xadc, l_HIO.mLightMaxRadius, l_HIO.mLightMaxRadius * 0.05f)) {
        init_modeSwOffWait();
    }
#else
    if (cLib_chaseF(&field_0xadc, 500.0f, 500.0f * 0.05f)) {
        init_modeSwOffWait();
    }
#endif
}

void daObjSwLight_c::init_modeSwOffWait() {
    // Light Candlestick Switch: Switch <%d> was turned on.
    OS_REPORT("光燭台スイッチ：スイッチ<%d>オンしました\n", getSwbit());
    fopAcM_onSwitch(this, getSwbit());
    field_0xae3 = 0;
    field_0xae1 = 0;
    setOnTimer();
    field_0xae0 = 1;
}

void daObjSwLight_c::modeSwOffWait() {
    checkHit();

    if (getSwbit2() != 0xff) {
        if (fopAcM_isSwitch(this, getSwbit2())) {
            field_0xae3 = 1;
        }
    }

    if (field_0xae3 != 0) {
        if (!fopAcM_isSwitch(this, getSwbit2())) {
#if DEBUG
            if (cLib_chaseF(&field_0xadc, 0.0f, l_HIO.mLightMaxRadius * 0.05f)) {
#else
            if (cLib_chaseF(&field_0xadc, 0.0f, 500.0f * 0.05f)) {
#endif
                init_modeSwOnWait();
            }
        } else {
#if DEBUG
            cLib_chaseF(&field_0xadc, l_HIO.mLightMaxRadius, l_HIO.mLightMaxRadius * 0.05f);
#else
            cLib_chaseF(&field_0xadc, 500.0f, 500.0f * 0.05);
#endif
        }

    } else {
        if (!cLib_checkBit<u8>(field_0xae6, (u8)1) && !cLib_checkBit<u8>(field_0xae6, 2) &&
            !cLib_calcTimer(&field_0xae4) &&
#if DEBUG
            cLib_chaseF(&field_0xadc, 0.0f, l_HIO.mLightMaxRadius * 0.05f))
        {
#else
            cLib_chaseF(&field_0xadc, 0.0f, 500.0f * 0.05f))
        {
#endif
            init_modeSwOnWait();
        }
    }

    if (getEvent() != 0xff) {
        orderEvent(getEvent(), 0xff, 1);
    }
}

void daObjSwLight_c::modeDead() {}

int daObjSwLight_c::checkHit() {
    field_0xae7 = field_0xae6;
    field_0xae6 = 0;

    if (mTgSph[0].ChkTgHit()) {
        cCcD_Obj* hit_obj = mTgSph[0].GetTgHitObj();
        if (hit_obj != NULL && hit_obj->ChkAtType(AT_TYPE_NORMAL_SWORD) && mTgSph[0].GetTgHitGObj())
        {
            if (mTgSph[0].GetTgHitGObj()->GetAtMtrl() == dCcD_MTRL_LIGHT) {
                cLib_onBit(field_0xae6, (u8)1);
            }
        }
    }

    if (mTgSph[1].ChkTgHit()) {
        cCcD_Obj* hit_obj = mTgSph[1].GetTgHitObj();
        if (hit_obj != NULL && hit_obj->ChkAtType(AT_TYPE_LANTERN_SWING) &&
            mTgSph[1].GetTgHitGObj())
        {
            if (mTgSph[1].GetTgHitGObj()->GetAtMtrl() == dCcD_MTRL_LIGHT) {
                cLib_onBit(field_0xae6, (u8)2);
            }
        }
    }

    return 1;
}

void daObjSwLight_c::setOnTimer() {
    if (getOnTime() == 0 || getOnTime() == 255) {
        field_0xae4 = 75;
    } else {
        field_0xae4 = getOnTime() * 15;
    }
}

int daObjSwLight_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    J3DModelData* modelData = (J3DModelData*)mpModel->getModelData();
    J3DMaterial* material = modelData->getMaterialNodePointer(field_0xae8);
    J3DGXColor* color = material->getTevKColor(0);

#if DEBUG
    u8 fVar1 = field_0xadc / l_HIO.mLightMaxRadius * 255.0f;
#else
    u8 fVar1 = field_0xadc / 500.0f * 255.0f;
#endif
    color->a = 0xff;
    color->r = fVar1;
    color->g = fVar1;
    color->b = fVar1;
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

#if DEBUG
    if (l_HIO.mCheckRendering) {
        mpBgW->CalcPlane();
    }
#endif

    return 1;
}

int daObjSwLight_c::Delete() {
    mSound.deleteObject();
    dComIfG_resDelete(&mPhase, l_arcName);
#if DEBUG
    l_HIO.removeHIO();
#endif
    return 1;
}

static int daObjSwLight_create1st(daObjSwLight_c* i_this) {
    fopAcM_ct(i_this, daObjSwLight_c);
    return i_this->create1st();
}

static int daObjSwLight_MoveBGDelete(daObjSwLight_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjSwLight_MoveBGExecute(daObjSwLight_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjSwLight_MoveBGDraw(daObjSwLight_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjSwLight_METHODS = {
    (process_method_func)daObjSwLight_create1st,     (process_method_func)daObjSwLight_MoveBGDelete,
    (process_method_func)daObjSwLight_MoveBGExecute, 0,
    (process_method_func)daObjSwLight_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_SwLight = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_SwLight,        // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjSwLight_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    554,                     // mPriority
    &daObjSwLight_METHODS,   // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
