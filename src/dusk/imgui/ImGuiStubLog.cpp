#include "ImGuiMenuTools.hpp"

#include "dusk/logging.h"

#include <imgui.h>

#include <mutex>
#include <vector>

namespace dusk {
    static ImGuiTextBuffer StubLogBuffer;
    static std::vector<int> LineOffsets;
    static bool StubLogPaused;
    static std::mutex StubLogMutex;

    void SendToStubLog(
        borealis::LogLevel level, std::string_view module, std::string_view message) {
        if (StubLogPaused) {
            return;
        }

        std::lock_guard lock(StubLogMutex);

        if (StubLogBuffer.size() > 1024 * 1024) {
            DuskLog.warn("Stub log FULL. Dropping logs!");
            return;
        }

        LineOffsets.push_back(StubLogBuffer.size());
        const auto levelName = borealis::to_string(level);
        StubLogBuffer.appendf("[%.*s | %.*s] %.*s\n", static_cast<int>(levelName.size()),
            levelName.data(), static_cast<int>(module.size()), module.data(),
            static_cast<int>(message.size()), message.data());
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
