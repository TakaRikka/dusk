#pragma once

#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/ElementDocument.h>

namespace dusk::ui {

struct WindowTab {
    Rml::String label;
    Rml::String defaultSelection;
    std::function<void(Rml::Element*, const Rml::String&)> setContent;
};

struct WindowModel {
    int activeTab = 0;
    Rml::String activeSelection;
    std::vector<WindowTab> tabs;
    std::vector<Rml::String> tabSelections;
    std::function<bool(const Rml::VariantList&)> actionHandler;

    void set_active_tab(
        Rml::DataModelHandle model, Rml::Event& event, const Rml::VariantList& arguments);
    void set_active_selection(
        Rml::DataModelHandle model, Rml::Event& event, const Rml::VariantList& arguments);
    void handle_action(
        Rml::DataModelHandle model, Rml::Event& event, const Rml::VariantList& arguments);
};

class Window {
public:
    Window(WindowModel model);
    ~Window();

    void show();
    void hide();

private:
    void render_active_tab() noexcept;

    WindowModel mModel;
    Rml::DataModelHandle mModelHandle;
    Rml::ElementDocument* mDocument = nullptr;
};

}  // namespace dusk::ui
