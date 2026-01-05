/**
 * @file d_a_e_yk.cpp
 * 
 * Enemy - Shadow Keese
 * 
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_yk.h"
#include "c/c_damagereaction.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"
#include "SSystem/SComponent/c_math.h"
#include "f_op/f_op_actor_enemy.h"

/**
 * @class daE_YK_HIO_c
 * @brief Shadow Keese Host Input Output class.
 * 
 */
class daE_YK_HIO_c : public JORReflexible {
public:
    daE_YK_HIO_c();
    virtual ~daE_YK_HIO_c() {}
    void genMessage(JORMContext*);

    /* 0x00 */ // vtable
    /* 0x04 */ s8 field_0x04;       ///< @brief Initialized to -1, appears unused.
    /* 0x08 */ f32 mModelScale;     ///< @brief Base model scale factor (default: 1.0).
    /* 0x0C */ f32 mFlySpeed;       ///< @brief Base flying speed (default: 15.0).
    /* 0x10 */ f32 mAttackRange;    ///< @brief Distance threshold for entering attack state (default: 250.0).
    /* 0x14 */ f32 mCruiseSpeed;    ///< @brief Speed when flying normally (default: 15.0).
    /* 0x18 */ f32 mChargeSpeed;    ///< @brief Speed when charging at player (default: 40.0).
};

/**
 * @brief Constructor for Shadow Keese HIO (Host Input Output) configuration class
 * 
 * Initializes tunable parameters that control Shadow Keese behavior:
 * - Model scale factor
 * - Movement speeds for different states
 * - Attack range threshold
 * 
 */
daE_YK_HIO_c::daE_YK_HIO_c() {
    field_0x04 = -1;
    mModelScale = 1.0f;
    mFlySpeed = 15.0f;
    mAttackRange = 250.0f;
    mCruiseSpeed = 15.0f;
    mChargeSpeed = 40.0f;   
}

static u8 hio_set;

static daE_YK_HIO_c l_HIO;

#if DEBUG
void daE_YK_HIO_c::genMessage(JORMContext* ctx) {
  ctx->genLabel("闇キース", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
  ctx->genSlider("基本サイズ", &mModelScale, 0.0f, 3.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x512, 24);
  ctx->genSlider("飛行速度", &mFlySpeed, 0.0f, 50.0f, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
  ctx->genSlider("戦闘開始範囲", &mAttackRange, 0.0f, 600.0f,0, NULL, 0xFFFF, 0xFFFF, 512, 24);
  ctx->genSlider("戦闘速度", &mCruiseSpeed, 0.0f, 50.0f, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
  ctx->genSlider("突進速度", &mChargeSpeed, 0.0f, 50.0f, 0, NULL, 0xFFFF, 0xFFFF, 512, 24);
}
#endif


/**
 * @brief Makes a Shadow Keese disappear with effects and item drops
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * 
 * Plays particle effects and sound when the Shadow Keese disappears.
 * Creates an item drop and handles switch activation if specified in parameters.
 */
static void yk_disappear(e_yk_class* i_this) {
    cXyz pos(0.65f,0.65f,0.65f);

    dComIfGp_particle_set(0x826c,&i_this->current.pos,0,&pos);
    dComIfGp_particle_set(0x826d,&i_this->current.pos,0,&pos);

    fopAcM_seStart(i_this,Z2SE_DARK_VANISH,0);
    fopAcM_createItemFromEnemyID(1,&i_this->current.pos,0xffffffff,0xffffffff,0,0,0,0);

    s32 param = fopAcM_GetParam(i_this) >> 0x18;

    if (param != 0xff) {
        dComIfGs_onSwitch(param, fopAcM_GetRoomNo(i_this));
    }
}

/**
 * @brief Initializes animation parameters for the Shadow Keese
 * 
 * @param i_this Pointer to the Shadow Keese instance
 * @param i_resIdx Resource index for the animation transform data
 * @param i_morf Animation morph rate
 * @param i_attr Animation play mode
 * @param i_rate Animation play speed
 * 
 * Sets up a new animation on the Shadow Keese's morph model using the specified parameters.
 * Updates the current resource index to track which animation is playing.
 */
static void anm_init(e_yk_class* i_this, int i_resIdx, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_this->mpMorfSO->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_YK",i_resIdx), i_attr, i_morf, i_rate, 0.0f,-1.0f);
    i_this->mResIdx = i_resIdx;
}

/**
 * @brief Renders the Shadow Keese model with environmental lighting
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * @return int Always returns 1
 * 
 * Handles the rendering of the Shadow Keese by:
 * - Setting up environmental lighting and TEV (Texture Environment) parameters
 * - Applying dark material settings appropriate for a shadow creature
 * - Drawing the morphing model with current animation state
 * 
 * The function uses the following rendering pipeline:
 * 1. Gets the J3D model from the morph object
 * 2. Configures environmental lighting based on current position
 * 3. Sets up special dark rendering state
 * 4. Renders the model
 * 5. Restores normal rendering state
 */
static int daE_YK_Draw(e_yk_class* i_this) {
    J3DModel* model = i_this->mpMorfSO->getModel();

    g_env_light.settingTevStruct(2,&i_this->current.pos,&i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model,&i_this->tevStr);

    dComIfGd_setListDark();

    i_this->mpMorfSO->entryDL();
    dComIfGd_setList();
    return 1;
}

/**
 * @brief Searches for a charged boomerang that can affect the Shadow Keese
 * 
 * @param param_0 Pointer to the actor being checked
 * @param param_1 Unused parameter
 * @return void* Returns the boomerang actor pointer if conditions are met, NULL otherwise
 * 
 * Checks if the given actor is:
 * - A valid actor
 * - A boomerang (PROC_BOOMERANG)
 * - Player is not in a certain state (0x80000)
 * - Boomerang is charged
 * - Boomerang parameter is 1
 * 
 * Used by the Shadow Keese's wind state to determine if it should be affected
 * by a charged boomerang's wind effect.
 */
static void* shot_b_sub(void* i_actor, void* param_1) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_BOOMERANG &&
                                !dComIfGp_checkPlayerStatus0(0,0x80000) &&
                                daPy_py_c::checkBoomerangCharge() &&
                                fopAcM_GetParam(i_actor) == 1) {
            return i_actor;
    }

    return 0;
}


/**
 * @brief Checks for line-of-sight obstruction between Shadow Keese and another actor
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * @param i_actorP Pointer to the target actor to check against
 * @return int Returns 1 if line of sight is blocked by background geometry, 0 if clear
 * 
 * Performs a line collision check between:
 * - Shadow Keese's eye position
 * - Target actor's position + 100 units up
 * 
 * Used to determine if background geometry (walls, terrain, etc.) is blocking
 * the Shadow Keese's view of potential targets. This affects targeting and
 * behavior decisions.
 */
static int other_bg_check(e_yk_class* i_this, fopAc_ac_c* i_actorP) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    dBgS_LinChk lin_chk;
    cXyz yk_pos;
    cXyz actor_pos;
    
    actor_pos = i_actorP->current.pos;
    actor_pos.y += 100.0f;
    
    yk_pos = _this->current.pos;
    yk_pos.y = _this->eyePos.y;
    
    lin_chk.Set(&yk_pos,&actor_pos,_this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief Checks if the Shadow Keese should target the player
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * @param i_distance Maximum distance threshold for targeting
 * @param i_angle Maximum angle difference threshold for targeting (1 means any angle)
 * @return int Returns 1 if targeting conditions are met, 0 otherwise
 * 
 * Determines if the Shadow Keese should target the player based on:
 * 1. Special case: Always returns 1 if distance threshold >= 50000.0f (used in Phantom Zant fights)
 * 2. Player must be below the keese
 * 3. Player must be within the specified distance
 * 4. Player must be within the specified angle range (unless i_angle == 1)
 * 5. No background geometry blocking line of sight
 * 
 * Used by various states to determine when to transition to combat behaviors.
 */
static int pl_check(e_yk_class* i_this, f32 i_distance, s16 i_angle) {
    if (i_distance >= 50000.0f) {
        return 1;
    }

    if (dComIfGp_getPlayer(0)->current.pos.y < i_this->current.pos.y && i_this->mDistanceXZFromPlayer < i_distance) {
        s16 angle_delta = i_this->shape_angle.y - i_this->mAngleFromPlayer;

        if (i_angle == 1 || angle_delta < i_angle && angle_delta > (s16)-i_angle){
            if (!other_bg_check(i_this,dComIfGp_getPlayer(0))) {
                return 1;
            } 
        }
    }

    return 0;   
}

/**
 * @brief Checks for and handles damage to the Shadow Keese
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * 
 * Processes collision-based damage from various sources:
 * - Boomerang: Transitions to WIND state
 * - Clawshot/Slingshot: Reduces health by 1
 * - Shield Attack: Transitions to CHANCE state with knockback
 * - Wolf Bite: Transitions to WOLFBITE state
 * - Other attacks: Applies knockback and invulnerability frames
 * 
 * Special handling:
 * - Sets invulnerability timer based on attack type
 * - Plays appropriate sound effects
 * - Handles death state when health reaches 0
 * - Manages collision flags and status
 */
static void damage_check(e_yk_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (i_this->mInvulnerabilityTimer == 0) {
        // Store current AtApid and TgApid then set them to 0
        i_this->mCollisionStatus.Move();

        // If keese Defense collider was hit
        if (i_this->mCollisionSphere.ChkTgHit()) {
            // Store the tg collider pointer as the current at collider in the info block
            i_this->mAtColliderInfo.mpCollider = i_this->mCollisionSphere.GetTgHitObj();

            // If keese was hit by the boomerang
            if (i_this->mAtColliderInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                i_this->mAction = ACT_WIND;
                i_this->mActionPhase = 0;
                
            } else {
                // Run through the default Attack collider checks first
                cc_at_check(i_this,&i_this->mAtColliderInfo);
                
                // If keese was hit by Clawshot or Slingshot, subtract 1 from health
                if (i_this->mAtColliderInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT) || i_this->mAtColliderInfo.mpCollider->ChkAtType(AT_TYPE_SLINGSHOT)) {
                    i_this->health--;
                }

                // If keese was hit by shield attack, set some fields and play controller vibration
                if (i_this->mAtColliderInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
                    i_this->mAction = ACT_CHANCE;
                    i_this->mActionPhase = 0;
                    i_this->mKnockbackSpeed = 70.0f;
                    i_this->mKnockbackAngle = i_this->shape_angle.y;
                    i_this->mDeathFlag = 0;

                    dComIfGp_getVibration().StartShock(2,0x1f,cXyz(0.0f,1.0f,0.0f));
                } else {
                    // If keese was hit by wolf bite, set some fields, set pause timer to 0, 
                    // play keese wolf bit sound
                    if (i_this->mAtColliderInfo.mpCollider->ChkAtType(AT_TYPE_WOLF_ATTACK) && (static_cast<daPy_py_c*>(player)->onWolfEnemyBiteAll(i_this,daPy_py_c::FLG2_UNK_8) != 0)) {
                        i_this->mAction = ACT_WOLFBITE;
                        i_this->mActionPhase = 0;
                        i_this->mInvulnerabilityTimer = 200;
                        dScnPly_c::setPauseTimer(0);
                        i_this->mCreature.startCreatureVoice(Z2SE_EN_YK_V_BITE,-1);
                    } else {
                        // If it was unknown attack, set some fields
                        if (i_this->mAtColliderInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
                            i_this->mInvulnerabilityTimer = 20;
                        } else {
                            i_this->mInvulnerabilityTimer = 10;
                        }

                        i_this->mKnockbackSpeed = cM_rndF(10.0f) + 70.0f;
                        i_this->mKnockbackAngle = i_this->mAtColliderInfo.mHitDirection.y;

                        // If keese is dead, play death sound, set morph speed and set the death flag
                        if (i_this->health <= 0) {
                            i_this->mCreature.startCreatureVoice(Z2SE_EN_YK_V_DEATH,-1);
                            i_this->mpMorfSO->setPlaySpeed(0.2f);
                            i_this->mDeathFlag = 1;
                        }
                    }
                }        
            }
        }
    }
}

/**
 * @brief Checks visibility and accessibility of path points for Shadow Keese navigation
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * @return int Returns 1 if valid path point found, 0 if no valid points
 * 
 * Performs two main checks:
 * 1. Line-of-sight check to each path point:
 *    - Checks for obstacles between keese and path points
 *    - Marks accessible points in check_index array
 * 
 * 2. Distance-based path point selection:
 *    - Iteratively checks distances to accessible points
 *    - Selects closest point within expanding radius
 *    - Updates path index and direction
 * 
 * Used for path-following behavior to ensure keese can navigate around obstacles
 * and maintain valid flight paths.
 */
static int path_check(e_yk_class* i_this) {
    static u8 check_index[255];

    if (i_this->mpPath) {
        dBgS_LinChk lin_chk;

        cXyz current_keese_pos;
        cXyz path_point_pos;
        current_keese_pos = i_this->current.pos;
        current_keese_pos.y += 100.0f;
        dPnt* points = i_this->mpPath->m_points;

        for (int i = 0; i < i_this->mpPath->m_num; i++, points++) {
            path_point_pos.x = points->m_position.x;
            path_point_pos.y = points->m_position.y + 100.0f;
            path_point_pos.z = points->m_position.z;

            lin_chk.Set(&current_keese_pos,&path_point_pos,i_this);

            if (!dComIfG_Bgsp().LineCross(&lin_chk)) {
                check_index[i] = 1;
            } else {
                check_index[i] = 0;
            }
        }

        f32 x,y,z;
        f32 f = 0.0f;
        bool tmp = false;
        
        for (int i = 0; i < 100; i++, f+= 50.0f) {
            points = i_this->mpPath->m_points;

            for (int j = 0; j < i_this->mpPath->m_num; j++, points++) {
                if (check_index[j] != 0) {
                    x = i_this->current.pos.x - points->m_position.x;
                    y = i_this->current.pos.y - points->m_position.y;
                    z = i_this->current.pos.z - points->m_position.z;

                    if (JMAFastSqrt(x*x + y*y + z*z) < f) {
                        i_this->mPathPntIdx = j - i_this->mPathDirection;
                        u16 pathNum = i_this->mpPath->m_num;

                        if (i_this->mPathPntIdx >= (s8)i_this->mpPath->m_num) {
                            i_this->mPathPntIdx = i_this->mpPath->m_num;
                        } else {
                            if (0 > i_this->mPathPntIdx) {
                                i_this->mPathPntIdx = 0;
                            }
                        }
                        
                        tmp = true;
                        break;
                    }
                }
            }

            if (tmp) break;
            
        }

        if (!tmp) {
            i_this->mPathActive = 0;
        } else {
            i_this->mPathActive = i_this->mPathIdx + 1;
            return 1;
        }
    }

    return 0;
}

/**
 * @brief Handles movement calculations for Shadow Keese flight
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * 
 * Calculates and applies movement towards a target position (mPathPntPos):
 * 1. Calculates direction vector to target
 * 2. Converts to yaw and pitch angles
 * 3. Smoothly interpolates current angles towards target angles
 * 4. Applies speed in resulting direction
 * 
 * Movement characteristics:
 * - Turn speed of 2000.0f units
 * - Smooth acceleration via mMoveInterpolation (0.0 to 1.0)
 * - Movement speed controlled by speedF member
 * - Uses matrix transformations for final velocity calculation
 */
static void fly_move(e_yk_class* i_this) {
    cXyz pos;

    f32 x = i_this->mPathPntPos.x - i_this->current.pos.x;
    f32 y = i_this->mPathPntPos.y - i_this->current.pos.y;
    f32 z = i_this->mPathPntPos.z - i_this->current.pos.z;

    s16 angle = cM_atan2s(x,z);
    f32 sqrt = JMAFastSqrt(x * x + z * z);
    s16 angle2 = -cM_atan2s(y,sqrt);

    cLib_addCalcAngleS2(&i_this->current.angle.y,angle,10,i_this->mTurnSpeed * i_this->mMoveInterpolation);

    i_this->mTurnSpeed = 2000.0f;

    cLib_addCalcAngleS2(&i_this->current.angle.x,angle2,10,i_this->mTurnSpeed * i_this->mMoveInterpolation);
    cLib_addCalc2(&i_this->mMoveInterpolation,1.0f,1.0f,0.04f);
    
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = i_this->speedF;
    
    mDoMtx_YrotS((MtxP)calc_mtx,i_this->current.angle.y);
    cMtx_XrotM((MtxP)calc_mtx,i_this->current.angle.x);
    MtxPosition(&pos,&i_this->speed);

    i_this->current.pos += i_this->speed;
}

/**
 * @brief Handles Shadow Keese behavior when perched on ceiling
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * 
 * State machine with two phases:
 * 0. Initialization:
 *    - Sets up perched animation with slight random speed variation
 *    - Transitions to phase 1
 * 
 * 1. Perched behavior:
 *    - 50% chance to make sound every ~32 frames
 *    - Smoothly maintains position at home point
 *    - Checks for player proximity to trigger combat
 * 
 * Will transition to FIGHT_FLY state if player comes within trigger range.
 * Uses smooth interpolation to maintain position and prevent jittering.
 */
static void e_yk_roof(e_yk_class* i_this) {
    switch (i_this->mActionPhase) {
    case 0:
        anm_init(i_this,9,15.0f,2,cM_rndF(0.1f) + 0.9f); // random number between 0.9 and 1.0
        i_this->mActionPhase = 1;
        break;
    case 1:
        if ((i_this->mFrameCounter & 0x1f) == 0 && cM_rndF(1.0f) < 0.5f) {
            i_this->mCreature.startCreatureVoice(Z2SE_EN_YK_V_NAKU,-1);
        }
    }

    cLib_addCalc2(&i_this->current.pos.x,i_this->home.pos.x,0.5f,fabsf(i_this->speed.x));
    cLib_addCalc2(&i_this->current.pos.y,i_this->home.pos.y,0.5f,fabsf(i_this->speed.y));
    cLib_addCalc2(&i_this->current.pos.z,i_this->home.pos.z,0.5f,fabsf(i_this->speed.z));

    if (pl_check(i_this,i_this->mPlayerTrigger,1)) {
        i_this->mAction = ACT_FIGHT_FLY;
        i_this->mActionPhase = 0;
    }
}

/**
 * @brief Handles the Shadow Keese's behavior when flying towards the player to engage in combat
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * 
 * Controls the Shadow Keese's movement when transitioning from passive to combat state.
 * The keese will fly directly towards the player's position while playing appropriate animations.
 * 
 * State transitions:
 * - If player moves out of range: Changes to ACT_RETURN or ACT_FLY based on mBehaviorMode
 * - If path point is available: Changes to ACT_PATH_FLY
 * - If close enough to player: Changes to ACT_FIGHT
 * 
 * Animation phases:
 * - Phase 0: Initializes flying animation
 * - Phase 1: Maintains flight and plays random vocalizations
 */
static void e_yk_fight_fly(e_yk_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    switch (i_this->mActionPhase) {
    case 0:
        anm_init(i_this,5,3.0f,2,1.0f);
        i_this->mActionPhase = 1;
        i_this->mMoveInterpolation = 0.0f;
        break;
    case 1:
        if ((i_this->mFrameCounter & 0xfU) == 0 && cM_rndF(1.0f) < 0.5f) {
            i_this->mCreature.startCreatureVoice(Z2SE_EN_YK_V_NAKU,-1);
        }
    }

    cLib_addCalc2(&i_this->speedF,l_HIO.mFlySpeed, 1.0f, 0.3f * l_HIO.mFlySpeed);
    i_this->mPathPntPos = player->current.pos;
    fly_move(i_this);

    if (!pl_check(i_this,50.0f + i_this->mPlayerTrigger,1)) {
        if (!path_check(i_this)) {
            if (i_this->mBehaviorMode == 0) {
                i_this->mAction = ACT_RETURN;
                i_this->mActionPhase = 0;
            } else {
                i_this->mAction = ACT_FLY;
                i_this->mActionPhase = 0;
            }
        } else {
            i_this->mAction = ACT_PATH_FLY;
            i_this->mActionPhase = 0;
        }
    } else {
        if (pl_check(i_this,l_HIO.mAttackRange,1)) {
            i_this->mAction = ACT_FIGHT;
            i_this->mActionPhase = 0;
        }
    }
}

/**
 * @brief Handles the Shadow Keese's combat behavior when circling around the player
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * 
 * Controls the Shadow Keese's movement pattern during combat, making it circle around 
 * the player while maintaining a certain distance. The keese picks random positions 
 * around the player to fly to, creating an erratic flight pattern.
 * 
 * State transitions:
 * - Phase 0: Initializes combat animation and timers
 * - Phase 1: Handles movement and position updates
 * 
 * The keese will transition to:
 * - ACT_ATTACK: When timer expires, initiating an attack on the player
 * - ACT_RETURN/ACT_FLY: If player moves out of range (based on mBehaviorMode)
 * - ACT_PATH_FLY: If a valid path point is available
 * 
 * Movement is smoothly interpolated using cLib_addCalc2 for position and angle updates.
 * Random factors are used to create unpredictable but controlled movement patterns.
 */
static void e_yk_fight(e_yk_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    s16 player_shape_angle_y = player->shape_angle.y;

    switch (i_this->mActionPhase) {
    case 0:
        anm_init(i_this,8,2.0f,2,cM_rndF(0.1f) + 1.0f);
        i_this->mActionPhase = 1;
        i_this->mActionTimers[0] = 0;
        i_this->mActionTimers[1]  = cM_rndF(100.0f) + 30.0f;
        break;
    case 1:
        if (i_this->mActionTimers[0] == 0) {
            mDoMtx_YrotS((MtxP)calc_mtx,player_shape_angle_y + (s16)cM_rndFX(12288.0f));

            cXyz pos;
            pos.x = 0.0f;
            pos.y = cM_rndF(100.0f) + 150.0f;
            pos.z = cM_rndF(150.0f) + 150.0f;

            MtxPosition(&pos,&i_this->mPathPntPos);
            i_this->mPathPntPos += player->current.pos;

            pos = i_this->mPathPntPos - i_this->current.pos;
            mDoMtx_YrotS((MtxP)calc_mtx,cM_atan2s(pos.x,pos.z));
            cMtx_XrotM((MtxP)calc_mtx,-cM_atan2s(pos.y,JMAFastSqrt(pos.x*pos.x + pos.z*pos.z)));

            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = l_HIO.mCruiseSpeed;

            MtxPosition(&pos,&i_this->speed);

            i_this->mActionTimers[0] = cM_rndF(30.0f) + 10.0f;
            i_this->mMoveInterpolation = 0.0f;
        }

        if (i_this->mActionTimers[1]  == 0) {
            i_this->mActionTimers[1]  = cM_rndF(100.0f) + 30.0f;
            i_this->mAction = ACT_ATTACK;
            i_this->mActionPhase = 0;
        }
    }

    cLib_addCalc2(&i_this->current.pos.x,i_this->mPathPntPos.x,0.2f,i_this->mMoveInterpolation * fabsf(i_this->speed.x));
    cLib_addCalc2(&i_this->current.pos.y,i_this->mPathPntPos.y,0.2f,i_this->mMoveInterpolation * fabsf(i_this->speed.y));
    cLib_addCalc2(&i_this->current.pos.z,i_this->mPathPntPos.z,0.2f,i_this->mMoveInterpolation * fabsf(i_this->speed.z));
    cLib_addCalc2(&i_this->mMoveInterpolation,1.0f,1.0f,0.1f);

    cLib_addCalcAngleS2(&i_this->current.angle.y,i_this->mAngleFromPlayer,4,0x800);

    if (!pl_check(i_this,i_this->mPlayerTrigger + 50.0f,1)) {
        if (!path_check(i_this)) {
            if (i_this->mBehaviorMode == 0) {
                i_this->mAction = ACT_RETURN;
                i_this->mActionPhase = 0;
            } else {
                i_this->mAction = ACT_FLY;
                i_this->mActionPhase = 0;
            }
        } else {
            i_this->mAction = ACT_PATH_FLY;
            i_this->mActionPhase = 0;
        }
    }
}

/**
 * @brief Handles Shadow Keese's attack behavior when targeting the player
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * 
 * State machine with phases:
 * 0. Initialization:
 *    - Sets up attack animation at 2x speed
 *    - Sets initial timer
 * 
 * 1. Targeting:
 *    - Positions 120 units above player
 *    - Waits for timer before charging
 * 
 * 2. Charge Attack:
 *    - Moves at charge speed toward target
 *    - Checks for shield hits (transitions to CHANCE state if blocked)
 *    - Times out to phase 3
 * 
 * 3. Recovery:
 *    - Slows down until returning to normal FIGHT state
 * 
 * Uses HIO-configured charge speed and interpolated movement.
 */
static void e_yk_attack(e_yk_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    
    f32 value = 0.0f;
    i_this->mMoveInterpolation = 0.0f;

    switch (i_this->mActionPhase) {
    case 0:
        anm_init(i_this,5,3.0f,2,2.0f);
        i_this->mActionPhase = 1;
        i_this->mActionTimers[1]  = 0x14;
        break;
    case 1:
        i_this->mPathPntPos = player->current.pos;
        i_this->mPathPntPos.y += 120.0f;
        i_this->mMoveInterpolation = 2.0f;

        if (i_this->mActionTimers[1]  == 0) {
            i_this->mActionPhase = 2;
            i_this->mActionTimers[0] = 0xf;
            i_this->mCreature.startCreatureVoice(Z2SE_EN_YK_V_ATTACK,-1);
        }
        break;
    case 2:
        value = l_HIO.mChargeSpeed;

        if (i_this->mCollisionSphere.ChkAtShieldHit()) {
            i_this->mAction = ACT_CHANCE;
            i_this->mActionPhase = 0;
            i_this->mKnockbackSpeed = 70.0f;
            i_this->mKnockbackAngle = i_this->shape_angle.y;
            i_this->mDeathFlag = 0;

            dComIfGp_getVibration().StartShock(2,0x1f,cXyz(0.0f,1.0f,0.0f));
            
        } else {
            if (i_this->mActionTimers[0] == 0) {
                i_this->mActionPhase = 3;
            }
        }
        break;
    case 3:
        if ((i_this->speedF <= 1.0f)) {
            i_this->mAction = ACT_FIGHT;
            i_this->mActionPhase = 0;
        }
    }

    cLib_addCalc2(&i_this->speedF,value,1.0f,0.2f * l_HIO.mChargeSpeed);
    fly_move(i_this);
}

/**
 * @brief Handles Shadow Keese's normal flying behavior
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * 
 * State machine with phases:
 * 0. Initialization:
 *    - Sets up flying animation
 * 
 * 1. Flying:
 *    - 50% chance to make sound every ~32 frames
 *    - Picks random point within range of home position
 *    - Smoothly flies to target point using interpolation
 *    - Resets timer for next point selection
 * 
 * Transitions to FIGHT_FLY if player comes within trigger range.
 * Uses HIO-configured fly speed and cruise speed.
 */
static void e_yk_fly(e_yk_class* i_this) {
    switch (i_this->mActionPhase) {
    case 0:
        anm_init(i_this,5,3.0f,2,1.0f);
        i_this->mActionPhase = 1;
        break;
    case 1:
        if ((i_this->mFrameCounter & 0x1f) == 0 && cM_rndF(1.0f) < 0.5f) {
            i_this->mCreature.startCreatureVoice(Z2SE_EN_YK_V_NAKU,-1);
        }

        if (i_this->mActionTimers[0] == 0) {
            i_this->mPathPntPos.x = i_this->home.pos.x + cM_rndFX(500.0f);
            i_this->mPathPntPos.y = i_this->home.pos.y + cM_rndFX(200.0f);
            i_this->mPathPntPos.z = i_this->home.pos.z + cM_rndFX(500.0f);

            cXyz pos = i_this->mPathPntPos - i_this->current.pos;

            mDoMtx_YrotS((MtxP)calc_mtx,cM_atan2s(pos.x,pos.z));
            cMtx_XrotM((MtxP)calc_mtx,-cM_atan2s(pos.y,JMAFastSqrt(pos.x*pos.x + pos.z*pos.z)));

            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = l_HIO.mCruiseSpeed;

            MtxPosition(&pos,&i_this->speed);
            i_this->mActionTimers[0] = cM_rndF(30.0f) + 10.0f;
            i_this->mMoveInterpolation = 0.0f;
        }
    }

    cLib_addCalc2(&i_this->speedF,l_HIO.mFlySpeed,1.0f,0.3f * l_HIO.mFlySpeed);
    fly_move(i_this);

    if (pl_check(i_this,i_this->mPlayerTrigger,1)) {
        i_this->mAction = ACT_FIGHT_FLY;
        i_this->mActionPhase = 0;
    }
}

/**
 * @brief Handles Shadow Keese returning to its home position
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * 
 * State machine with phases:
 * 0. Initialization:
 *    - Sets up flying animation
 *    - Resets movement interpolation
 * 
 * 1. Return Flight:
 *    - Flies directly to home position
 *    - Transitions to ROOF state when within 100 units
 *    - Can be interrupted by player proximity (FIGHT_FLY)
 * 
 * Uses HIO-configured fly speed and smooth movement interpolation.
 */
static void e_yk_return(e_yk_class* i_this) {
    switch (i_this->mActionPhase) {
    case 0:
        anm_init(i_this,5,3.0f,2,1.0f);
        i_this->mActionPhase = 1;
        i_this->mMoveInterpolation = 0.0f;

        // fallthrough
    case 1:
        break;
    }

    cLib_addCalc2(&i_this->speedF,l_HIO.mFlySpeed,1.0f, 0.3f * l_HIO.mFlySpeed);

    i_this->mPathPntPos = i_this->home.pos;
    fly_move(i_this);

    cXyz pos = i_this->current.pos - i_this->mPathPntPos;

    if (pos.abs() < 100.0f) {
        i_this->mAction = ACT_ROOF;
        i_this->mActionPhase = 0;
        
    } else {
        if (pl_check(i_this,i_this->mPlayerTrigger,1)) {
            i_this->mAction = ACT_FIGHT_FLY;
            i_this->mActionPhase = 0;
        }
    }
}

/**
 * @brief Handles Shadow Keese's path-following behavior
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * 
 * State machine with phases:
 * 0/1. Path Navigation:
 *    - Updates path point index based on direction
 *    - Handles path transitions between rooms
 *    - Manages path direction reversals
 *    - 50% chance to make sound every ~32 frames
 * 
 * 2/3. Point Movement:
 *    - Sets target to current path point + random offset
 *    - Moves to target point using interpolation
 *    - Transitions back to navigation when close enough
 * 
 * Uses HIO-configured fly speed and handles multi-room paths.
 */
static void e_yk_path_fly(e_yk_class* i_this) {
    switch (i_this->mActionPhase) {
    case 0:
        anm_init(i_this,5,3.0f,2,1.0f);
        i_this->mActionPhase = 1;

        // fallthrough
    case 1:
        if ((i_this->mFrameCounter & 0x1fU) == 0 && cM_rndF(1.0f) < 0.5f) {
            i_this->mCreature.startCreatureVoice(Z2SE_EN_YK_V_NAKU,-1);
        }

        i_this->mPathPntIdx += i_this->mPathDirection;

        if (i_this->mPathPntIdx >= (s8)i_this->mpPath->m_num) {
            if ((dPath_ChkClose(i_this->mpPath)) != 0) {
                i_this->mPathPntIdx = 0;
                
            } else {
                i_this->mPathDirection = 0xff;
                i_this->mPathPntIdx = i_this->mpPath->m_num - 2;
            }

            int roomNo = i_this->mpPath->m_nextID;

            if (roomNo != 0xFFFF) {
                i_this->mpPath = dPath_GetRoomPath(roomNo,fopAcM_GetRoomNo(i_this));
            }
        } else {
            if (i_this->mPathPntIdx < 0) {
                i_this->mPathDirection = 1;
                i_this->mPathPntIdx = 1;
            }
        }

        // fallthrough
    case 2: {
        i_this->mActionPhase = 3;

        dPnt* point = i_this->mpPath->m_points;
        point = &point[i_this->mPathPntIdx];

        i_this->mMoveInterpolation = 0.0f;
        i_this->mPathPntPos.x = point->m_position.x + cM_rndFX(150.0f);
        i_this->mPathPntPos.y = point->m_position.y + cM_rndFX(150.0f);
        i_this->mPathPntPos.z = point->m_position.z + cM_rndFX(150.0f);
        break;
    }
    case 3:
        cXyz pos = i_this->mPathPntPos - i_this->current.pos;

        if (pos.abs() < 200.0f) {
            i_this->mActionPhase = 1;
        }
    }

    cLib_addCalc2(&i_this->speedF,l_HIO.mFlySpeed,1.0f,0.3f * l_HIO.mFlySpeed);
    fly_move(i_this);
}

/**
 * @brief Handles Shadow Keese's stunned state after being shield-blocked
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * 
 * State machine with phases:
 * 0. Initialization:
 *    - Sets up stun animation at 1.5x speed
 *    - Sets random stun duration (100-130 frames)
 *    - Plays stun sound effect
 * 
 * 1. Stunned:
 *    - Bounces off ground with random velocity
 *    - Applies random rotation
 *    - Creates smoke effect on ground impact
 *    - Returns to FIGHT state when timer expires
 * 
 * Applies gravity and smooth rotation interpolation while stunned.
 */
static void e_yk_chance(e_yk_class* i_this) {
    switch (i_this->mActionPhase) {
    case 0:
        anm_init(i_this,8,2.0f,2,1.5f);
        i_this->mActionPhase = 1;
        i_this->mActionTimers[0] = cM_rndF(30.0f) + 100.0f;
        i_this->speed.x = 0.0f;
        i_this->speed.y = 0.0f;
        i_this->speed.z = 0.0f;
        i_this->mCreature.startCreatureVoice(Z2SE_EN_YK_V_BITE,-1);
        break;
    case 1:
        if (i_this->mActorCollisionHandler.ChkGroundHit()) {
            i_this->speed.y = cM_rndF(10.0f) + 10.0f;
            i_this->speed.x = cM_rndFX(10.0f);
            i_this->speed.z = cM_rndFX(10.0f);

            if (cM_rndF(1.0f) < 0.5f) {
                i_this->mStunRotation.z = 0;
            } else {
                i_this->mStunRotation.z = 0x8000;
            }
        
            i_this->mStunRotation.y = cM_rndF(65536.0f);
            fopAcM_effSmokeSet1(&i_this->mSmokeEffectId,&i_this->mSmokeEffectParams,
                                &i_this->current.pos,&i_this->shape_angle,
                                0.8f,&i_this->tevStr,1);

            i_this->mCreature.startCreatureVoice(Z2SE_EN_YK_V_FAINT,-1);
        }

        if (i_this->mActionTimers[0] == 0) {
            i_this->current.angle.z = 0;
            i_this->mAction = ACT_FIGHT;
            i_this->mActionPhase = 0;
            return;
        }
    }

    i_this->current.pos += i_this->speed;
    i_this->speed.y -= 2.0f;
    cLib_addCalcAngleS2(&i_this->current.angle.y,i_this->mStunRotation.y,2,0x1000);
    cLib_addCalcAngleS2(&i_this->current.angle.z,i_this->mStunRotation.z,2,0x1000);
}

/**
 * @brief Handles Shadow Keese's behavior when caught and thrown by Wolf Link
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * 
 * State machine with phases:
 * 0. Initialization:
 *    - Sets up caught animation
 * 
 * 1. Being Held:
 *    - Checks if Wolf Link still has hold
 *    - When thrown:
 *      - Sets throw angle based on left/right throw
 *      - Sets initial throw velocity (40.0f forward, -20.0f vertical)
 *      - Plays death sound
 *      - Transitions to phase 2
 * 
 * 2. Flying:
 *    - Applies gravity (-4.0f/frame)
 *    - Checks for ground collision
 *    - On ground hit: Plays impact sounds and transitions to phase 3
 * 
 * 3. Death:
 *    - Waits for timer (60 frames)
 *    - Calls yk_disappear and deletes actor
 * 
 * Maintains constant forward speed when bouncing on ground.
 */
static void e_yk_wolfbite(e_yk_class* i_this) {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    switch(i_this->mActionPhase) {
    case 0:
        anm_init(i_this,7,0.0f,2,1.0f);
        i_this->mActionPhase = 1;
        break;
    case 1:
        if (!player->checkWolfEnemyCatchOwn(i_this)) {
            if (player->checkWolfEnemyLeftThrow()) {
                i_this->current.angle.y = player->shape_angle.y + 0x4000;
            } else {
                i_this->current.angle.y = player->shape_angle.y - 0x4000;
            }

            i_this->speedF = 40.0f;
            i_this->speed.y = -20.0f;
            i_this->mCreature.startCreatureVoice(Z2SE_EN_YK_V_DEATH,-1);

            anm_init(i_this,4,1.0f,0,1.0f);

            i_this->mActionTimers[0] = 0x3c;
            i_this->mActionPhase = 2;
        }
        break;
    case 2:
        if (i_this->mActorCollisionHandler.ChkGroundHit()) {
            i_this->mCreature.startCreatureVoice(Z2SE_EN_YK_V_DEATH2,-1);
            i_this->mCreature.startCreatureSound(Z2SE_CM_BODYFALL_S,0,-1);
            i_this->mActionPhase = 3;
        }
        // break;
    case 3:
        if (i_this->mActionTimers[0] == 0) {
            yk_disappear(i_this);
            fopAcM_delete(i_this);
        }
    }

    cXyz pos, pos2;
    pos.x = 0.0f;
    pos.y = 0.0f; 
    pos.z = i_this->speedF;

    mDoMtx_YrotS((MtxP)calc_mtx,i_this->current.angle.y);
    MtxPosition(&pos, &pos2);
    i_this->speed.x = pos2.x;
    i_this->speed.z = pos2.z;

    i_this->current.pos += i_this->speed;
    i_this->speed.y -= 4.0f;

    if (i_this->mActorCollisionHandler.ChkGroundHit()) {
        cLib_addCalc0(&i_this->speedF,1.0f,15.0f);
    }
}

/**
 * @brief Handles Shadow Keese's behavior when caught in boomerang wind
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * 
 * State machine with phases:
 * 0. Initialization:
 *    - Sets up wind animation
 *    - Generates random rotation and position offsets
 *    - Transitions to phase 1
 * 
 * 1. Following Boomerang:
 *    - Maintains position relative to boomerang using offset
 *    - Plays spinning sound effect
 *    - Transitions to phase 2 if boomerang disappears
 * 
 * 2. Recovery:
 *    - Gradually reduces rotation
 *    - Waits for timer (0x3c frames)
 *    - Returns to FIGHT_FLY state
 * 
 * Continuously applies rotation around Y axis while in wind state.
 */
static void e_yk_wind(e_yk_class* i_this) {
    e_yk_class* yk = (e_yk_class*)fpcM_Search(shot_b_sub,i_this);
    i_this->speedF = 0.0f;
  
    switch(i_this->mActionPhase) {
    case 0:
        anm_init(i_this,6,3.0f,2,1.0f);
        i_this->mActionPhase = 1;
        i_this->mBoomrangXRotOffset = -(cM_rndFX(1000.0f) + 15000.0f);
        i_this->mBoomrangPosOffset.x = cM_rndFX(50.0f);
        i_this->mBoomrangPosOffset.y = cM_rndFX(50.0f);
        i_this->mBoomrangPosOffset.z = cM_rndFX(50.0f);
    case 1:
        if (!yk) {
            i_this->mActionPhase = 2;
            i_this->mActionTimers[0] = 0x3c;
            break;
            
        } else {
            i_this->current.pos = yk->current.pos + i_this->mBoomrangPosOffset;
            i_this->mCreature.startCreatureVoiceLevel(Z2SE_EN_YK_V_SPIN,-1);
            break;
        }
    case 2:
        cLib_addCalcAngleS2(&i_this->mBoomrangXRotOffset,0,4,0x1c2);

        if (i_this->mActionTimers[0] == 0) {
            i_this->mAction = ACT_FIGHT_FLY;
            i_this->mActionPhase = 0;
        }
    }

    i_this->current.angle.y += i_this->mBoomrangXRotOffset;
    i_this->shape_angle.y = i_this->current.angle.y;
    i_this->current.angle.z = 0;
}

/**
 * @brief Main action handler for Shadow Keese - controls state transitions and updates
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * 
 * Primary state machine controller that:
 * 1. Updates player tracking info (angle and distance)
 * 2. Checks for damage
 * 3. Manages collision flags
 * 4. Dispatches to appropriate state handler:
 *    - ROOF: Perched state
 *    - FIGHT_FLY: Approaching player
 *    - FIGHT: Combat circling
 *    - ATTACK: Charging attack
 *    - RETURN: Returning to home
 *    - FLY: Free flight
 *    - PATH_FLY: Following path
 *    - CHANCE: Stunned
 *    - WOLFBITE: Caught by Wolf Link
 *    - WIND: Caught in boomerang wind
 * 
 * Also handles:
 * - Knockback movement and death
 * - Smooth angle interpolation
 * - Ground collision offset adjustments
 */
static void action(e_yk_class* i_this) {
    cXyz pos;
    cXyz pos2;

    i_this->mAngleFromPlayer = fopAcM_searchPlayerAngleY(i_this);
    i_this->mDistanceXZFromPlayer = fopAcM_searchPlayerDistanceXZ(i_this);

    damage_check(i_this);

    i_this->mCollisionSphere.OffAtVsPlayerBit();
    s8 searchForLink = 0;
    
    switch(i_this->mAction) {
    case ACT_ROOF:
        e_yk_roof(i_this);
        break;
    case ACT_FIGHT_FLY:
        e_yk_fight_fly(i_this);
        break;
    case ACT_FIGHT:
        e_yk_fight(i_this);
        searchForLink = 1;
        break;
    case ACT_ATTACK:
        e_yk_attack(i_this);
        i_this->mCollisionSphere.OnAtVsPlayerBit();
        searchForLink = 1;
        break;
    case ACT_RETURN:
        e_yk_return(i_this);
        break;
    case ACT_FLY:
        e_yk_fly(i_this);
        break;
    case ACT_PATH_FLY:
        e_yk_path_fly(i_this);
        break;
    case ACT_CHANCE:
        e_yk_chance(i_this);
        break;
    case ACT_WOLFBITE:
        e_yk_wolfbite(i_this);
        break;
    case ACT_WIND:
        e_yk_wind(i_this);
    }

    searchForLink ? i_this->mCreature.setLinkSearch(true) : i_this->mCreature.setLinkSearch(false);

    if (i_this->mKnockbackSpeed > 0.1f) {
        cXyz pos;
        cXyz pos2;
        
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = -i_this->mKnockbackSpeed;

        mDoMtx_YrotS((MtxP)calc_mtx,i_this->mKnockbackAngle);
        MtxPosition(&pos,&pos2);
        i_this->current.pos += pos2;
        cLib_addCalc0(&i_this->mKnockbackSpeed,1.0f,5.0f);

        if (i_this->mDeathFlag != 0) {
            i_this->shape_angle.y += 0x1300;
            i_this->shape_angle.z += 0x1700;

            if (i_this->mKnockbackSpeed <= 1.0f || i_this->mActorCollisionHandler.ChkWallHit()) {
                yk_disappear(i_this);
                fopAcM_delete(i_this);
            }
        }
    } else {
        if (i_this->mAction != ACT_WIND) {
            cLib_addCalcAngleS2(&i_this->shape_angle.y,i_this->current.angle.y,4,0x2000);
            cLib_addCalcAngleS2(&i_this->shape_angle.x,0,4,0x2000);
            cLib_addCalcAngleS2(&i_this->shape_angle.z,i_this->current.angle.z,4,0x2000);
        }
    }

    i_this->current.pos.y -= 30.0f; 
    i_this->old.pos.y -= 30.0f;

    i_this->mActorCollisionHandler.CrrPos(dComIfG_Bgsp());

    i_this->current.pos.y += 30.0f;
    i_this->old.pos.y += 30.0f;
}

/**
 * @brief Main execution function for the Shadow Keese actor
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * @return int Always returns 1
 * 
 * Handles the main per-frame updates for the Shadow Keese, including:
 * - Timer management
 * - Action state execution
 * - Animation and model updates
 * - Particle effect management
 * - Collision and targeting setup
 * 
 * The function is skipped if Midna is currently talking.
 * 
 * State-specific behaviors:
 * - During WOLFBITE state: Attaches to wolf Link's mouth matrix
 * - During normal states: Updates attention targeting and transformation matrix
 * - Plays wing flap sounds on specific animation frames
 * - Manages shadow and wing particle effects
 * - Updates collision sphere position and radius
 */
static int daE_YK_Execute(e_yk_class* i_this) {
    fopEn_enemy_c* _this = static_cast<fopEn_enemy_c*>(i_this);
    cXyz pos, pos2;

    if (cDmrNowMidnaTalk()) {
        return 1;
    } else {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        i_this->mFrameCounter++;

        for (int i = 0; i < 4; i++) {
            if (i_this->mActionTimers[i] != 0) {
                i_this->mActionTimers[i]--;
            }
        }

        if (i_this->mInvulnerabilityTimer != 0) {
            i_this->mInvulnerabilityTimer--;
        }

        action(i_this);

        i_this->mpMorfSO->play(0,dComIfGp_getReverb(fopAcM_GetRoomNo(_this)));
        J3DModel* model = i_this->mpMorfSO->getModel();

        if (i_this->mAction == ACT_WOLFBITE && i_this->mActionPhase < 2) {
            fopAcM_OffStatus(_this,0);
            _this->attention_info.flags = 0;

            // need to define inline here
            MTXCopy(daPy_getLinkPlayerActorClass()->getWolfMouthMatrix(),mDoMtx_stack_c::get());
            model->setBaseTRMtx(mDoMtx_stack_c::get());
            mDoMtx_stack_c::multVecZero(&_this->current.pos);
        } else {
            if (_this->health > 0 && i_this->mDeathFlag == 0 && player->current.pos.y < _this->current.pos.y) {
                _this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            } else {
                fopAcM_OffStatus(i_this,0);
                _this->attention_info.flags = 0;
            }

            mDoMtx_stack_c::transS(_this->current.pos.x,_this->current.pos.y,_this->current.pos.z);
            mDoMtx_stack_c::YrotM(_this->shape_angle.y);
            mDoMtx_stack_c::ZrotM(_this->shape_angle.z);
            mDoMtx_stack_c::scaleM(l_HIO.mModelScale,l_HIO.mModelScale,l_HIO.mModelScale);

            model->setBaseTRMtx(mDoMtx_stack_c::get());
        }

        i_this->mpMorfSO->modelCalc();
        int res_idx = i_this->mResIdx;

        if (res_idx == 8 || res_idx == 5) {
            if (i_this->mpMorfSO->checkFrame(4.0f)) {
                if (i_this->mResIdx == 8) {
                    i_this->mCreature.startCreatureSound(Z2SE_EN_YK_WING,0,-1);
                } else {
                    i_this->mCreature.startCreatureSound(Z2SE_EN_YK_WING,0,-1);
                }   
            }
        } else if (res_idx == 6 && i_this->mpMorfSO->checkFrame(0.0)) {
            i_this->mCreature.startCreatureVoice(Z2SE_EN_YK_V_FURA,-1);
        }

        if (i_this->mResIdx != 4) {
            i_this->mShadowParticleId = dComIfGp_particle_set(i_this->mShadowParticleId,0x8434,&_this->current.pos,0,0);

            if (i_this->mResIdx != 9) {
                for (int i = 0; i < 2; i++) {
                    static u16 e_name[2] = {0x8432, 0x8433};
                    static u32 e_idx[2] = {5, 9};
                    i_this->mWingParticleIds[i] = dComIfGp_particle_set(i_this->mWingParticleIds[i],e_name[i],&_this->current.pos,0,0);
                    
                    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mWingParticleIds[i]);
                    if (emitter) {
                        emitter->setGlobalRTMatrix(model->getAnmMtx(e_idx[i]));
                    };
                }
            }
        }

        

        MTXCopy(model->getAnmMtx(2),(MtxP)calc_mtx);
        pos.set(0.0f,0.0f,0.0f);

        MtxPosition(&pos,&_this->eyePos);
        _this->attention_info.position = _this->eyePos;
        _this->attention_info.position.y += 20.0f;

        pos.set(0.0f,0.0f,0.0f);
        MtxPosition(&pos,&pos2);

        if (i_this->mInvulnerabilityTimer != 0) {
            pos2.z -= 20000.0f;
        }

        i_this->mCollisionSphere.SetC(pos2);
        i_this->mCollisionSphere.SetR(30.0f * l_HIO.mModelScale);

        dComIfG_Ccsp()->Set(&i_this->mCollisionSphere);
        cXyz i_effSize(0.5f,0.5f,0.5f);
        setMidnaBindEffect(_this,&i_this->mCreature,&_this->eyePos,&i_effSize);
    }

    return 1;
}

/**
 * @brief Checks if the Shadow Keese actor should be deleted
 * 
 * @param param_0 Pointer to the Shadow Keese actor instance
 * @return Always returns 1 to indicate deletion is allowed
 */
static int daE_YK_IsDelete(e_yk_class* param_0) {
    return 1;
}

/**
 * @brief Handles cleanup when Shadow Keese actor is deleted
 * 
 * @param i_this Pointer to the Shadow Keese actor instance
 * @return Always returns 1 to indicate successful deletion
 * 
 * Performs cleanup:
 * - Deletes resources associated with "E_YK"
 * - Resets first spawn flag if this was the first instance
 * - Stops animation if heap exists
 */
static int daE_YK_Delete(e_yk_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase,"E_YK");

    if (i_this->mIsFirstSpawn != 0) {
        hio_set = 0;
    }

    if (i_this->heap) {
        i_this->mpMorfSO->stopZelAnime();
    }

    return 1;
}

/**
 * @brief Initializes heap resources for Shadow Keese actor
 * 
 * @param i_this Pointer to the actor base class
 * @return 1 if initialization successful, 0 if failed
 * 
 * Creates and initializes the morphing model object (mpMorfSO):
 * - Uses "E_YK" model data and animation resources
 * - Sets up transform animations
 * - Configures creature parameters
 */
static int useHeapInit(fopAc_ac_c* i_this) {
    e_yk_class* yk = (e_yk_class*)i_this;

    yk->mpMorfSO = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_YK", 12), 
        NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_YK",9), 2, 1.0f, 0, -1, &yk->mCreature, 0x80000,0x11000084);
    if (!yk->mpMorfSO || !yk->mpMorfSO->mpModel) {
        return 0;
    }

    return 1;
}

/**
 * @brief Creates and initializes a new Shadow Keese actor instance
 * 
 * @param i_this Pointer to the actor base class
 * @return Phase step status (cPhs_ERROR_e on failure, phase_step otherwise)
 * 
 * Initialization steps:
 * 1. Loads "E_YK" resources
 * 2. Processes actor parameters:
 *    - Switch condition check
 *    - Behavior mode
 *    - Player trigger distance
 *    - Path index
 * 3. Initializes heap and model
 * 4. Sets up path following if path index valid
 * 5. Configures:
 *    - Collision spheres and handlers
 *    - Creature properties
 *    - Attention flags
 *    - Bounding box
 *    - Health and status
 * 
 * Special cases:
 * - Adjusts trigger distance for Phantom Zant fights
 * - Handles first spawn initialization
 */
static int daE_YK_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, e_yk_class);
    e_yk_class* yk = (e_yk_class*)i_this;

    int phase_step = dComIfG_resLoad(&yk->mPhase,"E_YK");

    if (phase_step == cPhs_COMPLEATE_e) {
        s32 param = fopAcM_GetParam(yk) >> 24;

        if (param != 0xFF && dComIfGs_isSwitch(param,fopAcM_GetRoomNo(yk))) {
            return cPhs_ERROR_e;
        }

        yk->mBehaviorMode = fopAcM_GetParam(yk);
        yk->mPlayerTriggerBase = (fopAcM_GetParam(yk) >> 8) & 0xf;
        yk->mPathIdx = fopAcM_GetParam(yk) >> 16;

        if (yk->mBehaviorMode == 0xff) {
            yk->mBehaviorMode = 0;
        }

        if (yk->mPlayerTriggerBase == 0xf) {
            yk->mPlayerTriggerBase = 10;
        }
        
        yk->mPlayerTrigger = 100.0f * yk->mPlayerTriggerBase;
        
        if (fopAcM_SearchByName(PROC_E_PZ)) {
            // For phantom zant fights. Used in pl_check above
            yk->mPlayerTrigger = 100000.0f; 
        }

        if (!fopAcM_entrySolidHeap(yk,useHeapInit,0x1740)) {
            return cPhs_ERROR_e;
        } else {
            if (yk->mPathIdx != 0xff) {
                yk->mpPath = dPath_GetRoomPath(yk->mPathIdx,fopAcM_GetRoomNo(yk));

                if (!yk->mpPath) {
                    return cPhs_ERROR_e;
                }

                yk->mPathActive = yk->mPathIdx + 1;
                yk->mPathDirection = 1;
                yk->mAction = ACT_PATH_FLY;
                
            } else {
                if (yk->mBehaviorMode == 1) {
                    yk->mAction = ACT_FLY;
                }
            }

            if (hio_set == 0) {
                yk->mIsFirstSpawn = 1;
                hio_set = 1;
                l_HIO.field_0x04 = -1;
            }

            yk->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

            fopAcM_SetMtx(yk,yk->mpMorfSO->getModel()->getBaseTRMtx());
            fopAcM_SetMin(yk,-200.0f,-200.0f,-200.0f);
            fopAcM_SetMax(yk,200.0f,200.0f,200.0f);

            yk->health = 1;
            yk->field_0x560 = 1;

            static dCcD_SrcSph cc_sph_src = {
                    {
                        {0x0, {{0x400, 0x1, 0xD}, {0xD8FBFDFF, 0x3}, 0x75}}, // mObj
                        {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                        {0x0}, // mGObjCo
                    }, // mObjInf
                    {
                        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
                    } // mSphAttr
            };

            yk->mCollisionStatus.Init(0x1e,0,yk);
            yk->mCollisionSphere.Set(cc_sph_src);
            yk->mCollisionSphere.SetStts(&yk->mCollisionStatus);

            yk->mActorCollisionHandler.Set(fopAcM_GetPosition_p(yk),
                            fopAcM_GetOldPosition_p(yk),yk,1,
                            &yk->mWallCollisionCircle, fopAcM_GetSpeed_p(yk),
                            0,0);

            yk->mWallCollisionCircle.SetWall(50.0f,50.0f);
            yk->mCreature.init(&yk->current.pos,&yk->eyePos,3,1);
            yk->mCreature.setEnemyName("E_yk");

            yk->mAtColliderInfo.mpSound = &yk->mCreature;
            yk->mAtColliderInfo.mPowerType = 1;
            yk->mFrameCounter = cM_rndF(65535.0f);

            daE_YK_Execute(yk);
        }
    }

    return phase_step;
}

static actor_method_class l_daE_YK_Method = {
    (process_method_func)daE_YK_Create,
    (process_method_func)daE_YK_Delete,
    (process_method_func)daE_YK_Execute,
    (process_method_func)daE_YK_IsDelete,
    (process_method_func)daE_YK_Draw,
};

actor_process_profile_definition g_profile_E_YK = {
    fpcLy_CURRENT_e,         // mLayerID  
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio 
    PROC_E_YK,               // mProcName        
    &g_fpcLf_Method.base,   // sub_method                    
    sizeof(e_yk_class),      // mSize                 
    0,                       // mSizeOther
    0,                       // mParameters       
    &g_fopAc_Method.base,    // sub_method                   
    188,                     // mPriority     
    &l_daE_YK_Method,        // sub_method               
    0x10050100,              // mStatus          
    fopAc_ENEMY_e,           // mActorType   
    fopAc_CULLBOX_CUSTOM_e,  // cullType   
};
