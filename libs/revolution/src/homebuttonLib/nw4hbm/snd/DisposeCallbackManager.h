#ifndef NW4HBM_SND_DISPOSE_CALLBACK_MANAGER_H
#define NW4HBM_SND_DISPOSE_CALLBACK_MANAGER_H

#include "DisposeCallback.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {
            class DisposeCallbackManager {
            public:
                static DisposeCallbackManager& GetInstance();

                void RegisterDisposeCallback(DisposeCallbackBase* callback);
                void UnregisterDisposeCallback(DisposeCallbackBase* callback);

                static void Dispose(void* mem, u32 size, void* arg);
                static void DisposeWave(void* mem, u32 size, void* arg);

            private:
                DisposeCallbackManager();

                /* 0x00 */ DisposeCallbackList mCallbackList;
            };
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
