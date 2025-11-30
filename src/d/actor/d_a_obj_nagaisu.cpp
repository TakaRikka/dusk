/**
 * @file d_a_obj_nagaisu.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_nagaisu.h"

static int daObjNagaisu_c_createHeap(fopAc_ac_c* i_this) {
    return ((daObjNagaisu_c*)i_this)->createHeap();
}

static char* l_arcName = "Nagaisu";

daObjNagaisu_c::daObjNagaisu_c() {}

daObjNagaisu_c::~daObjNagaisu_c() {
    dComIfG_resDelete(this, l_arcName);
}

int daObjNagaisu_c::createHeap() {
    J3DModelData* a_model_data_p = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    JUT_ASSERT(156, a_model_data_p != NULL);

    mpModel = mDoExt_J3DModel__create(a_model_data_p, J3DMdlFlag_DifferedDLBuffer, BMD_DEFAULT_DIFF_FLAGS);
    if (mpModel == NULL) {
        return 0;
    }

    daObjIsuChild_c* a_child_p = mChild;
    for (int i = 0; i < 60; i++) {
        if (!a_child_p->getDeadFlg() && !a_child_p->createHeap()) {
            return 0;
        }

        a_child_p++;
    }

    return 1;
}

const int daObjNagaisu_c::REMOVE_ISU_IDX[] = {
    6,
    7,
    11,
    14,
    18,
    21,
    29,
};

const dCcD_SrcCyl daObjIsuChild_c::s_CcDCyl = {
    {
        {0, {{0, 0, 0}, {0x400020, 0x11}, 0}},
        {dCcD_SE_NONE, 0, 0, 0, 0},
        {dCcD_SE_NONE, 0, 0, 0, 0},
        {0},
    },
    {
        {0.0f, 0.0f, 0.0f},
        0.0f,
        0.0f,
    },
};

int daObjNagaisu_c::create() {
    fopAcM_ct(this, daObjNagaisu_c);

    int phase_state = dComIfG_resLoad(this, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        current.pos.z -= 3300.0f;
        setIsu();

        if (!fopAcM_entrySolidHeap(this, daObjNagaisu_c_createHeap, 0x12000)) {
            return cPhs_ERROR_e;
        }

        init();

        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());

        daObjIsuChild_c* a_child_p = mChild;
        for (int i = 0; i < 60; i++) {
            a_child_p->create(this, &mCcStts);
            a_child_p++;
        }
    }

    return phase_state;
}

int daObjNagaisu_c::Delete() {
    daObjIsuChild_c* a_child_p = mChild;
    for (int i = 0; i < 60; i++) {
        a_child_p->Delete();
        a_child_p->setDeadFlg(true);
        a_child_p++;
    }

    this->~daObjNagaisu_c();
    return 1;
}

int daObjNagaisu_c::draw() {
    g_env_light.settingTevStruct(8, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dMdl_c* a_mdl_p = dMdl_mng_c::entry(mpModel->getModelData(), 0, current.roomNo);
    if (a_mdl_p != NULL) {
        daObjIsuChild_c* a_child_p = mChild;
        for (int i = 0; i < 60; i++) {
            a_child_p->draw(a_mdl_p);
            a_child_p++;
        }
    }

    return 1;
}

int daObjNagaisu_c::execute() {
    daObjIsuChild_c* a_child_p = mChild;
    for (int i = 0; i < 60; i++) {
        a_child_p->execute();
        a_child_p++;
    }

    return 1;
}

void daObjNagaisu_c::init() {
    mCcStts.Init(0xFF, 0, this);
}

void daObjNagaisu_c::setIsu() {
    daObjIsuChild_c* a_child_p = mChild;
    f32 start_x_pos = current.pos.x;
    cXyz prev_pos(current.pos.x, current.pos.y, current.pos.z);
    cXyz pos(prev_pos);

    int row_start_idx = 0;
    int section_start_idx = 3;
    u8 is_remove_isu = false;

    for (int i = 0; i < 60; i++) {
        if (i == row_start_idx) {
            row_start_idx += 6;
            pos.x = start_x_pos;

            // after seven rows, move farther back to leave room for the entrance way
            if (i == (6 * 7)) {
                pos.z = 600.0f + prev_pos.z;
            } else {
                pos.z = 300.0f + prev_pos.z;
            }
        } else if (i == section_start_idx) {
            section_start_idx += 6;
            pos.x = 790.0f + prev_pos.x;
            pos.z = prev_pos.z;
        } else {
            pos.x = 280.0f + prev_pos.x;
            pos.z = prev_pos.z;
        }

        prev_pos.x = pos.x;
        prev_pos.z = pos.z;

        is_remove_isu = false;
        for (int j = 0; j < 7; j++) {
            if (i == REMOVE_ISU_IDX[j]) {
                is_remove_isu = true;
                break;
            }
        }

        if (is_remove_isu) {
            a_child_p->setDeadFlg(true);
        } else {
            a_child_p->setDeadFlg(false);
            a_child_p->setPos(pos);
        }

        a_child_p++;
    }
}

daObjIsuChild_c::daObjIsuChild_c() {
    mIsDead = false;
    mpBgW = NULL;
}

daObjIsuChild_c::~daObjIsuChild_c() {}

int daObjIsuChild_c::createHeap() {
    mpBgW = new dBgW();
    if (mpBgW == NULL) {
        return 0;
    }

    mDoMtx_stack_c::transS(mPos.x, mPos.y, mPos.z);
    mDoMtx_stack_c::YrotM(-0x8000);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);

    if (mpBgW->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, 7), 1, &mBgMtx) == true) {
        return 0;
    }

    return 1;
}

void daObjIsuChild_c::create(daObjNagaisu_c* i_parent, dCcD_Stts* i_ccStts) {
    static const int COL_OFFSET_X[] = {
        60,
        -60,
    };

    if (!mIsDead) {
        for (int i = 0; i < 2; i++) {
            mCcCyl[i].Set(s_CcDCyl);
            mCcCyl[i].SetStts(i_ccStts);

            cXyz center(mPos.x + COL_OFFSET_X[i], mPos.y, mPos.z + -50.0f);
            mCcCyl[i].SetC(center);
            mCcCyl[i].SetH(180.0f);
            mCcCyl[i].SetR(65.0f);
        }

        if (dComIfG_Bgsp().Regist(mpBgW, i_parent)) {
            return;
        }
    }
}

int daObjIsuChild_c::execute() {
    if (!mIsDead) {
        if (chkHit()) {
            callEmt();
            mDoAud_seStart(Z2SE_OBJ_BREAK_WOOD_FURNITURE, &mPos, 0, 0);
            Delete();
            mIsDead = true;
        } else {
            for (int i = 0; i < 2; i++) {
                dComIfG_Ccsp()->Set(&mCcCyl[i]);
            }
        }
    }
}

int daObjIsuChild_c::draw(dMdl_c* i_mdl) {
    if (!mIsDead) {
        mDoMtx_stack_c::transS(mPos.x, mPos.y, mPos.z);
        mDoMtx_stack_c::YrotM(-0x8000);
        mMdlObj.setMtx(mDoMtx_stack_c::get());
        i_mdl->entryObj(&mMdlObj);
    }
}

void daObjIsuChild_c::Delete() {
    if (!mIsDead) {
        if (mpBgW != NULL && mpBgW->ChkUsed()) {
            dComIfG_Bgsp().Release(mpBgW);
        }
    }
}

bool daObjIsuChild_c::chkHit() {
    for (int i = 0; i < 2; i++) {
        if (mCcCyl[i].ChkTgHit()) {
            return true;
        }
    }

    return false;
}

void daObjIsuChild_c::callEmt() {
    cXyz pos(mPos.x, mPos.y, mPos.z);
    csXyz rot(0, -0x8000, 0);
    cXyz size(1.25f, 1.25f, 1.25f);

    for (int i = 0; i < 4; i++) {
        static const u16 PARTICLE_NAME[] = {
            dPa_RM(ID_ZI_S_LV5KAGU_BREAK_A),
            dPa_RM(ID_ZI_S_LV5KAGU_BREAK_B),
            dPa_RM(ID_ZI_S_LV5KAGU_BREAK_C),
            dPa_RM(ID_ZI_S_LV5KAGU_BREAK_D),
        };

        dComIfGp_particle_set(PARTICLE_NAME[i], &pos, &rot, &size);
    }
}

static int daObjNagaisu_create(daObjNagaisu_c* i_this) {
    fopAcM_ct(i_this, daObjNagaisu_c);
    return i_this->create();
}

static int daObjNagaisu_Delete(daObjNagaisu_c* i_this) {
    return i_this->Delete();
}

static int daObjNagaisu_execute(daObjNagaisu_c* i_this) {
    return i_this->execute();
}

static int daObjNagaisu_draw(daObjNagaisu_c* i_this) {
    return i_this->draw();
}

static actor_method_class daObjNagaisu_METHODS = {
    (process_method_func)daObjNagaisu_create,
    (process_method_func)daObjNagaisu_Delete,
    (process_method_func)daObjNagaisu_execute,
    0,
    (process_method_func)daObjNagaisu_draw,
};

extern actor_process_profile_definition g_profile_Obj_Nagaisu = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_Obj_Nagaisu,        // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daObjNagaisu_c),  // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  739,                     // mPriority
  &daObjNagaisu_METHODS,   // sub_method
  0x00040000,              // mStatus
  fopAc_ENV_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
