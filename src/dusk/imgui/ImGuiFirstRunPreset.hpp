#pragma once

namespace dusk {

class ImGuiFirstRunPreset {
public:
    void draw();

private:
    bool m_opened = false;
    bool m_done = false;
};

}  // namespace dusk
