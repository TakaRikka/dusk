#pragma once

#include "select_button.hpp"

#include <borealis/file_select.hpp>

#include <functional>
#include <vector>

namespace dusk::ui {

class FileButton : public BaseControlledSelectButton {
public:
    struct Props {
        Rml::String key;
        std::function<Rml::String()> getValue;
        std::function<void(Rml::String)> setValue;
        std::function<bool()> isDisabled;
        std::function<bool()> isModified;
        std::vector<borealis::file_select::Filter> filters;
        bool directoryMode = false;
    };

    FileButton(Rml::Element* parent, Props props);
    bool modified() const override;
    bool disabled() const override;

protected:
    Rml::String format_value() override;
    bool handle_nav_command(NavCommand command) override;

private:
    void open_picker();

    std::function<Rml::String()> mGetValue;
    std::function<void(Rml::String)> mSetValue;
    std::function<bool()> mIsDisabled;
    std::function<bool()> mIsModified;
    std::vector<borealis::file_select::Filter> mFilters;
    bool mDirectoryMode = false;
};

}  // namespace dusk::ui
