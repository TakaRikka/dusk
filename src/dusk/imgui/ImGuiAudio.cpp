#include "ImGuiConsole.hpp"
#include "ImGuiMenuTools.hpp"
#include "JSystem/JAudio2/JAISeMgr.h"
#include "JSystem/JAudio2/JAIStreamMgr.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JASDSPInterface.h"
#include "JSystem/JAudio2/JASTrack.h"

static void ShowAllDspChannels() {
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
            ImGui::Text("Position: %08X/%08X", channel.mSamplePosition, channel.mEndSample);
            ImGui::Text("Memory: %08X/%08X", channel.mWaveAramAddress, channel.mAramStreamPosition);
        }

        ImGui::EndChild();
    }
}

static void ShowAllTracks() {
    if (ImGui::Button("Pause all")) {
        for (auto& track : JASTrack::sTrackList) {
            track.pause(true);
        }
    }

    for (auto& track : JASTrack::sTrackList) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%p", &track);

        if (ImGui::BeginChild(buf, ImVec2(), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
            ImGui::Text("[%p]", &track);
            bool paused = track.mFlags.pause;
            ImGui::Checkbox("Paused", &paused);
            track.mFlags.pause = paused;

            for (int i = 0; i < JASTrack::MAX_CHILDREN; i++) {
                const auto child = track.getChild(i);
                if (child != nullptr) {
                    ImGui::Text("child: [%p]", child);
                }
            }
        }

        ImGui::EndChild();
    }
}

static void ShowAllJAIStreams() {
    auto& mgr = *JAIStreamMgr::getInstance();

    for (auto streamLink = mgr.getStreamList()->getFirst(); streamLink != nullptr; streamLink = streamLink->getNext()) {
        auto& stream = *streamLink->getObject();
        char buf[32];
        snprintf(buf, sizeof(buf), "%p", &stream);

        if (ImGui::BeginChild(buf, ImVec2(), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
            ImGui::Text("[%p]", &stream);
            bool paused = stream.status_.field_0x0.flags.paused;
            ImGui::Checkbox("Paused", &paused);
            stream.status_.field_0x0.flags.paused = paused;
        }

        ImGui::EndChild();
    }
}

static void ShowAllJAISes() {
    auto& mgr = *JAISeMgr::getInstance();

    for (int i = 0; i < JAISeMgr::NUM_CATEGORIES; i++) {
        const auto category = mgr.getCategory(i);

        char buf[32];
        snprintf(buf, sizeof(buf), "%i", i);

        if (ImGui::BeginChild(buf, ImVec2(), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
            ImGui::Text("Category: %i", i);
            if (ImGui::Button("Pause All")) {
                category->pause(true);
            }

            for (auto seLink = category->getSeList()->getFirst(); seLink != nullptr; seLink = seLink->getNext()) {
                const auto se = seLink->getObject();
                ImGui::Text("[%p]", se);
                ImGui::Text(se->status_.field_0x0.flags.paused ? "Paused" : "Not paused");
            }
        }

        ImGui::EndChild();
    }
}

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

        if (ImGui::BeginTabBar("Tabs")) {
            if (ImGui::BeginTabItem("DSP channels")) {
                ShowAllDspChannels();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("JAITrack")) {
                ShowAllTracks();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("JAIStream")) {
                ShowAllJAIStreams();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("JAISe")) {
                ShowAllJAISes();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }

    ImGui::End();
}