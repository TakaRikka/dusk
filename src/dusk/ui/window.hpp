#pragma once

#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/ElementDocument.h>

namespace dusk::ui {

struct WindowTab {
    Rml::String label;
    std::function<void(Rml::Element*)> setContent;
};

struct WindowModel {
    int activeTab = 0;
    std::vector<WindowTab> tabs;

    void set_active_tab(
        Rml::DataModelHandle model, Rml::Event& event, const Rml::VariantList& arguments);
};

class Window {
public:
    Window(WindowModel model);
    ~Window();

    void show();
    void hide();

private:
    WindowModel mModel;
    Rml::DataModelHandle mModelHandle;
    Rml::ElementDocument* mDocument;
};

}  // namespace dusk::ui