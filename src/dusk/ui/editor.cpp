#include "editor.hpp"

#include <RmlUi/Core.h>
#include <fmt/format.h>

#include "button.hpp"
#include "number_button.hpp"
#include "pane.hpp"
#include "select_button.hpp"
#include "string_button.hpp"

namespace dusk::ui {
namespace {

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
        return "";
    }
    return dComIfGs_getPlayerName();
}

void set_player_name(Rml::String name) {
    dComIfGs_setPlayerName(name.c_str());
}

Rml::String get_horse_name() {
    if (!has_save_data()) {
        return "";
    }
    return dComIfGs_getHorseName();
}

void set_horse_name(Rml::String name) {
    dComIfGs_setHorseName(name.c_str());
}

}  // namespace

EditorWindow::EditorWindow() {
    add_tab("Player Status", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Direction::Vertical);
        auto& rightPane = add_child<Pane>(content, Pane::Direction::Vertical);

        leftPane.add_section("Player");
        leftPane.add_child<StringButton>(leftPane.root(), StringButton::Props{
                                                              .key = "Player Name",
                                                              .getValue = get_player_name,
                                                              .setValue = set_player_name,
                                                              .maxLength = 16,
                                                          });
        leftPane.add_child<StringButton>(leftPane.root(), StringButton::Props{
                                                              .key = "Horse Name",
                                                              .getValue = get_horse_name,
                                                              .setValue = set_horse_name,
                                                              .maxLength = 16,
                                                          });
        leftPane.add_child<NumberButton>(leftPane.root(),
            NumberButton::Props{
                .key = "Max Health",
                .getValue = [] { return get_player_status()->getMaxLife(); },
                .setValue = [](int value) { return get_player_status()->setMaxLife(value); },
                .max = UINT16_MAX,  // TODO: actual max
            });
        leftPane.add_child<NumberButton>(leftPane.root(),
            NumberButton::Props{
                .key = "Health",
                .getValue = [] { return get_player_status()->getLife(); },
                .setValue = [](int value) { return get_player_status()->setLife(value); },
                .max = UINT16_MAX,  // TODO: actual max
            });
        leftPane.add_child<NumberButton>(leftPane.root(),
            NumberButton::Props{
                .key = "Max Oil",
                .getValue = [] { return get_player_status()->getMaxOil(); },
                .setValue = [](int value) { return get_player_status()->setMaxOil(value); },
                .max = UINT16_MAX,  // TODO: actual max
            });
        leftPane.add_child<NumberButton>(leftPane.root(),
            NumberButton::Props{
                .key = "Oil",
                .getValue = [] { return get_player_status()->getOil(); },
                .setValue = [](int value) { return get_player_status()->setOil(value); },
                .max = UINT16_MAX,  // TODO: actual max
            });

        leftPane.add_section("Equipment");
        leftPane.add_select_button({
            .key = "Equip X",
            .value = "TODO",
        });
        leftPane.add_select_button({
            .key = "Equip Y",
            .value = "TODO",
        });

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
