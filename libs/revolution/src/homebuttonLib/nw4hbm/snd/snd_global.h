#ifndef NW4HBM_SND_GLOBAL_H
#define NW4HBM_SND_GLOBAL_H

#include <revolution/types.h>

namespace nw4hbm {
    namespace snd {

        enum PanMode {
            PAN_MODE_DUAL,
            PAN_MODE_BALANCE,
        };

        enum PanCurve {
            PAN_CURVE_SQRT,
            PAN_CURVE_SQRT_0DB,
            PAN_CURVE_SQRT_0DB_CLAMP,

            PAN_CURVE_SINCOS,
            PAN_CURVE_SINCOS_0DB,
            PAN_CURVE_SINCOS_0DB_CLAMP,

            PAN_CURVE_LINEAR,
            PAN_CURVE_LINEAR_0DB,
            PAN_CURVE_LINEAR_0DB_CLAMP,
        };

        struct VoiceOutParam {
        public:
            VoiceOutParam()
                : volume(1.0f), pitch(1.0f), pan(0.0f), surroundPan(0.0f), fxSend(0.0f), lpf(0.0f) {
            }

        public:
            /* 0x00 */ f32 volume;
            /* 0x04 */ f32 pitch;
            /* 0x08 */ f32 pan;
            /* 0x0C */ f32 surroundPan;
            /* 0x10 */ f32 fxSend;
            /* 0x14 */ f32 lpf;
        };  // size = 0x18
    }  // namespace snd
}  // namespace nw4hbm

#endif
