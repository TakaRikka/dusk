#include "ImGuiConsole.hpp"
#include "ImGuiMenuTools.hpp"
#include <cmath>
#include "JSystem/JAudio2/JAISeq.h"
#include "JSystem/JAudio2/JAISeMgr.h"
#include "JSystem/JAudio2/JAISeqMgr.h"
#include "JSystem/JAudio2/JAIStreamMgr.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JASDSPChannel.h"
#include "JSystem/JAudio2/JASDSPInterface.h"
#include "JSystem/JAudio2/JASTrack.h"
#include "dusk/audio/DuskAudioSystem.h"
#include "dusk/audio/DuskDsp.hpp"

static std::array<u8, DSP_CHANNELS> channelSortIndices = {};
static std::array<u32, DSP_CHANNELS> lastResetCounts = {};

static bool sortUpdateCount = true;

static void DrawDirectionGauge(float pan, float dolby) {
    constexpr float R    = 20.0f;
    constexpr float SIZE = R * 2.0f + 4.0f;

    ImVec2 origin = ImGui::GetCursorScreenPos();
    ImGui::Dummy(ImVec2(SIZE, SIZE));
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 c = ImVec2(origin.x + SIZE * 0.5f, origin.y + SIZE * 0.5f);

    dl->AddCircle(c, R, IM_COL32(90, 90, 90, 255), 32);

    float dx = (pan - 0.5f) * 2.0f;
    float dy = dolby * 2.0f - 1.0f;
    float len = sqrtf(dx * dx + dy * dy);
    if (len > 1.0f) { dx /= len; dy /= len; }
    dl->AddLine(c, ImVec2(c.x + dx * R, c.y + dy * R), IM_COL32(255, 200, 50, 255), 1.5f);
}

static void DisplayDspChannel(int i) {
    using namespace dusk::audio;

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

        auto resetCount = GetResetCount(i);
        ImColor color = IM_COL32_WHITE;
        if (lastResetCounts[i] != resetCount) {
            lastResetCounts[i] = resetCount;
            color = IM_COL32(0, 0xFF, 0, 0xFF);
        }
        ImGui::TextColored(color, "Update count: %d, reset count: %ud", jasChannel.mUpdateCounter, resetCount);
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
            auto volume = VolumeFromU16(channel.mAutoMixerVolume);
            auto pitch = channel.mPitch / 4096.0f;
            DrawDirectionGauge(pan, dolby);
            ImGui::SameLine();
            ImGui::Text(
                "pan: %.2f  dolby: %.2f\nfx: %.2f  vol: %.2f  pitch: %.2f",
                pan, dolby, fxMix, volume, pitch);
        } else {
            ImGui::Text(
                "Bus connect: %04X(%.2f),%04X(%.2f),%04X(%.2f),%04X(%.2f),%04X(%.2f),%04X(%.2f)",
                channel.mOutputChannels[0].mBusConnect,
                VolumeFromU16(channel.mOutputChannels[0].mTargetVolume),
                channel.mOutputChannels[1].mBusConnect,
                VolumeFromU16(channel.mOutputChannels[1].mTargetVolume),
                channel.mOutputChannels[2].mBusConnect,
                VolumeFromU16(channel.mOutputChannels[2].mTargetVolume),
                channel.mOutputChannels[3].mBusConnect,
                VolumeFromU16(channel.mOutputChannels[3].mTargetVolume),
                channel.mOutputChannels[4].mBusConnect,
                VolumeFromU16(channel.mOutputChannels[4].mTargetVolume),
                channel.mOutputChannels[5].mBusConnect,
                VolumeFromU16(channel.mOutputChannels[5].mTargetVolume));
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
    std::ranges::stable_sort(
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
    ImGui::Checkbox("Dump channels to disk", &dusk::audio::DumpAudio);
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
            ImGui::SameLine();
            if (ImGui::Button("Resume All")) {
                category->pause(false);
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


static void ShowSeqTracks(JAISeq& seq) {
    JASTrack& root = seq.inner_.outputTrack;

    for (int group = 0; group < 2; group++) {
        JASTrack* groupTrack = root.getChild(group);
        if (groupTrack == nullptr) {
            continue;
        }

        for (int j = 0; j < JASTrack::MAX_CHILDREN; j++) {
            JASTrack* track = groupTrack->getChild(j);
            if (track == nullptr) {
                continue;
            }

            int trackIdx = group * 16 + j;
            char label[64];
            snprintf(label, sizeof(label), "Track %d (bank %hu, prog %hu)##%p",
                trackIdx, track->getBankNumber(), track->getProgNumber(), track);
            bool muted = track->mFlags.mute;
            if (ImGui::Checkbox(label, &muted)) {
                track->mute(muted);
            }
        }
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

    ImGui::Text("Active sequences: %d", mgr.getNumActiveSeqs());

    auto* seqList = mgr.getSeqList();
    for (auto* link = seqList->getFirst(); link != nullptr; link = link->getNext()) {
        JAISeq* seq = link->getObject();
        if (seq == nullptr) {
            continue;
        }

        char buf[32];
        snprintf(buf, sizeof(buf), "%p", seq);

        if (ImGui::BeginChild(buf, ImVec2(), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY)) {
            ImGui::Text("Seq [%p]", seq);
            ShowSeqTracks(*seq);
        }

        ImGui::EndChild();
    }
}

void dusk::ImGuiMenuTools::ShowAudioDebug() {
    if (!ImGuiConsole::CheckMenuViewToggle(ImGuiKey_F10, m_showAudioDebug)) {
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

void dusk::ImGuiMenuTools::ShowSaveEditor() {
    if (!ImGuiConsole::CheckMenuViewToggle(ImGuiKey_F6, m_showSaveEditor)) {
        return;
    }
    m_saveEditor.draw(m_showSaveEditor);
}
