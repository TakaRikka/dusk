/**
 * d_a_obj_zdoor.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_zdoor.h"
#include "d/actor/d_a_player.h"

static const cM3dGCylS l_cyl_data[3] = {
    {{
         0.0f,
         0.0f,
         0.0f,
     },
     35.0f,
     400.0f},
    {{
         0.0f,
         0.0f,
         0.0f,
     },
     35.0f,
     350.0f},
    {{
         0.0f,
         0.0f,
         0.0f,
     },
     30.0f,
     150.0f},
};

static const cM3dGCylS l_cyl_data2[3] = {
    {{
         0.0f,
         0.0f,
         0.0f,
     },
     80.0f,
     400.0f},
    {{
         0.0f,
         0.0f,
         0.0f,
     },
     80.0f,
     350.0f},
    {{
         0.0f,
         0.0f,
         0.0f,
     },
     50.0f,
     150.0f},
};

static const s16 l_open_limit_max[3] = {
    0x4E20,
    0x5555,
    0,
};

static const s16 l_open_limit_min[3] = {
    0x1388,
    -0x5555,
    -0x51C7,
};

static const s16 l_open_speed[3] = {
    0x00FA,
    0x0200,
    0x0200,
};

/*
    Checks if the player is in front or behind the door.
*/
static BOOL checkPlayerPos(daZdoor_c* i_this) {

    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz distance_to_player = player->current.pos - i_this->current.pos;

    mDoMtx_stack_c::YrotS(-i_this->current.angle.y);
    mDoMtx_stack_c::multVec(&distance_to_player, &distance_to_player);

    // Variable name could be the opposite; "is_behind".
    BOOL is_in_front = TRUE;
    if (distance_to_player.z > 0.0f) {
        is_in_front = FALSE;
    }
    return is_in_front;
}

static void doorCoHitCallBack(fopAc_ac_c* param_0, dCcD_GObjInf* param_1, fopAc_ac_c* colliding_actor,
                                  dCcD_GObjInf* param_3) {
    daZdoor_c* i_this = static_cast<daZdoor_c*>(param_0);

    fopAc_ac_c* player_actor = dComIfGp_getLinkPlayer();
    if (i_this != NULL) {
        u32 type = i_this->getType();
        s16 open_speed = l_open_speed[type & 0xFF];
        if (colliding_actor != NULL && colliding_actor == player_actor) {
            if (player_actor->speedF > 3.0f) {
                BOOL player_direction = checkPlayerPos(i_this);
                if (player_direction) {
                    i_this->mOpenSpeed = -open_speed;
                } else {
                    i_this->mOpenSpeed = open_speed;
                }
            }
        }
    }
}

static void doorTgHitCallBack(fopAc_ac_c* param_0, dCcD_GObjInf* param_1, fopAc_ac_c* colliding_actor,
                                  dCcD_GObjInf* param_3) {
    daZdoor_c* i_this = static_cast<daZdoor_c*>(param_0);

    fopAc_ac_c* player_actor = dComIfGp_getLinkPlayer();
    if (i_this == NULL) {
        return;
    }

    if (colliding_actor != player_actor) {
        return;
    }

    u32 type = i_this->getType();
    s16 open_speed = l_open_speed[type & 0xFF];

    if (param_3->ChkAtType(AT_TYPE_NORMAL_SWORD) == 0 && param_3->ChkAtType(AT_TYPE_UNK) == 0) {
        return;
    }

    if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_TURN_RIGHT || 
        daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_UNK_9) {
        open_speed *= 2.0f;
    }

    BOOL player_direction = checkPlayerPos(i_this);
    if (player_direction) {
        i_this->mOpenSpeed = -open_speed;
    } else {
        i_this->mOpenSpeed = open_speed;
    }
}

static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x11}, 0x19}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                  // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4},                  // mGObjTg
        {0x0},                                               // mGObjCo
    },                                                       // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            35.0f,               // mRadius
            400.0f               // mHeight
        }                        // mCyl
    }                            // mCylAttr
};

static dCcD_SrcCyl l_cyl_src2 = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x79}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},          // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4},          // mGObjTg
        {0x0},                                       // mGObjCo
    },                                               // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            80.0f,               // mRadius
            400.0f               // mHeight
        }                        // mCyl
    }                            // mCylAttr
};

void daZdoor_c::init_cyl() {
    mStts.Init(0xff, 0xff, this);
    for (int i = 0; i < 4; i++) {
        mCallbackCylinders[i].Set(l_cyl_src);
        mCallbackCylinders[i].SetStts(&mStts);
        mCallbackCylinders[i].SetCoHitCallback(&doorCoHitCallBack);
        mCallbackCylinders[i].SetTgHitCallback(&doorTgHitCallBack);
        mCallbackCylinders[i].cM3dGCyl::Set(l_cyl_data[mDoorType]);
    }

    mCylinder.Set(l_cyl_src2);
    mCylinder.SetStts(&mStts);
    mCylinder.cM3dGCyl::Set(l_cyl_data2[mDoorType]);
}

void daZdoor_c::set_cyl() {
    cXyz vecs[] = {
        cXyz(60.0f, 0.0f, 0.0f),
        cXyz(120.0f, 0.0f, 0.0f),
        cXyz(180.0f, 0.0f, 0.0f),
        cXyz(240.0f, 0.0f, 0.0)
    };

    cXyz current_vec;
    for (int i = 0; i < 4; i++) {
        current_vec = vecs[i];
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(current.angle.y);
        mDoMtx_stack_c::multVec(&current_vec, &current_vec);
        mCallbackCylinders[i].SetC(current_vec);
    }

    mCylinder.SetC(current.pos);
}

void daZdoor_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daZdoor_c::Create() {
    setBaseMtx();

    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());

    init_cyl();
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());

    home.angle.y = home.angle.z;
    current.angle.z = 0;
    home.angle.z = 0;
    mOpenAngle = current.angle.y - home.angle.y;

    return 1;
}

static char* l_arcName[3] = {
    "J_Zdoor",
    "J_Rdoor",
    "J_STdoa",
};

static char* l_bmdName[3] = {
    "J_Zdoor.bmd",
    "J_Rdoor.bmd",
    "J_STdoa.bmd",
};

int daZdoor_c::CreateHeap() {
    J3DModelData* modelData
        = (J3DModelData*)dComIfG_getObjectRes(l_arcName[mDoorType], l_bmdName[mDoorType]);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);

    if (mpModel == NULL) {
        return FALSE;
    }

    return TRUE;
}

static const u16 estimateSizeTbl[2] = {
    0x0CF0,
    0x0D30,
};

static char* l_dzbName[3] = {
    "J_Zdoor.dzb",
    "J_Rdoor.dzb",
    "J_STdoa.dzb",
};

int daZdoor_c::create1st() {
    mDoorType = getType();
    int phase_state = dComIfG_resLoad(&mPhaseReq, l_arcName[mDoorType]);
    if (phase_state == cPhs_COMPLEATE_e) {
        int dzb_id = dComIfG_getObjctResName2Index(l_arcName[mDoorType], l_dzbName[mDoorType]);
        JUT_ASSERT(604, dzb_id != -1);
        
        u16 estimate_size = estimateSizeTbl[mDoorType];
        phase_state = MoveBGCreate(l_arcName[mDoorType], dzb_id, 0x0, estimate_size, 0x0);

        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }
    }

    return phase_state;
}

int daZdoor_c::Execute(Mtx** i_mtx) {
    s16 current_open_angle = mOpenAngle;
    set_cyl();

    s16 open_limit_max = l_open_limit_max[mDoorType];
    s16 open_limit_min = l_open_limit_min[mDoorType];
    cLib_addCalcAngleS(&mOpenSpeed, 0, 0x14, 0x60, 0x10);

    mOpenAngle += mOpenSpeed;

    if (mOpenAngle > open_limit_max) {
        mOpenAngle = open_limit_max;
        if (current_open_angle == open_limit_max) {
            mOpenSpeed = 0;
        } else {
            mOpenSpeed = -mOpenSpeed;
        }
    } else if (mOpenAngle < open_limit_min) {
        mOpenAngle = open_limit_min;
        if (current_open_angle == open_limit_min) {
            mOpenSpeed = 0;
        } else {
            mOpenSpeed = -mOpenSpeed;
        }
    }

    current.angle.y = home.angle.y + mOpenAngle;
    if (old.angle.y != current.angle.y) {
        switch (mDoorType) {
            case 0:
                fopAcM_seStartLevel(this, Z2SE_OBJ_ZELDAROOM_DOOR_MOVE, 0);
                break;
            case 1:
                fopAcM_seStartLevel(this, Z2SE_OBJ_MOVE_JAIL_DOOR, 0);
                break;
            case 2:
                break;
        }
    }

    setBaseMtx();
    *i_mtx = &mBgMtx;

    for (int i = 0; i < 4; i++) {
        dComIfG_Ccsp()->Set(&mCallbackCylinders[i]);
    }

    dComIfG_Ccsp()->Set(&mCylinder);

    return 1;
}

int daZdoor_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel->mModelData, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    return 1;
}

int daZdoor_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName[mDoorType]);
    return 1;
}

static int daZdoor_create1st(daZdoor_c* i_this) {
    fopAcM_ct(i_this, daZdoor_c);

    return i_this->create1st();
}

static int daZdoor_MoveBGDelete(daZdoor_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daZdoor_MoveBGExecute(daZdoor_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daZdoor_MoveBGDraw(daZdoor_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daZdoor_METHODS = {
    (process_method_func)daZdoor_create1st,
    (process_method_func)daZdoor_MoveBGDelete,
    (process_method_func)daZdoor_MoveBGExecute,
    0,
    (process_method_func)daZdoor_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_ZDoor = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_ZDoor,         // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daZdoor_c),      // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  24,                     // mPriority
  &daZdoor_METHODS,       // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
