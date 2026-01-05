#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_fguard.h"

int daNpcFgd_c::createHeap() {
    int rv = NpcCreate(m_type);

    if (rv != 0) {
        if (mObjNum != 0) {
            rv = (field_0xac8 = ObjCreate(mObjNum)) != NULL;
            if (rv == 0) {
                mpMorf->stopZelAnime();
            }
        } else {
            field_0xac8 = NULL;
            rv = 1;
        }
    }

    return rv;
}

static int createHeapCallBack(fopAc_ac_c* a_this) {
    daNpcFgd_c* i_this = (daNpcFgd_c*)a_this;
    return i_this->createHeap();
}

void daNpcFgd_c::initPosAngle(Vec& i_pos, s16 param_2) {
    current.pos.set(i_pos);
    field_0xad4 = current.pos.y;
    shape_angle.y = param_2;
}

static int daNpcFgd_Create(void* a_this) {
    return static_cast<daNpcFgd_c*>(a_this)->create();
}

cPhs__Step daNpcFgd_c::create() {
    fopAcM_ct(this, daNpcFgd_c);

    mIsDarkWorld = dKy_darkworld_check();
    m_type = getType();
    mObjNum = getObjNum();

    cPhs__Step phase = (cPhs__Step)loadResrc(m_type, mObjNum);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x1910)) {
            OS_REPORT("隊列兵士アクター生成失敗しました！\n"); // Failed to generate the squad soldier actor!

            return cPhs_ERROR_e;
        }

        create_init();
    }

    return phase;
}

void daNpcFgd_c::initCollision() {
    mStts.Init(0xFF, 0xFF, this);
    mStts.SetRoomId(fopAcM_GetRoomNo(this));
    mCyl.SetStts(&mStts);
    mCyl.Set(m_cylDat);
    mCyl.SetR(Cd2_HIO_cylR(m_type));
    mCyl.SetH(Cd2_HIO_cylH(m_type));
}

void daNpcFgd_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, field_0xad4, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
    setAttention(m_type);
}

void daNpcFgd_c::create_init() {
    gravity = -3.0f;
    maxFallSpeed = -21.0f;
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -50.0f, -10.0f, -90.0f, 50.0f, 290.0f, 60.0f);

    f32 chkWallR = Cd2_HIO_chkWallR(m_type);
    f32 chkWallH = Cd2_HIO_chkWallH(m_type);
    mAcchCir.SetWall(chkWallH, chkWallR);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
              fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    mAcch.SetRoofNone();
    mAcch.SetWaterNone();
    mAcch.CrrPos(dComIfG_Bgsp());

    setEnvTevCol();
    setRoomNo();

    f32 groundH = mAcch.GetGroundH();
    current.pos.y = groundH;
    field_0xad4 = groundH;

    initCollision();

    mFlag = getFirstFlag();

    if (mFlag != 0) {
        setAnime(2);
    } else {
        setAnime(0);
    }

    mpMorf->setMorf(0.0f);
    setBaseMtx();
}

static int daNpcFgd_Delete(void* a_this) {
    static_cast<daNpcFgd_c*>(a_this)->~daNpcFgd_c();
    return 1;
}

daNpcFgd_c::~daNpcFgd_c() {
    removeResrc(m_type, mObjNum);

    if (heap != NULL) {
        mpMorf->stopZelAnime();
    }
}

void daNpcFgd_c::checkGroundHeight() {
    speed.y += gravity;

    if (speed.y < maxFallSpeed) {
        speed.y = maxFallSpeed;
    }

    current.pos.y += speed.y;
    mAcch.CrrPos(dComIfG_Bgsp());

    s16 sVar1 = 0;
    cM3dGPla plane;
    bool bVar1 = dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &plane);
    if (bVar1) {
        sVar1 = fopAcM_getPolygonAngle(&plane, shape_angle.y);
    }
    
    if (sVar1) {
        field_0xad4 = current.pos.y;
    }

    cLib_addCalc2(&field_0xad4, current.pos.y, 0.4f, 25.0f);
}

void daNpcFgd_c::setCollision() {
    #if DEBUG
    mCyl.SetR(Cd2_HIO_cylR(m_type));
    mCyl.SetH(Cd2_HIO_cylH(m_type));
    #endif
    
    cXyz sp18(current.pos.x, field_0xad4, current.pos.z);
    mCyl.SetC(sp18);
    dComIfG_Ccsp()->Set(&mCyl);
}

int daNpcFgd_c::execute() {
    mAcchCir.SetWallH(Cd2_HIO_chkWallH(m_type));
    mAcchCir.SetWallR(Cd2_HIO_chkWallR(m_type));

    if (mFlag) {
        fopAcM_OffStatus(this, 0x8000000);
    } else {
        fopAcM_OnStatus(this, 0x8000000);
        return 1;
    }

    checkGroundHeight();
    animation(-1);
    setCollision();
    setBaseMtx();

    return 1;
}

static int daNpcFgd_Execute(void* a_this) {
    return static_cast<daNpcFgd_c*>(a_this)->execute();
}

int daNpcFgd_c::draw() {
    if (mFlag == 0) {
        return 1;
    }

    drawNpc();
    drawObj(mObjNum, field_0xac8, Cd2_HIO_objScale(m_type));
    drawShadow(40.0f);

    return 1;
}

static int daNpcFgd_Draw(void* a_this) {
    return static_cast<daNpcFgd_c*>(a_this)->draw();
}

static int daNpcFgd_IsDelete(void* pa_this) {
    return 1;
}

static actor_method_class daNpcFgd_METHODS = {
    (process_method_func)daNpcFgd_Create,
    (process_method_func)daNpcFgd_Delete,
    (process_method_func)daNpcFgd_Execute,
    (process_method_func)daNpcFgd_IsDelete,
    (process_method_func)daNpcFgd_Draw,
};

actor_process_profile_definition g_profile_NPC_FGUARD = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_NPC_FGUARD,            // mProcName
    &g_fpcLf_Method.base,       // sub_method
    sizeof(daNpcFgd_c),         // mSize
    0,                          // mSizeOther
    0,                          // mParameters
    &g_fopAc_Method.base,       // sub_method
    409,                        // mPriority
    &daNpcFgd_METHODS,          // sub_method
    0x00040107,                 // mStatus
    fopAc_NPC_e,                // mActorType
    fopAc_CULLBOX_CUSTOM_e      // cullType
};
