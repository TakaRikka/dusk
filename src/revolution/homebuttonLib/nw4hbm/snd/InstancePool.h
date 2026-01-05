#ifndef NW4HBM_SND_INSTANCE_POOL_H
#define NW4HBM_SND_INSTANCE_POOL_H

#include <new>
#include <revolution/types.h>

namespace nw4hbm {
    namespace snd {
        namespace detail {
            class PoolImpl {
            public:
                PoolImpl() : mNext(NULL) {}

            protected:
                u32 CreateImpl(void* buffer, u32 size, u32 objSize);
                void DestroyImpl(void* buffer, u32 size);
                int CountImpl() const;

                void* AllocImpl();
                void FreeImpl(void* ptr);

            private:
                /* 0x00 */ PoolImpl* mNext;
            };

            template <typename T>
            class InstancePool : private PoolImpl {
            public:
                u32 Create(void* buffer, u32 size) { return CreateImpl(buffer, size, sizeof(T)); }

                void Destroy(void* buffer, u32 size) { DestroyImpl(buffer, size); }

                int Count() const { return CountImpl(); }

                T* Alloc() {
                    void* ptr = AllocImpl();
                    if (ptr == NULL) {
                        return NULL;
                    }

                    return new (ptr) T;
                }

                void Free(T* obj) {
                    if (obj != NULL) {
                        obj->~T();
                        FreeImpl(obj);
                    }
                }
            };

            template <typename T>
            class MemoryPool : private PoolImpl {
            public:
                u32 Create(void* buffer, u32 size) { return CreateImpl(buffer, size, sizeof(T)); }

                void Destroy(void* buffer, u32 size) { DestroyImpl(buffer, size); }

                int Count() const { return CountImpl(); }

                T* Alloc() { return static_cast<T*>(AllocImpl()); }

                void Free(T* obj) { FreeImpl(obj); }
            };
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
