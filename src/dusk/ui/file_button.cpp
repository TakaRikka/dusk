#include "file_button.hpp"

#include <aurora/lib/window.hpp>
#include <borealis/io.hpp>

#include <utility>

namespace dusk::ui {

FileButton::FileButton(Rml::Element* parent, Props props)
    : BaseControlledSelectButton{parent, {.key = std::move(props.key)}},
      mGetValue{std::move(props.getValue)}, mSetValue{std::move(props.setValue)},
      mIsDisabled{std::move(props.isDisabled)}, mIsModified{std::move(props.isModified)},
      mFilters{std::move(props.filters)}, mDirectoryMode{props.directoryMode} {}

bool FileButton::modified() const {
    return mIsModified ? mIsModified() : BaseControlledSelectButton::modified();
}

bool FileButton::disabled() const {
    return borealis::file_select::busy() || (mIsDisabled && mIsDisabled());
}

Rml::String FileButton::format_value() {
    const Rml::String location = mGetValue ? mGetValue() : "";
    if (location.empty()) {
        return "(none)";
    }
    const auto name = borealis::io::display_name(location);
    return name.empty() ? location : name;
}

bool FileButton::handle_nav_command(NavCommand command) {
    if (command != NavCommand::Confirm) {
        return false;
    }
    open_picker();
    return true;
}

void FileButton::open_picker() {
    if (disabled()) {
        return;
    }
    const std::string defaultLocation = mGetValue ? mGetValue() : "";
    auto complete = [setValue = mSetValue](borealis::file_select::Result result) {
        if (result.status == borealis::file_select::Status::Selected && !result.locations.empty() &&
            setValue)
        {
            setValue(std::move(result.locations.front()));
        }
    };
    if (mDirectoryMode) {
        borealis::file_select::open_folder(
            {
                .parentWindow = aurora::window::get_sdl_window(),
                .defaultLocation = defaultLocation,
            },
            std::move(complete));
        return;
    }
    borealis::file_select::open_file(
        {
            .parentWindow = aurora::window::get_sdl_window(),
            .filters = mFilters,
            .defaultLocation = defaultLocation,
        },
        std::move(complete));
}

}  // namespace dusk::ui
