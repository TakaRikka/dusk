#include "mods_window.hpp"

#include "dusk/mod_loader.hpp"
#include "dusk/mods/svc/ui.hpp"
#include "fmt/format.h"
#include "logs_window.hpp"
#include "mod_texture_provider.hpp"
#include "pane.hpp"

#include "Z2AudioLib/Z2SeMgr.h"
#include "m_Do/m_Do_audio.h"

#include <memory>
#include <string>
#include <string_view>

namespace dusk::ui {
namespace {

struct ModStatus {
    const char* badgeClass = "";
    const char* text = "";
};

bool mod_enabled(const mods::LoadedMod& mod) {
    return mod.cvarIsEnabled != nullptr && mod.cvarIsEnabled->getValue();
}

ModStatus mod_status(const mods::LoadedMod& mod) {
    if (mod.loadFailed) {
        return {"failed", "Failed"};
    }
    if (mod.active) {
        return {"active", "Active"};
    }
    if (mod.suspendedByProvider) {
        return {"suspended", "Suspended"};
    }
    return {"", "Disabled"};
}

// Truncates to at most maxBytes without splitting a UTF-8 sequence.
std::string snippet(std::string_view text, size_t maxBytes) {
    if (text.size() <= maxBytes) {
        return std::string{text};
    }
    size_t end = maxBytes;
    while (end > 0 && (static_cast<unsigned char>(text[end]) & 0xC0) == 0x80) {
        --end;
    }
    return std::string{text.substr(0, end)} + "...";
}

class ModListEntry : public FluentComponent<ModListEntry> {
public:
    ModListEntry(Rml::Element* parent, const mods::LoadedMod& mod)
        : FluentComponent{append(parent, "mod-entry")} {
        Rml::String iconRml;
        if (!mod.metadata.iconPath.empty()) {
            iconRml = fmt::format(R"(<img class="mod-icon" src="{}"/>)",
                mod_image_source(mod, mod.metadata.iconPath));
        } else {
            iconRml = R"(<icon class="mod-icon placeholder"/>)";
        }
        const auto status = mod_status(mod);
        mRoot->SetInnerRML(fmt::format(
            R"({})"
            R"(<div class="mod-entry-info">)"
            R"(<div class="mod-entry-name"><span class="mod-entry-name-text">{}</span>)"
            R"(<span class="mod-entry-version">v{}</span></div>)"
            R"(<div class="mod-entry-sub">{} - <span class="mod-entry-status {}">{}</span></div>)"
            R"(<div class="mod-entry-desc">{}</div>)"
            R"(</div>)",
            iconRml, escape(mod.metadata.name), escape(mod.metadata.version),
            escape(mod.metadata.author), status.badgeClass, status.text,
            escape(snippet(mod.metadata.description, 90))));
        mRoot->SetClass("inactive", !mod.active);
        mRoot->SetClass("failed", mod.loadFailed);

        on_nav_command([this](Rml::Event&, NavCommand cmd) {
            if (cmd == NavCommand::Confirm) {
                mRoot->DispatchEvent(Rml::EventId::Submit, {});
                return true;
            }
            return false;
        });
    }
};

class ModDetailHeader : public FluentComponent<ModDetailHeader> {
public:
    ModDetailHeader(
        Rml::Element* parent, const mods::LoadedMod& mod, std::function<void()> onShowLogs)
        : FluentComponent{append(parent, "mod-header")} {
        const bool hasBanner = !mod.metadata.bannerPath.empty();
        mRoot->SetClass(hasBanner ? "has-banner" : "no-banner", true);
        if (hasBanner) {
            mRoot->SetProperty("decorator", fmt::format(R"(image("{}" cover center top))",
                                                mod_image_source(mod, mod.metadata.bannerPath)));
        }

        auto* actions = append(mRoot, "div");
        actions->SetClass("mod-actions", true);
        const std::string modId = mod.metadata.id;
        if (mod_enabled(mod)) {
            if (!mod.inPlace) {
                make_button(actions, "Reload").on_pressed([modId] {
                    mods::ModLoader::instance().request_reload(modId);
                });
            }
            make_button(actions, "Disable").on_pressed([modId] {
                mods::ModLoader::instance().request_disable(modId);
            });
        } else {
            make_button(actions, "Enable").on_pressed([modId] {
                mods::ModLoader::instance().request_enable(modId);
            });
        }
        make_button(actions, "Logs").on_pressed(std::move(onShowLogs));

        listen(Rml::EventId::Keydown, [this](Rml::Event& event) {
            const auto cmd = map_nav_event(event);
            if (cmd != NavCommand::Left && cmd != NavCommand::Right) {
                return;
            }
            int index = -1;
            for (int i = 0; i < static_cast<int>(mButtons.size()); ++i) {
                if (mButtons[i]->contains(event.GetTargetElement())) {
                    index = i;
                    break;
                }
            }
            if (index == -1) {
                return;
            }
            const int next = index + (cmd == NavCommand::Right ? 1 : -1);
            if (next >= 0 && next < static_cast<int>(mButtons.size()) && mButtons[next]->focus()) {
                mDoAud_seStartMenu(kSoundItemFocus);
                event.StopPropagation();
            }
        });
    }

    bool focus() override {
        for (auto* button : mButtons) {
            if (button->focus()) {
                return true;
            }
        }
        return false;
    }

private:
    Button& make_button(Rml::Element* parent, Rml::String text) {
        auto button = std::make_unique<Button>(parent, std::move(text));
        Button& ref = *button;
        mChildren.emplace_back(std::move(button));
        mButtons.push_back(&ref);
        return ref;
    }

    std::vector<Button*> mButtons;
};

}  // namespace

ModsWindow::ModsWindow() : Window{Props{.tabBar = false, .styleSheets = {"res/rml/mods.rcss"}}} {
    mRoot->SetClass("mods", true);

    for (auto& trackedMod : mods::ModLoader::instance().mods()) {
        mSnapshot.push_back({
            .mod = &trackedMod,
            .active = trackedMod.active,
            .loadFailed = trackedMod.loadFailed,
            .enabled = mod_enabled(trackedMod),
            .suspended = trackedMod.suspendedByProvider,
            .cacheGeneration = trackedMod.cacheGeneration,
        });
    }

    set_content([this](Rml::Element* content) { build_content(content); });
}

void ModsWindow::build_content(Rml::Element* content) {
    mEntries.clear();
    mEntryMods.clear();

    auto& listPane = add_child<Pane>(content, Pane::Type::Controlled);
    listPane.root()->SetClass("mod-list", true);
    auto& detailPane = add_child<Pane>(content, Pane::Type::Uncontrolled);
    detailPane.root()->SetClass("mod-detail", true);

    if (mods::ModLoader::instance().mods().empty()) {
        listPane.add_text("No mods installed.");
        listPane.finalize();
        detailPane.finalize();
        return;
    }

    for (auto& trackedMod : mods::ModLoader::instance().mods()) {
        auto& entry = listPane.add_child<ModListEntry>(trackedMod);
        mEntries.push_back(&entry);
        mEntryMods.push_back(&trackedMod);
        listPane.register_control(entry, detailPane, [this, tracked = &trackedMod](Pane& pane) {
            mSelectedMod = tracked;
            pane.clear();
            build_detail(pane, *tracked);
            mark_current_entry();
        });
    }

    if (mSelectedMod == nullptr) {
        mSelectedMod = mEntryMods.front();
    }
    build_detail(detailPane, *mSelectedMod);
    mark_current_entry();

    listPane.finalize();
}

void ModsWindow::build_detail(Pane& pane, mods::LoadedMod& mod) {
    pane.root()->SetAttribute("mod-id", mod.metadata.id);
    pane.add_child<ModDetailHeader>(
        mod, [this, id = mod.metadata.id] { push(std::make_unique<LogsWindow>(id)); });

    Rml::String statusBadge;
    if (mod.loadFailed || mod.suspendedByProvider) {
        const auto status = mod_status(mod);
        statusBadge = fmt::format(
            R"(&nbsp;<span class="status-badge {}">{}</span>)", status.badgeClass, status.text);
    }
    pane.add_rml(fmt::format(R"(<div class="mod-title">{} )"
                             R"(<span class="mod-title-version">v{}</span>{}</div>)"
                             R"(<div class="mod-author">by {}</div>)",
        escape(mod.metadata.name), escape(mod.metadata.version), statusBadge,
        escape(mod.metadata.author)));

    if (mod.loadFailed && !mod.failureReason.empty()) {
        pane.add_rml(fmt::format(R"(<div class="mod-info-row">)"
                                 R"(<span class="mod-info-label failed">Reason</span>)"
                                 R"(<span class="mod-info-value">{}</span>)"
                                 R"(</div>)",
            escape(mod.failureReason)));
    } else if (mod.suspendedByProvider) {
        std::string providers;
        for (const auto& edge : mod.dependencies) {
            if (edge.required && edge.mod != nullptr && !edge.mod->active) {
                if (!providers.empty()) {
                    providers += ", ";
                }
                providers += edge.mod->metadata.name;
            }
        }
        pane.add_rml(fmt::format(R"(<div class="mod-info-row">)"
                                 R"(<span class="mod-info-label">Waiting on</span>)"
                                 R"(<span class="mod-info-value">{}</span>)"
                                 R"(</div>)",
            escape(providers)));
    }

    std::string activeDependents;
    for (const auto& edge : mod.dependents) {
        if (edge.mod != nullptr && edge.mod->active) {
            if (!activeDependents.empty()) {
                activeDependents += ", ";
            }
            activeDependents += edge.mod->metadata.name;
        }
    }
    if (mod.active && !activeDependents.empty()) {
        pane.add_rml(fmt::format(R"(<div class="mod-restart-note">{}</div>)",
            escape(fmt::format("Disabling or reloading also restarts: {}", activeDependents))));
    }

    if (!mod.metadata.description.empty()) {
        pane.add_text(mod.metadata.description)->SetClass("mod-description", true);
    }

    if (mod.active) {
        mods::svc::ui_build_mods_panels(mod, pane);
    }

    pane.finalize();
}

void ModsWindow::mark_current_entry() {
    for (size_t i = 0; i < mEntries.size(); ++i) {
        mEntries[i]->root()->SetClass("current", mEntryMods[i] == mSelectedMod);
    }
}

void ModsWindow::update() {
    bool dirty = false;
    for (auto& snapshot : mSnapshot) {
        const auto& mod = *snapshot.mod;
        if (mod.active != snapshot.active || mod.loadFailed != snapshot.loadFailed ||
            mod_enabled(mod) != snapshot.enabled || mod.suspendedByProvider != snapshot.suspended ||
            mod.cacheGeneration != snapshot.cacheGeneration)
        {
            snapshot.active = mod.active;
            snapshot.loadFailed = mod.loadFailed;
            snapshot.enabled = mod_enabled(mod);
            snapshot.suspended = mod.suspendedByProvider;
            snapshot.cacheGeneration = mod.cacheGeneration;
            dirty = true;
        }
    }
    if (dirty) {
        auto* focused = mDocument != nullptr ? mDocument->GetFocusLeafNode() : nullptr;
        bool hadContentFocus = false;
        for (auto* node = focused; node != nullptr; node = node->GetParentNode()) {
            if (node == mContentRoot) {
                hadContentFocus = true;
                break;
            }
        }
        rebuild_content();
        if (hadContentFocus) {
            for (size_t i = 0; i < mEntryMods.size(); ++i) {
                if (mEntryMods[i] == mSelectedMod) {
                    mEntries[i]->focus();
                    break;
                }
            }
        }
    }

    if (mSelectedMod != nullptr && mSelectedMod->active) {
        mods::svc::ui_update_mods_panels(*mSelectedMod);
    }

    Window::update();
}

}  // namespace dusk::ui
