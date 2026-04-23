#ifndef DUSK_VERSION_HPP
#define DUSK_VERSION_HPP

namespace dusk::version {
    enum class GameVersion : u8 {
        GcnUsa,
        GcnPal,
        GcnJpn,
        WiiUsaRev0,
        WiiUsa,
        WiiPal,
        WiiJpn,
        WiiKor,
    };

    bool isGcn();
    bool isWii();

    bool isRegionPal();
    bool isRegionJpn();

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
