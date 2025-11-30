#ifndef D_A_TAG_CHKPOINT_H
#define D_A_TAG_CHKPOINT_H

#include "d/d_com_inf_game.h"
#include "JSystem/JKernel/JKRHeap.h"

class daTag_Chk_c : public fopAc_ac_c {
public:
    int execute();

    u16 getAreaType() { return home.angle.z & 0x100; }
    u16 getHeight() { return fopAcM_GetParam(this) & 0xff; }
    u8 getSwBit() { return fopAcM_GetParam(this) >> 8; }
    u8 getSwBit2() { return fopAcM_GetParam(this) >> 16; }

    int create() {
        fopAcM_ct(this, daTag_Chk_c);

        if (getAreaType() == 0) {
            scale.x *= 50.0f;
            scale.y *= 100.0f;
            scale.z *= 50.0f;

            mVtxA.set(current.pos.x - scale.x, current.pos.y, current.pos.z - scale.z);
            mVtxB.set(current.pos.x + scale.x, current.pos.y + scale.y, current.pos.z + scale.z);
        } else {
            scale.x *= 100.0f;
            scale.y *= 100.0f;
            scale.z *= 0.0f;

            mVtxA.set(0.0f, current.pos.y, 0.0f);
            mVtxB.set(0.0f, current.pos.y + scale.y, 0.0f);
        }

        attention_info.position = current.pos;

        u16 height = getHeight();
        height != 0xff ? attention_info.position.y += height * 10.0f : attention_info.position.y += scale.y * 0.5f;

        eyePos = attention_info.position;
        return cPhs_COMPLEATE_e;
    }

private:
    /* 0x568 */ u8 field_0x568[8];
    /* 0x570 */ cXyz mVtxA;
    /* 0x57C */ cXyz mVtxB;
};

STATIC_ASSERT(sizeof(daTag_Chk_c) == 0x588);

#endif /* D_A_TAG_CHKPOINT_H */
