#include "mod_window.hpp"

#include "bool_button.hpp"
#include "number_button.hpp"
#include "string_button.hpp"

#include "m_Do/m_Do_audio.h"

namespace dusk::ui {

Component* build_mod_control(Pane& pane, Pane* helpPane, ModControlSpec spec) {
    const auto shared = std::make_shared<ModControlSpec>(std::move(spec));
    auto& s = *shared;
    Component* control = nullptr;
    switch (s.kind) {
    case ModControlSpec::Kind::Button:
        control = &pane.add_button(ControlledButton::Props{
                                       .text = s.label,
                                       .isDisabled = s.isDisabled,
                                   })
                       .on_pressed([shared] {
                           if (shared->onPressed) {
                               shared->onPressed();
                           }
                       });
        break;
    case ModControlSpec::Kind::Toggle:
        control = &pane.add_child<BoolButton>(BoolButton::Props{
            .key = s.label,
            .getValue = s.getBool,
            .setValue = s.setBool,
            .isDisabled = s.isDisabled,
            .isModified = s.isModified,
        });
        break;
    case ModControlSpec::Kind::Number:
        control = &pane.add_child<NumberButton>(NumberButton::Props{
            .key = s.label,
            .getValue = s.getInt,
            .setValue = s.setInt,
            .isDisabled = s.isDisabled,
            .isModified = s.isModified,
            .min = s.min,
            .max = s.max,
            .step = s.step,
            .prefix = s.prefix,
            .suffix = s.suffix,
        });
        break;
    case ModControlSpec::Kind::String:
        control = &pane.add_child<StringButton>(StringButton::Props{
            .key = s.label,
            .getValue = s.getString,
            .setValue = s.setString,
            .isDisabled = s.isDisabled,
            .isModified = s.isModified,
            .maxLength = s.maxLength,
        });
        break;
    case ModControlSpec::Kind::Select:
        if (helpPane == nullptr || s.options.empty()) {
            return nullptr;
        }
        control = &pane.add_child<ControlledSelectButton>(ControlledSelectButton::Props{
            .key = s.label,
            .getValue = [shared]() -> Rml::String {
                const int index = shared->getInt ? shared->getInt() : -1;
                if (index < 0 || index >= static_cast<int>(shared->options.size())) {
                    return "?";
                }
                return shared->options[index];
            },
            .isDisabled = s.isDisabled,
            .isModified = s.isModified,
        });
        break;
    }
    if (control == nullptr) {
        return nullptr;
    }

    if (helpPane != nullptr && (s.kind == ModControlSpec::Kind::Select || !s.helpRml.empty())) {
        pane.register_control(*control, *helpPane, [shared](Pane& help) {
            help.clear();
            if (shared->kind == ModControlSpec::Kind::Select) {
                for (int i = 0; i < static_cast<int>(shared->options.size()); ++i) {
                    help.add_button(
                            ControlledButton::Props{
                                .text = shared->options[i],
                                .isSelected =
                                    [shared, i] { return shared->getInt && shared->getInt() == i; },
                            })
                        .on_pressed([shared, i] {
                            mDoAud_seStartMenu(kSoundItemChange);
                            if (shared->setInt) {
                                shared->setInt(i);
                            }
                        });
                }
            }
            if (!shared->helpRml.empty()) {
                help.add_rml(shared->helpRml);
            }
        });
    }
    return control;
}

ModWindow::ModWindow(Desc desc) : mDesc(std::move(desc)) {
    mRoot->SetAttribute("mod-id", mDesc.modId);
    for (int i = 0; i < static_cast<int>(mDesc.tabs.size()); ++i) {
        add_tab(mDesc.tabs[i].title, [this, i](Rml::Element* content) {
            mActiveTab = i;
            auto& left = add_child<Pane>(content, Pane::Type::Controlled);
            auto& right = add_child<Pane>(content, Pane::Type::Uncontrolled);
            if (mDesc.tabs[i].build) {
                mDesc.tabs[i].build(*this, left, right);
            }
        });
    }
    if (!mDesc.rcss.empty()) {
        set_document_styles(mDesc.rcss);
    }
}

ModWindow::~ModWindow() {
    if (mDesc.onDestroyed) {
        mDesc.onDestroyed();
    }
}

void ModWindow::update() {
    if (mActiveTab >= 0 && mActiveTab < static_cast<int>(mDesc.tabs.size()) &&
        mDesc.tabs[mActiveTab].update)
    {
        mDesc.tabs[mActiveTab].update();
    }
    Window::update();
}

}  // namespace dusk::ui
