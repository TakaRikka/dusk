#include "window.hpp"

#include <RmlUi/Core.h>

#include "aurora/rmlui.hpp"

namespace dusk::ui {
namespace {

const Rml::String kWindowDocumentRml = R"RML(
<rml>
<head>
    <title>Window</title>
    <style>
        *, *:before, *:after {
          box-sizing: border-box;
        }

		body {
            width: 100%;
            height: 100%;
		    padding: 64dp;
	        font-family: "Fira Sans";
	        font-weight: normal;
	        font-style: normal;
	        font-size: 15dp;
	        color: #E0DBC8;
		}

        .window {
			max-width: 1088dp;
			max-height: 768dp;
            margin: auto;
            border-radius: 14dp;
            overflow: hidden;
            border: 2dp #92875B;
            backdrop-filter: blur(5dp);
            box-shadow: 0 0 25dp 5dp;
            background-color: rgba(21, 22, 16, 90%);
        }

        .window .tab-bar {
            display: flex;
            height: 64dp;
            background-color: rgba(217, 217, 217, 10%);
            font-family: "Fira Sans Condensed";
            font-weight: bold;
            font-size: 18dp;
            text-transform: uppercase;
            border-bottom: 2dp #92875B;
        }

        .window .tab-bar .tab {
            padding: 0 24dp;
            line-height: 64dp;
            opacity: 0.25;
            tab-index: auto;
            nav: horizontal;
            focus: auto;
        }

        .window .tab-bar .tab.active {
            opacity: 1;
            border-bottom: 4dp #C2A42D;
            font-effect: glow(0dp 4dp 0dp 4dp black);
            decorator: linear-gradient(to bottom, rgba(194, 164, 45, 0%) 0%, rgba(194, 164, 45, 15%) 100%);
        }

        .window .tab-bar .tab:focus-visible {
            opacity: 1;
            font-effect: glow(0dp 4dp 0dp 4dp black);
            decorator: linear-gradient(to bottom, rgba(194, 164, 45, 0%) 0%, rgba(194, 164, 45, 15%) 100%);
        }

        .window .content {
            display: flex;
            height: 100%;
        }

        .window .content .pane {
            display: flex;
            flex-flow: column;
            flex: 1 1 0;
            height: 100%;
            padding: 24dp;
            gap: 8dp;
            overflow: auto;
        }

        .window .content .pane:not(:last-of-type) {
            border-right: 1dp #92875B;
        }

        .section-heading {
            font-weight: bold;
            text-transform: uppercase;
            font-size: 22dp;
            opacity: 0.25;
        }

        .button {
            text-align: center;
            background-color: rgba(17, 16, 10, 20%);
            opacity: 0.9;
            padding: 8dp 16dp;
            border-radius: 14dp;
            box-shadow: rgba(146, 135, 91, 25%) 0 0 0 1dp;
            font-size: 20dp;
            transition: background-color 0.1s linear-in-out, opacity 0.1s linear-in-out;
        }

        .button.active, .button:hover {
            background-color: rgba(204, 184, 119, 20%);
            box-shadow: #C2A42D 0 0 0 2dp;
        }

        .button.selected, .button:active {
            opacity: 1;
            background-color: rgba(204, 184, 119, 40%);
            box-shadow: #C2A42D 0 0 0 2dp;
        }

        .select-button {
            display: flex;
            align-items: center;
            gap: 8dp;
            background-color: rgba(17, 16, 10, 20%);
            opacity: 0.9;
            padding: 8dp 16dp;
            border-radius: 14dp;
            box-shadow: rgba(146, 135, 91, 25%) 0 0 0 1dp;
            transition: background-color 0.1s linear-in-out, opacity 0.1s linear-in-out;
        }

        .select-button.active, .select-button:hover {
            background-color: rgba(204, 184, 119, 20%);
            box-shadow: #C2A42D 0 0 0 2dp;
        }

        .select-button.selected, .select-button:active {
            opacity: 1;
            background-color: rgba(204, 184, 119, 40%);
            box-shadow: #C2A42D 0 0 0 2dp;
        }

        .select-button .key {
            font-family: "Fira Sans Condensed";
            font-weight: bold;
            font-size: 18dp;
            text-transform: uppercase;
        }

        .select-button .value {
            margin-left: auto;
            font-size: 20dp;
        }
	</style>
</head>
<body data-model="window">
    <div class="window">
        <div class="tab-bar">
            <button class="tab"
                data-for="tab, i : tabs"
                data-class-active="i == active_tab"
                data-event-click="set_active_tab(i)">
                {{ tab.label }}
            </button>
        </div>
        <div id="content" class="content"></div>
    </div>
</body>
</rml>
)RML";

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
    mDocument = context->LoadDocumentFromMemory(kWindowDocumentRml);
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
