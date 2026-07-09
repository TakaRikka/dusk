#pragma once

#include "pane.hpp"
#include "window.hpp"

#include <climits>

namespace dusk::ui {

struct ModControlSpec {
    enum class Kind : u8 {
        Button,
        Toggle,
        Number,
        String,
        Select,
    };

    Kind kind = Kind::Button;
    Rml::String label;
    Rml::String helpRml;
    std::function<void()> onPressed;
    std::function<bool()> getBool;
    std::function<void(bool)> setBool;
    // Number value, or the selected option index for Select
    std::function<int()> getInt;
    std::function<void(int)> setInt;
    std::function<Rml::String()> getString;
    std::function<void(Rml::String)> setString;
    std::function<bool()> isDisabled;
    std::function<bool()> isModified;
    int min = 0;
    int max = INT_MAX;
    int step = 1;
    Rml::String prefix;
    Rml::String suffix;
    std::vector<Rml::String> options;
    int maxLength = -1;
};

Component* build_mod_control(Pane& pane, Pane* helpPane, ModControlSpec spec);

// A mod-owned tabbed two-pane window.
class ModWindow : public Window {
public:
    struct Tab {
        Rml::String title;
        std::function<void(ModWindow&, Pane& left, Pane& right)> build;
        std::function<void()> update;
    };
    struct Desc {
        Rml::String modId;
        std::vector<Tab> tabs;
        Rml::String rcss;
        std::function<void()> onDestroyed;
    };

    explicit ModWindow(Desc desc);
    ~ModWindow() override;

    void update() override;
    void force_close() { Document::hide(true); }

private:
    Desc mDesc;
    int mActiveTab = -1;
};

}  // namespace dusk::ui
