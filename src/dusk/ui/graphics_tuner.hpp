#pragma once

#include "component.hpp"
#include "document.hpp"
#include "ui.hpp"

#include <algorithm>
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
    void refresh();
    bool handle_nav_command(NavCommand cmd);

private:
    void apply(int value);

    Props mProps;
    Rml::Element* mPrevElem = nullptr;
    Rml::Element* mNextElem = nullptr;
    Rml::Element* mValueElem = nullptr;
};

enum class GraphicsOption {
    InternalResolution,
    ShadowResolution,
    Resampler,
    BloomMode,
    BloomMultiplier,
    DepthOfFieldMode,
    TextureReplacements,
};

struct GraphicsSetting {
    int min = 0;
    int max = 0;
    int defaultValue = 0;
    int step = 1;
    bool watchesRenderSize = false;
    int (*read)() = nullptr;
    void (*write)(int) = nullptr;
    Rml::String (*label)(int) = nullptr;
    const char* (*cvarName)() = nullptr;
    bool (*isModified)() = nullptr;

    static const GraphicsSetting& of(GraphicsOption option);

    void set(int value) const { write(std::clamp(value, min, max)); }
    Rml::String text() const { return label(read()); }
};

struct GraphicsTunerProps {
    GraphicsOption option;
    Rml::String title;
    Rml::String helpText;
};

class GraphicsTuner : public Document {
public:
    explicit GraphicsTuner(GraphicsTunerProps props);
    ~GraphicsTuner() override;

    void show() override;
    void hide(bool close) override;
    void update() override;
    bool focus() override;
    bool visible() const override;

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

    void reset_default();

    GraphicsSetting mSetting;
    std::vector<std::unique_ptr<Component> > mComponents;
    SteppedCarousel* mCarousel = nullptr;
    Rml::Element* mRoot;
    u64 mSubscription = 0;
    u32 mLastRenderWidth = 0;
    u32 mLastRenderHeight = 0;
};

}  // namespace dusk::ui
