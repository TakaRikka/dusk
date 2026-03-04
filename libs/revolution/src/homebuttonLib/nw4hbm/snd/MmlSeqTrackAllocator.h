#ifndef NW4HBM_SND_MML_SEQ_TRACK_ALLOCATOR_H
#define NW4HBM_SND_MML_SEQ_TRACK_ALLOCATOR_H

#include <revolution/types.h>

#include "InstancePool.h"
#include "SeqTrackAllocator.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {

            class MmlParser;
            class MmlSeqTrack;
            class SeqPlayer;
            class SeqTrack;

            class MmlSeqTrackAllocator : public SeqTrackAllocator {
            public:
                explicit MmlSeqTrackAllocator(MmlParser* parser) : mParser(parser) {}

                /* 0x0C */ virtual SeqTrack* AllocTrack(SeqPlayer* player);
                /* 0x10 */ virtual void FreeTrack(SeqTrack* track);

                u32 Create(void* buffer, u32 size);
                void Destroy(void* buffer, u32 size);

            private:
                /* 0x04 */ MmlParser* mParser;
                /* 0x08 */ InstancePool<MmlSeqTrack> mTrackPool;
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
