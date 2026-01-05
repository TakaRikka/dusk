/**
 * @file d_a_obj_picture.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_picture.h"

static int daObjPicture_c_createHeap(fopAc_ac_c* i_this) {
    return ((daObjPicture_c*)i_this)->createHeap();
}

static char* l_arcName = "L9Picture";

static Vec l_LINE_OFFSET_POS[] = {
    {-100.0f, 200.0f, -2.0f},
    {100.0f, 200.0f, -2.0f},
};

static Vec l_PIC_AT_OFFSET_POS = {
    0.0f, 100.0f, 0.0f
};

daObjPicture_c::daObjPicture_c() {
    memset(mpBgw, 0, 8);
    mpRopestModel = NULL;
    mpPicModel = NULL;
    memset(mpRopeMat, 0, 8);
}

daObjPicture_c::~daObjPicture_c() {
    dComIfG_resDelete(this, l_arcName);
}

static const int l_dzbIdx[] = {
    11, 10,
};

static const int l_PictureBmdIdx[] = {
    6, 7,
};

int daObjPicture_c::createHeap() {
    int bmd_idx = getArg_0() & 1;

    J3DModelData* a_model_data_p = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 5);
    JUT_ASSERT(116, a_model_data_p != NULL);

    mpRopestModel = mDoExt_J3DModel__create(a_model_data_p, 0x80000, 0x11000084);
    if (mpRopestModel == NULL) {
        return 0;
    }

    cXyz pos(current.pos);
    f32 size = 1.0f;

    for (int i = 0; i < 2; i++) {
        if (i == 1 && field_0xd26 != 0) {
            break;
        }

        mpBgw[i] = new dBgW();
        if (mpBgw[i] == NULL) {
            return 0;
        }

        if (i == 0) {
            pos.set(home.pos.x, home.pos.y + 325.0f, home.pos.z);
            size = 0.5f;
        }

        mDoMtx_stack_c::transS(pos.x, pos.y, pos.z);
        mDoMtx_stack_c::ZXYrotM(home.angle.x, home.angle.y, home.angle.z);
        mDoMtx_stack_c::scaleM(size, size, size);
        MTXCopy(mDoMtx_stack_c::get(), mBgMtx[i]);

        if (mpBgw[i]->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, l_dzbIdx[i]), 1, &mBgMtx[i]) == 1) {
            return 0;
        }

        if (i == 1) {
            mpBgw[i]->SetCrrFunc(dBgS_MoveBGProc_TypicalRotY);
        }
    }

    if (field_0xd26 != 0) {
        return 1;
    }

    a_model_data_p = (J3DModelData*)dComIfG_getObjectRes(l_arcName, l_PictureBmdIdx[bmd_idx]);
    JUT_ASSERT(179, a_model_data_p != NULL);

    mpPicModel = mDoExt_J3DModel__create(a_model_data_p, 0x80000, 0x11000084);
    if (mpPicModel == NULL) {
        return 0;
    }

    for (int i = 0; i < 2; i++) {
        mpRopeMat[i] = new mDoExt_3DlineMat1_c();
        if (mpRopeMat[i] == NULL) {
            OS_REPORT_ERROR("止め具ロープ：３Ｄラインマテリアルクラス生成失敗\n");
            return 0;
        }

        if (!mpRopeMat[i]->init(1, 10, (ResTIMG*)dComIfG_getObjectRes(l_arcName, 14), 1)) {
            OS_REPORT_ERROR("止め具ロープ：３Ｄラインマテリアルクラス初期化失敗\n");
            return 0;
        }
    }

    return 1;
}

int daObjPicture_c::create() {
    fopAcM_ct(this, daObjPicture_c);

    int phase_state = dComIfG_resLoad(this, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        field_0xd26 = 0;
        if (fopAcM_isSwitch(this, getSW_0())) {
            field_0xd26 = 1;
        }

        if (!fopAcM_entrySolidHeap(this, daObjPicture_c_createHeap, 0x4000)) {
            return cPhs_ERROR_e;
        }

        for (int i = 0; i < 2; i++) {
            if (i == 1 && field_0xd26 != 0) {
                break;
            }

            if (dComIfG_Bgsp().Regist(mpBgw[i], this)) {
                return cPhs_ERROR_e;
            }
        }

        init();
        setPicModelMtx();
        setTmgModelMtx();

        J3DModel* model = field_0xd26 != 0 ? mpRopestModel : mpPicModel;
        fopAcM_SetMtx(this, model->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -100.0f, -500.0f, -500.0f, 100.0f, 1500.0f, 500.0f);

        if (field_0xd26 == 0) {
            field_0x574.Init(0xFF, 0xFF, this);

            cXyz pos;
            cXyz picpos;
            getPosTmg(&pos);

            for (int i = 0; i < 2; i++) {
                mCps[i].Set(s_CcDCps);
                mCps[i].SetStts(&field_0x574);
                getLinePosPic(i, &picpos);

                mCps[i].cCcD_CpsAttr::Set(pos, picpos, 14.0f);
                mCps[i].CalcTgVec();
                mCps[i].OnTgNoSlingHitInfSet();
            }

            field_0x5b0.Init(0xFF, 0xFF, this);
            mAtCyl.Set(s_CcDCyl_pic_at);
            mAtCyl.SetStts(&field_0x5b0);
            mAtCyl.SetC(current.pos);
            mAtCyl.SetH(10.0f);
            mAtCyl.SetR(110.0f);

            field_0x5ec.Init(0xFF, 0xFF, this);
            getPosTmg(&pos);

            Vec sp1C = {0.0f, 0.0f, -230.0f};
            mDoMtx_stack_c::transS(pos);
            mDoMtx_stack_c::ZXYrotM(shape_angle);
            mDoMtx_stack_c::multVec(&sp1C, &pos);

            for (int i = 0; i < 2; i++) {
                mCps2[i].Set(s_CcDCps);
                mCps2[i].SetStts(&field_0x5ec);
                getLinePosPic(i, &picpos);

                mDoMtx_stack_c::transS(picpos);
                mDoMtx_stack_c::ZXYrotM(shape_angle);
                mDoMtx_stack_c::multVec(&sp1C, &picpos);

                mCps2[i].cCcD_CpsAttr::Set(pos, picpos, 14.0f);
                mCps2[i].CalcTgVec();
                mCps2[i].OnTgNoSlingHitInfSet();
            }

            setLinePos();
            mpBgw[1]->OffRoofRegist();
        }
    }

    return phase_state;
}

int daObjPicture_c::Delete() {
    for (int i = 0; i < 2; i++) {
        if (i == 1 && field_0xd26 != 0) {
            break;
        }

        if (mpBgw[i] != NULL && mpBgw[i]->ChkUsed()) {
            dComIfG_Bgsp().Release(mpBgw[i]);
        }
    }

    this->~daObjPicture_c();
    return 1;
}

int daObjPicture_c::draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpRopestModel, &tevStr);
    mDoExt_modelUpdate(mpRopestModel);

    if (field_0xd26 != 0) {
        return 1;
    }

    g_env_light.setLightTevColorType_MAJI(mpPicModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpPicModel);
    dComIfGd_setList();

    for (int i = 0; i < 2; i++) {
        GXColor rope_col = {0, 0, 0, 0};
        mpRopeMat[i]->update(10, 3.5f, rope_col, 0, &tevStr);
        dComIfGd_set3DlineMat(mpRopeMat[i]);
    }

    return 1;
}

int daObjPicture_c::execute() {
    if (field_0xd26 != 0) {
        setTmgModelMtx();
        return 1;
    }

    switch (field_0xd24) {
    case 0:
        if (chkHitRope() || chkHitBombTg()) {
            fopAcM_onSwitch(this, getSW_0());
            mDoAud_seStart(Z2SE_OBJ_ROPE_PAINT_CUT, &field_0xc88, 0, 0);
            field_0xd24 = 1;
            speed.set(0.0f, -1.0f, 0.0f);

            for (int i = 0; i < 10; i++) {
                if (i == 5 || i == 6) {
                    field_0xd28[i].x = 12.0f;
                } else if (i == 0 || i == 1) {
                    field_0xd28[i].x = 4.0f;
                } else {
                    field_0xd28[i].x = 7.0f;
                }

                field_0xd28[i].y = -1.0f;
                field_0xd28[i].z = 0.0f;
            }

            return 1;
        }

        for (int i = 0; i < 2; i++) {
            dComIfG_Ccsp()->Set(&mCps[i]);
            dComIfG_Ccsp()->Set(&mCps2[i]);
        }
        break;
    case 1: {
        cXyz sp14(field_0xc94.x, current.pos.y, field_0xc94.z);
        fopAcM_gc_c::gndCheck(&sp14);
        current.pos.y += speed.y;

        moveLineFall1();

        if (fopAcM_gc_c::getGroundY() > current.pos.y) {
            mDoAud_seStart(Z2SE_OBJ_ROPE_PAINT_LAND, &current.pos, 0, 0);
            current.pos.y = 5.0f + fopAcM_gc_c::getGroundY();
            field_0xd24 = 2;
            speed.y = -0.1f;
        }

        speed.y += gravity;
        cLib_chaseF(&current.pos.x, field_0xca0.x, 1.0f);
        cLib_chaseF(&current.pos.z, field_0xca0.z, 1.0f);
        break;
    }
    case 2: {
        cXyz sp8(current.pos);
        fopAcM_gc_c::gndCheck(&sp8);

        shape_angle.x += 0x400;
        if (shape_angle.x > 0x4000) {
            shape_angle.x = 0x4000;
        }

        moveLineFall2();

        if (field_0xd27 != 0 && shape_angle.x == 0x4000) {
            field_0xd24 = 3;
        }

        cLib_chaseF(&current.pos.x, field_0xca0.x, 1.0f);
        cLib_chaseF(&current.pos.z, field_0xca0.z, 1.0f);
        setPicAtCol();
        break;
    }
    case 3:
        break;
    }

    setPicModelMtx();
    setTmgModelMtx();
    mpBgw[1]->Move();
    return 1;
}

void daObjPicture_c::init() {
    field_0xd24 = 0;
    gravity = -3.0f;

    Vec sp8 = {0.0f, 0.0f, 100.0f};

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::multVec(&sp8, &field_0xc94);

    sp8.z = 15.0f;
    mDoMtx_stack_c::multVec(&sp8, &field_0xca0);
}

void daObjPicture_c::setPicModelMtx() {
    if (field_0xd26 == 0) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
        MTXCopy(mDoMtx_stack_c::get(), mBgMtx[1]);
        mpPicModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

void daObjPicture_c::setTmgModelMtx() {
    mDoMtx_stack_c::transS(home.pos.x, home.pos.y + 325.0f, home.pos.z);
    mDoMtx_stack_c::ZXYrotM(home.angle.x, home.angle.y, home.angle.z);
    mDoMtx_stack_c::scaleM(0.5f, 0.5f, 0.5f);
    mpRopestModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daObjPicture_c::getLinePosPic(int param_0, cXyz* o_pos) {
    *o_pos = l_LINE_OFFSET_POS[param_0];
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::multVec(o_pos, o_pos);
}

void daObjPicture_c::getPosTmg(cXyz* o_pos) {
    o_pos->set(home.pos.x, 325.0f + home.pos.y, home.pos.z);
}

void daObjPicture_c::setLinePos() {
    cXyz tmgpos;
    cXyz picpos[2];
    getPosTmg(&tmgpos);
    getLinePosPic(0, &picpos[0]);
    getLinePosPic(1, &picpos[1]);

    cXyz sp44;
    cXyz sp38;
    for (int i = 0; i < 2; i++) {
        sp44 = tmgpos - picpos[i];
        sp44.normalizeZP();

        cXyz* rope_pos_p = mpRopeMat[i]->getPos(0);
        *rope_pos_p = picpos[i];
        sp38 = *rope_pos_p;

        rope_pos_p++;
        for (int j = 1; j < 9; j++) {
            *rope_pos_p = sp38 + (sp44 * 20.0f);
            sp38 = *rope_pos_p;
            rope_pos_p++;
        }

        *rope_pos_p = tmgpos;
    }
}

void daObjPicture_c::moveLineFall1() {
    cXyz picpos[2];
    getLinePosPic(0, &picpos[0]);
    getLinePosPic(1, &picpos[1]);

    cXyz sp14;
    f32 var_f31 = 1.0f;
    cXyz* rope_pos_p;
    for (int i = 0; i < 2; i++) {
        rope_pos_p = mpRopeMat[i]->getPos(0);
        *rope_pos_p = picpos[i];
        rope_pos_p++;

        if (i == 0) {
            var_f31 = -1.0f;
        } else {
            var_f31 = 1.0f;
        }

        for (int j = 1; j < 10; j++) {
            cLib_addCalc0(&field_0xd28[j].x, 0.04f, 50.0f);
            field_0xd28[j].y += 0.45f * gravity;

            sp14.set(var_f31 * field_0xd28[j].x, field_0xd28[j].y, 0.0f);
            mDoMtx_stack_c::transS(*rope_pos_p);
            mDoMtx_stack_c::ZXYrotM(shape_angle);
            mDoMtx_stack_c::multVec(&sp14, rope_pos_p);
            rope_pos_p++;
        }
    }
}

void daObjPicture_c::moveLineFall2() {
    field_0xd27 = 1;

    cXyz sp18[2];
    speed.y += gravity;

    cXyz* rope_pos_p;
    for (int i = 0; i < 2; i++) {
        getLinePosPic(i, &sp18[i]);
        rope_pos_p = mpRopeMat[i]->getPos(0);
        *rope_pos_p = sp18[i];
        rope_pos_p++;

        for (int j = 1; j < 10; j++) {
            if (8.0f + fopAcM_gc_c::getGroundY() >= rope_pos_p->y) {
                rope_pos_p->y = 8.0f + fopAcM_gc_c::getGroundY();
            } else {
                rope_pos_p->y += 0.7f * speed.y;

                f32 var_f31 = rope_pos_p->absXZ(current.pos) * (cM_ssin(0x4000 - shape_angle.x) / cM_ssin(shape_angle.x));
                var_f31 += 5.0f + fopAcM_gc_c::getGroundY();

                if (shape_angle.x >= 0x2000 && rope_pos_p->y < var_f31) {
                    rope_pos_p->y = var_f31;
                }

                cLib_addCalcPosXZ2(rope_pos_p, sp18[i], 0.02f * (10 - j), 30.0f);
                field_0xd27 = 0;
            }

            rope_pos_p++;
        }
    }
}

bool daObjPicture_c::chkHitRope() {
    for (int i = 0; i < 2; i++) {
        if (mCps[i].ChkTgHit()) {
            cCcD_Obj* tg_hit_obj = mCps[i].GetTgHitObj();
            if (tg_hit_obj != NULL && tg_hit_obj->ChkAtType(AT_TYPE_ARROW)) {
                field_0xc88 = *mCps[i].GetTgHitPosP();
                return 1;
            }

            mCps[i].ClrTgHit();
        }
    }

    return 0;
}

bool daObjPicture_c::chkHitBombTg() {
    for (int i = 0; i < 2; i++) {
        if (mCps2[i].ChkTgHit()) {
            cCcD_Obj* tg_hit_obj = mCps2[i].GetTgHitObj();
            if (tg_hit_obj != NULL && tg_hit_obj->ChkAtType(AT_TYPE_BOMB)) {
                field_0xc88 = *mCps2[i].GetTgHitPosP();
                return 1;
            }

            mCps2[i].ClrTgHit();
        }
    }

    return 0;
}

void daObjPicture_c::setPicAtCol() {
    mPicAtPos = l_PIC_AT_OFFSET_POS;
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::multVec(&mPicAtPos, &mPicAtPos);
    mAtCyl.SetC(mPicAtPos);
    dComIfG_Ccsp()->Set(&mAtCyl);
}

static int daObjPicture_create(daObjPicture_c* i_this) {
    fopAcM_ct(i_this, daObjPicture_c);
    return i_this->create();
}

static int daObjPicture_Delete(daObjPicture_c* i_this) {
    return i_this->Delete();
}

static int daObjPicture_execute(daObjPicture_c* i_this) {
    return i_this->execute();
}

static int daObjPicture_draw(daObjPicture_c* i_this) {
    return i_this->draw();
}

#if DEBUG
const
#endif
dCcD_SrcCps daObjPicture_c::s_CcDCps = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x20A0, 0x11}, 0x0}}, // mObj
        {dCcD_SE_SWORD, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x4}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 14.0f}, // mCps
    } // mCpsAttr
};

#if DEBUG
const
#endif
dCcD_SrcCyl daObjPicture_c::s_CcDCyl_pic_at = {
    {
        {0x0, {{0x400, 0x1, 0x11}, {0x0, 0x11}, 0x79}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},              // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},              // mGObjTg
        {0x0},                                           // mGObjCo
    },                                                   // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            0.0f,                // mRadius
            0.0f                 // mHeight
        }  // mCyl
    }
};

static actor_method_class daObjPicture_METHODS = {
    (process_method_func)daObjPicture_create,
    (process_method_func)daObjPicture_Delete,
    (process_method_func)daObjPicture_execute,
    0,
    (process_method_func)daObjPicture_draw,
};

actor_process_profile_definition g_profile_Obj_Picture = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Picture,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjPicture_c), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  742,                    // mPriority
  &daObjPicture_METHODS,  // sub_method
  0x00040180,             // mStatus
  fopAc_ENV_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
