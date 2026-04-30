#pragma once

#include "button.hpp"
#include "component.hpp"
#include "select_button.hpp"

namespace dusk::ui {

using TabCallback = std::function<void()>;

struct Tab {
    Rml::String title;
    Button& button;
    TabCallback callback;
};

class TabBar : public Component {
public:
    struct Props {
        std::function<void()> onClose;
        int selectedTabIndex = -1;
        bool autoSelect = true;
    };

    explicit TabBar(Rml::Element* parent, Props props);

    bool focus() override;

    void add_tab(const Rml::String& title, TabCallback callback);
    bool set_active_tab(int index);
    bool focus_tab(int index);
    bool handle_nav_command(Rml::Event& event, NavCommand cmd);

private:
    int tab_containing(Rml::Element* element) const;

    Props mProps;
    std::vector<Tab> mTabs;
};

}  // namespace dusk::ui
