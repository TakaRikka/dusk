#ifndef D_BG_D_BG_PC_H
#define D_BG_D_BG_PC_H

#include <dolphin/types.h>

struct sBgPc {
    /* 0x00 */ u32 code0;
    /* 0x04 */ u32 code1;
    /* 0x08 */ u32 code2;
    /* 0x0C */ u32 code3;
    /* 0x10 */ u32 code4;
};  // Size: 0x14

#define BGPC_GET_BITS(code, shift, bits) \
    ((code >> shift) & ((1 << bits) - 1))

#define BGPC_CHECK_BIT(code, bit) \
    (code & (1 << bit))

class dBgPc {
public:
    void setCode(sBgPc&);
    
    s32 getExit() const { return BGPC_GET_BITS(m_code.code0, 0, 6); }
    s32 getPolyCol() const { return BGPC_GET_BITS(m_code.code0, 6, 8); }
    u32 getObjThrough() const { return BGPC_CHECK_BIT(m_code.code0, 14); }
    u32 getCamThrough() const { return BGPC_CHECK_BIT(m_code.code0, 15); }
    u32 getLinkThrough() const { return BGPC_CHECK_BIT(m_code.code0, 16); }
    u32 getArrowThrough() const { return BGPC_CHECK_BIT(m_code.code0, 17); }
    u32 getHSStick() const { return BGPC_CHECK_BIT(m_code.code0, 18); }
    u32 getBoomerangThrough() const { return BGPC_CHECK_BIT(m_code.code0, 19); }
    u32 getRopeThrough() const { return BGPC_CHECK_BIT(m_code.code0, 20); }
    u32 getHorseNoEntry() const { return BGPC_CHECK_BIT(m_code.code0, 21); }
    u32 getShdwThrough() const { return BGPC_CHECK_BIT(m_code.code0, 22); }
    u32 getBombThrough() const { return BGPC_CHECK_BIT(m_code.code0, 23); }
    s32 getSpl() const { return BGPC_GET_BITS(m_code.code0, 24, 4); }
    u32 getMagnet() const { return BGPC_GET_BITS(m_code.code0, 28, 2); }
    u32 getUnderwaterRoof() const { return BGPC_CHECK_BIT(m_code.code0, 30); }
    u32 getMonkeyBars() const { return (m_code.code0 >> 31) & 1; }

    s32 getLinkNo() const { return BGPC_GET_BITS(m_code.code1, 0, 8); }
    s32 getWallCode() const { return BGPC_GET_BITS(m_code.code1, 8, 4); }
    u32 getAtt0Code() const { return BGPC_GET_BITS(m_code.code1, 12, 4); }
    u32 getAtt1Code() const { return BGPC_GET_BITS(m_code.code1, 16, 3); }
    u32 getGroundCode() const { return BGPC_GET_BITS(m_code.code1, 19, 5); }
    u32 getIronBallThrough() const { return BGPC_CHECK_BIT(m_code.code1, 24); }
    u32 getAttackThrough() const { return BGPC_CHECK_BIT(m_code.code1, 25); }

    s32 getCamMoveBG() const { return BGPC_GET_BITS(m_code.code2, 0, 8); }
    s32 getRoomCam() const { return BGPC_GET_BITS(m_code.code2, 8, 8); }
    s32 getRoomPath() const { return BGPC_GET_BITS(m_code.code2, 16, 8); }
    s32 getRoomPathPnt() const { return BGPC_GET_BITS(m_code.code2, 24, 8); }

    u8 getRoomInf() const { return BGPC_GET_BITS(m_code.code4, 0, 8); }
    u32 maskNrm() const { return BGPC_CHECK_BIT(m_code.code4, 8); }
    // BUG: Seems like this uses the same mask as maskNrm
    u32 getWtr() const { return BGPC_CHECK_BIT(m_code.code4, 8); }
    s32 getSnd() const { return BGPC_GET_BITS(m_code.code4, 11, 8); }
    s32 getRoom() const { return BGPC_GET_BITS(m_code.code4, 20, 8); }

    /* 0x0 */ sBgPc m_code;
};

#endif /* D_BG_D_BG_PC_H */
