#include "overlay.hpp"

#include "aurora/lib/logging.hpp"
#include "dusk/achievements.h"
#include "dusk/action_bindings.h"
#include "controller_config.hpp"
#include "dusk/livesplit.h"
#include "dusk/speedrun.h"
#include "fmt/format.h"
#include "magic_enum.hpp"
#include "window.hpp"

#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_timer.h>
#include <algorithm>
#include <dolphin/pad.h>
#include <m_Do/m_Do_main.h>

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

namespace dusk::ui {
namespace {
aurora::Module Log{"dusk::ui::overlay"};

const Rml::String kDocumentSource = R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/overlay.rcss" />
</head>
<body>
    <fps id="fps" />
    <speedrun-timer id="speedrun-timer">
        <speedrun-rta id="speedrun-rta" />
        <speedrun-igt id="speedrun-igt" />
    </speedrun-timer>
</body>
</rml>
)RML";

constexpr std::array<std::pair<const char*, const char*>, 3> kAutoSaveLayers{{
    {"inner", "res/org-icon-inner.png"},
    {"outer", "res/org-icon-outer.png"},
    {"center", "res/org-icon-center.png"},
}};

constexpr auto kMenuNotificationDuration = std::chrono::milliseconds(2500);

constexpr std::array<const char*, 4> kFpsCorners = {"tl", "tr", "bl", "br"};

Rml::Element* create_toast(Rml::Element* parent, const Toast& toast) {
    if (toast.type == "autosave") {
        auto* logo = append(parent, "logo");
        for (const auto [cls, src] : kAutoSaveLayers) {
            auto* img = append(logo, "img");
            img->SetClass(cls, true);
            img->SetAttribute("src", src);
        }
        return logo;
    }

    auto* elem = append(parent, "toast");
    if (!toast.type.empty()) {
        elem->SetClass(toast.type, true);
    }
    {
        auto* heading = append(elem, "heading");
        if (toast.title.starts_with("<")) {
            heading->SetInnerRML(toast.title);
        } else {
            auto* span = append(heading, "span");
            span->SetInnerRML(toast.title);
        }
        if (toast.type == "achievement") {
            auto* icon = append(heading, "icon");
            icon->SetClass("trophy", true);
            mDoAud_seStartMenu(kSoundAchievementUnlock);
        } else if (toast.type == "controller") {
            auto* icon = append(heading, "icon");
            icon->SetClass("controller", true);
        }
    }
    {
        auto* message = append(elem, "message");
        if (toast.content.starts_with("<")) {
            message->SetInnerRML(toast.content);
        } else {
            auto* span = append(message, "span");
            span->SetInnerRML(toast.content);
        }
    }
    {
        auto* progress = append(elem, "progress");
        progress->SetAttribute("value", 1.f);
    }
    return elem;
}

Rml::Element* create_controller_warning(Rml::Element* parent) {
    auto* elem = append(parent, "toast");
    elem->SetClass("controller-warning", true);

    auto* heading = append(elem, "heading");
    auto* title = append(heading, "span");
    title->SetInnerRML("No controller assigned");
    auto* icon = append(heading, "icon");
    icon->SetClass("warning", true);

    auto* message = append(elem, "message");
    auto* content = append(message, "span");
    content->SetInnerRML("Configure controller port 1 in Settings.");

    return elem;
}

SDL_Gamepad* gamepad_for_port(u32 port) noexcept {
    const s32 index = PADGetIndexForPort(port);
    if (index < 0) {
        return nullptr;
    }
    return PADGetSDLGamepadForIndex(static_cast<u32>(index));
}

Rml::String back_button_name() {
    if (auto* gamepad = gamepad_for_port(PAD_CHAN0)) {
        switch (SDL_GetGamepadType(gamepad)) {
        case SDL_GAMEPAD_TYPE_PS3:
            return "Select";
        case SDL_GAMEPAD_TYPE_PS4:
            return "Share";
        case SDL_GAMEPAD_TYPE_PS5:
            return "Create";
        case SDL_GAMEPAD_TYPE_XBOX360:
            return "Back";
        case SDL_GAMEPAD_TYPE_XBOXONE:
            return "View";
        case SDL_GAMEPAD_TYPE_GAMECUBE:
            return "R + Start";
        default:
            break;
        }
    }
    return "Back";
}

#if defined(TARGET_ANDROID) || (defined(__APPLE__) && TARGET_OS_IOS && !TARGET_OS_MACCATALYST)
constexpr auto kMenuNotificationPrefix = "3-finger tap or";
#else
constexpr auto kMenuNotificationPrefix = "Press <b>F1</b> or";
#endif

Rml::Element* create_menu_notification(Rml::Element* parent) {
    auto* elem = append(parent, "toast");
    elem->SetClass("menu-notification", true);

    // Get name of button for action binding if the action is bound
    Rml::String padButton{};
    SDL_Gamepad* gamepad = gamepad_for_port(PAD_CHAN0);
    if (isActionBound(ActionBinds::OPEN_DUSKLIGHT_MENU, PAD_CHAN0) && gamepad != nullptr) {
        padButton = native_button_name(gamepad,
            getActionBindButton(ActionBinds::OPEN_DUSKLIGHT_MENU, PAD_CHAN0));
    } else {
        padButton = back_button_name();
    }

    auto* message = append(elem, "message");
    auto* row = append(message, "row");
    append(row, "span")->SetInnerRML(kMenuNotificationPrefix);
    auto* icon = append(row, "icon");
    icon->SetClass("controller", true);
    append(row, "span")->SetInnerRML("<b>" + escape(padButton) + "</b>");
    append(row, "span")->SetInnerRML("to open menu");

    return elem;
}

void remove_element(Rml::Element*& elem) noexcept {
    if (elem == nullptr) {
        return;
    }
    if (auto* parent = elem->GetParentNode()) {
        parent->RemoveChild(elem);
    }
    elem = nullptr;
}

}  // namespace

// https://vplesko.com/posts/how_to_implement_an_fps_counter.html
void Overlay::advance_fps_counter(float& outFps, Uint64 perfFreq) {
    if (perfFreq == 0) {
        outFps = 0.f;
        return;
    }

    const Uint64 curr = SDL_GetPerformanceCounter();
    if (!mFpsHavePrevCounter) {
        mFpsPrevCounter = curr;
        mFpsHavePrevCounter = true;
        outFps = 0.f;
        return;
    }

    const Uint64 processingTicks = curr - mFpsPrevCounter;
    mFpsPrevCounter = curr;

    mFpsFrameEvents.push_back({curr, processingTicks});
    mFpsSumTicks += processingTicks;

    while (!mFpsFrameEvents.empty() && mFpsFrameEvents.front().endCounter + perfFreq < curr) {
        mFpsSumTicks -= mFpsFrameEvents.front().processingTicks;
        mFpsFrameEvents.pop_front();
    }

    const auto n = mFpsFrameEvents.size();
    if (n == 0 || mFpsSumTicks == 0) {
        outFps = 0.f;
        return;
    }

    const double avgSeconds =
        static_cast<double>(mFpsSumTicks) / static_cast<double>(n) / static_cast<double>(perfFreq);
    outFps = static_cast<float>(1.0 / avgSeconds);
}

static std::string FormatTime(OSTime ticks) {
    OSCalendarTime t;
    OSTicksToCalendarTime(ticks, &t);
    return fmt::format("{0:02}:{1:02}:{2:02}.{3:03}", t.hour, t.min, t.sec, t.msec);
}

Overlay::Overlay() : Document(kDocumentSource) {
    mFpsCounter = mDocument->GetElementById("fps");
    mSpeedrunTimer = mDocument->GetElementById("speedrun-timer");
    mSpeedrunRta = mDocument->GetElementById("speedrun-rta");
    mSpeedrunIgt = mDocument->GetElementById("speedrun-igt");

    listen(mDocument, Rml::EventId::Focus, [](Rml::Event&) { Log.warn("Overlay received focus"); });
    listen(mDocument, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        if (event.GetTargetElement() == mCurrentToast) {
            if (get_toasts().empty() ||
                clock::now() >= mCurrentToastStartTime + get_toasts().front().duration)
            {
                mCurrentToast->SetPseudoClass("done", true);
            }
        } else if (mControllerWarning != nullptr &&
                   event.GetTargetElement() == mControllerWarning &&
                   !mControllerWarning->HasAttribute("open"))
        {
            mControllerWarning->SetPseudoClass("done", true);
        } else if (mMenuNotification != nullptr && event.GetTargetElement() == mMenuNotification &&
                   !mMenuNotification->HasAttribute("open"))
        {
            mMenuNotification->SetPseudoClass("done", true);
        }
    });
}

void Overlay::show() {
    if (mDocument != nullptr) {
        mDocument->Show(Rml::ModalFlag::None, Rml::FocusFlag::None, Rml::ScrollFlag::None);
    }
}

void Overlay::update() {
    Document::update();
    if (mDocument == nullptr) {
        return;
    }

    if (mFpsCounter != nullptr) {
        if (getSettings().video.enableFpsOverlay.getValue()) {
            const int idx = getSettings().video.fpsOverlayCorner.getValue();
            mFpsCounter->SetAttribute("open", "");
            mFpsCounter->SetAttribute("corner", kFpsCorners[idx]);

            const Uint64 perfFreq = SDL_GetPerformanceFrequency();
            float fps = 0.f;
            advance_fps_counter(fps, perfFreq);

            const Uint64 now = SDL_GetPerformanceCounter();
            // Limit updates to twice per second
            const bool refreshLabel =
                perfFreq == 0 || mFpsLastUpdate == 0 ||
                static_cast<double>(now - mFpsLastUpdate) >= 0.5 * static_cast<double>(perfFreq);
            if (refreshLabel) {
                mFpsLastUpdate = now;
                mFpsCounter->SetInnerRML(escape(fmt::format("{:.0f} FPS", fps)));
            }
        } else {
            mFpsCounter->RemoveAttribute("open");
            mFpsFrameEvents.clear();
            mFpsSumTicks = 0;
            mFpsHavePrevCounter = false;
            mFpsLastUpdate = 0;
        }
    }

#if !(defined(__ANDROID__) || (defined(__APPLE__) && TARGET_OS_IOS && !TARGET_OS_MACCATALYST))
    if (getSettings().game.speedrunMode && getSettings().game.liveSplitEnabled) {
        dusk::speedrun::updateLiveSplit();
        if (dusk::speedrun::consumeConnectedEvent()) {
            push_toast({.title = "LiveSplit connected", .duration = std::chrono::seconds(3)});
        }
        if (dusk::speedrun::consumeDisconnectedEvent()) {
            push_toast({.title = "LiveSplit disconnected", .duration = std::chrono::seconds(3)});
        }
    }
#endif

    if (mSpeedrunTimer != nullptr && mSpeedrunRta != nullptr && mSpeedrunIgt != nullptr) {
        if (getSettings().game.speedrunMode) {
            // L+R+A+Start to reset timer
            if (mDoCPd_c::getHoldL(PAD_1) && mDoCPd_c::getHoldR(PAD_1) &&
                mDoCPd_c::getHoldA(PAD_1) && mDoCPd_c::getTrigZ(PAD_1))
            {
                m_speedrunInfo.reset();
            }

            // L+R+A+Y to manually stop timer
            if (mDoCPd_c::getHoldL(PAD_1) && mDoCPd_c::getHoldR(PAD_1) &&
                mDoCPd_c::getHoldA(PAD_1) && mDoCPd_c::getTrigY(PAD_1))
            {
                if (m_speedrunInfo.m_isRunStarted) {
                    m_speedrunInfo.m_endTimestamp = OSGetTime() - m_speedrunInfo.m_startTimestamp;
                    m_speedrunInfo.m_isRunStarted = false;
                }
            }

            OSTime elapsedTime = 0;
            if (m_speedrunInfo.m_isRunStarted) {
                elapsedTime = OSGetTime() - m_speedrunInfo.m_startTimestamp;
            } else if (m_speedrunInfo.m_endTimestamp != 0) {
                elapsedTime = m_speedrunInfo.m_endTimestamp;
            }

            if (!m_speedrunInfo.m_isPauseIGT) {
                m_speedrunInfo.m_igtTimer = elapsedTime - m_speedrunInfo.m_totalLoadTime;
            }

            mSpeedrunTimer->SetAttribute("open", "");

            if (getSettings().game.showSpeedrunRTATimer) {
                mSpeedrunRta->SetAttribute("open", "");
                mSpeedrunRta->SetInnerRML(escape(fmt::format("RTA  {}", FormatTime(elapsedTime))));
            } else {
                mSpeedrunRta->RemoveAttribute("open");
            }

            mSpeedrunIgt->SetInnerRML(escape(fmt::format("IGT  {}", FormatTime(m_speedrunInfo.m_igtTimer))));
        } else {
            mSpeedrunTimer->RemoveAttribute("open");
        }
    }

    u32 count = 0;
    const bool showControllerWarning = PADGetIndexForPort(PAD_CHAN0) < 0 &&
                                       PADGetKeyButtonBindings(PAD_CHAN0, &count) == nullptr &&
                                       dynamic_cast<Window*>(top_document()) == nullptr &&
                                       dynamic_cast<WindowSmall*>(top_document()) == nullptr;
    if (showControllerWarning && mControllerWarning == nullptr) {
        mControllerWarning = create_controller_warning(mDocument);
    } else if (showControllerWarning && mControllerWarning != nullptr) {
        mControllerWarning->SetAttribute("open", "");
        mControllerWarning->SetPseudoClass("opened", true);
        mControllerWarning->SetPseudoClass("done", false);
    } else if (!showControllerWarning && mControllerWarning != nullptr) {
        if (mControllerWarning->IsPseudoClassSet("done") ||
            !mControllerWarning->IsPseudoClassSet("opened"))
        {
            remove_element(mControllerWarning);
        } else {
            mControllerWarning->RemoveAttribute("open");
        }
    }

    if (mMenuNotification != nullptr) {
        if (clock::now() >= mMenuNotificationStartTime + kMenuNotificationDuration) {
            if (mMenuNotification->IsPseudoClassSet("done") ||
                !mMenuNotification->IsPseudoClassSet("opened"))
            {
                remove_element(mMenuNotification);
            } else {
                mMenuNotification->RemoveAttribute("open");
            }
        } else {
            mMenuNotification->SetAttribute("open", "");
            mMenuNotification->SetPseudoClass("opened", true);
            mMenuNotification->SetPseudoClass("done", false);
        }
    }
    if (consume_menu_notification_request()) {
        if (mMenuNotification == nullptr) {
            mMenuNotification = create_menu_notification(mDocument);
        }
        mMenuNotificationStartTime = clock::now();
    }

    auto& toasts = get_toasts();
    if (mCurrentToast == nullptr) {
        if (!toasts.empty()) {
            const auto& toast = toasts.front();
            mCurrentToast = create_toast(mDocument, toast);
            mCurrentToastStartTime = clock::now();
        }
    } else if (!toasts.empty()) {
        const auto& toast = toasts.front();
        const float duration = std::chrono::duration<float>(toast.duration).count();
        const float elapsed =
            std::chrono::duration<float>(clock::now() - mCurrentToastStartTime).count();
        const float ratio = duration > 0.0f ? std::clamp(elapsed / duration, 0.0f, 1.0f) : 1.0f;
        const auto remaining = 1.f - ratio;
        Rml::ElementList list;
        mDocument->GetElementsByTagName(list, "progress");
        for (auto* elem : list) {
            elem->SetAttribute("value", remaining);
        }
        if (remaining == 0.f) {
            if (mCurrentToast->IsPseudoClassSet("done") ||
                // Fallback for large gaps in time where we never actually opened it
                !mCurrentToast->IsPseudoClassSet("opened"))
            {
                remove_element(mCurrentToast);
                toasts.pop_front();
            } else {
                mCurrentToast->RemoveAttribute("open");
            }
        } else {
            mCurrentToast->SetAttribute("open", "");
            mCurrentToast->SetPseudoClass("opened", true);
        }
    }
}

bool Overlay::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    Log.warn("Overlay received nav command: {}", magic_enum::enum_name(cmd));
    return false;
}

}  // namespace dusk::ui
