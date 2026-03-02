#ifndef NW4HBM_INSTANCEMANAGER_H
#define NW4HBM_INSTANCEMANAGER_H

#include "../ut/LinkList.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {
            template <typename T, int Ofs>
            class InstanceManager {
            public:
                typedef typename ut::LinkList<T, Ofs>::Iterator Iterator;
                void Append(T* obj) {
                    NW4HBM_ASSERT_CHECK_NULL(67, obj);
                    mFreeList.PushBack(obj);
                }

                void Remove(T* obj) {
                    NW4HBM_ASSERT_CHECK_NULL(84, obj);
                    mFreeList.Erase(obj);
                }

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
                    NW4HBM_ASSERT_CHECK_NULL(119, obj);

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
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
