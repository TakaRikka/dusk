#pragma once

#include "document.hpp"

namespace dusk::ui {

class Popover : public Document {
public:
    enum class Side {
        Below,
        Above,
        Left,
        Right,
    };

    Popover(Rml::Element* anchor, Side side, const Rml::String& windowClass = "");
    ~Popover() override;

    void show() override;
    void hide(bool close) override;
    bool focus() override;
    bool visible() const override;
    void update() override;

    Rml::Element* body() const { return mBody; }

    void dismiss();
    void on_close(std::function<void()> callback) { mOnClose = std::move(callback); }
    void on_focus(std::function<bool()> callback) { mOnFocus = std::move(callback); }

protected:
    bool handle_nav_command(Rml::Event& event, NavCommand cmd) override;

private:
    void reposition();
    void notify_close(bool restoreFocus);

    Rml::Element* mAnchor = nullptr;
    Side mSide;
    Rml::Element* mBody = nullptr;
    std::function<void()> mOnClose;
    std::function<bool()> mOnFocus;
};

}  // namespace dusk::ui
