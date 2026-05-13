#include "warp.hpp"

#include "editor.hpp"
#include "pane.hpp"

#include "dusk/map_loader_definitions.h"
#include "fmt/format.h"

namespace dusk::ui {
namespace {

constexpr int kMinLayer = -1;
constexpr int kMaxLayer = 14;

struct WarpSelectionState {
    int regionIdx = 0;
    int mapIdx = 0;
    int roomIdx = 0;
    int pointIdx = 0;
    int layer = -1;
    bool showInternalNames = false;
};

WarpSelectionState& selection_state() {
    static WarpSelectionState state;
    return state;
}

const RegionEntry* selected_region(const WarpSelectionState& state) {
    if (state.regionIdx < 0 || state.regionIdx >= static_cast<int>(gameRegions.size())) {
        return nullptr;
    }
    return &gameRegions[state.regionIdx];
}

const MapEntry* selected_map(const WarpSelectionState& state) {
    const auto* region = selected_region(state);
    if (region == nullptr || state.mapIdx < 0 || state.mapIdx >= static_cast<int>(region->maps.size())) {
        return nullptr;
    }
    return &region->maps[state.mapIdx];
}

const RoomEntry* selected_room(const WarpSelectionState& state) {
    const auto* map = selected_map(state);
    if (map == nullptr || state.roomIdx < 0 || state.roomIdx >= static_cast<int>(map->mapRooms.size())) {
        return nullptr;
    }
    return &map->mapRooms[state.roomIdx];
}

const s16* selected_point(const WarpSelectionState& state) {
    const auto* room = selected_room(state);
    if (room == nullptr || state.pointIdx < 0 ||
        state.pointIdx >= static_cast<int>(room->roomPoints.size()))
    {
        return nullptr;
    }
    return &room->roomPoints[state.pointIdx];
}

void clamp_indices(WarpSelectionState& state) {
    if (gameRegions.empty()) {
        state.regionIdx = -1;
        state.mapIdx = -1;
        state.roomIdx = -1;
        state.pointIdx = -1;
        state.layer = std::clamp(state.layer, kMinLayer, kMaxLayer);
        return;
    }

    state.regionIdx = std::clamp(state.regionIdx, 0, static_cast<int>(gameRegions.size()) - 1);
    const auto& region = gameRegions[state.regionIdx];
    if (region.maps.empty()) {
        state.mapIdx = -1;
        state.roomIdx = -1;
        state.pointIdx = -1;
        state.layer = std::clamp(state.layer, kMinLayer, kMaxLayer);
        return;
    }

    state.mapIdx = std::clamp(state.mapIdx, 0, static_cast<int>(region.maps.size()) - 1);
    const auto& map = region.maps[state.mapIdx];
    if (map.mapRooms.empty()) {
        state.roomIdx = -1;
        state.pointIdx = -1;
        state.layer = std::clamp(state.layer, kMinLayer, kMaxLayer);
        return;
    }

    state.roomIdx = std::clamp(state.roomIdx, 0, static_cast<int>(map.mapRooms.size()) - 1);
    const auto& room = map.mapRooms[state.roomIdx];
    if (room.roomPoints.empty()) {
        state.pointIdx = -1;
    } else {
        state.pointIdx = std::clamp(state.pointIdx, 0, static_cast<int>(room.roomPoints.size()) - 1);
    }

    state.layer = std::clamp(state.layer, kMinLayer, kMaxLayer);
}

bool can_warp(const WarpSelectionState& state) {
    return selected_point(state) != nullptr;
}

void reset_selection(WarpSelectionState& state) {
    state.roomIdx = 0;
    state.pointIdx = 0;
    state.layer = kMinLayer;
    clamp_indices(state);
}

void populate_map_picker(Pane& pane, WarpSelectionState& state) {
    pane.clear();
    clamp_indices(state);
    if (state.regionIdx < 0 || state.regionIdx >= static_cast<int>(gameRegions.size())) {
        return;
    }

    pane.add_button({
                    .text = "Show Internal Names",
                    .isSelected = [&state] { return state.showInternalNames; },
                })
        .on_pressed([&pane, &state] {
            mDoAud_seStartMenu(kSoundItemChange);
            state.showInternalNames = !state.showInternalNames;
            populate_map_picker(pane, state);
        });

    pane.add_section("Maps");
    const auto& region = gameRegions[state.regionIdx];
    for (int i = 0; i < static_cast<int>(region.maps.size()); ++i) {
        pane.add_button({
                        .text = stage_option_label(region.maps[i], state.showInternalNames),
                        .isSelected = [i, &state] { return state.mapIdx == i; },
                    })
            .on_pressed([i, &state] {
                mDoAud_seStartMenu(kSoundItemChange);
                if (state.mapIdx != i) {
                    state.mapIdx = i;
                    reset_selection(state);
                }
            });
    }
}

}  // namespace

WarpWindow::WarpWindow() {
    add_tab("Warp", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Type::Controlled);
        auto& rightPane = add_child<Pane>(content, Pane::Type::Uncontrolled);
        auto& state = selection_state();
        clamp_indices(state);

        leftPane.add_section("Destination");
        leftPane.register_control(
            leftPane.add_select_button({
                .key = "Region",
                .getValue =
                    [&state] {
                        clamp_indices(state);
                        const auto* region = selected_region(state);
                        return region == nullptr ? Rml::String{"None"} :
                                                   Rml::String{region->regionName};
                    },
            }),
            rightPane, [&state](Pane& pane) {
                pane.clear();
                for (int i = 0; i < static_cast<int>(gameRegions.size()); ++i) {
                    pane.add_button({
                                    .text = gameRegions[i].regionName,
                                    .isSelected = [i, &state] { return state.regionIdx == i; },
                                })
                        .on_pressed([i, &state] {
                            mDoAud_seStartMenu(kSoundItemChange);
                            if (state.regionIdx != i) {
                                state.regionIdx = i;
                                state.mapIdx = 0;
                                reset_selection(state);
                            }
                        });
                }
            });

        leftPane.register_control(
            leftPane.add_select_button({
                .key = "Map",
                .getValue =
                    [&state] {
                        clamp_indices(state);
                        const auto* map = selected_map(state);
                        return map == nullptr ? Rml::String{"None"} :
                                                stage_option_label(*map, state.showInternalNames);
                    },
            }),
            rightPane, [&state](Pane& pane) { populate_map_picker(pane, state); });

        leftPane.register_control(
            leftPane.add_select_button({
                .key = "Room",
                .getValue = [&state] {
                        clamp_indices(state);
                        const auto* room = selected_room(state);
                        return room == nullptr ? Rml::String{"None"} :
                                                 fmt::format("{}", room->roomNo);
                    },
                .isDisabled = [&state] {
                        clamp_indices(state);
                        const auto* map = selected_map(state);
                        return map == nullptr || map->mapRooms.size() <= 1;
                    },
            }),
            rightPane, [&state](Pane& pane) {
                pane.clear();
                clamp_indices(state);
                if (state.regionIdx < 0 || state.regionIdx >= static_cast<int>(gameRegions.size())) {
                    return;
                }
                const auto& region = gameRegions[state.regionIdx];
                if (state.mapIdx < 0 || state.mapIdx >= static_cast<int>(region.maps.size())) {
                    return;
                }
                const auto& map = region.maps[state.mapIdx];
                for (int i = 0; i < static_cast<int>(map.mapRooms.size()); ++i) {
                    pane.add_button({
                                    .text = fmt::format("{}", map.mapRooms[i].roomNo),
                                    .isSelected = [i, &state] { return state.roomIdx == i; },
                                })
                        .on_pressed([i, &state] {
                            mDoAud_seStartMenu(kSoundItemChange);
                            if (state.roomIdx != i) {
                                state.roomIdx = i;
                                state.pointIdx = 0;
                                clamp_indices(state);
                            }
                        });
                }
            });

        leftPane.register_control(
            leftPane.add_select_button({
                .key = "Point",
                .getValue = [&state] {
                        clamp_indices(state);
                        const auto* point = selected_point(state);
                        return point == nullptr ? Rml::String{"None"} : fmt::format("{}", *point);
                    },
                .isDisabled = [&state] {
                        clamp_indices(state);
                        const auto* room = selected_room(state);
                        return room == nullptr || room->roomPoints.size() <= 1;
                    },
            }),
            rightPane, [&state](Pane& pane) {
                pane.clear();
                clamp_indices(state);
                if (state.regionIdx < 0 || state.regionIdx >= static_cast<int>(gameRegions.size())) {
                    return;
                }
                const auto& region = gameRegions[state.regionIdx];
                if (state.mapIdx < 0 || state.mapIdx >= static_cast<int>(region.maps.size())) {
                    return;
                }
                const auto& map = region.maps[state.mapIdx];
                if (state.roomIdx < 0 || state.roomIdx >= static_cast<int>(map.mapRooms.size())) {
                    return;
                }
                const auto& room = map.mapRooms[state.roomIdx];
                for (int i = 0; i < static_cast<int>(room.roomPoints.size()); ++i) {
                    pane.add_button({
                                    .text = fmt::format("{}", room.roomPoints[i]),
                                    .isSelected = [i, &state] { return state.pointIdx == i; },
                                })
                        .on_pressed([i, &state] {
                            if (state.pointIdx != i) {
                                mDoAud_seStartMenu(kSoundItemChange);
                                state.pointIdx = i;
                                clamp_indices(state);
                            }
                        });
                }
            });

        leftPane.register_control(
            leftPane.add_select_button({
                .key = "Layer",
                .getValue = [&state] { return fmt::format("{}", state.layer); },
            }),
            rightPane, [&state](Pane& pane) {
                pane.clear();
                for (int layer = kMinLayer; layer <= kMaxLayer; ++layer) {
                    pane.add_button({
                                    .text = fmt::format("{}", layer),
                                    .isSelected = [layer, &state] { return state.layer == layer; },
                                })
                        .on_pressed([layer, &state] {
                            if (state.layer != layer) {
                                mDoAud_seStartMenu(kSoundItemChange);
                                state.layer = layer;
                            }
                        });
                }
            });

        leftPane.add_section("Action");
        leftPane.register_control(
            leftPane.add_button({
                        .text = "Warp",
                        .isDisabled = [&state] {
                            clamp_indices(state);
                            return !can_warp(state);
                        },
                    })
                .on_pressed([&state] {
                    clamp_indices(state);
                    if (!can_warp(state)) {
                        return;
                    }

                    mDoAud_seStartMenu(kSoundClick);
                    const auto& region = gameRegions[state.regionIdx];
                    const auto& map = region.maps[state.mapIdx];
                    const auto& room = map.mapRooms[state.roomIdx];
                    dComIfGp_setNextStage( map.mapFile, room.roomPoints[state.pointIdx], room.roomNo, state.layer);
                }),
            rightPane, [](Pane& pane) {
                pane.clear();
                pane.add_text("Warp to the selected destination.");
            });
    });
}

}  // namespace dusk::ui
