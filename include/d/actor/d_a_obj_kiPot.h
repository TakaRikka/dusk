#ifndef D_A_OBJ_KIPOT_H
#define D_A_OBJ_KIPOT_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daKiPot_c
 * @brief Coro Pot
 *
 * @details
 *
 */
class daKiPot_c : public fopAc_ac_c {
public:
    void setBaseMtx();
    int create();
    int Execute();
    void procMain();
    void init_modeWait();
    void modeWait();
    s32 chkEvent();
    int Draw();
    int Delete();

private:
    /* 0x568 */ u8 field_0x568[0x574 - 0x568];
    /* 0x574 */ u8 mMode;
    /* 0x575 */ u8 field_0x575;
    /* 0x576 */ u8 field_0x576[0x578 - 0x576];
};

STATIC_ASSERT(sizeof(daKiPot_c) == 0x578);

class daKiPot_HIO_c : public mDoHIO_entry_c {
public:
    daKiPot_HIO_c();
    virtual ~daKiPot_HIO_c() {};

    void genMessage(JORMContext*);

    /* 0x00 */ /* vtable */
    /* 0x06 */ u8 field_0x06;
};

#endif /* D_A_OBJ_KIPOT_H */
