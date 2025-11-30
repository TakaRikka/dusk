/**
 * @file d_a_obj_kznkarm.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_player.h"
#include "d/actor/d_a_obj_kznkarm.h"
#include "d/actor/d_a_obj_kazeneko.h"
#include "d/d_a_obj.h"

#if DEBUG

class daObjKznkarm_Hio_c : public JORReflexible {
public:
    daObjKznkarm_Hio_c();
    void genMessage(JORMContext* ctx);
    void default_set();
    void ct();
    void dt();

    /* 0x04 */ int mCount; 
    /* 0x04 */ daObjKznkarm_Attr_c mAttr; 
};

static daObjKznkarm_Hio_c M_hio;

daObjKznkarm_Hio_c::daObjKznkarm_Hio_c() {
    mCount = 0;
    default_set();
}

void daObjKznkarm_Hio_c::default_set() {
    mAttr = daObjKznkarm_c::M_attr;
}

void daObjKznkarm_Hio_c::ct() {
    if (mCount++ == 0) {
        daObj::HioVarious_c::init(this, "風見ネコの風受け");
    }
}

void daObjKznkarm_Hio_c::dt() {
    if (--mCount == 0) {
        daObj::HioVarious_c::clean(this);
    }
}

void daObjKznkarm_Hio_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("§ 風見ネコの風受けパラメータ設定  §\n", 0, 0, NULL, 0xffff, 0xffff, 0x200,
                            0x18);
    ctx->genSlider("重力", &mAttr.gravity, -50.0f, 0.0f, 0,
                             NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("最高落下速度", &mAttr.maximum_falling_speed, -200.0f, 0.0f, 0,
                             NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("水抵抗", &mAttr.water_drag, 0.0f, 1.0f, 0,
                             NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("跳ね返り係数", &mAttr.bounce_coeff, 0.0f, 1.0f, 0,
                             NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("回転角速度X", &mAttr.angular_speed.x, -0x4000,
                             0x3fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("回転角速度Y", &mAttr.angular_speed.y, -0x4000,
                             0x3fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("回転角速度Z", &mAttr.angular_speed.z, -0x4000,
                             0x3fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("置き位置X", &mAttr.position.x, -200.0f,
                             200.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("置き位置Y", &mAttr.position.y, -200.0f,
                             200.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("置き位置Z", &mAttr.position.z, -200.0f,
                             200.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("初期投げ力", &mAttr.initial_throwing_force, 0.0f, 300.0f,
                             0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("初期投げ上げ力", &mAttr.initial_throwing_upward_force, 0.0f, 300.0f,
                             0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("投げ回転角速度", &mAttr.throw_angular_speed, -0x8000,
                             0x7fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

#endif

 daObjKznkarm_Attr_c const daObjKznkarm_c::M_attr = {
    -5.0f, -30.0f, 0.6f, 0.94f,
     40.0f, 40.0f, -60.0f,
    -6.0f, -6.0f, -60.0f, 0.0f, 
    12.0f, 0, 0x1000, 0, 0xE000,
};

static char* l_arcName = "J_Kazami";

daObjKznkarm_c::actionFunc daObjKznkarm_c::ActionTable[4][2] = {
    &daObjKznkarm_c::initBroken,
    &daObjKznkarm_c::executeBroken,
    &daObjKznkarm_c::initCarry,
    &daObjKznkarm_c::executeCarry,
    &daObjKznkarm_c::initThrow,
    &daObjKznkarm_c::executeThrow,
    &daObjKznkarm_c::initStay,
    &daObjKznkarm_c::executeStay,
};

void daObjKznkarm_c::setAction(daObjKznkarm_c::Mode_e i_action) {
    JUT_ASSERT(209, i_action < MODE_MAX_e);
    mAction = ActionTable[i_action];
    callInit();
}

void daObjKznkarm_c::callInit() {
    JUT_ASSERT(225, mAction != NULL);
    (this->*mAction[0])();
}

void daObjKznkarm_c::callExecute() {
    JUT_ASSERT(238, mAction != NULL);
    (this->*mAction[1])();
}

inline daObjKznkarm_Attr_c* daObjKznkarm_c::attr() const {
    #if DEBUG
    return &M_hio.mAttr;
    #else
    return (daObjKznkarm_Attr_c*)&M_attr; 
    #endif
}

void daObjKznkarm_c::initBroken() {
    mMode = MODE_BROKEN_e;
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x80);
    daObjKazeNeko_c* kazeNeko = (daObjKazeNeko_c*)fpcM_SearchByID(parentActorID);
    if (kazeNeko != NULL) {
        kazeNeko->getFirstVec(&speed, fopAcM_GetParam(this));
    }
    field_0x7a8 = attr()->angular_speed.x;
    field_0x7aa = attr()->angular_speed.y;
    field_0x7ac = attr()->angular_speed.z;
    field_0x7b0 = 10;
}

void daObjKznkarm_c::executeBroken() {
    speed.y += gravity;
    if (speed.y < maxFallSpeed) {
        speed.y = maxFallSpeed;
    }

    current.pos += speed;
    if (field_0x7b0 != 0) {
        field_0x7b0--;
    }

    if (field_0x7b0 == 0) {
        f32 gndH = mAcch.GetGroundH();
        if (current.pos.y < gndH) {
            speed.y *= -1.0f;
            speed *= attr()->bounce_coeff;
            current.pos.y = gndH;
        }
        
        if (speed.abs() < fabsf(gravity)) {
            speed.zero();
            field_0x7a8 = 0;
            field_0x7aa = 0;
            field_0x7ac = 0;
            shape_angle.x = 0x4000;
            shape_angle.z = 0;
            current.pos.y = gndH;
            setAction(MODE_STAY_e);
        }
    }

    shape_angle.x = shape_angle.x + field_0x7a8;
    shape_angle.y = shape_angle.y + field_0x7aa;
    shape_angle.z = shape_angle.z + field_0x7ac;
    setBaseMtx();
    mAcch.CrrPos(dComIfG_Bgsp());
}

void daObjKznkarm_c::initCarry() {
    mMode = MODE_CARRY_e;
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x80);
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    field_0x7ae = 0;
    field_0x7af = 0;
}

void daObjKznkarm_c::executeCarry() {
    eyePos = attention_info.position = current.pos;
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (player->getGrabUpStart()) {
        field_0x7af = 1;
    }

    if (!fopAcM_checkCarryNow(this)) {
        fopAcM_carryOffRevise(this);
        setAction(MODE_THROW_e);
    }

    mDoMtx_stack_c::transS(current.pos);
    if (field_0x7af != 0) {
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::transM(attr()->field_0x18.x, attr()->field_0x18.y, attr()->field_0x18.z);
    } else {
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::ZrotM(shape_angle.z);
        mDoMtx_stack_c::XrotM(shape_angle.x);
        mDoMtx_stack_c::transM(attr()->position.x, attr()->position.y, attr()->position.z);
    }
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daObjKznkarm_c::initThrow() {
    mMode = MODE_THROW_e;
    fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x80);
    speed.zero();
    if (speedF > 1.0f) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        f32 dVar9 = player->speedF + attr()->initial_throwing_force;
        speed.set((dVar9 * cM_ssin(player->shape_angle.y)), attr()->initial_throwing_upward_force,
                    dVar9 * cM_scos(player->shape_angle.y));
    }

    field_0x7ac = attr()->throw_angular_speed;
}

void daObjKznkarm_c::executeThrow() {
    if (field_0x7ae != 0) {
        speed.y += gravity * 0.2f;
        if (speed.y < maxFallSpeed * 0.2f) {
            speed.y = maxFallSpeed * 0.2f;
        }
    } else {
        speed.y += gravity;
        if (speed.y < maxFallSpeed) {
            speed.y = maxFallSpeed;
        }
    }
    
    f32 savedSpeedY = speed.y;
    fopAcM_posMove(this, 0);
    mAcch.CrrPos(dComIfG_Bgsp());
    speed.y = savedSpeedY;
    
    if (mAcch.ChkGroundLanding()) {
        f32 speedAbs = speed.abs();
        cM3dGPla groundPla;
        bool hit = dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &groundPla);
        cXyz* normal = groundPla.GetNP();
        if (normal != NULL) {
            VECReflect(&speed, normal, &speed);
            speed *= speedAbs * attr()->bounce_coeff;
        } else {
            speed.y *= -attr()->bounce_coeff;
        }
        current.pos.y = mAcch.GetGroundH();
    } else if (mAcch.ChkGroundHit()) {
        speed.zero();
        setAction(MODE_STAY_e);
    }
    
    if (mAcch.ChkWallHit()) {
        f32 speedAbs = speed.abs();
        cM3dGPla wallPla;
        bool hit = dComIfG_Bgsp().GetTriPla(mAcchCir, &wallPla);
        cXyz* normal = wallPla.GetNP();
        if (normal != NULL) {
            VECReflect(&speed, normal, &speed);
            speed *= speedAbs * attr()->bounce_coeff;
        } else {
            speed.x *= -attr()->bounce_coeff;
            speed.z *= -attr()->bounce_coeff;
        }
        field_0x7ac *= -1;
    }
    
    if (field_0x7ae == 0) {
        if (mAcch.ChkWaterIn()) {
            cXyz effectPos = current.pos;
            effectPos.y = mAcch.m_wtr.GetHeight();
            for (int i = 0; i < 4; i++) {
                static u16 w_eff_id[4] = {
                    ID_ZI_J_DOWNWTRA_A, ID_ZI_J_DOWNWTRA_B, ID_ZI_J_DOWNWTRA_C, ID_ZI_J_DOWNWTRA_D,
                };

                field_0x798[i] = dComIfGp_particle_set(field_0x798[i], w_eff_id[i], &effectPos, &tevStr);
            }
            Z2GetAudioMgr()->seStart(Z2SE_CM_BODYFALL_WATER_S, &effectPos, 0, false, 1.0f, 1.0f, -1.0f, -1.0f, false);
            field_0x7ae = 1;
        }
    }
    
    if (field_0x7ae != 0) {
        speed.x *= attr()->water_drag;
        speed.z *= attr()->water_drag;
    }
    
    f32 fVar1 = (speed.absXZ() / attr()->initial_throwing_force);
    int angleOffset = (f32)field_0x7ac * fVar1;
    shape_angle.z += angleOffset;
    setBaseMtx();
}

void daObjKznkarm_c::initStay() {
    mMode = MODE_STAY_e;
    fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x80);
    cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    speedF = 0.0f;
    field_0x7a8 = getGroundSlope(shape_angle.y) - 0x4000;
    shape_angle.z = (s16)getGroundSlope(shape_angle.y - 0x4000);
}

void daObjKznkarm_c::executeStay() {
    if (shape_angle.x != field_0x7a8) {
        eyePos = attention_info.position = current.pos;
        cLib_chaseAngleS(&shape_angle.x, field_0x7a8, 0xc00);
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::ZrotM(shape_angle.z);
        mDoMtx_stack_c::XrotM(shape_angle.x);
        mDoMtx_stack_c::transM(attr()->position.x, attr()->position.y, attr()->position.z);
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
    if (fopAcM_checkCarryNow(this)) {
        setAction(MODE_CARRY_e);
    }
}

void daObjKznkarm_c::create_init() {
    fopAcM_setCullSizeBox(this, -10.0f, -15.0f, -15.0f, 120.0f, 40.0f, 15.0f);
    fopAcM_OnCarryType(this, fopAcM_CARRY_UNK_30);
    attention_info.distances[fopAc_attn_CARRY_e] = 0xd;
    gravity = attr()->gravity;
    maxFallSpeed = attr()->maximum_falling_speed;
    setAction(MODE_BROKEN_e);
    initBaseMtx();
    mAcchCir.SetWall(0.0f, 50.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    mAcch.SetRoofNone();
    mAcch.OffClrSpeedY();

    #if DEBUG
    M_hio.ct();
    #endif
}

void daObjKznkarm_c::initBaseMtx() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    setBaseMtx();
}

void daObjKznkarm_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::transM(-60.0f, 0.0f, 0.0f);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

s16 daObjKznkarm_c::getGroundSlope(s16 param_0) {
    cM3dGPla groundPla;
    bool hit = dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &groundPla);
    if (hit) {
        return fopAcM_getPolygonAngle(&groundPla, param_0);
    }
    return shape_angle.z;
}

inline int daObjKznkarm_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "arm.bmd");
    JUT_ASSERT(593, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return FALSE;
    }
    return TRUE;
}

static int createSolidHeap(fopAc_ac_c* a_this) {
    daObjKznkarm_c* i_this = ((daObjKznkarm_c*)a_this);
    return i_this->createHeap();
}

inline int daObjKznkarm_c::draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    
    cXyz shadowPos = cXyz(current.pos.x, current.pos.y + 50.0f, current.pos.z);
    mShadowId = dComIfGd_setShadow(mShadowId, 1, mpModel, &shadowPos, 600.0f, 0.0f,
                                   current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static int daObjKznkarm_Draw(daObjKznkarm_c* i_this) {
    return i_this->draw();
}

inline int daObjKznkarm_c::execute() {
    #if DEBUG
    gravity = attr()->gravity;
    maxFallSpeed = attr()->maximum_falling_speed;
    #endif

    callExecute();
    return 1;
}

static int daObjKznkarm_Execute(daObjKznkarm_c* i_this) {
    return i_this->execute();
}

static int daObjKznkarm_IsDelete(daObjKznkarm_c* i_this) {
    return 1;
}

inline daObjKznkarm_c::~daObjKznkarm_c() {
    #if DEBUG
    M_hio.dt();
    #endif
    dComIfG_resDelete(&mPhase, l_arcName);
}

static int daObjKznkarm_Delete(daObjKznkarm_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "KznkArm");
    i_this->~daObjKznkarm_c();
    return 1;
}

inline int daObjKznkarm_c::create() {
    fopAcM_ct(this, daObjKznkarm_c);
    int rv = dComIfG_resLoad(&mPhase, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createSolidHeap, 0x4000)) {
            return cPhs_ERROR_e;
        }
        create_init();
    }
    return rv;
}

static int daObjKznkarm_Create(fopAc_ac_c* i_this) {
    fopAcM_RegisterCreateID(daObjKznkarm_c, i_this, "KznkArm");
    return a_this->create();
}

static actor_method_class l_daObjKznkarm_Method = {
    (process_method_func)daObjKznkarm_Create,
    (process_method_func)daObjKznkarm_Delete,
    (process_method_func)daObjKznkarm_Execute,
    (process_method_func)daObjKznkarm_IsDelete,
    (process_method_func)daObjKznkarm_Draw,
};

extern actor_process_profile_definition g_profile_Obj_KznkArm = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_Obj_KznkArm,        // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daObjKznkarm_c),  // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  31,                      // mPriority
  &l_daObjKznkarm_Method,  // sub_method
  0x00044100,              // mStatus
  fopAc_ACTOR_e,           // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
