#include "popover.hpp"

#include <algorithm>

namespace dusk::ui {
namespace {

const Rml::String kDocumentSource = R"RML(
<rml>
<head>
    <link type="text/rcss" href="res/rml/popover.rcss" />
</head>
<body>
    <popover id="popover"/>
</body>
</rml>
)RML";

constexpr float kAnchorGapDp = 8.0f;
constexpr float kViewportMarginDp = 8.0f;

}  // namespace

Popover::Popover(Rml::Element* anchor, Side side, const Rml::String& windowClass)
    : Document{kDocumentSource}, mAnchor{anchor}, mSide{side},
      mBody{mDocument->GetElementById("popover")} {
    if (!windowClass.empty()) {
        mBody->SetClass(windowClass, true);
    }

    listen(mBody, Rml::EventId::Transitionend, [this](Rml::Event& event) {
        if (event.GetTargetElement() == mBody && !mBody->HasAttribute("open") &&
            Document::visible())
        {
            Document::hide(mPendingClose);
        }
    });

    listen(
        mDocument->GetContext()->GetRootElement(), Rml::EventId::Mousedown,
        [this](const Rml::Event& event) {
            const auto within = [](const Rml::Element* element, const Rml::Element* container) {
                while (element != nullptr && element != container) {
                    element = element->GetParentNode();
                }
                return element != nullptr;
            };
            if (visible() && !within(event.GetTargetElement(), mBody)) {
                dismiss();
            }
        },
        true);
}

Popover::~Popover() {
    notify_close(false);
}

void Popover::show() {
    Document::show();
    reposition();
    mBody->SetAttribute("open", "");
    focus();
}

void Popover::hide(bool close) {
    notify_close(true);
    mBody->RemoveAttribute("open");
    mPendingClose = close;
}

bool Popover::focus() {
    if (mOnFocus && mOnFocus()) {
        return true;
    }
    return mBody != nullptr && mBody->Focus(true);
}

bool Popover::visible() const {
    return mBody != nullptr && mBody->HasAttribute("open");
}

void Popover::update() {
    Document::update();
    if (!visible()) {
        return;
    }
    if (mAnchor == nullptr || !mAnchor->IsVisible(true)) {
        dismiss();
        return;
    }
    reposition();
}

void Popover::dismiss() {
    if (visible()) {
        hide(true);
    }
}

bool Popover::handle_nav_command(Rml::Event&, NavCommand cmd) {
    if (cmd == NavCommand::Cancel || cmd == NavCommand::Menu) {
        dismiss();
        return true;
    }
    return false;
}

void Popover::reposition() {
    auto* context = mDocument->GetContext();
    const auto dimensions = Rml::Vector2f{context->GetDimensions()};
    const float dpRatio = context->GetDensityIndependentPixelRatio();
    const float gap = kAnchorGapDp * dpRatio;
    const float margin = kViewportMarginDp * dpRatio;

    const Rml::Vector2f anchorPos = mAnchor->GetAbsoluteOffset(Rml::BoxArea::Border);
    const Rml::Vector2f anchorSize = mAnchor->GetBox().GetSize(Rml::BoxArea::Border);
    const Rml::Vector2f size = mBody->GetBox().GetSize(Rml::BoxArea::Border);

    Rml::Vector2f pos;
    switch (mSide) {
    case Side::Above:
        pos = {anchorPos.x + (anchorSize.x - size.x) * 0.5f, anchorPos.y - size.y - gap};
        break;
    case Side::Left:
        pos = {anchorPos.x - size.x - gap, anchorPos.y + (anchorSize.y - size.y) * 0.5f};
        break;
    case Side::Right:
        pos = {anchorPos.x + anchorSize.x + gap, anchorPos.y + (anchorSize.y - size.y) * 0.5f};
        break;
    case Side::Below:
    default:
        pos = {anchorPos.x + (anchorSize.x - size.x) * 0.5f, anchorPos.y + anchorSize.y + gap};
        break;
    }

    pos.x = std::clamp(pos.x, margin, std::max(margin, dimensions.x - size.x - margin));
    pos.y = std::clamp(pos.y, margin, std::max(margin, dimensions.y - size.y - margin));
    mBody->SetProperty(Rml::PropertyId::Left, Rml::Property{pos.x, Rml::Unit::PX});
    mBody->SetProperty(Rml::PropertyId::Top, Rml::Property{pos.y, Rml::Unit::PX});
}

void Popover::notify_close(bool restoreFocus) {
    if (mOnClose) {
        auto callback = std::move(mOnClose);
        mOnClose = nullptr;
        callback();
    }
    if (restoreFocus && mAnchor != nullptr && mAnchor->IsVisible(true)) {
        mAnchor->Focus(true);
    }
}

}  // namespace dusk::ui
