#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace dusk::autosplit {

struct SplitEvent {
    const char* id;
    const char* displayName;
    std::function<bool()> check;
};

struct MappedSplit {
    std::string name;
    std::string eventId;
};

enum class ImportResult {
    Success,
    FileNotFound,
    ReadError,
    NoSegments,
};

class Manager {
public:
    static Manager& get();

    void tick();
    void onSpeedrunStart();
    void onSpeedrunReset();

    const std::vector<SplitEvent>& catalog() const noexcept { return mCatalog; }
    const std::vector<MappedSplit>& splits() const noexcept { return mSplits; }
    const std::string& lssPath() const noexcept { return mLssPath; }

    const SplitEvent* findEvent(std::string_view id) const;

    ImportResult importFromLss(const std::string& path);
    void setEventForSplit(std::size_t index, std::string eventId);
    void clearSplits();

private:
    Manager();
    void load();
    void save();
    void captureSnapshot();

    std::vector<SplitEvent>  mCatalog;
    std::vector<MappedSplit> mSplits;
    std::vector<bool>        mSnapshot;
    std::string              mLssPath;
    std::size_t mCursor = 0;
    bool        mLoaded = false;
    bool        mRunActive = false;
};

}  // namespace dusk::autosplit
