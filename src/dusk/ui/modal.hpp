#pragma once

#include "button.hpp"
#include "window.hpp"

namespace dusk::ui {
class Modal;

struct ModalAction {
    Rml::String label;
    std::function<void(Modal&)> onPressed;
};

class Modal : public WindowSmall {
public:
    struct Props {
        Rml::String title;
        Rml::String bodyRml;
        std::vector<ModalAction> actions;
        std::function<void(Modal&)> onDismiss;
        Rml::String variant;
        bool isWarning = false;
        bool isError = false;
    };

    explicit Modal(Props props);

    bool focus() override;

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

private:
    void dismiss();

    Props mProps;
    std::vector<std::unique_ptr<Button> > mButtons;
};

}  // namespace dusk::ui
