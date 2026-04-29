#include "editor.hpp"

#include <RmlUi/Core.h>

#include "fmt/format.h"

#include "aurora/lib/dolphin/gd/gd.hpp"
#include "button.hpp"
#include "pane.hpp"
#include "select_button.hpp"

namespace dusk::ui {
namespace {
aurora::Module Log{"dusk::ui::editor"};

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
        return "Link";
    }
    return dComIfGs_getPlayerName();
}

Rml::String get_horse_name() {
    if (!has_save_data()) {
        return "Epona";
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

Rml::String make_select_row(std::string_view key, std::string_view label, const Rml::String& value,
    const Rml::String& activeSelection) {
    const char* selectedClass = key == activeSelection ? " selected" : "";
    return fmt::format(
        "<button class=\"select-button{0}\" data-event-click=\"set_active_selection('{1}')\">"
        "<div class=\"key\">{2}</div><div class=\"value\">{3}</div></button>",
        selectedClass, key, label, value);
}

Rml::String make_numeric_detail(
    std::string_view label, std::string_view decAction, std::string_view incAction) {
    return fmt::format(
        "<div class=\"pane detail-pane\">"
        "<div class=\"section-heading\">{0}</div>"
        "<div class=\"detail-controls\">"
        "<button class=\"button\" data-event-click=\"window_action('{1}')\">-1</button>"
        "<button class=\"button\" data-event-click=\"window_action('{2}')\">+1</button>"
        "</div>"
        "</div>",
        label, decAction, incAction);
}

template <typename TValue>
void adjust_u16(TValue& value, int delta, u16 minValue, u16 maxValue) {
    const int nextValue = std::clamp(
        static_cast<int>(value) + delta, static_cast<int>(minValue), static_cast<int>(maxValue));
    value = static_cast<u16>(nextValue);
}

void render_player_status_tab(Rml::Element* content, const Rml::String& activeSelection) {
    Rml::String leftPane = R"RML(<div class="pane"><div class="section-heading">Player</div>)RML";
    leftPane += make_select_row("player_name", "Player Name", get_player_name(), activeSelection);
    leftPane += make_select_row("horse_name", "Horse Name", get_horse_name(), activeSelection);
    leftPane += make_select_row(
        "max_health", "Max Health", value_for_player_selection("max_health"), activeSelection);
    leftPane +=
        make_select_row("health", "Health", value_for_player_selection("health"), activeSelection);
    leftPane += make_select_row(
        "max_oil", "Max Oil", value_for_player_selection("max_oil"), activeSelection);
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

EditorWindow::EditorWindow() {
    add_tab("Player Status", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Direction::Vertical);
        leftPane.add_section("Player");
        leftPane.add_select_button({
            .key = "Player Name",
            .getValue = get_player_name,
        });
        leftPane.add_select_button({
            .key = "Horse Name",
            .getValue = get_horse_name,
        });
        leftPane.add_select_button({
            .key = "Max Health",
            .getValue = [] { return value_for_player_selection("max_health"); },
        });
        leftPane.add_select_button({
            .key = "Max Oil",
            .getValue = [] { return value_for_player_selection("max_oil"); },
        });
        leftPane.add_select_button({
            .key = "Oil",
            .getValue = [] { return value_for_player_selection("oil"); },
        });
        leftPane.add_section("Equipment");
        leftPane.add_select_button({
            .key = "Equip X",
            .value = "TODO",
            .selected = true,
        });
        leftPane.add_select_button({
            .key = "Equip Y",
            .value = "TODO",
            .selected = false,
        });

        auto& rightPane = add_child<Pane>(content, Pane::Direction::Vertical);
        rightPane.add_button({
            .text = "Hello, world!",
        });
    });

    add_tab("Location", [this](Rml::Element* content) {
        // TODO
    });

    add_tab("Inventory", [this](Rml::Element* content) {
        // TODO
    });

    add_tab("Collection", [this](Rml::Element* content) {
        // TODO
    });

    add_tab("Flags", [this](Rml::Element* content) {
        // TODO
    });

    add_tab("Minigame", [this](Rml::Element* content) {
        // TODO
    });

    add_tab("Config", [this](Rml::Element* content) {
        // TODO
    });
}

}  // namespace dusk::ui
