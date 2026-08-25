#include "dusk/tvos/network_info.hpp"

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstring>

namespace dusk::tvos {

std::string lan_address() {
    ifaddrs* interfaces = nullptr;
    if (::getifaddrs(&interfaces) != 0 || interfaces == nullptr) {
        return {};
    }

    std::string found;
    for (ifaddrs* it = interfaces; it != nullptr; it = it->ifa_next) {
        if (it->ifa_addr == nullptr || it->ifa_addr->sa_family != AF_INET) {
            continue;
        }
        if ((it->ifa_flags & IFF_UP) == 0 || (it->ifa_flags & IFF_LOOPBACK) != 0) {
            continue;
        }
        // Wi-Fi and Ethernet are en* on Apple platforms; skip tunnels, bridges and awdl.
        if (it->ifa_name == nullptr || std::strncmp(it->ifa_name, "en", 2) != 0) {
            continue;
        }
        char text[INET_ADDRSTRLEN] = {};
        auto* addr = reinterpret_cast<sockaddr_in*>(it->ifa_addr);
        if (::inet_ntop(AF_INET, &addr->sin_addr, text, sizeof(text)) != nullptr) {
            found = text;
            break;
        }
    }

    ::freeifaddrs(interfaces);
    return found;
}

}  // namespace dusk::tvos
