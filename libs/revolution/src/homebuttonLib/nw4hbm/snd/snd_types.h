#ifndef NW4HBM_SND_TYPES_H
#define NW4HBM_SND_TYPES_H

#include <revolution/types.h>

#include "../ut/LinkList.h"

namespace nw4hbm {
    namespace snd {
        class LinkedInstance {
        public:
            /* 0x00 */ ut::LinkListNode mInstanceLink;
        };
        template <typename T, int Ofs>
        class InstanceManager {
        public:
            typedef typename ut::LinkList<T, Ofs>::Iterator Iterator;
            void Append(T* obj) { mFreeList.PushBack(obj); }
            void Remove(T* obj) { mFreeList.Erase(obj); }
            T* Alloc() {
                if (mFreeList.IsEmpty()) {
                    return NULL;
                } else {
                    T& obj = mFreeList.GetFront();
                    mFreeList.PopFront();
                    mActiveList.PushBack(&obj);
                    return &obj;
                }
            }
            void Free(T* obj) {
                if (!mActiveList.IsEmpty()) {
                    mActiveList.Erase(obj);
                    mFreeList.PushBack(obj);
                }
            }

            Iterator GetBeginIter() { return mActiveList.GetBeginIter(); }
            Iterator GetEndIter() { return mActiveList.GetEndIter(); }

        private:
            /* 0x00 */ ut::LinkList<T, Ofs> mFreeList;
            /* 0x0C */ ut::LinkList<T, Ofs> mActiveList;
        };

        static const int THREAD_STACK_SIZE = 1024;
        static const int STREAM_BUFFER_SIZE = 512;

        static const int CHANNEL_MAX = 2;

        static const int VOICE_MAX = 4;

        static const int PRIORITY_MAX = 255;

        static const int REMOTE_FILTER_MAX = 127;

        static const f32 VOLUME_MIN_DB = -90.4f;
        static const f32 VOLUME_MAX_DB = 6.0f;
        static const f32 VOLUME_RANGE_DB = -(VOLUME_MIN_DB - VOLUME_MAX_DB);
        static const int VOLUME_RANGE_MB = static_cast<int>(10 * VOLUME_RANGE_DB);

        typedef enum OutputLineFlag {
            OUTPUT_LINE_MAIN = (1 << 0),
            OUTPUT_LINE_REMOTE_N = (1 << 1),
        } OutputLineFlag;

        typedef enum OutputMode {
            OUTPUT_MODE_STEREO = 0,
            OUTPUT_MODE_SURROUND,
            OUTPUT_MODE_DPL2,
            OUTPUT_MODE_MONO
        } OutputMode;

        typedef enum AuxBus { AUX_A = 0, AUX_B, AUX_C, AUX_BUS_NUM } AuxBus;

        typedef enum SampleFormat {
            SAMPLE_FORMAT_PCM_S32 = 0,
            SAMPLE_FORMAT_PCM_S16,
            SAMPLE_FORMAT_PCM_S8,
            SAMPLE_FORMAT_DSP_ADPCM
        } SampleFormat;

        typedef struct SoundParam {
            /* 0x00 */ f32 volume;
            /* 0x04 */ f32 pitch;
            /* 0x08 */ f32 pan;
            /* 0x0C */ f32 surroundPan;
            /* 0x10 */ f32 fxSend;
            /* 0x14 */ f32 lpf;
            /* 0x18 */ s32 priority;
        } SoundParam;

        namespace detail {
            typedef struct AdpcmParam {
                /* 0x00 */ u16 coef[16];
                /* 0x20 */ u16 gain;
                /* 0x22 */ u16 pred_scale;
                /* 0x24 */ u16 yn1;
                /* 0x26 */ u16 yn2;
            } AdpcmParam;

            typedef struct AdpcmLoopParam {
                /* 0x00 */ u16 loop_pred_scale;
                /* 0x02 */ u16 loop_yn1;
                /* 0x04 */ u16 loop_yn2;
            } AdpcmLoopParam;

            typedef struct AdpcmInfo {
                /* 0x08 */ AdpcmParam adpcm;
                /* 0x28 */ AdpcmLoopParam adpcmloop;
                /* 0x2E */ u16 padding;
            } AdpcmInfo;

            typedef struct VoiceChannelParam {
                /* 0x00 */ void* waveData;
                /* 0x04 */ AdpcmInfo adpcmInfo;
            } VoiceChannelParam;
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
