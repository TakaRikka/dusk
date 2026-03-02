#ifndef NW4HBM_SND_CHANNEL_MANAGER_H
#define NW4HBM_SND_CHANNEL_MANAGER_H

#include "Channel.h"
#include "InstanceManager.h"
#include "snd_types.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {
            class ChannelManager : public InstanceManager<Channel, 0> {
            public:
                static ChannelManager& GetInstance() {
                    static ChannelManager instance;
                    return instance;
                }
                ChannelManager() {}
            };
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
