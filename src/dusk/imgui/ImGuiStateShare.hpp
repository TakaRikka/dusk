#ifndef DUSK_IMGUI_STATESHARE_HPP
#define DUSK_IMGUI_STATESHARE_HPP

#include "d/d_save.h"
#include <optional>
#include <string>
#include <vector>

namespace dusk {

struct SavedStateEntry {
    std::string name;
    std::string encoded;
};

class ImGuiStateShare {
public:
    void draw(bool& open);

private:
    std::string encodeCurrentState();
    bool applyEncodedState(const std::string& encoded, const std::string& name = {});
    void tickPendingApply();
    void loadStatesFile();
    void saveStatesFile();

    std::vector<SavedStateEntry> m_states;
    std::string m_statusMsg;
    std::optional<dSv_info_c> m_pendingInfo;
    int m_renamingIndex = -1;
    char m_renameBuffer[128] = {};
    bool m_loaded = false;
};

}

#endif
