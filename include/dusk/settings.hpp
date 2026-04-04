#ifndef DUSK_SETTINGS_HPP
#define DUSK_SETTINGS_HPP

#include "config_var.hpp"

namespace dusk::settings {
    using namespace dusk::config;

    namespace enhancements {
        extern ConfigVar<bool> FastIronBoots;
        extern ConfigVar<bool> InvertCameraXAxis;
        extern ConfigVar<bool> QuickTransform;
        extern ConfigVar<bool> RestoreWiiGlitches;
        extern ConfigVar<bool> EnableBloom;
        extern ConfigVar<bool> UseWaterProjectionOffset;
        extern ConfigVar<bool> MirrorMode;

        void Register();
    }

    void Register();
}

#endif  // DUSK_SETTINGS_HPP
