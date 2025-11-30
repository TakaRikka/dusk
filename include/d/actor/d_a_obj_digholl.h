#ifndef D_A_OBJ_DIGHOLL_H
#define D_A_OBJ_DIGHOLL_H

#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class daObjDigholl_c
 * @brief Wolf Dig Place (Passage)
 *
 * @details
 *
 */
class daObjDigholl_c : public fopAc_ac_c {
public:
    int create();
    ~daObjDigholl_c();
    int execute();
    int draw();

    void onDigStart() { field_0x568 = 1; }
    s16 getSceneNum() const { return field_0x56c; }

private:
    /* 0x568 */ u8 field_0x568;
    /* 0x569 */ u8 field_0x569;
    /* 0x56A */ u8 field_0x56a;
    /* 0x56B */ u8 field_0x56b;
    /* 0x56C */ s16 field_0x56c;
    /* 0x56E */ s16 field_0x56e;
};     // Size: 0x570

#endif /* D_A_OBJ_DIGHOLL_H */
