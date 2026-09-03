#include "runtime.hpp"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

namespace luau_runtime {
namespace {

constexpr char kUiWindowMetatable[] = "dusklight.ui_window";
constexpr char kUiDialogMetatable[] = "dusklight.ui_dialog";
constexpr char kUiElementMetatable[] = "dusklight.ui_element";
constexpr char kUiStyleMetatable[] = "dusklight.ui_style";
constexpr char kUiMenuTabMetatable[] = "dusklight.ui_menu_tab";
constexpr char kUiListMetatable[] = "dusklight.ui_list";

const char* ui_metatable(HandleKind kind) {
    switch (kind) {
    case HandleKind::UiWindow:
        return kUiWindowMetatable;
    case HandleKind::UiDialog:
        return kUiDialogMetatable;
    case HandleKind::UiStyle:
        return kUiStyleMetatable;
    case HandleKind::UiMenuTab:
        return kUiMenuTabMetatable;
    case HandleKind::UiList:
        return kUiListMetatable;
    default:
        return kUiElementMetatable;
    }
}

ScriptHandle& check_ui_handle(lua_State* state, int index, HandleKind kind) {
    return check_handle(state, index, ui_metatable(kind), kind);
}

ScriptHandle& check_config_var(lua_State* state, int index) {
    luaL_checktype(state, index, LUA_TUSERDATA);
    auto* handle = static_cast<ScriptHandle*>(lua_touserdata(state, index));
    if (handle == nullptr || handle->kind != HandleKind::ConfigVar || handle->value == 0 ||
        handle->vm == nullptr)
    {
        luaL_argerror(state, index, "expected a live ConfigVar");
    }
    return *handle;
}

bool call_callback(Callback& callback, int ref, int argumentCount, int resultCount,
    const char* name, std::string* outError = nullptr) {
    std::string error;
    if (call_ref(*callback.vm, ref, argumentCount, resultCount, kCallbackBudget, error)) {
        return true;
    }
    if (outError != nullptr) {
        *outError = std::move(error);
    } else {
        fail_callback(*callback.vm, name, error);
    }
    return false;
}

ModResult call_build(
    Callback& callback, int ref, int argumentCount, const char* name, ModError* outError) {
    std::string error;
    if (call_callback(callback, ref, argumentCount, 0, name, &error)) {
        return MOD_OK;
    }
    return set_error(outError, MOD_ERROR, error);
}

bool call_predicate(Callback& callback, int ref, int argumentCount, const char* name) {
    if (!call_callback(callback, ref, argumentCount, 1, name)) {
        return false;
    }
    lua_State* state = callback.vm->state;
    const bool result = lua_toboolean(state, -1) != 0;
    lua_pop(state, 1);
    return result;
}

void control_get(ModContext*, void* userData, UiControlValue* outValue) {
    auto& callback = *static_cast<Callback*>(userData);
    if (outValue == nullptr || !call_callback(callback, callback.refs[0], 0, 1, "control get")) {
        return;
    }
    lua_State* state = callback.vm->state;
    switch (static_cast<UiControlKind>(callback.tag)) {
    case UI_CONTROL_TOGGLE:
        outValue->bool_value = lua_toboolean(state, -1) != 0;
        break;
    case UI_CONTROL_NUMBER:
    case UI_CONTROL_SELECT: {
        int64_t value = 0;
        if (!to_int64(state, -1, value)) {
            fail_callback(*callback.vm, "control get", "callback must return an integer");
        } else {
            outValue->int_value = value;
        }
        break;
    }
    case UI_CONTROL_STRING:
    case UI_CONTROL_COLOR:
    case UI_CONTROL_FILE_PICKER: {
        size_t length = 0;
        const char* value = lua_tolstring(state, -1, &length);
        if (value == nullptr) {
            fail_callback(*callback.vm, "control get", "callback must return a string");
        } else {
            callback.returnedString.assign(value, length);
            outValue->string_value = callback.returnedString.c_str();
        }
        break;
    }
    default:
        break;
    }
    lua_pop(state, 1);
}

void control_set(ModContext*, void* userData, const UiControlValue* value) {
    auto& callback = *static_cast<Callback*>(userData);
    if (value == nullptr) {
        return;
    }
    lua_State* state = callback.vm->state;
    switch (static_cast<UiControlKind>(callback.tag)) {
    case UI_CONTROL_TOGGLE:
        lua_pushboolean(state, value->bool_value);
        break;
    case UI_CONTROL_NUMBER:
    case UI_CONTROL_SELECT:
        lua_pushinteger64(state, value->int_value);
        break;
    case UI_CONTROL_STRING:
    case UI_CONTROL_COLOR:
    case UI_CONTROL_FILE_PICKER:
        lua_pushstring(state, value->string_value != nullptr ? value->string_value : "");
        break;
    default:
        return;
    }
    call_callback(callback, callback.refs[1], 1, 0, "control set");
}

bool control_disabled(ModContext*, void* userData) {
    auto& callback = *static_cast<Callback*>(userData);
    return call_predicate(callback, callback.refs[3], 0, "control is_disabled");
}

bool control_modified(ModContext*, void* userData) {
    auto& callback = *static_cast<Callback*>(userData);
    return call_predicate(callback, callback.refs[4], 0, "control is_modified");
}

bool control_selected(ModContext*, void* userData) {
    auto& callback = *static_cast<Callback*>(userData);
    return call_predicate(callback, callback.refs[5], 0, "control is_selected");
}

void control_pressed(ModContext*, void* userData) {
    auto& callback = *static_cast<Callback*>(userData);
    call_callback(callback, callback.refs[2], 0, 0, "control on_pressed");
}

ModResult panel_build(ModContext*, UiElementHandle pane, void* userData, ModError* outError) {
    auto& callback = *static_cast<Callback*>(userData);
    push_ui_handle(callback.vm->state, *callback.vm, pane, HandleKind::UiElement);
    return call_build(callback, callback.refs[0], 1, "panel build", outError);
}

ModResult panel_update(ModContext*, void* userData, ModError* outError) {
    auto& callback = *static_cast<Callback*>(userData);
    return call_build(callback, callback.refs[1], 0, "panel update", outError);
}

ModResult group_build(ModContext*, UiElementHandle pane, void* userData, ModError* outError) {
    auto& callback = *static_cast<Callback*>(userData);
    push_ui_handle(callback.vm->state, *callback.vm, pane, HandleKind::UiElement);
    return call_build(callback, callback.refs[0], 1, "group build", outError);
}

ModResult tab_build(ModContext*, UiWindowHandle window, UiElementHandle left, UiElementHandle right,
    void* userData, ModError* outError) {
    auto& callback = *static_cast<Callback*>(userData);
    push_ui_handle(callback.vm->state, *callback.vm, window, HandleKind::UiWindow);
    push_ui_handle(callback.vm->state, *callback.vm, left, HandleKind::UiElement);
    push_ui_handle(callback.vm->state, *callback.vm, right, HandleKind::UiElement);
    return call_build(callback, callback.refs[0], 3, "window tab build", outError);
}

void window_closed(ModContext*, UiWindowHandle window, void* userData) {
    auto& callback = *static_cast<Callback*>(userData);
    push_ui_handle(callback.vm->state, *callback.vm, window, HandleKind::UiWindow);
    call_callback(callback, callback.refs[0], 1, 0, "window on_closed");
}

void dialog_action(ModContext*, UiDialogHandle dialog, void* userData) {
    auto& callback = *static_cast<Callback*>(userData);
    push_ui_handle(callback.vm->state, *callback.vm, dialog, HandleKind::UiDialog);
    call_callback(callback, callback.refs[0], 1, 0, "dialog action");
}

bool dialog_action_disabled(ModContext*, void* userData) {
    auto& callback = *static_cast<Callback*>(userData);
    return call_predicate(callback, callback.refs[1], 0, "dialog action is_disabled");
}

void dialog_dismissed(ModContext*, UiDialogHandle dialog, void* userData) {
    auto& callback = *static_cast<Callback*>(userData);
    push_ui_handle(callback.vm->state, *callback.vm, dialog, HandleKind::UiDialog);
    call_callback(callback, callback.refs[0], 1, 0, "dialog on_dismiss");
}

ModResult dialog_build(ModContext*, UiElementHandle pane, void* userData, ModError* outError) {
    auto& callback = *static_cast<Callback*>(userData);
    push_ui_handle(callback.vm->state, *callback.vm, pane, HandleKind::UiElement);
    return call_build(callback, callback.refs[1], 1, "dialog build", outError);
}

void menu_selected(ModContext*, void* userData) {
    auto& callback = *static_cast<Callback*>(userData);
    call_callback(callback, callback.refs[0], 0, 0, "menu tab on_selected");
}

void list_pressed(ModContext*, UiListHandle list, uint64_t key, void* userData) {
    auto& callback = *static_cast<Callback*>(userData);
    push_ui_handle(callback.vm->state, *callback.vm, list, HandleKind::UiList);
    lua_pushinteger64(callback.vm->state, static_cast<int64_t>(key));
    call_callback(callback, callback.refs[0], 2, 0, "list on_pressed");
}

bool list_selected(ModContext*, UiListHandle list, uint64_t key, void* userData) {
    auto& callback = *static_cast<Callback*>(userData);
    push_ui_handle(callback.vm->state, *callback.vm, list, HandleKind::UiList);
    lua_pushinteger64(callback.vm->state, static_cast<int64_t>(key));
    return call_predicate(callback, callback.refs[1], 2, "list is_selected");
}

bool list_disabled(ModContext*, UiListHandle list, uint64_t key, void* userData) {
    auto& callback = *static_cast<Callback*>(userData);
    push_ui_handle(callback.vm->state, *callback.vm, list, HandleKind::UiList);
    lua_pushinteger64(callback.vm->state, static_cast<int64_t>(key));
    return call_predicate(callback, callback.refs[2], 2, "list is_disabled");
}

std::vector<std::string> string_array(lua_State* state, int table, const char* field) {
    std::vector<std::string> result;
    lua_getfield(state, table, field);
    if (lua_isnil(state, -1)) {
        lua_pop(state, 1);
        return result;
    }
    luaL_checktype(state, -1, LUA_TTABLE);
    const int count = lua_objlen(state, -1);
    result.reserve(count);
    for (int i = 1; i <= count; ++i) {
        lua_rawgeti(state, -1, i);
        result.emplace_back(luaL_checkstring(state, -1));
        lua_pop(state, 1);
    }
    lua_pop(state, 1);
    return result;
}

std::vector<UiListItem> list_items(lua_State* state, int table, std::vector<std::string>& labels) {
    luaL_checktype(state, table, LUA_TTABLE);
    const int count = lua_objlen(state, table);
    labels.reserve(count);
    std::vector<UiListItem> items;
    items.reserve(count);
    for (int i = 1; i <= count; ++i) {
        lua_rawgeti(state, table, i);
        luaL_checktype(state, -1, LUA_TTABLE);
        labels.push_back(get_optional_string(state, -1, "label"));
        UiListItem item = UI_LIST_ITEM_INIT;
        item.key = static_cast<uint64_t>(get_optional_int(state, -1, "key", 0));
        items.push_back(item);
        lua_pop(state, 1);
    }
    for (size_t i = 0; i < items.size(); ++i) {
        items[i].label = labels[i].c_str();
    }
    return items;
}

UiControlKind control_kind(lua_State* state, const std::string& kind) {
    if (kind == "button")
        return UI_CONTROL_BUTTON;
    if (kind == "toggle")
        return UI_CONTROL_TOGGLE;
    if (kind == "number")
        return UI_CONTROL_NUMBER;
    if (kind == "string")
        return UI_CONTROL_STRING;
    if (kind == "select")
        return UI_CONTROL_SELECT;
    if (kind == "color")
        return UI_CONTROL_COLOR;
    if (kind == "group")
        return UI_CONTROL_GROUP;
    if (kind == "file_picker")
        return UI_CONTROL_FILE_PICKER;
    luaL_error(state, "unknown UI control kind '%s'", kind.c_str());
}

UiStyleScope style_scope(lua_State* state, const std::string& scope) {
    if (scope == "prelaunch")
        return UI_SCOPE_PRELAUNCH;
    if (scope == "window")
        return UI_SCOPE_WINDOW;
    if (scope == "menu_bar")
        return UI_SCOPE_MENU_BAR;
    if (scope == "overlay")
        return UI_SCOPE_OVERLAY;
    if (scope == "touch_controls")
        return UI_SCOPE_TOUCH_CONTROLS;
    if (scope == "graphics_tuner")
        return UI_SCOPE_GRAPHICS_TUNER;
    luaL_error(state, "unknown UI style scope '%s'", scope.c_str());
}

int pane_add_section(lua_State* state) {
    auto& pane = check_ui_handle(state, 1, HandleKind::UiElement);
    check_result(state,
        svc_ui->pane_add_section(pane.vm->subject, pane.value, luaL_checkstring(state, 2)),
        "ui pane_add_section");
    return 0;
}

int pane_add_text(lua_State* state) {
    auto& pane = check_ui_handle(state, 1, HandleKind::UiElement);
    UiElementHandle element = 0;
    check_result(state,
        svc_ui->pane_add_text(pane.vm->subject, pane.value, luaL_checkstring(state, 2), &element),
        "ui pane_add_text");
    push_ui_handle(state, *pane.vm, element, HandleKind::UiElement);
    return 1;
}

int pane_add_rml(lua_State* state) {
    auto& pane = check_ui_handle(state, 1, HandleKind::UiElement);
    UiElementHandle element = 0;
    check_result(state,
        svc_ui->pane_add_rml(pane.vm->subject, pane.value, luaL_checkstring(state, 2), &element),
        "ui pane_add_rml");
    push_ui_handle(state, *pane.vm, element, HandleKind::UiElement);
    return 1;
}

int pane_add_progress(lua_State* state) {
    auto& pane = check_ui_handle(state, 1, HandleKind::UiElement);
    UiElementHandle element = 0;
    check_result(state,
        svc_ui->pane_add_progress(
            pane.vm->subject, pane.value, static_cast<float>(luaL_checknumber(state, 2)), &element),
        "ui pane_add_progress");
    push_ui_handle(state, *pane.vm, element, HandleKind::UiElement);
    return 1;
}

int pane_add_control(lua_State* state) {
    auto& pane = check_ui_handle(state, 1, HandleKind::UiElement);
    luaL_checktype(state, 2, LUA_TTABLE);

    UiControlDesc desc = UI_CONTROL_DESC_INIT;
    const std::string kind = get_optional_string(state, 2, "kind");
    const std::string label = get_optional_string(state, 2, "label");
    const std::string help = get_optional_string(state, 2, "help_rml");
    const std::string prefix = get_optional_string(state, 2, "prefix");
    const std::string suffix = get_optional_string(state, 2, "suffix");
    desc.kind = control_kind(state, kind);
    desc.label = label.c_str();
    desc.help_rml = help.empty() ? nullptr : help.c_str();
    desc.min = get_optional_int(state, 2, "min", 0);
    desc.max = get_optional_int(state, 2, "max", 0);
    desc.step = get_optional_int(state, 2, "step", 1);
    desc.prefix = prefix.empty() ? nullptr : prefix.c_str();
    desc.suffix = suffix.empty() ? nullptr : suffix.c_str();
    desc.max_length = static_cast<int32_t>(get_optional_int(state, 2, "max_length", 0));
    desc.color_alpha = get_optional_bool(state, 2, "color_alpha", false);
    desc.directory_mode = get_optional_bool(state, 2, "directory_mode", false);
    desc.string_set_mode = get_optional_string(state, 2, "string_set_mode") == "change" ?
                               UI_STRING_SET_ON_CHANGE :
                               UI_STRING_SET_ON_COMMIT;

    std::vector<std::string> options = string_array(state, 2, "options");
    std::vector<const char*> optionPointers;
    optionPointers.reserve(options.size());
    for (const auto& option : options)
        optionPointers.push_back(option.c_str());
    desc.options = optionPointers.data();
    desc.option_count = optionPointers.size();

    std::vector<std::string> presets = string_array(state, 2, "color_presets");
    std::vector<const char*> presetPointers;
    presetPointers.reserve(presets.size());
    for (const auto& preset : presets)
        presetPointers.push_back(preset.c_str());
    desc.color_presets = presetPointers.data();
    desc.color_preset_count = presetPointers.size();

    std::vector<std::string> filterNames;
    std::vector<std::string> filterPatterns;
    std::vector<FileFilter> filters;
    lua_getfield(state, 2, "file_filters");
    if (!lua_isnil(state, -1)) {
        luaL_checktype(state, -1, LUA_TTABLE);
        const int count = lua_objlen(state, -1);
        filterNames.reserve(count);
        filterPatterns.reserve(count);
        filters.resize(count);
        for (int i = 1; i <= count; ++i) {
            lua_rawgeti(state, -1, i);
            filterNames.push_back(get_optional_string(state, -1, "name"));
            filterPatterns.push_back(get_optional_string(state, -1, "pattern"));
            lua_pop(state, 1);
        }
        for (int i = 0; i < count; ++i) {
            filters[i] = {filterNames[i].c_str(), filterPatterns[i].c_str()};
        }
    }
    lua_pop(state, 1);
    desc.file_filters = filters.data();
    desc.file_filter_count = filters.size();

    Callback& callback = retain_callback(*pane.vm);
    callback.tag = desc.kind;
    callback.refs[2] = ref_optional_function(state, 2, "on_pressed");
    callback.refs[3] = ref_optional_function(state, 2, "is_disabled");
    callback.refs[4] = ref_optional_function(state, 2, "is_modified");
    callback.refs[5] = ref_optional_function(state, 2, "is_selected");
    desc.user_data = &callback;
    desc.on_pressed = callback.refs[2] != LUA_NOREF ? control_pressed : nullptr;
    desc.is_disabled = callback.refs[3] != LUA_NOREF ? control_disabled : nullptr;
    desc.is_modified = callback.refs[4] != LUA_NOREF ? control_modified : nullptr;
    desc.is_selected = callback.refs[5] != LUA_NOREF ? control_selected : nullptr;

    lua_getfield(state, 2, "config_var");
    if (!lua_isnil(state, -1)) {
        auto& variable = check_config_var(state, -1);
        desc.binding = UI_BINDING_CONFIG_VAR;
        desc.config_var = variable.value;
    } else if (desc.kind != UI_CONTROL_BUTTON && desc.kind != UI_CONTROL_GROUP) {
        desc.binding = UI_BINDING_CALLBACKS;
        callback.refs[0] = ref_required_function(state, 2, "get");
        callback.refs[1] = ref_required_function(state, 2, "set");
        desc.get = control_get;
        desc.set = control_set;
    }
    lua_pop(state, 1);

    UiElementHandle element = 0;
    check_result(state, svc_ui->pane_add_control(pane.vm->subject, pane.value, &desc, &element),
        "ui pane_add_control");
    push_ui_handle(state, *pane.vm, element, HandleKind::UiElement);
    return 1;
}

int pane_add_group(lua_State* state) {
    auto& pane = check_ui_handle(state, 1, HandleKind::UiElement);
    auto& target = check_ui_handle(state, 2, HandleKind::UiElement);
    luaL_checktype(state, 3, LUA_TTABLE);
    const std::string label = get_optional_string(state, 3, "label");
    Callback& callback = retain_callback(*pane.vm);
    callback.refs[0] = ref_required_function(state, 3, "build");
    UiGroupDesc desc = UI_GROUP_DESC_INIT;
    desc.label = label.c_str();
    desc.build = group_build;
    desc.user_data = &callback;
    UiElementHandle element = 0;
    check_result(state,
        svc_ui->pane_add_group(pane.vm->subject, pane.value, target.value, &desc, &element),
        "ui pane_add_group");
    push_ui_handle(state, *pane.vm, element, HandleKind::UiElement);
    return 1;
}

int pane_add_list(lua_State* state) {
    auto& pane = check_ui_handle(state, 1, HandleKind::UiElement);
    luaL_checktype(state, 2, LUA_TTABLE);
    Callback& callback = retain_callback(*pane.vm);
    callback.refs[0] = ref_required_function(state, 2, "on_pressed");
    callback.refs[1] = ref_optional_function(state, 2, "is_selected");
    callback.refs[2] = ref_optional_function(state, 2, "is_disabled");

    std::vector<std::string> labels;
    std::vector<UiListItem> items;
    lua_getfield(state, 2, "items");
    if (!lua_isnil(state, -1))
        items = list_items(state, -1, labels);
    lua_pop(state, 1);

    UiListDesc desc = UI_LIST_DESC_INIT;
    desc.items = items.data();
    desc.item_count = items.size();
    desc.on_pressed = list_pressed;
    desc.is_selected = callback.refs[1] != LUA_NOREF ? list_selected : nullptr;
    desc.is_disabled = callback.refs[2] != LUA_NOREF ? list_disabled : nullptr;
    desc.user_data = &callback;
    UiListHandle list = 0;
    check_result(state, svc_ui->pane_add_list(pane.vm->subject, pane.value, &desc, &list),
        "ui pane_add_list");
    push_ui_handle(state, *pane.vm, list, HandleKind::UiList);
    return 1;
}

int element_set_text(lua_State* state) {
    auto& element = check_ui_handle(state, 1, HandleKind::UiElement);
    check_result(state,
        svc_ui->elem_set_text(element.vm->subject, element.value, luaL_checkstring(state, 2)),
        "ui elem_set_text");
    return 0;
}

int element_set_rml(lua_State* state) {
    auto& element = check_ui_handle(state, 1, HandleKind::UiElement);
    check_result(state,
        svc_ui->elem_set_rml(element.vm->subject, element.value, luaL_checkstring(state, 2)),
        "ui elem_set_rml");
    return 0;
}

int element_set_progress(lua_State* state) {
    auto& element = check_ui_handle(state, 1, HandleKind::UiElement);
    check_result(state,
        svc_ui->elem_set_progress(
            element.vm->subject, element.value, static_cast<float>(luaL_checknumber(state, 2))),
        "ui elem_set_progress");
    return 0;
}

int element_set_class(lua_State* state) {
    auto& element = check_ui_handle(state, 1, HandleKind::UiElement);
    check_result(state,
        svc_ui->elem_set_class(element.vm->subject, element.value, luaL_checkstring(state, 2),
            luaL_checkboolean(state, 3) != 0),
        "ui elem_set_class");
    return 0;
}

int list_set_items(lua_State* state) {
    auto& list = check_ui_handle(state, 1, HandleKind::UiList);
    std::vector<std::string> labels;
    auto items = list_items(state, 2, labels);
    check_result(state,
        svc_ui->list_set_items(list.vm->subject, list.value, items.data(), items.size()),
        "ui list_set_items");
    return 0;
}

int window_close(lua_State* state) {
    auto& window = check_ui_handle(state, 1, HandleKind::UiWindow);
    check_result(state, svc_ui->window_close(window.vm->subject, window.value), "ui window_close");
    window.value = 0;
    return 0;
}

int dialog_close(lua_State* state) {
    auto& dialog = check_ui_handle(state, 1, HandleKind::UiDialog);
    check_result(state, svc_ui->dialog_close(dialog.vm->subject, dialog.value), "ui dialog_close");
    dialog.value = 0;
    return 0;
}

int dialog_set_body(lua_State* state) {
    auto& dialog = check_ui_handle(state, 1, HandleKind::UiDialog);
    check_result(state,
        svc_ui->dialog_set_body(dialog.vm->subject, dialog.value, luaL_checkstring(state, 2)),
        "ui dialog_set_body");
    return 0;
}

int dialog_set_icon(lua_State* state) {
    auto& dialog = check_ui_handle(state, 1, HandleKind::UiDialog);
    check_result(state,
        svc_ui->dialog_set_icon(dialog.vm->subject, dialog.value, luaL_checkstring(state, 2)),
        "ui dialog_set_icon");
    return 0;
}

int style_unregister(lua_State* state) {
    auto& style = check_ui_handle(state, 1, HandleKind::UiStyle);
    check_result(
        state, svc_ui->unregister_styles(style.vm->subject, style.value), "ui unregister_styles");
    style.value = 0;
    return 0;
}

int menu_tab_unregister(lua_State* state) {
    auto& tab = check_ui_handle(state, 1, HandleKind::UiMenuTab);
    check_result(
        state, svc_ui->unregister_menu_tab(tab.vm->subject, tab.value), "ui unregister_menu_tab");
    tab.value = 0;
    return 0;
}

int register_mods_panel(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    luaL_checktype(state, 1, LUA_TTABLE);
    Callback& callback = retain_callback(vm);
    callback.refs[0] = ref_required_function(state, 1, "build");
    callback.refs[1] = ref_optional_function(state, 1, "update");
    UiModsPanelDesc desc = UI_MODS_PANEL_DESC_INIT;
    desc.build = panel_build;
    desc.update = callback.refs[1] != LUA_NOREF ? panel_update : nullptr;
    desc.user_data = &callback;
    check_result(state, svc_ui->register_mods_panel(vm.subject, &desc), "ui register_mods_panel");
    return 0;
}

int window_push(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    luaL_checktype(state, 1, LUA_TTABLE);
    std::vector<UiTabDesc> tabs;
    std::vector<std::string> titles;
    lua_getfield(state, 1, "tabs");
    luaL_checktype(state, -1, LUA_TTABLE);
    const int tabCount = lua_objlen(state, -1);
    tabs.reserve(tabCount);
    titles.reserve(tabCount);
    for (int i = 1; i <= tabCount; ++i) {
        lua_rawgeti(state, -1, i);
        titles.push_back(get_optional_string(state, -1, "title"));
        Callback& callback = retain_callback(vm);
        callback.refs[0] = ref_required_function(state, -1, "build");
        callback.refs[1] = ref_optional_function(state, -1, "update");
        UiTabDesc tab = UI_TAB_DESC_INIT;
        tab.build = tab_build;
        tab.update = callback.refs[1] != LUA_NOREF ? panel_update : nullptr;
        tab.user_data = &callback;
        tabs.push_back(tab);
        lua_pop(state, 1);
    }
    lua_pop(state, 1);
    for (size_t i = 0; i < tabs.size(); ++i)
        tabs[i].title = titles[i].c_str();

    const std::string rcss = get_optional_string(state, 1, "rcss");
    Callback& closed = retain_callback(vm);
    closed.refs[0] = ref_optional_function(state, 1, "on_closed");
    UiWindowDesc desc = UI_WINDOW_DESC_INIT;
    desc.tabs = tabs.data();
    desc.tab_count = tabs.size();
    desc.rcss = rcss.empty() ? nullptr : rcss.c_str();
    desc.on_closed = closed.refs[0] != LUA_NOREF ? window_closed : nullptr;
    desc.user_data = &closed;
    UiWindowHandle window = 0;
    check_result(state, svc_ui->window_push(vm.subject, &desc, &window), "ui window_push");
    push_ui_handle(state, vm, window, HandleKind::UiWindow);
    return 1;
}

UiDialogVariant dialog_variant(lua_State* state, const std::string& variant) {
    if (variant.empty() || variant == "normal")
        return UI_DIALOG_NORMAL;
    if (variant == "warning")
        return UI_DIALOG_WARNING;
    if (variant == "danger")
        return UI_DIALOG_DANGER;
    luaL_error(state, "unknown dialog variant '%s'", variant.c_str());
}

int dialog_push(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    luaL_checktype(state, 1, LUA_TTABLE);
    const std::string title = get_optional_string(state, 1, "title");
    const std::string body = get_optional_string(state, 1, "body_rml");
    const std::string icon = get_optional_string(state, 1, "icon");
    const std::string variant = get_optional_string(state, 1, "variant");

    std::vector<UiDialogAction> actions;
    std::vector<std::string> labels;
    lua_getfield(state, 1, "actions");
    luaL_checktype(state, -1, LUA_TTABLE);
    const int actionCount = lua_objlen(state, -1);
    actions.reserve(actionCount);
    labels.reserve(actionCount);
    for (int i = 1; i <= actionCount; ++i) {
        lua_rawgeti(state, -1, i);
        labels.push_back(get_optional_string(state, -1, "label"));
        Callback& callback = retain_callback(vm);
        callback.refs[0] = ref_optional_function(state, -1, "on_pressed");
        callback.refs[1] = ref_optional_function(state, -1, "is_disabled");
        UiDialogAction action = UI_DIALOG_ACTION_INIT;
        action.on_pressed = callback.refs[0] != LUA_NOREF ? dialog_action : nullptr;
        action.is_disabled = callback.refs[1] != LUA_NOREF ? dialog_action_disabled : nullptr;
        action.user_data = &callback;
        action.keep_open = get_optional_bool(state, -1, "keep_open", false);
        actions.push_back(action);
        lua_pop(state, 1);
    }
    lua_pop(state, 1);
    for (size_t i = 0; i < actions.size(); ++i)
        actions[i].label = labels[i].c_str();

    Callback& callback = retain_callback(vm);
    callback.refs[0] = ref_optional_function(state, 1, "on_dismiss");
    callback.refs[1] = ref_optional_function(state, 1, "build");
    UiDialogDesc desc = UI_DIALOG_DESC_INIT;
    desc.title = title.c_str();
    desc.body_rml = body.c_str();
    desc.icon = icon.empty() ? nullptr : icon.c_str();
    desc.variant = dialog_variant(state, variant);
    desc.actions = actions.data();
    desc.action_count = actions.size();
    desc.on_dismiss = callback.refs[0] != LUA_NOREF ? dialog_dismissed : nullptr;
    desc.build = callback.refs[1] != LUA_NOREF ? dialog_build : nullptr;
    desc.user_data = &callback;
    UiDialogHandle dialog = 0;
    check_result(state, svc_ui->dialog_push(vm.subject, &desc, &dialog), "ui dialog_push");
    push_ui_handle(state, vm, dialog, HandleKind::UiDialog);
    return 1;
}

int is_any_document_visible(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    bool visible = false;
    check_result(
        state, svc_ui->is_any_document_visible(vm.subject, &visible), "ui is_any_document_visible");
    lua_pushboolean(state, visible);
    return 1;
}

int register_styles(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    const auto scope = style_scope(state, luaL_checkstring(state, 1));
    UiStyleHandle style = 0;
    check_result(state,
        svc_ui->register_styles(vm.subject, scope, luaL_checkstring(state, 2), &style),
        "ui register_styles");
    push_ui_handle(state, vm, style, HandleKind::UiStyle);
    return 1;
}

int register_styles_file(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    const auto scope = style_scope(state, luaL_checkstring(state, 1));
    UiStyleHandle style = 0;
    check_result(state,
        svc_ui->register_styles_file(vm.subject, scope, luaL_checkstring(state, 2), &style),
        "ui register_styles_file");
    push_ui_handle(state, vm, style, HandleKind::UiStyle);
    return 1;
}

int register_menu_tab(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    luaL_checktype(state, 1, LUA_TTABLE);
    const std::string label = get_optional_string(state, 1, "label");
    Callback& callback = retain_callback(vm);
    callback.refs[0] = ref_required_function(state, 1, "on_selected");
    UiMenuTabDesc desc = UI_MENU_TAB_DESC_INIT;
    desc.label = label.c_str();
    desc.on_selected = menu_selected;
    desc.user_data = &callback;
    UiMenuTabHandle tab = 0;
    check_result(state, svc_ui->register_menu_tab(vm.subject, &desc, &tab), "ui register_menu_tab");
    push_ui_handle(state, vm, tab, HandleKind::UiMenuTab);
    return 1;
}

int push_toast(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    luaL_checktype(state, 1, LUA_TTABLE);
    const std::string type = get_optional_string(state, 1, "type");
    const std::string title = get_optional_string(state, 1, "title_rml");
    const std::string body = get_optional_string(state, 1, "body_rml");
    UiToastDesc desc = UI_TOAST_DESC_INIT;
    desc.type = type.empty() ? nullptr : type.c_str();
    desc.title_rml = title.empty() ? nullptr : title.c_str();
    desc.body_rml = body.empty() ? nullptr : body.c_str();
    desc.duration_ms = static_cast<uint32_t>(get_optional_int(state, 1, "duration_ms", 0));
    check_result(state, svc_ui->push_toast(vm.subject, &desc), "ui push_toast");
    return 0;
}

int get_clipboard_text(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    size_t size = 0;
    check_result(
        state, svc_ui->get_clipboard_text(vm.subject, nullptr, 0, &size), "ui get_clipboard_text");
    std::vector<char> text(size + 1);
    check_result(state, svc_ui->get_clipboard_text(vm.subject, text.data(), text.size(), nullptr),
        "ui get_clipboard_text");
    lua_pushlstring(state, text.data(), size);
    return 1;
}

int set_clipboard_text(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    check_result(state, svc_ui->set_clipboard_text(vm.subject, luaL_checkstring(state, 1)),
        "ui set_clipboard_text");
    return 0;
}

}  // namespace

void push_ui_handle(lua_State* state, Vm& vm, uint64_t value, HandleKind kind) {
    push_handle(state, vm, value, kind, ui_metatable(kind));
}

int open_ui(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_ui == nullptr) {
        service_unavailable(state, "UiService");
    }
    static const luaL_Reg kElementMethods[] = {
        {"add_section", pane_add_section},
        {"add_text", pane_add_text},
        {"add_rml", pane_add_rml},
        {"add_progress", pane_add_progress},
        {"add_control", pane_add_control},
        {"add_group", pane_add_group},
        {"add_list", pane_add_list},
        {"set_text", element_set_text},
        {"set_rml", element_set_rml},
        {"set_progress", element_set_progress},
        {"set_class", element_set_class},
        {nullptr, nullptr},
    };
    static const luaL_Reg kWindowMethods[] = {{"close", window_close}, {nullptr, nullptr}};
    static const luaL_Reg kDialogMethods[] = {
        {"close", dialog_close},
        {"set_body", dialog_set_body},
        {"set_icon", dialog_set_icon},
        {nullptr, nullptr},
    };
    static const luaL_Reg kStyleMethods[] = {{"unregister", style_unregister}, {nullptr, nullptr}};
    static const luaL_Reg kMenuMethods[] = {
        {"unregister", menu_tab_unregister}, {nullptr, nullptr}};
    static const luaL_Reg kListMethods[] = {{"set_items", list_set_items}, {nullptr, nullptr}};
    create_handle_metatable(state, kUiElementMetatable, kElementMethods, "UiElement");
    create_handle_metatable(state, kUiWindowMetatable, kWindowMethods, "UiWindow");
    create_handle_metatable(state, kUiDialogMetatable, kDialogMethods, "UiDialog");
    create_handle_metatable(state, kUiStyleMetatable, kStyleMethods, "UiStyle");
    create_handle_metatable(state, kUiMenuTabMetatable, kMenuMethods, "UiMenuTab");
    create_handle_metatable(state, kUiListMetatable, kListMethods, "UiList");

    lua_newtable(state);
    set_function(state, vm, "register_mods_panel", register_mods_panel);
    set_function(state, vm, "window_push", window_push);
    set_function(state, vm, "dialog_push", dialog_push);
    set_function(state, vm, "is_any_document_visible", is_any_document_visible);
    set_function(state, vm, "register_styles", register_styles);
    set_function(state, vm, "register_styles_file", register_styles_file);
    set_function(state, vm, "register_menu_tab", register_menu_tab);
    set_function(state, vm, "push_toast", push_toast);
    set_function(state, vm, "get_clipboard_text", get_clipboard_text);
    set_function(state, vm, "set_clipboard_text", set_clipboard_text);
    lua_setreadonly(state, -1, true);
    return 1;
}

}  // namespace luau_runtime
