#ifndef D_MSG_D_MSG_UNIT_H
#define D_MSG_D_MSG_UNIT_H

#include "JSystem/JMessage/JMessage.h"

class dMsgUnit_c {
public:
    dMsgUnit_c();
    void setTag(int, int, char*, bool);

    virtual ~dMsgUnit_c();
};

extern dMsgUnit_c g_msg_unit;

inline void dMsgUnit_setTag(int param_0, int param_1, char* param_2) {
    g_msg_unit.setTag(param_0, param_1, param_2, true);
}

#endif /* D_MSG_D_MSG_UNIT_H */
