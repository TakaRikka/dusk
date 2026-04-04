#include "dusk/settings.hpp"
#include "dusk/config.hpp"

namespace dusk::settings::enhancements {
    ConfigVar<bool> FastIronBoots("enhancements.fast_iron_boots", false);
    ConfigVar<bool> InvertCameraXAxis("enhancements.invert_camera_x_axis", false);
    ConfigVar<bool> QuickTransform("enhancements.quick_transform", false);
    ConfigVar<bool> RestoreWiiGlitches("enhancements.restore_wii_glitches", false);
    ConfigVar<bool> EnableBloom("enhancements.enable_bloom", true);
    ConfigVar<bool> UseWaterProjectionOffset("enhancements.use_water_projection_offset", false);
    ConfigVar<bool> MirrorMode("enhancements.mirror_mode", false);

    void Register() {
        Register(FastIronBoots);
        Register(InvertCameraXAxis);
        Register(QuickTransform);
        Register(RestoreWiiGlitches);
        Register(EnableBloom);
        Register(UseWaterProjectionOffset);
        Register(MirrorMode);
    }
}

namespace dusk::settings {
    void Register() {
        enhancements::Register();
    }
}
