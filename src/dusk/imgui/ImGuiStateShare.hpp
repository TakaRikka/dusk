#ifndef DUSK_IMGUI_STATESHARE_HPP
#define DUSK_IMGUI_STATESHARE_HPP

#include "d/d_save.h"
#include <optional>
#include <string>

namespace dusk {
    class ImGuiStateShare {
    public:
        void draw(bool& open);

    private:
        void copyState();
        bool pasteState();
        void tickPendingApply();

        std::string m_statusMsg;
        std::optional<dSv_info_c> m_pendingInfo;
    };
}

#endif
