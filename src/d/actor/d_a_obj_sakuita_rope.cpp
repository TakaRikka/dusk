/**
 * @file d_a_obj_sakuita_rope.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_sakuita_rope.h"
#include "d/d_com_inf_game.h"
#include "d/d_path.h"

#if DEBUG
#include "d/d_a_obj.h"

class daObjItaRope_Hio_c : public JORReflexible {
public:
    daObjItaRope_Hio_c();
    void genMessage(JORMContext*);
    void ct();
    void dt();

    void default_set() {
        mAttr = daObjItaRope_c::M_attr;
    }

    /* 0x4 */ int field_0x4;
    /* 0x8 */ daObjItaRope_Attr_c mAttr;
};

daObjItaRope_Hio_c::daObjItaRope_Hio_c() {
    field_0x4 = 0;
    default_set();
}

void daObjItaRope_Hio_c::ct() {
    if (field_0x4++ == 0) {
        daObj::HioVarious_c::init(this, "牧場のカラカラ板用紐");
    }
}

void daObjItaRope_Hio_c::dt() {
    if (--field_0x4 == 0) {
        daObj::HioVarious_c::clean(this);
    }
}

void daObjItaRope_Hio_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("§ 牧場のカラカラ板用紐パラメータ設定  §\n", 0, 0, NULL, 0xffff, 0xffff, 0x200,
                  0x18);
    ctx->genSlider("重力", &mAttr.mGravity, -200.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("紐の重さ", &mAttr.mStringWeight, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("windRate", &mAttr.mWindRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("speedRate", &mAttr.mSpeedRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("offsetRate", &mAttr.mOffsetRate, -5.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

#endif

int Sakuita_c::setModelData(J3DModelData* i_modelData) {
    mModel = mDoExt_J3DModel__create(i_modelData, 0x20000, 0x11000084);
    JUT_ASSERT(140, mModel != NULL);
    return 1;
}

daObjItaRope_Attr_c const daObjItaRope_c::M_attr = {
    -5.0f, 0.2f, 0.5f, 0.8f, 0,
};

void Sakuita_c::create(cXyz* param_1, s16 param_2, s8 param_3) {
    field_0x04 = param_1;
    field_0x36 = param_2;
    field_0x3a = param_3;
    field_0x08.set(*param_1);
    field_0x08.y -= 75.0f;
    field_0x14 = field_0x08;
    field_0x20.zero();
    calcAngle();
    initBaseMtx();
}

void Sakuita_c::execute(cXyz* param_1, s16 param_2) {
    field_0x36 = param_2;
    setPlatePos(param_1);
    calcAngle();
    setBaseMtx();
}

void Sakuita_c::draw() {
    dComIfGp_entrySimpleModel(mModel, field_0x3a);
}

void Sakuita_c::initBaseMtx() {
    setBaseMtx();
}

void Sakuita_c::setBaseMtx() {
    mDoMtx_stack_c::transS(field_0x04->x, field_0x04->y - 6.0f, field_0x04->z);
    mDoMtx_stack_c::ZrotM(mRotation.z);
    mDoMtx_stack_c::XrotM(mRotation.x);
    mDoMtx_stack_c::YrotM(mRotation.y);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void Sakuita_c::setPlatePos(cXyz* param_1) {
    cXyz acStack_34(*param_1);
    f32 dVar5 = param_1->abs();
    if (cM_rnd() < 0.1f) {
        acStack_34 *= -60.0f;
    } else {
        acStack_34.zero();
    }
    cXyz cStack_40 = *field_0x04 - field_0x08;
    cStack_40.y += 15.0f;
    field_0x38 += 0x1400;
    if (cLib_chaseF(&field_0x2c, 0.0f, 100.0f) != 0) {
        field_0x38 = 0;
    }
    cStack_40 += field_0x20;
    if (cM_rnd() < 0.4f) {
        cStack_40 += acStack_34; 
        if (field_0x2c < 8192.0f) {
            field_0x2c += dVar5 * 900.0f;
        }
    }
    cStack_40.normalizeZP();
    field_0x08 = *field_0x04 + cStack_40 * -75.0f;
    field_0x20 = (field_0x20 + (field_0x14 - field_0x08)) * 0.5f;
    field_0x14 = field_0x08;
}

void Sakuita_c::calcAngle() {
    cXyz local_28 = *field_0x04 - field_0x08;
    mRotation.x = cM_atan2s(local_28.z, local_28.y);
    mRotation.y = field_0x36 + field_0x2c * cM_scos(field_0x38);
    mRotation.z = -cM_atan2s(local_28.x, local_28.y);
}

static char* l_arcName = "A_Sakuita";

#if DEBUG
daObjItaRope_Hio_c M_hio;
#endif

inline daObjItaRope_c::~daObjItaRope_c() {
#if DEBUG
    M_hio.dt();
#endif
    delete [] mRopes;
    delete [] mSakuitas;

    if (field_0x636 != 0) {
        J3DModelData* modelData = (J3DModelData*) dComIfG_getObjectRes(l_arcName, "A_Sakuita.bmd");
        JUT_ASSERT(320, modelData != NULL);
        dComIfGp_removeSimpleModel(modelData, fopAcM_GetRoomNo(this));
    }
    dComIfG_resDelete(&mPhase, l_arcName);
}

void daObjItaRope_c::create_init() {
    gravity = attr().mGravity;
    JUT_ASSERT(359, getPathID() != 0xff);
    dPath* path = dPath_GetRoomPath(getPathID(), fopAcM_GetRoomNo(this));
    JUT_ASSERT(361, path != NULL && (path->m_num == 2));
    cXyz cStack_60(path->m_points[0].m_position);
    cXyz cStack_6c(path->m_points[1].m_position);
    current.pos = (cStack_60 + cStack_6c) * 0.5f;
    field_0x620 = cStack_6c - cStack_60;
    if (fabsf(field_0x620.x) > fabsf(field_0x620.z)) {
        field_0x62c = 1.0f / field_0x620.x;
        field_0x634 = 1;
    } else {
        field_0x62c = 1.0f / field_0x620.z;
        field_0x634 = 0;
    }
    shape_angle.y = cM_atan2s(field_0x620.x, field_0x620.z);
    shape_angle.x = cM_atan2s(-field_0x620.y, field_0x620.absXZ());
    f32 zCull = field_0x620.absXZ() * 0.5f + 3.0f;
    fopAcM_setCullSizeBox(this, -75.0f, -140.0f, -zCull, 75.0f, 20.0f, zCull);
    f32 dVar16 = field_0x620.abs();
    field_0x630 = dVar16 * (1.0f / (field_0x635 - 1));
    cXyz cStack_78 = field_0x620 * (field_0x630 / dVar16);
    cXyz* pPos = mLineMat.getPos(0);
    RopeWork_c* pRope = mRopes;
    *pPos = cStack_60;
    int i = 1;
    pPos++;
    for (; i < field_0x635 - 1; i++, pPos++, pRope++) {
        pPos[0] = pPos[-1] + cStack_78;
        pRope->field_0x0c = pPos[0];
        pRope->field_0x00.zero();
    }
    *pPos = cStack_6c;
    setNormalRopePos();
    initBaseMtx();
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "A_Sakuita.bmd");
    JUT_ASSERT(420, modelData != NULL);
    if (dComIfGp_addSimpleModel(modelData, fopAcM_GetRoomNo(this), 0) == -1) {
        OS_REPORT("\x1b[43;30mカラカラ板：シンプルモデル登録失敗しました。\n\x1b[m");
    }
    field_0x636 = 1;
    pPos = mLineMat.getPos( 0) + 1;
    Sakuita_c* pSakuita = mSakuitas;
    for (int i = 0; i < field_0x635 - 2; i++, pPos++, pSakuita++) {
        s16 sVar12 = cLib_targetAngleY(pPos, pPos + 1) + 0x4000;
        pSakuita->create(pPos, sVar12, fopAcM_GetRoomNo(this));
    }

    #if DEBUG
    M_hio.ct();
    #endif
}

void daObjItaRope_c::initBaseMtx() {
    setBaseMtx();
    fopAcM_SetMtx(this, mMtx);
}

void daObjItaRope_c::setBaseMtx() {
    mDoMtx_trans(mMtx, current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_ZXYrotM(mMtx, shape_angle.x, shape_angle.y, 0);
}

void daObjItaRope_c::setNormalRopePos() {
    cXyz* pPos = mLineMat.getPos(0) + 1;
    RopeWork_c* pRope = mRopes;
    int i;
    cXyz cStack_34;
    cXyz cStack_40 = dKyw_get_AllWind_vecpow(&current.pos);
    if (cM_rnd() < 0.1f) {
        cStack_40 *= field_0x630 * attr().mWindRate;
    } else {
        cStack_40.zero();
    }

    for (i = 1; i < field_0x635 - 1; i++, pPos++, pRope++) {
        pPos[0].y = pRope->field_0x0c.y;
        cStack_34 = pPos[0] - pPos[-1];
        cStack_34.y += gravity;
        cStack_34 += pRope->field_0x00;
        if (cM_rnd() < 0.6f) {
            cStack_34 += cStack_40;
        }
        cStack_34.normalizeZP();
        pPos[0] = pPos[-1] + (cStack_34 * field_0x630);
    }

    pPos = mLineMat.getPos(0) + field_0x635 - 2;
    pRope = mRopes + field_0x635 - 3;
    for (i = field_0x635 - 2; i > 0; i--, pPos--, pRope--) {
        cStack_34 = pPos[0] - pPos[1];
        cStack_34.y += gravity;
        cStack_34 += pRope->field_0x00;
        cStack_34.normalizeZP();
        pPos[0] = pPos[1] + (cStack_34 * field_0x630);
    }

    pPos = mLineMat.getPos(0) + 1;
    pRope = mRopes;
    cXyz* startPos = mLineMat.getPos(0);
    for (i = 1; i < field_0x635 - 1; i++, pPos++, pRope++) {
        f32 dVar10 = startPos->y + (field_0x620.y * getStartRate(pPos));
        pRope->field_0x00 = (*pPos - pRope->field_0x0c) * attr().mSpeedRate;
        pRope->field_0x0c = *pPos;
        pPos->y += (dVar10 - pPos->y) * attr().mOffsetRate;
    }
    mLineMat.getPos(0);
}

inline int daObjItaRope_c::createHeap() {
    mRopes = NULL;
    mRopes = new RopeWork_c[field_0x635 - 2];
    if (mRopes == NULL) {
        return 0;
    }

    mSakuitas = NULL;
    mSakuitas = new Sakuita_c[field_0x635 - 2];
    if (mSakuitas == NULL) {
        return 0;
    }

    if (mLineMat.init(1, field_0x635, (ResTIMG*)dComIfG_getObjectRes(l_arcName, "Obj_Rope.bti"), 1) == 0) {
        return 0;
    }

    f32* pSize = mLineMat.getSize(0);
    for (int i = 0; i < field_0x635; i++, pSize++) {
        *pSize = 2.0f;
    }

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "A_Sakuita.bmd");
    JUT_ASSERT(585, modelData != NULL);

    Sakuita_c* pSakuita = mSakuitas;
    for (int i = 0; i < field_0x635 - 2; i++, pSakuita++) {
        pSakuita->setModelData(modelData);
    }
    return 1;
}

static int createSolidHeap(fopAc_ac_c* i_actor) {
    daObjItaRope_c* i_this = static_cast<daObjItaRope_c*>(i_actor);
    return i_this->createHeap();
}

Sakuita_c::~Sakuita_c() {}

Sakuita_c::Sakuita_c() {}

RopeWork_c::~RopeWork_c() {}

RopeWork_c::RopeWork_c() {}

int daObjItaRope_c::draw() {
    static GXColor l_color = {0x14, 0xF, 0x0, 0xFF};
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    mLineMat.update(field_0x635, l_color, &tevStr);
    dComIfGd_set3DlineMat(&mLineMat);
    Sakuita_c* pSakuita = mSakuitas;
    for (int i = 0; i < field_0x635 - 2; i++, pSakuita++) {
        pSakuita->draw();
    }
    return 1;
}

static int daObjItaRope_Draw(daObjItaRope_c* i_this) {
    return i_this->draw();
}

int daObjItaRope_c::execute() {
    gravity = attr().mGravity;
    setNormalRopePos();
    cXyz cStack_2c = dKyw_get_AllWind_vecpow(&current.pos);
    cXyz* pPos = mLineMat.getPos(0) +  1;
    Sakuita_c* pSakuita = mSakuitas;
    for (int i = 1; i < field_0x635 - 1; i++, pPos++, pSakuita++) {
        cXyz cStack_38 = pPos[1] -  pPos[0];
        s16 angle = cLib_targetAngleY(pPos, pPos + 1) + 0x4000;
        pSakuita->execute(&cStack_2c, angle);
    }
    return 1;
}

static int daObjItaRope_Execute(daObjItaRope_c* i_this) {
    return i_this->execute();
}

static int daObjItaRope_IsDelete(daObjItaRope_c* i_this) {
    return 1;
}

static int daObjItaRope_Delete(daObjItaRope_c* i_this) {
    fopAcM_RegisterDeleteID(i_this,  "Obj_ItaRope");
    i_this->~daObjItaRope_c();
    return 1;
}

int daObjItaRope_c::create() {
    fopAcM_ct(this, daObjItaRope_c);
    int rv = dComIfG_resLoad(&mPhase, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        field_0x635 = getJointNum() + 2;
        if (field_0x635 < 3) {
            field_0x635 = 3;
        }
        if (fopAcM_entrySolidHeap(this, createSolidHeap, 0x1420) == 0) {
            return cPhs_ERROR_e;
        }
        create_init();
    }
    return rv;
}

static int daObjItaRope_Create(fopAc_ac_c* i_actor) {
    fopAcM_RegisterCreateID(daObjItaRope_c, i_actor, "Obj_ItaRope");
    return static_cast<daObjItaRope_c*>(i_actor)->create();
}

static actor_method_class l_daObjItaRope_Method = {
    (process_method_func)daObjItaRope_Create,
    (process_method_func)daObjItaRope_Delete,
    (process_method_func)daObjItaRope_Execute,
    (process_method_func)daObjItaRope_IsDelete,
    (process_method_func)daObjItaRope_Draw,
};

actor_process_profile_definition g_profile_Obj_ItaRope = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_ItaRope,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjItaRope_c), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  35,                     // mPriority
  &l_daObjItaRope_Method, // sub_method
  0x00040180,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
