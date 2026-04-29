#include "pane.hpp"

#include "ui.hpp"

namespace dusk::ui {
namespace {

Rml::Element* createRoot(Rml::Element* parent) {
    auto* doc = parent->GetOwnerDocument();
    auto elem = doc->CreateElement("div");
    elem->SetClass("pane", true);
    return parent->AppendChild(std::move(elem));
}

}  // namespace

Pane::Pane(Rml::Element* parent) : Component(createRoot(parent)) {}

Rml::Element* Pane::add_section(const Rml::String& text) {
    auto* elem = append(mRoot, "div");
    elem->SetClass("section-heading", true);
    elem->SetInnerRML(escape(text));
    return elem;
}

Rml::Element* Pane::add_text(const Rml::String& text) {
    auto* elem = append(mRoot, "div");
    elem->SetInnerRML(escape(text));
    return elem;
}

void Pane::clear() {
    clear_children();
}

}  // namespace dusk::ui