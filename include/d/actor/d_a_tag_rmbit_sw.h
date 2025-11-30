#ifndef D_A_TAG_RMBIT_SW_H
#define D_A_TAG_RMBIT_SW_H

#include "d/d_com_inf_game.h"

class daTagRmbitSw_c : public fopAc_ac_c {
public:
    daTagRmbitSw_c();
    virtual ~daTagRmbitSw_c();
    int create();
    int Delete();
    int draw();
    int execute();
    void init();
    u8 chkPlyrInTag();

    s8 getArg0_RoomSaveTableNo() { return fopAcM_GetParamBit(this, 0, 8); }
    s8 getArg1_RoomNo() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getArg2_CngBitVal() { return fopAcM_GetParamBit(this, 16, 1); }
    u8 getArg3_CngBitFlg() { return fopAcM_GetParamBit(this, 17, 1); }
    u8 getSwBit() { return fopAcM_GetParamBit(this, 24, 8); }

    /* 0x56C */ f32 field_0x56c;
    /* 0x570 */ f32 field_0x570;
    /* 0x574 */ f32 field_0x574;

};  // Size: 0x578

#endif /* D_A_TAG_RMBIT_SW_H */
