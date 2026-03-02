#ifndef NW4HBM_SND_SEQ_SOUND_H
#define NW4HBM_SND_SEQ_SOUND_H

#include <revolution/types.h>

#include "debug.h"

#include "../ut/LinkList.h"

namespace nw4hbm {
    namespace snd {

        class SeqSoundHandle;

        namespace detail {
            class NoteOnCallback;
            class SeqTrackAllocator;
            class SeqPlayer;
            class BasicSound;
            template <typename T>
            class SoundInstanceManager;
        }  // namespace detail

        namespace detail {
            class SeqSound : public BasicSound {
                friend class SeqSoundHandle;

            public:
                typedef void (*NotifyAsyncEndCallback)(bool result, const void* seqBase,
                                                       s32 seqOffset, void* userData);

                class SeqLoadCallback {
                public:
                    typedef enum Result {
                        /* 0 */ RESULT_SUCCESS = 0,
                        /* 1 */ RESULT_FAILED,
                        /* 2 */ RESULT_CANCELED,
                        /* 3 */ RESULT_ASYNC,
                        /* 4 */ RESULT_RETRY
                    } Result;

                    /* 0x08 */ virtual ~SeqLoadCallback() {}
                    /* 0x0C */ virtual Result LoadData(SeqSound::NotifyAsyncEndCallback callback,
                                                       void* callbackArg, u32 userData) const = 0;
                    /* 0x10 */ virtual void CancelLoading(u32 userData) const = 0;
                };

                explicit SeqSound(SoundInstanceManager<SeqSound>* nanager);

                /* 0x08 */ NW4HBM_UT_RUNTIME_TYPEINFO;
                /* 0x28 */ virtual void Shutdown();
                /* 0x4C */ virtual void SetPlayerPriority(int priority);
                /* 0x50 */ virtual bool IsAttachedTempSpecialHandle();
                /* 0x54 */ virtual void DetachTempSpecialHandle();
                /* 0x58 */ virtual void InitParam();
                /* 0x68 */ virtual BasicPlayer& GetBasicPlayer() { return mSeqPlayer; }
                /* 0x6C */ virtual const BasicPlayer& GetBasicPlayer() const { return mSeqPlayer; }

                SeqPlayer::SetupResult Setup(SeqTrackAllocator* trackAllocator, u32 allocTrackFlags,
                                             int voices, NoteOnCallback* callback);

                void Prepare(const void* seqBase, s32 seqOffset);
                void Prepare(const SeqLoadCallback* callback, u32 callbackData);

                void SetChannelPriority(int priority);

            private:
                static void NotifyLoadAsyncEndSeqData(bool result, const void* seqBase,
                                                      s32 seqOffset, void* userData);

                /* 0x0D8 */ SeqPlayer mSeqPlayer;
                /* 0x1F4 */ SeqSoundHandle* mTempSpecialHandle;
                /* 0x1F8 */ SoundInstanceManager<SeqSound>* mManager;
                /* 0x1E8 */ bool mLoadingFlag;
                /* 0x1EC */ const SeqLoadCallback* mCallback;
                /* 0x1F0 */ u32 mCallbackData;
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
