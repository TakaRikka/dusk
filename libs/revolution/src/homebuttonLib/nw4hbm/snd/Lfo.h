#ifndef NW4HBM_SND_LFO_H
#define NW4HBM_SND_LFO_H

#include <revolution/types.h>

namespace nw4hbm {
    namespace snd {
        namespace detail {

            typedef struct LfoParam {
                LfoParam() { Init(); }

                void Init();

                /* 0x00 */ f32 depth;
                /* 0x04 */ f32 speed;
                /* 0x08 */ u32 delay;
                /* 0x0C */ u8 range;
                /* 0x0D */ u8 padding[3];
            } LfoParam;

            class Lfo {
            public:
                Lfo() : mDelayCounter(0), mCounter(0.0f) {}

                LfoParam& GetParam() { return mParam; }
                void SetParam(const LfoParam& rParam) { mParam = rParam; }

                void Reset();
                void Update(int msec);

                f32 GetValue() const;

            private:
                static const int TABLE_SIZE = 32;

                static s8 GetSinIdx(int idx);

                /* 0x00 */ LfoParam mParam;
                /* 0x10 */ u32 mDelayCounter;
                /* 0x14 */ f32 mCounter;
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
