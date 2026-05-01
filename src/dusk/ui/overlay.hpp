#pragma once

#include "button.hpp"
#include "component.hpp"
#include "document.hpp"
#include "ui.hpp"

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace dusk::ui {

class SteppedCarousel : public Component {
public:
    struct Props {
        int min = 0;
        int max = 0;
        int step = 1;
        std::function<int()> getValue;
        std::function<void(int)> onChange;
        std::function<Rml::String(int)> formatValue;
    };

    SteppedCarousel(Rml::Element* parent, Props props);

    bool focus() override;
    void update() override;

private:
    bool handle_nav_command(NavCommand cmd);
    void apply(int value);

    Props mProps;
    Rml::Element* mValueElem = nullptr;
};

enum class GraphicsOption {
    InternalResolution,
    ShadowResolution,
};

Rml::String format_graphics_setting_value(GraphicsOption option, int value);

struct OverlayProps {
    GraphicsOption option;
    Rml::String title;
    Rml::String helpText;
    int valueMin = 0;
    int valueMax = 0;
};

class Overlay : public Document {
public:
    explicit Overlay(OverlayProps props);

    void update() override;
    bool focus() override;

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

private:
    template <typename T, typename... Args>
    requires std::is_base_of_v<Component, T> T& add_component(Args&&... args) {
        auto child = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *child;
        mComponents.emplace_back(std::move(child));
        return ref;
    }

    void dismiss();
    void reset_default();

    GraphicsOption mOption;
    int mValueMin = 0;
    int mValueMax = 0;
    std::vector<std::unique_ptr<Component> > mComponents;
    bool mDismissed = false;
};

}  // namespace dusk::ui
