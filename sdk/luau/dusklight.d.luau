--!strict

export type IntegerInput = integer | number
export type ConfigValue = boolean | integer | number | string
export type ConfigVar<T> = {
    get: (self: ConfigVar<T>) -> T,
    set: (self: ConfigVar<T>, value: T) -> (),
    subscribe: (self: ConfigVar<T>, callback: (value: T, previous: T) -> ()) -> ConfigSubscription,
    unregister: (self: ConfigVar<T>) -> (),
}
export type ConfigSubscription = {
    unsubscribe: (self: ConfigSubscription) -> (),
}
export type ConfigDesc = {
    name: string,
    type: "bool" | "int" | "float" | "string",
    default: ConfigValue?,
}
export type ConfigModule = {
    register: (desc: ConfigDesc) -> ConfigVar<any>,
    subscribe: <T>(variable: ConfigVar<T>, callback: (value: T, previous: T) -> ()) -> ConfigSubscription,
}

export type OverlayHandle = { remove: (self: OverlayHandle) -> () }
export type OverlayModule = {
    add_file: (discPath: string, bundlePath: string) -> OverlayHandle,
    add_buffer: (discPath: string, data: string) -> OverlayHandle,
}
export type TextureHandle = { unregister: (self: TextureHandle) -> () }
export type TextureKey = {
    texture_hash: integer,
    tlut_hash: integer?,
    width: IntegerInput,
    height: IntegerInput,
    gx_format: IntegerInput,
    has_tlut: boolean?,
}
export type TextureData = {
    data: string,
    width: IntegerInput?,
    height: IntegerInput?,
    mip_count: IntegerInput?,
    gx_format: IntegerInput?,
}
export type TextureModule = {
    hash_wildcard: integer,
    tlut_wildcard: integer,
    register_file: (bundlePath: string) -> TextureHandle,
    register_data: (key: TextureKey, data: TextureData) -> TextureHandle,
}

export type UiElement = {
    add_section: (self: UiElement, title: string) -> (),
    add_text: (self: UiElement, text: string) -> UiElement,
    add_rml: (self: UiElement, rml: string) -> UiElement,
    add_progress: (self: UiElement, value: number) -> UiElement,
    add_control: (self: UiElement, desc: UiControlDesc) -> UiElement,
    add_group: (self: UiElement, target: UiElement, desc: UiGroupDesc) -> UiElement,
    add_list: (self: UiElement, desc: UiListDesc) -> UiList,
    set_text: (self: UiElement, text: string) -> (),
    set_rml: (self: UiElement, rml: string) -> (),
    set_progress: (self: UiElement, value: number) -> (),
    set_class: (self: UiElement, name: string, active: boolean) -> (),
}
export type UiControlDesc = {
    kind: "button" | "toggle" | "number" | "string" | "select" | "color" | "group" | "file_picker",
    label: string,
    help_rml: string?,
    config_var: ConfigVar<any>?,
    get: (() -> ConfigValue)?,
    set: ((value: ConfigValue) -> ())?,
    on_pressed: (() -> ())?,
    is_disabled: (() -> boolean)?,
    is_modified: (() -> boolean)?,
    is_selected: (() -> boolean)?,
    min: IntegerInput?,
    max: IntegerInput?,
    step: IntegerInput?,
    prefix: string?,
    suffix: string?,
    options: {string}?,
    max_length: IntegerInput?,
    color_presets: {string}?,
    color_alpha: boolean?,
    string_set_mode: ("commit" | "change")?,
    file_filters: {{name: string, pattern: string}}?,
    directory_mode: boolean?,
}
export type UiGroupDesc = { label: string, build: (pane: UiElement) -> () }
export type UiListItem = { key: IntegerInput, label: string }
export type UiListDesc = {
    items: {UiListItem}?,
    on_pressed: (list: UiList, key: integer) -> (),
    is_selected: ((list: UiList, key: integer) -> boolean)?,
    is_disabled: ((list: UiList, key: integer) -> boolean)?,
}
export type UiList = { set_items: (self: UiList, items: {UiListItem}) -> () }
export type UiWindow = { close: (self: UiWindow) -> () }
export type UiDialog = {
    close: (self: UiDialog) -> (),
    set_body: (self: UiDialog, rml: string) -> (),
    set_icon: (self: UiDialog, icon: string) -> (),
}
export type UiStyle = { unregister: (self: UiStyle) -> () }
export type UiMenuTab = { unregister: (self: UiMenuTab) -> () }
export type UiTabDesc = {
    title: string,
    build: (window: UiWindow, left: UiElement, right: UiElement) -> (),
    update: (() -> ())?,
}
export type UiModule = {
    register_mods_panel: (desc: {build: (panel: UiElement) -> (), update: (() -> ())?}) -> (),
    window_push: (desc: {tabs: {UiTabDesc}, rcss: string?, on_closed: ((window: UiWindow) -> ())?}) -> UiWindow,
    dialog_push: (desc: {
        title: string,
        body_rml: string,
        variant: ("normal" | "warning" | "danger")?,
        icon: string?,
        actions: {{label: string, on_pressed: ((dialog: UiDialog) -> ())?, keep_open: boolean?, is_disabled: (() -> boolean)?}},
        on_dismiss: ((dialog: UiDialog) -> ())?,
        build: ((pane: UiElement) -> ())?,
    }) -> UiDialog,
    is_any_document_visible: () -> boolean,
    register_styles: (scope: string, rcss: string) -> UiStyle,
    register_styles_file: (scope: string, path: string) -> UiStyle,
    register_menu_tab: (desc: {label: string, on_selected: () -> ()}) -> UiMenuTab,
    push_toast: (desc: {type: string?, title_rml: string?, body_rml: string?, duration_ms: IntegerInput?}) -> (),
    get_clipboard_text: () -> string,
    set_clipboard_text: (text: string) -> (),
}

export type LogModule = {
    write: (level: "trace" | "debug" | "info" | "warn" | "error", message: string) -> (),
    trace: (message: string) -> (),
    debug: (message: string) -> (),
    info: (message: string) -> (),
    warn: (message: string) -> (),
    error: (message: string) -> (),
}
export type HostModule = {
    version: string,
    mod_id: () -> string,
    mod_name: () -> string,
    mod_version: () -> string,
    mod_dir: () -> string,
    data_dir: () -> string,
    on_update: (callback: () -> ()) -> (),
    on_shutdown: (callback: () -> ()) -> (),
    fail: (message: string) -> never,
}
export type ResourceModule = { load: (relativePath: string) -> string }

declare function require(path: "dusklight.log"): LogModule
declare function require(path: "dusklight.host"): HostModule
declare function require(path: "dusklight.config"): ConfigModule
declare function require(path: "dusklight.resource"): ResourceModule
declare function require(path: "dusklight.overlay"): OverlayModule
declare function require(path: "dusklight.texture"): TextureModule
declare function require(path: "dusklight.ui"): UiModule
declare function require(path: string): any
