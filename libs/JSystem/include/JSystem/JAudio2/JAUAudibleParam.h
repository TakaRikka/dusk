#ifndef JAUAUDIBLEPARAM_H
#define JAUAUDIBLEPARAM_H

#include <types.h>
#include "helpers/endian.h"

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JAUAudibleParam {
    f32 getDopplerPower() const {
        return field_0x0.bytes.mDopplerPower * (1.0f / 15.0f);
    }

    union {
        struct {
            BE(u16) f0;
            BE(u16) f1;
        } half;
        struct {
            u8 mDopplerPower : 4;
            u8 mCalculatePriority : 1;
            u8 mCalcDistanceVolume : 1;
            u8 mCalcFxMix : 1;
            u8 mCullAtMaxDistance : 1;
            u8 mCalcPan : 1;
            u8 mCalcDolby : 1;
            /**
             * Most bits in this field are unused, 8 indicates clamping of volume
             * to ensure it doesn't go below 0.2 in mixChannelOut()
             */
            u8 mClampMinVolume : 6;
            u8 b2;
            u8 b3;
        } bytes;
        BE(u32) raw;
    } field_0x0;
};

#endif /* JAUAUDIBLEPARAM_H */
