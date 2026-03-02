#ifndef NW4HBM_SND_SOUND_INSTANCE_MANAGER_H
#define NW4HBM_SND_SOUND_INSTANCE_MANAGER_H

#include <new>

#include <revolution/types.h>

#include "InstancePool.h"

#include "../ut/Lock.h"
#include "../ut/inlines.h"

#include <revolution/os.h>

#include "../db/assert.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {

            template <typename T>
            class SoundInstanceManager {
            public:
                u32 Create(void* buffer, u32 size) {
                    NW4HBM_ASSERT_CHECK_NULL(59, buffer);
                    return mPool.Create(buffer, size);
                }

                void Destroy(void* buffer, u32 size) {
                    NW4HBM_ASSERT_CHECK_NULL(76, buffer);
                    mPool.Destroy(buffer, size);
                }

                T* Alloc(int priority) {
                    NW4R_ASSERT_MINMAXLT(92, priority, 0, 127);
                    ut::AutoInterruptLock lock;
                    T* sound;

                    void* ptr = mPool.Alloc();

                    if (ptr != NULL) {
                        sound = new (ptr) T(this);
                    } else {
                        if (mPriorityList.IsEmpty()) {
                            return NULL;
                        }
                        sound = &mPriorityList.GetFront();

                        if (sound == NULL) {
                            return NULL;
                        }

                        if (priority < sound->CalcCurrentPlayerPriority()) {
                            return NULL;
                        }
                        sound->Stop();

                        ptr = mPool.Alloc();
                        NW4HBM_ASSERT_CHECK_NULL(114, ptr);
                        sound = new (ptr) T(this);
                    }

                    InsertPriorityList(sound, priority);
                    return sound;
                }

                void Free(T* sound) {
                    NW4HBM_ASSERT_CHECK_NULL(134, sound);
                    ut::AutoInterruptLock lock;

                    if (mPriorityList.IsEmpty()) {
                        return;
                    }

                    RemovePriorityList(sound);
                    sound->~T();
                    mPool.Free(sound);
                }

                u32 GetActiveCount() const { return mPriorityList.GetSize(); }

                u32 GetFreeCount() const { return mPool.Count(); }

                T* GetLowestPrioritySound() {
                    if (mPriorityList.IsEmpty()) {
                        return NULL;
                    }

                    return static_cast<T*>(&mPriorityList.GetFront());
                }

                void InsertPriorityList(T* sound, int priority) {
                    TPrioList::Iterator it = mPriorityList.GetBeginIter();

                    for (; it != mPriorityList.GetEndIter(); it++) {
                        if (priority < it->CalcCurrentPlayerPriority()) {
                            break;
                        }
                    }

                    mPriorityList.Insert(it, sound);
                }

                void RemovePriorityList(T* sound) { mPriorityList.Erase(sound); }

                void SortPriorityList() {
                    TPrioList listsByPrio[T::PRIORITY_MAX + 1];

                    while (!mPriorityList.IsEmpty()) {
                        T& rSound = mPriorityList.GetFront();
                        mPriorityList.PopFront();
                        listsByPrio[rSound.CalcCurrentPlayerPriority()].PushBack(&rSound);
                    }

                    for (int i = 0; i < T::PRIORITY_MAX + 1; i++) {
                        while (!listsByPrio[i].IsEmpty()) {
                            T& rSound = listsByPrio[i].GetFront();
                            listsByPrio[i].PopFront();
                            mPriorityList.PushBack(&rSound);
                        }
                    }
                }

                void UpdatePriority(T* sound, int priority) {
                    RemovePriorityList(sound);
                    InsertPriorityList(sound, priority);
                }

            private:
                typedef ut::LinkList<T, offsetof(T, mPriorityLink)> TPrioList;

            private:
                /* 0x00 */ MemoryPool<T> mPool;
                /* 0x04 */ TPrioList mPriorityList;
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
