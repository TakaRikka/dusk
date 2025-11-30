#ifndef D_A_TAG_LV6GATE_H
#define D_A_TAG_LV6GATE_H

#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"

class daTagLv6Gate_c : public fopAc_ac_c {
public:
    int createHeap();
    int create();
    inline int execute();
    int draw();

    inline ~daTagLv6Gate_c();

    inline void initBaseMtx();
    inline void setBaseMtx();
    inline void create_init();
    inline bool checkOpenArea();

    void stabMasterSword() { mIsMasterSwordStabbed = 1; }
    u8 getSwitchNo1() { return fopAcM_GetParam(this) & 0xFF; }
    u8 getSwitchNo2() { return fopAcM_GetParam(this) >> 0x8 & 0xFF; }

    /* 0x568 */ J3DModel* mpModel[2];
    /* 0x570 */ mDoExt_btkAnm* mpBtk[2];
    /* 0x578 */ dBgW mBgW[2];
    /* 0x6F8 */ Mtx field_0x6f8[2];
    /* 0x758 */ request_of_phase_process_class mPhase;
    /* 0x760 */ f32 field_0x760[2];
    /* 0x768 */ s16 mEvtId;
    /* 0x76A */ bool mIsMasterSwordStabbed;
};  // Size: 0x76C

#endif /* D_A_TAG_LV6GATE_H */
