#include "window.hpp"

#include <RmlUi/Core.h>

#include "aurora/rmlui.hpp"

namespace dusk::ui {
namespace {

bool setup_window_model(Rml::Context* context, WindowModel& model, Rml::DataModelHandle& handle) {
    Rml::DataModelConstructor constructor = context->CreateDataModel("window");
    if (!constructor) {
        return false;
    }

    if (auto tab_handle = constructor.RegisterStruct<WindowTab>()) {
        tab_handle.RegisterMember("label", &WindowTab::label);
    } else {
        return false;
    }

    if (!constructor.RegisterArray<std::vector<WindowTab> >()) {
        return false;
    }

    constructor.Bind("active_tab", &model.activeTab);
    constructor.Bind("tabs", &model.tabs);
    constructor.Bind("active_selection", &model.activeSelection);
    constructor.BindEventCallback("set_active_tab", &WindowModel::set_active_tab, &model);
    constructor.BindEventCallback("set_active_selection", &WindowModel::set_active_selection, &model);
    constructor.BindEventCallback("window_action", &WindowModel::handle_action, &model);

    handle = constructor.GetModelHandle();
    return true;
}

Rml::ElementDocument* get_document_from_event(Rml::Event& event) {
    auto* currentElem = event.GetCurrentElement();
    if (currentElem == nullptr) {
        return nullptr;
    }
    return currentElem->GetOwnerDocument();
}

Rml::Element* get_content_element(Rml::ElementDocument* document) {
    if (document == nullptr) {
        return nullptr;
    }
    return document->GetElementById("content");
}

void clear_children(Rml::Element* element) {
    if (element == nullptr) {
        return;
    }
    while (element->GetNumChildren() > 0) {
        element->RemoveChild(element->GetFirstChild());
    }
}

void ensure_tab_selection_state(WindowModel& model) {
    if (model.tabSelections.size() < model.tabs.size()) {
        model.tabSelections.resize(model.tabs.size());
    }
    if (model.activeTab < 0 || model.activeTab >= static_cast<int>(model.tabs.size())) {
        model.activeTab = 0;
    }
    if (model.tabs.empty()) {
        model.activeSelection.clear();
        return;
    }

    Rml::String& tabSelection = model.tabSelections[model.activeTab];
    if (tabSelection.empty()) {
        tabSelection = model.tabs[model.activeTab].defaultSelection;
    }
    model.activeSelection = tabSelection;
}

void render_active_tab_content(WindowModel& model, Rml::ElementDocument* document) {
    auto* content = get_content_element(document);
    if (content == nullptr) {
        return;
    }

    clear_children(content);
    if (model.tabs.empty()) {
        return;
    }

    ensure_tab_selection_state(model);
    const WindowTab& tab = model.tabs[model.activeTab];
    if (tab.setContent) {
        tab.setContent(content, model.activeSelection);
    }
}

}  // namespace

void WindowModel::set_active_tab(
    Rml::DataModelHandle model, Rml::Event& event, const Rml::VariantList& arguments) {
    if (arguments.empty()) {
        return;
    }

    const int tabIndex = arguments[0].Get<int>();
    if (tabIndex < 0 || tabIndex >= static_cast<int>(tabs.size()) || tabIndex == activeTab) {
        return;
    }

    activeTab = tabIndex;
    ensure_tab_selection_state(*this);
    model.DirtyVariable("active_tab");
    model.DirtyVariable("active_selection");
    render_active_tab_content(*this, get_document_from_event(event));
}

void WindowModel::set_active_selection(
    Rml::DataModelHandle model, Rml::Event& event, const Rml::VariantList& arguments) {
    if (arguments.empty() || tabs.empty()) {
        return;
    }

    const Rml::String selection = arguments[0].Get<Rml::String>();
    ensure_tab_selection_state(*this);
    if (activeSelection == selection) {
        return;
    }

    activeSelection = selection;
    tabSelections[activeTab] = selection;
    model.DirtyVariable("active_selection");
    render_active_tab_content(*this, get_document_from_event(event));
}

void WindowModel::handle_action(
    Rml::DataModelHandle model, Rml::Event& event, const Rml::VariantList& arguments) {
    bool shouldRerender = true;
    if (actionHandler) {
        shouldRerender = actionHandler(arguments);
    }
    if (!shouldRerender) {
        return;
    }

    model.DirtyVariable("active_tab");
    model.DirtyVariable("active_selection");
    render_active_tab_content(*this, get_document_from_event(event));
}

Window::Window(WindowModel model) : mModel(std::move(model)) {
    auto* context = aurora::rmlui::get_context();
    if (context == nullptr) {
        return;
    }

    setup_window_model(context, mModel, mModelHandle);

    mDocument = context->LoadDocument("res/rml/window.rml");
    if (mDocument == nullptr) {
        return;
    }

    ensure_tab_selection_state(mModel);
    render_active_tab();
}

void Window::render_active_tab() noexcept {
    render_active_tab_content(mModel, mDocument);
}

Window::~Window() {
    auto* context = aurora::rmlui::get_context();
    if (context != nullptr && mDocument != nullptr) {
        context->UnloadDocument(mDocument);
        mDocument = nullptr;
    }
}

void Window::show() {
    if (mDocument != nullptr) {
        mDocument->Show();
    }
}

void Window::hide() {
    if (mDocument != nullptr) {
        mDocument->Hide();
    }
}

}  // namespace dusk::ui
