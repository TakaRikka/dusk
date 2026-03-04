#ifndef NW4HBM_SND_BASIC_SOUND_H
#define NW4HBM_SND_BASIC_SOUND_H

#include <revolution/types.h>
#include <revolution/wpad.h>

#include "MoveValue.h"
#include "snd_global.h"
#include "snd_types.h"

#include "../ut/LinkList.h"
#include "../ut/RuntimeTypeInfo.h"
#include "../ut/inlines.h"

namespace nw4hbm {
    namespace snd {

        class SoundHandle;
        class SoundPlayer;

        namespace detail {
            namespace {
                class BasicPlayer;
            }
            class ExternalSoundPlayer;
            class PlayerHeap;
        }  // namespace detail

        namespace detail {
            class BasicSound {
            public:
                typedef struct AmbientParamUpdateCallback {
                    typedef enum ParamUpdateFlags {
                        /* 1 */ PARAM_UPDATE_VOLUME = (1 << 0),
                        /* 2 */ PARAM_UPDATE_PAN = (1 << 1),
                        /* 4 */ PARAM_UPDATE_SURROUND_PAN = (1 << 2),
                        /* 8 */ PARAM_UPDATE_PRIORITY = (1 << 3),
                    } ParamUpdateFlags;

                    /* 0x0C */ virtual void detail_Update(SoundParam* param, u32 id,
                                                          BasicSound* sound, const void* arg,
                                                          u32 flags) = 0;
                } AmbientParamUpdateCallback;

                typedef struct AmbientArgUpdateCallback {
                    /* 0x0C */ virtual void detail_Update(void* arg, const BasicSound* sound) = 0;
                } AmbientArgUpdateCallback;

                typedef struct AmbientArgAllocaterCallback {
                    /* 0x0C */ virtual void* detail_AllocAmbientArg(u32 size) = 0;
                    /* 0x10 */ virtual void detail_FreeAmbientArg(void* arg,
                                                                  const BasicSound* sound) = 0;
                } AmbientArgAllocaterCallback;

                typedef struct AmbientArgInfo {
                    /* 0x00 */ AmbientParamUpdateCallback* paramUpdateCallback;
                    /* 0x04 */ AmbientArgUpdateCallback* argUpdateCallback;
                    /* 0x08 */ AmbientArgAllocaterCallback* argAllocaterCallback;
                    /* 0x0C */ void* arg;
                    /* 0x10 */ u32 argSize;
                } AmbientArgInfo;

                static const u32 INVALID_ID = 0xFFFFFFFF;
                static const int PRIORITY_MAX = 127;

            public:
                BasicSound();

                /* 0x08 */ NW4HBM_UT_RUNTIME_TYPEINFO;
                /* 0x0C */ virtual ~BasicSound() {}
                /* 0x10 */ virtual void Update();
                /* 0x14 */ virtual void StartPrepared();
                /* 0x18 */ virtual void Stop(int frames = 0);
                /* 0x1C */ virtual void Pause(bool flag, int frames);
                /* 0x20 */ virtual void SetAutoStopCounter(int count);
                /* 0x24 */ virtual void FadeIn(int frames);
                /* 0x28 */ virtual void Shutdown();
                /* 0x2C */ virtual bool IsPrepared() const;
                /* 0x30 */ virtual bool IsPause() const;
                /* 0x34 */ virtual void SetInitialVolume(f32 volume);
                /* 0x38 */ virtual void SetVolume(f32 volume, int frames);
                /* 0x3C */ virtual void SetPitch(f32 pitch);
                /* 0x40 */ virtual void SetPan(f32 pan);
                /* 0x44 */ virtual void SetSurroundPan(f32 pan);
                /* 0x48 */ virtual void SetLpfFreq(f32 freq);
                /* 0x4C */ virtual void SetPlayerPriority(int priority);
                /* 0x50 */ virtual bool IsAttachedTempSpecialHandle() = 0;
                /* 0x54 */ virtual void DetachTempSpecialHandle() = 0;
                /* 0x58 */ virtual void InitParam();
                /* 0x5C */ virtual BasicPlayer& GetBasicPlayer() = 0;
                /* 0x60 */ virtual const BasicPlayer& GetBasicPlayer() const = 0;

                PlayerHeap* GetPlayerHeap() { return mHeap; }
                void SetPlayerHeap(PlayerHeap* heap) { mHeap = heap; }

                bool IsAttachedGeneralHandle();
                void DetachGeneralHandle();

                bool IsAttachedTempGeneralHandle();
                void DetachTempGeneralHandle();

                SoundPlayer* GetSoundPlayer() { return mSoundPlayer; }
                void SetSoundPlayer(SoundPlayer* player) { mSoundPlayer = player; }

                ExternalSoundPlayer* GetExternalSoundPlayer() { return mExtSoundPlayer; }
                void SetExternalSoundPlayer(ExternalSoundPlayer* extPlayer) {
                    mExtSoundPlayer = extPlayer;
                }

                AmbientParamUpdateCallback* GetAmbientParamUpdateCallback() {
                    return mAmbientParamUpdateCallback;
                }
                AmbientArgUpdateCallback* GetAmbientArgUpdateCallback() {
                    return mAmbientArgUpdateCallback;
                }
                void ClearAmbientArgUpdateCallback() { mAmbientArgUpdateCallback = NULL; }

                AmbientArgAllocaterCallback* GetAmbientArgAllocaterCallback() {
                    return mAmbientArgAllocaterCallback;
                }

                void* GetAmbientArg() { return mAmbientArg; }
                SoundParam& GetAmbientParam() { return mAmbientParam; }

                void SetAmbientParamCallback(AmbientParamUpdateCallback* paramUpdateCallback,
                                             AmbientArgUpdateCallback* argUpdateCallback,
                                             AmbientArgAllocaterCallback* argAllocaterCallback,
                                             void* callbackArg);

                void SetPriority(int priority) { mPriority = priority; }

                u32 GetId() const { return mId; }
                void SetId(u32 id);

                f32 GetMoveVolume() { return mExtMoveVolume.GetValue(); }

                f32 GetInitialVolume() const;
                f32 GetPan() const;
                f32 GetSurroundPan() const;
                f32 GetPitch() const;
                f32 GetVolume() const;

                void SetOutputLine(int flag);
                bool IsEnabledOutputLine() const;
                int GetOutputLine() const;

                f32 GetMainOutVolume() const;
                void SetMainOutVolume(f32 volume);

                f32 GetRemoteOutVolume(int remote) const;
                void SetRemoteOutVolume(int remote, f32 volume);

                void SetFxSend(AuxBus bus, f32 send);

                int CalcCurrentPlayerPriority() const {
                    return ut::Clamp(mPriority + mAmbientParam.priority, (s32)0, (s32)PRIORITY_MAX);
                }

            private:
                /* 0x04 */ PlayerHeap* mHeap;
                /* 0x08 */ SoundHandle* mGeneralHandle;
                /* 0x0C */ SoundHandle* mTempGeneralHandle;
                /* 0x10 */ SoundPlayer* mSoundPlayer;
                /* 0x14 */ ExternalSoundPlayer* mExtSoundPlayer;
                /* 0x18 */ AmbientParamUpdateCallback* mAmbientParamUpdateCallback;
                /* 0x1C */ AmbientArgUpdateCallback* mAmbientArgUpdateCallback;
                /* 0x20 */ AmbientArgAllocaterCallback* mAmbientArgAllocaterCallback;
                /* 0x24 */ void* mAmbientArg;
                /* 0x28 */ SoundParam mAmbientParam;
                /* 0x44 */ MoveValue<f32, int> mFadeVolume;
                /* 0x54 */ MoveValue<f32, int> mPauseFadeVolume;
                /* 0x64 */ bool mStartFlag;
                /* 0x65 */ bool mStartedFlag;
                /* 0x66 */ bool mAutoStopFlag;
                /* 0x67 */ bool mPauseFlag;
                /* 0x68 */ bool mPauseFadeFlag;
                /* 0x69 */ bool mFadeOutFlag;
                /* 0x6C */ int mAutoStopCounter;
                /* 0x70 */ u32 mUpdateCounter;
                /* 0x74 */ u8 mPriority;
                /* 0x78 */ u32 mId;
                /* 0x7C */ MoveValue<f32, int> mExtMoveVolume;
                /* 0x8C */ f32 mInitVolume;
                /* 0x90 */ f32 mExtPan;
                /* 0x94 */ f32 mExtSurroundPan;
                /* 0x98 */ f32 mExtPitch;
                /* 0x9C */ bool mOutputLineFlagEnable;
                /* 0xA0 */ int mOutputLineFlag;
                /* 0xA4 */ f32 mMainOutVolume;
                /* 0xA8 */ f32 mRemoteOutVolume[WPAD_MAX_CONTROLLERS];

            public:
                /* 0xB8 */ ut::LinkListNode mPriorityLink;
                /* 0xC0 */ ut::LinkListNode mSoundPlayerPlayLink;
                /* 0xC8 */ ut::LinkListNode mSoundPlayerPriorityLink;
                /* 0xD0 */ ut::LinkListNode mExtSoundPlayerPlayLink;

                friend class SoundHandle;
            };

            typedef ut::LinkList<BasicSound, offsetof(BasicSound, mPriorityLink)>
                BasicSoundPrioList;
            typedef ut::LinkList<BasicSound, offsetof(BasicSound, mSoundPlayerPlayLink)>
                BasicSoundPlayerPlayList;
            typedef ut::LinkList<BasicSound, offsetof(BasicSound, mSoundPlayerPriorityLink)>
                BasicSoundPlayerPrioList;
            typedef ut::LinkList<BasicSound, offsetof(BasicSound, mExtSoundPlayerPlayLink)>
                BasicSoundExtPlayList;

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
