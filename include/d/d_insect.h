#ifndef D_D_INSECT_H
#define D_D_INSECT_H

#include "f_op/f_op_actor.h"

class dInsect_c : public fopAc_ac_c {
public:
    enum Sex {
        /* 0 */ SEX_MALE,
        /* 1 */ SEX_FEMALE,
    };

    enum Location {
        /* 0 */ LOC_OUTSIDE,
        /* 1 */ LOC_AGITHA,
        /* 2 */ LOC_UNK_2,
        /* 3 */ LOC_UNK_3,
    };

    dInsect_c();
    void Insect_GetDemoMain();
    void CalcZBuffer(f32);

    virtual void Insect_Release() { field_0x56c = 1; }
    bool ChkGetDemo() { return m_mode > 0; }
    void setItemNo(u8 i_itemNo) { m_itemNo = i_itemNo; }
    void setSaveBitNo(s16 i_saveBitNo) { m_saveBitNo = i_saveBitNo; }
    bool CheckZ() { return (u32)field_0x57c > field_0x578; }

public:
    /* 0x56C */ u8 field_0x56c;
    /* 0x56D */ bool mDraw;
    /* 0x570 */ u32 mParticleKey1;
    /* 0x574 */ u32 mParticleKey2;
    /* 0x578 */ u32 field_0x578;
    /* 0x57C */ f32 field_0x57c;
    /* 0x580 */ u8 m_itemNo;
    /* 0x581 */ u8 mSex;
    /* 0x582 */ s16 m_saveBitNo;
    /* 0x584 */ u8 field_0x584;
    /* 0x585 */ u8 field_0x585;
    /* 0x586 */ u8 field_0x586[2];
    /* 0x588 */ u32 m_itemId;
    /* 0x58C */ int m_mode;

};

#endif /* D_D_INSECT_H */
