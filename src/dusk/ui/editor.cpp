#include "editor.hpp"

#include <RmlUi/Core.h>

#include "fmt/format.h"

#include "aurora/lib/dolphin/gd/gd.hpp"
#include "button.hpp"
#include "pane.hpp"
#include "select_button.hpp"

#include <charconv>

namespace dusk::ui {
namespace {
aurora::Module Log{"dusk::ui::editor"};

bool has_save_data() {
    return dComIfGs_getSaveData() != nullptr;
}

dSv_player_status_a_c* get_player_status() {
    if (!has_save_data()) {
        return nullptr;
    }
    return &dComIfGs_getSaveData()->getPlayer().getPlayerStatusA();
}

Rml::String get_player_name() {
    if (!has_save_data()) {
        return "";
    }
    return dComIfGs_getPlayerName();
}

void set_player_name(Rml::String name) {
    dComIfGs_setPlayerName(name.c_str());
}

Rml::String get_horse_name() {
    if (!has_save_data()) {
        return "";
    }
    return dComIfGs_getHorseName();
}

void set_horse_name(Rml::String name) {
    dComIfGs_setHorseName(name.c_str());
}

Rml::String value_for_player_selection(const Rml::String& selection) {
    dSv_player_status_a_c* status = get_player_status();
    if (selection == "get_horse_name") {
        return get_horse_name();
    }
    if (status == nullptr) {
        return "?";
    }
    if (selection == "max_health") {
        return fmt::format("{}", static_cast<u16>(status->mMaxLife));
    }
    if (selection == "health") {
        return fmt::format("{}", static_cast<u16>(status->mLife));
    }
    if (selection == "max_oil") {
        return fmt::format("{}", static_cast<u16>(status->mMaxOil));
    }
    if (selection == "oil") {
        return fmt::format("{}", static_cast<u16>(status->mOil));
    }
    return "Unknown";
}

Rml::String make_select_row(std::string_view key, std::string_view label, const Rml::String& value,
    const Rml::String& activeSelection) {
    const char* selectedClass = key == activeSelection ? " selected" : "";
    return fmt::format(
        "<button class=\"select-button{0}\" data-event-click=\"set_active_selection('{1}')\">"
        "<div class=\"key\">{2}</div><div class=\"value\">{3}</div></button>",
        selectedClass, key, label, value);
}

Rml::String make_numeric_detail(
    std::string_view label, std::string_view decAction, std::string_view incAction) {
    return fmt::format(
        "<div class=\"pane detail-pane\">"
        "<div class=\"section-heading\">{0}</div>"
        "<div class=\"detail-controls\">"
        "<button class=\"button\" data-event-click=\"window_action('{1}')\">-1</button>"
        "<button class=\"button\" data-event-click=\"window_action('{2}')\">+1</button>"
        "</div>"
        "</div>",
        label, decAction, incAction);
}

template <typename TValue>
void adjust_u16(TValue& value, int delta, u16 minValue, u16 maxValue) {
    const int nextValue = std::clamp(
        static_cast<int>(value) + delta, static_cast<int>(minValue), static_cast<int>(maxValue));
    value = static_cast<u16>(nextValue);
}

void render_player_status_tab(Rml::Element* content, const Rml::String& activeSelection) {
    Rml::String leftPane = R"RML(<div class="pane"><div class="section-heading">Player</div>)RML";
    leftPane += make_select_row("player_name", "Player Name", get_player_name(), activeSelection);
    leftPane += make_select_row("horse_name", "Horse Name", get_horse_name(), activeSelection);
    leftPane += make_select_row(
        "max_health", "Max Health", value_for_player_selection("max_health"), activeSelection);
    leftPane +=
        make_select_row("health", "Health", value_for_player_selection("health"), activeSelection);
    leftPane += make_select_row(
        "max_oil", "Max Oil", value_for_player_selection("max_oil"), activeSelection);
    leftPane += make_select_row("oil", "Oil", value_for_player_selection("oil"), activeSelection);
    leftPane += "</div>";

    Rml::String rightPane;
    if (activeSelection == "max_health") {
        rightPane = make_numeric_detail("Max Health", "max_health.dec", "max_health.inc");
    } else if (activeSelection == "health") {
        rightPane = make_numeric_detail("Health", "health.dec", "health.inc");
    } else if (activeSelection == "max_oil") {
        rightPane = make_numeric_detail("Max Oil", "max_oil.dec", "max_oil.inc");
    } else if (activeSelection == "oil") {
        rightPane = make_numeric_detail("Oil", "oil.dec", "oil.inc");
    }

    Rml::Factory::InstanceElementText(content, leftPane + rightPane);
}

bool handle_editor_action(const Rml::VariantList& arguments) {
    if (arguments.empty() || !has_save_data()) {
        return true;
    }

    const Rml::String action = arguments[0].Get<Rml::String>();
    dSv_player_status_a_c* status = get_player_status();
    if (status == nullptr) {
        return true;
    }

    if (action == "max_health.inc") {
        adjust_u16(status->mMaxLife, 1, 0, 0xFFFF);
        return true;
    } else if (action == "max_health.dec") {
        adjust_u16(status->mMaxLife, -1, 0, 0xFFFF);
        if (status->mLife > status->mMaxLife) {
            status->mLife = status->mMaxLife;
        }
        return true;
    }

    if (action == "health.inc") {
        adjust_u16(status->mLife, 1, 0, status->mMaxLife);
        return true;
    } else if (action == "health.dec") {
        adjust_u16(status->mLife, -1, 0, status->mMaxLife);
        return true;
    }

    if (action == "max_oil.inc") {
        adjust_u16(status->mMaxOil, 1, 0, 0xFFFF);
        return true;
    } else if (action == "max_oil.dec") {
        adjust_u16(status->mMaxOil, -1, 0, 0xFFFF);
        if (status->mOil > status->mMaxOil) {
            status->mOil = status->mMaxOil;
        }
        return true;
    }

    if (action == "oil.inc") {
        adjust_u16(status->mOil, 1, 0, status->mMaxOil);
        return true;
    } else if (action == "oil.dec") {
        adjust_u16(status->mOil, -1, 0, status->mMaxOil);
        return true;
    }

    return false;
}

}  // namespace

class ControlledSelectButton : public SelectButton {
public:
    ControlledSelectButton(Rml::Element* parent, Props props)
        : SelectButton(parent, std::move(props)) {}

    void update() override {
        set_disabled(is_disabled());
        set_value_label(format_value());
        SelectButton::update();
    }

protected:
    virtual Rml::String format_value() = 0;
    virtual bool is_disabled() { return false; }
};

class BaseStringButton : public ControlledSelectButton {
public:
    struct Props {
        Rml::String key;
        Rml::String type = "text";
        int maxLength = -1;
    };

    BaseStringButton(Rml::Element* parent, Props props)
        : ControlledSelectButton(parent, {std::move(props.key)}), mType(std::move(props.type)),
          mMaxLength(props.maxLength) {
        mInputListeners.reserve(3);
    }

    void update() override {
        if (mPendingStopEditing) {
            stop_editing(mPendingCommit, mPendingRefocusRoot);
        }
        ControlledSelectButton::update();
    }

    void start_editing() {
        if (mInputElem != nullptr) {
            return;
        }
        auto* doc = mRoot->GetOwnerDocument();
        auto elemPtr = doc->CreateElement("input");
        mInputElem = rmlui_dynamic_cast<Rml::ElementFormControlInput*>(elemPtr.get());
        if (mInputElem == nullptr) {
            return;
        }
        mInputElem->SetAttribute("type", mType);
        mInputElem->SetAttribute("value", format_value());
        if (mMaxLength > -1) {
            mInputElem->SetAttribute("maxlength", mMaxLength);
        }
        mRoot->AppendChild(std::move(elemPtr));
        mValueElem->SetProperty(Rml::PropertyId::Visibility, Rml::Style::Visibility::Hidden);
        mInputElem->Focus(true);
        const int end = static_cast<int>(Rml::StringUtilities::LengthUTF8(mInputElem->GetValue()));
        mInputElem->SetSelectionRange(0, end);
        set_selected(true);
        mInputListeners.emplace_back(std::make_unique<ScopedEventListener>(
            mInputElem, Rml::EventId::Keydown, [this](Rml::Event& event) {
                const auto cmd = map_nav_event(event);
                if (cmd == NavCommand::Confirm) {
                    request_stop_editing(true, true);
                    event.StopImmediatePropagation();
                } else if (cmd == NavCommand::Cancel) {
                    request_stop_editing(false, true);
                    event.StopImmediatePropagation();
                }
            }));
        mInputListeners.emplace_back(std::make_unique<ScopedEventListener>(
            mInputElem, Rml::EventId::Click, [](Rml::Event& event) { event.StopPropagation(); }));
        mInputListeners.emplace_back(std::make_unique<ScopedEventListener>(mInputElem,
            Rml::EventId::Blur, [this](Rml::Event&) { request_stop_editing(true, false); }));
    }

    void request_stop_editing(bool commit, bool refocusRoot) {
        mPendingStopEditing = true;
        mPendingCommit = commit;
        mPendingRefocusRoot = refocusRoot;
    }

protected:
    bool handle_nav_command(NavCommand cmd) override {
        if (cmd == NavCommand::Confirm) {
            if (mInputElem == nullptr) {
                start_editing();
            } else {
                request_stop_editing(true, true);
            }
            return true;
        } else if (cmd == NavCommand::Cancel) {
            request_stop_editing(false, true);
            return true;
        }
        return false;
    }

    virtual void set_value(Rml::String value) = 0;

private:
    void stop_editing(bool commit = true, bool refocusRoot = false) {
        if (mInputElem == nullptr) {
            return;
        }
        mPendingStopEditing = false;
        if (commit) {
            set_value(mInputElem->GetValue());
        }
        mInputListeners.clear();
        mRoot->RemoveChild(mInputElem);
        mInputElem = nullptr;
        mValueElem->SetProperty(Rml::PropertyId::Visibility, Rml::Style::Visibility::Visible);
        set_selected(false);
        if (refocusRoot) {
            mRoot->Focus(true);
        }
    }

    Rml::ElementFormControlInput* mInputElem = nullptr;
    std::vector<std::unique_ptr<ScopedEventListener> > mInputListeners;
    Rml::String mType;
    int mMaxLength;
    bool mPendingStopEditing = false;
    bool mPendingCommit = true;
    bool mPendingRefocusRoot = false;
};

class StringButton : public BaseStringButton {
public:
    struct Props {
        Rml::String key;
        std::function<Rml::String()> getValue;
        std::function<void(Rml::String)> setValue;
        int maxLength = -1;
    };

    StringButton(Rml::Element* parent, Props props)
        : BaseStringButton(parent,
              {
                  .key = std::move(props.key),
                  .maxLength = props.maxLength,
              }),
          mGetValue(std::move(props.getValue)), mSetValue(std::move(props.setValue)) {}

protected:
    Rml::String format_value() override { return mGetValue(); }
    void set_value(Rml::String value) override {
        if (mSetValue) {
            mSetValue(std::move(value));
        }
    }

private:
    std::function<Rml::String()> mGetValue;
    std::function<void(Rml::String)> mSetValue;
};

struct IntSelectProps {
    Rml::String key;
    std::function<int()> getValue;
    std::function<void(int)> setValue;
    int min = 0;
    int max = INT_MAX;
    int step = 1;
};

class IntSelectButton : public BaseStringButton {
public:
    using Props = IntSelectProps;

    IntSelectButton(Rml::Element* parent, Props props)
        : BaseStringButton(parent, {.key = std::move(props.key), .type = "number"}),
          mGetValue(std::move(props.getValue)), mSetValue(std::move(props.setValue)),
          mMin(props.min), mMax(props.max), mStep(props.step) {}

protected:
    Rml::String format_value() override { return fmt::to_string(mGetValue()); }
    void set_value(Rml::String value) override {
        if (!mSetValue) {
            return;
        }

        int parsedValue = 0;
        const char* begin = value.data();
        const char* end = begin + value.size();
        const auto result = std::from_chars(begin, end, parsedValue);
        if (result.ec != std::errc() || result.ptr != end) {
            return;
        }

        mSetValue(std::clamp(parsedValue, mMin, mMax));
    }

    bool handle_nav_command(NavCommand cmd) override {
        if (cmd == NavCommand::Left) {
            mSetValue(std::clamp(mGetValue() - mStep, mMin, mMax));
            return true;
        } else if (cmd == NavCommand::Right) {
            mSetValue(std::clamp(mGetValue() + mStep, mMin, mMax));
            return true;
        }
        return BaseStringButton::handle_nav_command(cmd);
    }

private:
    std::function<int()> mGetValue;
    std::function<void(int)> mSetValue;
    int mMin;
    int mMax;
    int mStep;
};

EditorWindow::EditorWindow() {
    add_tab("Player Status", [this](Rml::Element* content) {
        auto& leftPane = add_child<Pane>(content, Pane::Direction::Vertical);
        auto& rightPane = add_child<Pane>(content, Pane::Direction::Vertical);

        leftPane.add_section("Player");
        leftPane.add_child<StringButton>(leftPane.root(), StringButton::Props{
                                                              .key = "Player Name",
                                                              .getValue = get_player_name,
                                                              .setValue = set_player_name,
                                                              .maxLength = 16,
                                                          });
        leftPane.add_child<StringButton>(leftPane.root(), StringButton::Props{
                                                              .key = "Horse Name",
                                                              .getValue = get_horse_name,
                                                              .setValue = set_horse_name,
                                                              .maxLength = 16,
                                                          });
        leftPane.add_child<IntSelectButton>(leftPane.root(),
            IntSelectButton::Props{
                .key = "Max Health",
                .getValue = [] { return get_player_status()->getMaxLife(); },
                .setValue = [](int value) { return get_player_status()->setMaxLife(value); },
                .max = UINT16_MAX, // TODO: actual max
            });
        leftPane.add_child<IntSelectButton>(leftPane.root(),
            IntSelectButton::Props{
                .key = "Health",
                .getValue = [] { return get_player_status()->getLife(); },
                .setValue = [](int value) { return get_player_status()->setLife(value); },
                .max = UINT16_MAX, // TODO: actual max
            });
        leftPane.add_child<IntSelectButton>(leftPane.root(),
            IntSelectButton::Props{
                .key = "Max Oil",
                .getValue = [] { return get_player_status()->getMaxOil(); },
                .setValue = [](int value) { return get_player_status()->setMaxOil(value); },
                .max = UINT16_MAX, // TODO: actual max
            });
        leftPane.add_child<IntSelectButton>(leftPane.root(),
            IntSelectButton::Props{
                .key = "Oil",
                .getValue = [] { return get_player_status()->getOil(); },
                .setValue = [](int value) { return get_player_status()->setOil(value); },
                .max = UINT16_MAX, // TODO: actual max
            });

        leftPane.add_section("Equipment");
        leftPane.add_select_button({
            .key = "Equip X",
            .value = "TODO",
        });
        leftPane.add_select_button({
            .key = "Equip Y",
            .value = "TODO",
        });

        rightPane.add_button({
            .text = "Hello, world!",
        });
    });

    add_tab("Location", [this](Rml::Element* content) {
        // TODO
    });

    add_tab("Inventory", [this](Rml::Element* content) {
        // TODO
    });

    add_tab("Collection", [this](Rml::Element* content) {
        // TODO
    });

    add_tab("Flags", [this](Rml::Element* content) {
        // TODO
    });

    add_tab("Minigame", [this](Rml::Element* content) {
        // TODO
    });

    add_tab("Config", [this](Rml::Element* content) {
        // TODO
    });
}

}  // namespace dusk::ui
