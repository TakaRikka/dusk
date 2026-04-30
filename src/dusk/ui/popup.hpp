#pragma once

#include <RmlUi/Core/ElementDocument.h>

#include "button.hpp"
#include "event.hpp"

#include <chrono>
#include <memory>
#include <optional>
#include <vector>

namespace dusk::ui {

class Window;

class Popup {
public:
    Popup(Window& settingsWindow, Window& editorWindow);
    ~Popup();

    Popup(const Popup&) = delete;
    Popup& operator=(const Popup&) = delete;

    void show();
    void hide();
    void toggle();
    bool is_visible() const;
    void update() noexcept;

private:
    void set_selected_tab(int index);
    bool focus_tab(int index);

    Window& mSettingsWindow;
    Window& mEditorWindow;
    Rml::ElementDocument* mDocument = nullptr;
    std::vector<std::unique_ptr<Button> > mTabs;
    std::vector<std::function<void()> > mTabActions;
    std::unique_ptr<Button> mCloseButton;
    int mSelectedTabIndex = 0;
    bool mVisible = false;
    std::optional<std::chrono::steady_clock::time_point> mHideDeadline;
    std::unique_ptr<ScopedEventListener> mKeyListener;
};

}  // namespace dusk::ui
