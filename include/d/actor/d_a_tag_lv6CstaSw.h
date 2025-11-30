#ifndef D_A_TAG_LV6CSTASW_H
#define D_A_TAG_LV6CSTASW_H

#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_hostIO.h"

class daLv6CstaSw_c : public fopAc_ac_c {
public:
    void setBaseMtx();
    int create();
    static fopAc_ac_c* searchSekizoAct(void*, void*);
    bool areaCheck();
    int Execute();
    int Draw();
    int Delete();

    u8 getSw() { return fopAcM_GetParamBit(this, 0, 8); }

private:
    /* 0x568 */ u8 mSwitch;
    /* 0x569 */ u8 unused[0x570 - 0x569];
};

class daLv6CstaSw_HIO_c : public mDoHIO_entry_c {
public:
    daLv6CstaSw_HIO_c();
    virtual ~daLv6CstaSw_HIO_c();

    void genMessage(JORMContext*);

    u8 unk_0x4;
};

#endif /* D_A_TAG_LV6CSTASW_H */
