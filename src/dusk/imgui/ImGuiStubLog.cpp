#include <vector>
#include <mutex>

#include "dusk/logging.h"
#include "imgui.h"
#include "ImGuiMenuTools.hpp"

namespace dusk {
    static ImGuiTextBuffer StubLogBuffer;
    static std::vector<int> LineOffsets;
    static bool StubLogPaused;
    static std::mutex StubLogMutex;

    const char* LogLevelName(const AuroraLogLevel level) {
        switch (level) {
        case LOG_DEBUG:
            return "DEBUG";
        case LOG_INFO:
            return "INFO";
        case LOG_WARNING:
            return "WARNING";
        case LOG_ERROR:
            return "ERROR";
        case LOG_FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
        }
    }

    void SendToStubLog(AuroraLogLevel level, const char* module, const char* message) {
        if (StubLogPaused) {
            return;
        }

        std::lock_guard lock(StubLogMutex);

        if (StubLogBuffer.size() > 1024 * 1024) {
            DuskLog.warn("Stub log FULL. Dropping logs!");
            return;
        }

        LineOffsets.push_back(StubLogBuffer.size());
        const auto levelName = LogLevelName(level);
        StubLogBuffer.appendf("[%s | %s] %s\n", levelName, module, message);
    }

    void ImGuiMenuTools::ShowStubLog() {
        std::lock_guard lock(StubLogMutex);

        if (!m_showStubLog) {
            return;
        }

        if (ImGui::Begin("Stub log", &m_showStubLog)) {
            ImGui::Checkbox("Redirect stub log", &StubLogEnabled);
            ImGui::SameLine();
            ImGui::Checkbox("Pause", &StubLogPaused);

            ImGui::Text("Line count (this frame): %zu", LineOffsets.size());

            ImGui::Separator();

            if (ImGui::BeginChild("scrolling")) {
                ImGuiListClipper clipper;
                clipper.Begin(static_cast<int>(LineOffsets.size()));
                while (clipper.Step()) {
                    for (int idx = clipper.DisplayStart; idx < clipper.DisplayEnd; idx++) {
                        const char* lineStart = StubLogBuffer.begin() + LineOffsets[idx];
                        const char* lineEnd = idx == LineOffsets.size() - 1 ? StubLogBuffer.end() : StubLogBuffer.begin() + LineOffsets[idx + 1];
                        ImGui::TextUnformatted(lineStart, lineEnd);
                    }
                }

                clipper.End();
            }

            ImGui::EndChild();
        }

        ImGui::End();
    }

    void ClearPastFrame() {
        if (StubLogPaused) {
            return;
        }
        StubLogBuffer.clear();
        LineOffsets.clear();
    }

    void ImGuiMenuTools::afterDraw() {
        std::lock_guard lock(StubLogMutex);

        ClearPastFrame();
    }
}
