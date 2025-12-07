/**
 * @file d_a_obj_itamato.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_itamato.h"
#include "d/d_cc_d.h"
#include "d/actor/d_a_npc.h"
#include "JSystem/JHostIO/JORFile.h"

struct daObj_ItaMato_HIOParam {
    /* 0x00 */ f32 attn_offset;
    /* 0x04 */ f32 gravity;
    /* 0x08 */ f32 scale;
    /* 0x0C */ f32 real_shadow_size;
    /* 0x10 */ f32 shake_pow;
};

class daObj_ItaMato_Param_c {
public:
    virtual ~daObj_ItaMato_Param_c() {}

    static daObj_ItaMato_HIOParam const m;
};

#if DEBUG
#define PARAM mHIO->mAttr
#define PARAM_CLASS daObj_ItaMato_HIO_c

class daObj_ItaMato_HIO_c : public mDoHIO_entry_c {
public:
    daObj_ItaMato_HIO_c();
    void genMessage(JORMContext* ctx);
    void listenPropertyEvent(const JORPropertyEvent*);

    /* 0x8 */ daObj_ItaMato_HIOParam mAttr;
};
#else
#define PARAM daObj_ItaMato_Param_c::m
#define PARAM_CLASS daObj_ItaMato_Param_c
#endif

daObj_ItaMato_HIOParam const daObj_ItaMato_Param_c::m = {
    110.0f,
    -3.0f,
    1.0f,
    400.0f,
    20.0f,
};

#if DEBUG
daObj_ItaMato_HIO_c::daObj_ItaMato_HIO_c() {
    mAttr = daObj_ItaMato_Param_c::m;
}

void daObj_ItaMato_HIO_c::listenPropertyEvent(const JORPropertyEvent* i_event) {
    char buffer[2000];
    size_t len;
    JORReflexible::listenPropertyEvent(i_event);
    JORFile jorFile;

    switch ((u32)i_event->id) {
        case 0x40000002:
            if (jorFile.open(6, "すべてのファイル(*.*)", NULL, NULL, NULL)) {
                memset(buffer, 0, sizeof(buffer));
                len = 0;
                sprintf(buffer + len, "%.3ff,\t//  注目オフセット\n", mAttr.attn_offset);
                len = strlen(buffer);
                sprintf(buffer + len, "%.3ff,\t//  重力\n", mAttr.gravity);
                len = strlen(buffer);
                sprintf(buffer + len, "%.3ff,\t//  スケ−ル\n", mAttr.scale);
                len = strlen(buffer);
                sprintf(buffer + len, "%.3ff,\t//  リアル影サイズ\n", mAttr.real_shadow_size);
                len = strlen(buffer);
                sprintf(buffer + len, "%.3ff,\t//  揺れパワ−\n", mAttr.shake_pow);
                len = strlen(buffer);
                jorFile.writeData(buffer, len);
                jorFile.close();
                OS_REPORT("write append success!::%6d\n", len);
            } else {
                OS_REPORT("write append failure!\n");
            }
            break;
    }
}

void daObj_ItaMato_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genSlider("注目オフセット　", &mAttr.attn_offset, 0.0f, 1000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("重力　　　　　　", &mAttr.gravity, -100.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("スケ−ル　　　　", &mAttr.scale, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("リアル影サイズ　", &mAttr.real_shadow_size, 0.0f, 10000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genSlider("揺れパワ−　　　", &mAttr.shake_pow, 0.0f, 90.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

static dCcD_SrcGObjInf const l_ccDObjData = {
    {0x0, {{0x0, 0x0, 0x0}, {0xD8FBFDFF, 0x1F}, 0x79}}, // mObj
    {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
    {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
    {0x0}, // mGObjCo
};

static dCcD_SrcSph l_ccDSph = {
    l_ccDObjData,
    {
        {{0.0f, 0.0f, 0.0f}, 0.0f} // mSph
    } // mSphAttr
};

static PARAM_CLASS l_HIO;

static char* l_resName = "H_ItaMato";

daObj_ItaMato_c::~daObj_ItaMato_c() {
    OS_REPORT("|%06d:%x|daObj_ItaMato_c -> デストラクト\n", g_Counter.mCounter0, this);

    #if DEBUG
    if (mHIO != NULL) {
        mHIO->removeHIO();
    }
    #endif

    dComIfG_resDelete(&mPhase, getResName());
}

cPhs__Step daObj_ItaMato_c::create() {
    fopAcM_ct(this, daObj_ItaMato_c);

    mType = getType();
    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, getResName());
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) no:%d, SWBit:%02x, SWBit2:%02x<%08x> -> roomNo.%d", fopAcM_getProcNameString(this), getType(), getNo(),
                  getBitSW(), getBitSW2(), fopAcM_GetParam(this), fopAcM_GetRoomNo(this));
        if (getBitSW() != 0xFF) {
            if (dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                OS_REPORT("===>isDelete:TRUE\n");
                return cPhs_ERROR_e;
            }
        }

        OS_REPORT("\n");

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x16B0)) {
            return cPhs_ERROR_e;
        }

        health = 1;
        J3DModelData* modelData = mpModels[health]->getModelData();
        fopAcM_SetMtx(this, mpModels[health]->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -50.0f, -200.0f, 200.0f, 200.0f, 200.0f);

        #if DEBUG
        mHIO = &l_HIO;
        mHIO->entryHIO("板的");
        #endif

        mBgc.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mStts.Init(0xFF, 0, this);
        mSph.Set(l_ccDSph);
        mSph.SetStts(&mStts);
        mSph.SetTgHitCallback(tgHitCallBack);
        mBgc.CrrPos(dComIfG_Bgsp());
        mGndChk = mBgc.m_gnd;
        mGroundH = mBgc.GetGroundH();
        if (mGroundH != -1e9f) {
            setEnvTevColor();
            setRoomNo();
        }

        daNpcT_offTmpBit(60); // dSv_event_tmp_flag_c::T_0060 - Kakariko Village - During bow and arrow game
        daNpcT_offTmpBit(63); // dSv_event_tmp_flag_c::T_0063 - Kakariko Village - [Bow and arrow game] Missed the final target!
        daNpcT_offTmpBit(64); // dSv_event_tmp_flag_c::T_0064 - Kakariko Village - [Bow and arrow game] Attempting to hit target...
        daNpcT_offTmpBit(65); // dSv_event_tmp_flag_c::T_0065 - Kakariko Village - [Bow and arrow game] Destroy first one with Clawshot
        daNpcT_offTmpBit(66); // dSv_event_tmp_flag_c::T_0066 - Kakariko Village - [Bow and arrow game] Restart from the middle

        if (strcmp(dComIfGp_getStartStageName(), "F_SP103") == 0 && dComIfGp_roomControl_getStayNo() == 1) {
            field_0xa18 = 1;
        }

        Execute();
    }

    return phase;
}

int daObj_ItaMato_c::CreateHeap() {
    J3DModelData* modelData = NULL;
    modelData = (J3DModelData*)dComIfG_getObjectRes(getResName(), 5);
    if (modelData == NULL) {
        return 0;
    }

    mpModels[0] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModels[0] == NULL) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes(getResName(), 4);
    if (modelData == NULL) {
        return 0;
    }

    mpModels[1] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModels[1] == NULL) {
        return 0;
    }

    return 1;
}

int daObj_ItaMato_c::Delete() {
    OS_REPORT("|%06d:%x|daObj_ItaMato_c -> Delete\n", g_Counter.mCounter0, this);
    this->~daObj_ItaMato_c();
    return 1;
}

int daObj_ItaMato_c::Execute() {
    cXyz sp38, sp44, sp50;

    mStts.Move();
    field_0xa16 = 1;

    if (field_0xa18 != 0) {
        field_0xa16 = 0;
    } else if (getBitSW2() == 0xFF || (getBitSW2() != 0xFF && dComIfGs_isSwitch(getBitSW2(), fopAcM_GetRoomNo(this)))) {
        field_0xa16 = daNpcT_chkTmpBit(60) == FALSE;
    }

    attention_info.flags = 0;
    field_0xa15 = 0;

    if (field_0x9e8 != 0) {
        field_0x9e8--;
    }

    if (field_0x9e8 == 0 && mSph.ChkTgHit()) {
        setSwayParam(mSph.GetTgHitAc());

        if (!mSph.GetTgHitObj()->ChkAtType(AT_TYPE_BOOMERANG)) {
            field_0xa15 = 1;
            if (health != 0) {
                health--;

                if (health < 1) {
                    u8 i_no = 4;

                    if (field_0xa18 != 0) {
                        if (getBitSW2() != 0xFF) {
                            if (!dComIfGs_isSwitch(getBitSW2(), fopAcM_GetRoomNo(this))) {
                                dComIfGs_onSwitch(getBitSW2(), fopAcM_GetRoomNo(this));
                            }
                        }

                        dComIfGs_setTmpReg(0xF4FF, dComIfGs_getTmpReg(0xF4FF) + 1);
                        if (dComIfGp_getAttention()->GetLockonList(0) != NULL) {
                            if (dComIfGp_getAttention()->LockonTruth()) {
                                if (this == dComIfGp_getAttention()->GetLockonList(0)->getActor()) {
                                    i_no |= 0x80;
                                }
                            }
                        }

                        dComIfGs_setTmpReg(0xF3FF, i_no);
                    } else if (getBitSW() != 0xFF) {
                        if (!dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                            dComIfGs_onSwitch(getBitSW(), fopAcM_GetRoomNo(this));
                        }
                    }
                }

                fopAcM_seStartCurrent(this, Z2SE_OBJ_ARW_TRGT_BRK, 0);
                setPrtcls();
                mFake = 0;
            }
        }
    }

    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mBgc.CrrPos(dComIfG_Bgsp());
    mGndChk = mBgc.m_gnd;
    mGroundH = mBgc.GetGroundH();
    if (mGroundH != -1e9f) {
        setEnvTevColor();
        setRoomNo();
    }

    for (int i = 0; i < 3; i++) {
        if (field_0xa17 != 0) {
            mDoMtx_stack_c::YrotS(field_0x9fc[i]);
            sp44.set(0.0f, 0.0f, field_0x9f0[i]);
            mDoMtx_stack_c::multVec(&sp44, &sp50);
            field_0x9d0[i].x = sp50.z;
            field_0x9d0[i].z = -sp50.x;
            field_0x9fc[i] -= cM_deg2s(45.0f);
        } else {
            mDoMtx_stack_c::YrotS(field_0xa02);
            sp44.set(0.0f, 0.0f, field_0x9f0[i] * cM_ssin(field_0x9fc[i]));
            mDoMtx_stack_c::multVec(&sp44, &sp50);
            field_0x9d0[i].x = sp50.z;
            field_0x9d0[i].z = -sp50.x;
            field_0x9fc[i] += cM_deg2s(45.0f);
        }
    }

    cLib_addCalc2(&field_0x9f0[0], 0.0f, 0.075f, 50.0f);
    cLib_addCalc2(&field_0x9f0[1], 0.0f, 0.1f, 100.0f);
    cLib_addCalc2(&field_0x9f0[2], 0.0f, 0.125f, 125.0f);
    setMtx();
    attention_info.position = current.pos;
    attention_info.position.y += PARAM.attn_offset;
    eyePos = attention_info.position;

    if (field_0xa16 == 0) {
        u32 tgType = 0xD8FBFDFF;
        sp38 = current.pos;

        if (health == 0) {
            mSph.OffTgSetBit();
            sp38.y += 10.0f;
            mSph.SetC(sp38);
            mSph.SetR(10.0f);
        } else {
            if (daNpcT_chkTmpBit(60) && !mFake) {
                tgType = 0xD8FBDDFF;
            }

            mSph.SetTgType(tgType);

            if (mFake) {
                mSph.SetR(100.0f);
            } else {
                mSph.SetR(60.0f);
            }

            sp38.y += 110.0f;
            mSph.SetC(sp38);
        }

        dComIfG_Ccsp()->Set(&mSph);
    }

    mSph.ClrCoHit();
    mSph.ClrTgHit();

    return 1;
}

int daObj_ItaMato_c::Draw() {
    if (field_0xa16 == 0 && (field_0xa18 == 0 || health != 0)) {
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);

        if (field_0xa18 != 0) {
            g_env_light.setLightTevColorType_MAJI(mpModels[1], &tevStr);
            mpModels[1]->getModelData()->getMaterialNodePointer(0)->getShape()->hide();
            mDoExt_modelUpdateDL(mpModels[1]);
            mpModels[1]->getModelData()->getMaterialNodePointer(0)->getShape()->show();
        } else {
            g_env_light.setLightTevColorType_MAJI(mpModels[health], &tevStr);
            mDoExt_modelUpdateDL(mpModels[health]);
        }

        if (field_0xa18 == 0 && mGroundH != -1e9f) {
            mShadowKey = dComIfGd_setShadow(mShadowKey, 1, mpModels[health], &current.pos, 400.0f, 20.0f,
                                            current.pos.y, mGroundH, mGndChk, &tevStr, 0, 1.0f,
                                            dDlst_shadowControl_c::getSimpleTex());
        }
    }

    return 1;
}

int daObj_ItaMato_c::createHeapCallBack(fopAc_ac_c* a_this) {
    daObj_ItaMato_c* i_this = (daObj_ItaMato_c*)a_this;
    return i_this->CreateHeap();
}

void daObj_ItaMato_c::tgHitCallBack(fopAc_ac_c* param_1, dCcD_GObjInf* param_2, fopAc_ac_c* param_3, dCcD_GObjInf* param_4) {
    daObj_ItaMato_c* i_this = (daObj_ItaMato_c*)param_1;
    u8 cutType = 0;

    if (param_3 != NULL) {
        if (fopAcM_GetProfName(param_3) == PROC_ALINK) {
            cutType = ((daPy_py_c*)param_3)->getCutType();
        } else {
            cutType = daPy_py_c::CUT_TYPE_NM_VERTICAL;
        }
    }

    i_this->setCutType(cutType);
}

const char* daObj_ItaMato_c::getResName() {
    return l_resName;
}

void daObj_ItaMato_c::setSwayParam(fopAc_ac_c* i_actor) {
    f32 fVar1 = 1.0f;
    f32 fVar2[3] = {0.0f, 0.0f, PARAM.shake_pow};

    field_0xa02 = fopAcM_searchActorAngleY(this, i_actor) - shape_angle.y + 0x8000;
    field_0x9e8 = 8;
    field_0xa17 = 0;

    if (fopAcM_GetProfName(i_actor) == PROC_ALINK) {
        switch (mCutType) {
            case 3:
            case 4:
            case 6:
            case 7:
                if (mCutType == 3) {
                    field_0xa02 -= (s16)0x4000;
                }

                if (mCutType == 4) {
                    field_0xa02 += (s16)0x4000;
                }

                fVar1 = 0.6f;
                break;

            case 8:
                field_0x9e8 = 20;
                field_0xa17 = 1;
                break;

            case 5:
            default:
                fVar1 = 0.6f;
                break;
        }
    } else {
        fVar1 = 0.6f;
    }

    for (int i = 0; i < 3; i++) {
        field_0x9f0[i] = fVar1 * cM_deg2s(fVar2[i]);
        field_0x9fc[i] = 0x4000;
    }
}

void daObj_ItaMato_c::setPrtcls() {
    static u16 const id[3] = {
        dPa_RM(ID_ZF_S_ITAMATO_BOAD00),
        dPa_RM(ID_ZF_S_ITAMATO_BOAD01),
        dPa_RM(ID_ZF_S_ITAMATO_SMK),
    };

    for (int i = 0; i < 3; i++) {
        mPrtcls[i] = dComIfGp_particle_set(mPrtcls[i], id[i], &current.pos, &shape_angle, NULL);
        dComIfGp_particle_levelEmitterOnEventMove(mPrtcls[i]);
    }
}

void daObj_ItaMato_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daObj_ItaMato_c::setRoomNo() {
    s8 id = dComIfG_Bgsp().GetRoomId(mGndChk);
    fopAcM_SetRoomNo(this, id);
    mStts.SetRoomId(id);
}

void daObj_ItaMato_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::XrotM(field_0x9d0[2].x);
    mDoMtx_stack_c::ZrotM(field_0x9d0[2].z);
    mDoMtx_stack_c::scaleM(scale);
    mpModels[health]->setBaseTRMtx(mDoMtx_stack_c::get());
}

static int daObj_ItaMato_Create(void* a_this) {
    return static_cast<daObj_ItaMato_c*>(a_this)->create();
}

static int daObj_ItaMato_Delete(void* a_this) {
    return static_cast<daObj_ItaMato_c*>(a_this)->Delete();
}

static int daObj_ItaMato_Execute(void* a_this) {
    return static_cast<daObj_ItaMato_c*>(a_this)->Execute();
}

static int daObj_ItaMato_Draw(void* a_this) {
    return static_cast<daObj_ItaMato_c*>(a_this)->Draw();
}

static int daObj_ItaMato_IsDelete(void* a_this) {
    return 1;
}

static actor_method_class daObj_ItaMato_MethodTable = {
    (process_method_func)daObj_ItaMato_Create,
    (process_method_func)daObj_ItaMato_Delete,
    (process_method_func)daObj_ItaMato_Execute,
    (process_method_func)daObj_ItaMato_IsDelete,
    (process_method_func)daObj_ItaMato_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_ITAMATO = {
  fpcLy_CURRENT_e,            // mLayerID
  7,                          // mListID
  fpcPi_CURRENT_e,            // mListPrio
  PROC_OBJ_ITAMATO,           // mProcName
  &g_fpcLf_Method.base,      // sub_method
  sizeof(daObj_ItaMato_c),    // mSize
  0,                          // mSizeOther
  0,                          // mParameters
  &g_fopAc_Method.base,       // sub_method
  83,                         // mPriority
  &daObj_ItaMato_MethodTable, // sub_method
  0x00044100,                 // mStatus
  fopAc_ACTOR_e,              // mActorType
  fopAc_CULLBOX_CUSTOM_e,     // cullType
};
