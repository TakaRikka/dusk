#ifndef JASREGISTERPARAM_H
#define JASREGISTERPARAM_H

#include "dolphin/types.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
class JASRegisterParam {
public:
    enum RegID {
        REG0,
        REG1,
        REG2,
        REG3,
    };

    JASRegisterParam();
    void init();
    void write(JASRegisterParam::RegID, u32);
    u32 read(JASRegisterParam::RegID);

    u16 field_0x0[14];
};

#endif /* JASREGISTERPARAM_H */
