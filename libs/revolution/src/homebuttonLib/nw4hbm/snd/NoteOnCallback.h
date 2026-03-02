#ifndef NW4HBM_SND_NOTE_ON_CALLBACK_H
#define NW4HBM_SND_NOTE_ON_CALLBACK_H

#include <revolution/types.h>

#include "Channel.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {

            typedef struct NoteOnInfo {
                /* 0x00 */ int prgNo;
                /* 0x04 */ int key;
                /* 0x08 */ int velocity;
                /* 0x0C */ int length;
                /* 0x10 */ int initPan;
                /* 0x14 */ int priority;
                /* 0x18 */ int voiceOutCount;
                /* 0x1C */ Channel::ChannelCallback channelCallback;
                /* 0x20 */ u32 channelCallbackData;
            };

            class SeqPlayer;
            class NoteOnCallback {
            public:
                /* 0x08 */ virtual ~NoteOnCallback() {}
                /* 0x0C */ virtual Channel* NoteOn(SeqPlayer* player, int bankNo,
                                                   const NoteOnInfo& noteOnInfo) = 0;
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
