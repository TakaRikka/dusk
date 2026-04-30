#ifndef DUSK_VERSION_HPP
#define DUSK_VERSION_HPP

/**
 * Functionality for switching game behavior based on the loaded game version (e.g. PAL/JPN, GC/Wii)
 */
namespace dusk::version {
    enum class GameVersion : u8 {
        GcnUsa = VERSION_GCN_USA,
        GcnPal = VERSION_GCN_PAL,
        GcnJpn = VERSION_GCN_JPN,
        WiiUsaRev0 = VERSION_WII_USA_R0,
        WiiUsa = VERSION_WII_USA_R2,
        WiiPal = VERSION_WII_PAL,
        WiiJpn = VERSION_WII_JPN,
        WiiKor = VERSION_WII_KOR,
    };

    bool isGcn();
    bool isWii();
    bool isPalOrAtLeastWiiR2();

    bool isRegionPal();
    bool isRegionJpn();
    bool isRegionUsa();

    GameVersion getGameVersion();

    const DVDDiskID& getDiskID();

    void init();

    template<typename T>
    struct VersionOption {
        GameVersion mVersion;
        T mValue;

        constexpr VersionOption(GameVersion version, T value) : mVersion(version), mValue(value) {}
    };

    template<typename T>
    const T& versionSelect(const std::initializer_list<VersionOption<T>> options) {
        const auto version = getGameVersion();
        for (const auto& opt : options) {
            if (opt.mVersion == version) {
                return opt.mValue;
            }
        }

        // Unable to find value.
        abort();
    }

    template<typename T>
    const T& versionSelect(const std::initializer_list<VersionOption<T>> options, const T& defaultValue) {
        const auto version = getGameVersion();
        for (const auto& opt : options) {
            if (opt.mVersion == version) {
                return opt.mValue;
            }
        }

        return defaultValue;
    }
}  // namespace dusk::version

#endif  // DUSK_VERSION_HPP
