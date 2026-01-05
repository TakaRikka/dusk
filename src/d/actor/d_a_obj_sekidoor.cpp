/**
 * @file d_a_obj_sekidoor.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_sekidoor.h"
   

static struct {
    u32 bmdIdx;
    u32 resIdx;
} l_bmdData[1] = {4, 1};

static struct {
    u32 dzbIdx;
    u32 resIdx;
} l_dzbData[1] = {7, 1};

static char* l_resNameList[2] = {"", "SekiDoor"};

int daObj_SekiDoor_c::create() {
    fopAcM_ct(this, daObj_SekiDoor_c);

    mBitSW = 0;
    
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, l_resNameList[l_bmdData[mBitSW].resIdx]);
    if (step == cPhs_COMPLEATE_e) {
        if (getBitSW() != 0xff){
            if (dComIfGs_isSwitch(getBitSW(), fopAcM_GetRoomNo(this))) {
                return cPhs_ERROR_e;
            }
        }

        step = (cPhs__Step)MoveBGCreate(l_resNameList[l_dzbData[mBitSW].resIdx], 
            l_dzbData[mBitSW].dzbIdx, dBgS_MoveBGProc_TypicalRotY, 0xc20, NULL
        );

        if (step == cPhs_ERROR_e) {
            return step;
        }
    }

    return step;
}

int daObj_SekiDoor_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(
        l_resNameList[l_bmdData[mBitSW].resIdx],
        l_bmdData[mBitSW].bmdIdx
    );
    
    if (model_data == NULL){
        return 0;
    }
    
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);

    if (mpModel == NULL) {
        return 0;
    }
    return 1;
}

int daObj_SekiDoor_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());

    return 1;
}

int daObj_SekiDoor_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_resNameList[l_bmdData[mBitSW].resIdx]);

    return 1;
}


const u8 daObj_SekiDoor_Param_c::m = 0;

static const f32 reference_posy = 460.0f;

static const f32 rising_speed_y = 4.0f;


int daObj_SekiDoor_c::Execute(Mtx** i_mtx) {
    cXyz temp_y_position;
    *i_mtx = &mBgMtx;

    if (mOpening != 0) {
        if (mDestroyed != 0) {
            if(!dComIfGp_event_runCheck){
                fopAcM_delete(this);
            }
        }
        else {
            if (cM3d_IsZero(mDoorPosY) && mFrameCounter == 0) {
                mOpenSpeed = 0;
                mFrameCounter++;
            }
            else {
                if (mOpenSpeed != 0) {
                    mRotation.z = mOpenSpeed * cM_ssin(mFrameCounter << 14);
    
                    if (cLib_chaseS(&mOpenSpeed, 0, 0x10)) {
                        dComIfGp_getVibration().StopQuake(0xf);
                    }
                    
                    mFrameCounter++;
                }
                if (mOpenSpeed == 0) {
                    temp_y_position = current.pos;
                    if (mDoorPosY < reference_posy) {
                        speed.y = rising_speed_y;
                        cLib_chaseF(&mDoorPosY, reference_posy, speed.y);
    
                        temp_y_position.y += mDoorPosY;

                        mDoAud_seStartLevel(Z2SE_OBJ_SEKI_DOOR_OP, &temp_y_position,
                            0, 0);
                    }
                    else {
                        temp_y_position.y += mDoorPosY;
                        
                        mDoAud_seStart(Z2SE_OBJ_SEKI_DOOR_OP_ST, NULL,
                            0, 0);
    
                        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 0xf, 
                            cXyz(0.0f, 1.0f, 0.0f));
                        mDestroyed = 1;
                    }
                    
                }
            }
        }
    }
    else {
        speed.setall(0.0f);
        mFrameCounter = 0;
        mOpenSpeed = 0;
        mDoorPosY = 0.0f;
        mDestroyed = 0;
    }    

    setBaseMtx();
    setPrtcls();
    return 1;
}

int daObj_SekiDoor_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    return 1;
}

void daObj_SekiDoor_c::evtSkip() {
    if (mFrameCounter != 0){
        dComIfGp_getVibration().StopQuake(0xf);
    }
    mRotation.setall(0);
    mDoorPosY = reference_posy;
    mOpening = true;
    mDestroyed = true;
}


void daObj_SekiDoor_c::setPrtcls() {
    static const u16 id[4] = {
        0x8B85, 0x8B86, 0x8B87, 0x8B88,
    };

    cXyz particle_pos = current.pos;
    cXyz particle_scale(1.0f,1.0f,1.0f);
    particle_pos.y += mDoorPosY;
    
    if (mOpening == false) {
        return;
    }
    if (mDestroyed != false) {
        for (int index = 2; index <= 3; index++){
            u16 id_partcl = id[index];

            mpEmitters[index] = dComIfGp_particle_set(
                mpEmitters[index], id_partcl, &particle_pos,
                NULL, &particle_scale
            );
        }
    }


    else {
        if (mDoorPosY < reference_posy) {
            for (int index = 0; index <= 1; index++){
                mpEmitters[index] = dComIfGp_particle_set(
                    mpEmitters[index], id[index], &particle_pos,
                    NULL, &particle_scale
                );
                
                JPABaseEmitter* emitter_p = dComIfGp_particle_getEmitter(mpEmitters[index]);
                if (emitter_p != NULL) {
                    emitter_p->setGlobalTranslation(particle_pos.x, particle_pos.y,particle_pos.z);
                }
            }
        }
    }
}

void daObj_SekiDoor_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObj_SekiDoor_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + mDoorPosY, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::ZrotM(mRotation.z);

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

static int daObj_SekiDoor_Create(void* i_this) {
    return static_cast<daObj_SekiDoor_c*>(i_this)->create();
}

static int daObj_SekiDoor_Delete(void* i_this) {
    return static_cast<daObj_SekiDoor_c*>(i_this)->MoveBGDelete();
}

static int daObj_SekiDoor_Execute(void* i_this) {
    return static_cast<daObj_SekiDoor_c*>(i_this)->MoveBGExecute();
}

static int daObj_SekiDoor_Draw(void* i_this) {
    return static_cast<daObj_SekiDoor_c*>(i_this)->MoveBGDraw();
}

static int daObj_SekiDoor_IsDelete(void* param_0) {
    return 1;
}


static daObj_SekiDoor_Param_c l_HIO;

static actor_method_class daObj_SekiDoor_MethodTable = {
    (process_method_func)daObj_SekiDoor_Create,
    (process_method_func)daObj_SekiDoor_Delete,
    (process_method_func)daObj_SekiDoor_Execute,
    (process_method_func)daObj_SekiDoor_IsDelete,
    (process_method_func)daObj_SekiDoor_Draw,
};

actor_process_profile_definition g_profile_OBJ_SEKIDOOR = {
    fpcLy_CURRENT_e,             // mLayerID
    7,                           // mListID
    fpcPi_CURRENT_e,             // mListPrio
    PROC_OBJ_SEKIDOOR,           // mProcName
    &g_fpcLf_Method.base,       // sub_method
    sizeof(daObj_SekiDoor_c),    // mSize
    0,                           // mSizeOther
    0,                           // mParameters
    &g_fopAc_Method.base,        // sub_method
    87,                          // mPriority
    &daObj_SekiDoor_MethodTable, // sub_method
    0x00044000,                  // mStatus
    fopAc_ACTOR_e,               // mActorType
    fopAc_CULLBOX_CUSTOM_e,      // cullType
  };
