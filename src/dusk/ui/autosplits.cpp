#include "autosplits.hpp"

#include "Z2AudioLib/Z2SeMgr.h"
#include "aurora/lib/window.hpp"
#include "dusk/file_select.hpp"
#include "fmt/format.h"
#include "m_Do/m_Do_audio.h"
#include "pane.hpp"
#include "ui.hpp"

#include <chrono>

namespace dusk::ui {
namespace {

using autosplit::ImportResult;
using autosplit::Manager;

constexpr SDL_DialogFileFilter kLssFilters[] = {
    {"LiveSplit splits", "lss"},
    {"All files",        "*"},
};

void on_lss_picked(void* /*userdata*/, const char* path, const char* error) {
    if (path == nullptr) {
        if (error != nullptr) {
            push_toast({
                .title = "Import canceled",
                .content = error,
                .duration = std::chrono::seconds(3),
            });
        }
        return;
    }
    const auto result = Manager::get().importFromLss(path);
    switch (result) {
    case ImportResult::Success:
        push_toast({
            .title = "Splits imported",
            .content = fmt::format("{} splits loaded", Manager::get().splits().size()),
            .duration = std::chrono::seconds(3),
        });
        break;
    case ImportResult::FileNotFound:
        push_toast({.title = "Import failed", .content = "File not found",
            .duration = std::chrono::seconds(3)});
        break;
    case ImportResult::ReadError:
        push_toast({.title = "Import failed", .content = "Could not read the file",
            .duration = std::chrono::seconds(3)});
        break;
    case ImportResult::NoSegments:
        push_toast({.title = "Import failed",
            .content = "No <Segments> block found in this .lss file",
            .duration = std::chrono::seconds(3)});
        break;
    }
}

void open_lss_picker() {
    const auto& current = Manager::get().lssPath();
    ShowFileSelect(&on_lss_picked, nullptr, aurora::window::get_sdl_window(),
        kLssFilters, static_cast<int>(std::size(kLssFilters)),
        current.empty() ? nullptr : current.c_str(), false);
}

Rml::String split_value_label(const autosplit::MappedSplit& split) {
    if (split.eventId.empty()) return "—";
    if (const auto* ev = Manager::get().findEvent(split.eventId)) {
        return ev->displayName;
    }
    return "—";
}

void populate_catalog(Pane& pane, std::size_t splitIndex) {
    pane.clear();
    pane.add_section("Assign Event");

    auto& mgr = Manager::get();
    for (const auto& ev : mgr.catalog()) {
        const std::string id(ev.id);
        pane.add_button({
            .text = ev.displayName,
            .isSelected = [id, splitIndex] {
                const auto& splits = Manager::get().splits();
                return splitIndex < splits.size() && splits[splitIndex].eventId == id;
            },
        }).on_pressed([id, splitIndex] {
            mDoAud_seStartMenu(kSoundClick);
            Manager::get().setEventForSplit(splitIndex, id);
        });
    }

    pane.add_section("Other");
    pane.add_button({
        .text = "Unmap",
        .isSelected = [splitIndex] {
            const auto& splits = Manager::get().splits();
            return splitIndex < splits.size() && splits[splitIndex].eventId.empty();
        },
    }).on_pressed([splitIndex] {
        mDoAud_seStartMenu(kSoundClick);
        Manager::get().setEventForSplit(splitIndex, "");
    });

    pane.finalize();
}

void populate_left(Pane& leftPane, Pane& rightPane) {
    leftPane.clear();
    auto& mgr = Manager::get();

    if (mgr.splits().empty()) {
        leftPane.add_section("Import");
        leftPane.register_control(
            leftPane.add_button("Import Splits from LiveSplit...").on_pressed([] {
                mDoAud_seStartMenu(kSoundClick);
                open_lss_picker();
            }),
            rightPane, [](Pane& pane) {
                pane.add_section("How it works");
                pane.add_text(
                    "Pick a .lss file containing the splits you are using on livesplit "
                    "to map game events.");
                pane.finalize();
            });
        leftPane.finalize();
        return;
    }

    leftPane.add_section(fmt::format("Splits ({})", mgr.splits().size()));
    for (std::size_t i = 0; i < mgr.splits().size(); ++i) {
        auto& btn = leftPane.add_select_button({
            .key = fmt::format("{:02}. {}", i + 1, mgr.splits()[i].name),
            .getValue = [i] {
                const auto& splits = Manager::get().splits();
                if (i >= splits.size()) return Rml::String("—");
                return split_value_label(splits[i]);
            },
        });
        auto* btnPtr = &btn;
        btn.listen(Rml::EventId::Submit, [i, &rightPane, btnPtr](Rml::Event& event) {
            if (event.GetTargetElement() != btnPtr->root()) return;
            populate_catalog(rightPane, i);
        });
        btn.on_nav_command([i, &rightPane](Rml::Event&, NavCommand cmd) {
            if (cmd == NavCommand::Right) {
                populate_catalog(rightPane, i);
            }
            return false;
        });
    }

    leftPane.add_section("Actions");
    leftPane.register_control(
        leftPane.add_button("Re-import from LiveSplit...").on_pressed([] {
            mDoAud_seStartMenu(kSoundClick);
            open_lss_picker();
        }),
        rightPane, [](Pane& pane) {
            pane.add_text("Re-import the .lss file. Mappings on segments whose name "
                          "hasn't changed are preserved.");
            pane.finalize();
        });
    leftPane.register_control(
        leftPane.add_button("Clear Splits").on_pressed([] {
            mDoAud_seStartMenu(kSoundClick);
            Manager::get().clearSplits();
        }),
        rightPane, [](Pane& pane) {
            pane.add_text("Remove all imported splits and their mappings.");
            pane.finalize();
        });

    leftPane.finalize();

    rightPane.add_section("Map a split");
    rightPane.add_text("Click a split on the left to choose which game event triggers it.");
    rightPane.finalize();
}

}  // namespace

AutosplitsWindow::AutosplitsWindow() {
    snapshot();
    add_tab("Autosplits", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);
        populate_left(leftPane, rightPane);
    });
}

void AutosplitsWindow::snapshot() {
    mSnapshot = Manager::get().splits();
    mSnapshotPath = Manager::get().lssPath();
}

void AutosplitsWindow::update() {
    const auto& current = Manager::get().splits();
    bool dirty = current.size() != mSnapshot.size() ||
                 Manager::get().lssPath() != mSnapshotPath;
    if (!dirty) {
        for (std::size_t i = 0; i < current.size(); ++i) {
            if (current[i].name != mSnapshot[i].name ||
                current[i].eventId != mSnapshot[i].eventId) {
                dirty = true;
                break;
            }
        }
    }
    if (dirty) {
        snapshot();
        refresh_active_tab();
    }
    Window::update();
}

}  // namespace dusk::ui
