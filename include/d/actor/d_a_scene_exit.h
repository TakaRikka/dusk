#ifndef D_A_SCENE_EXIT_H
#define D_A_SCENE_EXIT_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-unsorted
 * @class daScex_c
 * @brief Scene Exit
 *
 * @details
 *
 */
class daScex_c : public fopAc_ac_c {
public:
    daScex_c() {}
    int checkWork();
    int execute();
    inline int create();

    u8 getArg1() { return (fopAcM_GetParam(this) >> 8) & 0xFF; }
    u8 getArg0() { return fopAcM_GetParam(this); }
    u8 getSwNo() { return fopAcM_GetParam(this) >> 0x18; }
    u16 getOffEventBit() { return home.angle.z & 0xFFF; }
    u16 getOnEventBit() { return home.angle.x & 0xFFF; }
    u8 getPathID() { return (fopAcM_GetParam(this) >> 0x10) & 0xFF; }
    void setSceneChangeOK() { mSceneChangeOK = true; }

    BOOL checkArea(const cXyz* param_0) {
        cXyz spC;
        mDoMtx_multVec(mMatrix, param_0, &spC);
        if (spC.y >= 0.0f && spC.y <= scale.y && fabsf(spC.x) <= scale.x && fabsf(spC.z) <= scale.z) {
            return TRUE;
        }

        return FALSE;
    }

    /* 0x568 */ Mtx mMatrix;
    /* 0x598 */ u8 field_0x598;
    /* 0x599 */ bool mSceneChangeOK;
};

#endif /* D_A_SCENE_EXIT_H */
