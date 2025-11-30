#ifndef D_A_BOOMERANG_H
#define D_A_BOOMERANG_H

#include "JSystem/J2DGraph/J2DScreen.h"
#include "d/d_particle_copoly.h"
#include "f_op/f_op_actor_mng.h"

#define BOOMERANG_LOCK_MAX 5

class daBoomerang_sight_c : public dDlst_base_c {
public:
    int createHeap();
    void initialize();
    int playAnime(int, int);
    void initFrame(int);
    void copyNumData(int);
    void setSight(cXyz const*, int);

    virtual void draw();
    virtual ~daBoomerang_sight_c() {}

    u8 getReserve() const { return mReserve; }
    u8 getAlpha(int i_index) { return m_alpha[i_index]; }
    void offRedSight() { m_redSight = false; }
    void onReserve() { mReserve = 1; }
    void offReserve() { mReserve = 0; }

private:
    /* 0x04 */ J2DScreen* m_cursorYellowScrn;
    /* 0x08 */ J2DPane* m_cursorYellowAllPane;
    /* 0x0C */ J2DPane* m_cursorYellow0Pane;
    /* 0x10 */ J2DPane* m_cursorYellow1Pane;
    /* 0x14 */ J2DPane* m_cursorYellow2Pane;
    /* 0x18 */ J2DAnmTevRegKey* m_cursorYellow2Brk;
    /* 0x1C */ J2DAnmTevRegKey* m_cursorYellowBrk;
    /* 0x20 */ J2DScreen* m_cursorRedScrn;
    /* 0x24 */ J2DPane* m_cursorRedAllPane;
    /* 0x28 */ J2DPane* m_cursorRed0Pane;
    /* 0x2C */ J2DPane* m_cursorRed1Pane;
    /* 0x30 */ J2DPane* m_cursorRed2Pane;
    /* 0x34 */ J2DAnmTevRegKey* m_cursorRed2Brk;
    /* 0x38 */ J2DAnmTevRegKey* m_cursorRedBrk;
    /* 0x3C */ J2DScreen* m_cursorOrangeScrn;
    /* 0x40 */ J2DPane* m_cursorOrangeAllPane;
    /* 0x44 */ J2DPane* m_cursorOrange0Pane;
    /* 0x48 */ J2DPane* m_cursorOrange1Pane;
    /* 0x4C */ J2DPane* m_cursorOrange2Pane;
    /* 0x50 */ J2DAnmTevRegKey* m_cursorOrange2Brk;
    /* 0x54 */ J2DAnmTevRegKey* m_cursorOrangeBrk;
    /* 0x58 */ J2DAnmColor* m_cursorYellowBpk;
    /* 0x5C */ J2DAnmTransform* m_cursorYellowBck;
    /* 0x60 */ bool m_redSight;
    /* 0x61 */ u8 mReserve;
    /* 0x62 */ u8 m_alpha[6];
    /* 0x68 */ f32 m_proj_posX[6];
    /* 0x80 */ f32 m_proj_posY[6];
    /* 0x98 */ f32 field_0x98[6];
    /* 0xB0 */ f32 field_0xb0[6];
    /* 0xC8 */ cXyz m_pos[6];
};  // Size: 0x110

class daAlink_c;

/**
 * @ingroup actors-items
 * @class daBoomerang_c
 * @brief Gale Boomerang
 *
 * @details Forest Temple dungeon item.
 *
 */
class daBoomerang_c : public fopAc_ac_c {
public:
    enum daBoomerang_FLG0 {
        /* 0x001 */ FLG0_1 = 0x1,
        /* 0x002 */ FLG0_2 = 0x2,
        /* 0x004 */ FLG0_4 = 0x4,
        /* 0x010 */ FLG0_10 = 0x10,
        /* 0x020 */ FLG0_20 = 0x20,
        /* 0x040 */ FLG0_40 = 0x40,
        /* 0x080 */ FLG0_80 = 0x80,
        /* 0x100 */ FLG0_100 = 0x100,
        /* 0x200 */ FLG0_200 = 0x200,
        /* 0x400 */ FLG0_400 = 0x400,
        /* 0x800 */ FLG0_LOCK_DIST_CANCEL = 0x800,
        /* 0x080 */ WIND_CATCH = 0x80,
    };

    void windModelCallBack();
    int draw();
    void lockLineCallback(fopAc_ac_c*);
    void moveLineCallback(fopAc_ac_c*);
    void pushLockList(int);
    void cancelLockActorBase(fopAc_ac_c*);
    void setAimActorBase(fopAc_ac_c*);
    int setLockActor(fopAc_ac_c*, int);
    void resetLockActor();
    void setRoomInfo();
    void setKeepMatrix();
    void setMoveMatrix();
    void setRotAngle();
    void setAimPos();
    void checkBgHit(cXyz*, cXyz*);
    JPABaseEmitter* setEffectTraceMatrix(u32*, u16);
    void setEffect();
    int procWait();
    int procMove();
    int execute();
    ~daBoomerang_c();
    int createHeap();
    int create();
    daBoomerang_c();

    u32 checkStateFlg0(daBoomerang_FLG0 i_flag) const { return m_stateFlg0 & i_flag; }
    void onStateFlg0(daBoomerang_FLG0 flag) { m_stateFlg0 |= flag; }
    void offStateFlg0(daBoomerang_FLG0 flag) { m_stateFlg0 &= ~flag; }
    u32 getReturnFlg() { return checkStateFlg0(FLG0_1); }
    void onWindCatch() { onStateFlg0(WIND_CATCH); }
    bool checkForceDelete() const { return m_forceDelete; }
    void onForceDelete() { m_forceDelete = true; }
    bool checkCharge() const { return true; }
    void setThrow() { fopAcM_SetParam(this, 1); }
    void setAimActor(fopAc_ac_c* i_actor) { (this->*m_setAimActorFn)(i_actor); }
    void cancelLockActor(fopAc_ac_c* i_actor) { (this->*m_cancelLockActorFn)(i_actor); }
    u8 getLockReserve() const { return m_sight.getReserve(); }
    bool getLockCntMax() { return m_lockCnt >= BOOMERANG_LOCK_MAX; }
    void onLockDistanceCancel() { onStateFlg0(FLG0_LOCK_DIST_CANCEL); }

private:
    /* 0x568 */ J3DModel* mp_boomModel;
    /* 0x56C */ J3DModel* mp_shippuModel;
    /* 0x570 */ J3DAnmTextureSRTKey* m_windBtk;
    /* 0x574 */ mDoExt_bckAnm m_shippuBck;
    /* 0x590 */ J3DModel* mp_setboomEfModel;
    /* 0x594 */ J3DAnmTextureSRTKey* m_waitEffBtk;
    /* 0x598 */ daBoomerang_sight_c m_sight;
    /* 0x6A8 */ fpc_ProcID m_lockLineActorID;
    /* 0x6AC */ fpc_ProcID m_lockActorIDs[BOOMERANG_LOCK_MAX];
    /* 0x6C0 */ fopAc_ac_c* m_lockActors[BOOMERANG_LOCK_MAX]; // Actors corresponding to the five target points
    /* 0x6D4 */ fpc_ProcID field_0x6d4;
    /* 0x6D8 */ fopAc_ac_c* field_0x6d8;
    /* 0x6DC */ cXyz m_lockActorsPositions[BOOMERANG_LOCK_MAX];
    /* 0x718 */ u8 field_0x718[BOOMERANG_LOCK_MAX];
    /* 0x720 */ Z2SoundObjSimple m_sound;
    /* 0x740 */ dBgS_ObjAcch m_acch;
    /* 0x918 */ dPaPo_c m_papo;
    /* 0x950 */ u8 m_lockCnt;
    /* 0x951 */ u8 field_0x951;
    /* 0x952 */ u8 field_0x952;
    /* 0x953 */ s8 m_reverb;
    /* 0x954 */ u8 field_0x954;
    /* 0x955 */ u8 field_0x955;
    /* 0x956 */ bool m_forceDelete;
    /* 0x957 */ u8 field_0x957;
    /* 0x958 */ s16 field_0x958;
    /* 0x95A */ s16 m_modelRotY;
    /* 0x95C */ s16 field_0x95c;
    /* 0x95E */ s16 field_0x95e;
    /* 0x960 */ s16 field_0x960;
    /* 0x962 */ s16 field_0x962;
    /* 0x964 */ u32 m_stateFlg0;
    /* 0x968 */ u32 field_0x968;
    /* 0x96C */ u32 field_0x96c;
    /* 0x970 */ u32 field_0x970;
    /* 0x974 */ u32 field_0x974;
    /* 0x978 */ u32 field_0x978;
    /* 0x97C */ u32 field_0x97c;
    /* 0x980 */ f32 mGroundY;
    /* 0x984 */ f32 m_lockLineSqdist;
    /* 0x988 */ f32 field_0x988;
    /* 0x98C */ f32 m_shippuFrame;
    /* 0x990 */ cXyz m_aimPos;
    /* 0x99C */ cXyz field_0x99c;
    /* 0x9A8 */ f32 m_shippuSize;
    /* 0x9AC */ cXyz m_moveTargetPos;
    /* 0x9B8 */ cXyz field_0x9b8;
    /* 0x9C4 */ cXyz m_movePos;
    /* 0x9D0 */ dCcD_Stts m_colliderStts;
    /* 0xA0C */ dCcD_Cps m_atCps;
    /* 0xB50 */ dCcD_Cyl m_windAtCyl;
    /* 0xC8C */ dBgS_BoomerangLinChk m_boomLineChk;
    /* 0xCFC */ dBgS_ObjLinChk m_lineChk;
    /* 0xD6C */ dBgS_ObjGndChk m_gndChk;
    /* 0xDC0 */ int (daBoomerang_c::*m_procFn)();
    /* 0xDCC */ void (daBoomerang_c::*m_setAimActorFn)(fopAc_ac_c*);
    /* 0xDD8 */ void (daBoomerang_c::*m_cancelLockActorFn)(fopAc_ac_c*);
};

STATIC_ASSERT(sizeof(daBoomerang_c) == 0xDE4);

class daBoomerang_HIO_c0 {
public:
    static u16 const m_lockWaitTime;
    static f32 const m_minCircleR;
    static f32 const m_middleCircleR;
    static f32 const m_maxCircleR;
    static f32 const m_scale;
    static f32 const m_lockWindScale;
};

#endif /* D_A_BOOMERANG_H */
