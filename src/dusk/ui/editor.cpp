#include "editor.hpp"

#include <RmlUi/Core.h>

#include "fmt/format.h"

namespace dusk::ui {
namespace {

const Rml::String kLocationContent = R"RML(
<div class="pane">
    <div class="section-heading">Save Location</div>
    <button class="select-button">
        <div class="key">Stage</div>
        <div class="value">F_SP108</div>
    </button>
    <button class="select-button">
        <div class="key">Room</div>
        <div class="value">1</div>
    </button>
    <button class="select-button">
        <div class="key">Spawn ID</div>
        <div class="value">0</div>
    </button>
    <div class="section-heading">Horse Location</div>
    <button class="select-button">
        <div class="key">Position</div>
        <div class="value">34814, -260, -41181</div>
    </button>
</div>
<div class="pane"></div>
)RML";

bool has_save_data() {
    return dComIfGs_getSaveData() != nullptr;
}

dSv_player_status_a_c* get_player_status() {
    if (!has_save_data()) {
        return nullptr;
    }
    return &dComIfGs_getSaveData()->getPlayer().getPlayerStatusA();
}

Rml::String get_player_name() {
    if (!has_save_data()) {
        return nullptr;
    }
    return dComIfGs_getPlayerName();
}

Rml::String get_horse_name() {
    if (!has_save_data()) {
        return nullptr;
    }
    return dComIfGs_getHorseName();
}

Rml::String value_for_player_selection(const Rml::String& selection) {
    dSv_player_status_a_c* status = get_player_status();
    if (selection == "get_horse_name") {
        return get_horse_name();
    }
    if (status == nullptr) {
        return "?";
    }
    if (selection == "max_health") {
        return fmt::format("{}", static_cast<u16>(status->mMaxLife));
    }
    if (selection == "health") {
        return fmt::format("{}", static_cast<u16>(status->mLife));
    }
    if (selection == "max_oil") {
        return fmt::format("{}", static_cast<u16>(status->mMaxOil));
    }
    if (selection == "oil") {
        return fmt::format("{}", static_cast<u16>(status->mOil));
    }
    return "Unknown";
}

Rml::String make_select_row(std::string_view key, std::string_view label, const Rml::String& value, const Rml::String& activeSelection) {
    const char* selectedClass = key == activeSelection ? " selected" : "";
    return fmt::format(
        "<button class=\"select-button{0}\" data-event-click=\"set_active_selection('{1}')\">"
        "<div class=\"key\">{2}</div><div class=\"value\">{3}</div></button>",
        selectedClass,
        key,
        label,
        value
    );
}

Rml::String make_numeric_detail(std::string_view label, std::string_view decAction, std::string_view incAction) {
    return fmt::format(
        "<div class=\"pane detail-pane\">"
        "<div class=\"section-heading\">{0}</div>"
        "<div class=\"detail-controls\">"
        "<button class=\"button\" data-event-click=\"window_action('{1}')\">-1</button>"
        "<button class=\"button\" data-event-click=\"window_action('{2}')\">+1</button>"
        "</div>"
        "</div>",
        label,
        decAction,
        incAction
    );
}

template <typename TValue>
void adjust_u16(TValue& value, int delta, u16 minValue, u16 maxValue) {
    const int nextValue = std::clamp(static_cast<int>(value) + delta, static_cast<int>(minValue), static_cast<int>(maxValue));
    value = static_cast<u16>(nextValue);
}

void render_player_status_tab(Rml::Element* content, const Rml::String& activeSelection) {
    Rml::String leftPane = R"RML(<div class="pane"><div class="section-heading">Player</div>)RML";
    leftPane += make_select_row("player_name", "Player Name", get_player_name(), activeSelection);
    leftPane += make_select_row("horse_name", "Horse Name", get_horse_name(), activeSelection);
    leftPane += make_select_row("max_health", "Max Health", value_for_player_selection("max_health"), activeSelection);
    leftPane += make_select_row("health", "Health", value_for_player_selection("health"), activeSelection);
    leftPane += make_select_row("max_oil", "Max Oil", value_for_player_selection("max_oil"), activeSelection);
    leftPane += make_select_row("oil", "Oil", value_for_player_selection("oil"), activeSelection);
    leftPane += "</div>";

    Rml::String rightPane;
    if (activeSelection == "max_health") {
        rightPane = make_numeric_detail("Max Health", "max_health.dec", "max_health.inc");
    } else if (activeSelection == "health") {
        rightPane = make_numeric_detail("Health", "health.dec", "health.inc");
    } else if (activeSelection == "max_oil") {
        rightPane = make_numeric_detail("Max Oil", "max_oil.dec", "max_oil.inc");
    } else if (activeSelection == "oil") {
        rightPane = make_numeric_detail("Oil", "oil.dec", "oil.inc");
    }

    Rml::Factory::InstanceElementText(content, leftPane + rightPane);
}

bool handle_editor_action(const Rml::VariantList& arguments) {
    if (arguments.empty() || !has_save_data()) {
        return true;
    }

    const Rml::String action = arguments[0].Get<Rml::String>();
    dSv_player_status_a_c* status = get_player_status();
    if (status == nullptr) {
        return true;
    }

    if (action == "max_health.inc") {
        adjust_u16(status->mMaxLife, 1, 0, 0xFFFF);
        return true;
    } else if (action == "max_health.dec") {
        adjust_u16(status->mMaxLife, -1, 0, 0xFFFF);
        if (status->mLife > status->mMaxLife) {
            status->mLife = status->mMaxLife;
        }
        return true;
    }

    if (action == "health.inc") {
        adjust_u16(status->mLife, 1, 0, status->mMaxLife);
        return true;
    } else if (action == "health.dec") {
        adjust_u16(status->mLife, -1, 0, status->mMaxLife);
        return true;
    }

    if (action == "max_oil.inc") {
        adjust_u16(status->mMaxOil, 1, 0, 0xFFFF);
        return true;
    } else if (action == "max_oil.dec") {
        adjust_u16(status->mMaxOil, -1, 0, 0xFFFF);
        if (status->mOil > status->mMaxOil) {
            status->mOil = status->mMaxOil;
        }
        return true;
    }

    if (action == "oil.inc") {
        adjust_u16(status->mOil, 1, 0, status->mMaxOil);
        return true;
    } else if (action == "oil.dec") {
        adjust_u16(status->mOil, -1, 0, status->mMaxOil);
        return true;
    }

    return false;
}

}  // namespace

EditorWindow::EditorWindow()
    : Window({.tabs = {
        {"Player Status",
            "player_name",
            [](Rml::Element* content, const Rml::String& activeSelection) { render_player_status_tab(content, activeSelection);
        }},
        {"Location",
            "",
            [](Rml::Element* content, const Rml::String&) { Rml::Factory::InstanceElementText(content, kLocationContent);
        }},
        {"Inventory"},
    },
    .actionHandler = handle_editor_action
}){}

}  // namespace dusk::ui
