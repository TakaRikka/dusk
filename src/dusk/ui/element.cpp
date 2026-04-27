#include "element.hpp"

#include <RmlUi/Core/ElementDocument.h>

#include <string>

namespace dusk::ui {

std::string escape(std::string_view text) {
    std::string result;
    result.reserve(text.size());
    for (char c : text) {
        switch (c) {
        case '&':
            result += "&amp;";
            break;
        case '<':
            result += "&lt;";
            break;
        case '>':
            result += "&gt;";
            break;
        case '"':
            result += "&quot;";
            break;
        case '\'':
            result += "&apos;";
            break;
        default:
            result += c;
            break;
        }
    }
    return result;
}

Rml::Element* append(Rml::Element* parent, std::string_view tag, std::string_view id) {
    if (parent == nullptr) {
        return nullptr;
    }

    Rml::ElementDocument* document = parent->GetOwnerDocument();
    if (document == nullptr) {
        document = dynamic_cast<Rml::ElementDocument*>(parent);
    }
    if (document == nullptr) {
        return nullptr;
    }

    Rml::ElementPtr child = document->CreateElement(std::string(tag));
    Rml::Element* rawChild = child.get();
    if (!id.empty()) {
        rawChild->SetId(std::string(id));
    }
    return parent->AppendChild(std::move(child));
}

Rml::Element* append_text(Rml::Element* parent, std::string_view tag, std::string_view text,
                          std::string_view id) {
    Rml::Element* element = append(parent, tag, id);
    set_text(element, text);
    return element;
}

void set_text(Rml::Element* element, std::string_view text) {
    if (element != nullptr) {
        element->SetInnerRML(escape(text));
    }
}

void set_props(Rml::Element* element,
               std::initializer_list<std::pair<std::string_view, std::string_view> > properties) {
    if (element == nullptr) {
        return;
    }
    for (const auto& [name, value] : properties) {
        element->SetProperty(std::string(name), std::string(value));
    }
}

}  // namespace dusk::ui
