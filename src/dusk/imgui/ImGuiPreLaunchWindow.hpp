#pragma once

#include <thread>
#include "dusk/iso_validate.hpp"

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
    void drawDiscVerification();

    std::string m_selectedIsoPath;
    std::string m_errorString;
    bool m_isPal = false;
    struct {
        std::thread task;
        iso::VerificationStatus status;
        iso::ValidationError error;
        bool isRunning;
        bool shouldOpenReport;
    } m_discVerification;
};
}  // namespace dusk
