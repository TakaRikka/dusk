#include "editor.hpp"

#include <RmlUi/Core.h>

namespace dusk::ui {
namespace {

const Rml::String kPlayerStatusContent = R"RML(
<div class="pane">
    <div class="section-heading">Player</div>
    <button class="select-button">
        <div class="key">Player Name</div>
        <div class="value">Link</div>
    </button>
    <button class="select-button">
        <div class="key">Horse Name</div>
        <div class="value">Epona</div>
    </button>
    <button class="select-button">
        <div class="key">Max Health</div>
        <div class="value">15</div>
    </button>
    <button class="select-button">
        <div class="key">Health</div>
        <div class="value">12</div>
    </button>
    <button class="select-button">
        <div class="key">Max Oil</div>
        <div class="value">0</div>
    </button>
    <button class="select-button">
        <div class="key">Oil</div>
        <div class="value">0</div>
    </button>
    <div class="section-heading">Equipment</div>
    <button class="select-button selected">
        <div class="key">Equip X</div>
        <div class="value">Spinner</div>
    </button>
    <button class="select-button">
        <div class="key">Equip Y</div>
        <div class="value">None</div>
    </button>
    <button class="select-button">
        <div class="key">Combo Equip X</div>
        <div class="value">None</div>
    </button>
    <button class="select-button">
        <div class="key">Combo Equip Y</div>
        <div class="value">None</div>
    </button>
    <button class="select-button">
        <div class="key">Clothes</div>
        <div class="value">Hero's Clothes</div>
    </button>
</div>
<!-- TODO: right pane is going to be dynamic based on the highlighted left pane value -->
<div class="pane">
    <button class="button">Slot 0 (Gale Boomerang)</button>
    <button class="button">Slot 1 (Lantern)</button>
    <button class="button">Slot 2 (Spinner)</button>
    <button class="button">Slot 3 (Iron Boots)</button>
    <button class="button">Slot 4 (Hero's Bow)</button>
    <button class="button">Slot 5 (Hawkeye)</button>
    <button class="button">Slot 6 (Ball and Chain)</button>
    <button class="button">Slot 7 (None)</button>
    <button class="button">Slot 8 (Dominion Rod)</button>
</div>
)RML";

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

}  // namespace

EditorWindow::EditorWindow()
    : Window({.tabs = {
                  {"Player Status",
                      [](Rml::Element* content) {
                          // TODO: actually bind values and events. wonder if we should have
                          // a SettingsPane element or something for sharing?
                          Rml::Factory::InstanceElementText(content, kPlayerStatusContent);
                      }},
                  {"Location",
                      [](Rml::Element* content) {
                          Rml::Factory::InstanceElementText(content, kLocationContent);
                      }},
                  {"Inventory"},
              }}) {}

}  // namespace dusk::ui