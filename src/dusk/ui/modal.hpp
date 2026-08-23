#pragma once

#include "button.hpp"
#include "pane.hpp"
#include "window.hpp"

namespace dusk::ui {
class Modal;

struct ModalAction {
    Rml::String label;
    std::function<void(Modal&)> onPressed;
    std::function<bool()> isDisabled;
};

class Modal : public WindowSmall {
public:
    struct Props {
        Rml::String title;
        Rml::String bodyRml;
        std::vector<ModalAction> actions;
        std::function<void(Modal&)> onDismiss;
        Rml::String variant;
        Rml::String icon = "";
        bool isVertical = false;
    };

    explicit Modal(Props props);

    void update() override;
    bool focus() override;

    Pane& content_pane();
    void set_body(const Rml::String& bodyRml);
    void set_icon(const Rml::String& icon);

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

private:
    void add_action(ModalAction action);
    void dismiss();

    Props mProps;
    Rml::Element* mContentRoot = nullptr;
    std::unique_ptr<Pane> mContentPane;
    std::function<void(Modal&)> mPendingAction;
    std::vector<std::unique_ptr<Button>> mButtons;
};

}  // namespace dusk::ui
