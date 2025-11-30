#ifndef C_BG_W_H
#define C_BG_W_H

#include "dolphin/types.h"

class cBgW_BgId {
private:
    /* 0x0 */ u16 m_id;
    /* 0x4 vtable */

public:
    void Release();
    bool ChkUsed() const;
    void Regist(int);

    virtual ~cBgW_BgId() {}

    cBgW_BgId() { Ct(); }
    void Ct() { m_id = 0x100; }

    u16 GetId() const { return m_id; }
};

bool cBgW_CheckBGround(float a1);
bool cBgW_CheckBRoof(float a1);
bool cBgW_CheckBWall(float a1);

#endif /* C_BG_W_H */
