#pragma once

#include "window.hpp"

#include <pad.h>

namespace dusk::ui {

class ControllerConfigWindow : public Window {
public:
    ControllerConfigWindow();

    void update() override;
    void hide(bool close) override;

private:
    enum class Page {
        Controller,
        Buttons,
        Triggers,
        Sticks,
    };

    void build_port_tab(Rml::Element* content, int port);
    void render_page(class Pane& pane, int port, Page page);
    void refresh_controller_page();
    void poll_pending_binding();
    void finish_pending_binding(int completedPort);
    void unmap_pending_binding();
    bool capture_active() const;
    bool pending_input_neutral() const;
    Rml::String pending_button_label() const;
    Rml::String pending_axis_label() const;
    void cancel_pending_binding();
    void finish_pending_key_binding();
    Rml::String pending_key_label() const;

    Page mPage = Page::Controller;
    Pane* mRightPane = nullptr;
    int mActivePort = 0;
    int mPendingPort = -1;
    bool mPendingBindingArmed = false;
    bool mSuppressNavigationUntilNeutral = false;
    int mSuppressNavigationPort = -1;
    PADButtonMapping* mPendingButtonMapping = nullptr;
    PADAxisMapping* mPendingAxisMapping = nullptr;
    int mPendingKeyButton = -1;
    int mPendingKeyAxis = -1;
};

}  // namespace dusk::ui
