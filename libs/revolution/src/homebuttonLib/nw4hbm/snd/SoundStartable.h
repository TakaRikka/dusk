#ifndef NW4HBM_SND_SOUND_STARTABLE_H
#define NW4HBM_SND_SOUND_STARTABLE_H

#include <revolution/types.h>

namespace nw4hbm {
    namespace snd {
        class SoundHandle;

        namespace detail {
            class ExternalSoundPlayer;
        }

        class SoundStartable {
        public:
            typedef enum StartResult {
                START_SUCCESS = 0,
                START_ERR_LOW_PRIORITY,
                START_ERR_INVALID_LABEL_STRING,
                START_ERR_INVALID_SOUNDID,
                START_ERR_NOT_DATA_LOADED,
                START_ERR_NOT_ENOUGH_PLAYER_HEAP,
                START_ERR_CANNOT_OPEN_FILE,
                START_ERR_NOT_AVAILABLE,
                START_ERR_CANNOT_ALLOCATE_TRACK,
                START_ERR_NOT_ENOUGH_INSTANCE,
                START_ERR_INVALID_PARAMETER,
                START_ERR_INVALID_SEQ_START_LOCATION_LABEL,

                START_ERR_USER = 128,
                START_ERR_UNKNOWN = 255,
            } StartResult;

            typedef struct StartInfo {
                typedef enum EnableFlag {
                    ENABLE_START_OFFSET = (1 << 0),
                    ENABLE_PLAYER_ID = (1 << 1),
                    ENABLE_PLAYER_PRIORITY = (1 << 2)
                } EnableFlag;

                typedef enum StartOffsetType {
                    START_OFFSET_TYPE_SAMPLE = 0,
                    START_OFFSET_TYPE_MILLISEC,
                } StartOffsetType;

                /* 0x00 */ StartOffsetType startOffsetType;
                /* 0x04 */ int startOffset;
                /* 0x08 */ u32 playerId;
                /* 0x0C */ int playerPriority;
                /* 0x10 */ int voiceOutCount;
            } StartInfo;

        public:
            /* 0x08 */ virtual ~SoundStartable() {}
            /* 0x0C */ virtual StartResult
            detail_SetupSound(SoundHandle* soundHandle, u32 id,
                              detail::BasicSound::AmbientArgInfo* ambientArgInfo,
                              detail::ExternalSoundPlayer* extPlayer, bool hold,
                              const StartInfo* startInfo) = 0;
            /* 0x10 */ virtual u32 detail_ConvertLabelStringToSoundId(const char* label) = 0;

            bool StartSound(SoundHandle* soundHandle, u32 id) {
                return detail_StartSound(soundHandle, id, NULL, NULL, NULL) == START_SUCCESS;
            }
            bool StartSound(SoundHandle* soundHandle, unsigned int id) {
                return detail_StartSound(soundHandle, id, NULL, NULL, NULL) == START_SUCCESS;
            }
            bool StartSound(SoundHandle* soundHandle, int id) {
                return detail_StartSound(soundHandle, id, NULL, NULL, NULL) == START_SUCCESS;
            }

            bool HoldSound(SoundHandle* soundHandle, u32 id) {
                return detail_HoldSound(soundHandle, id, NULL, NULL, NULL) == START_SUCCESS;
            }
            bool HoldSound(SoundHandle* soundHandle, unsigned int id) {
                return detail_HoldSound(soundHandle, id, NULL, NULL, NULL) == START_SUCCESS;
            }
            bool HoldSound(SoundHandle* soundHandle, int id) {
                return detail_HoldSound(soundHandle, id, NULL, NULL, NULL) == START_SUCCESS;
            }

            bool PrepareSound(SoundHandle* soundHandle, u32 id) {
                return detail_PrepareSound(soundHandle, id, NULL, NULL, NULL) == START_SUCCESS;
            }
            bool PrepareSound(SoundHandle* soundHandle, unsigned int id) {
                return detail_PrepareSound(soundHandle, id, NULL, NULL, NULL) == START_SUCCESS;
            }
            bool PrepareSound(SoundHandle* soundHandle, int id) {
                return detail_PrepareSound(soundHandle, id, NULL, NULL, NULL) == START_SUCCESS;
            }

        private:
            StartResult detail_StartSound(SoundHandle* soundHandle, u32 id,
                                          detail::BasicSound::AmbientArgInfo* ambientArgInfo,
                                          detail::ExternalSoundPlayer* extPlayer,
                                          const StartInfo* startInfo);

            StartResult detail_HoldSound(SoundHandle* soundHandle, u32 id,
                                         detail::BasicSound::AmbientArgInfo* ambientArgInfo,
                                         detail::ExternalSoundPlayer* extPlayer,
                                         const StartInfo* startInfo);

            StartResult detail_PrepareSound(SoundHandle* soundHandle, u32 id,
                                            detail::BasicSound::AmbientArgInfo* ambientArgInfo,
                                            detail::ExternalSoundPlayer* extPlayer,
                                            const StartInfo* startInfo);
        };

    }  // namespace snd
}  // namespace nw4hbm

#endif
