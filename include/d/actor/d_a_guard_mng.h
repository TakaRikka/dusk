#ifndef D_A_GUARD_MNG_H
#define D_A_GUARD_MNG_H

#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_tag_guard.h"

/**
 * @ingroup actors-unsorted
 * @class daGuardMng_c
 * @brief Guard Manager?
 *
 * @details
 *
 */
class daGuardMng_c : public fopAc_ac_c {
public:
    void countMerchantNum() {
        mMerchantNum++;
    }

    int execute() {
        static u32 const mGuardDataTbl[2] = {
            0x00000200, 0x00000201,
        };

        if (!daPy_py_c::checkNowWolf() || field_0x573 >= mLimitNum) {
            return 1;
        } else {
            if (field_0x571 < mMerchantNum) {
                field_0x571 = mMerchantNum;
            }

            checkMerchantNum();

            if (mpTagGuard) {
                if (field_0x571 > mMerchantNum && 3 < (field_0x571 - mMerchantNum)) {
                    mpTagGuard->createGuard(mGuardDataTbl[0]);
                    field_0x573++;
                    field_0x571 = mMerchantNum;
                    dComIfGs_onSaveDunSwitch(0x3c);
                }
            }
        }

        return 1;
    }

    u8 getLimitNum() {
        return fopAcM_GetParam(this) & 0xff;
    }

    void create_init() {
        mLimitNum = getLimitNum();
        field_0x573 = 0;
    }

    int create() {
        fopAcM_ct(this, daGuardMng_c);
        create_init();
        return cPhs_COMPLEATE_e;
    }

    u8 checkMerchantNum();
    void checkAppearTag(daTagGuard_c*);

    /* 0x568 */ daTagGuard_c* mpTagGuard;
    /* 0x56C */ f32 field_0x56c;
    /* 0x570 */ u8 mMerchantNum;
    /* 0x571 */ u8 field_0x571;
    /* 0x572 */ u8 mLimitNum;
    /* 0x573 */ u8 field_0x573;
};

#endif /* D_A_GUARD_MNG_H */
