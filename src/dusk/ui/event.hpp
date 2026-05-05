#pragma once

#include <RmlUi/Core.h>

#include <functional>

namespace dusk::ui {

class ScopedEventListener final : public Rml::EventListener {
public:
    using Callback = std::function<void(Rml::Event&)>;

    ScopedEventListener(
        Rml::Element* element, Rml::EventId event, Callback callback, bool capture = false);
    ScopedEventListener(
        Rml::Element* element, Rml::String event, Callback callback, bool capture = false);
    ~ScopedEventListener() override;

    ScopedEventListener(const ScopedEventListener&) = delete;
    ScopedEventListener& operator=(const ScopedEventListener&) = delete;
    ScopedEventListener(ScopedEventListener&&) = delete;
    ScopedEventListener& operator=(ScopedEventListener&&) = delete;

    void ProcessEvent(Rml::Event& event) override;
    void OnDetach(Rml::Element* element) override;

private:
    Rml::Element* mElement = nullptr;
    Rml::EventId mEvent = Rml::EventId::Invalid;
    Rml::String mEventName;
    bool mCapture = false;
    Callback mCallback;
};

}  // namespace dusk::ui
