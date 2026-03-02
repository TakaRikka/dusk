#ifndef JAUAUDIBLEPARAM_H
#define JAUAUDIBLEPARAM_H

#include <types.h>

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAUAudibleParam {
    f32 getDopplerPower() const {
        return (u32)((*(u8*)&field_0x0.raw >> 4) & 0xf) * (1.0f / 15.0f);
    }

    union {
        struct {
            u16 f0;
            u16 f1;
        } half;
        struct {
            u8 b0_0 : 4;
            u8 b0_4 : 1;
            u8 b0_5 : 1;
            u8 b0_6 : 1;
            u8 b0_7 : 1;
            u8 b1_0 : 1;
            u8 b1_1 : 1;
            u8 b1_2_7 : 6;
            u8 b2;
            u8 b3;
        } bytes;
        u32 raw;
    } field_0x0;
};

#endif /* JAUAUDIBLEPARAM_H */
