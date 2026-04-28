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
    constructor.BindEventCallback("set_active_tab", &WindowModel::set_active_tab, &model);

    handle = constructor.GetModelHandle();
    return true;
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
    model.DirtyVariable("active_tab");

    // Replace window content with new tab content
    auto* currentElem = event.GetCurrentElement();
    if (currentElem == nullptr) {
        return;
    }
    auto* doc = currentElem->GetOwnerDocument();
    if (doc == nullptr) {
        return;
    }
    auto* content = doc->GetElementById("content");
    if (content == nullptr) {
        return;
    }
    while (content->GetNumChildren() > 0) {
        content->RemoveChild(content->GetFirstChild());
    }
    if (tabs[tabIndex].setContent) {
        tabs[tabIndex].setContent(content);
    }
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
    mModel.tabs[0].setContent(mDocument->GetElementById("content"));
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
