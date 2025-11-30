#ifndef D_A_TAG_POFIRE_H
#define D_A_TAG_POFIRE_H

#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_hostIO.h"

class daTagPoFire_c : public fopAc_ac_c {
public:
    void setBaseMtx();
    int create();
    int Execute();
    int Draw();
    int Delete();

    void setFireFlag(u8 flag) { field_0x569 = flag; }

    /* 0x568 */ u8 field_0x568;
    /* 0x569 */ u8 field_0x569;
    /* 0x56A */ u16 field_0x56a;
};

class daTagPoFire_HIO_c : public mDoHIO_entry_c {
public:
    daTagPoFire_HIO_c();
    virtual ~daTagPoFire_HIO_c();

    void genMessage(JORMContext*);

    u8 unk_0x4;
};

#endif /* D_A_TAG_POFIRE_H */
