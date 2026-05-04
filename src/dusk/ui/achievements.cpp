#include "achievements.hpp"

#include "Z2AudioLib/Z2SeMgr.h"
#include "dusk/achievements.h"
#include "fmt/format.h"
#include "m_Do/m_Do_audio.h"
#include "nav_types.hpp"
#include "pane.hpp"

namespace dusk::ui {
namespace {

struct CategoryInfo {
    AchievementCategory cat;
    const char* label;
};

constexpr CategoryInfo kCategories[] = {
    {AchievementCategory::Story,      "Story"},
    {AchievementCategory::Collection, "Collection"},
    {AchievementCategory::Challenge,  "Challenge"},
    {AchievementCategory::Minigame,   "Minigame"},
    {AchievementCategory::Misc,       "Misc"},
    {AchievementCategory::Glitched,   "Glitched"},
};

Rml::String build_achievement_info_rml(const Achievement& a) {
    Rml::String s = fmt::format(
        R"(<div class="achievement-header">)"
        R"(<span class="achievement-name{}">{}</span>)"
        R"(<span class="achievement-badge{}">{}</span>)"
        R"(</div>)"
        R"(<p class="achievement-desc">{}</p>)",
        a.unlocked ? " unlocked" : "",
        a.name,
        a.unlocked ? " unlocked" : " locked",
        a.unlocked ? "Unlocked" : "Locked",
        a.description
    );

    if (a.isCounter) {
        float fraction = a.goal > 0 ? float(a.progress) / float(a.goal) : 1.0f;
        s += fmt::format(
            R"(<progressbar value="{:.3f}" class="{}"/>)"
            R"(<span class="achievement-progress">{} / {}</span>)",
            fraction,
            a.unlocked ? "progress-done" : "progress-ongoing",
            a.progress,
            a.goal
        );
    }

    return s;
}

class AchievementRow : public FluentComponent<AchievementRow> {
public:
    AchievementRow(Rml::Element* parent, const Achievement& a)
        : FluentComponent(createRowRoot(parent))
    {
        auto& btn = add_child<Button>(Button::Props{"×"});
        mClearButton = &btn;
        btn.root()->SetClass("achievement-clear", true);

        btn.on_nav_command([this, key = std::string(a.key)](Rml::Event&, NavCommand cmd) {
            if (cmd == NavCommand::Confirm) {
                if (mConfirming) {
                    mDoAud_seStartMenu(Z2SE_SY_CURSOR_OK);
                    AchievementSystem::get().clearOne(key.c_str());
                    resetConfirm();
                } else {
                    mConfirming = true;
                    mClearButton->set_text("Clear?");
                }
                return true;
            }
            if (cmd == NavCommand::Cancel && mConfirming) {
                resetConfirm();
                return true;
            }
            return false;
        });

        Component::listen(btn.root(), Rml::EventId::Blur, [this](Rml::Event&) {
            resetConfirm();
        });

        auto* infoDiv = append(mRoot, "div");
        infoDiv->SetClass("achievement-info", true);
        infoDiv->SetInnerRML(build_achievement_info_rml(a));
    }

    bool focus() override { return mClearButton->focus(); }

private:
    static Rml::Element* createRowRoot(Rml::Element* parent) {
        auto* doc = parent->GetOwnerDocument();
        auto elem = doc->CreateElement("div");
        elem->SetClass("achievement-row", true);
        return parent->AppendChild(std::move(elem));
    }

    void resetConfirm() {
        mConfirming = false;
        mClearButton->set_text("×");
    }

    Button* mClearButton = nullptr;
    bool mConfirming = false;
};

}  // namespace

AchievementsWindow::AchievementsWindow() {
    const auto all = AchievementSystem::get().getAchievements();

    for (const auto& catInfo : kCategories) {
        int catTotal = 0;
        for (const auto& a : all) {
            if (a.category == catInfo.cat) {
                ++catTotal;
            }
        }
        if (catTotal == 0) {
            continue;
        }

        add_tab(catInfo.label, [this, cat = catInfo.cat](Rml::Element* content) {
            const auto achievements = AchievementSystem::get().getAchievements();

            int total = 0, unlocked = 0;
            for (const auto& a : achievements) {
                if (a.category == cat) {
                    ++total;
                    if (a.unlocked) {
                        ++unlocked;
                    }
                }
            }

            auto& pane = add_child<Pane>(content, Pane::Type::Controlled);

            pane.add_section(fmt::format("{} / {} unlocked", unlocked, total));

            for (const auto& a : achievements) {
                if (a.category != cat) {
                    continue;
                }
                pane.add_child<AchievementRow>(a);
            }

            pane.add_section("Actions");

            auto& clearAllBtn = pane.add_button("Clear All Achievements");
            auto* clearAllPtr = &clearAllBtn;
            auto confirmingAll = std::make_shared<bool>(false);

            clearAllBtn.on_nav_command([clearAllPtr, confirmingAll](Rml::Event&, NavCommand cmd) {
                if (cmd == NavCommand::Confirm) {
                    if (*confirmingAll) {
                        mDoAud_seStartMenu(Z2SE_SY_CURSOR_OK);
                        AchievementSystem::get().clearAll();
                        *confirmingAll = false;
                        clearAllPtr->set_text("Clear All Achievements");
                    } else {
                        *confirmingAll = true;
                        clearAllPtr->set_text("Are you sure?");
                    }
                    return true;
                }
                if (cmd == NavCommand::Cancel && *confirmingAll) {
                    *confirmingAll = false;
                    clearAllPtr->set_text("Clear All Achievements");
                    return true;
                }
                return false;
            });
            clearAllBtn.listen(Rml::EventId::Blur, [clearAllPtr, confirmingAll](Rml::Event&) {
                *confirmingAll = false;
                clearAllPtr->set_text("Clear All Achievements");
            });

            pane.finalize();
        });
    }
}

void AchievementsWindow::update() {
    const auto current = AchievementSystem::get().getAchievements();
    bool dirty = current.size() != mSnapshot.size();
    if (!dirty) {
        for (size_t i = 0; i < current.size(); ++i) {
            if (current[i].progress != mSnapshot[i].progress ||
                current[i].unlocked != mSnapshot[i].unlocked)
            {
                dirty = true;
                break;
            }
        }
    }
    if (dirty) {
        mSnapshot = current;
        refresh_active_tab();
    }
    Window::update();
}

}  // namespace dusk::ui
