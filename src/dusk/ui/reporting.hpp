#pragma once

#if DUSK_ENABLE_SENTRY_NATIVE

#include "component.hpp"
#include "window.hpp"

#include <memory>
#include <vector>

namespace dusk::ui {

class CrashReportWindow : public WindowSmall {
public:
    CrashReportWindow();

    bool focus() override;

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

private:
    std::vector<std::unique_ptr<Component>> mButtons;
};

}  // namespace dusk::ui

#endif
