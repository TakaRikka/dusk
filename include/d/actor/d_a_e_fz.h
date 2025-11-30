/**
* @file d_a_e_fz.h
* @brief Freezard header file.
*
* @details This header contains information about the Mini Freezard actor.
*
*/

#ifndef D_A_E_FZ_H
#define D_A_E_FZ_H

#include "SSystem/SComponent/c_phase.h"
#include "Z2AudioLib/Z2Creature.h"
#include "f_op/f_op_actor.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "m_Do/m_Do_hostIO.h"

/**
 * @enum daE_FZ_Action
 * @brief Mini Freezard current action. 
 * 
 */
enum daE_FZ_Action {
    ACT_WAIT,      /**< Mini Freezard is waiting. */ 
    ACT_MOVE,      /**< Mini Freezard is moving to attack. */
    ACT_ATTACK,    /**< Mini Freezard is attacking. */
    ACT_DAMAGE,    /**< Mini Freezard is damaged. */
    ACT_ROLLMOVE   /**< Mini Freezard is spinning around Blizzeta.  
                        This action is only taken when actor is loaded in with parameter 3 
                        (during the Blizzeta fight).*/
};

/**
 * @ingroup actors-enemies
 * @class daE_FZ_c
 * @brief Mini Freezard
 * 
 * @details Miniature Freezard enemy.
 * 
 */
class daE_FZ_c : public fopEn_enemy_c {
public:
    s32 draw();
    void setActionMode(int, int);
    void setReflectAngle();
    void mBoundSoundset();
    void deadnextSet(bool);
    void damage_check();
    bool way_gake_check();
    void executeWait();
    void executeMove();
    void executeAttack();
    void executeDamage();
    void executeRollMove();
    void action();
    void mtx_set();
    void cc_set();
    s32 execute();
    void demoDelete();
    s32 _delete();
    s32 CreateHeap();
    s32 create();

private:
    /* 0x5AC */ request_of_phase_process_class mPhaseReq;   /**< Phase request used when creating the actor. */
    /* 0x5B4 */ J3DModel* mpModel;                          /**< Pointer to the Mini Freezard's model. */
    /* 0x5B8 */ mDoExt_invisibleModel mInvisibleModel;      /**< Invisible model used for the Mini Freezard. */
    /* 0x5C0 */ Z2CreatureEnemy mCreature;                  /**< Z2CreatureEnemy class used for playing sound effects. */
    /* 0x664 */ cXyz mUserWork;                             /**< ??? */
    /* 0x670 */ cXyz field_0x670;                           /**< Appears unused. Set to current position on actor creation. */
    /* 0x67C */ cXyz field_0x67c[4];                        /**< ??? */
    /* 0x6AC */ cXyz field_0x6ac[4];                        /**< ??? */
    /* 0x6DC */ cXyz field_0x6dc;                           /**< ??? */
    /* 0x6E8 */ cXyz field_0x6e8;                           /**< ??? */
    /* 0x6F4 */ s32 mActionMode;                            /**< Current action mode. */
    /* 0x6F8 */ s32 mActionPhase;                           /**< Current phase of @ref mActionMode. */
    /* 0x6FC */ f32 field_0x6fc;                            /**< ??? */
    /* 0x700 */ f32 mRadiusBase;                            /**< Base radius value used by @ref mTgCoSph and @ref mAtSph. */
    /* 0x704 */ s16 field_0x704;                            /**< ??? */
    /* 0x706 */ s16 mAngleFromPlayer;                       /**< Angle the mini freezard is from the player. */
    /* 0x708 */ s16 mLastWallHitAngle;                      /**< Angle of the last wall the mini freezard hit. */
    /* 0x70C */ s32 field_0x70c;                            /**< ??? */
    /* 0x710 */ u8 field_0x710;                             /**< ??? */
    /* 0x711 */ u8 field_0x711;                             /**< ??? */
    /* 0x712 */ u8 field_0x712;                             /**< ??? */
    /* 0x713 */ u8 field_0x713;                             /**< ??? */
    /* 0x714 */ u8 field_0x714;                             /**< ??? */
    /* 0x715 */ u8 field_0x715;                             /**< ??? */
    /* 0x716 */ u8 field_0x716;                             /**< ??? */
    /* 0x717 */ u8 field_0x717;                             /**< ??? */
    /* 0x718 */ fopAc_ac_c* mpBlizzetaActor;                /**< Pointer to Blizzeta's actor (if used). Used to figure out what the mini freezard's angle and positioning should be when Blizzeta is attacking with them. */
    /* 0x71C */ u32 mParticleSet[3];                        /**< Particle set */
    /* 0x728 */ u8 field_0x728[4];                          /**< ??? */
    /* 0x72C */ dBgS_AcchCir mAcchCir;                      /**< Checks for actor collision with background circles? */
    /* 0x76C */ dBgS_ObjAcch mObjAcch;                      /**< Checks for actor collision with background objects. */
    /* 0x944 */ dCcD_Stts mStts;                            /**< Track Mini Freezard collider status. */
    /* 0x980 */ dCcD_Sph mTgCoSph;                          /**< Sphere collider used when Mini Freezard is attacked / pushed. */
    /* 0xAB8 */ dCcD_Sph mAtSph;                            /**< Sphere collider used when Mini Freezard is attacking. */
    /* 0xBF0 */ dCcU_AtInfo mAtInfo;                        /**< Tracks information when the Mini Freezard is attacked. */
    /* 0xC14 */ u8 field_0xc14;                             /**< ??? */
    /* 0xC15 */ u8 field_0xc15;                             /**< ??? */
    /* 0xC16 */ u8 field_0xc16;                             /**< ??? */
    /* 0xC17 */ u8 field_0xc17;                             /**< ??? */
    /* 0xC18 */ u8 field_0xc18;                             /**< ??? */
    /* 0xC19 */ u8 field_0xc19;                             /**< ??? */
    /* 0xC1A */ u8 field_0xc1a;                             /**< ??? */
    /* 0xC1B */ u8 field_0xc1b;                             /**< ??? */
    /* 0xC1C */ u8 field_0xc1c;                             /**< ??? */
    /* 0xC1D */ u8 field_0xc1d;                             /**< ??? */
    /* 0xC1E */ u8 field_0xc1e;                             /**< ??? */
    /* 0xC1F */ u8 field_0xc1f;                             /**< ??? */
    /* 0xC20 */ u8 field_0xc20;                             /**< ??? */
    /* 0xC21 */ u8 field_0xc21;                             /**< ??? */
    /* 0xC22 */ u8 field_0xc22;                             /**< ??? */
    /* 0xC23 */ u8 field_0xc23;                             /**< ??? */
};

STATIC_ASSERT(sizeof(daE_FZ_c) == 0xC24);

#endif /* D_A_E_FZ_H */
