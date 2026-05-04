#include "event.hpp"

#include <utility>

namespace dusk::ui {

ScopedEventListener::ScopedEventListener(
    Rml::Element* element, Rml::EventId event, Callback callback, bool capture)
    : mElement(element), mEvent(event), mCapture(capture), mCallback(std::move(callback)) {
    mElement->AddEventListener(mEvent, this, mCapture);
}

ScopedEventListener::ScopedEventListener(
    Rml::Element* element, Rml::String event, Callback callback, bool capture)
    : mElement(element), mEventName(std::move(event)), mCapture(capture),
      mCallback(std::move(callback)) {
    mElement->AddEventListener(mEventName, this, mCapture);
}

ScopedEventListener::~ScopedEventListener() {
    if (mElement != nullptr) {
        if (!mEventName.empty()) {
            mElement->RemoveEventListener(mEventName, this, mCapture);
        } else {
            mElement->RemoveEventListener(mEvent, this, mCapture);
        }
        mElement = nullptr;
    }
}

void ScopedEventListener::ProcessEvent(Rml::Event& event) {
    if (mCallback) {
        mCallback(event);
    }
}

void ScopedEventListener::OnDetach(Rml::Element* element) {
    if (element == mElement) {
        mElement = nullptr;
    }
}

}  // namespace dusk::ui
