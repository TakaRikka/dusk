#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tbox.h"
#include "d/d_tresure.h"
#include "d/d_item_data.h"
#include "d/actor/d_a_midna.h"
#include "d/d_path.h"
#include "d/d_bg_w.h"
#include "f_op/f_op_camera_mng.h"
#include "SSystem/SComponent/c_math.h"
#include "Z2AudioLib/Z2Instances.h"
#include <cmath.h>

static const f32 l_cull_size_box[6] = { -150.0f, -10.0f, -150.0f, 150.0f, 300.0f, 100.0f };

static const cM3dGCylS l_cyl_info[3] = {
    { 0.0f, 0.0f, -40.0f, 40.0f, 60.0f },
    { 0.0f, 0.0f, -60.0f, 50.0f, 70.0f },
    { 0.0f, 0.0f, -60.0f, 70.0f, 80.0f },
};

static char* l_staff_name = "TREASURE";

static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x1e}, {0x0, 0x0}, 0x79}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_METAL, 0x2, 0x0, 0x0, 0x3}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {0.0f, 0.0f, 0.0f}, // mCenter
        0.0f, // mRadius
        0.0f // mHeight
    } // mCyl
};

static daTbox_ModelInfo l_modelInfo[3] = {
    { "Dalways", 12, 7, 25, 26, 14, 19 },
    { "Dalways", 13, 8, 27, 28, 14, 19 },
    { "BoxC", 14, 8, 20, 21, 11, 17 },
};

static cXyz l_light_offset(0.0f, 35.0f, 0.0f);

class daTbox_HIO_c : public mDoHIO_entry_c {
public:
    daTbox_HIO_c();
    virtual ~daTbox_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ u8 mItemNo;
    /* 0x05 */ u8 mUseDebugItemNo;
    /* 0x06 */ u8 mClearSwitch;
    /* 0x07 */ u8 mDemoType;
    /* 0x08 */ u8 mDebugLightColorEnabled;
    /* 0x09 */ u8 mTimerDisplay;
    /* 0x0A */ u8 mCheckDisplay;
    /* 0x0B */ u8 mSaveOff;
    /* 0x0C */ f32 mGravity;
    /* 0x10 */ cXyz mLightOffset;
    /* 0x1C */ s16 mLightColorR;
    /* 0x1E */ s16 mLightColorG;
    /* 0x20 */ s16 mLightColorB;
    /* 0x24 */ f32 mLightPower;
    /* 0x28 */ f32 mFluctuationRate;
    /* 0x2C */ f32 mEnvColorInfluence;
    /* 0x30 */ s16 mFluctuationSpeed;
    /* 0x32 */ u8 mForPlacementCheck;
};

STATIC_ASSERT(sizeof(daTbox_HIO_c) == 0x34);

daTbox_HIO_c::daTbox_HIO_c() {
    mItemNo = 1;
    mUseDebugItemNo = 0;
    mGravity = -2.0f;
    mDemoType = 0;
    mTimerDisplay = 0;
    mCheckDisplay = 0;
    mClearSwitch = 0;
    mSaveOff = 0;
    mDebugLightColorEnabled = 0;
    mLightColorR = 0;
    mLightColorG = 0;
    mLightColorB = 0;
    mLightOffset = l_light_offset;
    mLightPower = 70.0f;
    mFluctuationRate = 0.3f;
    mFluctuationSpeed = 7500;
    mEnvColorInfluence = 0.55f;
    mForPlacementCheck = 0;
}

daTbox_ModelInfo* daTbox_c::getModelInfo() {
    return &l_modelInfo[getShapeType()];
}

static const u32 l_open_se_id[3] = { Z2SE_OBJ_TBOX_OPEN_A, Z2SE_OBJ_TBOX_OPEN_B, Z2SE_OBJ_TBOX_OPEN_C };

cPhs__Step daTbox_c::commonShapeSet() {
    daTbox_ModelInfo* model_info = getModelInfo();

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(model_info->mArcName, model_info->mModelResNo);
    JUT_ASSERT(0x191, modelData != NULL);

    mpAnm = new mDoExt_bckAnm();
    if (mpAnm == NULL) {
        return cPhs_ERROR_e;
    }

    J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(model_info->mArcName, model_info->mBckResNo);
    if (!mpAnm->init(bck, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false)) {
        return cPhs_ERROR_e;
    }

    mOpenSeId = l_open_se_id[getShapeType()];
    u32 model_flag = 0x80000;
    if (getShapeType() == SHAPE_BOSSKEY) {
        model_flag = 0;
    }

    mpModel = mDoExt_J3DModel__create(modelData, model_flag, 0x11000084);
    if (mpModel == NULL) {
        return cPhs_ERROR_e;
    }

    if (!strcmp(dComIfGp_getStartStageName(), "D_MN01B")) {  // Deku Toad chest
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Dalways", 15);
        JUT_ASSERT(0x1BD, modelData != NULL);
        mpSlimeModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (mpSlimeModel == NULL) {
            return cPhs_ERROR_e;
        }
    }

    if (checkAppear()) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(model_info->mArcName, model_info->mEffectResNo);
        JUT_ASSERT(0x1CE, modelData != NULL);
        mpEffectModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (mpEffectModel == NULL) {
            return cPhs_ERROR_e;
        }

        mpEffectAnm = new mDoExt_brkAnm();
        if (mpEffectAnm == NULL) {
            return cPhs_ERROR_e;
        }

        J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(model_info->mArcName, model_info->mBrkResNo);
        JUT_ASSERT(0x1E1, brk != NULL);
        if (!mpEffectAnm->init(modelData, brk, TRUE, J3DFrameCtrl::EMode_NONE, 0.0f, 0, -1)) {
            return cPhs_ERROR_e;
        }
    }

    mpModel->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(home.angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    if (mpEffectModel != NULL) {
        mpEffectModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    mDoMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
    return cPhs_COMPLEATE_e;
}

cPhs__Step daTbox_c::effectShapeSet() {
    return cPhs_COMPLEATE_e;
}

cPhs__Step daTbox_c::envShapeSet() {
    return cPhs_COMPLEATE_e;
}

cPhs__Step daTbox_c::bgCheckSet() {
    daTbox_ModelInfo* model_info = getModelInfo();

    cBgD_t* bgd = (cBgD_t*)dComIfG_getObjectRes(model_info->mArcName, model_info->mOpenDzbResNo);
    JUT_ASSERT(528, bgd != NULL);
    mpOpenBgW = new dBgW();
    if (mpOpenBgW == NULL) {
        return cPhs_ERROR_e;
    }

    if (mpOpenBgW->Set(bgd, 1, &mBgMtx) == 1) {
        return cPhs_ERROR_e;
    }

    mpOpenBgW->SetCrrFunc(dBgS_MoveBGProc_TypicalRotY);
    mpOpenBgW->Move();
    mpBgCollision = NULL;
    return cPhs_COMPLEATE_e;
}

void daTbox_c::lightReady() {
    setLightPos();
    mLight.mColor.r = 0;
    mLight.mColor.g = 0;
    mLight.mColor.b = 0;
    mLight.mPow = 0.0f;
    mLight.mFluctuation = 0.0f;
    mAllcolRatio = 0.0f;
}

void daTbox_c::setLightPos() {
    cXyz offset(l_light_offset);
    cXyz pos;
    cLib_offsetPos(&pos, &current.pos, current.angle.y, &offset);
    mLight.mPosition.set(pos.x, pos.y, pos.z);
}

int daTbox_c::checkEnv() {
    return true;
}

int daTbox_c::checkAppear() {
    if (getFuncType() == 2 || (getSwType() == 15 && getFuncType() == 1)) {
        return true;
    }
    return false;
}

int daTbox_c::checkOpen() {
    return dComIfGs_isTbox(getTboxNo());
}

void daTbox_c::clrDzb() {
    if (mpBgCollision != NULL) {
        dComIfG_Bgsp().Release(mpBgCollision);
        mpBgCollision = NULL;
    }
}

void daTbox_c::setDzb() {
    clrDzb();

    if (checkOpen()) {
        mpBgCollision = mpOpenBgW;
        // "Opened Check Set\n"
        OS_REPORT("開いたチェックセット\n");
    } else {
        mpBgCollision = mpBgW;
        // "Closed Check Set\n"
        OS_REPORT("閉じたチェックセット\n");
    }

    bool rt = dComIfG_Bgsp().Regist(mpBgCollision, this);
    JUT_ASSERT(0x285, !rt);
}

void daTbox_c::surfaceProc() {
    if (mpBgCollision != NULL && flagCheck(0x20)) {
        if (field_0x750 < -1.0f) {
            field_0x750 += 1.0f;
        } else {
            flagOff(0x20);
            field_0x750 = 0.0f;
        }

        mDoMtx_stack_c::transS(current.pos.x, current.pos.y + field_0x750, current.pos.z);
        mDoMtx_stack_c::YrotM(home.angle.y);
        MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
        mpBgCollision->Move();
    }
}

int daTbox_c::checkNormal() {
    int func_type = getFuncType();
    if (func_type == 0 || func_type == 6 || func_type == 5 || func_type == 3) {
        return true;
    }

    if (getSwNo() >= 0xC0) {
        return false;
    }

    if (dComIfGs_isSwitch(getSwNo(), fopAcM_GetRoomNo(this))) {
        return true;
    } else {
        return false;
    }
}

int daTbox_c::checkEnvEffectTbox() {
    if (getShapeType() == SHAPE_SMALL) {
        return false;
    }

    if (getItemNo() == fpcNm_ITEM_BOOMERANG || getItemNo() == fpcNm_ITEM_MAP || getItemNo() == fpcNm_ITEM_COMPUS) {
        return true;
    }

    if (getShapeType() == SHAPE_BOSSKEY) {
        return true;
    }

    return dItem_data::chkFlag(getItemNo(), 0x40);
}

u32 daTbox_c::calcHeapSize() {
    int shape_type = getShapeType();
    checkOpen();
    checkAppear();

    u32 heap_size = 0;
    switch (shape_type) {
        case SHAPE_SMALL:
            heap_size = 0x1840;
            break;
        case SHAPE_LARGE:
            heap_size = 0x2040;
            break;
        case SHAPE_BOSSKEY:
            heap_size = 0x3060;
            break;
    }

    return heap_size;
}

int daTbox_c::CreateHeap() {
    cPhs__Step step = commonShapeSet();
    if (step != cPhs_COMPLEATE_e) {
        return false;
    }

    if (checkEnv()) {
        step = envShapeSet();
        if (step != cPhs_COMPLEATE_e) {
            return false;
        }
    }

    if (!checkOpen()) {
        step = effectShapeSet();
        if (step != cPhs_COMPLEATE_e) {
            return false;
        }
    }

    step = bgCheckSet();
    if (step != cPhs_COMPLEATE_e) {
        return false;
    }

    return true;
}

void daTbox_c::CreateInit() {
    int func_type = getFuncType();
    flagClr();

    mpAnm->setPlaySpeed(0.0f);
    initPos();
    initAnm();

    shape_angle.z = 0;
    shape_angle.x = 0;
    current.angle.z = 0;
    current.angle.x = 0;

    mStts.Init(0xff, 0xff, this);
    mCyl.Set(l_cyl_src);
    mCyl.SetStts(&mStts);
    mCyl.cM3dGCyl::Set(l_cyl_info[getShapeType()]);

    if (mpBgW != NULL) {
        dComIfG_Bgsp().Release(mpBgW);
    }

    if (flagCheck(2)) {
        clrDzb();
    } else if (mpBgCollision == NULL) {
        setDzb();
    }

    if (func_type == 5 || func_type == 6 || field_0x9cc == 1) {
        mAcchCir.SetWall(500.0f, 0.0f);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), &current.angle, &shape_angle);
        gravity = -2.0f;
        field_0x97d = true;
        field_0x97c = true;
    } else if (func_type == 3) {
        mAcchCir.SetWall(500.0f, 0.0f);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), NULL, NULL);
    }

    mTboxNo = getTboxNo();
    mEventId = dComIfGp_getEventManager().getEventIdx(this, getEvent());

    if (getShapeType() == SHAPE_BOSSKEY) {
        eventInfo.setArchiveName(getModelInfo()->mArcName);
        field_0x984 = dComIfGp_getEventManager().getEventIdx(this, "DEFAULT_TREASURE_BOSS", 0xff);
    }

    initBaseMtx();

    lightReady();
    mAllcolRatio = 1.0f;

    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, l_cull_size_box[0], l_cull_size_box[1], l_cull_size_box[2],
                          l_cull_size_box[3], l_cull_size_box[4], l_cull_size_box[5]);
}

void daTbox_c::initPos() {
    int func_type = getFuncType();
    if ((func_type == 6 && getSwType() == 15) || func_type == 7) {
        dPath* path_p = dPath_GetRoomPath(getPathId(), -1);
        JUT_ASSERT(0x3B7, path_p != NULL);

        if (func_type == 6 && path_p->m_num != 2 && path_p->m_num != 3) {
            fopAcM_setWarningMessage(this, __FILE__, 0x3BD, "Num of Jump TBox's path point must be 2 or 3!");
        }

        if (dComIfGs_isSwitch(getSwNo(), fopAcM_GetRoomNo(this))) {
            dPnt* pnt = &path_p->m_points[path_p->m_num - 1];
            current.pos = pnt->m_position;
            home.pos = pnt->m_position;
        }
    } else if (func_type == 6 && getSwType() == 0 && checkDrop()) {
        cXyz pos;
        if (!calcJumpGoalAndAngle(&pos, &current.angle.y)) {
            // "Jump Treasure Chest: Failed to get Jump Goal!\n"
            OS_REPORT_ERROR("ジャンプ宝箱：ジャンプ先の取得に失敗しました！\n");
        }

        current.pos = pos;
        home.pos = pos;
        getDropSAngle(&home.angle.y);
    }

    cXyz start = home.pos;
    cXyz end = home.pos;
    start.y += 5.0f;
    end.y -= 30.0f;

    bool line_check = fopAcM_lc_c::lineCheck(&start, &end, this);
    if ((line_check && fopAcM_lc_c::checkMoveBG()) || (!line_check && func_type != 3)) {
        // "Treasure Chest position initialization: No BG within 25cm, waiting for MoveBG!\n"
        OS_REPORT("宝箱位置初期設定：２５ｃｍ以内にＢＧが無いのでMoveBGを待ちます！\n");
        field_0x9cc = 1;
        mMode = MODE_EXEC_WAIT;
    } else {
        // "Treasure Chest position initialization complete!\n"
        OS_REPORT("宝箱位置初期設定完了！\n");
        field_0x9cc = 0;
        mMode = MODE_EXEC;
    }
}

void daTbox_c::initAnm() {
    int func_type = getFuncType();

    if (checkOpen()) {
        mpAnm->setFrame(mpAnm->getEndFrame());
        if (func_type == 3) {
            cXyz pos = current.pos;
            if (fopAcM_gc_c::gndCheck(&pos)) {
                current.pos.y = fopAcM_gc_c::getGroundY();
                shape_angle.z = 0;
                shape_angle.x = 0;
            }

            home.pos = current.pos;
            attention_info.position = current.pos;
            eyePos = current.pos;
        }

        setAction(&daTbox_c::actionWait);
        int tbox_no = getTboxNo();
        dTres_c::offStatus(0, tbox_no, 1);
    } else if (!checkEnv()) {
        setAction(&daTbox_c::actionOpenWait);
    } else if (checkNormal()) {
        if (func_type == 6 && !checkDrop()) {
            setAction(&daTbox_c::actionDropWait);
        } else if (func_type == 3) {
            if (fopAcM_isSwitch(this, getSwNo())) {
                cXyz pos = current.pos;
                if (fopAcM_gc_c::gndCheck(&pos)) {
                    current.pos.y = fopAcM_gc_c::getGroundY();
                    shape_angle.z = 0;
                    shape_angle.x = 0;
                }

                home.pos = current.pos;
                attention_info.position = current.pos;
                eyePos = current.pos;
                setAction(&daTbox_c::actionOpenWait);
            } else {
                setAction(&daTbox_c::actionDropWaitForWeb);
                flagOn(2);
                field_0x9fd = 1;
            }
        } else if (func_type == 0) {
            setAction(&daTbox_c::actionOpenWait);
        } else {
            setAction(&daTbox_c::actionOpenWait);
        }

        int tbox_no = getTboxNo();
        dTres_c::onStatus(0, tbox_no, 1);
    } else {
        int tbox_no = getTboxNo();
        dTres_c::offStatus(0, tbox_no, 1);

        switch (func_type) {
        case 2:
            setAction(&daTbox_c::actionGenocide);
            mTimer = 65;
            flagOn(3);
            field_0x75a = 120;
            break;
        case 1:
            if (getSwType() == 15) {
                setAction(&daTbox_c::actionSwOnWait);
            } else {
                setAction(&daTbox_c::actionSwOnWait2);
            }
            field_0x75a = 120;
            flagOn(3);
            break;
        }
    }
}

int daTbox_c::boxCheck() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    if (!strcmp(dComIfGp_getStartStageName(), "R_SP01") && fopAcM_GetRoomNo(this) == 7 &&
                                                           player->getKandelaarFlamePos() == NULL) {
        return false;
    }

    cXyz vec = player->attention_info.position - current.pos;
    f32 dist2_xz = vec.abs2XZ();
    f32 dist_y = fabsf(player->current.pos.y - current.pos.y);
    if (dist2_xz < 22500.0f && fopAcM_seenPlayerAngleY(this) < 0x2000 &&
                               fopAcM_seenActorAngleY(player, this) < 0x2000 && dist_y < 20.0f) {
        return true;
    } else {
        return false;
    }
}

void daTbox_c::demoProcOpen() {
    if (mDemoFrame < 1000) {
        mDemoFrame++;
    }

    if (mDemoFrame == 152) {
        dKy_efplight_cut(&mLight);
    } else if (mDemoFrame > 112) {
        lightDownProc();
    } else if (mDemoFrame > 2) {
        lightUpProc();
    }

    if (mDemoFrame > 22) {
        mLight.mPow = cM_ssin(mDemoFrame * 7500) * 21.0f + 70.0f;
    }

    lightColorProc();
    environmentProc();
}

void daTbox_c::lightColorProc() {
    static int const key_frame[7] = {0x00, 0x07, 0x1e, 0x25, 0x2c, 0x5c, 0x8c};
    static GXColorS10 const key_color[7] = {
        {0x00, 0x00, 0x00, 0xff},
        {0xff, 0xc3, 0x8a, 0xff},
        {0xff, 0xc3, 0x8a, 0xff},
        {0x37, 0x34, 0x24, 0xff},
        {0xff, 0xc3, 0x8a, 0xff},
        {0xff, 0xc3, 0x8a, 0xff},
        {0x00, 0x00, 0x00, 0xff},
    };

    int var_r25, var_r24, var_r28, var_r30;
    if (mDemoFrame < 140) {
        for (var_r30 = 0; var_r30 < 7; var_r30++) {
            if (key_frame[var_r30] <= mDemoFrame && key_frame[var_r30 + 1] > mDemoFrame) {
                var_r25 = key_frame[var_r30];
                var_r24 = key_frame[var_r30 + 1];
                var_r28 = var_r30 + 1;
                break;
            }
        }

        int num_frames = var_r24 - var_r25;
        GXColorS10 color;
        color.r = std::fabs<int>(key_color[var_r30 + 1].r - key_color[var_r30].r) / num_frames;
        color.g = std::fabs<int>(key_color[var_r30 + 1].g - key_color[var_r30].g) / num_frames;
        color.b = std::fabs<int>(key_color[var_r30 + 1].b - key_color[var_r30].b) / num_frames;
        color.r++;
        color.g++;
        color.b++;

        cLib_chaseS(&mLight.mColor.r, key_color[var_r28].r, color.r);
        cLib_chaseS(&mLight.mColor.g, key_color[var_r28].g, color.g);
        cLib_chaseS(&mLight.mColor.b, key_color[var_r28].b, color.b);
    }
}

void daTbox_c::environmentProc() {
    if (mDemoFrame > 139) {
        cLib_addCalc(&mAllcolRatio, 1.0f, 0.05f, 0.1f, 0.05f);
    } else if (mDemoFrame > 169) {
        mAllcolRatio = 1.0f;
    }
}

void daTbox_c::lightUpProc() {
    cLib_addCalc(&mLight.mPow, 70.0f, 0.05f, 5.0f, 1.0f);
}

void daTbox_c::lightDownProc() {
    cLib_addCalc0(&mLight.mPow, 0.05f, 5.0f);
}

void daTbox_c::dropProcInitCall() {
    switch (getSwType()) {
    case 0:
        dropProcInit2();
        break;
    case 15:
        dropProcInit();
        break;
    }
}

void daTbox_c::dropProcInit() {
    dPath* path_p = dPath_GetRoomPath(getPathId(), -1);
    JUT_ASSERT(0x56A, path_p != NULL);

    cXyz pos = current.pos;
    cXyz pnt1 = path_p->m_points[1].m_position;

    f32 var_f30;
    if (path_p->m_num == 2) {
        field_0x97c = false;

        f32 delta_y = pos.y - pnt1.y;
        f32 abs_gravity = std::fabsf(fopAcM_GetGravity(this));

        var_f30 = JMAFastSqrt(2.0f * delta_y / abs_gravity);
        speedF = pos.absXZ(pnt1) / var_f30;
        setRotAxis(&pos, &pnt1);
    } else if (path_p->m_num == 3) {
        cXyz pnt2(path_p->m_points[2].m_position);
        if (pos.y < pnt2.y) {
            OS_REPORT_ERROR("落下開始点より落下点の方が上にあります！！！\n");
            JUT_ASSERT(1423, FALSE);
        }

        f32 delta_y1 = pos.y - pnt2.y;
        f32 delta_y2 = pnt1.y - pos.y;
        f32 abs_gravity = fabsf(fopAcM_GetGravity(this));
        f32 dist_xz = pos.absXZ(pnt2);

        speedF = dist_xz * JMAFastSqrt(abs_gravity) /
            (JMAFastSqrt(2.0f * delta_y2) + JMAFastSqrt(2.0f * (delta_y1 + delta_y2)));
        speed.y = JMAFastSqrt(2.0f * abs_gravity * delta_y2);
        setRotAxis(&pos, &pnt1);
        var_f30 = JMAFastSqrt(2.0f / abs_gravity) *
            (JMAFastSqrt(delta_y1) + JMAFastSqrt(delta_y1 + delta_y2));
    }

    field_0x9c8 = var_f30 / 19.0f;
}

int daTbox_c::calcJumpGoalAndAngle(cXyz* i_pos, s16* i_angle) {
    dPath* path_p = dPath_GetRoomPath(getPathId(), -1);
    if (path_p != NULL) {
        cXyz home_pos = home.pos;
        cXyz vec2 = path_p->m_points[1].m_position;
        f32 dist_xz = home_pos.absXZ(vec2);

        s16 angle;
        getDropSAngle(&angle);

        cXyz goal(0.0f, 0.0f, dist_xz);
        mDoMtx_stack_c::YrotS(angle);
        mDoMtx_stack_c::multVec(&goal, &goal);
        goal += home_pos;
        goal.y = vec2.y;

        *i_pos = goal;
        *i_angle = angle;
        return true;
    }

    return false;
}

bool daTbox_c::getDropSAngle(s16* i_angle) {
    if (getFuncType() == 6 && getSwType() == 0 && getDir() != 0) {
        *i_angle = cM_deg2s((getDir() - 1) * 24 + 12);
        return true;
    }

    return false;
}

int daTbox_c::getDir() {
    if (getSwType() == 15) {
        if (dComIfGs_isSwitch(getSwNo(), fopAcM_GetRoomNo(this))) {
            return 1;
        } else {
            return 0;
        }
    } else {
        u16 bit = 1;
        u16 dir = 0;
        for (int i = 0; i < 4; i++) {
            if (fopAcM_isSwitch(this, getSwNo() + i)) {
                dir += bit;
            }
            bit = bit << 1;
        }
        return dir;
    }
}

void daTbox_c::setRotAxis(cXyz const* i_pos, cXyz const* i_goalPos) {
    mRotAxis = *i_goalPos - *i_pos;
    mRotAxis.y = 0.0f;
    mDoMtx_stack_c::YrotS(0x4000);
    mDoMtx_stack_c::multVec(&mRotAxis, &mRotAxis);
}

void daTbox_c::dropProcInit2() {
    cXyz goal_pos;
    if (!calcJumpGoalAndAngle(&goal_pos, &current.angle.y)) {
        // "Jump Treasure Chest: Failed to get jump goal!\n"
        OSReport_Error("ジャンプ宝箱：ジャンプ先の取得に失敗しました！\n");
    }

    dPath* path_p = dPath_GetRoomPath(getPathId(), -1);
    f32 temp;
    if (path_p->m_num == 2) {
        cXyz vec1 = current.pos;
        cXyz vec2 = path_p->m_points[1].m_position;
        field_0x97c = false;
        f32 delta_y = vec1.y - vec2.y;
        f32 abs_gravity = fabsf(fopAcM_GetGravity(this));
        temp = JMAFastSqrt(2.0f * delta_y / abs_gravity);
        speedF = vec1.absXZ(vec2) / temp;
        setRotAxis(&vec1, &goal_pos);
    } else {
        // "Falling Memory Treasure Chest: There's not 2 points!\n"
        OS_REPORT_ERROR("落下方向記憶宝箱：ポイントが２点ではありません！\n");
        JUT_ASSERT(1601, FALSE);
    }

    field_0x9c8 = temp / 19.0f;
}

void daTbox_c::dropProc() {
    if (field_0x9c8 != 0) {
        s16 temp = field_0x9c6;
        field_0x9c4 = 3400;
        field_0x9c6 += field_0x9c4;
        if (temp < 0 && field_0x9c6 > 0) {
            field_0x9c8--;
        }
    } else if (field_0x9c8 == 0) {
        field_0x9c4 = 0;
        mDoMtx_identity(field_0x988);
    }

    if (mAcch.ChkGroundLanding() && field_0x97d) {
        field_0x9c8 = 0;
        field_0x9c4 = 0;
        mDoMtx_identity(field_0x988);
        speedF = 0.0f;

        cXyz vec1(2.0f, 2.0f, 2.0f);
        dComIfGp_particle_setPolyColor(0xe7, mAcch.m_gnd, &current.pos, &tevStr, &home.angle,
                                       &vec1, 0, NULL, fopAcM_GetRoomNo(this), NULL);
        dComIfGp_getVibration().StartShock(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));

        u32 sound_id = 0;
        cXyz chkpos = current.pos;
        chkpos.y += 10.0f;

        dBgS_ObjGndChk gnd_chk;
        gnd_chk.SetPos(&chkpos);
        gnd_chk.SetActorPid(base.base.id);
        dComIfG_Bgsp().GroundCross(&gnd_chk);

        int bg_index = gnd_chk.GetBgIndex();
        if (bg_index >= 0 && bg_index < 0x100) {
            sound_id = dComIfG_Bgsp().GetMtrlSndId(gnd_chk);
        }

        mDoAud_seStart(JA_SE_OBJ_BLOCK_FALL_NORMAL, &eyePos, sound_id, dComIfGp_getReverb(home.roomNo));
    }

    if (home.pos.abs(current.pos) > 400.0f) {
        field_0x97c = true;
        field_0x97d = true;
    }
}

void daTbox_c::demoInitAppear() {
    mpEffectAnm->setFrame(0.0f);
    mpEffectAnm->setPlaySpeed(1.0f);

    cXyz particle_pos(current.pos.x, current.pos.y + 55.0f, current.pos.z);
    csXyz particle_rot = home.angle;

    static u16 const eff_id[6] = {0x8840, 0x8841, 0x8842, 0x8843, 0x8844, 0x8845};
    for (u32 i = 0; i < 6; i++) {
        dComIfGp_particle_set(eff_id[i], &particle_pos, &particle_rot, &scale, 0xff, NULL, -1, NULL, NULL, NULL);
    }

    flagOff(0x40);
    flagOn(1);
    fopAcM_seStart(this, Z2SE_OBJ_T_BOX_EMERGE, 0);
}

void daTbox_c::demoProcAppear() {
    if (field_0x75a != 0) {
        field_0x75a--;
    }

    if (mpEffectAnm->getFrame() == 110.0f) {
        flagOff(1);
    }

    if (mpEffectAnm->play()) {
        mpEffectAnm->setPlaySpeed(0.0f);
        dComIfGp_evmng_cutEnd(mStaffId);
    }
}

int daTbox_c::demoProc() {
    static char* action_table[] = {"WAIT", "OPEN", "APPEAR", "OPEN_SHORT", "DROP"};
    int act_idx = dComIfGp_evmng_getMyActIdx(mStaffId, action_table, ARRAY_SIZEU(action_table), 0, 0);
    int tbox_no;

    if (dComIfGp_evmng_getIsAddvance(mStaffId)) {
        field_0x758 = false;
        switch (act_idx) {
        case 1:
            OpenInit();
            tbox_no = getTboxNo();
            dTres_c::offStatus(0, tbox_no, 1);
            break;
        case 2:
            flagOn(0x20);
            field_0x750 = -130.0f;
            setDzb();
            flagOff(2);
            tbox_no = getTboxNo();
            dTres_c::onStatus(0, tbox_no, 1);
            demoInitAppear();
            break;
        case 3:
            OpenInit_com();
            tbox_no = getTboxNo();
            dTres_c::offStatus(0, tbox_no, 1);
            break;
        case 4:
            field_0x9ca = 7;
        }
    }

    switch (act_idx) {
    case 2:
        demoProcAppear();
        surfaceProc();
        break;
    case 1:
        if (field_0x758) {
            dComIfGp_evmng_cutEnd(mStaffId);
        } else {
            if (field_0x97e != 0) {
                mpAnm->setFrame(daPy_getPlayerActorClass()->getBaseAnimeFrame());
            }

            mpAnm->play();

            if (mpAnm->getFrameCtrl()->checkState(1)) {
                field_0x758 = true;
                dComIfGp_evmng_cutEnd(mStaffId);
            }
        }

        field_0x97e++;
        break;
    case 3:
        if (field_0x758) {
            dComIfGp_evmng_cutEnd(mStaffId);
        } else {
            if (field_0x97e != 0) {
                mpAnm->setFrame(daPy_getPlayerActorClass()->getBaseAnimeFrame());
            }

            mpAnm->play();

            if (mpAnm->getFrameCtrl()->checkState(1)) {
                field_0x758 = true;
                dComIfGp_evmng_cutEnd(mStaffId);
            }
        }

        field_0x97e++;
        break;
    case 4:
        if (field_0x9ca > -2) {
            field_0x9ca--;
        }

        if (field_0x9ca == 0) {
            dropProcInitCall();
        } else if (field_0x9ca < 0) {
            dropProc();
            if (mAcch.ChkGroundHit() && field_0x97d) {
                dComIfGp_evmng_cutEnd(mStaffId);
            }
        }
        break;
    default:
        dComIfGp_evmng_cutEnd(mStaffId);
        break;
    }

    if (flagCheck(0x10)) {
        demoProcOpen();
    }

    if (flagCheck(8)) {
        dKy_set_allcol_ratio(mAllcolRatio);
    }

    return false;
}

void daTbox_c::OpenInit_com() {
    field_0x97e = 0;

    if (!field_0x718) {
        dComIfGs_onTbox(getTboxNo());
        setDzb();
        if (mpBgCollision != NULL) {
            mpBgCollision->Move();
        }
    }

    if (checkEnvEffectTbox()) {
        mAllcolRatio = 0.55f;
        dKy_set_allcol_ratio(mAllcolRatio);
        flagOn(8);
        dKy_efplight_set(&mLight);

        if (getShapeType() == SHAPE_LARGE) {
            J3DAnmTransform* bck =
                (J3DAnmTransform*)dComIfG_getObjectRes(getModelInfo()->mArcName, 9);
            mpAnm->init(bck, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, bck->getFrameMax(), true);
            mOpenSeId = Z2SE_OBJ_TBOX_OPEN_B_SLOW;
        }
    }

    mpAnm->setPlaySpeed(0.0f);
    fopAcM_seStart(this, mOpenSeId, 0);
}

void daTbox_c::OpenInit() {
    OpenInit_com();
    field_0x759 = 1;
    mDemoFrame = 0;
    flagOn(0x10);
    setLightPos();
}

int daTbox_c::actionWait() {
    return true;
}

int daTbox_c::actionDemo() {
    if (dComIfGp_evmng_endCheck(eventInfo.getEventId())) {
        if (field_0x718) {
            mpAnm->setPlaySpeed(0.0f);
            mpAnm->setFrame(0.0f);
            setAction(&daTbox_c::actionOpenWait);
        } else {
            setAction(&daTbox_c::actionWait);
        }

        dComIfGp_event_reset();
        dKy_set_allcol_ratio(1.0f);
        flagOff(0x18);
        dComIfGp_event_setItemPartner(NULL);

        if (getItemNo() == fpcNm_ITEM_KAKERA_HEART) {
            if (!strcmp(dComIfGp_getStartStageName(), "F_SP121")) {
                if (fopAcM_GetRoomNo(this) == 0) {
                    switch (getTboxNo()) {
                    case 3:
                        dComIfGs_setEventReg(0xedff, dComIfGs_getEventReg(0xedff) | 0x40);
                        break;
                    case 2:
                        dComIfGs_setEventReg(0xebff, dComIfGs_getEventReg(0xebff) | 0x10);
                        break;
                    default:
                        // "Piece of Heart: Unexpected configuration. Couldn't set Event Bit!\n"
                        OSReport_Error("ハートの欠片：想定外の配置です。イベントビットセットできませんでした！\n");
                        break;
                    }
                } else if (fopAcM_GetRoomNo(this) == 3) {
                    switch (getTboxNo()) {
                    case 5:
                        dComIfGs_setEventReg(0xf0ff, dComIfGs_getEventReg(0xf0ff) | 0x80);
                        break;
                    default:
                        // "Piece of Heart: Unexpected configuration. Couldn't set Event Bit!\n"
                        OSReport_Error("ハートの欠片：想定外の配置です。イベントビットセットできませんでした！\n");
                        break;
                    }
                }
            } else if (!strcmp(dComIfGp_getStartStageName(), "F_SP109")) {
                if (fopAcM_GetRoomNo(this) == 0 && getTboxNo() == 21) {
                    dComIfGs_setEventReg(0xefff, dComIfGs_getEventReg(0xefff) | 0x10);
                }
            }
        }
    } else {
        demoProc();
    }

    field_0x9f4++;
    return true;
}

int daTbox_c::actionDemo2() {
    if ((getEvent() == 0xFF && dComIfGp_evmng_endCheck("DEFAULT_TREASURE_APPEAR")) ||
                (getEvent() != 0xFF && dComIfGp_evmng_endCheck(mEventId))) {
        setAction(&daTbox_c::actionOpenWait);
        dComIfGp_event_reset();
    } else {
        demoProc();
    }

    return true;
}

int daTbox_c::actionDropDemo() {
    if (mEventId != -1) {
        if (dComIfGp_evmng_endCheck(mEventId)) {
            setAction(&daTbox_c::actionOpenWait);
            dComIfGp_event_reset();
            setDzb();
            home.pos = current.pos;

            if (field_0x9c9 != 0) {
                camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
                camera->mCamera.Start();
                camera->mCamera.SetTrimSize(0);
                field_0x9c9 = 0;
            }
        } else {
            demoProc();
        }
    } else {
        dropProc();
        if (mAcch.ChkGroundLanding() && field_0x97d) {
            setAction(&daTbox_c::actionOpenWait);
            home.pos = current.pos;
        }
    }
    return true;
}

u8 daTbox_c::getBombItemNo(u8 i_bombType, u8 i_itemNo) {
    u8 new_item_no = i_itemNo;
    switch (i_bombType) {
    case fpcNm_ITEM_NORMAL_BOMB:
        switch (i_itemNo) {
        case fpcNm_ITEM_WATER_BOMB_5:
        case fpcNm_ITEM_WATER_BOMB_30:
        case fpcNm_ITEM_BOMB_INSECT_5:
        case fpcNm_ITEM_BOMB_INSECT_20:
            new_item_no = fpcNm_ITEM_BOMB_5;
            break;
        case fpcNm_ITEM_WATER_BOMB_10:
        case fpcNm_ITEM_BOMB_INSECT_10:
            new_item_no = fpcNm_ITEM_BOMB_10;
            break;
        case fpcNm_ITEM_WATER_BOMB_20:
            new_item_no = fpcNm_ITEM_BOMB_20;
            break;
        case fpcNm_ITEM_BOMB_INSECT_30:
            new_item_no = fpcNm_ITEM_BOMB_30;
            break;
        }
        break;
    case fpcNm_ITEM_WATER_BOMB:
        switch (i_itemNo) {
        case fpcNm_ITEM_BOMB_INSECT_20:
        case fpcNm_ITEM_BOMB_INSECT_30:
            new_item_no = fpcNm_ITEM_WATER_BOMB_30;
            break;
        case fpcNm_ITEM_BOMB_5:
        case fpcNm_ITEM_BOMB_INSECT_5:
            new_item_no = fpcNm_ITEM_WATER_BOMB_5;
            break;
        case fpcNm_ITEM_BOMB_10:
        case fpcNm_ITEM_BOMB_INSECT_10:
            new_item_no = fpcNm_ITEM_WATER_BOMB_10;
            break;
        case fpcNm_ITEM_BOMB_20:
        case fpcNm_ITEM_BOMB_30:
            new_item_no = fpcNm_ITEM_WATER_BOMB_20;
            break;
        }
        break;
    case fpcNm_ITEM_POKE_BOMB:
        switch (i_itemNo) {
        case fpcNm_ITEM_WATER_BOMB_30:
            new_item_no = fpcNm_ITEM_BOMB_INSECT_20;
            break;
        case fpcNm_ITEM_BOMB_5:
        case fpcNm_ITEM_WATER_BOMB_5:
            new_item_no = fpcNm_ITEM_BOMB_INSECT_5;
            break;
        case fpcNm_ITEM_BOMB_10:
        case fpcNm_ITEM_BOMB_20:
        case fpcNm_ITEM_BOMB_30:
        case fpcNm_ITEM_WATER_BOMB_10:
        case fpcNm_ITEM_WATER_BOMB_20:
            new_item_no = fpcNm_ITEM_BOMB_INSECT_10;
            break;
        }
        break;
    case fpcNm_ITEM_BOMB_BAG_LV1:
        break;
    }

    // "Bomb Check\n"
    OS_REPORT("\x1b[43;30m爆弾チェック\n");
    if (i_itemNo == new_item_no) {
        // "Get bomb %d as is\n"
        OS_REPORT("爆弾 %d をそのままゲットします\n\x1b[m", new_item_no);
    } else {
        // "Bomb %d replaced by %d\n"
        OS_REPORT("爆弾 %d を %d に差し替えました\n\x1b[m", i_itemNo, new_item_no);
    }

    return new_item_no;
}

u8 daTbox_c::getBombItemNo2(u8 i_bombType1, u8 i_bombType2, u8 i_itemNo) {
    u8 new_type, new_item_no = i_itemNo;
    switch (i_itemNo) {
    case fpcNm_ITEM_BOMB_5:
    case fpcNm_ITEM_BOMB_10:
    case fpcNm_ITEM_BOMB_20:
    case fpcNm_ITEM_BOMB_30:
        new_type = fpcNm_ITEM_NORMAL_BOMB;
        break;
    case fpcNm_ITEM_WATER_BOMB_5:
    case fpcNm_ITEM_WATER_BOMB_10:
    case fpcNm_ITEM_WATER_BOMB_20:
    case fpcNm_ITEM_WATER_BOMB_30:
        new_type = fpcNm_ITEM_WATER_BOMB;
        break;
    case fpcNm_ITEM_BOMB_INSECT_5:
    case fpcNm_ITEM_BOMB_INSECT_10:
    case fpcNm_ITEM_BOMB_INSECT_20:
    case fpcNm_ITEM_BOMB_INSECT_30:
        new_type = fpcNm_ITEM_POKE_BOMB;
        break;
    }

    // "Bomb Check - 2 Bags\n"
    OS_REPORT("\x1b[43;30m爆弾チェック・袋２個\n");
    // "Bomb Type is %d\n"
    OS_REPORT("爆弾の種別は %d です\n", new_type);

    if (i_bombType1 == new_type || i_bombType2 == new_type) {
        // "There's a bag with the same bomb in it\n"
        OS_REPORT("同じ爆弾入りの袋があるのでそこに入れます\n");
    } else if (i_bombType1 == fpcNm_ITEM_BOMB_BAG_LV1 || i_bombType2 == fpcNm_ITEM_BOMB_BAG_LV1) {
        // "This is an empty bag so put it in\n"
        OS_REPORT("空の袋があるのでそこに入れます\n");
    } else {
        switch (new_type) {
        case fpcNm_ITEM_NORMAL_BOMB:
            if (i_bombType1 == fpcNm_ITEM_WATER_BOMB || i_bombType2 == fpcNm_ITEM_WATER_BOMB) {
                new_item_no = getBombItemNo(fpcNm_ITEM_WATER_BOMB, i_itemNo);
            } else {
                new_item_no = getBombItemNo(fpcNm_ITEM_POKE_BOMB, i_itemNo);
            }
            break;
        case fpcNm_ITEM_WATER_BOMB:
            if (i_bombType1 == fpcNm_ITEM_NORMAL_BOMB || i_bombType2 == fpcNm_ITEM_NORMAL_BOMB) {
                new_item_no = getBombItemNo(fpcNm_ITEM_NORMAL_BOMB, i_itemNo);
            } else {
                new_item_no = getBombItemNo(fpcNm_ITEM_POKE_BOMB, i_itemNo);
            }
            break;
        case fpcNm_ITEM_POKE_BOMB:
            if (i_bombType1 == fpcNm_ITEM_NORMAL_BOMB || i_bombType2 == fpcNm_ITEM_NORMAL_BOMB) {
                new_item_no = getBombItemNo(fpcNm_ITEM_NORMAL_BOMB, i_itemNo);
            } else {
                new_item_no = getBombItemNo(fpcNm_ITEM_WATER_BOMB, i_itemNo);
            }
            break;
        }
    }

    OS_REPORT("\x1b[m\n");
    return new_item_no;
}

u8 daTbox_c::getBombItemNo3(u8 i_bombType1, u8 i_bombType2, u8 i_bombType3, u8 i_itemNo) {
    u8 new_type, new_item_no = i_itemNo;
    switch (i_itemNo) {
    case fpcNm_ITEM_BOMB_5:
    case fpcNm_ITEM_BOMB_10:
    case fpcNm_ITEM_BOMB_20:
    case fpcNm_ITEM_BOMB_30:
        new_type = fpcNm_ITEM_NORMAL_BOMB;
        break;
    case fpcNm_ITEM_WATER_BOMB_5:
    case fpcNm_ITEM_WATER_BOMB_10:
    case fpcNm_ITEM_WATER_BOMB_20:
    case fpcNm_ITEM_WATER_BOMB_30:
        new_type = fpcNm_ITEM_WATER_BOMB;
        break;
    case fpcNm_ITEM_BOMB_INSECT_5:
    case fpcNm_ITEM_BOMB_INSECT_10:
    case fpcNm_ITEM_BOMB_INSECT_20:
    case fpcNm_ITEM_BOMB_INSECT_30:
        new_type = fpcNm_ITEM_POKE_BOMB;
        break;
    }

    // "Bomb Check - 3 Bags\n"
    OS_REPORT("\x1b[43;30m爆弾チェック・袋３個\n");
    // "Bomb Type is %d\n"
    OS_REPORT("爆弾の種別は %d です\n", new_type);

    if (i_bombType1 == new_type || i_bombType2 == new_type || i_bombType3 == new_type) {
        // "There's a bag with the same bomb in it\n"
        OS_REPORT("同じ爆弾入りの袋があるのでそこに入れます\n");
    } else if (i_bombType1 == fpcNm_ITEM_BOMB_BAG_LV1 || i_bombType2 == fpcNm_ITEM_BOMB_BAG_LV1 || i_bombType3 == fpcNm_ITEM_BOMB_BAG_LV1) {
        // "This is an empty bag so put it in\n"
        OS_REPORT("空の袋があるのでそこに入れます\n");
    } else {
        switch (new_type) {
        case fpcNm_ITEM_NORMAL_BOMB:
            if (i_bombType1 == fpcNm_ITEM_WATER_BOMB || i_bombType2 == fpcNm_ITEM_WATER_BOMB || i_bombType3 == fpcNm_ITEM_WATER_BOMB) {
                new_item_no = getBombItemNo(fpcNm_ITEM_WATER_BOMB, i_itemNo);
            } else {
                new_item_no = getBombItemNo(fpcNm_ITEM_POKE_BOMB, i_itemNo);
            }
            break;
        case fpcNm_ITEM_WATER_BOMB:
            if (i_bombType1 == fpcNm_ITEM_NORMAL_BOMB || i_bombType2 == fpcNm_ITEM_NORMAL_BOMB || i_bombType3 == fpcNm_ITEM_NORMAL_BOMB) {
                new_item_no = getBombItemNo(fpcNm_ITEM_NORMAL_BOMB, i_itemNo);
            } else {
                new_item_no = getBombItemNo(fpcNm_ITEM_POKE_BOMB, i_itemNo);
            }
            break;
        case fpcNm_ITEM_POKE_BOMB:
            if (i_bombType1 == fpcNm_ITEM_NORMAL_BOMB || i_bombType2 == fpcNm_ITEM_NORMAL_BOMB || i_bombType3 == fpcNm_ITEM_NORMAL_BOMB) {
                new_item_no = getBombItemNo(fpcNm_ITEM_NORMAL_BOMB, i_itemNo);
            } else {
                new_item_no = getBombItemNo(fpcNm_ITEM_WATER_BOMB, i_itemNo);
            }
            break;
        }
    }

    OS_REPORT("\x1b[m\n");
    return new_item_no;
}

u8 daTbox_c::getBombItemNoMain(u8 i_itemNo) {
    u8 bomb_type[3], bomb_max[3], bomb_num[3];
    u8 bag_count = 0;

    for (int i = 0; i < 3; i++) {
        bomb_type[i] = dComIfGs_getItem(SLOT_15 + i, false);
        bomb_max[i] = dComIfGs_getBombMax(bomb_type[i]);
        bomb_num[i] = dComIfGs_getBombNum(i);

        if (bomb_type[i] != fpcNm_ITEM_NONE) {
            bag_count++;
        }
    }

#if DEBUG
    // "====Bomb Bag count is %d====\n"
    OS_REPORT("====バクダン袋の数は %d 個です====\n", bag_count);
    for (int i = 0; i < 3; i++) {
        // "Bomb Bag<%d> : Type <%d> Max <%d> Current <%d>\n"
        OS_REPORT("爆弾袋<%d> : 種別 <%d> 最大 <%d> 現在 <%d>\n", i, bomb_type[i], bomb_max[i], dComIfGs_getBombNum(i));
    }
    OS_REPORT("==================================\n");
#endif

    switch (bag_count) {
    case 0:
        break;
    case 1:
        i_itemNo = getBombItemNo(bomb_type[0], i_itemNo);
        break;
    case 2:
        if (bomb_num[0] == bomb_max[0] && bomb_max[0] != 0) {
            if (bomb_num[1] == bomb_max[1] && bomb_max[1] != 0) {
                // "Both Bomb Bags are full!\n"
                OS_REPORT("\x1b[43;30mバクダン袋２つ共一杯でした！\n\x1b[m");
            } else {
                // "Bag 2 Get Bomb!\n"
                OS_REPORT("\x1b[43;30m袋２のバクダンをゲット！\n\x1b[m");
                i_itemNo = getBombItemNo(bomb_type[1], i_itemNo);
            }
        } else if (bomb_num[1] == bomb_max[1] && bomb_max[1] != 0) {
            // "Bag 1 Get Bomb!\n"
            OS_REPORT("\x1b[43;30m袋１のバクダンをゲット！\n\x1b[m");
            i_itemNo = getBombItemNo(bomb_type[0], i_itemNo);
        } else {
            // "Bag 1 and 2 Get Bomb!\n"
            OS_REPORT("\x1b[43;30m袋１と２のバクダンをゲット！\n\x1b[m");
            i_itemNo = getBombItemNo2(bomb_type[0], bomb_type[1], i_itemNo);
        }
        break;
    case 3:
        if (bomb_num[0] == bomb_max[0] && bomb_max[0] != 0) {
            if (bomb_num[1] == bomb_max[1] && bomb_max[1] != 0) {
                if (bomb_num[2] == bomb_max[2] && bomb_max[2] != 0) {
                    // "All 3 Bomb Bags are full!\n"
                    OS_REPORT("\x1b[43;30mバクダン袋３つ共一杯でした！\n\x1b[m");
                } else {
                    // "Bag 3 Get Bomb!\n"
                    OS_REPORT("\x1b[43;30m袋３のバクダンをゲット！\n\x1b[m");
                    i_itemNo = getBombItemNo(bomb_type[2], i_itemNo);
                }
            } else if (bomb_num[2] == bomb_max[2] && bomb_max[2] != 0) {
                // "Bag 2 Get Bomb!\n"
                OS_REPORT("\x1b[43;30m袋２のバクダンをゲット！\n\x1b[m");
                i_itemNo = getBombItemNo(bomb_type[1], i_itemNo);
            } else {
                // "Bag 2 or 3 Get Bomb!\n"
                OS_REPORT("\x1b[43;30m袋２か３のバクダンをゲット！\n\x1b[m");
                i_itemNo = getBombItemNo2(bomb_type[1], bomb_type[2], i_itemNo);
            }
        } else if (bomb_num[1] == bomb_max[1] && bomb_max[1] != 0) {
            if (bomb_num[2] == bomb_max[2] && bomb_max[2] != 0) {
                // "Bag 1 Get Bomb!\n"
                OS_REPORT("\x1b[43;30m袋１のバクダンをゲット！\n\x1b[m");
                i_itemNo = getBombItemNo(bomb_type[0], i_itemNo);
            } else {
                // "Bag 1 and 3 Get Bomb!\n"
                OS_REPORT("\x1b[43;30m袋１と３のバクダンをゲット！\n\x1b[m");
                i_itemNo = getBombItemNo2(bomb_type[0], bomb_type[2], i_itemNo);
            }
        } else if (bomb_num[2] == bomb_max[2] && bomb_max[2] != 0) {
            // "Bag 1 and 2 Get Bomb!\n"
            OS_REPORT("\x1b[43;30m袋１と２のバクダンをゲット！\n\x1b[m");
            i_itemNo = getBombItemNo2(bomb_type[0], bomb_type[1], i_itemNo);
        } else {
            // "Bag 1, 2 and 3 Get Bomb!\n"
            OS_REPORT("\x1b[43;30m袋１と２と３のバクダンをゲット！\n\x1b[m");
            i_itemNo = getBombItemNo3(bomb_type[0], bomb_type[1], bomb_type[2], i_itemNo);
        }
        break;
    default:
        // "Get Item: Wrong number of bomb bags!<%d>\n"
        OS_REPORT("\x1b[41;37mゲットアイテム：バクダン袋の数がおかしい！<%d>\n\x1b[m", bag_count);
        JUT_ASSERT(2749, FALSE);
        break;
    }

    return i_itemNo;
}

int daTbox_c::setGetDemoItem() {
    u8 item_no = getItemNo();
    if (item_no == fpcNm_ITEM_BOMB_5 || item_no == fpcNm_ITEM_BOMB_10 || item_no == fpcNm_ITEM_BOMB_20 || item_no == fpcNm_ITEM_BOMB_30 ||
        item_no == fpcNm_ITEM_WATER_BOMB_5 || item_no == fpcNm_ITEM_WATER_BOMB_10 || item_no == fpcNm_ITEM_WATER_BOMB_20 || item_no == fpcNm_ITEM_WATER_BOMB_30 ||
        item_no == fpcNm_ITEM_BOMB_INSECT_5 || item_no == fpcNm_ITEM_BOMB_INSECT_10 || item_no == fpcNm_ITEM_BOMB_INSECT_20 || item_no == fpcNm_ITEM_BOMB_INSECT_30)
    {
        item_no = getBombItemNoMain(item_no);
    }

    fpc_ProcID item_id;
    if (field_0x718) {
        item_id = fopAcM_createItemForPresentDemo(&current.pos, item_no, 1, -1, -1, NULL, NULL);
    } else {
        item_id = fopAcM_createItemForTrBoxDemo(&current.pos, item_no, -1, -1, NULL, NULL);
    }

    if (item_id != fpcM_ERROR_PROCESS_ID_e) {
        dComIfGp_event_setItemPartnerId(item_id);
    }

    return true;
}

int daTbox_c::actionOpenWait() {
    daMidna_c* midna = daPy_py_c::getMidnaActor();
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (eventInfo.checkCommandDoor()) {
        dComIfGp_event_onEventFlag(4);

        if (getShapeType() != SHAPE_SMALL && player->checkNowWolf() &&
                                             !midna->checkMetamorphoseEnable()) {
            setAction(&daTbox_c::actionNotOpenDemo);
            mStaffId = dComIfGp_evmng_getMyStaffId(l_staff_name, NULL, 0);
            demoProc();
            field_0x9f4 = 0;
        } else {
            field_0x718 = player->checkTreasureRupeeReturn(getItemNo());
            setGetDemoItem();
            setAction(&daTbox_c::actionDemo);
            mStaffId = dComIfGp_evmng_getMyStaffId(l_staff_name, NULL, 0);
            demoProc();
            field_0x9f4 = 0;
        }
    } else if (boxCheck()) {
        eventInfo.onCondition(4);
        if (getShapeType() == SHAPE_SMALL) {
            eventInfo.setEventName("DEFAULT_TREASURE_SIMPLE");
        } else if (player->checkNowWolf() && !midna->checkMetamorphoseEnable()) {
            eventInfo.setEventName("DEFAULT_TREASURE_NOTOPEN");
        } else if (getShapeType() == SHAPE_BOSSKEY) {
            eventInfo.setEventId(field_0x984);
        } else if (checkEnvEffectTbox()) {
            eventInfo.setEventName("DEFAULT_TREASURE_EFFECT");
        } else {
            eventInfo.setEventName("DEFAULT_TREASURE_NORMAL");
        }
    }

    return true;
}

int daTbox_c::actionNotOpenDemo() {
    if (dComIfGp_evmng_endCheck(mEventId)) {
        setAction(&daTbox_c::actionOpenWait);
        dComIfGp_event_reset();
    } else {
        demoProc();
    }
    return true;
}

int daTbox_c::checkDrop() {
    if (getSwType() == 15) {
        if (dComIfGs_isSwitch(getSwNo(), fopAcM_GetRoomNo(this))) {
            return true;
        }
    } else if (getSwType() == 0) {
        for (int i = 0; i < 4; i++) {
            if (fopAcM_isSwitch(this, getSwNo() + i)) {
                return true;
            }
        }
    }

    return false;
}

void daTbox_c::settingDropDemoCamera() {
    camera_class* player_camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    player_camera->mCamera.Stop();

    dStage_MapEvent_dt_c* maptooldata = dEvt_control_c::searchMapEventData(getEvent(), fopAcM_GetRoomNo(this));
    JUT_ASSERT(0xB89, maptooldata != NULL);

    player_camera->mCamera.SetTrimSize(maptooldata->field_0x1);

    dStage_roomDt_c* roomdt = dComIfGp_roomControl_getStatusRoomDt(fopAcM_GetRoomNo(this));
    JUT_ASSERT(0xB8E, roomdt != NULL);
    
    stage_camera_class* stage_camera = roomdt->getCamera();
    stage_camera2_data_class* stage_camera_data = stage_camera->m_entries;
    stage_camera_data += maptooldata->data.maptool.field_0x16;
    
    stage_arrow_class* stage_arrow = roomdt->getArrow();
    stage_arrow_data_class* stage_arrow_data = stage_arrow->m_entries;
    stage_arrow_data += stage_camera_data->m_arrow_idx;

    cXyz spA0;
    cXyz spAC;

    spA0.x = stage_arrow_data->position.x;
    spA0.z = stage_arrow_data->position.z;
    spAC.x = home.pos.x;
    spAC.z = home.pos.z;
    f32 var_f30 = spA0.abs(spAC);

    cXyz spB8;
    cXyz spC4(cXyz::BaseY);

    s16 angle;
    getDropSAngle(&angle);

    spB8.x = stage_arrow_data->position.x;
    spB8.y = 0.0f;
    spB8.z = stage_arrow_data->position.z;
    spB8 -= home.pos;

    Mtx mtx;
    MTXRotAxisRad(mtx, &spC4, cM_s2rad(angle));
    mDoMtx_multVec(mtx, &spB8, &spB8);
    spB8 += home.pos;
    spB8.y = stage_arrow_data->position.y;

    cXyz cam_eye;
    cXyz cam_center;

    cam_eye = spB8;
    cam_center = home.pos;

    f32 var_f29 = cM_ssin(stage_arrow_data->angle.x);
    f32 var_f28 = cM_scos(stage_arrow_data->angle.x);
    f32 dist = var_f30 * (var_f29 / var_f28);
    if (stage_arrow_data->angle.x > 0) {
        dist = -dist;
    }

    cam_center.y = cam_eye.y + dist;
    player_camera->mCamera.Set(cam_center, cam_eye, 0, (f32)stage_camera_data->field_0x11);
    field_0x9c9 = 1;
}

int daTbox_c::actionSwOnWait() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(&daTbox_c::actionDemo2);
        mStaffId = dComIfGp_evmng_getMyStaffId(l_staff_name, NULL, 0);
        demoProc();
    } else if (dComIfGs_isSwitch(getSwNo(), fopAcM_GetRoomNo(this))) {
        if (getEvent() == 0xff) {
            fopAcM_orderOtherEvent(this, "DEFAULT_TREASURE_APPEAR", 0xffff, 1, 0);
        } else {
            fopAcM_orderOtherEventId(this, mEventId, getEvent(), 0xffff, 0, 1);
        }
        eventInfo.onCondition(2);
    }

    return true;
}

int daTbox_c::actionSwOnWait2() {
    if (dComIfGs_isSwitch(getSwNo(), fopAcM_GetRoomNo(this))) {
        setAction(&daTbox_c::actionOpenWait);
        setDzb();
        flagOff(0x43);
        int tbox_no = getTboxNo();
        dTres_c::onStatus(0, tbox_no, 1);
    } else {
        flagOn(0x40);
    }

    return true;
}

int daTbox_c::actionDropWait() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(&daTbox_c::actionDropDemo);
        clrDzb();
        field_0x97d = false;
        mStaffId = dComIfGp_evmng_getMyStaffId(l_staff_name, NULL, 0);
        if (getSwType() == 0) {
            settingDropDemoCamera();
        }
    } else if (checkDrop()) {
        if (mEventId != -1) {
            fopAcM_orderOtherEventId(this, mEventId, getEvent(), 0xffff, 0, 1);
            eventInfo.onCondition(2);
        } else {
            dropProcInitCall();
            setAction(&daTbox_c::actionDropDemo);
            field_0x97d = false;
        }
    }

    return true;
}

int daTbox_c::actionGenocide() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(&daTbox_c::actionDemo2);
        mStaffId = dComIfGp_evmng_getMyStaffId(l_staff_name, NULL, 0);
        demoProc();
    } else if (!fopAcM_myRoomSearchEnemy(fopAcM_GetRoomNo(this))) {
        if (mTimer != 0) {
            mTimer--;
        } else {
            if (getEvent() == 0xff) {
                fopAcM_orderOtherEvent(this, "DEFAULT_TREASURE_APPEAR", 0xffff, 1, 0);
            } else {
                fopAcM_orderOtherEventId(this, mEventId, getEvent(), 0xffff, 0, 1);
            }
            eventInfo.onCondition(2);
            dComIfGs_onSwitch(getSwNo(), fopAcM_GetRoomNo(this));
        }
    }
    return true;
}

int daTbox_c::actionDropWaitForWeb() {
    if (field_0x9fd != 0) {
        if (field_0x9fc != 0) {
            field_0x9fd = 0;
            return true;
        }
    } else {
        if (field_0x9fc == 0) {
            setAction(&daTbox_c::actionDropForWeb);
            shape_angle.z = 0;
            shape_angle.x = 0;
            fopAcM_SetGravity(this, -2.0f);
            fopAcM_onSwitch(this, getSwNo());

            // "Treasure Chest: Falling!<%d>\n"
            OS_REPORT("宝箱：落下！<%d>\n", getSwNo());
        }
    }

    setBaseMtx();
    field_0x9fc = 0;
    return true;
}

int daTbox_c::actionDropForWeb() {
    fopAcM_posMoveF(this, NULL);
    mAcch.CrrPos(dComIfG_Bgsp());

    home.pos = current.pos;
    attention_info.position = current.pos;
    eyePos = current.pos;

    setBaseMtx();

    if (mAcch.ChkGroundLanding()) {
        cXyz vec(2.0f, 2.0f, 2.0f);
        dComIfGp_particle_setPolyColor(0xe7, mAcch.m_gnd, &current.pos, &tevStr, &home.angle,
                                       &vec, 0, NULL, fopAcM_GetRoomNo(this), NULL);
        dComIfGp_getVibration().StartShock(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        setDzb();

        if (mpBgCollision != NULL) {
            mpBgCollision->Move();
        }
    
        setAction(&daTbox_c::actionOpenWait);
    }

    dTres_c::setPosition(getTboxNo(), &current.pos);
    return true;
}

void daTbox_c::initBaseMtx() {
    field_0x9c4 = 0;
    mRotAxis = cXyz::BaseX;
    mDoMtx_identity(field_0x988);

    mpModel->setBaseScale(scale);
    if (mpSlimeModel != NULL) {
        mpSlimeModel->setBaseScale(scale);
    }

    setBaseMtx();

    if (mpBgW != NULL) {
        mpBgW->Move();
    }
    if (mpOpenBgW != NULL) {
        mpOpenBgW->Move();
    }
}

void daTbox_c::setBaseMtx() {
    Mtx mtx;
    if (field_0x9c4 == 0) {
        mDoMtx_identity(mtx);
    } else {
        MTXRotAxisRad(mtx, &mRotAxis, cM_s2rad(field_0x9c4));
    }

    s16 angle;
    if (getDropSAngle(&angle) && std::fabs(speed.y) > 2.0f) {
        cLib_addCalcAngleS(&home.angle.y, angle, 10, 0xc00, 0x400);
        shape_angle = home.angle;
    }

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::transM(0.0f, 50.0f, 0.0f);
    mDoMtx_stack_c::concat(mtx);
    mDoMtx_stack_c::concat(field_0x988);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::transM(0.0f, -50.0f, 0.0f);

    if (field_0x9fc != 0) {
        mpModel->setBaseTRMtx(mDrawMtx);
    } else {
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    mDoMtx_concat(mtx, field_0x988, field_0x988);
    MTXCopy(mpModel->getBaseTRMtx(), mBgMtx);

    if (mpSlimeModel != NULL) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y + 15.0f, current.pos.z);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mpSlimeModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

static daTbox_HIO_c l_HIO;

void daTbox_c::mode_proc_call() {
    static daTbox_modeFn l_func[2] = {
        &daTbox_c::mode_exec_wait,
        &daTbox_c::mode_exec,
    };
    (this->*l_func[mMode])();
}

void daTbox_c::mode_exec_wait() {
    bool bvar1 = false;

    if (field_0x9cc != 0) {
        flagOn(0x40);
        cXyz start(home.pos);
        cXyz end(home.pos);
        start.y += 5.0f;
        end.y -= 30.0f;

        if (fopAcM_lc_c::lineCheck(&start, &end, this) && fopAcM_lc_c::checkMoveBG()) {
            bvar1 = true;
            home.pos = *fopAcM_lc_c::getCrossP();
            current.pos = home.pos;
        }
    }

    if (bvar1) {
        flagOff(0x40);
        mMode = MODE_EXEC;
        mode_exec();
    }
}

void daTbox_c::mode_exec() {
    action();

    if (getFuncType() == 5 || getFuncType() == 6 || field_0x9cc == 1) {
        fopAcM_posMoveF(this, NULL);

        if (field_0x97c) {
            mAcch.CrrPos(dComIfG_Bgsp());
        } else {
            mAcch.ClrGroundHit();
        }

        attention_info.position = current.pos;
        eyePos = current.pos;
        setBaseMtx();

        if (mpBgCollision == mpOpenBgW) {
            mpBgCollision->Move();
        }

        dTres_c::setPosition(getTboxNo(), &current.pos);
    }
}

cPhs__Step daTbox_c::create1st() {
    if (!mParamsInit) {
        field_0x980 = home.angle.x;
        field_0x982 = home.angle.z;
        home.angle.z = 0;
        home.angle.x = 0;
        mParamsInit = true;
    }

    if (getShapeType() > 2) {
        OS_REPORT("\x1b[43;30mTbox Type Error!!!<%d>\n\x1b[m", getShapeType());
        JUT_ASSERT(3544, FALSE);
    }

    daTbox_ModelInfo* model_info = getModelInfo();
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhase, model_info->mArcName);
    if (step != cPhs_COMPLEATE_e) {
        return step;
    }

    step = (cPhs__Step)MoveBGCreate(model_info->mArcName, model_info->mClosedDzbResNo,
                                    dBgS_MoveBGProc_TypicalRotY, calcHeapSize(), NULL);
    if (step == cPhs_ERROR_e) {
        return step;
    }

    CreateInit();
    attention_info.flags = fopAc_AttnFlag_JUEL_e;
    attention_info.flags |= fopAc_AttnFlag_UNK_0x400000;
    return step;
}

int daTbox_c::Execute(Mtx** param_0) {
    mode_proc_call();
    *param_0 = &mBgMtx;

    cXyz center(l_cyl_info[getShapeType()].mCenter);
    mDoMtx_stack_c::YrotS(shape_angle.y);
    mDoMtx_stack_c::multVec(&center, &center);
    center += current.pos;

    if ((getSwNo() != 0xff && fopAcM_isSwitch(this, getSwNo())) || getSwNo() == 0xff) {
        mCyl.SetC(center);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    return true;
}

int daTbox_c::Draw() {
    if (flagCheck(0x40)) {
        return true;
    }

    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);

    if (mpEffectModel != NULL && mpEffectAnm->getPlaySpeed() != 0.0f) {
        g_env_light.setLightTevColorType_MAJI(mpEffectModel, &tevStr);
        mpEffectAnm->entry(mpEffectModel->getModelData());
        mDoExt_modelUpdateDL(mpEffectModel);
        mDoExt_brkAnmRemove(mpEffectModel->getModelData());
    }

    if (flagCheck(1)) {
        return true;
    }

    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    if (mpSlimeModel != NULL) {
        dComIfGd_setXluListBG();
        mDoExt_modelUpdateDL(mpSlimeModel);
        dComIfGd_setList();
    }

    mpAnm->entry(mpModel->getModelData());

    if (getShapeType() == SHAPE_BOSSKEY) {
        fopAcM_setEffectMtx(this, mpModel->getModelData());
    }

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return true;
}

int daTbox_c::Delete() {
    if (mpBgCollision != NULL) {
        dComIfG_Bgsp().Release(mpBgCollision);
    }

    dComIfG_resDelete(&mPhase, getModelInfo()->mArcName);
    return true;
}

cPhs__Step daTbox_create1st(daTbox_c* i_this) {
    fopAcM_ct(i_this, daTbox_c);
    return i_this->create1st();
}

static int daTbox_MoveBGDelete(daTbox_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daTbox_MoveBGExecute(daTbox_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daTbox_MoveBGDraw(daTbox_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daTbox_METHODS = {
    (process_method_func)daTbox_create1st,
    (process_method_func)daTbox_MoveBGDelete,
    (process_method_func)daTbox_MoveBGExecute,
    NULL,
    (process_method_func)daTbox_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_TBOX = {
    fpcLy_CURRENT_e,
    4,
    fpcPi_CURRENT_e,
    PROC_TBOX,
    &g_fpcLf_Method.base,
    sizeof(daTbox_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x10c,
    &daTbox_METHODS,
    0x44100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};

AUDIO_INSTANCES
