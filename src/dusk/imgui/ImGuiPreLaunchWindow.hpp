#pragma once

namespace dusk {
class ImGuiPreLaunchWindow {
private:
    int m_CurMenu = 0;
    bool m_IsFirstDraw = true;
    std::string m_initialGraphicsBackend;

    bool isSelectedPathValid() const;

public:
    ImGuiPreLaunchWindow();
    void draw();

    void drawMainMenu();
    void drawOptions();

    std::string m_selectedIsoPath;
    std::string m_errorString;
};
}  // namespace dusk
