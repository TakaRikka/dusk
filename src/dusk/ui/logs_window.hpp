#pragma once

#include "window.hpp"

#include <deque>
#include <string>
#include <vector>

#include "dusk/mods/log_buffer.hpp"

namespace dusk::ui {

class LogsWindow : public Window {
public:
    explicit LogsWindow(std::string modFilter = {});
    void update() override;

private:
    struct DisplayLine {
        uint64_t seq = 0;
        bool shown = true;
    };

    void build_content(Rml::Element* content);
    void rebuild_lines();
    void refresh_lines();
    void update_visible_window();
    bool line_visible(const mods::log::Line& line) const;
    Rml::Element* append_log_line(const mods::log::Line& line);
    void copy_to_clipboard();

    std::string mModFilter;
    LogLevel mMinLevel = LOG_LEVEL_DEBUG;
    std::vector<std::string> mModIds;
    uint64_t mNextSeq = 0;
    std::vector<mods::log::Line> mScratch;
    std::deque<DisplayLine> mLines;
    Rml::Element* mLinesElem = nullptr;
    Rml::Element* mScrollElem = nullptr;
    bool mStickToBottom = true;
    Uint64 mLastRefresh = 0;
};

}  // namespace dusk::ui
