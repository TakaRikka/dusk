#ifndef D_BG_D_BG_PLC_H
#define D_BG_D_BG_PLC_H

#include "d/d_bg_pc.h"

struct sBgPlc {
    /* 0x0 */ u32 magic;        // "SPLC"
    /* 0x4 */ u16 m_code_size;  // Should normally always be 0x14
    /* 0x6 */ u16 m_num;        // Number of sBgPc entries to follow
    /* 0x8 */ sBgPc m_code[0];  // m_num size array
};

class dBgPlc {
public:
    dBgPlc();
    ~dBgPlc();
    void setBase(void*);
    sBgPc* getCode(int, sBgPc**) const;
    u32 getGrpCode(int) const;

    static const int ZELDA_CODE_SIZE = sizeof(sBgPc);

private:
    /* 0x00 */ sBgPlc* m_base;
};

#endif /* D_BG_D_BG_PLC_H */
