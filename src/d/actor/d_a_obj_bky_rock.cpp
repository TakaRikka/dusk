/**
 * @file d_a_obj_bky_rock.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_bky_rock.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_bg_w.h"
#include "d/d_model.h"
#include "d/d_com_inf_game.h"

static int daBkyRock_c_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daBkyRock_c*>(i_this)->createHeap();
}

static char* l_arcName = "BYRock";

daBkyRock_c::exeProc daBkyRock_c::s_exeProc[3] = {
    &daBkyRock_c::exeModeNoDamage,
    &daBkyRock_c::exeModeDamage,
    &daBkyRock_c::exeModePiece,
};

daBkyRock_c::daBkyRock_c() {
    mBgW = NULL;
    mMode = MODE_0;
    memset(mModels, 0, sizeof(mModels));
}

daBkyRock_c::~daBkyRock_c() {
    dComIfG_resDelete(this, l_arcName);
}

int daBkyRock_c::create() {
    fopAcM_ct(this, daBkyRock_c);
    u8 swBit0 = getSwBit0();
    if (fopAcM_isSwitch(this, swBit0)) {
        return cPhs_ERROR_e;
    }
    int rv = dComIfG_resLoad(this, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(
            this, daBkyRock_c_createHeap, 0x2000) == 0) {
            return cPhs_ERROR_e;
        }
        if (getNameNo() == 1) {
            if (dComIfG_Bgsp().Regist(mBgW, this) != 0) {
                return cPhs_ERROR_e;
            }
        }
        init();
    }
    return rv;
}

int daBkyRock_c::execute() {
    middleExe();
    return 1;
}

int daBkyRock_c::draw() {
    g_env_light.settingTevStruct( 0,
                                         &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(
        
        mModels[mMode], &tevStr);
    if (field_0x57a) {
       dMdl_c* dMdl = dMdl_mng_c::entry(mModels[2]->getModelData(), NULL, current.roomNo);
        if (dMdl != NULL) {
            _pieceData* piece = mPieces;
            for (int i = 0; i < 20; i++, piece++) {
                mDoMtx_stack_c::transS(piece->position.x, piece->position.y, piece->position.z);
                mDoMtx_stack_c::YrotM(piece->rotationX);
                mDoMtx_stack_c::ZrotM(piece->rotationZ);
                mDoMtx_stack_c::scaleM(piece->scale, piece->scale, piece->scale);
                piece->mMdlObj.setMtx(mDoMtx_stack_c::get());
                dMdl->entryObj(&piece->mMdlObj);
            }
        }
    }
    if (mMode != MODE_2) {
        mDoExt_modelUpdate(mModels[mMode]);
    }
    return 1;
}

int daBkyRock_c::Delete() {
    if (dComIfGp_getVibration().CheckQuake()) {
        dComIfGp_getVibration().StopQuake(0x1f);
    }
    if (getNameNo() == 1 && mBgW != NULL && mBgW->ChkUsed()) {
        dComIfG_Bgsp().Release(mBgW);
    }
    this->~daBkyRock_c();
    return 1;
}

void daBkyRock_c::setModelMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mModels[mMode]->setBaseTRMtx(mDoMtx_stack_c::get());
}

static int const VIBMODE_POWER[3] = {
    2, 3, 3,
};

static int const PARTICLE_SCALE[2] = {
    0x6D6, 0x8E3,
};

static int const PARTICLE_OFFSET_POS_Y[2] = {
    0x168, 0x12C,
};

static u16 const FIRST_PARTICLE_NAME[4] = {
    0x89C4, 0x89C5, 0x89C6, 0x89C7,
};

static u16 const SECOND_PARTICLE_NAME[6] = {
    0x89C2, 0x89C3, 0x89C4, 0x89C5, 0x89C6, 0x89C7,
};

dCcD_SrcCyl const daBkyRock_c::s_CcDCyl = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x20a0, 0x11}, 0x0}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x1}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {0.0f, 0.0f, 0.0f}, // mCenter
        0.0f, // mRadius
        0.0f // mHeight
    } // mCyl
};

static int const BMD_IDX[2][3] = {
    {4, 5, 8},
    {6, 7, 8},
};

int daBkyRock_c::createHeap() {
    int nameNo = getNameNo();
    for (int i = 0; i < 3; i++) {
        J3DModelData* a_model_data_p =
            (J3DModelData*)dComIfG_getObjectRes(l_arcName, BMD_IDX[nameNo][i]);
        JUT_ASSERT(481, a_model_data_p != NULL);
        mModels[i] = mDoExt_J3DModel__create(a_model_data_p, 0x80000, 0x11000084);
        if (mModels[i] == NULL) {
            return 0;
        }
    }
    if (nameNo == 1) {
        mBgW = new dBgW();
        if (mBgW == NULL) {
            return 0;
        }
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        MTXCopy(mDoMtx_stack_c::get(), mMtx);
        cBgD_t* bgD = (cBgD_t*)dComIfG_getObjectRes(l_arcName, 11);
        if (mBgW->Set(bgD, 1, &mMtx) == 1) {
            return 0;
        }
    }
    return 1;
}

static Vec const COL_OFFSET_POS[2][2] = {
    {{-100.0f, -250.0f, -100.0f}, {100.0f, -250.0f, 0.0f}},
    {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
};

static int const COL_INIT_HEIGHT[2][2] = {
    {0x190, 0x28A}, {0x384, 0x384},
};

static int const COL_INIT_RADIUS[2][2] = {
    0xFA, 0x226, 0x28A, 0x320,
};

void daBkyRock_c::init() {
    mMode = MODE_0;
    field_0x57a = false;
    int nameNo = getNameNo();
    mStts.Init(0xff, 0, this);
    for (int i = 0; i < 2; i++) {
        mCyls[i].Set(s_CcDCyl);
        mCyls[i].SetStts(&mStts);
        cXyz cylCenter(current.pos.x + COL_OFFSET_POS[nameNo][i].x,
                       current.pos.y + COL_OFFSET_POS[nameNo][i].y,
                       current.pos.z + COL_OFFSET_POS[nameNo][i].z);
        mCyls[i].SetC(cylCenter);
        mCyls[i].SetH(COL_INIT_HEIGHT[nameNo][i]);
        mCyls[i].SetR(COL_INIT_RADIUS[nameNo][i]);
    }
    mCyls[1].SetTgHitMark(CcG_Tg_UNK_MARK_9);
    field_0x579 = false;
    mVibrationTimer = 0;
    setModelMtx();
    fopAcM_SetMtx(this, mModels[mMode]->getBaseTRMtx());
    fopAcM_setCullSizeFar(this, 1.5f);
    fopAcM_setCullSizeBox(this, -800.0f, -500.0f, -800.0f, 800.0f, 1000.0f, 800.0f);
}

void daBkyRock_c::middleExe() {
    if (s_exeProc[mMode] != NULL) {
        (this->*(s_exeProc[mMode]))();
    }
}

void daBkyRock_c::exeModeNoDamage() {
    if (chkHitBombArrow()) {
        initChangeModeBefore();
        callBombEmt(4, FIRST_PARTICLE_NAME);
        fopAcM_seStartCurrent(this, Z2SE_OBJ_BOMB_ROCK_BRK_WTR_1, 0);
        mMode = MODE_1;
        initChangeModeAfter();
    }
    setModelMtx();
    for (int i = 0; i < 2; i++) {
        dComIfG_Ccsp()->Set(&mCyls[i]);
    }
}

void daBkyRock_c::exeModeDamage() {
    if (mVibrationTimer != 0) {
        mVibrationTimer--;
        if (mVibrationTimer <= 0) {
            if (dComIfGp_getVibration().CheckQuake()) {
                dComIfGp_getVibration().StopQuake(0x1f);
            }
        }
    }
    pieceMove();
    bool hitBombArrow = chkHitBombArrow();
    if (!hitBombArrow) {
        field_0x579 = true;
    }
    if (hitBombArrow && field_0x579) {
        initChangeModeBefore();
        callBombEmt(6, SECOND_PARTICLE_NAME);
        fopAcM_seStartCurrent(this, Z2SE_OBJ_BOMB_ROCK_BRK_WTR_2, 0);
        mMode = MODE_2;
        u8 swBit0 = getSwBit0();
        fopAcM_onSwitch(this, swBit0);
        initChangeModeAfter();
    }
    setModelMtx();
    for (int i = 0; i < 2; i++) {
        dComIfG_Ccsp()->Set(&mCyls[i]);
    }
}

void daBkyRock_c::exeModePiece() {
    pieceMove();
    if (mVibrationTimer != 0) {
        if (!dComIfGp_getVibration().CheckQuake()) {
            dComIfGp_getVibration().StartQuake(VIBMODE_POWER[mMode], 0x1f,
                                               cXyz(0.0f, 1.0f, 0.0f));
        }
        mVibrationTimer--;
        if (mVibrationTimer <= 0) {
            fopAcM_delete(this);
        }
    }
}

bool daBkyRock_c::chkHitBombArrow() {
    if (getNameNo() == 1) {
        u8 swBit1 = getSwBit1();
        if (!fopAcM_isSwitch(this, swBit1)) {
            return false;
        }
    }
    for (int i = 0; i < 2; i++) {
        if (i == 0 && mCyls[i].ChkTgHit()) {
            cCcD_Obj* hitObj = mCyls[i].GetTgHitObj();
            if (hitObj != NULL && hitObj->ChkAtType(AT_TYPE_BOMB)) {
                return 1;
            }
            mCyls[i].ClrTgHit();
        }
    }
    return 0;
}

static int const VIBMODE_TIMER[2] = {
    30, 40,
};

void daBkyRock_c::initChangeModeBefore() {
    dComIfGp_getVibration().StartQuake(VIBMODE_POWER[mMode], 0x1f, cXyz(0.0f, 1.0f, 0.0f));
    mVibrationTimer = VIBMODE_TIMER[mMode];
    pieceMoveInit();
}

void daBkyRock_c::initChangeModeAfter() {
    fopAcM_SetMtx(this, mModels[mMode]->getBaseTRMtx());
}

void daBkyRock_c::pieceMove() {
    if (field_0x57a) {
        f32 dVar5 = current.pos.y;
        if (fopAcM_wt_c::waterCheck(&current.pos)) {
            dVar5 = fopAcM_wt_c::getWaterY() - 150.0f;
        }
        field_0x57a = false;
        _pieceData* piece = mPieces;
        for (int i = 0; i < 20; i++, piece++) {
            cLib_addCalc(&piece->position.x, piece->targetX, 0.1f, 15.0f, 2.0f);
            cLib_addCalc(&piece->position.z, piece->targetZ, 0.1f, 15.0f, 2.0f);
            piece->speedY -= 8.0f;
            piece->position.y += piece->speedY;
            if (piece->position.y > dVar5) {
                field_0x57a = true;
            } else {
                piece->scale *= 0.9f;
                if (piece->scale > 0.05f) {
                    field_0x57a = true;
                }
            }
            // !@bug Passing an s16 pointer as a f32 pointer
            cLib_addCalc((f32*)&piece->rotationSpeed, 0.0f, 0.1f, 15.0f, 2.0f);
            piece->rotationX += piece->rotationSpeed;
            piece->rotationZ += piece->rotationSpeed;
        }
    }
}

static int const DROP_OFFSET_POS[2] = {
    0x015E, 0x0DAC,
};

static int const INIT_SPEED_Y_BASE[2] = {
    20, 40,
};

static int const INIT_SPEED_Y_RANGE[2] = {
    60, 120,
};

static int const PIECE_SCALE_RANGE[2] = {
    25, 35,
};

static int const PIECE_SCALE_BASE[2] = {
    8, 20,
};

void daBkyRock_c::pieceMoveInit() {
    int nameNo = getNameNo();
    field_0x57a = true;
    f32 speedYBase = INIT_SPEED_Y_BASE[nameNo];
    f32 speedYRange = INIT_SPEED_Y_RANGE[nameNo];
    int mode = mMode;
    _pieceData* piece = mPieces;
    for (int i = 0; i < 20; i++, piece++) {
        int local_84;
        if ((i & 1) != 0) {
            local_84 = 1;
        } else {
            local_84 = -1;
        }
        int local_88;
        if ((i & 2) != 0) {
            local_88 = 1;
        } else {
            local_88 = -1;
        }
        piece->position.x = current.pos.x + local_84 * cM_rndF(500.0f);
        piece->position.y = current.pos.y + cM_rndF(500.0f);
        piece->position.z = current.pos.z + local_88 * cM_rndF(500.0f);
        f32 dropOffset = DROP_OFFSET_POS[nameNo] + cM_rndF(800.0f);
        piece->targetX = current.pos.x + local_84 * dropOffset;
        piece->targetZ = current.pos.z + local_88 * dropOffset;
        piece->scale = 0.01f * (PIECE_SCALE_BASE[nameNo] + cM_rndF(PIECE_SCALE_RANGE[mode]));
        piece->speedY = speedYBase + cM_rndF(speedYRange);
        piece->rotationX = cM_rndF(65535.0f);
        piece->rotationZ = cM_rndF(65535.0f);
        piece->rotationSpeed = local_84 * ((s16)cM_rndF(4096.0f) + 0x2000);
    }
}

void daBkyRock_c::callBombEmt(int particleNum, u16 const* particleIds) {
    int nameNo = getNameNo();
    cXyz pos(current.pos.x, current.pos.y - PARTICLE_OFFSET_POS_Y[nameNo], current.pos.z);
    csXyz rotation(0, 0, 0);
    f32 scale = PARTICLE_SCALE[nameNo] * 0.001f;
    cXyz particleScale(scale, scale, scale);
    for (int i = 0; i < particleNum; i++) {
        dComIfGp_particle_set(particleIds[i], &pos, &rotation, &particleScale);
    }
}

static int daBkyRock_create(daBkyRock_c* i_this) {
    fopAcM_ct(i_this, daBkyRock_c);
    return i_this->create();
}

static int daBkyRock_Delete(daBkyRock_c* i_this) {
    return i_this->Delete();
}

static int daBkyRock_execute(daBkyRock_c* i_this) {
    return i_this->execute();
}

static int daBkyRock_draw(daBkyRock_c* i_this) {
    return i_this->draw();
}

static actor_method_class daBkyRock_METHODS = {
    (process_method_func)daBkyRock_create,
    (process_method_func)daBkyRock_Delete,
    (process_method_func)daBkyRock_execute,
    NULL,
    (process_method_func)daBkyRock_draw,
};

extern actor_process_profile_definition g_profile_BkyRock = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_BkyRock,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daBkyRock_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  728,                    // mPriority
  &daBkyRock_METHODS,     // sub_method
  0x00044100,             // mStatus
  fopAc_ENV_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
