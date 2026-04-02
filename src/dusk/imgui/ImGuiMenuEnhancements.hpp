#ifndef DUSK_IMGUI_MENUENHANCEMENTS_HPP
#define DUSK_IMGUI_MENUENHANCEMENTS_HPP

#include <aurora/aurora.h>
#include <pad.h>
#include <string>

#include "imgui.h"

namespace dusk {
    struct EnhancementsSettings {
        bool fastIronBoots;
        bool invertCameraXAxis;
        bool quickTransform;
        bool restoreWiiGlitches;
        bool enableBloom;
        bool useWaterProjectionOffset;
    };

    class ImGuiMenuEnhancements {
    public:
        ImGuiMenuEnhancements();
        void draw();

        static EnhancementsSettings m_enhancements;
    };
}

#endif  // DUSK_IMGUI_MENUENHANCEMENTS_HPP
