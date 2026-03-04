#ifndef NW4HBM_SND_ENV_GENERATOR_H
#define NW4HBM_SND_ENV_GENERATOR_H

#include "../ut/LinkList.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {
            class EnvGenerator {
            public:
                typedef enum Status {
                    STATUS_ATTACK = 0,
                    STATUS_DECAY,
                    STATUS_SUSTAIN,
                    STATUS_RELEASE
                } Status;

                EnvGenerator();

                void Init();
                void Reset();
                f32 GetValue() const;
                void Update(int msec);

                Status GetStatus() const { return mStatus; }
                void SetStatus(Status status) { mStatus = status; }

                void SetAttack(int attack);
                void SetDecay(int decay);
                void SetSustain(int sustain);
                void SetRelease(int release);

            private:
                static const int DECIBEL_SQUARE_TABLE_SIZE = 128;

                static const vf32 VOLUME_INIT;
                static const int ATTACK_INIT = 127;
                static const int DECAY_INIT = 127;
                static const int SUSTAIN_INIT = 127;
                static const int RELEASE_INIT = 127;

            private:
                f32 CalcRelease(int release);
                int CalcDecibelSquare(int scale);

                /* 0x00 */ Status mStatus;
                /* 0x04 */ f32 mValue;
                /* 0x08 */ f32 mDecay;
                /* 0x0C */ f32 mRelease;
                /* 0x10 */ f32 mAttack;
                /* 0x14 */ u8 mSustain;
                /* 0x16 */ u16 padding;

                static const s16 DecibelSquareTable[DECIBEL_SQUARE_TABLE_SIZE];
            };
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
