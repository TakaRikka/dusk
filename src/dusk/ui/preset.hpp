#pragma once
#include "component.hpp"
#include "document.hpp"
#include <memory>
#include <vector>

namespace dusk::ui {

class PresetWindow : public Document {
public:
    PresetWindow();

    void show() override;
    void hide(bool close) override;
    bool visible() const override;
    bool focus() override;

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

private:
    Rml::Element* mRoot = nullptr;
    std::vector<std::unique_ptr<Component>> mButtons;
};

}  // namespace dusk::ui
