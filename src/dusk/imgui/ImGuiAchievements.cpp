#include "ImGuiAchievements.hpp"
#include "ImGuiConfig.hpp"
#include "dusk/achievements.h"
#include "dusk/settings.h"
#include "fmt/format.h"
#include "imgui.h"

namespace dusk {

void ImGuiAchievements::notify(std::string name) {
    if (m_notifyTimer <= 0.f) {
        m_notifyName = std::move(name);
        m_notifyTimer = NOTIFY_DURATION;
    } else {
        m_notifyQueue.push(std::move(name));
    }
}

void ImGuiAchievements::showNotification() {
    if (!getSettings().game.enableAchievementNotifications.getValue()) {
        return;
    }
    if (m_notifyTimer <= 0.f) {
        if (m_notifyQueue.empty()) {
            return;
        }
        m_notifyName = std::move(m_notifyQueue.front());
        m_notifyQueue.pop();
        m_notifyTimer = NOTIFY_DURATION;
    }

    m_notifyTimer -= ImGui::GetIO().DeltaTime;

    const float alpha = std::min({
        m_notifyTimer / NOTIFY_FADE_TIME,
        (NOTIFY_DURATION - m_notifyTimer) / NOTIFY_FADE_TIME,
        1.0f
    });

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const float padding = 12.0f;
    ImGui::SetNextWindowPos(
        ImVec2(viewport->WorkPos.x + viewport->WorkSize.x - padding, viewport->WorkPos.y + padding),
        ImGuiCond_Always, ImVec2(1.0f, 0.0f)
    );

    ImGui::SetNextWindowBgAlpha(alpha * 0.92f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.06f, 0.01f, alpha * 0.92f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.8f, 0.1f, alpha));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, alpha));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.0f, 10.0f));

    constexpr ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoInputs;

    if (ImGui::Begin("##achievement_notify", nullptr, flags)) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.82f, 0.1f, alpha));
        ImGui::TextUnformatted("Achievement Unlocked!");
        ImGui::PopStyleColor();
        ImGui::Spacing();
        ImGui::TextUnformatted(m_notifyName.c_str());
    }
    ImGui::End();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
}

void ImGuiAchievements::draw(bool& open) {
    showNotification();

    if (!open) {
        return;
    }

    ImGui::SetNextWindowSizeConstraints(ImVec2(640, 200), ImVec2(800, 900));
    ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
    
    if (!ImGui::Begin(
        "Achievements", &open,
        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav)
    )
    {
        ImGui::End();
        return;
    }

    const auto achievements = AchievementSystem::get().getAchievements();

    int unlocked = 0;
    for (const auto& a : achievements) {
        if (a.unlocked) {
            ++unlocked;
        }
    }

    ImGui::Text("%d / %d achievements unlocked", unlocked, (int)achievements.size());
    ImGui::SameLine();
    config::ImGuiCheckbox("Notifications", getSettings().game.enableAchievementNotifications);
    ImGui::Separator();

    static const struct {
        AchievementCategory cat;
        const char* label;
        ImVec4 color;
    } ACHIEVEMENT_CATEGORIES[] = {
        {AchievementCategory::Story, "Story", ImVec4(1.0f, 0.82f, 0.1f, 1.0f)},
        {AchievementCategory::Collection, "Collection", ImVec4(0.3f, 0.85f, 0.4f, 1.0f)},
        {AchievementCategory::Challenge, "Challenge", ImVec4(1.0f, 0.65f, 0.15f, 1.0f)},
        {AchievementCategory::Minigame, "Minigame", ImVec4(0.5f, 0.85f, 1.0f, 1.0f)},
        {AchievementCategory::Misc, "Misc", ImVec4(0.65f, 0.65f, 0.65f, 1.0f)},
        {AchievementCategory::Glitched, "Glitched", ImVec4(0.75f, 0.4f, 1.0f, 1.0f)},
    };

    const float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

    if (ImGui::BeginTabBar("##achievement_tabs", ImGuiTabBarFlags_FittingPolicyScroll)) {
        for (const auto& catInfo : ACHIEVEMENT_CATEGORIES) {
            int catTotal = 0, catUnlocked = 0;
            for (const auto& a : achievements) {
                if (a.category == catInfo.cat) {
                    ++catTotal;
                    if (a.unlocked) {
                        ++catUnlocked;
                    }
                }
            }
            if (catTotal == 0) {
                continue;
            }

            const std::string tabLabel = fmt::format("{} ({}/{})###{}", catInfo.label, catUnlocked, catTotal, catInfo.label);
            
            ImGui::PushStyleColor(ImGuiCol_Text, catInfo.color);
            const bool tabOpen = ImGui::BeginTabItem(tabLabel.c_str());
            ImGui::PopStyleColor();

            if (tabOpen) {
                ImGui::BeginChild(
                    "##cat_list",
                    ImVec2(0, -footerHeight),
                    ImGuiChildFlags_None,
                    ImGuiWindowFlags_NoBackground
                );

                ImGui::Spacing();

                for (const auto& a : achievements) {
                    if (a.category != catInfo.cat) {
                        continue;
                    }
                    ImGui::PushID(a.key);
                    ImGui::BeginGroup();

                    ImGui::PushStyleColor(
                        ImGuiCol_Text,
                        a.unlocked ?
                            ImVec4(1.0f, 0.65f, 0.15f, 1.0f) :
                            ImGui::GetStyleColorVec4(ImGuiCol_Text)
                    );

                    ImGui::TextUnformatted(a.name);
                    ImGui::PopStyleColor();

                    const char* statusLabel = a.unlocked ? "[Unlocked]" : "[Locked]";
                    ImGui::SameLine(
                        ImGui::GetContentRegionMax().x -
                        ImGui::CalcTextSize(statusLabel).x
                    );

                    if (a.unlocked) {
                        ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "%s", statusLabel);
                    } else {
                        ImGui::TextColored(ImVec4(0.8f, 0.2f, 0.2f, 1.0f), "%s", statusLabel);
                    }

                    ImGui::TextDisabled("%s", a.description);

                    if (a.isCounter) {
                        const float fraction = a.goal > 0 ? (float)(a.progress) / (float)(a.goal) : 1.0f;
                        const std::string overlay = fmt::format("{} / {}", a.progress, a.goal);
                        ImGui::PushStyleColor(
                            ImGuiCol_PlotHistogram,
                            a.unlocked ?
                                ImVec4(0.4f, 0.7f, 0.1f, 1.0f) :
                                ImVec4(0.2f, 0.45f, 0.8f, 1.0f)
                        );
                        ImGui::ProgressBar(fraction, ImVec2(-1.0f, 0.0f), overlay.c_str());
                        ImGui::PopStyleColor();
                    }

                    ImGui::EndGroup();

                    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                        ImGui::OpenPopup("##ctx");
                    }

                    if (ImGui::BeginPopup("##ctx")) {
                        ImGui::TextDisabled("%s", a.name);
                        ImGui::Separator();
                        if (ImGui::MenuItem("Clear Achievement")) {
                            AchievementSystem::get().clearOne(a.key);
                        }
                        ImGui::EndPopup();
                    }

                    ImGui::Spacing();
                    ImGui::PopID();
                }

                ImGui::EndChild();
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }

    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Clear All Achievements")) {
        ImGui::OpenPopup("##confirm_clear");
    }

    if (ImGui::BeginPopup("##confirm_clear")) {
        ImGui::Text("Reset all achievement progress?");
        ImGui::Spacing();
        if (ImGui::Button("Yes, reset all")) {
            AchievementSystem::get().clearAll();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

}  // namespace dusk
