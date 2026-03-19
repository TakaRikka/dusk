#include "ImGuiConsole.hpp"
#include "ImGuiMenuTools.hpp"
#include "JSystem/JAudio2/JAISeMgr.h"
#include "JSystem/JAudio2/JAISeqMgr.h"
#include "JSystem/JAudio2/JAIStreamMgr.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JASDSPChannel.h"
#include "JSystem/JAudio2/JASDSPInterface.h"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "JSystem/JAudio2/JASTrack.h"

static std::array<u8, DSP_CHANNELS> channelSortIndices = {};

static bool sortUpdateCount = true;

static void DisplayDspChannel(int i) {
    auto& channel = JASDsp::CH_BUF[i];
    auto& jasChannel = JASDSPChannel::sDspChannels[i];
    if (!channel.mIsActive) {
        return;
    }

    char buf[64];
    snprintf(buf, sizeof(buf), "%d", i);

    if (ImGui::BeginChild(buf, ImVec2(), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
        ImGui::Text("[%02X]", i);
        ImGui::SameLine();
        ImGui::Text("Update count: %d", jasChannel.mUpdateCounter);
        ImGui::TextUnformatted(channel.mLoopFlag ? "Loop: true" : "Loop: false");
        ImGui::SameLine();
        ImGui::Text("Priority: %hd", jasChannel.mPriority);
        ImGui::Text("Format: %02X/%02X", channel.mSamplesPerBlock, channel.mBytesPerBlock);
        ImGui::Text("Position: %08X/%08X", channel.mSamplePosition, channel.mEndSample);
        ImGui::SameLine();
        ImGui::Text("Memory: %08X/%08X", channel.mWaveAramAddress, channel.mAramStreamPosition);

        if (channel.mAutoMixerBeenSet) {
            auto pan = (channel.mAutoMixerPanDolby >> 8) / 127.5f;
            auto dolby = (channel.mAutoMixerPanDolby & 0xFF) / 127.5f;
            auto fxMix = (channel.mAutoMixerFxMix >> 8) / 127.5f;
            auto volume = (channel.mAutoMixerVolume) / (f32) JASDriver::getChannelLevel_dsp();
            ImGui::Text(
                "Auto mixer active (pan: %f, dolby: %f, fx: %f, volume: %f)",
                pan, dolby, fxMix, volume);
        } else {
            ImGui::Text(
                "Bus connect: %04X,%04X,%04X,%04X,%04X,%04X",
                channel.mOutputChannels[0].mBusConnect,
                channel.mOutputChannels[1].mBusConnect,
                channel.mOutputChannels[2].mBusConnect,
                channel.mOutputChannels[3].mBusConnect,
                channel.mOutputChannels[4].mBusConnect,
                channel.mOutputChannels[5].mBusConnect);
        }
    }

    ImGui::EndChild();
}

static void InitChannelSortIndices() {
    for (int i = 0; i < channelSortIndices.size(); i++) {
        channelSortIndices[i] = i;
    }
}

static void SortChannelsByUpdateCount() {
    InitChannelSortIndices();
    std::ranges::sort(
        channelSortIndices,
        [](u8 a, u8 b) {
            auto& jasChannelA = JASDSPChannel::sDspChannels[a];
            auto& jasChannelB = JASDSPChannel::sDspChannels[b];

            return jasChannelA.mUpdateCounter > jasChannelB.mUpdateCounter;
    });
}

static void ShowAllDspChannels() {
    int activeChannels = 0;
    for (int i = 0; i < DSP_CHANNELS; i++) {
        if (JASDsp::CH_BUF[i].mIsActive) {
            activeChannels++;
        }
    }

    ImGui::Text("Active channels: %d", activeChannels);
    ImGui::Checkbox("Sort by update count", &sortUpdateCount);

    if (sortUpdateCount) {
        SortChannelsByUpdateCount();
        for (u8 index : channelSortIndices) {
            DisplayDspChannel(index);
        }
    } else {
        for (int i = 0; i < DSP_CHANNELS; i++) {
            DisplayDspChannel(i);
        }
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
            bool muted = track.mFlags.mute;
            ImGui::Checkbox("Muted", &muted);
            track.mFlags.mute = muted;

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


static void ShowAllJAISeqs() {
    auto& mgr = *JAISeqMgr::getInstance();

    if (ImGui::Button("Pause")) {
        mgr.pause(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Unpause")) {
        mgr.pause(false);
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

            if (ImGui::BeginTabItem("JAISeq")) {
                ShowAllJAISeqs();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }

    ImGui::End();
}