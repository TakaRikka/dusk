#pragma once

#include "dusk/app_info.hpp"
#include "dusk/mods/loader/loader.hpp"
#include "mods/svc/http.h"
#include "mods/svc/net.h"
#include "mods/svc/websocket.h"

#include <borealis/version.h>
#include <fmt/format.h>

#include <algorithm>
#include <ranges>
#include <span>
#include <string>
#include <string_view>

namespace dusk::mods::svc {

inline bool ascii_iequals(std::string_view left, std::string_view right) {
    if (left.size() != right.size()) {
        return false;
    }
    const auto ascii_lower = [](char value) {
        return value >= 'A' && value <= 'Z' ? static_cast<char>(value - 'A' + 'a') : value;
    };
    return std::ranges::equal(
        left, right, [&](char a, char b) { return ascii_lower(a) == ascii_lower(b); });
}

inline bool valid_header_name(std::string_view name) {
    constexpr std::string_view Separators{"()<>@,;:\\\"/[]?={} \t"};
    return !name.empty() && std::ranges::all_of(name, [&](unsigned char value) {
        return value > 32 && value < 127 &&
               Separators.find(static_cast<char>(value)) == std::string_view::npos;
    });
}

inline bool valid_header_value(std::string_view value, bool allowHorizontalTab) {
    return std::ranges::all_of(value, [=](unsigned char character) {
        return character >= 32 || (allowHorizontalTab && character == '\t');
    }) && std::ranges::none_of(value, [](unsigned char character) { return character == 127; });
}

inline bool is_reserved_header(std::string_view name, std::span<const std::string_view> reserved) {
    return std::ranges::any_of(
        reserved, [&](std::string_view value) { return ascii_iequals(name, value); });
}

inline bool valid_header(std::string_view name, std::string_view value,
    std::span<const std::string_view> reserved, bool allowHorizontalTab = false) {
    return valid_header_name(name) && valid_header_value(value, allowHorizontalTab) &&
           !is_reserved_header(name, reserved);
}

inline bool declares_import(const LoadedMod& mod, std::string_view serviceId) {
    return std::ranges::any_of(
        mod.manifestInfo.imports, [&](const ModManifestInfo::Import& serviceImport) {
            return serviceImport.id == serviceId;
        });
}

inline bool is_network_service(std::string_view serviceId) {
    return serviceId == HTTP_SERVICE_ID || serviceId == NET_SERVICE_ID ||
           serviceId == WEBSOCKET_SERVICE_ID;
}

inline std::string user_agent(const LoadedMod& mod) {
    std::string version{mod.metadata.version};
    for (char& character : version) {
        const auto value = static_cast<unsigned char>(character);
        if (value <= 32 || value >= 127) {
            character = '_';
        }
    }
    return fmt::format("{}/{} {}/{}", AppName, BOREALIS_APP_VERSION, mod.metadata.id, version);
}

}  // namespace dusk::mods::svc::network
