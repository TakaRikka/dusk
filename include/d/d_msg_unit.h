#ifndef D_MSG_D_MSG_UNIT_H
#define D_MSG_D_MSG_UNIT_H

#include "JSystem/JMessage/JMessage.h"

class dMsgUnit_c {
public:
    dMsgUnit_c();
    void setTag(int, int, TEXT_SPAN, bool);

#if TARGET_PC
    void setTag_jpn(int, int, TEXT_SPAN, bool);
#endif

    virtual ~dMsgUnit_c();
};

DUSK_GAME_EXTERN dMsgUnit_c g_msg_unit;

inline void dMsgUnit_setTag(int param_0, int param_1, TEXT_SPAN param_2) {
    g_msg_unit.setTag(param_0, param_1, param_2, true);
}

#endif /* D_MSG_D_MSG_UNIT_H */
