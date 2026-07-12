#include "logs_window.hpp"

#include <array>
#include <ctime>

#include <SDL3/SDL_timer.h>
#include <fmt/format.h>

#include "pane.hpp"

namespace dusk::ui {
namespace {

const char* level_name(LogLevel level) {
    switch (level) {
    case LOG_LEVEL_TRACE:
        return "Trace";
    case LOG_LEVEL_DEBUG:
        return "Debug";
    case LOG_LEVEL_INFO:
        return "Info";
    case LOG_LEVEL_WARN:
        return "Warn";
    case LOG_LEVEL_ERROR:
        return "Error";
    }
    return "?";
}

const char* level_logger_name(LogLevel level) {
    switch (level) {
    case LOG_LEVEL_TRACE:
        return "TRACE";
    case LOG_LEVEL_DEBUG:
        return "DEBUG";
    case LOG_LEVEL_INFO:
        return "INFO";
    case LOG_LEVEL_WARN:
        return "WARNING";
    case LOG_LEVEL_ERROR:
        return "ERROR";
    }
    return "?";
}

const char* level_class(LogLevel level) {
    switch (level) {
    case LOG_LEVEL_TRACE:
        return "lvl-trace";
    case LOG_LEVEL_DEBUG:
        return "lvl-debug";
    case LOG_LEVEL_INFO:
        return "lvl-info";
    case LOG_LEVEL_WARN:
        return "lvl-warn";
    case LOG_LEVEL_ERROR:
        return "lvl-error";
    }
    return "lvl-info";
}

std::string format_time(int64_t timeMs) {
    const auto seconds = static_cast<std::time_t>(timeMs / 1000);
    std::tm localTime{};
#if _WIN32
    localtime_s(&localTime, &seconds);
#else
    localtime_r(&seconds, &localTime);
#endif
    std::array<char, 16> buffer{};
    std::strftime(buffer.data(), buffer.size(), "%H:%M:%S", &localTime);
    return fmt::format("{}.{:03}", buffer.data(), timeMs % 1000);
}

Rml::Element* append_span(Rml::Element* parent, const char* className, const Rml::String& text) {
    auto* span = append(parent, "span");
    span->SetClass(className, true);
    append_text(span, text);
    return span;
}

}  // namespace

LogsWindow::LogsWindow(std::string modFilter)
    : Window{Props{.tabBar = false, .styleSheets = {"res/rml/logs.rcss"}}},
      mModFilter{std::move(modFilter)} {
    mRoot->SetClass("logs", true);
    set_content([this](Rml::Element* content) { build_content(content); });
}

void LogsWindow::build_content(Rml::Element* content) {
    auto* toolbar = append(content, "div");
    toolbar->SetClass("log-toolbar", true);

    auto* title = append(toolbar, "div");
    title->SetClass("log-title", true);
    title->SetInnerRML("Logs");

    auto* modLabel = append(toolbar, "div");
    modLabel->SetClass("log-title-mod", true);
    modLabel->SetInnerRML(mModFilter.empty() ? "All mods" : fmt::format("{}", escape(mModFilter)));

    append(toolbar, "div")->SetClass("log-toolbar-spacer", true);

    for (const LogLevel level :
        {LOG_LEVEL_TRACE, LOG_LEVEL_DEBUG, LOG_LEVEL_INFO, LOG_LEVEL_WARN, LOG_LEVEL_ERROR})
    {
        add_child<ControlledButton>(toolbar,
            ControlledButton::Props{
                .text = level_name(level),
                .isSelected = [this, level] { return mMinLevel <= level; },
            })
            .on_pressed([this, level] {
                mMinLevel = level;
                rebuild_lines();
            });
    }

    append(toolbar, "div")->SetClass("log-toolbar-spacer", true);

    add_child<Button>(toolbar, "Copy").on_pressed([this] { copy_to_clipboard(); });
    add_child<Button>(toolbar, "Clear").on_pressed([this] {
        mods::log::clear();
        rebuild_lines();
    });

    auto& pane = add_child<Pane>(content, Pane::Type::Uncontrolled);
    pane.root()->SetClass("log-view", true);
    mScrollElem = pane.root();
    mLinesElem = append(pane.root(), "div");
    mLinesElem->SetClass("log-lines", true);
    pane.finalize();

    listen(mScrollElem, Rml::EventId::Scroll, [this](Rml::Event&) {
        const float bottom = mScrollElem->GetScrollHeight() - mScrollElem->GetClientHeight();
        mStickToBottom = mScrollElem->GetScrollTop() >= bottom - 4.0f;
    });

    rebuild_lines();
}

void LogsWindow::update() {
    Window::update();
    if (mLinesElem == nullptr) {
        return;
    }

    const Uint64 perfFreq = SDL_GetPerformanceFrequency();
    const Uint64 now = SDL_GetPerformanceCounter();
    // Limit refreshes to ~8 per second
    const bool refresh =
        perfFreq == 0 || mLastRefresh == 0 ||
        static_cast<double>(now - mLastRefresh) >= 0.125 * static_cast<double>(perfFreq);
    if (refresh) {
        mLastRefresh = now;
        refresh_lines();
    }

    // Applied every frame: layout of freshly appended lines is deferred, so a
    // single post-append scroll would land short of the real bottom.
    if (mStickToBottom && mScrollElem != nullptr) {
        mScrollElem->SetScrollTop(mScrollElem->GetScrollHeight() - mScrollElem->GetClientHeight());
    }

    update_visible_window();
}

// Mark items fully outside the scroll view as `visibility: hidden;`.
// They retain their layout, but stops RmlUi from trying to render them.
void LogsWindow::update_visible_window() {
    const float viewTop = mScrollElem->GetAbsoluteOffset(Rml::BoxArea::Border).y;
    const float viewHeight = mScrollElem->GetClientHeight();
    const int count = mLinesElem->GetNumChildren();
    for (int i = 0; i < count && i < static_cast<int>(mLines.size()); ++i) {
        auto* elem = mLinesElem->GetChild(i);
        const float top = elem->GetAbsoluteOffset(Rml::BoxArea::Border).y - viewTop;
        const bool shown = top + elem->GetOffsetHeight() >= -viewHeight && top <= viewHeight * 2.0f;
        if (shown != mLines[i].shown) {
            mLines[i].shown = shown;
            if (shown) {
                elem->RemoveProperty("visibility");
            } else {
                elem->SetProperty("visibility", "hidden");
            }
        }
    }
}

void LogsWindow::refresh_lines() {
    mScratch.clear();
    const auto [firstSeq, nextSeq] = mods::log::copy_since(mNextSeq, mScratch);
    mNextSeq = nextSeq;

    // Drop displayed lines that fell out of the buffer (ring wrap or clear)
    while (!mLines.empty() && mLines.front().seq < firstSeq) {
        if (auto* first = mLinesElem->GetFirstChild()) {
            mLinesElem->RemoveChild(first);
        }
        mLines.pop_front();
    }

    if (mScratch.empty()) {
        return;
    }
    for (const auto& line : mScratch) {
        if (line.modIndex >= mModIds.size()) {
            mModIds = mods::log::ids();
            break;
        }
    }
    for (const auto& line : mScratch) {
        if (!line_visible(line)) {
            continue;
        }
        append_log_line(line);
        mLines.push_back({.seq = line.seq});
    }
}

void LogsWindow::rebuild_lines() {
    if (mLinesElem == nullptr) {
        return;
    }
    mModIds = mods::log::ids();
    mScratch.clear();
    const auto [_, nextSeq] = mods::log::copy_since(0, mScratch);
    mNextSeq = nextSeq;
    mLines.clear();
    while (auto* child = mLinesElem->GetFirstChild()) {
        mLinesElem->RemoveChild(child);
    }

    for (const auto& line : mScratch) {
        if (!line_visible(line)) {
            continue;
        }
        append_log_line(line);
        mLines.push_back({.seq = line.seq});
    }
    mStickToBottom = true;
}

bool LogsWindow::line_visible(const mods::log::Line& line) const {
    if (line.level < mMinLevel) {
        return false;
    }
    if (mModFilter.empty()) {
        return true;
    }
    return line.modIndex < mModIds.size() && mModIds[line.modIndex] == mModFilter;
}

Rml::Element* LogsWindow::append_log_line(const mods::log::Line& line) {
    std::string_view modId;
    if (line.source == mods::log::Source::Loader) {
        modId = "loader";
    } else if (line.modIndex < mModIds.size()) {
        modId = std::string_view{mModIds[line.modIndex]};
    } else {
        modId = "?";
    }

    auto* elem = append(mLinesElem, "div");
    elem->SetClass("log-line", true);
    elem->SetClass(level_class(line.level), true);

    constexpr const char* kNbsp = "\xc2\xa0";
    append_span(elem, "log-time", format_time(line.timeMs));
    append_text(elem, kNbsp);
    append_span(elem, "log-mod", fmt::format("[{}]", modId));
    append_text(elem, kNbsp);
    append_span(elem, "log-msg", line.message);

    return elem;
}

void LogsWindow::copy_to_clipboard() {
    mModIds = mods::log::ids();
    std::vector<mods::log::Line> lines;
    mods::log::copy_since(0, lines);

    std::string text;
    for (const auto& line : lines) {
        if (!line_visible(line)) {
            continue;
        }
        const std::string_view modId =
            line.modIndex < mModIds.size() ? std::string_view{mModIds[line.modIndex]} : "?";
        text += fmt::format("{} [{}] [{}] {}\n", format_time(line.timeMs),
            level_logger_name(line.level), modId, line.message);
    }
    Rml::GetSystemInterface()->SetClipboardText(text);
    push_toast({.content = "Copied to clipboard", .duration = std::chrono::seconds(2)});
}

}  // namespace dusk::ui
