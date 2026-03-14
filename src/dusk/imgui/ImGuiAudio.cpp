#include "ImGuiConsole.hpp"
#include "ImGuiMenuTools.hpp"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JASDSPInterface.h"

void dusk::ImGuiMenuTools::ShowAudioDebug() {
    if (!ImGuiConsole::CheckMenuViewToggle(ImGuiKey_F7, m_showAudioDebug)) {
        return;
    }

    if (!ImGui::Begin("Audio Debug", &m_showAudioDebug)) {
        ImGui::End();
        return;
    }

    {
        JASCriticalSection cs;
        for (int i = 0; i < DSP_CHANNELS; i++) {
            auto& channel = JASDsp::CH_BUF[i];
            if (!channel.mIsActive) {
                continue;
            }

            char buf[64];
            snprintf(buf, sizeof(buf), "%d", i);

            if (ImGui::BeginChild(buf, ImVec2(), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
                ImGui::Text("[%02X]", i);
                ImGui::TextUnformatted(channel.mLoopFlag ? "Loop: true" : "Loop: false");
                ImGui::Text("Format: %02X/%02X", channel.mSamplesPerBlock, channel.mBytesPerBlock);
            }

            ImGui::EndChild();
        }
    }

    ImGui::End();
}