/**
 * @file d_a_obj_smtile.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_smtile.h"
#include "d/d_com_inf_game.h"
#include "SSystem/SComponent/c_counter.h"

static u32 l_bmdData[1][2] = {
    4, 1,
};

static char* l_resNameList[2] = {
    "",
    "A_SMTile",
};

static s8 l_tileMoveData[21][4] = {
    0x01, 0x03, 0x05, 0x01, 0x01, 0x02, 0x04, 0x01, 0x01, 0x01, 0x03, 0x01, 0x01, 0x02,
    0x04, 0x00, 0x00, 0x02, 0x03, 0x01, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x01,
    0x00, -0x1, 0x02, 0x01, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x04, 0x00, 0x00, 0x01, 0x03, 0x00, -0x1, 0x02, 0x03, -0x1, 0x02, 0x04, 0x00,
    0x00, 0x02, 0x03, 0x02, -0x1, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, -0x1,
    0x02, 0x02, -0x1, 0x03, 0x05, 0x02, -0x1, 0x02, 0x04, 0x02, -0x1, 0x01, 0x03, 0x02,
};

daObj_SMTile_c::~daObj_SMTile_c() {
    OS_REPORT("|%06d:%x|daObj_SMTile_c -> デストラクト\n", g_Counter.mCounter0, this);
    dComIfG_resDelete(
        &mPhase,
        l_resNameList[l_bmdData[mType][1]]);
}

int daObj_SMTile_c::create() {
    fopAcM_ct(this, daObj_SMTile_c);
    mType = getType();
    int rv = dComIfG_resLoad(
        &mPhase,
       l_resNameList[l_bmdData[mType][1]]);
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) SWBit:%02x<%08x> -> roomNo.%d", fopAcM_getProcNameString(this), getType(), getBitSW(), fopAcM_GetParam(this),
                              fopAcM_GetRoomNo(this));
        if (getBitSW() != 0xff) {
            if (dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                OS_REPORT("===>isDelete:TRUE\n");
                return cPhs_ERROR_e;
            }
        }
        OS_REPORT("\n");
        if (fopAcM_entrySolidHeap(this, createHeapCallBack, 0x800) == 0) {
            return cPhs_ERROR_e;
        }
        field_0xb2b = 1;
        Execute();
    }
    return rv;
}

f32 const daObj_SMTile_Param_c::m[2] = {
    600.0f, 20.0f,
};

int daObj_SMTile_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(
        l_resNameList[l_bmdData[mType][1]],
        l_bmdData[mType][0]);
    if (modelData == NULL) {
        return 0;
    }
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mModel == NULL) {
        return 0;
    }
    J3DAnmTevRegKey* regKey =
        (J3DAnmTevRegKey*) dComIfG_getObjectRes(l_resNameList[l_bmdData[mType][1]], 7);
    if (regKey != NULL) {
        if (mBrk.init(modelData, regKey, 1, 0, 0.0f, 0, -1) != 0) {
            return 1;
        }
    }
    return 0;
}

int daObj_SMTile_c::Delete() {
    OS_REPORT("|%06d:%x|daObj_SMTile_c -> Delete\n", g_Counter.mCounter0, this);
    this->~daObj_SMTile_c();
    return 1;
}

int daObj_SMTile_c::Execute() {
    if (home.roomNo == dComIfGp_roomControl_getStayNo()) {
        if (field_0xb2b != 0) {
            for (int i = 0; i < 21; i++) {
                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleIds[i]);
                if (emitter != NULL) {
                    emitter->becomeDeleteEmitter();
                }
                mParticleIds[i] = 0;
                if (mEmitters[i] != NULL) {
                    mEmitters[i]->becomeDeleteEmitter();
                    mEmitters[i] = NULL;
                }
                mParticleTimers[i] = 0;
                field_0x9d4[i] = 0;
                field_0xa28[i] = 0;
            }
            mBrk.setPlaySpeed(0.0f);
            mBrk.setFrame(mBrk.getStartFrame());
            setDstPos();
            field_0xa7c = 0;
            field_0xb2b = 0;
        }
        if (field_0xb29 != 0) {
            switch (field_0xa7c) {
            case 0:
                setPrtcls(3, 0);
                mBrk.setPlaySpeed(1.0f);
                mDoAud_seStart(Z2SE_OBJ_SEKI_TFORCE_ON, 0, 0, 0);
                break;
            case 50:
                setPrtcls(1, 0);
                break;
            case 75:
                setPrtcls(2, 1);
                break;
            case 96:
                setPrtcls(2, 2);
                break;
            case 117:
                setPrtcls(2, 3);
                break;
            case 138:
                setPrtcls(2, 4);
                break;
            case 159:
                setPrtcls(2, 5);
                break;
            case 200:
                for (int i = 0; i < 21; i++) {
                    mDoAud_seStart(Z2SE_OBJ_SEKI_TILE_ON_LAST, &field_0x788[i], 0, 0);
                }
                break;
            }
            
            if (field_0xa7c > 240) {
                field_0xb29 = 0;
            } else {
                if (field_0xa7c > 220) {
                    field_0xb2a = 1;
                }
                f32 fVar1 = (240 - field_0xa7c) / 40.0f;
                if (1.0f < fVar1) {
                    fVar1 = 1.0f;
                }
                touchPrtcls(fVar1);
                field_0xa7c++;
            }
        }
        mBrk.play();
        setMtx();
        attention_info.position = current.pos;
        eyePos = attention_info.position;
        return 1;
    } 
    return 0;
}

int daObj_SMTile_c::Draw() {
    J3DModelData* modelData = mModel->getModelData();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    mBrk.entry(modelData);
    mDoExt_modelUpdateDL(mModel);
    mBrk.remove(modelData);
    return 1;
}

int daObj_SMTile_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daObj_SMTile_c*>(i_this)->CreateHeap();
}

void daObj_SMTile_c::setDstPos() {
    cXyz local_2c;
    cXyz local_38;
    mDoMtx_stack_c::YrotS(current.angle.y);
    for (int i = 0; i < 21; i++) {
        local_2c.setall(0.0f);
        local_2c.x = l_tileMoveData[i][0];
        local_2c.z = l_tileMoveData[i][1];
        local_38 = local_2c;
        local_38.x *= daObj_SMTile_Param_c::m[0];
        local_38.z *= daObj_SMTile_Param_c::m[0];
        mDoMtx_stack_c::multVec(&local_38, &field_0x68c[i]);
        switch ((u8)l_tileMoveData[i][3]) {
        case 0:
            local_2c.z += 1.0f;
            break;
        case 1:
            local_2c.x += 1.0f;
            break;
        case 2:
            local_2c.x -= 1.0f;
            break;
        case 3:
            local_2c.z -= 1.0f;
            break;    
        }

        local_2c.x *= daObj_SMTile_Param_c::m[0];
        local_2c.z *= daObj_SMTile_Param_c::m[0];
        mDoMtx_stack_c::multVec(&local_2c, &field_0x590[i]);
    }
}

void daObj_SMTile_c::setPrtcls(int param_1, int param_2) {
    switch(param_1) {
    case 1:
    case 2:
        for (int i = 0; i < 21; i++) {
            if (param_2 == l_tileMoveData[i][2]) {
                if (param_1 == 1) {
                    mDoAud_seStart(Z2SE_OBJ_SEKI_TILE_EMERGE, &field_0x788[i], 0, 0);
                }
                field_0x788[i] = current.pos + field_0x68c[i];
                mParticleTimers[i] = daObj_SMTile_Param_c::m[1];
                field_0xa28[i] = param_1;
            }
        }
        break;
    case 3:
        for (int i = 0; i < 21; i++) {
            field_0x788[i + 21] = current.pos + field_0x590[i];
            mEmitters[i] = dComIfGp_particle_set(0x86ee, field_0x788 + i + 21, NULL, NULL);
        }
        break;
    }
}

static u16 const id[4] = {
    0xFFFF, 0x86EC, 0x86ED, 0x86EE,
};

void daObj_SMTile_c::touchPrtcls(f32 param_1) {
    cXyz local_3c;
    cXyz cStack_48(1.0f, 1.0f, 1.0f);
    int local_30 = param_1 * 255.0f;
    mDoMtx_stack_c::YrotS(current.angle.y);
    for (int i = 0; i < 21; i++) {
        if ((field_0xa28[i] == 1) || (field_0xa28[i] == 2)) {
            mParticleIds[i] =
                dComIfGp_particle_set(mParticleIds[i], id[field_0xa28[i]], &field_0x788[i], 0, 0);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleIds[i]);
            if (emitter != NULL) {
                f32 dVar6 = daObj_SMTile_Param_c::m[1] - mParticleTimers[i];
                dVar6 /= daObj_SMTile_Param_c::m[1];
                local_3c.setall(0.0f);
                switch ((u8)l_tileMoveData[i][3]) {
                case 0:
                    local_3c.z = daObj_SMTile_Param_c::m[0] * dVar6;
                    break;
                case 1:
                    local_3c.x = daObj_SMTile_Param_c::m[0] * dVar6;
                    break;
                case 2:
                    local_3c.x = daObj_SMTile_Param_c::m[0] * dVar6 * -1.0f;
                    break;
                case 3:
                    local_3c.z = daObj_SMTile_Param_c::m[0] * dVar6 * -1.0f;
                    break;
                }

                mDoMtx_stack_c::multVec(&local_3c, &local_3c);
                field_0x788[i] = current.pos + field_0x68c[i] + local_3c;
                if (mParticleTimers[i] > 0 && cLib_calcTimer(&mParticleTimers[i]) == 0) {
                    mDoAud_seStart(Z2SE_OBJ_SEKI_TILE_ON, &field_0x788[i], 0, 0);
                }
                emitter->setGlobalTranslation(field_0x788[i].x, field_0x788[i].y, field_0x788[i].z);
                emitter->setGlobalAlpha(local_30);
            }
        }
    }
}

void daObj_SMTile_c::setMtx() {
    csXyz acStack_18(shape_angle);
    acStack_18.y += 0x8000;
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(acStack_18);
    mDoMtx_stack_c::scaleM(scale);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static int daObj_SMTile_Create(void* i_this) {
    return static_cast<daObj_SMTile_c*>(i_this)->create();
}

static int daObj_SMTile_Delete(void* i_this) {
    return static_cast<daObj_SMTile_c*>(i_this)->Delete();
}

static int daObj_SMTile_Execute(void* i_this) {
    return static_cast<daObj_SMTile_c*>(i_this)->Execute();
}

static int daObj_SMTile_Draw(void* i_this) {
    return static_cast<daObj_SMTile_c*>(i_this)->Draw();
}

static int daObj_SMTile_IsDelete(void* i_this) {
    return 1;
}

static daObj_SMTile_Param_c l_HIO;

static actor_method_class daObj_SMTile_MethodTable = {
    (process_method_func)daObj_SMTile_Create,
    (process_method_func)daObj_SMTile_Delete,
    (process_method_func)daObj_SMTile_Execute,
    (process_method_func)daObj_SMTile_IsDelete,
    (process_method_func)daObj_SMTile_Draw,
};

actor_process_profile_definition g_profile_OBJ_SMTILE = {
  fpcLy_CURRENT_e,           // mLayerID
  7,                         // mListID
  fpcPi_CURRENT_e,           // mListPrio
  PROC_OBJ_SMTILE,           // mProcName
  &g_fpcLf_Method.base,     // sub_method
  sizeof(daObj_SMTile_c),    // mSize
  0,                         // mSizeOther
  0,                         // mParameters
  &g_fopAc_Method.base,      // sub_method
  89,                        // mPriority
  &daObj_SMTile_MethodTable, // sub_method
  0x00044000,                // mStatus
  fopAc_ACTOR_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,    // cullType
};
