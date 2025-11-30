#ifndef D_A_B_ZANT_MOBILE_H
#define D_A_B_ZANT_MOBILE_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"
#include "d/d_bg_w.h"

/**
 * @ingroup actors-enemies
 * @class daB_ZANTZ_c
 * @brief Zant (Mobile)
 *
 * @details Palace of Twilight dungeon boss (Moving Helmet Structure).
 *
 */
class daB_ZANTZ_c : public fopEn_enemy_c {
public:
    int draw();
    void setSnortEffect(int);
    void calcSnortEffect();
    void setMouthMode(u8);
    void setAppearMode(u8);
    f32 getGroundPos();
    void action();
    void cc_set();
    int execute();
    int _delete();
    int CreateHeap();
    int create();

    int getMouthMode() { return mMouthMode; }
    int getAppearMode() { return mAppearMode; }
    bool isSearchContinue() { return mSearchContinue != false; }
    void offSearchContinue() { mSearchContinue = false; }

private:
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x5B8 */ mDoExt_brkAnm* mpBrk;
    /* 0x5BC */ Z2CreatureEnemy mSound;
    /* 0x660 */ f32 field_0x660;
    /* 0x664 */ f32 field_0x664;
    /* 0x668 */ int field_0x668;
    /* 0x66C */ u8 mMouthMode;
    /* 0x66D */ u8 mAppearMode;
    /* 0x66E */ u8 field_0x66e;
    /* 0x66F */ u8 field_0x66f;
    /* 0x670 */ u8 field_0x670;
    /* 0x671 */ u8 mSearchContinue;
    /* 0x674 */ Mtx mBgMtx;
    /* 0x6A4 */ dBgW* mpBgW;
    /* 0x6A8 */ dCcD_Stts mCcStts;
    /* 0x6E4 */ dCcD_Sph field_0x6e4[3];
    /* 0xA8C */ dCcD_Cyl mSnortColliders[2];
    /* 0xD04 */ u32 field_0xd04;
    /* 0xD08 */ u32 field_0xd08;
    /* 0xD0C */ u32 field_0xd0c;
    /* 0xD10 */ u8 mInitHIO;
};

STATIC_ASSERT(sizeof(daB_ZANTZ_c) == 0xd14);

class daB_ZANTZ_HIO_c {
public:
    daB_ZANTZ_HIO_c();
    virtual ~daB_ZANTZ_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 mModelSize;
};


#endif /* D_A_B_ZANT_MOBILE_H */
