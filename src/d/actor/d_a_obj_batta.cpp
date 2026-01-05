/**
 * @file d_a_obj_batta.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_batta.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_com_inf_game.h"
#include "d/d_menu_insect.h"
#include "f_pc/f_pc_name.h"

class daObj_BattaHIO_c : public JORReflexible {
public:
    daObj_BattaHIO_c();
    virtual ~daObj_BattaHIO_c() {}

    void genMessage(JORMContext* ctx);

    /* 0x4 */ s8 mId;
    /* 0x8 */ f32 mModelScaleFemale;
    /* 0xC */ f32 mModelScaleMale;
};

namespace {

static dCcD_SrcSph cc_sph_src = {
    {
        {0x0, {{AT_TYPE_0, 0, 0}, {0x014040, 0x43}, 0}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                     // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                          // mGObjTg
        {0x0},                                                       // mGObjCo
    },                                                               // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 20.0f}  // mSph
    }  // mSphAttr
};

};

daObj_BattaHIO_c::daObj_BattaHIO_c() {
    mId = -1;
    mModelScaleMale = 1.0f;
    mModelScaleFemale = 1.4f;
}

static bool hio_set;

static daObj_BattaHIO_c l_HIO;

#if DEBUG

void daObj_BattaHIO_c::genMessage(JORMContext* ctx) {
    // Golden bugs
    ctx->genLabel("黄金蟲(バッタ)", 0x80000001, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Model scale (Male)
    ctx->genSlider("モデルスケール(オス)", &mModelScaleMale, 0.1f, 4.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Model scale (Female)
    ctx->genSlider("モデルスケール(メス)", &mModelScaleFemale, 0.1f, 4.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

#endif

void daObjBATTA_c::setAction(actionFunc i_func) {
    if (mActionFunc != NULL) {
        field_0x9e8 = -1;
        (this->*mActionFunc)();
    }

    mPrevActionFunc = mActionFunc;
    mActionFunc = i_func;
    field_0x9e8 = 0;
    (this->*mActionFunc)();
}

static u32 const l_batta_brk_index[2] = {
    15,
    14,
};

static u32 const l_batta_btk_index[2] = {
    19, 18,
};

inline int daObjBATTA_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Bat", 0xb);
    JUT_ASSERT(256, modelData != NULL);
    mpMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                  (J3DAnmTransform*)dComIfG_getObjectRes("Bat", 8), 2, 1.0f, 0, -1,
                                  &mCreature, 0x80000, 0x11000284);
    if (mpMorf == NULL ||  mpMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* morfModel = mpMorf->getModel();
    mBrk = new mDoExt_brkAnm();
    if (mBrk == NULL) {
        return 0;
    }

    ;
    if (!mBrk->init(morfModel->getModelData(), (J3DAnmTevRegKey*)dComIfG_getObjectRes("Bat", l_batta_brk_index[mSex]), 1, 2, 1.0f, 0, -1)) {
        return 0;
    }

    mBtk = new mDoExt_btkAnm();
    if (mBtk == NULL) {
        return 0;
    }

    if (!mBtk->init(morfModel->getModelData(), (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Bat", l_batta_btk_index[mSex]), 1, 2, 1.0f, 0, -1)) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return ((daObjBATTA_c*)i_this)->CreateHeap();
}

static int daObjBATTA_Create(fopAc_ac_c* i_this) {
    return ((daObjBATTA_c*)i_this)->create();
}

static int daObjBATTA_Delete(daObjBATTA_c* i_this) {
    i_this->_delete();
    return 1;
}

void daObjBATTA_c::wait() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (field_0x9e8 == 0) {
        mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Bat", 8), 2, 4.0f, 1.0f, 0.0f,
                       -1.0f);
        speedF = 0.0f;
        field_0x9f1 = 0;
        gravity = -5.0f;
        maxFallSpeed = -20.0f;
        mActionTimer = cM_rndF(160.0f) + 90.0f;
        field_0x9e8++;
    } else if (field_0x9e8 != -1) {
        if (mDraw) {
            batta_setParticle();
        }

        if (mAcch.ChkGroundHit()) {
            if (current.pos.absXZ(player->current.pos) < 200.0f && player->getSpeedF() > 19.0f)
            {
                field_0x9f1 = 1;
                setAction(&daObjBATTA_c::jump);
            } else {
                mActionTimer--;
                if (mActionTimer == 0) {
                    setAction(&daObjBATTA_c::turn);
                }
            }
        }
    }
}

void daObjBATTA_c::turn() {
    daPy_py_c* player = (daPy_py_c*)daPy_getPlayerActorClass();
    cXyz* flamePos = player->getKandelaarFlamePos();
    if (field_0x9e8 == 0) {
        if (flamePos != NULL && current.pos.absXZ(*flamePos) < 1800.0f) {
            field_0x9ec = cLib_targetAngleY(&current.pos, flamePos) + cM_rndFX(12288.0f);
        } else if (home.pos.absXZ(current.pos) < 1000.0f) {
            field_0x9ec = cM_rndFX(32768.0f);
        } else {
            field_0x9ec = cLib_targetAngleY(&current.pos, &home.pos) + cM_rndFX(8192.0f);
        }

        mActionTimer = 16;
        field_0x9e8++;
    } else if (field_0x9e8 != -1) {
        if (mDraw) {
            batta_setParticle();
        }

        if (current.pos.absXZ(player->current.pos) < 200.0f && player->getSpeedF() > 19.0f) {
            field_0x9f1 = 1;
            setAction(&daObjBATTA_c::jump);
        }
        else {
            cLib_addCalcAngleS2(&current.angle.y, field_0x9ec, 4, 0x4000);
            if (abs(current.angle.y - field_0x9ec) < 0x1000) {
                mActionTimer--;
                if (mActionTimer == 0) {
                    setAction(&daObjBATTA_c::jump);
                }
            }
        }
    }
}

void daObjBATTA_c::jump() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz* flamePos = player->getKandelaarFlamePos();
    if (field_0x9e8 == 0) {
        mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Bat", 7), 0, 4.0f, 2.0f, 0.0f,
                                 -1.0f);
        speed.y = cM_rndFX(5.0f) + 25.0f;
        speedF = cM_rndFX(4.0f) + 20.0f;

        if (flamePos == NULL || (flamePos != NULL && current.pos.absXZ(*flamePos) > 800.0f)) {
            if ((int)cM_rndF(4.0f) == 0) {
                field_0x9f1 = 1;
            }
        }
        field_0x9e8++;
    } else if (field_0x9e8 != -1) {
        if (mDraw) {
            batta_setParticle();
        }

        if (mpMorf->isStop()) {
            if (field_0x9f0 == 0 && field_0x9f1 != 0 && speed.y < -2.5f) {
                speed.y = -2.5f;
                setAction(&daObjBATTA_c::fly_up);
            } else if (mAcch.ChkGroundHit()) {
                setAction(&daObjBATTA_c::wait);
            }
        }
    }
}

void daObjBATTA_c::fly_up() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz* flamePos = player->getKandelaarFlamePos();

    if (field_0x9e8 == 0) {
        mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Bat", 6), 2, 4.0f, 0.5f, 0.0f,
                       -1.0f);
        gravity = 0.0f;
        maxFallSpeed = -20.0f;

        if (flamePos != NULL && current.pos.absXZ(*flamePos) < 1800.0f) {
            field_0x9ec = cLib_targetAngleY(&current.pos, flamePos) + cM_rndFX(8192.0f);
        } else if (home.pos.absXZ(current.pos) < 1000.0f) {
            field_0x9ec = cM_rndFX(32768.0f);
        } else {
            field_0x9ec = cLib_targetAngleY(&current.pos, &home.pos) + cM_rndFX(8192.0f);
        }

        if ((field_0x9f0 & 2) != 0) {
            field_0x9f2 = 120;
        } else {
            field_0x9f2 = cM_rndF(20.0f) + 40.0f;
        }
        field_0x9e8++;
    } else if (field_0x9e8 != -1) {
        u16 angleDiff = abs(current.angle.y - field_0x9ec);
        if (mDraw) {
            batta_setParticle();
        }

        s16 angleStep;
        if (angleDiff < 0x4000) {
            angleStep = 0x200;
        } else {
            angleStep = 0x400;
        }

        cLib_chaseAngleS(&current.angle.y, field_0x9ec, angleStep);
        cLib_chaseF(&speedF, 40.0f, 0.3f);
        cLib_chaseF(&speed.y, 6.0f, 0.2f);

        if (field_0x9f2 == 0) {
            if ((field_0x9f0 & 2) != 0) {
                fopAcM_delete(this);
            } else {
                setAction(&daObjBATTA_c::fly_down);
            }
        }

        mCreature.startCreatureSoundLevel(Z2SE_INSCT_BTBT, 0, -1);
    }
}

void daObjBATTA_c::fly_down() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz* flamePos = player->getKandelaarFlamePos();
    if (field_0x9e8 == 0) {
        gravity = -5.0f;
        maxFallSpeed = -20.0f;

        if (flamePos != NULL && current.pos.absXZ(*flamePos) < 1800.0f) {
            field_0x9ec = cLib_targetAngleY(&current.pos, flamePos) + cM_rndFX(8192.0f);
        } else {
            field_0x9ec = cM_rndFX(32768.0f);
        }

        field_0x9e8++;
    } else if (field_0x9e8 != -1) {
        if (mDraw) {
            batta_setParticle();
        }

        cLib_chaseAngleS(&current.angle.y, field_0x9ec, 0x100);
        cLib_chaseF(&speedF, 40.0f, 0.2f);

        if (mAcch.ChkGroundHit()) {
            setAction(&daObjBATTA_c::wait);
        }

        mCreature.startCreatureSoundLevel(Z2SE_INSCT_BTBT, 0, -1);
    }
}

void daObjBATTA_c::bin_wait() {
    if (field_0x9e8 == 0) {
        J3DAnmTransform* anm = (J3DAnmTransform*)dComIfG_getObjectRes("Bat", 8);
        mpMorf->setAnm(anm, 2, 4.0f, 1.0f, 0.0f, -1.0f);
        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
        mDraw = false;
        gravity = 0.0f;
        maxFallSpeed = 0.0f;
        field_0x9e8++;
    } else if (field_0x9e8 != -1 && field_0x56c != 0) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        current.angle.y = player->current.angle.y - 0x4000;

        if (strcmp("R_SP160", dComIfGp_getStartStageName()) == 0 &&
            dComIfGp_getStartStageRoomNo() == 3)
        {
            field_0x9f0 = 1;
            mDraw = true;
            mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
            mAcch.SetObj();
            mAcchCir.SetWall(10.0f, 10.0f);
            mDraw = 1;
            setAction(&daObjBATTA_c::wait);
        } else {
            setAction(&daObjBATTA_c::bin_action);
        }
    }
}

void daObjBATTA_c::bin_action() {
    if (field_0x9e8 == 0) {
        J3DAnmTransform* anm = (J3DAnmTransform*)dComIfG_getObjectRes("Bat", 6);
        mpMorf->setAnm(anm, 2, 4.0f, 0.5f, 0.0f, -1.0f);
        maxFallSpeed = -2.0f;
        gravity = -2.0f;
        scale.setall(0.4f);
        field_0x9f2 = 10;
        mDraw = true;
        field_0x9e8++;
    } else if (field_0x9e8 != -1) {
        f32 scaleValue = scale.x;
        if (field_0x9f2 != 0) {
            cLib_chaseF(&scaleValue, 0.6f, 0.1f);
        } else {
            cLib_addCalc2(&scaleValue, 0.0f, 62.0f, 0.05f);
            scale.setall(scaleValue);
            if (scaleValue < 0.1) {
                fopAcM_delete(this);
                return;
            }
        }
        scale.setall(scaleValue);
        if (mDraw) {
            batta_setParticle();
        }
    }
}

void daObjBATTA_c::hook() {
    if (field_0x9e8 == 0) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        cXyz hookOffset(0.0f, -10.0f, 0.0f);
        mSph.SetTgType(0);
        u32 actorId = fopAcM_GetID(this);
        player->setHookshotCarryOffset(actorId, &hookOffset);
        field_0x9e8++;
    } else if (field_0x9e8 == -1) {
        mSph.SetTgType(0x14040);
        home.pos = current.pos;
    } else {
        if (mDraw) {
            batta_setParticle();
        }

        if (!fopAcM_CheckStatus(this, fopAcM_STATUS_HOOK_CARRY_NOW)) {
            setAction(&daObjBATTA_c::wait);
        }
    }
}

void daObjBATTA_c::boomerang() {
    if (field_0x9e8 == 0) {
        mSph.SetTgType(0);
        dCcD_GObjInf* hitObj = mSph.GetTgHitGObj();
        mBoomerangMove.initOffset(&current.pos, hitObj);
        gravity = 0.0f;
        maxFallSpeed = 0.0f;
        field_0x9e8++;
    } else if (field_0x9e8 == -1) {
        mSph.SetTgType(0x14040);
        daPy_py_c* player = daPy_getPlayerActorClass();
        cXyz playerPos = player->current.pos;
        playerPos.y += 100.0f;
        old.pos = playerPos;
        current.pos = old.pos;
        home.pos = current.pos;
    } else {
        if (mDraw) {
            batta_setParticle();
        }

        if (mBoomerangMove.posMove(&current.pos, &current.angle.y, NULL, 0x1c00) == 0) {
            setAction(&daObjBATTA_c::wait);
        } else {
            mBoomerangMove.bgCheckAfterOffset(&current.pos);
        }
    }
}

void daObjBATTA_c::batta_setParticle() {
    if (CheckZ()) {
        cLib_chaseF(&mParticleScale, 0.0f, 1.0f);
    } else {
        cLib_chaseF(&mParticleScale, 1.0f, 1.0f);
    }

    if (field_0x9f0 != 2) {
        if (strcmp("R_SP160", dComIfGp_getStartStageName()) == 0) {
            if (strcmp("R_SP160", dComIfGp_getStartStageName()) != 0) {
                return;
            }

            if (dComIfGp_getStartStageRoomNo() == 3) {
                return;
            }
        }
        mParticleKey1 = dComIfGp_particle_set(mParticleKey1, 0xa1b, &current.pos, &tevStr,
                                              &shape_angle, NULL, 0xff, NULL, -1, NULL, NULL, NULL);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey1);
        if (emitter != NULL) {
            emitter->setGlobalParticleScale(mParticleScale * scale.x, mParticleScale * scale.x);
        }

        mParticleKey2 = dComIfGp_particle_set(mParticleKey2, 0xa1c, &current.pos, &tevStr,
                                              &shape_angle, NULL, 0xff, NULL, -1, 0, 0, 0);
        emitter = dComIfGp_particle_getEmitter(mParticleKey2);
        if (emitter != NULL) {
            emitter->setGlobalParticleScale(mParticleScale * scale.x, mParticleScale * scale.x);
        }

        mCreature.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
    }
}

void daObjBATTA_c::action() {
    fopAcM_posMoveF(this, 0);

    if (field_0x9f0 != 2) {
        if (!checkAction(&daObjBATTA_c::boomerang)) {
            mAcch.CrrPos(dComIfG_Bgsp());
        }
    }

    (this->*mActionFunc)();
    shape_angle = current.angle;

    if (field_0x9f0 != 2 && field_0x9f0 == 0) {
        Insect_GetDemoMain();
    }
}

void daObjBATTA_c::hit_check() {
    dCcU_AtInfo info;
    mStts.Move();
    
    if (mSph.ChkTgHit()) {
        info.mpCollider = mSph.GetTgHitObj();
        if (info.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
            setAction(&daObjBATTA_c::hook);
        } else {
            setAction(&daObjBATTA_c::boomerang);
        }
    }
}

static u8 const l_batta_sex[2] = {fpcNm_ITEM_M_GRASSHOPPER, fpcNm_ITEM_F_GRASSHOPPER};

int daObjBATTA_c::execute() {
    if ( ChkGetDemo()) {
        field_0x57c = field_0x578 + 10000.0f;
        mCreature.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
        Insect_GetDemoMain();
        batta_setParticle();
        return 1;
    }
    
    if (field_0x9f2 != 0) {
        field_0x9f2--;
    }

    if (field_0x9f0 == 0) {
        hit_check();
    }

    action();
    mBrk->play();
    mBtk->play();
    mpMorf->play(NULL, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mtx_set();
    if (field_0x9f0 == 0) {
        mSph.SetC(current.pos);
        mSph.SetR(scale.x * 20.0f);
        cCcS* ccS = dComIfG_Ccsp();
        ccS->Set(&mSph);
        eyePos = current.pos;
        attention_info.position = eyePos;
    }
    return 1;
}

int daObjBATTA_c::_delete() {
    dComIfG_resDelete(&mPhase, "Bat");

    if (mIsHIOOwner != 0) {
        hio_set = false;
        mDoHIO_DELETE_CHILD(l_HIO.mId);
    }

    if (heap != NULL) {
        mpMorf->stopZelAnime();
    }

    return 1;
}

void daObjBATTA_c::mtx_set() {
    J3DModel* model = mpMorf->getModel();
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    model->setBaseScale(scale);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
}

inline int daObjBATTA_c::draw() {
    if (mDraw) {
        dInsect_c::CalcZBuffer(50.0f);
        J3DModel* model = mpMorf->getModel();
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);

        mBtk->entry(model->getModelData());
        mBrk->entry(model->getModelData());

        if (field_0x9f0 != 2) {
            mpMorf->entryDL();
            if (field_0x9f0 != 1) {
                dComIfGd_setSimpleShadow(&current.pos, mAcch.GetGroundH(), 15.0f, mAcch.m_gnd, 0,
                                         -0.6f, dDlst_shadowControl_c::getSimpleTex());
            }
        }
    }

    return 1;
}

static int daObjBATTA_Draw(daObjBATTA_c* i_this) {
    return i_this->draw();
}

static int daObjBATTA_Execute(daObjBATTA_c* i_this) {
    return i_this->execute();
}

static u16 const l_musiya_num[2] = {
    0x197, /* dSv_event_flag_c::F_0407 - Misc. - Grasshopper (M) */
    0x198, /* dSv_event_flag_c::F_0408 - Misc. - Grasshopper (F) */
};

int daObjBATTA_c::create() {
    u8 uVar9 = (fopAcM_GetParam(this) & 0xf00) >> 8;

    fopAcM_ct(this, daObjBATTA_c);
    int rv = dComIfG_resLoad(&mPhase, "Bat");
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("BATTA PARAM %x\n", fopAcM_GetParam(this));
        field_0x9f0 = fopAcM_GetParam(this) & 3;
        if (field_0x9f0 == 3) {
            field_0x9f0 = 0;
        }
        
        mSex = (fopAcM_GetParam(this) & 0x10) >> 4;
        if (mSex != 0) {
            scale.setall(l_HIO.mModelScaleFemale);
        } else {
            scale.setall(l_HIO.mModelScaleMale);
        }
        
        if (uVar9 == 15) {
            uVar9 = 0;
        }
        
        setItemNo(l_batta_sex[mSex]);
        setSaveBitNo(l_musiya_num[mSex]);
        
        if (field_0x9f0 != 2) {
            if (uVar9 != 0) {
                if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[SEX_MALE]]) || !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[SEX_FEMALE]])) {
                    return cPhs_ERROR_e;
                }
            } else {
                if (strcmp("R_SP160", dComIfGp_getStartStageName()) == 0 &&
                    dComIfGp_getStartStageRoomNo() == 3)
                {
                    if (dMenu_Insect_c::isCatchNotGiveInsect(m_itemNo)) {
                        return cPhs_ERROR_e;
                    }
                    if ((mSex == SEX_MALE &&
                        !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[SEX_MALE]])) ||
                        (mSex != SEX_MALE &&
                        !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[SEX_FEMALE]])))
                    {
                        return cPhs_ERROR_e;
                    }
                }
            }
        }
        
        mParticleScale = 0.0f;
        
        OS_REPORT("/////////////////// BATTA ////////////////////////\n");

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0xb00)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("/////////////////// BATTA SET OK /////////////////\n");
        
        if (!hio_set) {
            hio_set = true;
            mIsHIOOwner = true;
            // Golden Bugs
            l_HIO.mId = mDoHIO_CREATE_CHILD("黄金蟲(バッタ)", &l_HIO);
        }
        
        J3DModel* model = mpMorf->getModel();
        fopAcM_SetMtx(this, model->getBaseTRMtx());
        fopAcM_SetMin(this, -50.0f, -50.0f, -50.0f);
        fopAcM_SetMax(this, 50.0f, 50.0f, 50.0f);
        
        attention_info.flags = 0;
        health = 10;
        field_0x560 = 10;
        
        mCreature.init(&current.pos, &eyePos, 3, 1);
        
        if (field_0x9f0 == 0) {
            attention_info.distances[fopAc_attn_CARRY_e] = 0x5d;
            mStts.Init(100, 0, this);
            mSph.Set(cc_sph_src);
            mSph.SetStts(&mStts);
        }
        
        if (field_0x9f0 == 2) {
            setAction(&daObjBATTA_c::bin_wait);
        } else {
            mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
            mAcch.SetObj();
            mAcchCir.SetWall(10.0f, 10.0f);
            gravity = -5.0f;
            maxFallSpeed = -20.0f;
            current.angle.y = cM_rndFX(32768.0f);
            setAction(&daObjBATTA_c::wait);
        }
        
        daObjBATTA_Execute(this);
    }
    return rv;
}

static int daObjBATTA_IsDelete(daObjBATTA_c* i_this) {
    return 1;
}

static actor_method_class l_daObjBATTA_Method = {
    (process_method_func)daObjBATTA_Create,
    (process_method_func)daObjBATTA_Delete,
    (process_method_func)daObjBATTA_Execute,
    (process_method_func)daObjBATTA_IsDelete,
    (process_method_func)daObjBATTA_Draw,
};

actor_process_profile_definition g_profile_Obj_Batta = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Batta,         // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjBATTA_c),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  486,                    // mPriority
  &l_daObjBATTA_Method,   // sub_method
  0x000C0120,             // mStatus
  fopAc_ENV_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
