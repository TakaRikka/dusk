#include "ImGuiStateShare.hpp"
#include "ImGuiMenuTools.hpp"
#include "ImGuiConsole.hpp"

#include "imgui.h"
#include "fmt/format.h"
#include "absl/strings/escaping.h"

#include "d/d_com_inf_game.h"
#include "dusk/main.h"

#include <zstd.h>

namespace dusk {

#pragma pack(push, 1)
struct StateSharePacket {
    char    stageName[8];
    int8_t  roomNo;
    int8_t  layer;
    int16_t startPoint;
    // followed by raw dSv_info_c bytes
};
#pragma pack(pop)

static constexpr size_t PACKET_TOTAL = sizeof(StateSharePacket) + sizeof(dSv_info_c);

void ImGuiStateShare::copyState() {
    dSv_restart_c& restart = g_dComIfG_gameInfo.info.getRestart();

    StateSharePacket pkt = {};
    if (const char* s = g_dComIfG_gameInfo.play.getLastPlayStageName())
        strncpy(pkt.stageName, s, 7);
    pkt.roomNo     = restart.getRoomNo();
    pkt.layer      = dComIfGp_getStartStageLayer();
    pkt.startPoint = restart.getStartPoint();

    std::string raw(PACKET_TOTAL, '\0');
    memcpy(raw.data(), &pkt, sizeof(pkt));
    memcpy(raw.data() + sizeof(pkt), &g_dComIfG_gameInfo.info, sizeof(dSv_info_c));

    size_t bound = ZSTD_compressBound(raw.size());
    std::string compressed(bound, '\0');
    compressed.resize(ZSTD_compress(compressed.data(), bound, raw.data(), raw.size(), 1));

    std::string encoded = absl::Base64Escape(compressed);
    ImGui::SetClipboardText(encoded.c_str());
    m_statusMsg = "Copied to clipboard.";
}

bool ImGuiStateShare::pasteState() {
    const char* clip = ImGui::GetClipboardText();
    if (!clip || clip[0] == '\0') {
        m_statusMsg = "Clipboard is empty.";
        return false;
    }

    std::string decoded;
    if (!absl::Base64Unescape(clip, &decoded)) {
        m_statusMsg = "Invalid base64.";
        return false;
    }

    unsigned long long dSize = ZSTD_getFrameContentSize(decoded.data(), decoded.size());
    if (dSize == ZSTD_CONTENTSIZE_ERROR || dSize == ZSTD_CONTENTSIZE_UNKNOWN || dSize < PACKET_TOTAL) {
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

    memcpy(&g_dComIfG_gameInfo.info, raw.data() + sizeof(pkt), sizeof(dSv_info_c));
    dComIfGp_setNextStage(pkt.stageName, pkt.startPoint, pkt.roomNo, pkt.layer);
    m_pendingInfo = g_dComIfG_gameInfo.info;

    m_statusMsg = fmt::format("Warping to {} room {} layer {}.", pkt.stageName, (int)pkt.roomNo, (int)pkt.layer);
    return true;
}

void ImGuiStateShare::tickPendingApply() {
    if (!m_pendingInfo.has_value() || dComIfGp_isEnableNextStage())
        return;
    g_dComIfG_gameInfo.info = *m_pendingInfo;
    m_pendingInfo.reset();
}

void ImGuiStateShare::draw(bool& open) {
    if (dusk::IsGameLaunched)
        tickPendingApply();

    if (!open)
        return;

    if (!ImGui::Begin("State Share", &open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav)) {
        ImGui::End();
        return;
    }

    if (!dusk::IsGameLaunched) ImGui::BeginDisabled();
    if (ImGui::Button("Copy State"))    copyState();
    ImGui::SameLine();
    if (ImGui::Button("Import State"))  pasteState();
    if (!dusk::IsGameLaunched) ImGui::EndDisabled();

    if (!m_statusMsg.empty()) {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::TextWrapped("%s", m_statusMsg.c_str());
    }

    ImGui::End();
}

void ImGuiMenuTools::ShowStateShare() {
    if (!ImGuiConsole::CheckMenuViewToggle(ImGuiKey_F8, m_showStateShare))
        return;
    m_stateShare.draw(m_showStateShare);
}

}
