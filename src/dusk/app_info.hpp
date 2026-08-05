#pragma once

#include <borealis/app_info.hpp>

namespace dusk {
    /** Application identity fields for Borealis modules */
    inline constexpr borealis::AppInfo AppInfo{
        .orgName = "TwilitRealm",
        .appName = "Dusklight",
        .githubOwner = "TwilitRealm",
        .githubRepo = "dusklight",
        .discordApplicationId = "1495632471994405035",
    };

    /**
     * \brief The internal application name for the game.
     *
     * This gets used for file paths and such, and cannot be changed!
     */
    constexpr auto AppName = "Dusklight";

}
