#ifndef NW4HBM_SND_MIDI_SEQ_TRACK_H
#define NW4HBM_SND_MIDI_SEQ_TRACK_H

#include "SeqTrack.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {

            // No midi for you
            class MidiSeqTrack : public SeqTrack {
                MidiSeqTrack();
                virtual ~MidiSeqTrack() {}
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
