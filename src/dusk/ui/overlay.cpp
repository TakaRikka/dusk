#include "overlay.hpp"

#include "aurora/lib/logging.hpp"
#include "magic_enum.hpp"

#include <algorithm>

namespace dusk::ui {
namespace {
aurora::Module Log{"dusk::ui::overlay"};

const Rml::String kDocumentSource = R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/overlay.rcss" />
</head>
<body>
    <toast class="achievement">
        <title><icon class="trophy" /> Achievement Unlocked!</title>
        <message>
            <span>Rollgoal Novice</span>
            <!--icon class="arrow-forward" /-->
        </message>
        <progress id="timer" value="1" />
    </toast>
</body>
</rml>
)RML";

Rml::Element* createToast(Rml::Element* parent, const Toast& toast) {
    auto* doc = parent->GetOwnerDocument();
    auto elem = doc->CreateElement("toast");
    return parent->AppendChild(std::move(elem));
}

}  // namespace

// class Toast : public Component {
// public:
//     Toast(Rml::Element* elem) : Component(createToast(elem, toast)) {}
//
//     void update() override;
//
//     bool finished() const { return mFinished; }
//
// private:
//     bool mFinished = false;
// };

Overlay::Overlay() : Document(kDocumentSource) {
    listen(mDocument, Rml::EventId::Focus, [](Rml::Event&) { Log.warn("Overlay received focus"); });
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

    // const auto now = clock::now();
    // const float duration = std::chrono::duration<float>(mDuration).count();
    // const float elapsed = std::chrono::duration<float>(now - mStartTime).count();
    // const float ratio = duration > 0.0f ? std::clamp(elapsed / duration, 0.0f, 1.0f) : 1.0f;
    // if (auto* timer = mDocument->GetElementById("timer")) {
    //     timer->SetAttribute("value", 1.0f - ratio);
    // }
    // if (ratio == 1.f) {
    //     Rml::ElementList list;
    //     mDocument->GetElementsByTagName(list, "toast");
    //     for (auto* elem : list) {
    //         elem->RemoveAttribute("open");
    //     }
    // }
}

bool Overlay::handle_nav_command(Rml::Event& event, NavCommand cmd) {
    Log.warn("Overlay received nav command: {}", magic_enum::enum_name(cmd));
    return false;
}

}  // namespace dusk::ui
