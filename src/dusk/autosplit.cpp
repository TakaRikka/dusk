#include "dusk/autosplit.h"

#include "dusk/io.hpp"
#include "dusk/livesplit.h"
#include "dusk/main.h"
#include "dusk/settings.h"

#include "d/d_com_inf_game.h"
#include "d/d_item_data.h"
#include "d/d_save.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_player.h"

#include "nlohmann/json.hpp"

#include <filesystem>

namespace dusk::autosplit {
namespace {

using json = nlohmann::json;

constexpr auto kFileName = "autosplits.json";

std::string xml_unescape(std::string_view s) {
    std::string out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size();) {
        if (s[i] == '&') {
            if (s.compare(i, 5, "&amp;") == 0)  { out += '&';  i += 5; continue; }
            if (s.compare(i, 4, "&lt;") == 0)   { out += '<';  i += 4; continue; }
            if (s.compare(i, 4, "&gt;") == 0)   { out += '>';  i += 4; continue; }
            if (s.compare(i, 6, "&quot;") == 0) { out += '"';  i += 6; continue; }
            if (s.compare(i, 6, "&apos;") == 0) { out += '\''; i += 6; continue; }
        }
        out += s[i++];
    }
    return out;
}

std::string extract_name(std::string_view block) {
    constexpr std::string_view kCdataOpen  = "<![CDATA[";
    constexpr std::string_view kCdataClose = "]]>";
    const auto open = block.find(kCdataOpen);
    if (open != std::string_view::npos) {
        const auto start = open + kCdataOpen.size();
        const auto end = block.find(kCdataClose, start);
        if (end != std::string_view::npos) {
            return std::string(block.substr(start, end - start));
        }
    }
    return xml_unescape(block);
}

std::vector<std::string> parse_lss_segment_names(std::string_view xml) {
    constexpr std::string_view kSegmentsOpen  = "<Segments>";
    constexpr std::string_view kSegmentsClose = "</Segments>";
    constexpr std::string_view kSegmentOpen   = "<Segment>";
    constexpr std::string_view kSegmentClose  = "</Segment>";
    constexpr std::string_view kNameOpen      = "<Name>";
    constexpr std::string_view kNameClose     = "</Name>";

    std::vector<std::string> names;

    const auto segsStart = xml.find(kSegmentsOpen);
    if (segsStart == std::string_view::npos) return names;
    const auto segsEnd = xml.find(kSegmentsClose, segsStart);
    if (segsEnd == std::string_view::npos) return names;

    const auto body = xml.substr(
        segsStart + kSegmentsOpen.size(),
        segsEnd - segsStart - kSegmentsOpen.size());

    for (std::size_t i = 0;;) {
        const auto sStart = body.find(kSegmentOpen, i);
        if (sStart == std::string_view::npos) break;
        const auto sEnd = body.find(kSegmentClose, sStart);
        if (sEnd == std::string_view::npos) break;
        const auto seg = body.substr(
            sStart + kSegmentOpen.size(),
            sEnd - sStart - kSegmentOpen.size());
        const auto nStart = seg.find(kNameOpen);
        if (nStart != std::string_view::npos) {
            const auto nEnd = seg.find(kNameClose, nStart);
            if (nEnd != std::string_view::npos) {
                names.push_back(extract_name(seg.substr(
                    nStart + kNameOpen.size(),
                    nEnd - nStart - kNameOpen.size())));
            }
        }
        i = sEnd + kSegmentClose.size();
    }
    return names;
}

}  // namespace

Manager& Manager::get() {
    static Manager instance;
    return instance;
}

Manager::Manager() {
    mCatalog = {
        { "iron_boots", "Iron Boots",
            [] { return dComIfGs_isItemFirstBit(dItemNo_HVY_BOOTS_e) != 0; } },
        { "forest_temple", "Forest Temple Cleared",
            [] { return dComIfGs_isEventBit(dSv_event_flag_c::M_022) != 0; } },
        { "ganondorf", "Ganondorf Defeated",
            [] {
                const auto* link = static_cast<const daAlink_c*>(daPy_getPlayerActorClass());
                return link != nullptr && link->mProcID == daAlink_c::PROC_GANON_FINISH;
            } },
    };
}

const SplitEvent* Manager::findEvent(std::string_view id) const {
    for (const auto& ev : mCatalog) {
        if (id == ev.id) return &ev;
    }
    return nullptr;
}

void Manager::load() {
    if (mLoaded) return;
    mLoaded = true;

    const auto path = ConfigPath / kFileName;
    std::error_code ec;
    if (!std::filesystem::exists(path, ec)) return;

    try {
        auto bytes = io::FileStream::ReadAllBytes(path);
        auto j = json::parse(bytes);
        if (!j.is_object()) return;

        if (j.contains("lssPath") && j["lssPath"].is_string()) {
            mLssPath = j["lssPath"].get<std::string>();
        }
        if (j.contains("splits") && j["splits"].is_array()) {
            for (const auto& entry : j["splits"]) {
                if (!entry.is_object() || !entry.contains("name")) continue;
                MappedSplit s;
                s.name = entry["name"].get<std::string>();
                if (entry.contains("eventId") && entry["eventId"].is_string()) {
                    auto id = entry["eventId"].get<std::string>();
                    if (findEvent(id) != nullptr) s.eventId = std::move(id);
                }
                mSplits.push_back(std::move(s));
            }
        }
    } catch (const std::exception&) {}
}

void Manager::save() {
    json j;
    j["lssPath"] = mLssPath;
    auto& arr = j["splits"] = json::array();
    for (const auto& s : mSplits) {
        arr.push_back({ {"name", s.name}, {"eventId", s.eventId} });
    }
    try {
        io::FileStream::WriteAllText(ConfigPath / kFileName, j.dump(2));
    } catch (const std::exception&) {}
}

void Manager::captureSnapshot() {
    mSnapshot.assign(mSplits.size(), false);
    for (std::size_t i = 0; i < mSplits.size(); ++i) {
        if (const auto* ev = findEvent(mSplits[i].eventId); ev != nullptr) {
            mSnapshot[i] = ev->check();
        }
    }
}

void Manager::onSpeedrunStart() {
    load();
    mCursor = 0;
    captureSnapshot();
    mRunActive = true;
}

void Manager::onSpeedrunReset() {
    mRunActive = false;
    mCursor = 0;
    mSnapshot.clear();
}

void Manager::tick() {
    load();
    if (!IsGameLaunched || !mRunActive) return;

    const auto& s = getSettings().game;
    if (!s.speedrunMode.getValue() ||
        !s.liveSplitEnabled.getValue() ||
        !s.autosplitEnabled.getValue()) {
        return;
    }
    if (!speedrun::isConnected()) return;

    while (mCursor < mSplits.size()) {
        const auto& split = mSplits[mCursor];
        const auto* ev = split.eventId.empty() ? nullptr : findEvent(split.eventId);
        if (ev == nullptr) return;

        const bool now = ev->check();
        if (now && !mSnapshot[mCursor]) {
            speedrun::split();
            mSnapshot[mCursor] = true;
            ++mCursor;
            continue;
        }
        return;
    }
}

ImportResult Manager::importFromLss(const std::string& path) {
    load();

    std::error_code ec;
    if (!std::filesystem::exists(path, ec)) return ImportResult::FileNotFound;

    std::vector<u8> bytes;
    try {
        bytes = io::FileStream::ReadAllBytes(path.c_str());
    } catch (const std::exception&) {
        return ImportResult::ReadError;
    }

    const std::string_view xml(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    auto names = parse_lss_segment_names(xml);
    if (names.empty()) return ImportResult::NoSegments;

    std::vector<MappedSplit> next;
    next.reserve(names.size());
    for (std::size_t i = 0; i < names.size(); ++i) {
        MappedSplit s;
        s.name = std::move(names[i]);
        if (i < mSplits.size() && mSplits[i].name == s.name) {
            s.eventId = mSplits[i].eventId;
        }
        next.push_back(std::move(s));
    }

    mSplits = std::move(next);
    mLssPath = path;
    mCursor = 0;
    mSnapshot.clear();
    mRunActive = false;
    save();
    return ImportResult::Success;
}

void Manager::setEventForSplit(std::size_t index, std::string eventId) {
    load();
    if (index >= mSplits.size()) return;
    if (!eventId.empty() && findEvent(eventId) == nullptr) return;
    mSplits[index].eventId = std::move(eventId);
    save();
}

void Manager::clearSplits() {
    load();
    mSplits.clear();
    mLssPath.clear();
    mCursor = 0;
    mSnapshot.clear();
    mRunActive = false;
    save();
}

}  // namespace dusk::autosplit
