#include "ImGuiStateShare.hpp"
#include "ImGuiMenuTools.hpp"
#include "ImGuiConsole.hpp"

#include "imgui.h"
#include "fmt/format.h"
#include "absl/strings/escaping.h"
#include "nlohmann/json.hpp"

#include "d/d_com_inf_game.h"
#include "dusk/main.h"
#include "dusk/io.hpp"
#include "dusk/logging.h"
#include "dusk/settings.h"
#include "../file_select.hpp"
#include "aurora/lib/window.hpp"

#include <unordered_set>
#include <zstd.h>

namespace dusk {

using json = nlohmann::json;

#pragma pack(push, 1)
struct StateSharePacket {
    char    stageName[8];
    int8_t  roomNo;
    int8_t  layer;
    int16_t startPoint;
    // followed by raw dSv_info_c bytes
};
#pragma pack(pop)

static constexpr size_t PACKET_TOTAL     = sizeof(StateSharePacket) + sizeof(dSv_info_c);
static constexpr size_t PACKET_SAVE_ONLY = sizeof(StateSharePacket) + sizeof(dSv_save_c);
static constexpr auto STATES_FILENAME = "states.json";

static bool ValidateEncodedState(const std::string&);

void ImGuiStateShare::onMergeFileSelected(void* userdata, const char* path, const char* /*error*/) {
    auto* self = static_cast<ImGuiStateShare*>(userdata);
    if (path != nullptr) {
        self->m_pendingMergePath = path;
    }
}



static std::string GetStatesFilePath() {
    return (dusk::ConfigPath / STATES_FILENAME).string();
}

void ImGuiStateShare::loadStatesFile() {
    m_loaded = true;
    const std::filesystem::path filePath = dusk::ConfigPath / STATES_FILENAME;
    if (!std::filesystem::exists(filePath)) {
        return;
    }
    try {
        const std::string pathStr = filePath.string();
        auto data = io::FileStream::ReadAllBytes(pathStr.c_str());
        auto j = json::parse(data);
        if (!j.is_array()) {
            return;
        }
        for (const auto& entry : j) {
            if (!entry.contains("name") || !entry.contains("data")) {
                continue;
            }
            SavedStateEntry s;
            s.name    = entry["name"].get<std::string>();
            s.encoded = entry["data"].get<std::string>();
            m_states.push_back(std::move(s));
        }
    } catch (const std::exception& e) {
        m_statusMsg = fmt::format("Failed to load states: {}", e.what());
    }
}

void ImGuiStateShare::saveStatesFile() {
    json j = json::array();
    for (const auto& s : m_states) {
        j.push_back(json{{"name", s.name}, {"data", s.encoded}});
    }
    try {
        io::FileStream::WriteAllText(GetStatesFilePath().c_str(), j.dump(2));
    } catch (const std::exception& e) {
        m_statusMsg = fmt::format("Failed to save states: {}", e.what());
    }
}

std::string ImGuiStateShare::encodeCurrentState() {
    StateSharePacket pkt = {};
    strncpy(pkt.stageName, dComIfGp_getStartStageName(), 7);
    pkt.roomNo     = dComIfGp_getStartStageRoomNo();
    pkt.layer      = dComIfGp_getStartStageLayer();
    pkt.startPoint = dComIfGp_getStartStagePoint();

    std::string raw(PACKET_TOTAL, '\0');
    memcpy(raw.data(), &pkt, sizeof(pkt));
    memcpy(raw.data() + sizeof(pkt), &g_dComIfG_gameInfo.info, sizeof(dSv_info_c));

    size_t bound = ZSTD_compressBound(raw.size());
    std::string compressed(bound, '\0');
    compressed.resize(ZSTD_compress(compressed.data(), bound, raw.data(), raw.size(), 1));

    return absl::Base64Escape(compressed);
}

bool ImGuiStateShare::applyEncodedState(const std::string& encoded, const std::string& name) {
    std::string decoded;
    if (!absl::Base64Unescape(encoded, &decoded)) {
        m_statusMsg = "Invalid base64.";
        return false;
    }

    unsigned long long dSize = ZSTD_getFrameContentSize(decoded.data(), decoded.size());
    if (dSize == ZSTD_CONTENTSIZE_ERROR || dSize == ZSTD_CONTENTSIZE_UNKNOWN) {
        m_statusMsg = "Not a valid state string.";
        return false;
    }

    const bool isFull    = (dSize == PACKET_TOTAL);
    const bool isPartial = (dSize == PACKET_SAVE_ONLY);
    if (!isFull && !isPartial) {
        m_statusMsg = "Not a valid state string.";
        return false;
    }

    std::string raw(static_cast<size_t>(dSize), '\0');
    size_t result = ZSTD_decompress(raw.data(), raw.size(), decoded.data(), decoded.size());
    if (ZSTD_isError(result)) {
        m_statusMsg = fmt::format("Decompression failed: {}", ZSTD_getErrorName(result));
        return false;
    }

    StateSharePacket pkt;
    memcpy(&pkt, raw.data(), sizeof(pkt));
    pkt.stageName[7] = '\0';

    if (isFull) {
        memcpy(&g_dComIfG_gameInfo.info, raw.data() + sizeof(pkt), sizeof(dSv_info_c));
        m_pendingInfo = g_dComIfG_gameInfo.info;
        m_pendingSavedata.reset();
    } else {
        memcpy(&g_dComIfG_gameInfo.info.mSavedata, raw.data() + sizeof(pkt), sizeof(dSv_save_c));
        m_pendingSavedata = g_dComIfG_gameInfo.info.mSavedata;
        m_pendingInfo.reset();
    }

    s16 spawnPoint = pkt.startPoint == -4 ? -1 : pkt.startPoint;
    if (spawnPoint == -1) {
        dComIfGs_setRestartRoomParam(pkt.roomNo & 0x3F);
    }

    dusk::getTransientSettings().stateShareLoadActive = true;
    dComIfGp_setNextStage(pkt.stageName, spawnPoint, pkt.roomNo, pkt.layer, 0.0f, 0, 1, 0, 0, 1, 3);

    if (name.empty()) {
        m_statusMsg = fmt::format("{} room {} layer {}.", pkt.stageName, (int)pkt.roomNo, (int)pkt.layer);
    } else {
        m_statusMsg = fmt::format("{}: {} room {} layer {}.", name, pkt.stageName, (int)pkt.roomNo, (int)pkt.layer);
    }
    return true;
}

void ImGuiStateShare::tickPendingApply() {
    if (!m_pendingInfo.has_value() && !m_pendingSavedata.has_value()) {
        return;
    }
    if (dComIfGp_isEnableNextStage()) {
        return;
    }
    if (m_pendingInfo.has_value()) {
        g_dComIfG_gameInfo.info = *m_pendingInfo;
        m_pendingInfo.reset();
    } else {
        g_dComIfG_gameInfo.info.mSavedata = *m_pendingSavedata;
        m_pendingSavedata.reset();
    }
    dComIfGp_offOxygenShowFlag();
    dComIfGp_setMaxOxygen(600);
    dComIfGp_setOxygen(600);
    dusk::getTransientSettings().stateShareLoadActive = false;
}

static bool ValidateEncodedState(const std::string& encoded) {
    std::string decoded;
    if (!absl::Base64Unescape(encoded, &decoded)) {
        return false;
    }
    unsigned long long dSize = ZSTD_getFrameContentSize(decoded.data(), decoded.size());
    return dSize == PACKET_TOTAL || dSize == PACKET_SAVE_ONLY;
}

void ImGuiStateShare::mergeFromFile(const std::string& path) {
    try {
        auto data = io::FileStream::ReadAllBytes(path.c_str());
        auto j = json::parse(data);
        if (!j.is_array()) {
            m_statusMsg = "File does not contain a JSON array.";
            return;
        }

        std::unordered_set<std::string> existingNames;
        for (const auto& s : m_states) {
            existingNames.insert(s.name);
        }

        int added   = 0;
        int skipped = 0;
        for (const auto& entry : j) {
            if (!entry.contains("name") || !entry.contains("data")) {
                ++skipped;
                continue;
            }
            const std::string name    = entry["name"].get<std::string>();
            const std::string encoded = entry["data"].get<std::string>();
            if (!ValidateEncodedState(encoded)) {
                ++skipped;
                continue;
            }
            if (existingNames.count(name)) {
                ++skipped;
                continue;
            }
            SavedStateEntry s;
            s.name    = name;
            s.encoded = encoded;
            existingNames.insert(s.name);
            m_states.push_back(std::move(s));
            ++added;
        }

        if (added > 0) {
            saveStatesFile();
        }
        m_statusMsg = fmt::format("Merged: {} added, {} skipped.", added, skipped);
    } catch (const std::exception& e) {
        m_statusMsg = fmt::format("Failed to load file: {}", e.what());
    }
}

void ImGuiStateShare::draw(bool& open) {
    if (dusk::IsGameLaunched) {
        tickPendingApply();
    }

    if (!m_loaded) {
        loadStatesFile();
    }

    if (!m_pendingMergePath.empty()) {
        mergeFromFile(m_pendingMergePath);
        m_pendingMergePath.clear();
    }

    if (!open) {
        return;
    }

    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 0), ImVec2(FLT_MAX, FLT_MAX));
    if (!ImGui::Begin("State Manager", &open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav)) {
        ImGui::End();
        return;
    }

    const bool gameRunning = dusk::IsGameLaunched;

    const float rowH  = ImGui::GetTextLineHeightWithSpacing();
    const float listH = rowH * 8 + ImGui::GetStyle().FramePadding.y * 2;
    ImGui::BeginChild("##states", ImVec2(0, listH), true);

    if (m_states.empty()) {
        ImGui::TextDisabled("No saved states. Save or import one below.");
    }

    int toDelete = -1;
    for (int i = 0; i < (int)m_states.size(); ++i) {
        ImGui::PushID(i);

        if (m_renamingIndex == i) {
            ImGui::SetNextItemWidth(150);
            bool done = ImGui::InputText("##rename", m_renameBuffer, sizeof(m_renameBuffer),
                                         ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);
            if (done) {
                if (m_renameBuffer[0] != '\0') {
                    m_states[i].name = m_renameBuffer;
                }
                m_renamingIndex = -1;
                saveStatesFile();
            } else if (ImGui::IsItemDeactivated()) {
                m_renamingIndex = -1;
            }
        } else {
            ImGui::Selectable(m_states[i].name.c_str(), false, ImGuiSelectableFlags_None, ImVec2(150, 0));
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Double-click to rename");
                if (ImGui::IsMouseDoubleClicked(0)) {
                    m_renamingIndex = i;
                    strncpy(m_renameBuffer, m_states[i].name.c_str(), sizeof(m_renameBuffer) - 1);
                    m_renameBuffer[sizeof(m_renameBuffer) - 1] = '\0';
                    ImGui::SetKeyboardFocusHere(-1);
                }
            }
        }

        ImGui::SameLine();
        if (!gameRunning) { ImGui::BeginDisabled(); }
        if (ImGui::Button("Load")) {
            applyEncodedState(m_states[i].encoded, m_states[i].name);
        }
        if (!gameRunning) { ImGui::EndDisabled(); }

        ImGui::SameLine();
        if (ImGui::Button("Copy")) {
            ImGui::SetClipboardText(m_states[i].encoded.c_str());
            m_statusMsg = fmt::format("'{}' copied to clipboard.", m_states[i].name);
        }

        ImGui::SameLine();
        if (ImGui::Button("Del")) {
            toDelete = i;
        }

        ImGui::PopID();
    }

    if (toDelete >= 0) {
        if (m_renamingIndex == toDelete) { m_renamingIndex = -1; }
        m_states.erase(m_states.begin() + toDelete);
        saveStatesFile();
    }

    ImGui::EndChild();

    // Toolbar
    if (!gameRunning) { ImGui::BeginDisabled(); }
    if (ImGui::Button("Save")) {
        SavedStateEntry entry;
        entry.name    = fmt::format("State {}", m_states.size() + 1);
        entry.encoded = encodeCurrentState();
        m_states.push_back(std::move(entry));
        saveStatesFile();
        m_statusMsg = fmt::format("Saved as '{}'.", m_states.back().name);
    }
    if (!gameRunning) { ImGui::EndDisabled(); }

    ImGui::SameLine();
    if (ImGui::Button("Import Clipboard")) {
        const char* clip = ImGui::GetClipboardText();
        if (!clip || clip[0] == '\0') {
            m_statusMsg = "Clipboard is empty.";
        } else {
            std::string clipStr = clip;
            if (!ValidateEncodedState(clipStr)) {
                m_statusMsg = "Clipboard does not contain a valid state.";
            } else {
                SavedStateEntry entry;
                entry.name    = fmt::format("Imported {}", m_states.size() + 1);
                entry.encoded = std::move(clipStr);
                m_states.push_back(std::move(entry));
                saveStatesFile();
                m_statusMsg = fmt::format("Imported as '{}'.", m_states.back().name);
            }
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Load Pack")) {
        static constexpr SDL_DialogFileFilter filter = {"State pack", "json"};
        ShowFileSelect(&onMergeFileSelected, this, aurora::window::get_sdl_window(), &filter, 1, nullptr, false);
    }

    if (!m_states.empty()) {
        ImGui::SameLine();
        if (ImGui::Button("Clear All")) {
            ImGui::OpenPopup("##clearall");
        }

        if (ImGui::BeginPopup("##clearall")) {
            ImGui::Text("Delete all saved states?");
            ImGui::Spacing();
            if (ImGui::Button("Yes, clear all")) {
                m_states.clear();
                m_renamingIndex = -1;
                saveStatesFile();
                m_statusMsg = "All states cleared.";
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    if (!m_statusMsg.empty()) {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextWrapped("%s", m_statusMsg.c_str());
    }

    ImGui::End();
}

void ImGuiMenuTools::ShowStateShare() {
    if (!ImGuiConsole::CheckMenuViewToggle(ImGuiKey_F8, m_showStateShare)) {
        return;
    }
    m_stateShare.draw(m_showStateShare);
}

}
