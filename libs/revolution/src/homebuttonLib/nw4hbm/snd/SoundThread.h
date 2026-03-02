#ifndef NW4HBM_SND_SOUND_THREAD_H
#define NW4HBM_SND_SOUND_THREAD_H

#include <revolution/types.h>

#include "AxManager.h"

#include "../ut/LinkList.h"
#include "../ut/inlines.h"

#include <revolution/os/OSMessage.h>
#include <revolution/os/OSMutex.h>
#include <revolution/os/OSThread.h>

namespace nw4hbm {
    namespace snd {
        namespace detail {

            class SoundThread {
            public:
                class Callback {
                public:
                    /* 0x00 */ ut::LinkListNode mLink;

                    /* 0x08 */ virtual ~Callback() {}
                    /* 0x0C */ virtual void UpdatePlayer() {}
                    /* 0x10 */ virtual void EndSoundFrame() {}
                };
                typedef ut::LinkList<Callback, offsetof(Callback, mLink)> CallbackList;

                static SoundThread& GetInstance();

                bool Create(s32 priority);
                BOOL Shutdown();

                OSMutex& GetSoundMutex() { return mMutex; }

            private:
                typedef enum ThreadMessage {
                    MSG_NONE = 0,
                    MSG_AX_CALLBACK,
                    MSG_SHUTDOWN,
                } ThreadMessage;

                static const int MSG_QUEUE_CAPACITY = 8;

                SoundThread() : mCreateFlag(false) {}

                ~SoundThread() {}

                void SoundThreadProc();

                static void AxCallback();
                static void* SoundThreadFunc(void* arg);

                void Lock() { OSLockMutex(&mMutex); }
                void Unlock() { OSUnlockMutex(&mMutex); }

            private:
                /* 0x0000 */ OSThread mThread;
                /* 0x0318 */ u64 mThreadStack[THREAD_STACK_SIZE];
                /* 0x2318 */ OSThreadQueue mThreadQueue;
                /* 0x2320 */ mutable OSMutex mMutex;
                /* 0x2338 */ OSMessageQueue mMsgQueue;
                /* 0x2358 */ OSMessage mMsgBuffer[MSG_QUEUE_CAPACITY];
                /* 0x2378 */ AxManager::CallbackListNode mAxCallbackNode;
                /* 0x2384 */ CallbackList mCallbackList;
                /* 0x2390 */ u32 mProcessTick;
                /* 0x2394 */ bool mCreateFlag;
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
