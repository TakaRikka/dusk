/**
 * @file d_a_obj_kag.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_kag.h"
#include "d/d_menu_insect.h"
#include "d/d_s_play.h"
#include "d/d_cc_uty.h"

enum I_Kag_RES_File_ID {
    /* BCK */
    /* 0x06 */ BCK_KAG_FLY = 0x6,
    /* 0x07 */ BCK_KAG_WALK,

    /* BMDE */
    /* 0x0A */ BMDE_KAG = 0xA,

    /* BRK */
    /* 0x0D */ BRK_KAG_M = 0xD,
    /* 0x0E */ BRK_KAG_O,

    /* BTK */
    /* 0x11 */ BTK_KAG_M = 0x11,
    /* 0x12 */ BTK_KAG_O,
};

class daObj_KagHIO_c : public JORReflexible {
public:
    daObj_KagHIO_c();
    virtual ~daObj_KagHIO_c() {}

    void genMessage(JORMContext*);

    /* 黄金蟲(カゲロウ) - Golden Bug (Mayfly) */
    /* 0x04 */ s8 id;
    /* 0x08 */ f32 model_scale_female;  // モデルスケール(メス) - Model Scale (Female)
    /* 0x0C */ f32 model_scale_male;    // モデルスケール(オス) - Model Scale (Male)
};

static u8 l_initHIO;

static daObj_KagHIO_c l_HIO;

namespace {
    static dCcD_SrcSph cc_sph_src = {
        {
            {0x0, {{AT_TYPE_0, 0, 0}, {0x14040, 0x43}, 0}},
            {dCcD_SE_NONE, 0, 0, 0, 0},
            {dCcD_SE_NONE, 0, 0, 0, 2},
            {0},
        },
        {
            {{0.0f, 0.0f, 0.0f}, 20.0f}
        }
    };
}

daObj_KagHIO_c::daObj_KagHIO_c() {
    id = -1;
    model_scale_male = 0.8f;
    model_scale_female = 0.8f;
}

void daObjKAG_c::setAction(actionFunc action) {
    if (mAction) {
        field_0x83c = -1;
        (this->*mAction)();
    }

    mPrevAction = mAction;
    mAction = action;
    field_0x83c = 0;

    (this->*mAction)();
}

static int const l_kag_brk_index[2] = {
    BRK_KAG_O, BRK_KAG_M,
};

static int const l_kag_btk_index[2] = {
    BTK_KAG_O, BTK_KAG_M,
};

int daObjKAG_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("I_Kag", BMDE_KAG);

    JUT_ASSERT(246, modelData != NULL);

    mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("I_Kag", BCK_KAG_FLY), 2, 1.0f, 0, -1, &mSound, 0, 0x11000284);
    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mpModelMorf->getModel();

    mpBrkAnm = new mDoExt_brkAnm();
    if (mpBrkAnm == NULL) {
        return 0;
    }

    if (!mpBrkAnm->init(model->getModelData(), (J3DAnmTevRegKey*)dComIfG_getObjectRes("I_Kag", l_kag_brk_index[field_0x85d]), 1, 2, 1.0f, 0, -1)) {
        return 0;
    }

    mpBtkAnm = new mDoExt_btkAnm();
    if (mpBtkAnm == NULL) {
        return 0;
    }

    if (!mpBtkAnm->init(model->getModelData(), (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("I_Kag", l_kag_btk_index[field_0x85d]), 1, 2, 1.0f, 0, -1)) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    return static_cast<daObjKAG_c*>(a_this)->CreateHeap();
}

static int daObjKAG_Create(fopAc_ac_c* a_this) {
    return static_cast<daObjKAG_c*>(a_this)->create();
}

static int daObjKAG_Delete(daObjKAG_c* i_this) {
    i_this->_delete();
    return 1;
}

bool daObjKAG_c::Kag_Bgcheck(cXyz* param_1, csXyz* param_2, cXyz* param_3) {
    dBgS_LinChk lin_chk;
    lin_chk.SetObj();
    lin_chk.Set(&old.pos, &current.pos, NULL);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(lin_chk, &plane);

        if (param_1 != NULL) {
            *param_1 = lin_chk.GetCross();
        }

        cXyz* pcVar1 = plane.GetNP();

        if (param_2 != NULL) {
            cXyz spcc(0.0f, 0.0f, 0.0f);
            cXyz spd8(0.0f, pcVar1->y, pcVar1->z);
            param_2->z = -cM_atan2s(pcVar1->x, spcc.abs(spd8));
            param_2->x = cM_atan2s(pcVar1->z, pcVar1->y);
        }

        if (param_3 != NULL) {
            *param_3 = *pcVar1;
        }

        return true;
    }

    return false;
}

void daObjKAG_c::fly() {
    if (field_0x83c == 0) {
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("I_Kag", BCK_KAG_FLY), 2, 5.0f, 0.5f, 0.0f, -1.0f);
        field_0x852 = 0;
        field_0x85a = cM_rndF(30.0f) + 18.0f;
        field_0x84e = cM_rndFX(32768.0f);
        field_0x854 = cM_rndF(512.0f) + 512.0f;
        field_0x85b = cM_rndF(18.0f) + 18.0f;
        field_0x84c = cM_rndFX(12288.0f);
        field_0x856 = cM_rndF(512.0f) + 512.0f;
        field_0x83c++;
    } else if (field_0x83c != -1) {
        daPy_py_c* player = daPy_getPlayerActorClass();

        if (mDraw) {
            kag_setParticle();
        }

        fopAcM_posMoveF(this, NULL);
        field_0x852 += 0x2000;
        current.pos.y += cM_ssin(field_0x852) * 1.8f;

        f32 temp = 0.0f;
        if (field_0x848 != temp) {
            f32 fVar1 = std::abs(field_0x808.x);
            f32 fVar2 = std::abs(field_0x808.y);
            f32 fVar3 = std::abs(field_0x808.z);

            if (fVar2 > fVar1 && fVar2 > fVar3) {
                current.pos.y += field_0x848 * field_0x808.y;
            } else if (fVar1 > fVar2 && fVar1 > fVar3) {
                current.pos.x += field_0x848 * field_0x808.x;
            } else {
                current.pos.z += field_0x848 * field_0x808.z;
            }
        }

        if (Kag_Bgcheck(&current.pos, &field_0x814, &field_0x808)) {
            current.angle.z = 0;
            current.angle.x = 0;

            if ((s16)cM_rndF(10.0f) == 0) {
                setAction(&daObjKAG_c::wait);
                return;
            }

            speed.y = 0.0f;
            speedF = 0.0f;
            field_0x848 = cM_rndF(2.0f) + 6.0f;
        } else {
            cLib_chaseAngleS(&field_0x814.x, 0, 0x100);
            cLib_chaseAngleS(&field_0x814.y, 0, 0x100);
            cLib_chaseAngleS(&field_0x814.z, 0, 0x100);
        }

        cXyz* flamePos = player->getKandelaarFlamePos();
        if (field_0x85a == 0) {
            if (flamePos != NULL && current.pos.absXZ(*flamePos) < 1800.0f) {
                field_0x84e = cLib_targetAngleY(&current.pos, flamePos) + cM_rndFX(8192.0f);
                
                if (current.pos.absXZ(*flamePos) < 200.0f) {
                    field_0x85a = cM_rndF(4.0f) + 4.0f;
                } else {
                    field_0x85a = cM_rndF(30.0f) + 18.0f;
                }

                field_0x854 = cM_rndF(512.0f) + 512.0f;
            } else if (field_0x85e == 1 || current.pos.absXZ(home.pos) < 800.0f) {
                field_0x85a = cM_rndF(30.0f) + 18.0f;
                field_0x84e = cM_rndFX(32768.0f);
                field_0x854 = cM_rndF(512.0f) + 512.0f;
            } else {
                field_0x85a = 90;
                field_0x84e = cLib_targetAngleY(&current.pos, &home.pos) + cM_rndFX(8192.0f);
                field_0x854 = cM_rndF(512.0f) + 512.0f;
            }
        }

        if (flamePos != NULL) {
            home.pos = current.pos;
        }

        f32 fVar1;
        if (field_0x85e == 1) {
            fVar1 = 1200.0f;
        } else {
            fVar1 = 300.0f;
        }

        if ((flamePos != NULL && current.pos.absXZ(*flamePos) < 200.0f && field_0x81c + 200.0f < current.pos.y) || field_0x81c + fVar1 < current.pos.y) {
            field_0x85b = cM_rndF(10.0f) + 10.0f;
            field_0x84c = cM_rndF(12288.0f) + 4096.0f;
            field_0x856 = cM_rndF(512.0f) + 512.0f;
        } else if (field_0x81c + 80.0f > current.pos.y) {
            if (current.pos.y < field_0x81c) {
                current.pos.y = field_0x81c;
            }

            field_0x85b = cM_rndF(10.0f) + 10.0f;
            field_0x84c = -4096.0f - cM_rndF(12288.0f);
            field_0x856 = 0x400;
        } else if (field_0x85b == 0) {
            field_0x85b = cM_rndF(18.0f) + 18.0f;
            field_0x84c = cM_rndFX(12288.0f);
            field_0x856 = cM_rndF(256.0f) + 256.0f;
        }

        if (flamePos != NULL && current.pos.absXZ(*flamePos) < 400.0f) {
            cLib_chaseF(&field_0x820, 4.0f, 0.1f);
        } else {
            cLib_chaseF(&field_0x820, 8.0f, 0.1f);
        }

        cLib_chaseAngleS(&current.angle.y, field_0x84e, field_0x854);
        cLib_chaseAngleS(&current.angle.x, field_0x84c, field_0x856);
        cLib_chaseF(&speedF, field_0x820 * cM_scos(current.angle.x), 1.0f);
        cLib_chaseF(&speed.y, -field_0x820 * cM_ssin(current.angle.x), 1.0f);
        cLib_chaseF(&field_0x848, 0.0f, 0.2f);
        mSound.startCreatureSoundLevel(Z2SE_INSCT_BTBT, 0, -1);
    }
}

void daObjKAG_c::wait() {
    if (field_0x83c == 0) {
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("I_Kag", BCK_KAG_WALK), 2, 5.0f, 0.0f, 0.0f, -1.0f);
        speed.y = 0.0f;
        speedF = 0.0f;
        field_0x85a = cM_rndF(220.0f) + 30.0f;
        field_0x83c++;
    } else if (field_0x83c != -1) {
        if (mDraw) {
            kag_setParticle();
        }

        if (field_0x85a == 0) {
            if ((s16)cM_rndF(3.0f) != 0) {
                setAction(&daObjKAG_c::fly);
            } else {
                setAction(&daObjKAG_c::walk);
            }
        }
    }
}

void daObjKAG_c::walk() {
    if (field_0x83c == 0) {
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("I_Kag", BCK_KAG_WALK), 2, 5.0f, 1.0f, 0.0f, -1.0f);
        field_0x85a = cM_rndF(30.0f) + 30.0f;
        field_0x83c++;
    } else if (field_0x83c == -1) {
        speedF = 0.0f;
    } else {
        cXyz sp12c(0.0f, 0.0f, speedF);
        cXyz sp138(0.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::ZXYrotS(field_0x814);
        mDoMtx_stack_c::YrotM(current.angle.y);
        mDoMtx_stack_c::multVec(&sp12c, &sp138);
        current.pos += sp138;

        dBgS_LinChk lin_chk, lin_chk2;
        cXyz sp144(0.0f, -20.0f, 0.0f);
        cXyz sp150(0.0f, 20.0f, 0.0f);
        cXyz sp15c(0.0f, 20.0f, 20.0f);
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(field_0x814);
        mDoMtx_stack_c::YrotM(current.angle.y);
        mDoMtx_stack_c::multVec(&sp144, &sp144);
        mDoMtx_stack_c::multVec(&sp150, &sp150);
        mDoMtx_stack_c::multVec(&sp15c, &sp15c);
        lin_chk.SetObj();
        lin_chk2.SetObj();
        lin_chk.Set(&sp150, &sp144, NULL);
        lin_chk2.Set(&sp150, &sp15c, NULL);
        speedF = 0.0f;

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            cM3dGPla plane;
            dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
            cXyz* pcVar1 = plane.GetNP();
            cXyz sp168(0.0f, 0.0f, 0.0f);
            cXyz sp174(0.0f, pcVar1->y, pcVar1->z);
            f32 fVar1 = sp168.abs(sp174);
            current.pos = lin_chk.GetCross();
            field_0x814.z = -cM_atan2s(pcVar1->x, fVar1);
            field_0x814.x = cM_atan2s(pcVar1->z, pcVar1->y);

            if (dComIfG_Bgsp().LineCross(&lin_chk2)) {
                field_0x84e += 0x100;
            } else {
                field_0x84e = current.angle.y;
                speedF = 0.8f;
            }
        } else {
            field_0x84e += 0x100;
        }

        cLib_chaseAngleS(&current.angle.y, field_0x84e, 0x100);

        if (mDraw) {
            kag_setParticle();
        }

        if (field_0x85a == 0) {
            if ((s16)cM_rndF(1.0f) != 0) {
                setAction(&daObjKAG_c::fly);
            } else {
                setAction(&daObjKAG_c::wait);
            }
        }

        mSound.startCreatureSoundLevel(Z2SE_INSCT_KSKS, 0, -1);
    }
}

void daObjKAG_c::bin_wait() {
    if (field_0x83c == 0) {
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("I_Kag", BCK_KAG_FLY), J3DFrameCtrl::EMode_LOOP, 5.0f, 0.0f, 0.0f, -1.0f);
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
        mDraw = false;
        gravity = 0.0f;
        maxFallSpeed = 0.0f;
        field_0x83c++;
    } else if (field_0x83c != -1 && field_0x56c != 0) {
        current.angle.y = daPy_getPlayerActorClass()->current.angle.y - 0x4000;

        if (strcmp("R_SP160", dComIfGp_getStartStageName()) == 0 && dComIfGp_getStartStageRoomNo() == 3) {
            field_0x85e = 1;
            mDraw = true;
            setAction(&daObjKAG_c::fly);
        } else {
            setAction(&daObjKAG_c::bin_action);
        }
    }
}

void daObjKAG_c::bin_action() {
    if (field_0x83c == 0) {
        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("I_Kag", BCK_KAG_FLY), J3DFrameCtrl::EMode_LOOP, 5.0f, 0.5f, 0.0f, -1.0f);
        mDraw = true;
        maxFallSpeed = -2.0f;
        gravity = -2.0f;
        scale.setall(0.4f);
        field_0x85a = 10;
        field_0x83c++;
    } else if (field_0x83c != -1) {
        f32 fVar1 = scale.x;
        fopAcM_posMoveF(this, NULL);

        if (field_0x85a != 0) {
            cLib_chaseF(&fVar1, 0.6f, 0.1f);
        } else {
            cLib_addCalc2(&fVar1, 0.0f, 62.0f, 0.05f);
            scale.setall(fVar1);

            if (fVar1 < 0.1) {
                fopAcM_delete(this);
                return;
            }
        }

        scale.setall(fVar1);

        if (mDraw) {
            kag_setParticle();
        }
    }
}

void daObjKAG_c::hook() {
    if (field_0x83c == 0) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        cXyz spb0(0.0f, hREG_F(2) + -10.0f, 0.0f);

        mSph.SetTgType(0);
        player->setHookshotCarryOffset(fopAcM_GetID(this), &spb0);
        field_0x83c++;
    } else if (field_0x83c == -1) {
        mSph.SetTgType(0x14040);
        home.pos = current.pos;
    } else {
        if (mDraw) {
            kag_setParticle();
        }

        if (!fopAcM_CheckStatus(this, fopAcM_STATUS_HOOK_CARRY_NOW)) {
            setAction(&daObjKAG_c::fly);
        }

        dBgS_LinChk lin_chk;
        lin_chk.SetObj();
        lin_chk.Set(&old.pos, &current.pos, NULL);

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            cM3dGPla plane;
            cXyz spbc;
            
            dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
            cXyz* pcVar1 = plane.GetNP();
            spbc = lin_chk.GetCross();
            spbc.x += pcVar1->x * 20.0f;
            spbc.y += pcVar1->y * 20.0f;
            spbc.z += pcVar1->z * 20.0f;

            old.pos = spbc;
            current.pos = old.pos;
        }
    }
}

void daObjKAG_c::boomerang() {
    if (field_0x83c == 0) {
        mSph.SetTgType(0);
        mBoomerangMove.initOffset(&current.pos, mSph.GetTgHitGObj());
        field_0x83c++;
    } else if (field_0x83c == -1) {
        mSph.SetTgType(0x14040);
        daPy_py_c* player = daPy_getPlayerActorClass();
        cXyz sp2c;
        sp2c = player->current.pos;
        sp2c.y += 100.0f;
        old.pos = sp2c;
        current.pos = old.pos;
        home.pos = current.pos;
    } else {
        if (mDraw) {
            kag_setParticle();
        }

        if (mBoomerangMove.posMove(&current.pos, &current.angle.y, NULL, 0x1C00) == 0) {
            setAction(&daObjKAG_c::fly);
        }

        mBoomerangMove.bgCheckAfterOffset(&current.pos);
    }
}

void daObjKAG_c::action() {
    (this->*mAction)();
    shape_angle = current.angle;

    if (field_0x85e == 0) {
        Insect_GetDemoMain();
    }
}

static u8 const l_kag_itemno[2] = {
    0xD6,
    0xD7,
};

void daObjKAG_c::checkGroundPos() {
    cXyz sp28(current.pos);
    mGndChk.SetPos(&sp28);

    f32 fVar1 = dComIfG_Bgsp().GroundCross(&mGndChk);
    if (fVar1 != -G_CM3D_F_INF) {
        field_0x81c = fVar1;
    }
}

void daObjKAG_c::hit_check() {
    dCcU_AtInfo atInfo;
    mStts.Move();

    if (mSph.ChkTgHit()) {
        atInfo.mpCollider = mSph.GetTgHitObj();

        if (atInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
            setAction(&daObjKAG_c::hook);
        } else {
            setAction(&daObjKAG_c::boomerang);
        }
    }
}

int daObjKAG_c::execute() {
    if (ChkGetDemo()) {
        field_0x57c = field_0x578 + 10000.0f;
        mSound.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
        Insect_GetDemoMain();
        kag_setParticle();
        return 1;
    }

    if (field_0x85a != 0) {
        field_0x85a--;
    }

    if (field_0x85b != 0) {
        field_0x85b--;
    }

    checkGroundPos();

    if (field_0x85e == 0) {
        hit_check();
    }

    action();
    mpBrkAnm->play();
    mpBtkAnm->play();
    mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    setBaseMtx();

    if (field_0x85e == 0) {
        mSph.SetC(current.pos);
        mSph.SetR(scale.x * 20.0f);
        dComIfG_Ccsp()->Set(&mSph);
        eyePos = current.pos;
        attention_info.position = eyePos;
    }

    return 1;
}

int daObjKAG_c::_delete() {
    dComIfG_resDelete(&mPhase, "I_Kag");

    if (field_0x85f != 0) {
        l_initHIO = 0;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (heap != NULL) {
        mpModelMorf->stopZelAnime();
    }

    return 1;
}

void daObjKAG_c::setBaseMtx() {
    J3DModel* model = mpModelMorf->getModel();
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x814);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    model->setBaseScale(scale);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mpModelMorf->modelCalc();
}

int daObjKAG_c::draw() {
    if (mDraw) {
        CalcZBuffer(50.0f);
        J3DModel* model = mpModelMorf->getModel();
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);

        mpBtkAnm->entry(model->getModelData());
        mpBrkAnm->entry(model->getModelData());

        if (field_0x85e != 2) {
            fopAcM_setEffectMtx(this, model->getModelData());
            mpModelMorf->entryDL();

            if (field_0x85e != 1) {
                dComIfGd_setSimpleShadow(&current.pos, field_0x81c, 15.0f, mGndChk, 0, -0.6f, dDlst_shadowControl_c::getSimpleTex());
            }
        }
    }

    return 1;
}

static int daObjKAG_Draw(daObjKAG_c* i_this) {
    return i_this->draw();
}

static int daObjKAG_Execute(daObjKAG_c* i_this) {
    return i_this->execute();
}

static u16 const l_musiya_num[2] = {
    0x01A7,
    0x01A8
};

cPhs__Step daObjKAG_c::create() {
    u8 uVar1 = (fopAcM_GetParam(this) & 0xF00) >> 8;

    fopAcM_ct(this, daObjKAG_c);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, "I_Kag");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("KAG PARAM %x\n", fopAcM_GetParam(this));
        
        field_0x85e = fopAcM_GetParam(this) & 3;
        if (field_0x85e == 3) {
            field_0x85e = 0;
        }

        field_0x85d = (fopAcM_GetParam(this) & 16) >> 4;
        if (field_0x85d != 0) {
            scale.setall(l_HIO.model_scale_female);
        } else {
            scale.setall(l_HIO.model_scale_male);
        }

        if (uVar1 == 15) {
            uVar1 = 0;
        }

        setItemNo(l_kag_itemno[field_0x85d]);
        setSaveBitNo(l_musiya_num[field_0x85d]);

        if (field_0x85e != 2) {
            if (uVar1 != 0) {
                if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[0]]) ||
                    !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[1]])) {
                    return cPhs_ERROR_e;
                }
            } else if (strcmp("R_SP160", dComIfGp_getStartStageName()) == 0 && dComIfGp_getStartStageRoomNo() == 3) {
                if (dMenu_Insect_c::isCatchNotGiveInsect(m_itemNo)) {
                    return cPhs_ERROR_e;
                }

                if ((field_0x85d == 0 && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[0]])) ||
                    (field_0x85d != 0 && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[1]]))) {
                    return cPhs_ERROR_e;
                }
            }
        }

        field_0x844 = 0.0f;

        OS_REPORT("//////////// I_kag /////////////\n");

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0xF40)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("/////////// I_kag SET OK ///////\n");

        if (l_initHIO == 0) {
            l_initHIO = 1;
            field_0x85f = 1;
            l_HIO.id = mDoHIO_CREATE_CHILD("黄金蟲(カゲロウ)", &l_HIO);
        }

        fopAcM_SetMtx(this, mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -50.0f, -50.0f, -50.0f);
        fopAcM_SetMax(this, 50.0f, 50.0f, 50.0f);
        attention_info.flags = 0;
        health = 10;
        field_0x560 = 10;

        mSound.init(&current.pos, &eyePos, 3, 1);

        if (field_0x85e == 0) {
            attention_info.distances[fopAc_attn_CARRY_e] = 93;
            mStts.Init(100, 0, this);
            mSph.Set(cc_sph_src);
            mSph.SetStts(&mStts);
        }

        if (field_0x85e == 2) {
            setAction(&daObjKAG_c::bin_wait);
        } else {
            setAction(&daObjKAG_c::fly);
        }

        daObjKAG_Execute(this);
    }

    return phase;
}

static int daObjKAG_IsDelete(daObjKAG_c* i_this) {
    return 1;
}

void daObjKAG_c::kag_setParticle() {
    if (CheckZ()) {
        cLib_chaseF(&field_0x844, 0.0f, 1.0f);
    } else {
        cLib_chaseF(&field_0x844, 1.0f, 1.0f);
    }

    if (field_0x85e != 2 && (strcmp("R_SP160", dComIfGp_getStartStageName()) != 0 || (strcmp("R_SP160", dComIfGp_getStartStageName()) == 0 && dComIfGp_getStartStageRoomNo() != 3))) {
        mParticleKey1 = dComIfGp_particle_set(mParticleKey1, 0xA1B, &current.pos, &tevStr, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);

        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey1);
        if (emitter != NULL) {
            emitter->setGlobalParticleScale(field_0x844 * scale.x, field_0x844 * scale.x);
        }

        mParticleKey2 = dComIfGp_particle_set(mParticleKey2, 0xA1C, &current.pos, &tevStr, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);

        emitter = dComIfGp_particle_getEmitter(mParticleKey2);
        if (emitter != NULL) {
            emitter->setGlobalParticleScale(field_0x844 * scale.x, field_0x844 * scale.x);
        }

        mSound.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
    }
}

static actor_method_class l_daObjKAG_Method = {
    (process_method_func)daObjKAG_Create,
    (process_method_func)daObjKAG_Delete,
    (process_method_func)daObjKAG_Execute,
    (process_method_func)daObjKAG_IsDelete,
    (process_method_func)daObjKAG_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Kag = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Kag,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjKAG_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  485,                    // mPriority
  &l_daObjKAG_Method,     // sub_method
  0x000C0100,             // mStatus
  fopAc_ENV_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
