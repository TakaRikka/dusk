#pragma once

#include "mods/svc/ui.h"

namespace dusk::mods::svc::ui_v1 {

constexpr uint16_t kMajorVersion = 1;
constexpr uint16_t kMinorVersion = 5;

struct UiDialogAction {
    const char* label;
    UiDialogActionFn on_pressed;
    void* user_data;
    bool keep_open;
};

struct UiDialogDesc {
    uint32_t struct_size;
    const char* title;
    const char* body_rml;
    UiDialogVariant variant;
    const char* icon;
    const UiDialogAction* actions;
    size_t action_count;
    UiDialogActionFn on_dismiss;
    void* user_data;
    UiPaneBuildFn build;
};

struct UiService {
    ServiceHeader header;

    ModResult (*register_mods_panel)(ModContext* ctx, const UiModsPanelDesc* desc);
    ModResult (*pane_add_section)(ModContext* ctx, UiElementHandle pane, const char* title);
    ModResult (*pane_add_text)(
        ModContext* ctx, UiElementHandle pane, const char* text, UiElementHandle* out_elem);
    ModResult (*pane_add_rml)(
        ModContext* ctx, UiElementHandle pane, const char* rml, UiElementHandle* out_elem);
    ModResult (*pane_add_progress)(
        ModContext* ctx, UiElementHandle pane, float value, UiElementHandle* out_elem);
    ModResult (*pane_add_control)(ModContext* ctx, UiElementHandle pane, const UiControlDesc* desc,
        UiElementHandle* out_elem);

    ModResult (*elem_set_text)(ModContext* ctx, UiElementHandle elem, const char* text);
    ModResult (*elem_set_rml)(ModContext* ctx, UiElementHandle elem, const char* rml);
    ModResult (*elem_set_progress)(ModContext* ctx, UiElementHandle elem, float value);
    ModResult (*elem_set_class)(
        ModContext* ctx, UiElementHandle elem, const char* name, bool active);

    ModResult (*window_push)(ModContext* ctx, const UiWindowDesc* desc, UiWindowHandle* out_window);
    ModResult (*window_close)(ModContext* ctx, UiWindowHandle window);

    ModResult (*dialog_push)(ModContext* ctx, const UiDialogDesc* desc, UiDialogHandle* out_dialog);
    ModResult (*dialog_close)(ModContext* ctx, UiDialogHandle dialog);
    ModResult (*dialog_set_body)(ModContext* ctx, UiDialogHandle dialog, const char* body_rml);
    ModResult (*dialog_set_icon)(ModContext* ctx, UiDialogHandle dialog, const char* icon);
    ModResult (*dialog_add_action)(
        ModContext* ctx, UiDialogHandle dialog, const UiDialogAction* action);

    ModResult (*is_any_document_visible)(ModContext* ctx, bool* out_visible);

    ModResult (*register_styles)(
        ModContext* ctx, UiStyleScope scope, const char* rcss, UiStyleHandle* out_style);
    ModResult (*register_styles_file)(
        ModContext* ctx, UiStyleScope scope, const char* path, UiStyleHandle* out_style);
    ModResult (*unregister_styles)(ModContext* ctx, UiStyleHandle style);

    ModResult (*register_menu_tab)(
        ModContext* ctx, const UiMenuTabDesc* desc, UiMenuTabHandle* out_tab);
    ModResult (*unregister_menu_tab)(ModContext* ctx, UiMenuTabHandle tab);

    ModResult (*push_toast)(ModContext* ctx, const UiToastDesc* desc);

    ModResult (*get_clipboard_text)(
        ModContext* ctx, char* buffer, size_t bufferSize, size_t* outLength);
    ModResult (*set_clipboard_text)(ModContext* ctx, const char* text);

    ModResult (*pane_add_group)(ModContext* ctx, UiElementHandle group_pane,
        UiElementHandle target_pane, const UiGroupDesc* desc, UiElementHandle* out_elem);
};

}  // namespace dusk::mods::svc::ui_v1
