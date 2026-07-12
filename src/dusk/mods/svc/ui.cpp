#include "ui.hpp"

#include "config.hpp"
#include "registry.hpp"
#include "slot_map.hpp"

#include "aurora/lib/logging.hpp"
#include "dusk/mod_loader.hpp"
#include "dusk/mods/loader/loader.hpp"
#include "dusk/ui/menu_bar.hpp"
#include "dusk/ui/mod_window.hpp"
#include "dusk/ui/modal.hpp"
#include "dusk/ui/ui.hpp"
#include "mods/svc/ui.h"

#include <RmlUi/Core.h>
#include <aurora/rmlui.hpp>
#include <fmt/format.h>

#include <algorithm>
#include <climits>
#include <cstdint>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace dusk::mods::svc::ui_impl {
namespace {

aurora::Module Log("dusk::mods::ui");

enum class UiSlotKind : u8 {
    Window,
    Dialog,
    Pane,
    Text,
    Progress,
    Control,
    Style,
    MenuTab,
};

const char* slot_kind_name(UiSlotKind kind) {
    switch (kind) {
    case UiSlotKind::Window:
        return "window";
    case UiSlotKind::Dialog:
        return "dialog";
    case UiSlotKind::Pane:
        return "pane";
    case UiSlotKind::Text:
        return "text";
    case UiSlotKind::Progress:
        return "progress";
    case UiSlotKind::Control:
        return "control";
    case UiSlotKind::Style:
        return "style";
    case UiSlotKind::MenuTab:
        return "menu tab";
    default:
        return "unknown";
    }
}

// Game thread only: all mutations happen in service calls made from mod code, in UI callbacks
// (ui::update), or in the loader's deactivate paths.
struct UiSlot {
    UiSlotKind kind = UiSlotKind::Window;
    // Pane/Text/Progress/Control: freed automatically when the element is destroyed
    Rml::Element* element = nullptr;
    // Pane payload
    ui::Pane* pane = nullptr;
    ui::Pane* helpPane = nullptr;
    // Window/Dialog payload (non-owning; the document stack owns the document)
    ui::Document* document = nullptr;
    UiWindowClosedFn onClosed = nullptr;
    void* onClosedUserData = nullptr;
    // Style payload
    ui::DocumentScope styleScope = ui::DocumentScope::None;
    std::string styleId;
    // Cached rendered values for element setters. These make the natural "set every update"
    // style cheap when the displayed value has not changed.
    std::string elementRml;
    float elementFloat = 0.0f;
    bool hasElementValue = false;
};

SlotMap<UiSlot> s_slots;

struct ModUiPanel {
    UiPanelBuildFn build = nullptr;
    UiPanelUpdateFn update = nullptr;
    void* userData = nullptr;
};
std::unordered_map<const LoadedMod*, ModUiPanel> s_modPanels;

struct ModMenuTab {
    uint64_t handle = 0;
    std::string label;
    UiPressedFn onSelected = nullptr;
    void* userData = nullptr;
};
std::unordered_map<const LoadedMod*, std::vector<ModMenuTab>> s_modMenuTabs;
bool s_menuTabsDirty = false;

UiSlot* slot_from_handle(uint64_t handle) {
    auto* entry = s_slots.find(handle);
    return entry != nullptr ? &entry->value : nullptr;
}

// Note: s_slots may reallocate on any later allocation, so callers must not hold the returned
// slot reference across calls that can allocate (e.g. mod build callbacks); re-resolve instead.
UiSlot& alloc_slot(LoadedMod& mod, UiSlotKind kind, uint64_t& outHandle) {
    outHandle = s_slots.emplace(mod, UiSlot{.kind = kind});
    return s_slots.find(outHandle)->value;
}

UiSlot* resolve(LoadedMod& mod, uint64_t handle, UiSlotKind kind, const char* what) {
    auto* entry = s_slots.find_owned(handle, mod);
    if (entry == nullptr || entry->value.kind != kind) {
        Log.error("[{}] {}: stale or invalid {} handle {:#x}", mod.metadata.id, what,
            slot_kind_name(kind), handle);
        return nullptr;
    }
    return &entry->value;
}

// Whether the registration a callback was created under is still live. Callbacks captured by
// host-owned UI must check this before calling into the mod: `mod->active` alone is true again
// once a reload completes, but captured fn pointers still target the unloaded image. Teardown
// frees the slots (ui_remove_mod), which invalidates every callback built under them.
bool slot_live(uint64_t handle) {
    return s_slots.find(handle) != nullptr;
}

bool dialog_open(uint64_t handle) {
    auto* slot = slot_from_handle(handle);
    return slot != nullptr && slot->kind == UiSlotKind::Dialog && slot->document != nullptr &&
           slot->document->active();
}

// Frees the slot when the tracked element is destroyed (tab rebuilds, window teardown, ...).
// The generation check makes a late detach of an already-recycled slot a no-op.
class SlotDetachListener final : public Rml::EventListener {
public:
    explicit SlotDetachListener(uint64_t handle) : m_handle{handle} {}

    void ProcessEvent(Rml::Event&) override {}

    void OnDetach(Rml::Element*) override {
        s_slots.erase(m_handle);
        delete this;
    }

private:
    uint64_t m_handle;
};

void track_element(uint64_t handle, UiSlot& slot, Rml::Element& element) {
    slot.element = &element;
    element.AddEventListener(Rml::EventId::Click, new SlotDetachListener{handle});
}

template <typename T, typename Fn>
T guarded_call(LoadedMod& mod, const char* what, T fallback, Fn&& fn) {
    if (!mod.active) {
        return fallback;
    }
    try {
        return fn();
    } catch (const std::exception& e) {
        fail_mod(mod, MOD_ERROR, fmt::format("exception in {}: {}", what, e.what()));
    } catch (...) {
        fail_mod(mod, MOD_ERROR, fmt::format("unknown exception in {}", what));
    }
    return fallback;
}

template <typename Fn>
void guarded_call(LoadedMod& mod, const char* what, Fn&& fn) {
    if (!mod.active) {
        return;
    }
    try {
        fn();
    } catch (const std::exception& e) {
        fail_mod(mod, MOD_ERROR, fmt::format("exception in {}: {}", what, e.what()));
    } catch (...) {
        fail_mod(mod, MOD_ERROR, fmt::format("unknown exception in {}", what));
    }
}

// Shared by panel/tab build and update callbacks: translates a non-OK result or an escaped
// exception into fail_mod, mirroring mod_update handling.
template <typename Fn>
void invoke_mod_ui_callback(LoadedMod& mod, const char* what, Fn&& fn) {
    ModError error = MOD_ERROR_INIT;
    const ModResult result = guarded_call(mod, what, MOD_OK, [&] { return fn(&error); });
    if (result != MOD_OK && mod.active) {
        fail_mod(
            mod, result, error.message[0] != '\0' ? error.message : fmt::format("{} failed", what));
    }
}

uint64_t wrap_pane(LoadedMod& mod, ui::Pane& pane, ui::Pane* helpPane) {
    uint64_t handle = 0;
    auto& slot = alloc_slot(mod, UiSlotKind::Pane, handle);
    slot.pane = &pane;
    slot.helpPane = helpPane;
    track_element(handle, slot, *pane.root());
    return handle;
}

int clamp_to_int(int64_t value) {
    return static_cast<int>(std::clamp<int64_t>(value, INT_MIN, INT_MAX));
}

std::function<bool()> wrap_predicate(
    LoadedMod& mod, UiPredicateFn fn, void* userData, uint64_t guardHandle) {
    if (fn == nullptr) {
        return {};
    }
    return [modPtr = &mod, fn, userData, guardHandle] {
        if (!slot_live(guardHandle)) {
            return false;
        }
        return guarded_call(*modPtr, "control predicate", false,
            [&] { return fn(modPtr->context.get(), userData); });
    };
}

void wire_callback_binding(
    LoadedMod& mod, const UiControlDesc& desc, ui::ModControlSpec& spec, uint64_t guardHandle) {
    auto* modPtr = &mod;
    const auto get = desc.get;
    const auto set = desc.set;
    auto* userData = desc.user_data;
    const auto getValue = [modPtr, get, userData, guardHandle] {
        UiControlValue value = UI_CONTROL_VALUE_INIT;
        if (!slot_live(guardHandle)) {
            return value;
        }
        guarded_call(*modPtr, "control getter", [&] {
            get(modPtr->context.get(), userData, &value);
        });
        return value;
    };
    const auto setValue = [modPtr, set, userData, guardHandle](const UiControlValue& value) {
        if (!slot_live(guardHandle)) {
            return;
        }
        guarded_call(*modPtr, "control setter", [&] {
            set(modPtr->context.get(), userData, &value);
        });
    };
    switch (desc.kind) {
    case UI_CONTROL_TOGGLE:
        spec.getBool = [getValue] { return getValue().bool_value; };
        spec.setBool = [setValue](bool value) {
            UiControlValue raw = UI_CONTROL_VALUE_INIT;
            raw.bool_value = value;
            setValue(raw);
        };
        break;
    case UI_CONTROL_NUMBER:
    case UI_CONTROL_SELECT:
        spec.getInt = [getValue] { return clamp_to_int(getValue().int_value); };
        spec.setInt = [setValue](int value) {
            UiControlValue raw = UI_CONTROL_VALUE_INIT;
            raw.int_value = value;
            setValue(raw);
        };
        break;
    case UI_CONTROL_STRING:
        spec.getString = [getValue]() -> Rml::String {
            const UiControlValue value = getValue();
            return value.string_value != nullptr ? value.string_value : "";
        };
        spec.setString = [setValue](Rml::String value) {
            UiControlValue raw = UI_CONTROL_VALUE_INIT;
            raw.string_value = value.c_str();
            setValue(raw);
        };
        break;
    default:
        break;
    }
}

// The lambdas re-resolve the var on every call, so a control whose var was unregistered
// mid-flight degrades to a no-op instead of a dangling read.
bool wire_config_var_binding(LoadedMod& mod, const UiControlDesc& desc, ui::ModControlSpec& spec) {
    auto* modPtr = &mod;
    const uint64_t varHandle = desc.config_var;
    switch (desc.kind) {
    case UI_CONTROL_TOGGLE: {
        const auto find = [modPtr, varHandle] {
            return static_cast<ConfigVar<bool>*>(
                config_find_var(*modPtr, varHandle, CONFIG_VAR_BOOL));
        };
        if (find() == nullptr) {
            return false;
        }
        spec.getBool = [find] {
            const auto* var = find();
            return var != nullptr && var->getValue();
        };
        spec.setBool = [find](bool value) {
            auto* var = find();
            if (var == nullptr || var->getValue() == value) {
                return;
            }
            var->setValue(value);
            config_mark_dirty();
        };
        if (!spec.isModified) {
            spec.isModified = [find] {
                const auto* var = find();
                return var != nullptr && var->getValue() != var->getDefaultValue();
            };
        }
        return true;
    }
    case UI_CONTROL_NUMBER:
    case UI_CONTROL_SELECT: {
        const auto find = [modPtr, varHandle] {
            return static_cast<ConfigVar<s64>*>(
                config_find_var(*modPtr, varHandle, CONFIG_VAR_INT));
        };
        if (find() == nullptr) {
            return false;
        }
        spec.getInt = [find] {
            const auto* var = find();
            return var != nullptr ? clamp_to_int(var->getValue()) : 0;
        };
        spec.setInt = [find](int value) {
            auto* var = find();
            if (var == nullptr || var->getValue() == value) {
                return;
            }
            var->setValue(value);
            config_mark_dirty();
        };
        if (!spec.isModified) {
            spec.isModified = [find] {
                const auto* var = find();
                return var != nullptr && var->getValue() != var->getDefaultValue();
            };
        }
        return true;
    }
    case UI_CONTROL_STRING: {
        const auto find = [modPtr, varHandle] {
            return static_cast<ConfigVar<std::string>*>(
                config_find_var(*modPtr, varHandle, CONFIG_VAR_STRING));
        };
        if (find() == nullptr) {
            return false;
        }
        spec.getString = [find]() -> Rml::String {
            const auto* var = find();
            return var != nullptr ? var->getValue() : "";
        };
        spec.setString = [find](Rml::String value) {
            auto* var = find();
            if (var == nullptr || var->getValue() == value) {
                return;
            }
            var->setValue(std::move(value));
            config_mark_dirty();
        };
        if (!spec.isModified) {
            spec.isModified = [find] {
                const auto* var = find();
                return var != nullptr && var->getValue() != var->getDefaultValue();
            };
        }
        return true;
    }
    default:
        return false;
    }
}

void on_mod_window_destroyed(uint64_t handle) {
    const auto* entry = s_slots.find(handle);
    if (entry == nullptr || entry->value.kind != UiSlotKind::Window) {
        return;
    }
    auto released = s_slots.take(handle);
    auto* mod = released->owner;
    const UiWindowClosedFn onClosed = released->value.onClosed;
    void* userData = released->value.onClosedUserData;
    if (mod != nullptr && onClosed != nullptr) {
        guarded_call(*mod, "window on_closed callback", [&] {
            onClosed(mod->context.get(), handle, userData);
        });
    }
}

void on_mod_dialog_destroyed(uint64_t handle) {
    auto* slot = slot_from_handle(handle);
    if (slot != nullptr && slot->kind == UiSlotKind::Dialog) {
        s_slots.erase(handle);
    }
}

class ModDialog final : public ui::Modal {
public:
    ModDialog(Props props, std::function<void()> onDestroyed)
        : Modal{std::move(props)}, m_onDestroyed{std::move(onDestroyed)} {}

    ~ModDialog() override {
        if (m_onDestroyed) {
            m_onDestroyed();
        }
    }

    void close() { pop(); }
    void force_close() { Document::hide(true); }

private:
    std::function<void()> m_onDestroyed;
};

void push_stacked_document(std::unique_ptr<ui::Document> document) {
    if (auto* previousTop = ui::top_document()) {
        previousTop->push(std::move(document));
    } else {
        ui::push_document(std::move(document));
    }
}

// Shared by dialog_push and dialog_add_action; the guard handle keeps a
// pressed callback from calling into a torn-down mod.
ui::ModalAction make_dialog_action(LoadedMod& mod, uint64_t handle, const UiDialogAction& action) {
    return {
        .label = action.label,
        .onPressed =
            [modPtr = &mod, handle, fn = action.on_pressed, userData = action.user_data,
                keepOpen = action.keep_open != 0](ui::Modal& modal) {
                if (!dialog_open(handle)) {
                    return;  // already being torn down
                }
                if (fn != nullptr) {
                    guarded_call(*modPtr, "dialog action callback", [&] {
                        fn(modPtr->context.get(), handle, userData);
                    });
                }
                // The callback may have closed the dialog already
                if (!keepOpen && dialog_open(handle)) {
                    static_cast<ModDialog&>(modal).close();
                }
            },
    };
}

}  // namespace

ModResult ui_register_mods_panel(LoadedMod& mod, const UiModsPanelDesc& desc) {
    s_modPanels[&mod] = {desc.build, desc.update, desc.user_data};
    return MOD_OK;
}

void ui_build_mods_panels(LoadedMod& mod, ui::Pane& pane) {
    const auto it = s_modPanels.find(&mod);
    if (it == s_modPanels.end()) {
        return;
    }
    const uint64_t paneHandle = wrap_pane(mod, pane, nullptr);
    const auto& panel = it->second;
    if (!mod.active || panel.build == nullptr) {
        return;
    }
    invoke_mod_ui_callback(mod, "mod UI panel build", [&](ModError* error) {
        return panel.build(mod.context.get(), paneHandle, panel.userData, error);
    });
}

void ui_update_mods_panels(LoadedMod& mod) {
    const auto it = s_modPanels.find(&mod);
    if (it == s_modPanels.end()) {
        return;
    }
    const auto& panel = it->second;
    if (!mod.active || panel.update == nullptr) {
        return;
    }
    invoke_mod_ui_callback(mod, "mod UI panel update", [&](ModError* error) {
        return panel.update(mod.context.get(), panel.userData, error);
    });
}

ModResult ui_pane_add_section(LoadedMod& mod, uint64_t pane, const char* title) {
    auto* slot = resolve(mod, pane, UiSlotKind::Pane, "pane_add_section");
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    slot->pane->add_section(title);
    return MOD_OK;
}

ModResult ui_pane_add_text(LoadedMod& mod, uint64_t pane, const char* text, uint64_t* outElem) {
    auto* slot = resolve(mod, pane, UiSlotKind::Pane, "pane_add_text");
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    auto* elem = slot->pane->add_text(text);
    if (outElem != nullptr) {
        auto& elemSlot = alloc_slot(mod, UiSlotKind::Text, *outElem);
        elemSlot.elementRml = ui::escape(text);
        elemSlot.hasElementValue = true;
        track_element(*outElem, elemSlot, *elem);
    }
    return MOD_OK;
}

ModResult ui_pane_add_rml(LoadedMod& mod, uint64_t pane, const char* rml, uint64_t* outElem) {
    auto* slot = resolve(mod, pane, UiSlotKind::Pane, "pane_add_rml");
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    auto* elem = slot->pane->add_rml(rml);
    if (outElem != nullptr) {
        auto& elemSlot = alloc_slot(mod, UiSlotKind::Text, *outElem);
        elemSlot.elementRml = rml;
        elemSlot.hasElementValue = true;
        track_element(*outElem, elemSlot, *elem);
    }
    return MOD_OK;
}

ModResult ui_pane_add_progress(LoadedMod& mod, uint64_t pane, float value, uint64_t* outElem) {
    auto* slot = resolve(mod, pane, UiSlotKind::Pane, "pane_add_progress");
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    auto* elem = ui::append(slot->element, "progress");
    elem->SetAttribute("value", value);
    if (outElem != nullptr) {
        auto& elemSlot = alloc_slot(mod, UiSlotKind::Progress, *outElem);
        elemSlot.elementFloat = value;
        elemSlot.hasElementValue = true;
        track_element(*outElem, elemSlot, *elem);
    }
    return MOD_OK;
}

ModResult ui_pane_add_control(
    LoadedMod& mod, uint64_t pane, const UiControlDesc& desc, uint64_t* outElem) {
    auto* slot = resolve(mod, pane, UiSlotKind::Pane, "pane_add_control");
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }

    ui::ModControlSpec spec;
    spec.label = desc.label;
    spec.helpRml = desc.help_rml != nullptr ? desc.help_rml : "";
    spec.isDisabled = wrap_predicate(mod, desc.is_disabled, desc.user_data, pane);
    spec.isModified = wrap_predicate(mod, desc.is_modified, desc.user_data, pane);
    switch (desc.kind) {
    case UI_CONTROL_BUTTON:
        spec.kind = ui::ModControlSpec::Kind::Button;
        spec.onPressed = [modPtr = &mod, fn = desc.on_pressed, userData = desc.user_data,
                             guardHandle = pane] {
            if (!slot_live(guardHandle)) {
                return;
            }
            guarded_call(*modPtr, "control on_pressed callback", [&] {
                fn(modPtr->context.get(), userData);
            });
        };
        break;
    case UI_CONTROL_TOGGLE:
        spec.kind = ui::ModControlSpec::Kind::Toggle;
        break;
    case UI_CONTROL_NUMBER:
        spec.kind = ui::ModControlSpec::Kind::Number;
        if (desc.min != desc.max) {
            spec.min = clamp_to_int(desc.min);
            spec.max = clamp_to_int(desc.max);
            if (spec.max < spec.min) {
                std::swap(spec.min, spec.max);
            }
        }
        spec.step = desc.step < 1 ? 1 : clamp_to_int(desc.step);
        spec.prefix = desc.prefix != nullptr ? desc.prefix : "";
        spec.suffix = desc.suffix != nullptr ? desc.suffix : "";
        break;
    case UI_CONTROL_STRING:
        spec.kind = ui::ModControlSpec::Kind::String;
        spec.maxLength = desc.max_length < 1 ? -1 : desc.max_length;
        break;
    case UI_CONTROL_SELECT:
        spec.kind = ui::ModControlSpec::Kind::Select;
        if (slot->helpPane == nullptr) {
            Log.error("[{}] pane_add_control: SELECT controls need a help pane (mod window tabs)",
                mod.metadata.id);
            return MOD_UNSUPPORTED;
        }
        for (size_t i = 0; i < desc.option_count; ++i) {
            spec.options.emplace_back(desc.options[i]);
        }
        break;
    default:
        return MOD_INVALID_ARGUMENT;
    }

    if (desc.kind != UI_CONTROL_BUTTON) {
        if (desc.binding == UI_BINDING_CONFIG_VAR) {
            if (!wire_config_var_binding(mod, desc, spec)) {
                Log.error("[{}] pane_add_control: config var handle {:#x} is unknown or its type "
                          "does not match the control kind",
                    mod.metadata.id, desc.config_var);
                return MOD_INVALID_ARGUMENT;
            }
        } else {
            wire_callback_binding(mod, desc, spec, pane);
        }
    }

    // Copy the pane pointers out: allocating the control's slot below may reallocate s_slots
    auto* paneComponent = slot->pane;
    auto* helpPane = slot->helpPane;
    auto* control = ui::build_mod_control(*paneComponent, helpPane, std::move(spec));
    if (control == nullptr) {
        return MOD_UNSUPPORTED;
    }
    if (outElem != nullptr) {
        auto& elemSlot = alloc_slot(mod, UiSlotKind::Control, *outElem);
        track_element(*outElem, elemSlot, *control->root());
    }
    return MOD_OK;
}

ModResult ui_elem_set_text(LoadedMod& mod, uint64_t elem, const char* text) {
    auto* slot = resolve(mod, elem, UiSlotKind::Text, "elem_set_text");
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    const std::string rml = ui::escape(text);
    if (slot->hasElementValue && slot->elementRml == rml) {
        return MOD_OK;
    }
    slot->elementRml = rml;
    slot->hasElementValue = true;
    slot->element->SetInnerRML(slot->elementRml);
    return MOD_OK;
}

ModResult ui_elem_set_rml(LoadedMod& mod, uint64_t elem, const char* rml) {
    auto* slot = resolve(mod, elem, UiSlotKind::Text, "elem_set_rml");
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    if (slot->hasElementValue && slot->elementRml == rml) {
        return MOD_OK;
    }
    slot->elementRml = rml;
    slot->hasElementValue = true;
    slot->element->SetInnerRML(rml);
    return MOD_OK;
}

ModResult ui_elem_set_progress(LoadedMod& mod, uint64_t elem, float value) {
    auto* slot = resolve(mod, elem, UiSlotKind::Progress, "elem_set_progress");
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    if (slot->hasElementValue && slot->elementFloat == value) {
        return MOD_OK;
    }
    slot->elementFloat = value;
    slot->hasElementValue = true;
    slot->element->SetAttribute("value", value);
    return MOD_OK;
}

ModResult ui_elem_set_class(LoadedMod& mod, uint64_t elem, const char* name, bool active) {
    auto* entry = s_slots.find_owned(elem, mod);
    if (entry == nullptr || entry->value.element == nullptr) {
        Log.error(
            "[{}] elem_set_class: stale or invalid element handle {:#x}", mod.metadata.id, elem);
        return MOD_INVALID_ARGUMENT;
    }
    entry->value.element->SetClass(name, active);
    return MOD_OK;
}

ModResult ui_window_push(LoadedMod& mod, const UiWindowDesc& desc, uint64_t& outHandle) {
    outHandle = 0;
    if (!aurora::rmlui::is_initialized()) {
        return MOD_UNAVAILABLE;
    }
    if (desc.rcss != nullptr && desc.rcss[0] != '\0' &&
        Rml::Factory::InstanceStyleSheetString(desc.rcss) == nullptr)
    {
        Log.error("[{}] window_push: failed to parse window RCSS", mod.metadata.id);
        return MOD_INVALID_ARGUMENT;
    }

    uint64_t handle = 0;
    {
        auto& slot = alloc_slot(mod, UiSlotKind::Window, handle);
        slot.onClosed = desc.on_closed;
        slot.onClosedUserData = desc.user_data;
    }

    ui::ModWindow::Desc windowDesc;
    windowDesc.modId = mod.metadata.id;
    windowDesc.rcss = desc.rcss != nullptr ? desc.rcss : "";
    windowDesc.onDestroyed = [handle] { on_mod_window_destroyed(handle); };
    for (size_t i = 0; i < desc.tab_count; ++i) {
        const UiTabDesc& tab = desc.tabs[i];
        ui::ModWindow::Tab hostTab;
        hostTab.title = tab.title;
        hostTab.build = [modPtr = &mod, handle, build = tab.build, userData = tab.user_data](
                            ui::ModWindow&, ui::Pane& left, ui::Pane& right) {
            if (build == nullptr || !slot_live(handle) || !modPtr->active) {
                return;
            }
            const uint64_t leftHandle = wrap_pane(*modPtr, left, &right);
            const uint64_t rightHandle = wrap_pane(*modPtr, right, nullptr);
            invoke_mod_ui_callback(*modPtr, "mod UI tab build", [&](ModError* error) {
                return build(
                    modPtr->context.get(), handle, leftHandle, rightHandle, userData, error);
            });
        };
        if (tab.update != nullptr) {
            hostTab.update = [modPtr = &mod, handle, update = tab.update,
                                 userData = tab.user_data] {
                if (!slot_live(handle) || !modPtr->active) {
                    return;
                }
                invoke_mod_ui_callback(*modPtr, "mod UI tab update", [&](ModError* error) {
                    return update(modPtr->context.get(), userData, error);
                });
            };
        }
        windowDesc.tabs.push_back(std::move(hostTab));
    }

    // The first tab builds during construction, which can allocate slots; only
    // re-resolve the window slot afterwards.
    auto window = std::make_unique<ui::ModWindow>(std::move(windowDesc));
    if (auto* slot = slot_from_handle(handle)) {
        slot->document = window.get();
    }
    push_stacked_document(std::move(window));
    outHandle = handle;
    return MOD_OK;
}

ModResult ui_window_close(LoadedMod& mod, uint64_t handle) {
    auto* slot = resolve(mod, handle, UiSlotKind::Window, "window_close");
    if (slot == nullptr || slot->document == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    slot->document->hide(true);
    return MOD_OK;
}

ModResult ui_dialog_push(LoadedMod& mod, const UiDialogDesc& desc, uint64_t& outHandle) {
    outHandle = 0;
    if (!aurora::rmlui::is_initialized()) {
        return MOD_UNAVAILABLE;
    }
    uint64_t handle = 0;
    alloc_slot(mod, UiSlotKind::Dialog, handle);

    const char* defaultIcon = "";
    ui::Modal::Props props;
    switch (desc.variant) {
    case UI_DIALOG_WARNING:
        defaultIcon = "warning";
        break;
    case UI_DIALOG_DANGER:
        props.variant = "danger";
        defaultIcon = "error";
        break;
    default:
        break;
    }
    props.title = ui::escape(desc.title);
    props.bodyRml = desc.body_rml;
    props.icon = desc.icon != nullptr ? desc.icon : defaultIcon;
    props.onDismiss = [modPtr = &mod, handle, fn = desc.on_dismiss, userData = desc.user_data](
                          ui::Modal& modal) {
        if (!dialog_open(handle)) {
            return;  // already being torn down
        }
        if (fn != nullptr) {
            guarded_call(*modPtr, "dialog on_dismiss callback", [&] {
                fn(modPtr->context.get(), handle, userData);
            });
        }
        if (dialog_open(handle)) {
            static_cast<ModDialog&>(modal).close();
        }
    };
    for (size_t i = 0; i < desc.action_count; ++i) {
        props.actions.push_back(make_dialog_action(mod, handle, desc.actions[i]));
    }

    auto dialog =
        std::make_unique<ModDialog>(std::move(props), [handle] { on_mod_dialog_destroyed(handle); });
    if (auto* slot = slot_from_handle(handle)) {
        slot->document = dialog.get();
    }
    push_stacked_document(std::move(dialog));
    outHandle = handle;
    return MOD_OK;
}

ModResult ui_dialog_close(LoadedMod& mod, uint64_t handle) {
    auto* slot = resolve(mod, handle, UiSlotKind::Dialog, "dialog_close");
    if (slot == nullptr || slot->document == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    // Programmatic close: no dismiss notification, no sound
    static_cast<ModDialog*>(slot->document)->close();
    return MOD_OK;
}

ModResult ui_dialog_set_body(LoadedMod& mod, uint64_t handle, const char* rml) {
    auto* slot = resolve(mod, handle, UiSlotKind::Dialog, "dialog_set_body");
    if (slot == nullptr || slot->document == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    static_cast<ModDialog*>(slot->document)->set_body(rml);
    return MOD_OK;
}

ModResult ui_dialog_set_icon(LoadedMod& mod, uint64_t handle, const char* icon) {
    auto* slot = resolve(mod, handle, UiSlotKind::Dialog, "dialog_set_icon");
    if (slot == nullptr || slot->document == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    static_cast<ModDialog*>(slot->document)->set_icon(icon);
    return MOD_OK;
}

ModResult ui_dialog_add_action(LoadedMod& mod, uint64_t handle, const UiDialogAction& action) {
    auto* slot = resolve(mod, handle, UiSlotKind::Dialog, "dialog_add_action");
    if (slot == nullptr || slot->document == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    static_cast<ModDialog*>(slot->document)->add_action(make_dialog_action(mod, handle, action));
    return MOD_OK;
}

ModResult ui_register_menu_tab(LoadedMod& mod, const UiMenuTabDesc& desc, uint64_t& outHandle) {
    outHandle = 0;
    for (const auto& [owner, tabs] : s_modMenuTabs) {
        for (const auto& tab : tabs) {
            if (owner != &mod && tab.label == desc.label) {
                Log.warn("[{}] register_menu_tab: label '{}' is already used by [{}]",
                    mod.metadata.id, desc.label, owner->metadata.id);
            }
        }
    }
    uint64_t handle = 0;
    alloc_slot(mod, UiSlotKind::MenuTab, handle);
    s_modMenuTabs[&mod].push_back({.handle = handle,
        .label = desc.label,
        .onSelected = desc.on_selected,
        .userData = desc.user_data});
    s_menuTabsDirty = true;
    outHandle = handle;
    return MOD_OK;
}

ModResult ui_unregister_menu_tab(LoadedMod& mod, uint64_t handle) {
    auto* slot = resolve(mod, handle, UiSlotKind::MenuTab, "unregister_menu_tab");
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    const auto it = s_modMenuTabs.find(&mod);
    if (it != s_modMenuTabs.end()) {
        std::erase_if(it->second, [&](const auto& tab) { return tab.handle == handle; });
        if (it->second.empty()) {
            s_modMenuTabs.erase(it);
        }
    }
    s_slots.erase_owned(handle, mod);
    s_menuTabsDirty = true;
    return MOD_OK;
}

std::vector<ModMenuTabEntry> ui_mod_menu_tabs() {
    // The consumer (a MenuBar being constructed) now reflects the current tab
    // set, so a pending rebuild for earlier mutations is moot.
    s_menuTabsDirty = false;
    std::vector<ModMenuTabEntry> entries;
    for (auto& mod : ModLoader::instance().mods()) {
        if (!mod.active) {
            continue;
        }
        const auto it = s_modMenuTabs.find(&mod);
        if (it == s_modMenuTabs.end()) {
            continue;
        }
        for (const auto& tab : it->second) {
            entries.push_back({.label = tab.label,
                .onSelected = [modPtr = &mod, handle = tab.handle, fn = tab.onSelected,
                                  userData = tab.userData] {
                    if (!slot_live(handle) || !modPtr->active) {
                        return;  // registered by a since-unloaded mod image
                    }
                    guarded_call(*modPtr, "menu tab on_selected callback", [&] {
                        fn(modPtr->context.get(), userData);
                    });
                }});
        }
    }
    return entries;
}

void ui_sync_menu_tabs() {
    if (!s_menuTabsDirty) {
        return;
    }
    s_menuTabsDirty = false;
    if (aurora::rmlui::is_initialized()) {
        ui::MenuBar::rebuild();
    }
}

bool ui_any_document_visible() {
    return ui::any_document_visible();
}

ModResult ui_register_styles(
    LoadedMod& mod, uint32_t scope, const char* rcss, uint64_t& outHandle) {
    outHandle = 0;
    ui::DocumentScope docScope;
    switch (scope) {
    case UI_SCOPE_PRELAUNCH:
        docScope = ui::DocumentScope::Prelaunch;
        break;
    case UI_SCOPE_WINDOW:
        docScope = ui::DocumentScope::Window;
        break;
    case UI_SCOPE_MENU_BAR:
        docScope = ui::DocumentScope::MenuBar;
        break;
    case UI_SCOPE_OVERLAY:
        docScope = ui::DocumentScope::Overlay;
        break;
    case UI_SCOPE_TOUCH_CONTROLS:
        docScope = ui::DocumentScope::TouchControls;
        break;
    case UI_SCOPE_GRAPHICS_TUNER:
        docScope = ui::DocumentScope::GraphicsTuner;
        break;
    default:
        return MOD_INVALID_ARGUMENT;
    }

    uint64_t handle = 0;
    auto& slot = alloc_slot(mod, UiSlotKind::Style, handle);
    slot.styleScope = docScope;
    slot.styleId = fmt::format("{}:{:x}", mod.metadata.id, handle);
    if (!ui::register_scoped_styles(docScope, slot.styleId, rcss)) {
        Log.error("[{}] register_styles: failed to parse RCSS", mod.metadata.id);
        s_slots.erase(handle);
        return MOD_INVALID_ARGUMENT;
    }
    outHandle = handle;
    return MOD_OK;
}

ModResult ui_register_styles_file(
    LoadedMod& mod, uint32_t scope, const char* path, uint64_t& outHandle) {
    outHandle = 0;
    if (mod.bundle == nullptr) {
        return MOD_UNAVAILABLE;
    }
    std::vector<u8> data;
    const std::string entry = std::string{"res/"} + path;
    try {
        data = mod.bundle->readFile(entry);
    } catch (const std::runtime_error& e) {
        Log.error("[{}] register_styles_file '{}' failed: {}", mod.metadata.id, entry, e.what());
        return MOD_UNAVAILABLE;
    }
    const std::string rcss{data.begin(), data.end()};
    return ui_register_styles(mod, scope, rcss.c_str(), outHandle);
}

ModResult ui_unregister_styles(LoadedMod& mod, uint64_t handle) {
    auto* slot = resolve(mod, handle, UiSlotKind::Style, "unregister_styles");
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    auto released = s_slots.take_owned(handle, mod);
    ui::unregister_scoped_styles(released->value.styleScope, released->value.styleId);
    return MOD_OK;
}

void ui_remove_mod(LoadedMod& mod) {
    s_modPanels.erase(&mod);
    if (s_modMenuTabs.erase(&mod) != 0) {
        s_menuTabsDirty = true;
    }
    auto entries = s_slots.take_all(mod);
    for (auto& entry : entries) {
        auto& slot = entry.value;
        switch (slot.kind) {
        case UiSlotKind::Window: {
            auto* window = static_cast<ui::ModWindow*>(slot.document);
            if (window != nullptr) {
                window->force_close();
            }
            break;
        }
        case UiSlotKind::Dialog: {
            auto* dialog = static_cast<ModDialog*>(slot.document);
            if (dialog != nullptr) {
                dialog->force_close();
            }
            break;
        }
        case UiSlotKind::Style:
            ui::unregister_scoped_styles(slot.styleScope, slot.styleId);
            break;
        default:
            break;
        }
    }
}

}  // namespace dusk::mods::svc::ui_impl

namespace dusk::mods::svc {
namespace {

// Validation of the tagged control descriptor: required fields per kind/binding. Value
// translation and cvar wiring live in loader/ui.cpp.
bool valid_control_desc(const UiControlDesc& desc) {
    if (desc.struct_size < sizeof(UiControlDesc) || desc.label == nullptr) {
        return false;
    }
    switch (desc.kind) {
    case UI_CONTROL_BUTTON:
        return desc.on_pressed != nullptr;
    case UI_CONTROL_TOGGLE:
    case UI_CONTROL_NUMBER:
    case UI_CONTROL_STRING:
    case UI_CONTROL_SELECT:
        break;
    default:
        return false;
    }
    if (desc.kind == UI_CONTROL_SELECT) {
        if (desc.options == nullptr || desc.option_count == 0) {
            return false;
        }
        for (size_t i = 0; i < desc.option_count; ++i) {
            if (desc.options[i] == nullptr) {
                return false;
            }
        }
    }
    switch (desc.binding) {
    case UI_BINDING_CALLBACKS:
        return desc.get != nullptr && desc.set != nullptr;
    case UI_BINDING_CONFIG_VAR:
        return desc.config_var != 0;
    default:
        return false;
    }
}

ModResult ui_register_mods_panel(ModContext* context, const UiModsPanelDesc* desc) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(UiModsPanelDesc) ||
        desc->build == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_register_mods_panel(*mod, *desc);
}

ModResult ui_pane_add_section(ModContext* context, UiElementHandle pane, const char* title) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || pane == 0 || title == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_pane_add_section(*mod, pane, title);
}

ModResult ui_pane_add_text(
    ModContext* context, UiElementHandle pane, const char* text, UiElementHandle* outElem) {
    if (outElem != nullptr) {
        *outElem = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || pane == 0 || text == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_pane_add_text(*mod, pane, text, outElem);
}

ModResult ui_pane_add_rml(
    ModContext* context, UiElementHandle pane, const char* rml, UiElementHandle* outElem) {
    if (outElem != nullptr) {
        *outElem = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || pane == 0 || rml == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_pane_add_rml(*mod, pane, rml, outElem);
}

ModResult ui_pane_add_progress(
    ModContext* context, UiElementHandle pane, float value, UiElementHandle* outElem) {
    if (outElem != nullptr) {
        *outElem = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || pane == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_pane_add_progress(*mod, pane, value, outElem);
}

ModResult ui_pane_add_control(ModContext* context, UiElementHandle pane, const UiControlDesc* desc,
    UiElementHandle* outElem) {
    if (outElem != nullptr) {
        *outElem = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || pane == 0 || desc == nullptr || !valid_control_desc(*desc)) {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_pane_add_control(*mod, pane, *desc, outElem);
}

ModResult ui_elem_set_text(ModContext* context, UiElementHandle elem, const char* text) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || elem == 0 || text == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_elem_set_text(*mod, elem, text);
}

ModResult ui_elem_set_rml(ModContext* context, UiElementHandle elem, const char* rml) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || elem == 0 || rml == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_elem_set_rml(*mod, elem, rml);
}

ModResult ui_elem_set_progress(ModContext* context, UiElementHandle elem, float value) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || elem == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_elem_set_progress(*mod, elem, value);
}

ModResult ui_elem_set_class(
    ModContext* context, UiElementHandle elem, const char* name, bool active) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || elem == 0 || name == nullptr || name[0] == '\0') {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_elem_set_class(*mod, elem, name, active);
}

ModResult ui_window_push(ModContext* context, const UiWindowDesc* desc, UiWindowHandle* outWindow) {
    if (outWindow != nullptr) {
        *outWindow = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(UiWindowDesc) ||
        desc->tabs == nullptr || desc->tab_count == 0)
    {
        return MOD_INVALID_ARGUMENT;
    }
    for (size_t i = 0; i < desc->tab_count; ++i) {
        const UiTabDesc& tab = desc->tabs[i];
        if (tab.struct_size < sizeof(UiTabDesc) || tab.title == nullptr || tab.build == nullptr) {
            return MOD_INVALID_ARGUMENT;
        }
    }
    uint64_t handle = 0;
    const auto result = ui_impl::ui_window_push(*mod, *desc, handle);
    if (result == MOD_OK && outWindow != nullptr) {
        *outWindow = handle;
    }
    return result;
}

ModResult ui_window_close(ModContext* context, UiWindowHandle window) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || window == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_window_close(*mod, window);
}

ModResult ui_dialog_push(ModContext* context, const UiDialogDesc* desc, UiDialogHandle* outDialog) {
    if (outDialog != nullptr) {
        *outDialog = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(UiDialogDesc) ||
        desc->title == nullptr || desc->body_rml == nullptr || desc->actions == nullptr ||
        desc->action_count == 0 || desc->variant > UI_DIALOG_DANGER)
    {
        return MOD_INVALID_ARGUMENT;
    }
    for (size_t i = 0; i < desc->action_count; ++i) {
        if (desc->actions[i].label == nullptr) {
            return MOD_INVALID_ARGUMENT;
        }
    }
    uint64_t handle = 0;
    const auto result = ui_impl::ui_dialog_push(*mod, *desc, handle);
    if (result == MOD_OK && outDialog != nullptr) {
        *outDialog = handle;
    }
    return result;
}

ModResult ui_dialog_close(ModContext* context, UiDialogHandle dialog) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || dialog == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_dialog_close(*mod, dialog);
}

ModResult ui_is_any_document_visible(ModContext* context, bool* outVisible) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outVisible == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    *outVisible = ui_impl::ui_any_document_visible();
    return MOD_OK;
}

ModResult ui_register_styles(
    ModContext* context, UiStyleScope scope, const char* rcss, UiStyleHandle* outStyle) {
    if (outStyle != nullptr) {
        *outStyle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || rcss == nullptr || scope > UI_SCOPE_GRAPHICS_TUNER) {
        return MOD_INVALID_ARGUMENT;
    }
    uint64_t handle = 0;
    const auto result = ui_impl::ui_register_styles(*mod, scope, rcss, handle);
    if (result == MOD_OK && outStyle != nullptr) {
        *outStyle = handle;
    }
    return result;
}

ModResult ui_register_styles_file(
    ModContext* context, UiStyleScope scope, const char* path, UiStyleHandle* outStyle) {
    if (outStyle != nullptr) {
        *outStyle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || path == nullptr || !is_safe_resource_path(path) ||
        scope > UI_SCOPE_GRAPHICS_TUNER)
    {
        return MOD_INVALID_ARGUMENT;
    }
    uint64_t handle = 0;
    const auto result = ui_impl::ui_register_styles_file(*mod, scope, path, handle);
    if (result == MOD_OK && outStyle != nullptr) {
        *outStyle = handle;
    }
    return result;
}

ModResult ui_unregister_styles(ModContext* context, UiStyleHandle style) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || style == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_unregister_styles(*mod, style);
}

ModResult ui_register_menu_tab(
    ModContext* context, const UiMenuTabDesc* desc, UiMenuTabHandle* outTab) {
    if (outTab != nullptr) {
        *outTab = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(UiMenuTabDesc) ||
        desc->label == nullptr || desc->label[0] == '\0' || desc->on_selected == nullptr)
    {
        return MOD_INVALID_ARGUMENT;
    }
    uint64_t handle = 0;
    const auto result = ui_impl::ui_register_menu_tab(*mod, *desc, handle);
    if (result == MOD_OK && outTab != nullptr) {
        *outTab = handle;
    }
    return result;
}

ModResult ui_unregister_menu_tab(ModContext* context, UiMenuTabHandle tab) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || tab == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_unregister_menu_tab(*mod, tab);
}

ModResult ui_dialog_set_body(ModContext* context, UiDialogHandle dialog, const char* bodyRml) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || dialog == 0 || bodyRml == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_dialog_set_body(*mod, dialog, bodyRml);
}

ModResult ui_dialog_set_icon(ModContext* context, UiDialogHandle dialog, const char* icon) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || dialog == 0 || icon == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_dialog_set_icon(*mod, dialog, icon);
}

ModResult ui_dialog_add_action(
    ModContext* context, UiDialogHandle dialog, const UiDialogAction* action) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || dialog == 0 || action == nullptr || action->label == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return ui_impl::ui_dialog_add_action(*mod, dialog, *action);
}

constexpr UiService s_uiService{
    .header = SERVICE_HEADER(UiService, UI_SERVICE_MAJOR, UI_SERVICE_MINOR),
    .register_mods_panel = ui_register_mods_panel,
    .pane_add_section = ui_pane_add_section,
    .pane_add_text = ui_pane_add_text,
    .pane_add_rml = ui_pane_add_rml,
    .pane_add_progress = ui_pane_add_progress,
    .pane_add_control = ui_pane_add_control,
    .elem_set_text = ui_elem_set_text,
    .elem_set_rml = ui_elem_set_rml,
    .elem_set_progress = ui_elem_set_progress,
    .elem_set_class = ui_elem_set_class,
    .window_push = ui_window_push,
    .window_close = ui_window_close,
    .dialog_push = ui_dialog_push,
    .dialog_close = ui_dialog_close,
    .dialog_set_body = ui_dialog_set_body,
    .dialog_set_icon = ui_dialog_set_icon,
    .dialog_add_action = ui_dialog_add_action,
    .is_any_document_visible = ui_is_any_document_visible,
    .register_styles = ui_register_styles,
    .register_styles_file = ui_register_styles_file,
    .unregister_styles = ui_unregister_styles,
    .register_menu_tab = ui_register_menu_tab,
    .unregister_menu_tab = ui_unregister_menu_tab,
};

}  // namespace

void ui_build_mods_panels(LoadedMod& mod, ui::Pane& pane) {
    ui_impl::ui_build_mods_panels(mod, pane);
}

void ui_update_mods_panels(LoadedMod& mod) {
    ui_impl::ui_update_mods_panels(mod);
}

std::vector<ModMenuTabEntry> ui_mod_menu_tabs() {
    return ui_impl::ui_mod_menu_tabs();
}

constinit const ServiceModule g_uiModule{
    .id = UI_SERVICE_ID,
    .majorVersion = UI_SERVICE_MAJOR,
    .minorVersion = UI_SERVICE_MINOR,
    .service = &s_uiService,
    .modDetached = ui_impl::ui_remove_mod,
    .frameEnd = ui_impl::ui_sync_menu_tabs,
};

}  // namespace dusk::mods::svc
