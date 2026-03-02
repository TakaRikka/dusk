#ifndef NW4HBM_SND_DEBUG_H
#define NW4HBM_SND_DEBUG_H

namespace nw4hbm {
    namespace snd {

        enum DebugWarningFlag {
            DEBUG_WARNING_NOT_ENOUGH_INSTANCE,
            DEBUG_WARNING_NOT_ENOUGH_SEQSOUND,
            DEBUG_WARNING_NOT_ENOUGH_STRMSOUND,
            DEBUG_WARNING_NOT_ENOUGH_WAVESOUND,
            DEBUG_WARNING_NOT_ENOUGH_SEQTRACK,
            DEBUG_WARNING_NOT_ENOUGH_STRMCHANNEL,
        };

        namespace detail {

            enum DebugSoundType {
                DEBUG_SOUND_TYPE_SEQSOUND,
                DEBUG_SOUND_TYPE_STRMSOUND,
                DEBUG_SOUND_TYPE_WAVESOUND,
            };
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

namespace nw4hbm {
    namespace snd {
        namespace detail {
            bool Debug_GetWarningFlag(DebugWarningFlag warning);
            DebugWarningFlag Debug_GetDebugWarningFlagFromSoundType(DebugSoundType type);
            char const* Debug_GetSoundTypeString(DebugSoundType type);
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
