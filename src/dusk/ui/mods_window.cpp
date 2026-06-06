#include "mods_window.hpp"

#include <cmath>
#include <ranges>
#include <string>

#include "bool_button.hpp"
#include "dusk/mod_loader.hpp"
#include "fmt/format.h"
#include "number_button.hpp"
#include "pane.hpp"

namespace dusk::ui {

ModsWindow::ModsWindow() {
    add_tab("Mods", [this](Rml::Element* content) {
        // Master-detail: left = list of mods + refresh, right = the selected
        // mod's enable toggle, settings, and any custom panel it builds.
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Controlled);

        leftPane.add_section("Mods");
        leftPane.add_button(Rml::String{"Refresh Mods"}).on_pressed([this] {
            mDoAud_seStartMenu(kSoundItemChange);
            dusk::ModLoader::instance().refresh();
            refresh_active_tab();
        });

        auto modsView = dusk::ModLoader::instance().mods();
        if (std::ranges::empty(modsView)) {
            leftPane.add_text("No mods installed.");
            leftPane.add_rml(
                "<br/>Each mod is a folder (or <i>.dusk</i> package) inside the <b>mods</b> folder "
                "in your Dusklight data folder, containing a library for your platform "
                "(<i>.so</i>, <i>.dll</i>, or <i>.dylib</i>). Add one and press "
                "<b>Refresh Mods</b>.");
            return;
        }

        leftPane.add_section("Installed Mods");
        for (const dusk::LoadedMod& entry : modsView) {
            const std::string id = entry.metadata.id;
            auto& item = leftPane.add_button(Rml::String{entry.metadata.name});

            // Focusing a mod fills the right pane with its details + settings.
            leftPane.register_control(item, rightPane, [this, id](Pane& pane) {
                mFocusedModId = id;
                dusk::LoadedMod* mod = dusk::ModLoader::instance().find(id);
                if (mod == nullptr) {
                    return;
                }
                pane.add_rml(fmt::format("<b>{}</b><br/>{}<br/><br/>Version: {}<br/>Author: {}",
                    mod->metadata.name,
                    mod->metadata.description.empty() ? "No description provided."
                                                      : mod->metadata.description,
                    mod->metadata.version.empty() ? "?" : mod->metadata.version,
                    mod->metadata.author.empty() ? "Unknown" : mod->metadata.author));

                pane.add_section("Options");
                pane.add_child<BoolButton>(BoolButton::Props{
                    .key = "Enabled",
                    .getValue = [id] { return dusk::ModLoader::instance().isEnabled(id); },
                    .setValue =
                        [id](bool value) {
                            mDoAud_seStartMenu(kSoundItemChange);
                            dusk::ModLoader::instance().setEnabled(id, value);
                        },
                    .isModified = [] { return false; },
                });

                for (const dusk::ModSetting& setting : mod->settings) {
                    const std::string key = setting.key;
                    if (setting.type == dusk::SettingType::Bool) {
                        pane.add_child<BoolButton>(BoolButton::Props{
                            .key = Rml::String{setting.label},
                            .getValue =
                                [id, key] {
                                    return dusk::ModLoader::instance().getSetting(id, key) != 0.0;
                                },
                            .setValue =
                                [id, key](bool value) {
                                    mDoAud_seStartMenu(kSoundItemChange);
                                    dusk::ModLoader::instance().setSetting(id, key, value ? 1.0 : 0.0);
                                },
                            .isModified = [] { return false; },
                        });
                    } else {
                        const int step =
                            setting.step != 0.0 ? static_cast<int>(setting.step) : 1;
                        pane.add_child<NumberButton>(NumberButton::Props{
                            .key = Rml::String{setting.label},
                            .getValue =
                                [id, key] {
                                    return static_cast<int>(
                                        std::lround(dusk::ModLoader::instance().getSetting(id, key)));
                                },
                            .setValue =
                                [id, key](int value) {
                                    mDoAud_seStartMenu(kSoundItemChange);
                                    dusk::ModLoader::instance().setSetting(
                                        id, key, static_cast<double>(value));
                                },
                            .isModified = [] { return false; },
                            .min = static_cast<int>(setting.minValue),
                            .max = static_cast<int>(setting.maxValue),
                            .step = step,
                        });
                    }
                }

                // A mod may also build its own custom panel (e.g. live status).
                for (const auto& cb : mod->tab_content) {
                    cb.build_fn(static_cast<void*>(pane.root()), cb.userdata);
                }
            });
        }
    });
}

void ModsWindow::update() {
    // Drive the focused mod's live UI callbacks (custom panels).
    if (!mFocusedModId.empty()) {
        if (dusk::LoadedMod* mod = dusk::ModLoader::instance().find(mFocusedModId)) {
            for (const auto& cb : mod->tab_updates) {
                cb.update_fn(cb.userdata);
            }
        }
    }
    Window::update();
}

}  // namespace dusk::ui
