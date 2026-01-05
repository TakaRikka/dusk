/**
 * @file d_a_obj_cblock.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_obj_cblock.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"
#include "d/d_path.h"
#include "d/d_s_play.h"

#if DEBUG
#define TRAVEL_TIME (l_HIO.mTravelTime)
#define TRAVEL_DIST (l_HIO.mTravelDist)
#else
#define TRAVEL_TIME (13)
#define TRAVEL_DIST (50.0f)
#endif

class daObjCBlk_HIO_c : public mDoHIO_entry_c {
public:
    daObjCBlk_HIO_c();
    virtual ~daObjCBlk_HIO_c() {}

#if DEBUG
    void genMessage(JORMContext*);
#endif

    /* 0x06 */ u8 field_0x06;
    /* 0x07 */ u8 field_0x07;
    /* 0x08 */ u8 mWaitTime;
    /* 0x09 */ u8 mTravelTime;
    /* 0x0C */ f32 mGravity;
    /* 0x10 */ f32 mTravelDist;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1c;
};

static cXyz l_chainOffset(0.0f, 105.0f, 30.0f);

static cXyz l_smokeSetOffset(0.0f, 0.0f, -200.0f);

static cull_box const l_cull_box = {
    {-250.0f, 0.0f, -450.0f},
    {250.0f, 250.0f, 400.0f},
};

static char* l_arcName = "P_Cblock";

static f32 l_side_co_offset[12] = {
    -255.0f, 0.0f, -30.0f, -135.0f,
    0.0f, -30.0f, 135, 0.0f,
    -30.0f, 255.0f, 0.0f, -30.0f,
};

static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x19}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            60.0f, // mRadius
            300.0f // mHeight
        } // mCyl
    }
};

#if DEBUG
daObjCBlk_HIO_c::daObjCBlk_HIO_c() {
    field_0x06 = 0;
    field_0x07 = 0;
    mGravity = -6.0f;
    mTravelDist = 50.0f;
    mTravelTime = 13;
    mWaitTime = 8;
    field_0x14 = 35.0f;
    field_0x18 = 35.0f;
    field_0x1c = 15.0f;
}

static f32 dummy1() {
    return 15.0f;
}

void daObjCBlk_HIO_c::genMessage(JORMContext* ctx) {
    // "Chained block"
    ctx->genLabel("チェーン付きブロック", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("重力", &mGravity, -10.0f, -0.1f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("移動時間", &mTravelTime, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("タメ時間", &mWaitTime, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("移動距離", &mTravelDist, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

static daObjCBlk_HIO_c l_HIO;
#endif

void daObjCBlk_c::initBaseMtx() {
    model1->setBaseScale(scale);
    setBaseMtx();
}

void daObjCBlk_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y,current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    model1->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjCBlk_c::Create() {
    u8 pathId = getPathID();
    roomPath = dPath_GetRoomPath(pathId, fopAcM_GetHomeRoomNo(this));
    swNo = getSwNo();
    arg0 = getArg0();
    if (pathId != 0xff) {
        u8 pnt_index;
        if (fopAcM_isSwitch(this, swNo)) {
            pnt_index = 1;
        } else {
            pnt_index = 0;
        }
        dPnt* pnt = dPath_GetPnt(roomPath, pnt_index);
        JUT_ASSERT(284, pnt != NULL)
        current.pos = pnt->m_position;
    }
    initBaseMtx();
    fopAcM_SetMtx(this, model1->getBaseTRMtx());
    stts.Init(0xff, 0xff, this);
    for (int i = 0; i < 4; i++) {
        cyls[i].Set(l_cyl_src);
        cyls[i].SetStts(&stts);
    }
    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z, l_cull_box.max.x,
                          l_cull_box.max.y, l_cull_box.max.z);
    cXyz acStack_40(0.0f, 0.0f, 50.0f);
    mDoMtx_stack_c::YrotS(current.angle.y);
    mDoMtx_stack_c::multVec(&acStack_40, &acStack_40);
    chain_s* pChain = chains;
    cXyz* pos = field_0xc24;
    getChainBasePos(&pChain->position);
    cXyz cStack_4c(pChain->position);
    for (int i = 0; i < 7; i++) {
        *pos = cStack_4c + acStack_40;
        cStack_4c = *pos;
        pos++;
    }
    initWait();
    return 1;
}

int daObjCBlk_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(350, modelData != NULL);
    model1 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (model1 == NULL) {
        return 0;
    }
    chains = new chain_s[6];
    if (chains == NULL) {
        return 0;
    }
    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 5);
    JUT_ASSERT(369, modelData != NULL);
    chain_s* pChain = chains;
    for (int i = 0; i < 6; i++) {
        pChain->model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (pChain->model == NULL) {
            return 0;
        }
        pChain++;
    }
    modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 6);
    JUT_ASSERT(386, modelData != NULL);
    model2 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (model2 == NULL) {
        return 0;
    } 
    return 1;
}

daObjCBlk_c::chain_s::~chain_s() {
}

daObjCBlk_c::chain_s::chain_s() {
}

int daObjCBlk_c::create() {
    fopAcM_ct(this, daObjCBlk_c);
    int rv = dComIfG_resLoad(&phase, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        rv = MoveBGCreate(l_arcName, 9, dBgS_MoveBGProc_TypicalRotY, 0x1000, NULL);
        if (rv == cPhs_ERROR_e) {
            return rv;
        }
        OS_REPORT("CHAIN BLOCK:%x\n", fopAcM_GetParam(this));

#if DEBUG
        l_HIO.entryHIO("チェーン付きブロック");
#endif
    }
    return rv;
}

int daObjCBlk_c::Execute(f32 (**mtx)[3][4]) {
    *mtx = &mBgMtx;
    return 1;
}

void daObjCBlk_c::block_mode_proc_call() {
    static modeFunc l_func[2] = {
        &daObjCBlk_c::modeWait,
        &daObjCBlk_c::modeWalk,
    };
    (this->*(l_func[field_0xc1e]))();
}

#if DEBUG
static void dummy5() {
    OS_REPORT("Delete -> ChainBlock(id=%d)\n");
    OS_REPORT("Create -> ChainBlock(id=%d)\n");
}
#endif

void daObjCBlk_c::initWait() {
    field_0xc1f = 0;
}

void daObjCBlk_c::modeWait() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    bool bVar1 = false;
    cXyz cStack_2c;
    cXyz cStack_38;
    cXyz cStack_44(0.0f, 0.0f, 1.0f);
    getChainBasePos(&cStack_2c);
    cStack_38 = cStack_2c - chains[5].position;
    mDoMtx_stack_c::YrotS(shape_angle.y);
    mDoMtx_stack_c::multVec(&cStack_44, &cStack_44);
    cStack_38.normalizeZP();
    cStack_44.normalizeZP();
    f32 inprod = cStack_38.inprod(cStack_44);
    if (std::fabs(inprod) > 0.95000005f + KREG_F(4)) {
        bVar1 = true;
    }
    if (field_0xc1c != 0 && field_0xc1e != 0) {
        setPower(power);
        if (bVar1) {
            field_0xca3++;
            if (!checkWall() && player->getBaseAnimeFrame() > 6.0f && player->getBaseAnimeFrame() < 8.0f)
            {
#if DEBUG
                if (field_0xca3 > l_HIO.mWaitTime) {
#else
                if (field_0xca3 > 8) {
#endif
                    initWalk();
                }
            }
        }
    } else {
        field_0xca3 = 0;
    }
}

void daObjCBlk_c::initWalk() {
    u8 travelTime = TRAVEL_TIME;
    walkTimer = travelTime;
    field_0xca6 = 0x7fff / travelTime;
    field_0xc1f = 1;
}

void daObjCBlk_c::modeWalk() {
    u8 travelTime = TRAVEL_TIME;
    f32 travelDist = TRAVEL_DIST;

    cLib_calcTimer(&walkTimer);
    f32 dVar5 = (1.0f - cM_scos(field_0xca6 * (travelTime - walkTimer))) * 0.5f;
    cXyz local_58(0.0f, 0.0f, 1.0f);
    mDoMtx_stack_c::YrotS(home.angle.y);
    mDoMtx_stack_c::multVec(&local_58, &local_58);
    current.pos.x = home.pos.x + local_58.x * (travelDist * (field_0xca2 + dVar5));
    current.pos.z = home.pos.z + local_58.z * (travelDist * (field_0xca2 + dVar5));
    setPower(power);
    fopAcM_seStartLevel(this, Z2SE_OBJ_CBLOCK_SLIDEMOVE, 0);
    if (walkTimer == 0) {
        initWait();
        field_0xca2++;
    }
}

void daObjCBlk_c::getChainBasePos(cXyz* pPos) {
    *pPos = l_chainOffset;
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(pPos, pPos);
}

void daObjCBlk_c::setPower(f32 power) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    chain_s* pChain = chains + 5;
    cXyz local_28;
    local_28 = pChain[-1].position - pChain[0].position;
    s16 angle = cM_atan2s(local_28.x, local_28.z);
    player->setOutPower(power, angle, 0);
}

static f32 dummy2() {
    f32 f0 = -6.0f;
#if DEBUG
    f32 f1 = 3.0f / 20.0f;
#else
    f32 f1 = 3.0f / 40.0f;
#endif
    f32 f2 = 100.0f;
    f32 f3 = 15.0f;
    f32 f4 = 35.0f;
#if DEBUG
    f32 f5 = 5.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;
#else
    f64 f5 = 0.5;
    f64 f6 = 3.0;
    f64 f7 = 0.0;
    f32 f8 = 195.0f;
    f32 f9 = 210.0f;
#endif
    return f0 + f1 * (f2 - f3 * (f4 - f5 * (f6 - f7 * f8 - (f7 - f9))));
}

BOOL daObjCBlk_c::checkWall() {
    cXyz acStack_30[2] = {cXyz(-95.0f, 5.0f, 1.0f), cXyz(95.0f, 5.0f, 1.0f)};
    cXyz cStack_40(0.0f, 0.0f, 1.0f);
    cStack_40.z = TRAVEL_DIST;
    BOOL rv = FALSE;
    for (int i = 0; i < 2; i++) {
        cXyz cStack_4c(acStack_30[i]);
        cXyz cStack_58(cStack_40);
        mDoMtx_stack_c::YrotS(current.angle.y);
        mDoMtx_stack_c::multVec(&cStack_4c, &cStack_4c);
        cStack_4c += current.pos;
        mDoMtx_stack_c::multVec(&cStack_58, &cStack_58);
        cStack_58 += cStack_4c;
#if DEBUG
        field_0xcb0_debug[i] = cStack_4c;
        field_0xcc8_debug[i] = cStack_58;
#endif
        if (fopAcM_lc_c::lineCheck(&cStack_4c, &cStack_58, this)) {
            rv = TRUE;
        }
    }
    return rv;
}

int daObjCBlk_c::Draw() {
    return 1;
}

int daObjCBlk_c::Delete() {
    dComIfG_resDelete(&phase, l_arcName);
#if DEBUG
    l_HIO.removeHIO();
#endif
    return 1;
}

static int daObjCBlk_Draw(daObjCBlk_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daObjCBlk_Execute(daObjCBlk_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjCBlk_Delete(daObjCBlk_c* i_this) {
    fpc_ProcID unusedId = fopAcM_GetID(i_this);
    return i_this->MoveBGDelete();
}

static int daObjCBlk_Create(fopAc_ac_c* i_this) {
    daObjCBlk_c* cblock = static_cast<daObjCBlk_c*>(i_this);
    fpc_ProcID unusedId = fopAcM_GetID(i_this);
    return cblock->create();
}

#if DEBUG
static const u32 dummy3[3] = {
    0xff0000ff,
    0x0000ffff,
    0x00ff00ff,
};
#endif

AUDIO_INSTANCES;

static actor_method_class l_daObjCBlk_Method = {
    (process_method_func)daObjCBlk_Create,
    (process_method_func)daObjCBlk_Delete,
    (process_method_func)daObjCBlk_Execute,
    NULL,
    (process_method_func)daObjCBlk_Draw,
};

actor_process_profile_definition g_profile_Obj_ChainBlock = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_ChainBlock,    // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjCBlk_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  597,                    // mPriority
  &l_daObjCBlk_Method,    // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
