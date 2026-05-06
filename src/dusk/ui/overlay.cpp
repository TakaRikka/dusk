#include "overlay.hpp"

#include "aurora/lib/logging.hpp"
#include "magic_enum.hpp"

#include <algorithm>

#include "dusk/achievements.h"

namespace dusk::ui {
namespace {
aurora::Module Log{"dusk::ui::overlay"};

const Rml::String kDocumentSource = R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/overlay.rcss" />
</head>
<body>
</body>
</rml>
)RML";

Rml::Element* create_toast(Rml::Element* parent, const Toast& toast) {
    auto* elem = append(parent, "toast");
    if (!toast.type.empty()) {
        elem->SetClass(toast.type, true);
    }
    {
        auto* heading = append(elem, "heading");
        auto* span = append(heading, "span");
        span->SetInnerRML(toast.title);
        if (toast.type == "achievement") {
            auto* icon = append(heading, "icon");
            icon->SetClass("trophy", true);
            mDoAud_seStartMenu(kSoundAchievementUnlock);
        }
    }
    {
        auto* message = append(elem, "message");
        auto* span = append(message, "span");
        span->SetInnerRML(toast.content);
    }
    {
        auto* progress = append(elem, "progress");
        progress->SetAttribute("value", 1.f);
    }
    return elem;
}

}  // namespace

Overlay::Overlay() : Document(kDocumentSource) {
    listen(mDocument, Rml::EventId::Focus, [](Rml::Event&) { Log.warn("Overlay received focus"); });
    listen(mDocument, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        if (event.GetTargetElement() == mCurrentToast) {
            if (get_toasts().empty() ||
                clock::now() >= mCurrentToastStartTime + get_toasts().front().duration)
            {
                mCurrentToast->SetPseudoClass("done", true);
            }
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
                mCurrentToast->GetParentNode()->RemoveChild(mCurrentToast);
                mCurrentToast = nullptr;
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
