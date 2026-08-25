#ifndef DUSK_TVOS_NETWORK_INFO_HPP
#define DUSK_TVOS_NETWORK_INFO_HPP

#include <string>

namespace dusk::tvos {

// The device's LAN IPv4 address, for display on the TV so a phone can reach the transfer server.
// Empty when there is no usable route -- the caller shows a "connect to a network" state rather
// than an address nobody can open.
std::string lan_address();

}  // namespace dusk::tvos

#endif  // DUSK_TVOS_NETWORK_INFO_HPP
