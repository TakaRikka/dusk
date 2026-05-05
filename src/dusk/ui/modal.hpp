#pragma once

#include "button.hpp"
#include "document.hpp"

namespace dusk::ui {

struct ModalAction {
    Rml::String label;
    std::function<void()> onPressed;
};

class Modal : public Document {
public:
    struct Props {
        Rml::String title;
        Rml::String bodyRml;
        std::vector<ModalAction> actions;
        std::function<void()> onDismiss;
        bool doBlur = false;
    };

    explicit Modal(Props props);

    void show() override;
    void hide(bool close) override;
    bool visible() const override;
    bool focus() override;

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

private:
    void dismiss();

    Props mProps;
    Rml::Element* mRoot = nullptr;
    std::vector<std::unique_ptr<Button> > mButtons;
};

}  // namespace dusk::ui
