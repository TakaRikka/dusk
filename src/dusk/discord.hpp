#pragma once

#ifdef DUSK_DISCORD

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>

namespace dusk::discord::rpc {

struct User {
    std::string id;
    std::string username;
    std::string discriminator;
    std::string avatar;
};

struct Presence {
    std::string state;
    std::string details;
    int64_t startTimestamp = 0;
    std::string largeImageKey;
    std::string largeImageText;
};

struct EventHandlers {
    std::function<void(const User&)> ready;
    std::function<void(int, std::string_view)> disconnected;
    std::function<void(int, std::string_view)> error;
};

void initialize(std::string applicationId, EventHandlers handlers);
void run_callbacks();
void update_presence(Presence presence);
void clear_presence();
void shutdown();

}  // namespace dusk::discord::rpc

#endif  // DUSK_DISCORD
