/**
 * d_a_crod.cpp
 * Dominion Rod Object
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_crod.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_cstaF.h"
#include "d/actor/d_a_cstatue.h"

#define RES_CROD_BALL_BMD 0x22
#define RES_CROD_BALL_BRK 0x40
#define RES_CROD_BALL_BTK 0x46

enum daCrod_ANM {
    /* 13 */ ANM_REVIVE_DEMO = 13,
    /* 14 */ ANM_WAIT_A,
    /* 15 */ ANM_WAIT_A_T,
    /* 16 */ ANM_WAIT_A_T_START,
    /* 17 */ ANM_WAIT_B,
    /* 18 */ ANM_WAIT_C,
};

// not sure what this is
static u8 const lit_3759[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const Vec l_localRodPos = {81.0f, -12.5f, -12.0f};

int daCrod_c::createHeap() {
    J3DModelData* model_data =
        (J3DModelData*)dComIfG_getObjectRes(daAlink_c::getAlinkArcName(), RES_CROD_BALL_BMD);
    mpBallModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000284);

    if (mpBallModel == NULL) {
        return 0;
    }

    J3DAnmTransform* bck =
        (J3DAnmTransform*)dComIfG_getObjectRes(daAlink_c::getAlinkArcName(), ANM_WAIT_A);
    if (!mBck.init(bck, TRUE, 2, 1.0f, 0, -1, false)) {
        return 0;
    }

    return 1;
}

static int daCrod_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daCrod_c*>(i_this)->createHeap();
}

static dCcD_SrcCps l_atCpsSrc = {
    {
        {0, {{AT_TYPE_1000000, 0, 0x1B}, {0, 0}, 0}},
        {dCcD_SE_STONE, 0, 0, 0, {2}},
        {dCcD_SE_NONE, 0, 0, 0, {0}},
        {0},
    },
    {
        {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            20.0f,
        },
    }
};

int daCrod_c::create() {
    fopAcM_ct(this, daCrod_c);

    if (fopAcM_GetParam(this) == 6) {
        fopAcM_setStageLayer(this);
        mCcStts.Init(200, 0, this);
        return cPhs_COMPLEATE_e;
    }

    if (!fopAcM_entrySolidHeap(this, daCrod_createHeap, 0x10C0)) {
        return cPhs_ERROR_e;
    }

    mCcStts.Init(10, 0, this);
    mAtCps.Set(l_atCpsSrc);
    mAtCps.SetStts(&mCcStts);

    model = mpBallModel;

    J3DModelData* model_data = mpBallModel->getModelData();
    mpBallBrk =
        (J3DAnmTevRegKey*)dComIfG_getObjectRes(daAlink_c::getAlinkArcName(), RES_CROD_BALL_BRK);
    mpBallBrk->searchUpdateMaterialID(model_data);
    model_data->entryTevRegAnimator(mpBallBrk);

    mpBallBtk =
        (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(daAlink_c::getAlinkArcName(), RES_CROD_BALL_BTK);
    mpBallBtk->searchUpdateMaterialID(model_data);
    model_data->entryTexMtxAnimator(mpBallBtk);

    fopAcM_setStageLayer(this);

    daAlink_c* alink = daAlink_getAlinkActorClass();
    if (alink->getCopyRodMtx()) {
        mDoMtx_multVec(alink->getCopyRodMtx(), &l_localRodPos, &current.pos);
    }

    mBck.play();
    mBck.entry(mpBallModel->getModelData());

    setMatrix();
    setRoomInfo();

    mLight.mPosition = current.pos;
    mLight.mColor.r = 150;
    mLight.mColor.g = 170;
    mLight.mColor.b = 90;
    setLightPower();
    dKy_plight_set(&mLight);

    return cPhs_COMPLEATE_e;
}

static int daCrod_Create(fopAc_ac_c* i_this) {
    return static_cast<daCrod_c*>(i_this)->create();
}

daCrod_c::~daCrod_c() {
    if (fopAcM_GetParam(this) == 6) {
        if (daAlink_getAlinkActorClass() != NULL) {
            daAlink_getAlinkActorClass()->clearIronBallActor();
        }
    } else {
        dKy_plight_cut(&mLight);
    }
}

static int daCrod_Delete(daCrod_c* i_this) {
    i_this->~daCrod_c();
    return 1;
}

void daCrod_c::setRoomInfo() {}

void daCrod_c::setMatrix() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpBallModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daCrod_c::posMove() {
    speed.set(
        speedF * cM_ssin(current.angle.y) * cM_scos(current.angle.x),
        speedF * cM_ssin(current.angle.x),
        speedF * cM_scos(current.angle.y) * cM_scos(current.angle.x)
    );
    current.pos += speed;
}

void daCrod_c::setBckAnm(u16 i_anmResID) {
    J3DAnmTransform* bck =
        (J3DAnmTransform*)dComIfG_getObjectRes(daAlink_c::getAlinkArcName(), i_anmResID);
    mBck.init(bck, TRUE, -1, 1.0f, 0, -1, true);
}

void daCrod_c::setReturn() {
    if (fopAcM_GetParam(this) == 4) {
        setBckAnm(ANM_WAIT_A_T);
    }

    fopAcM_SetParam(this, 5);
    speedF = daAlink_getAlinkActorClass()->getCopyRodBallReturnSpeed();
    mControllActorKeep.clearData();
}

void daCrod_c::setLightPower() {
    if (daAlink_getAlinkActorClass()->checkCopyRodTopUse()) {
        mLight.mPow = 300.0f;
        mLight.mFluctuation = 50.0f;
    } else {
        mLight.mPow = 0.0f;
        mLight.mFluctuation = 0.0f;
    }
}

int daCrod_c::execute() {
    daAlink_c* player = daAlink_getAlinkActorClass();

    if (fopAcM_GetParam(this) == 6) {
        if (player->getIronBallCenterPos() != NULL) {
            current.pos = *player->getIronBallCenterPos();
            attention_info.position = current.pos;
            eyePos = current.pos;
        }
        return 1;
    } else {
        cXyz sp50;

        if (mBck.isStop()) {
            setBckAnm(ANM_WAIT_A_T);
        }

        if (player->checkCopyRodRevive()) {
            if (field_0x732 == 0) {
                setBckAnm(ANM_REVIVE_DEMO);
                mBck.setPlayMode(2);
                mBck.setLoopFrame(32.0f);
            }

            field_0x732 = 1;
            mLight.mPow = mBck.getFrame() * 300.0f * 0.03125f;

            if (mLight.mPow > 300.0f) {
                mLight.mPow = 300.0f;
            }
        } else {
            setLightPower();

            if (mAtCps.ChkAtHit()) {
                fopAc_ac_c* hit_ac = mAtCps.GetAtHitAc();

                if (player->checkCopyRodEquip() && hit_ac != NULL &&
                    (fopAcM_GetName(hit_ac) == PROC_CSTATUE ||
                     fopAcM_GetName(hit_ac) == PROC_CSTAF))
                {
                    fopAcM_SetParam(this, 4);
                    mControllActorKeep.setData(hit_ac);
                    mCameraActorKeep.setData(hit_ac);

                    u16 anm_id;
                    if (fopAcM_GetName(hit_ac) == PROC_CSTATUE &&
                        static_cast<daCstatue_c*>(hit_ac)->checkBossType())
                    {
                        anm_id = ANM_WAIT_C;
                    } else {
                        anm_id = ANM_WAIT_B;
                    }
                    setBckAnm(anm_id);
                } else {
                    setReturn();
                }
            } else {
                if (field_0x732 != 0) {
                    field_0x732 = 0;
                    setBckAnm(ANM_WAIT_A);
                }
            }
        }

        static Vec shootInitLocalPos = {0.0f, getFlyInitY(), getFlyInitZ()};

        if (fopAcM_GetParam(this) == 2) {
            fopAcM_SetParam(this, 3);
            current.angle.x = -player->getBodyAngleX();
            current.angle.y = player->shape_angle.y + player->getBodyAngleY();
            speedF = player->getCopyRodBallSpeed();

            mDoMtx_stack_c::transS(*player->getRootPosP());
            mDoMtx_stack_c::ZXYrotM(-current.angle.x, current.angle.y, 0);
            mDoMtx_stack_c::multVec(&shootInitLocalPos, &current.pos);
            field_0x734 = current.pos;
            field_0x731 = 0;
        } else if (fopAcM_GetParam(this) == 4) {
            if (mControllActorKeep.getActor() == NULL) {
                setReturn();
            }
        } else if (fopAcM_GetParam(this) == 0) {
            if (dComIfGp_checkPlayerStatus0(0, 0x80)) {
                fopAcM_SetParam(this, 1);
                setBckAnm(ANM_WAIT_A_T_START);
            }
        } else if (fopAcM_GetParam(this) == 1) {
            if (!dComIfGp_checkPlayerStatus0(0, 0x80)) {
                fopAcM_SetParam(this, 0);
                setBckAnm(ANM_WAIT_A);
            }
        }

        if (fopAcM_GetParam(this) == 5) {
            sp50 = player->getBoomerangCatchPos() - current.pos;

            if (sp50.abs2() < speedF * speedF) {
                mCameraActorKeep.clearData();

                if (player->returnCopyRod()) {
                    fopAcM_SetParam(this, 0);
                    setBckAnm(ANM_WAIT_A);
                    fopAcM_seStart(this, Z2SE_AL_COPYROD_CATCH, 0);
                } else {
                    fopAcM_delete(this);
                    return 1;
                }
            } else {
                current.angle.x = cM_atan2s(sp50.y, sp50.absXZ());
                current.angle.y = sp50.atan2sX_Z();
            }
        }

        if (fopAcM_GetParam(this) == 3 || fopAcM_GetParam(this) == 5) {
            posMove();

            if (fopAcM_GetParam(this) == 3) {
                mAtCps.SetStartEnd(old.pos, current.pos);
                sp50 = current.pos - old.pos;

                mAtCps.SetAtVec(sp50);
                dComIfG_Ccsp()->Set(&mAtCps);
                fopAcM_seStartLevel(this, Z2SE_AL_COPYROD_THROW, 0);

                f32 fly_max = player->getCopyRodBallDisFlyMax();
                if (!player->checkCopyRodEquip() || field_0x731 ||
                    field_0x734.abs2(current.pos) > fly_max * fly_max)
                {
                    setReturn();
                }
            } else {
                fopAcM_seStartLevel(this, Z2SE_AL_COPYROD_RETURN, 0);
            }
        } else {
            mAtCps.ResetAtHit();

            if (fopAcM_GetParam(this) == 4) {
                fopAc_ac_c* control_actor = mControllActorKeep.getActor();
                if (fopAcM_GetName(control_actor) == PROC_CSTATUE) {
                    current.pos = static_cast<daCstatue_c*>(control_actor)->getBallPos();
                } else {
                    current.pos = static_cast<daCstaF_c*>(control_actor)->getBallPos();
                }
            } else {
                if (player->getCopyRodMtx() != NULL) {
                    mDoMtx_multVec(player->getCopyRodMtx(), &l_localRodPos, &current.pos);
                }

                if (daAlink_getAlinkActorClass()->checkCopyRodTopUse()) {
                    fopAcM_seStartLevel(this, Z2SE_AL_COPYROD_WAIT,
                                        fopAcM_GetParam(this) == 1 ? 1 : 0);
                }
            }
        }

        eyePos = current.pos;
        attention_info.position = current.pos;

        daAlink_c::simpleAnmPlay(mpBallBrk);
        daAlink_c::simpleAnmPlay(mpBallBtk);
        mBck.play();
        mBck.entry(mpBallModel->getModelData());

        setRoomInfo();
        setMatrix();
        mLight.mPosition = current.pos;
    }

    return 1;
}

static int daCrod_Execute(daCrod_c* i_this) {
    return i_this->execute();
}

int daCrod_c::draw() {
    if (fopAcM_GetParam(this) == 6 ||
        (!daAlink_getAlinkActorClass()->checkCopyRodTopUse() && field_0x732 == 0))
    {
        return 1;
    }

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpBallModel, &tevStr);
    mDoExt_modelUpdateDL(mpBallModel);
    return 1;
}

static int daCrod_Draw(daCrod_c* i_this) {
    return i_this->draw();
}

static actor_method_class l_daCrod_Method = {
    (process_method_func)daCrod_Create,  (process_method_func)daCrod_Delete,
    (process_method_func)daCrod_Execute, (process_method_func)NULL,
    (process_method_func)daCrod_Draw,
};

actor_process_profile_definition g_profile_CROD = {
    fpcLy_CURRENT_e,
    8,
    fpcPi_CURRENT_e,
    PROC_CROD,
    &g_fpcLf_Method.base,
    sizeof(daCrod_c),
    0,
    0,
    &g_fopAc_Method.base,
    688,
    &l_daCrod_Method,
    0x60000,
    5,
    fopAc_CULLBOX_0_e,
};
