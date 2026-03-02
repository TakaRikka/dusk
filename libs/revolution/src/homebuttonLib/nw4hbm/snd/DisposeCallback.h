#ifndef NW4HBM_SND_DISPOSE_CALLBACK_H
#define NW4HBM_SND_DISPOSE_CALLBACK_H

#include "../ut/LinkList.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {

            // we want the vtable to get a unique name but function parameters need not to be unique
            // so we use a dummy class for that and cast to `DisposeCallback` when needed
            class DisposeCallbackBase {};

            namespace {

                class DisposeCallback : public DisposeCallbackBase {
                public:
                    /* 0x00 */ ut::LinkListNode mDisposeLink;

                    /* 0x08 */ virtual ~DisposeCallback()
#ifdef MAKE_DTOR_ZERO
                        = 0
#endif
                    {};

                    /* 0x0C */ virtual void InvalidateData(const void* start, const void* end) = 0;
                    /* 0x10 */ virtual void InvalidateWaveData(const void* start,
                                                               const void* end) = 0;
                };

            }  // namespace

            typedef ut::LinkList<DisposeCallback, offsetof(DisposeCallback, mDisposeLink)>
                DisposeCallbackList;

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
