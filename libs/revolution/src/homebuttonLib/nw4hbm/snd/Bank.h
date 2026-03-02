#ifndef NW4HBM_SND_BANK_H
#define NW4HBM_SND_BANK_H

#include <revolution/types.h>

#include "BankFile.h"
#include "NoteOnCallback.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {

            class Channel;

            class Bank {
            public:
                explicit Bank(const void* bankData);
                ~Bank();

                Channel* NoteOn(const NoteOnInfo& noteOnInfo) const;

                void SetWaveDataAddress(const void* waveData) {
                    NW4HBM_ASSERT_CHECK_NULL(47, waveData);
                    mWaveDataAddress = waveData;
                }

            private:
                /* 0x00 */ BankFileReader mBankReader;
                /* 0x0C */ const void* mWaveDataAddress;
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
