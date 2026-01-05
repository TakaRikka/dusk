/**
 * @file d_a_e_fz.cpp
 * @brief Enemy - Mini Freezard
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_fz.h"
#include "f_op/f_op_actor_enemy.h"
#include "d/actor/d_a_mirror.h"
#include "d/actor/d_a_b_yo.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "SSystem/SComponent/c_math.h"
#include "SSystem/SComponent/c_xyz.h"

class daE_FZ_HIO_c {
public:
    daE_FZ_HIO_c();
    virtual ~daE_FZ_HIO_c() {};

public:
    /* 0x04 */ s8 field_0x04;
    /* 0x05 */ u8 field_0x05[0x06 - 0x05];
    /* 0x06 */ s16 field_0x06;
    /* 0x08 */ s16 field_0x08;
    /* 0x0A */ u8 field_0x0A[0x0C - 0x0A];
    /* 0x0C */ f32 field_0x0c;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
};

namespace {

static dCcD_SrcSph cc_fz_src = {
    {
        {0x0, {{0x100, 0x1, 0x0}, {0xd0fbfdff, 0x43}, 0x65}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x6}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

static dCcD_SrcSph cc_fz_at_src = {
    {
        {0x0, {{0x100, 0x1, 0x1d}, {0x0, 0x0}, 0x0}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

}  // namespace

daE_FZ_HIO_c::daE_FZ_HIO_c() {
    field_0x04 = -1;
    field_0x0c = 1.4f;
    field_0x10 = 1000.0f;
    field_0x14 = 800.0f;
    field_0x18 = 350.0f;
    field_0x1c = 2.0f;
    field_0x20 = 6.0f;
    field_0x24 = 10.0f;
    field_0x28 = 50.0f;
    field_0x2c = 3.0f;
    field_0x06 = 0x28;
    field_0x30 = 40.0f;
    field_0x08 = 0x78;
    field_0x34 = 80.0f;
}

s32 daE_FZ_c::draw() {
    if (field_0x714 == 2 && !checkItemGet(fpcNm_ITEM_IRONBALL,1)) {
        return 1;
    }

    J3DModel* model = mpModel;

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel->mModelData, &tevStr);

    mDoExt_modelUpdateDL(mpModel);
    daMirror_c::entry(mpModel);

    cXyz pos;

    pos.set(current.pos.x, current.pos.y + 10.0f, current.pos.z);
    field_0x70c = dComIfGd_setShadow(field_0x70c, 1, model, &pos, 300.0f, 0.0f, current.pos.y,
                                     mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr,
                                     0, 1.0f, &dDlst_shadowControl_c::mSimpleTexObj);
    return 1;
}

static void daE_FZ_Draw(daE_FZ_c* i_this) {
    i_this->draw();
}

void daE_FZ_c::setActionMode(int i_actionMode, int i_actionPhase) {
    mActionMode = i_actionMode;
    mActionPhase = i_actionPhase;
}

void daE_FZ_c::setReflectAngle() {
    s16 x = current.angle.y - mLastWallHitAngle;

    if (abs(x) > 18432) {
        current.angle.y = (mLastWallHitAngle << 1) - (current.angle.y + 32768);
    } else {
        if (x > 14336) {
            current.angle.y = mLastWallHitAngle + 14336;
        } else {
            if (x < -14336) {
                current.angle.y = mLastWallHitAngle - 14336;
            }
        }
    }
}

void daE_FZ_c::mBoundSoundset() {
    u32 speed = speedF;

    if (speed < 1)
        speed = 1;

    mCreature.startCreatureSound(Z2SE_EN_FZ_BOUND,speed,-1);
}

void daE_FZ_c::deadnextSet(bool param_0) {
    health = 0;

    if (!param_0) {
        if (field_0x714 != 3) {
            dComIfGp_getVibration().StartShock(2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        }
        mCreature.startCollisionSE(Z2SE_HIT_HAMMER, 0x20);
    }

    mTgCoSph.ClrTgHit();
    fopAcM_OffStatus(this,0);
    attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;

    mAtSph.OffAtSetBit();
    mTgCoSph.OffTgSetBit();

    mCreature.startCreatureSound(Z2SE_EN_FZ_DEATH, 0, -1);

    speedF = 0.0f;
    field_0x6fc = 0;
    setActionMode(ACT_DAMAGE,0);
}

static u8 data_806C1BA0;

static daE_FZ_HIO_c l_HIO;

void daE_FZ_c::damage_check() {
  csXyz s_pos;
  cXyz pos;
  cXyz pos2;
  cXyz pos3;

  if (1 < health) {
    scale.set(l_HIO.field_0x0c, l_HIO.field_0x0c, l_HIO.field_0x0c);
    setMidnaBindEffect(this, &mCreature, &current.pos, &scale);

    if (field_0x712 == 0) {
      pos.set(dComIfGp_getPlayer(0)->current.pos);
      mStts.Move();
      
      if (field_0x714 == 3) {
        if (mTgCoSph.ChkTgHit()) {
          mAtInfo.mpCollider = mTgCoSph.GetTgHitObj();
          
          if (mTgCoSph.GetTgHitObj()->ChkAtType(AT_TYPE_IRON_BALL)) {
            deadnextSet(false);
          }
        }
      } else {
        if (mTgCoSph.ChkTgHit()) {
          mAtInfo.mpCollider = mTgCoSph.GetTgHitObj();
          
          if (mTgCoSph.GetTgHitObj()->ChkAtType(AT_TYPE_40) || mTgCoSph.GetTgHitObj()->ChkAtType(AT_TYPE_BOOMERANG)) {
            current.angle.y = fopAcM_searchPlayerAngleY(this) + 32768;
            f32 tmp_l_hio = l_HIO.field_0x28;
            speedF = tmp_l_hio;
            field_0x6fc = tmp_l_hio;
            mBoundSoundset();
            setActionMode(ACT_DAMAGE,1);
            return;
          }

          pos2 = current.pos - *mTgCoSph.GetTgHitPosP();
          pos3.set(*mTgCoSph.GetTgHitPosP());
          
          s_pos.x = 0;
          s_pos.y = pos2.atan2sX_Z();
          s_pos.z = 0;

          if (mTgCoSph.GetTgHitObj()->ChkAtType(AT_TYPE_SPINNER) || mTgCoSph.GetTgHitObj()->ChkAtType(AT_TYPE_ARROW)) {
            current.angle.y = fopAcM_searchPlayerAngleY(this) + 32768;
            f32 tmp_l_hio = l_HIO.field_0x28;
            speedF = tmp_l_hio;
            field_0x6fc = tmp_l_hio;
            mBoundSoundset();
            dComIfGp_setHitMark(2,this,&pos3,&s_pos,0,AT_TYPE_0);
            setActionMode(ACT_DAMAGE,1);
            return;
          }

          cXyz cStack_54(l_HIO.field_0x0c, l_HIO.field_0x0c, l_HIO.field_0x0c);
          dComIfGp_particle_set(0x85ba, &current.pos, &shape_angle, &cStack_54);
              
          if (mTgCoSph.GetTgHitObj()->ChkAtType(AT_TYPE_HOOKSHOT)) {
            health -= 20;

            if (1 < health) {
              current.angle.y = fopAcM_searchPlayerAngleY(this) + 32768;
              
              f32 tmp_l_hio = l_HIO.field_0x28;
              speedF = tmp_l_hio;
              field_0x6fc = tmp_l_hio;
              mCreature.startCreatureSound(Z2SE_EN_FZ_DAMAGE,0,-1);

              f32 tmp_l_hio2 = l_HIO.field_0x28;
              speedF = tmp_l_hio2;
              field_0x6fc = tmp_l_hio2;
              setActionMode(ACT_DAMAGE,1);
              dComIfGp_setHitMark(3,this,&pos3,&s_pos,0,AT_TYPE_0);
              return;
            }

            deadnextSet(true);
            dComIfGp_setHitMark(1,this,&pos3,&s_pos,0,AT_TYPE_0);
            return;
          }

          if (mTgCoSph.GetTgHitObj()->ChkAtType(AT_TYPE_IRON_BALL)) {
            deadnextSet(false);
            mTgCoSph.ClrTgHit();
            dComIfGp_setHitMark(3,this,&pos3,&s_pos,0,AT_TYPE_0);
            return;
          }

          cc_at_check(this,&mAtInfo);

          if (mAtInfo.mHitStatus == 0) {
            dComIfGp_setHitMark(1,this,&pos3,&s_pos,0,AT_TYPE_0);
          } else {
            dComIfGp_setHitMark(3,this,&pos3,&s_pos,0,AT_TYPE_0);
          }

          mLastWallHitAngle = mAtInfo.mHitDirection.y;
          setReflectAngle();
          current.angle.y += -32768;
          field_0x712 = 10;

          if (1 < health) {
            mCreature.startCreatureSound(Z2SE_EN_FZ_DAMAGE,0,-1);
            f32 tmp_l_hio = l_HIO.field_0x28;
            speedF = tmp_l_hio;
            field_0x6fc = tmp_l_hio;
            setActionMode(ACT_DAMAGE,1);
            return;
          }

          deadnextSet(true);
          return;     
        } else {        
          if (mObjAcch.ChkGroundHit() && mTgCoSph.ChkCoHit()) {
            fopAc_ac_c* co_hit_actor = mTgCoSph.GetCoHitAc();

            if (fopAcM_IsActor(co_hit_actor) && fopAcM_GetName(co_hit_actor) == PROC_E_FZ) {
              pos = current.pos - mTgCoSph.GetCoHitAc()->current.pos;
              mTgCoSph.ClrCoHit();
              f32 co_hit_actor_speed = co_hit_actor->speedF;

              if (co_hit_actor_speed  > l_HIO.field_0x28 * 0.2f || speedF > l_HIO.field_0x28 * 0.2f) {
                pos = current.pos - co_hit_actor->current.pos;
                mLastWallHitAngle = pos.atan2sX_Z();
                setReflectAngle();

                f32 tmp2 = speedF;
                f32 tmp = co_hit_actor->speedF;

                if (speedF > tmp) {
                  co_hit_actor->speedF = tmp2;
                  static_cast<daE_FZ_c*>(co_hit_actor)->field_0x6fc = tmp2;
                } else {
                  speedF = tmp;
                  field_0x6fc = tmp;
                }

                mBoundSoundset();
                setActionMode(ACT_DAMAGE,5);
                return;
              }
            }
          }
          
          if (mAtSph.ChkAtHit()) {
            fopAc_ac_c* player = dComIfGp_getPlayer(0);
            fopAc_ac_c* at_hit_actor = mAtSph.GetAtHitAc();

            current.angle.y = fopAcM_searchPlayerAngleY(this) + 32768;

            if (player != at_hit_actor) {
              mAtSph.ClrAtHit();
            } else {
              if (mAtSph.ChkAtShieldHit()) {
                f32 l_hio_28 = l_HIO.field_0x28;
                speedF = l_hio_28;
                field_0x6fc = l_hio_28;
                setActionMode(ACT_DAMAGE,1);
                
              } else {
                if (mActionMode != ACT_DAMAGE) {
                  field_0x712 = 10;
                  setActionMode(ACT_DAMAGE,3);
                }
              }
              mBoundSoundset();
              mAtSph.ClrAtHit();
            }
          }
        }
      }
    }
  }
}

bool daE_FZ_c::way_gake_check() {
    cXyz pos;
    dBgS_GndChk gnd_chk;
    
    if (mObjAcch.ChkWallHit()) {
        return false;
    }

    cMtx_YrotS((MtxP)calc_mtx,current.angle.y);

    pos.x = 0.0f;
    pos.y = 300.0f;
    pos.z = 70.0f;

    MtxPosition(&pos,&field_0x6dc);

    field_0x6dc += current.pos;
    field_0x6e8.set(field_0x6dc);
    gnd_chk.SetPos(&field_0x6e8);
    
    field_0x6e8.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (field_0x6e8.y == -G_CM3D_F_INF) {
        field_0x6e8.y = current.pos.y;
        return true;
    } else if (current.pos.y - field_0x6e8.y > 100.0f) {
        return true;
    }

    return false;
}

void daE_FZ_c::executeWait() {
  cXyz pos;
  cXyz pos2;
  s16 angle;
  f32 tmp = l_HIO.field_0x14;
  
  switch (mActionPhase) {
  case 0:
    if (fopAcM_wayBgCheck(this,200.0f,50.0f)) {
      angle = cM_rndFX(10000.0f) + 32768.0f;
    } else {
      
      pos2.x = home.pos.x + cM_rndFX(l_HIO.field_0x10);
      pos2.y = home.pos.y;
      pos2.z = home.pos.z + cM_rndFX(l_HIO.field_0x10);
      
      pos = pos2 - current.pos;
      
      angle = pos.atan2sX_Z() - current.angle.y;

      if (angle > 12288) {
        angle = 12288;
      }
      else {
        if (angle < -12288)
          angle = -12288;
      }
    }

    mAngleFromPlayer = current.angle.y + angle;
    field_0x710 = l_HIO.field_0x06 + cM_rndFX(l_HIO.field_0x30);
    mActionPhase = 1;
  case 1:
    if (way_gake_check()) {
      pos2 = current.pos - home.pos;
      angle = pos2.atan2sX_Z();
      mAngleFromPlayer = angle;
      current.angle.y = angle;
    }

    if (field_0x714 == 4) {
      field_0x710 = 10;
      speedF = 0.0f;
      angle = fopAcM_searchPlayerAngleY(this);
      mAngleFromPlayer = angle;
      current.angle.y = angle;
      tmp = l_HIO.field_0x18;
    }

    if (mObjAcch.ChkGroundHit() && dComIfG_Bgsp().GetPolyAtt0(mObjAcch.m_gnd) == 8) {
        angle = shape_angle.y - mAngleFromPlayer;
    
        if (abs(angle) < 512 && field_0x710 == 0) {
          cLib_addCalc0(&speedF, 0.1f, 0.1f);
        }  
    } else {
        cLib_addCalc0(&speedF,0.1f,l_HIO.field_0x2c);
    }
    
    if (field_0x710 == 0 && speedF < 0.2f) {
      angle = shape_angle.y - mAngleFromPlayer;

      if (abs(angle) < 512) {
        current.angle.y = shape_angle.y;
        setActionMode(ACT_MOVE,0);
      } 
    }
  }

  cLib_addCalcAngleS2(&shape_angle.y,mAngleFromPlayer,8,1280);

  if (fopAcM_searchPlayerDistance(this) <= tmp && !way_gake_check()) {
    if (!fopAcM_otherBgCheck(this,dComIfGp_getPlayer(0))) {
      current.angle.y = shape_angle.y;
      setActionMode(ACT_ATTACK,0);
    }
  }
}

void daE_FZ_c::executeMove() {
    switch (mActionPhase) {
        case 0:
            field_0x710 = l_HIO.field_0x08 + cM_rndFX(l_HIO.field_0x34);
            mActionPhase = 1; 
        case 1:
            cLib_addCalcAngleS2(&current.angle.y,mAngleFromPlayer,8,256);
            cLib_addCalc2(&speedF,l_HIO.field_0x1c,1.0f,3.0f);
            
            if (fopAcM_wayBgCheck(this, 200.0f,50.0f) != 0 || field_0x710 == 0) {
                setActionMode(ACT_WAIT,0);
            }
        default:
            if (way_gake_check()) {
                speedF = 0;
                setActionMode(ACT_WAIT,0);
            } else {
                shape_angle.y = current.angle.y;
                
                if (fopAcM_searchPlayerDistance(this) <= l_HIO.field_0x14) {
                    setActionMode(ACT_ATTACK,0);
                }
            }
    }
}

void daE_FZ_c::executeAttack() {
    switch (mActionPhase) {
    case 0:
        cLib_addCalcAngleS2(&current.angle.y,fopAcM_searchPlayerAngleY(this),8,0x300);
        if (way_gake_check() == 0) {
            cLib_addCalc2(&speedF,l_HIO.field_0x20,0.7f,1.0f);
        } else {
            speedF = 0.0f;
        }
    default:
        shape_angle.y = current.angle.y;
        if (!(fopAcM_searchPlayerDistance(this) >= l_HIO.field_0x10)) {
            if (fopAcM_otherBgCheck(this,dComIfGp_getPlayer(0)) == 0) {
                return;
            }
        }
    }

    setActionMode(ACT_WAIT,0);
}

void daE_FZ_c::executeDamage() {
  cXyz pos;
  pos.set(l_HIO.field_0x0c, l_HIO.field_0x0c, l_HIO.field_0x0c);
  f32 tmp;

  switch(mActionPhase) {
  case 0:
    dComIfGp_particle_set(0x85b8,&current.pos,&shape_angle,&pos);
    dComIfGp_particle_set(0x85b9,&current.pos,&shape_angle,&pos);

    if (field_0x714 != 3) {
      field_0x564 = 25;
      fopAcM_createItemFromEnemyID(field_0x564,&current.pos,-1,-1,0,0,0,0);
    } else {
      if (cM_rnd() < 0.2f) {
        fopAcM_createItem(&current.pos,0,-1,-1,0,0,0);
      }
    }
    fopAcM_delete(this);
    break;
  case 1:
    tmp = l_HIO.field_0x28; 
    speedF = tmp;
    field_0x6fc = tmp;
  case 5:
    mStts.SetWeight(118);
    current.angle.y < 0 ? field_0x704 = 0 : field_0x704 = 1;
    mActionPhase = 2;
  case 2:
    if (mObjAcch.ChkGroundHit() && dComIfG_Bgsp().GetPolyAtt0(mObjAcch.m_gnd) == 8) {
      tmp = 0.2f;
    } else {
      tmp = 1.0f;
    }

    cLib_addCalc0(&speedF,0.1f,tmp);

    if (field_0x704 == 0) {
      s16 value = 4096.0f - (4096.0f / field_0x6fc) * (field_0x6fc - speedF);
      shape_angle.y -= value;
    } else {
      s16 value = 4096.0f - (4096.0f / field_0x6fc) * (field_0x6fc - speedF);
      shape_angle.y += value;
    }

    if (mObjAcch.ChkWallHit()) {
      mLastWallHitAngle = mAcchCir.GetWallAngleY();
      setReflectAngle();
      mBoundSoundset();
    }

    if (speedF < 0.3f) {
      current.angle.y = shape_angle.y;
      mStts.SetWeight(100);
      setActionMode(ACT_WAIT,0);
    }
    break;
  case 3:
    mAngleFromPlayer = fopAcM_searchPlayerAngleY(this);
    if (current.angle.y < 0) {
      field_0x704 = 0;
    }
    else {
      field_0x704 = 1;
    }
    tmp = l_HIO.field_0x24;
    speedF = tmp;
    field_0x6fc = tmp;
    mActionPhase = 4;
  case 4:
    if (field_0x704 == 0) {
      s16 value = 4096.0f - (4096.0f / field_0x6fc) * (field_0x6fc - speedF);
      shape_angle.y -= value;
    }
    else {
      s16 value = 4096.0f - (4096.0f / field_0x6fc) * (field_0x6fc - speedF);
      shape_angle.y += value;
    }
    
    cLib_addCalcAngleS2(&current.angle.y,mAngleFromPlayer,1,512);
    cLib_addCalc0(&speedF,0.1f,0.1f);
    
    if (mObjAcch.ChkWallHit()) {
      mLastWallHitAngle = mAcchCir.GetWallAngleY();
      setReflectAngle();
      mBoundSoundset();
    }

    if (speedF < 0.2f) {
      current.angle.y = shape_angle.y;
      setActionMode(ACT_ATTACK,0);
    }
    break;
  case 6:
    health = 0;
    if (field_0x710 == 1 || mObjAcch.ChkGroundHit()) {
      mActionPhase = 0;
    }
  }
}

void daE_FZ_c::executeRollMove() {
    if (fopAcM_SearchByID(fopAcM_GetLinkId(this),&mpBlizzetaActor) == 0 || !mpBlizzetaActor) return;
    u32 model_no = static_cast<daB_YO_c*>(mpBlizzetaActor)->getModelNo();
    
    if (model_no < 4 || 6 < model_no) {
        fopAcM_delete(this);
        return;
    }

    cXyz pos;

    s16 roll_angle = static_cast<daB_YO_c*>(mpBlizzetaActor)->getFrizadRollAngle();
    f32 mode_rarius = 100.0f + static_cast<daB_YO_c*>(mpBlizzetaActor)->getModeRarius();
    if (mode_rarius < 400.0f)
        mode_rarius = 400.0f;

    switch (mActionPhase) {
    case 0:
        field_0x704 = 0;
        speedF = 0.0f;
        mRadiusBase = 0;
        mActionPhase = 1;
        field_0x710 = (20 - field_0x715) * 2;
    case 1:
        if (field_0x710 == 0) {
            cLib_chaseF(&mRadiusBase,1.0f,0.03f);
        }

        cLib_chaseAngleS(&field_0x704,1024,16);
        pos = mpBlizzetaActor->current.pos;

        pos.x += (f32)(mode_rarius * cM_ssin(roll_angle + field_0x715 * 0xccc));
        pos.z += (f32)(mode_rarius * cM_scos(roll_angle + field_0x715 * 0xccc));

        current.pos = pos;

        if (static_cast<daB_YO_c*>(mpBlizzetaActor)->getFrizadAttack() == 3) {
            mActionPhase = 2;
            speedF = 60.0f;
            current.angle.y = cLib_targetAngleY(&static_cast<daB_YO_c*>(mpBlizzetaActor)->current.pos,&current.pos);
        }
        break;
    case 2:
        cLib_chaseF(&mRadiusBase,1.0,0.1);
        cLib_chaseAngleS(&field_0x704,512,16);
        
        if (mObjAcch.ChkWallHit() || !mObjAcch.ChkGroundHit()) {
            setActionMode(ACT_DAMAGE,0);
            mCreature.startCreatureSound(Z2SE_EN_FZ_DEATH,0,-1);
            return;
        }

        if (mAtSph.ChkAtHit()) {
            fopAc_ac_c* at_hit_actor = mAtSph.GetAtHitAc();
            if ((fopAcM_GetName(at_hit_actor) == PROC_ALINK) || mAtSph.ChkAtShieldHit()) {
                setActionMode(ACT_DAMAGE,0);
                return;
            }
        }
    }

    shape_angle.y += field_0x704;
}

void daE_FZ_c::action() {
    int linkSearch;
    int i;

    if (field_0x714 == 1 && mObjAcch.ChkGroundHit()) {
        fopAcM_OffStatus(this, 0x4000);
        field_0x714 = 0;
    }

    if (!fopAcM_otherBgCheck(this, dComIfGp_getPlayer(0))) {
        fopAcM_OnStatus(this, 0);
        attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(this, 0);
        attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
    }

    linkSearch = false;
    damage_check();

    switch (mActionMode) {
    case ACT_WAIT:
        executeWait();
        break;
    case ACT_MOVE:
        executeMove();
        break;
    case ACT_ATTACK:
        executeAttack();
        linkSearch = 1;
        break;
    case ACT_DAMAGE:
        executeDamage();
        break;
    case ACT_ROLLMOVE:
        executeRollMove();
        linkSearch = 1;
    }

    mCreature.setLinkSearch(linkSearch);
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    field_0x714 == 3 ? mAcchCir.SetWall(35.0f, 70.0f) : mAcchCir.SetWall(35.0f, 60.0f);
    mObjAcch.CrrPos(dComIfG_Bgsp());

    if (1 < health) {
        u32 speed = speedF;

        if (speed < 1)
            speed = 1;

        mCreature.startCreatureSoundLevel(Z2SE_EN_FZ_MOVE, speed, -1);

        cXyz mtxPos;
        for (int i = 0; i < 4; i++) {
            cMtx_YrotS((MtxP)calc_mtx, (current.angle.y & 0xffffc000) + (i * 0x4000));
            mtxPos.x = 0.0f;
            mtxPos.y = 100.0f;
            mtxPos.z = 70.0f;
            MtxPosition(&mtxPos, &field_0x67c[i]);
            field_0x67c[i] += current.pos;

            mtxPos.x = 0.0f;
            mtxPos.y = 100.0f;
            mtxPos.z = 40.0f;
            MtxPosition(&mtxPos, &field_0x6ac[i]);
            field_0x6ac[i] += current.pos;
        }

        if (1 < health && mAcchCir.ChkWallHit()) {
            if (fopAcM_GetName(dComIfG_Bgsp().GetActorPointer(mAcchCir)) != PROC_BG) {
                dBgS_LinChk lin_chk;
                dBgS_LinChk lin_chk2;

                for (i = 0; i < 2; i++) {
                    lin_chk.Set(&field_0x67c[i], &field_0x6ac[i], this);
                    lin_chk2.Set(&field_0x67c[i + 2], &field_0x6ac[i + 2], this);

                    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                        if (dComIfG_Bgsp().LineCross(&lin_chk2)) {
                            deadnextSet(false);
                            break;
                        }
                    }
                }
            }
        }

        if (field_0x714 != 3) {
            if (health > 1 && !mObjAcch.ChkGroundHit()) {
                dBgS_GndChk gnd_chk;
                cXyz pos;

                pos.set(current.pos);
                pos.y += 300.0f;
                gnd_chk.SetPos(&pos);
                pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

                if (pos.y != -G_CM3D_F_INF) {
                    field_0x710 = 0;

                    if (current.pos.y - pos.y > 400.0f && field_0x713 == 0) {
                        field_0x713 = 1;
                    }
                } else {
                    field_0x710 = 0xfa;
                    setActionMode(ACT_DAMAGE, 6);
                }

                if (field_0x713 != 0) {
                    field_0x713++;

                    if (10 < field_0x713) {
                        setActionMode(ACT_DAMAGE, 6);
                    }
                }
            } else {
                field_0x713 = 0;
            }
        }
    }
}

void daE_FZ_c::mtx_set() {
  mDoMtx_stack_c::transS(current.pos.x,current.pos.y,current.pos.z);
  mDoMtx_stack_c::ZXYrotM(shape_angle);
  mDoMtx_stack_c::scaleM(l_HIO.field_0x0c,l_HIO.field_0x0c,l_HIO.field_0x0c);
  mDoMtx_stack_c::scaleM(mRadiusBase,mRadiusBase,mRadiusBase);
  mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daE_FZ_c::cc_set() {
    cXyz pos;
    cXyz pos2;

    pos.set(0.0f,60.0f,0.0f);
    mDoMtx_stack_c::multVec(&pos,&eyePos);
    attention_info.position = eyePos;
    attention_info.position.y += 25.0f;

    mDoMtx_stack_c::copy(mpModel->getBaseTRMtx());

    pos.set(0.0f,40.0f,0.0f);
    mDoMtx_stack_c::multVec(&pos,&pos2);

    mTgCoSph.SetC(pos2);
    mTgCoSph.SetR(mRadiusBase * 60.0f);

    dComIfG_Ccsp()->Set(&mTgCoSph);
    
    mDoMtx_stack_c::copy(mpModel->getBaseTRMtx());

    pos.set(0.0f,25.0f,0.0f);
    mDoMtx_stack_c::multVec(&pos,&pos2);

    mAtSph.SetC(pos2);
    mAtSph.SetR(mRadiusBase * 40.0f);

    dComIfG_Ccsp()->Set(&mAtSph);
}

s32 daE_FZ_c::execute() {
    if (field_0x714 == 2) {
        if (checkItemGet(fpcNm_ITEM_IRONBALL,1) == 0) {
            return 1;
        }

        if (attention_info.distances[fopAc_attn_BATTLE_e] == 0) {
            attention_info.distances[fopAc_attn_BATTLE_e] = 69;
            fopAcM_SetGroup(this,2);
            #if DEBUG
            fopAcM_OnStatus(this,0);
            #endif
            attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
        }
    }

    if (field_0x710 != 0)
        field_0x710 -= 1;

    if (field_0x711 != 0)
        field_0x711 -= 1;

    if (field_0x712 != 0)
        field_0x712 -= 1;

    action();  // set current action
    mtx_set(); // update model matrix
    cc_set();  // update sphere colliders

    mCreature.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    mUserWork = current.pos - old.pos;
    mUserWork *= 0.8f;

    static u16 ice_name[3] = {0x8221, 0x8222, 0x8223};
    for (int i = 0; i < 3; i++) {
        mParticleSet[i] = dComIfGp_particle_set(mParticleSet[i], ice_name[i], &current.pos, 0, 0);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleSet[i]);

        if (emitter) {
            mDoMtx_stack_c::copy(mpModel->getBaseTRMtx());
            mDoMtx_stack_c::transM(0.0f, 40.0f, 0.0f);
            emitter->setGlobalSRTMatrix(mDoMtx_stack_c::get());
            emitter->setParticleCallBackPtr(dPa_control_c::getParticleTracePCB());
            emitter->setUserWork((uintptr_t)(&mUserWork));
        }
    }

    return 1;
}

static void daE_FZ_Execute(daE_FZ_c* i_this) {
    i_this->execute();
}

void daE_FZ_c::demoDelete() {
    for (int i = 0; i < 3; i++) {
        dComIfGp_particle_levelEmitterOnEventMove(mParticleSet[i]);
    }

    fopAcM_delete(this);
}

static bool daE_FZ_IsDelete(daE_FZ_c* i_this) {
    return true;
}

s32 daE_FZ_c::_delete() {
    dComIfG_resDelete(&mPhaseReq,"E_FZ");

    if (field_0xc21 != 0) {
        data_806C1BA0 = 0;
    }

    if (heap) {
        mCreature.deleteObject();
    }

    return 1;
}

static void daE_FZ_Delete(daE_FZ_c* i_this) {
    i_this->_delete();
}

s32 daE_FZ_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("E_FZ", 3);
    mpModel = mDoExt_J3DModel__create(model_data, 0, 0x11020203);

    if (mpModel == 0) {
        return 0;
    }

    return mInvisibleModel.create(mpModel, 1) != 0 ? 1 : 0;
}

static int useHeapInit(fopAc_ac_c* i_this) {
  return static_cast<daE_FZ_c*>(i_this)->CreateHeap();
}

s32 daE_FZ_c::create() {
  fopAcM_ct(this,daE_FZ_c);

  s32 phase = dComIfG_resLoad(&mPhaseReq,"E_FZ");
  if (phase == cPhs_COMPLEATE_e) {
    if (!fopAcM_entrySolidHeap(this,useHeapInit,6480)) {
      return cPhs_ERROR_e;
    }
    if (data_806C1BA0 == 0) {
      data_806C1BA0 = 1;
      field_0xc21 = 1;
      l_HIO.field_0x04 = -1;
    }

    attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    attention_info.distances[fopAc_attn_BATTLE_e] = 69;
    
    fopAcM_SetMtx(this,mpModel->getBaseTRMtx());
    fopAcM_SetMin(this,-200.0f,-200.0f,-200.0f);
    fopAcM_SetMax(this,200.0f,200.0f,200.0f);

    mStts.Init(100,0,this);
    health = 80;
    field_0x560 = 80;

    field_0x714 = fopAcM_GetParam(this);
    field_0x715 = fopAcM_GetParam(this) >> 8;
    
    if (field_0x714 == 255)
        field_0x714 = 0;
    
    if (field_0x714 == 1 || field_0x714 == 3) {
      speed.y = cM_rndFX(10.0f) + 30.0f;
      f32 rng = cM_rndFX(1.0f);
      speedF = rng + 4.0f;
      field_0x6fc = rng + 4.0f;
      if (field_0x714 == 1) {
        fopAcM_OnStatus(this,fopAcM_STATUS_UNK_0x4000);
      }
    }

    mObjAcch.Set(fopAcM_GetPosition_p(this),fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), 0, 0);
    
    if (field_0x714 == 3) {
      mAcchCir.SetWall(35.0f,70.0f);
    }
    else {
      mAcchCir.SetWall(20.0f,60.0f);
    }

    mObjAcch.CrrPos(dComIfG_Bgsp());

    mTgCoSph.Set(cc_fz_src);
    mTgCoSph.SetStts(&mStts);

    mAtSph.Set(cc_fz_at_src);
    mAtSph.SetStts(&mStts);

    mCreature.init(&current.pos,&eyePos,3,1);
    mCreature.setEnemyName("E_fz");

    mAtInfo.mpSound = &mCreature;
    mAtInfo.mPowerType = 1;

    gravity = -5.0f;

    shape_angle.z = 0;
    shape_angle.x = 0;
    
    s16 random = cM_rndFX(10000.0f);
    shape_angle.y = random;
    current.angle.y = random;

    field_0x670.set(current.pos);
    
    for (int i = 0; i < 4; i++) {
      field_0x67c[i].set(current.pos);
    }

    if (field_0x714 == 2 && !checkItemGet(fpcNm_ITEM_IRONBALL,1)) {
      attention_info.distances[fopAc_attn_BATTLE_e] = 0;
      fopAcM_SetGroup(this,0);
      fopAcM_OffStatus(this,0);
      attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
    }

    if (field_0x714 == 3) {
      mRadiusBase = 0.0f;
      attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
      mAtSph.SetAtType(AT_TYPE_CSTATUE_SWING);
      mAtSph.SetAtSpl(dCcG_At_Spl_UNK_1);
      setActionMode(ACT_ROLLMOVE,0);
    } else {
      mAtSph.SetAtMtrl(dCcD_MTRL_ICE);
      mRadiusBase = 1.0f;
      cM_rnd() < 0.5f ? setActionMode(ACT_WAIT,0) : setActionMode(ACT_MOVE,0);
    }

    mtx_set();
  }

  return phase;
}

static void daE_FZ_Create(daE_FZ_c* i_this) {
    i_this->create();
}

static actor_method_class l_daE_FZ_Method = {
    (process_method_func)daE_FZ_Create,
    (process_method_func)daE_FZ_Delete,
    (process_method_func)daE_FZ_Execute,
    (process_method_func)daE_FZ_IsDelete,
    (process_method_func)daE_FZ_Draw,
};

actor_process_profile_definition g_profile_E_FZ = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_FZ,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daE_FZ_c),       // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  169,                    // mPriority
  &l_daE_FZ_Method,       // sub_method
  0x00040120,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_0_e,      // cullType
};
