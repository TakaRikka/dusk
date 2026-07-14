# Dusklight Mod API

Mods are `.dusk` bundles: zip archives that can contain code (in the form of native libraries), resources, DVD overlay
files, and texture replacements. Mods may be enabled, disabled and reloaded at runtime.

When code mods are loaded, they get dynamically linked by the operating system to the running game process. The mod
exports lifecycle functions that Dusklight calls into (`mod_initialize`, `mod_update`, `mod_shutdown`), and the mod
communicates with the host via **services**: plain C APIs, individually versioned. Dusklight exports several built-in
services, and mods may export services of their own, permitting framework mods and cross-mod integration.

Beyond services, mods have full access to the original game's code: include game headers, call directly into any public
function, read and write data fields, and hook the vast majority of game functions.

## Table of Contents

1. [Getting Started](#getting-started)
2. [mod.json](#modjson)
3. [Anatomy of a Code Mod](#anatomy-of-a-code-mod)
4. [Services](#services)
5. [Built-in Services](#built-in-services)
6. [Hooking Game Functions](#hooking-game-functions)
7. [Asset Overlays](#asset-overlays)
8. [Runtime Lifecycle](#runtime-lifecycle)
9. [Error Handling](#error-handling)
10. [Advanced](#advanced)

---

## Getting Started

Fork the [mod template](../mods/template_mod/), a self-contained CMake project that uses the Dusklight mod SDK.

```
my_mod/
├── CMakeLists.txt
├── mod.json
├── src/mod.cpp
├── res/       (optional bundled resources)
├── overlay/   (optional game file overrides)
└── textures/  (optional texture replacements)
```

**CMakeLists.txt:**

```cmake
cmake_minimum_required(VERSION 3.25)
project(my_mod CXX)

set(DUSKLIGHT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dusklight" CACHE PATH "Path to dusklight source root")
add_subdirectory("${DUSKLIGHT_DIR}/sdk" dusklight-sdk EXCLUDE_FROM_ALL)

add_mod(my_mod
        FEATURES game          # optional
        SOURCES src/mod.cpp
        MOD_JSON mod.json
        RES_DIR res            # optional
        OVERLAY_DIR overlay    # optional
        TEXTURES_DIR textures  # optional
)
```

Available features:
- `game`: Allows calling into and hooking game code. Mods that **only** use services may omit it, providing a wider
  range of compatibility with Dusklight versions and a slightly faster build process.
- `webgpu`: Allows importing the WebGPU API (`webgpu/webgpu.h`). Must be enabled when using
  [GfxService](#gfxservice-modssvcgfxh).

Building produces `my_mod.dusk` in `build/<preset>/mods/` (configurable via the `DUSK_MODS_OUTPUT_DIR` cache variable).
Dusklight searches a `mods/` directory next to the app in addition to the user directory, so a dev build launched from
`build/<preset>/` picks these up automatically: rebuild, relaunch (or click Reload), done.

For a regular game install, copy the `.dusk` into the user mods folder:

- Windows: `%APPDATA%\TwilitRealm\Dusklight\mods`
- Linux: `~/.local/share/TwilitRealm/Dusklight/mods`
- macOS: `~/Library/Application Support/TwilitRealm/Dusklight/mods`

Passing `--mods <dir>` on the command line replaces the user directory with one of your choosing.

---

## mod.json

```json
{
  "id": "com.example.my_mod",
  "name": "My Mod",
  "version": "1.0.0",
  "author": "Your Name",
  "description": "A short description shown in the mod manager.",
  "icon": "res/my_icon.png",
  "banner": "res/my_banner.png"
}
```

`id` is required: a unique, stable identifier (reverse-DNS style; periods, underscores, and alphanumerics). Everything
else is optional but recommended.

`icon` and `banner` are bundle-relative paths to PNG images for the in-game mod manager: the square icon (e.g.
512x512), the banner (~3.5:1). If omitted, `res/icon.png` and `res/banner.png` are used automatically when present.

---

## Anatomy of a Code Mod

```cpp
#include "mods/service.hpp"
#include "mods/svc/log.h"

DEFINE_MOD();                          // once, in exactly one translation unit
IMPORT_SERVICE(LogService, svc_log);   // resolved by the loader before mod_initialize

extern "C" {

MOD_EXPORT ModResult mod_initialize(ModError* error) {
    svc_log->info(mod_ctx, "hello from my_mod");
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError* error) {   // called every frame
    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError* error) {
    return MOD_OK;
}
}
```

All three lifecycle exports are required. `mod_ctx` is your mod's identity token, set by the loader before
`mod_initialize` runs. Pass it as the first argument to every service call.

---

## Services

A service is a struct of C function pointers with a version header. You declare what you use at file scope, and the
loader resolves it before your mod initializes:

```cpp
IMPORT_SERVICE(LogService, svc_log);              // required, latest minor version
IMPORT_SERVICE_VERSION(LogService, svc_log, 0);   // required, minimum minor version 0 (for backwards compatibility)
IMPORT_OPTIONAL_SERVICE(SomeService, svc_maybe);  // may be null
```

Each service is individually versioned, and there may be multiple major versions of a service provided at once,
allowing backwards compatibility with older mods while still changing services fundamentally if necessary. A **major**
bump is a breaking change, treated as a different service entirely. For **additive** changes, a service appends new
functions to the end of the struct without breaking existing callers and simply bumps the minor version.

`IMPORT_SERVICE` and `IMPORT_OPTIONAL_SERVICE` require the latest minor version compiled against, making every field in
the service safe to call. A mod can use `IMPORT_SERVICE_VERSION` (or its optional counterpart) with an older minor
version to remain compatible with older Dusklight versions, then use `SERVICE_HAS` to check at runtime for fields added
after that explicitly requested version.

The contract (see `sdk/include/mods/api.h` for the full version):

- **A required import is guaranteed valid.** If the service is missing or too old, the mod fails to load with a clear
  error. No need to null check at call sites.
- **Anything at or below the minor version you imported can be called unconditionally.** The default macros import
  the service type's current minor version; the versioned macros explicitly override that minimum.
- Optional imports may be null; check once in `mod_initialize`.
- Fields newer than your imported minor version must be gated behind `SERVICE_HAS(service, ServiceType, field)` plus a
  null check.

---

## Built-in Services

### LogService (`mods/svc/log.h`)

```cpp
IMPORT_SERVICE(LogService, svc_log);

svc_log->info(mod_ctx, "spawned the thing");
svc_log->warn(mod_ctx, "that looks wrong");
svc_log->error(mod_ctx, "very bad");
svc_log->write(mod_ctx, LOG_LEVEL_DEBUG, "verbose details");
```

Messages appear in the console prefixed with your mod ID. Messages are plain UTF-8 strings and are copied before the
call returns; use `snprintf` or `fmt::format` for formatting.

### ResourceService (`mods/svc/resource.h`)

Loads files from the `res/` tree of your `.dusk` archive. Paths are relative to `res/` (pass `"config.txt"`, not
`"res/config.txt"`); absolute paths and `..` are rejected.

```cpp
IMPORT_SERVICE(ResourceService, svc_resource);

ResourceBuffer buf = RESOURCE_BUFFER_INIT;
if (svc_resource->load(mod_ctx, "config.txt", &buf) == MOD_OK) {
    // buf.data / buf.size
    svc_resource->free(mod_ctx, &buf);
}
```

Missing files return `MOD_UNAVAILABLE`. Always `free` what you `load`. Note that the bundle is read-only; for writable
storage, use the directory from `svc_host->mod_dir(mod_ctx)`.

### HostService (`mods/svc/host.h`)

Mod metadata and runtime interaction with the loader:

```cpp
IMPORT_SERVICE(HostService, svc_host);

const char* id  = svc_host->mod_id(mod_ctx);
const char* dir = svc_host->mod_dir(mod_ctx);  // writable per-mod directory
svc_host->fail(mod_ctx, MOD_ERROR, "something unrecoverable happened");  // disables the mod
```

`get_service`/`publish_service` provide dynamic service lookup; see [Exporting Services](#exporting-services).

**Lifecycle watches.** If your mod provides a service that hands out per-caller state (registrations, callbacks,
handles), watch other mods' lifecycle and drop what you hold for a mod when it detaches.

```cpp
IMPORT_SERVICE(HostService, svc_host);

void on_mod_lifecycle(ModContext* ctx, ModContext* subject, const char* subject_id,
    ModLifecycleEvent event, void* user_data) {
    if (event == MOD_LIFECYCLE_DETACHED) {
        drop_state_for(subject);  // same ModContext* the subject passed into your service
    }
}

uint64_t watch = 0;
svc_host->watch_mod_lifecycle(mod_ctx, on_mod_lifecycle, nullptr, &watch);
```

`MOD_LIFECYCLE_DETACHED` fires on the game thread at a lifecycle safe point, after the subject's `mod_shutdown` ran and
every service dropped its state. For your own mod's teardown, use `mod_shutdown` instead.

### HookService (`mods/svc/hook.h`)

Installs hooks on game functions and resolves symbols by name. You'll rarely call it directly; use the typed helpers in
`mods/hook.hpp` described in [Hooking Game Functions](#hooking-game-functions).

### OverlayService (`mods/svc/overlay.h`)

Registers DVD file overlays at runtime: the dynamic counterpart to the static `overlay/` directory (see
[Asset Overlays](#asset-overlays)). Overlay a disc path with a file from your bundle, or with a caller-owned buffer
(copied on registration):

```cpp
IMPORT_SERVICE(OverlayService, svc_overlay);

OverlayHandle handle = 0;
svc_overlay->add_file(mod_ctx, "/res/Msgus.arc", "res/replacement.arc", &handle);
svc_overlay->add_buffer(mod_ctx, "/generated.txt", data, size, nullptr);
svc_overlay->remove(mod_ctx, handle);
```

`disc_path` must be absolute (leading `/`) and is matched against the disc case-insensitively. Paths that don't exist
on the disc are added as new files. Changes are applied at the next frame boundary, and data the game already read
stays in memory until the file is re-read: sometimes a scene reload, and in the worst case, a full restart.

See [Asset Overlays](#asset-overlays) for priority and conflict handling.

### TextureService (`mods/svc/texture.h`)

Registers texture replacements at runtime: the dynamic counterpart to the static `textures/` directory (see
[Asset Overlays](#asset-overlays)). Two forms: raw texel data with an explicit key, or an encoded `.dds`/`.png` from
your bundle whose filename encodes the key:

```cpp
IMPORT_SERVICE(TextureService, svc_texture);

// Encoded file; filename follows the replacement naming convention.
TextureReplacementHandle handle = 0;
svc_texture->register_file(mod_ctx, "res/tex1_32x32_$_6.png", &handle);

// Raw data: match by texel-data pointer or by content hash (TEXTURE_KEY_SOURCE).
TextureKey key = TEXTURE_KEY_INIT;
key.kind = TEXTURE_KEY_POINTER;
key.pointer = someTexObj.data;
TextureData data = TEXTURE_DATA_INIT;
data.data = pixels; data.size = pixelsSize;
data.width = 32; data.height = 32; data.gx_format = GX_TF_RGBA8_PC;
svc_texture->register_data(mod_ctx, &key, &data, nullptr);

svc_texture->unregister(mod_ctx, handle);
```

Filenames use the same Dolphin-style convention as the user's `texture_replacements` directory:
`tex1_{w}x{h}_{texhash}[_{tluthash}]_{fmt}.dds|.png`, where hashes may be `$` (wildcard). `_mipN` sidecar files next to
a registered file are picked up automatically. Files are decoded lazily on first use by the renderer; raw data is copied
at registration. Registrations follow your mod's lifecycle.

See [Asset Overlays](#asset-overlays) for priority and conflict handling.

### ConfigService (`mods/svc/config.h`)

Persistent, mod-scoped configuration variables. Each var is stored in the user's `config.json` under
`mod.<escaped mod id>.<name>` (escaping: `.` → `_`, `_` → `__`, so `com.example.my_mod` becomes `com_example_my__mod`),
next to the host's own settings:

```cpp
IMPORT_SERVICE(ConfigService, svc_config);

ConfigVarDesc desc = CONFIG_VAR_DESC_INIT;
desc.name = "speedMultiplier";  // 1-64 chars from [A-Za-z0-9_-]; "enabled" is reserved
desc.type = CONFIG_VAR_FLOAT;
desc.default_float = 1.0;
ConfigVarHandle var = 0;
svc_config->register_var(mod_ctx, &desc, &var);

double speed = 1.0;
svc_config->get_float(mod_ctx, var, &speed);
svc_config->set_float(mod_ctx, var, 2.0);

// Optional: get notified when the value changes.
void on_speed_changed(ModContext* ctx, ConfigVarHandle var, const ConfigVarValue* value,
    const ConfigVarValue* previous, void* user_data) {
    /* value->float_value is the new value, previous->float_value the old one */
}
svc_config->subscribe(mod_ctx, var, on_speed_changed, nullptr, nullptr);
```

Types: `CONFIG_VAR_BOOL` (`bool`), `CONFIG_VAR_INT` (`int64_t`), `CONFIG_VAR_FLOAT` (`double`), `CONFIG_VAR_STRING`
(UTF-8; `get_string` copies into a caller buffer, pass a `NULL` buffer with size 0 to query the length). Accessors are
typed and must match the registration.

Change callbacks fire on the game thread whenever the value changes at runtime (your own `set_*` calls included).
Writes that store the same value are silent. Values applied from `config.json` or `--cvar` at registration do
**not** fire callbacks; read the value after `register_var` for the starting state.

### UiService (`mods/svc/ui.h`)

Integrate seamlessly with Dusklight's UI system: add controls and buttons to your mod's detail pane in the Mods window,
create custom windows and modal dialogs, apply custom RCSS stylesheets (anywhere!), and add menu bar tabs.

**Mod panel:** Registers or replaces the panel rendered in your mod's detail pane; `build` runs every time the detail
content is rebuilt, and `update` runs every frame while that mod is selected. While your mod is selected, the detail
pane carries your mod's id as a `mod-id` attribute (like custom window roots), so scoped RCSS can target it (e.g.
`[mod-id="com.example.mod"]`).

```cpp
IMPORT_SERVICE(UiService, svc_ui);

UiElementHandle statusText = 0;

ModResult build(ModContext*, UiElementHandle panel, void*, ModError*) {
    svc_ui->pane_add_section(mod_ctx, panel, "Status");
    svc_ui->pane_add_text(mod_ctx, panel, "starting...", &statusText);
    svc_ui->pane_add_progress(mod_ctx, panel, 0.5f, nullptr);
    return MOD_OK;
}

ModResult update(ModContext*, void*, ModError*) {
    svc_ui->elem_set_text(mod_ctx, statusText, "running");
    return MOD_OK;
}

UiModsPanelDesc panel = UI_MODS_PANEL_DESC_INIT;
panel.build = build;
panel.update = update;
svc_ui->register_mods_panel(mod_ctx, &panel);
```

Element setters must match the element kind: `elem_set_text`/`elem_set_rml` on text rows, and `elem_set_progress` on
progress bars. `elem_set_class` sets or clears an RCSS class on any element handle, for styling via scoped or
per-window RCSS. A non-`MOD_OK` result from `build`/`update` fails your mod, as do exceptions thrown from any UI
callback.

**Controls:** `pane_add_control` adds an input row described by a `UiControlDesc`: `UI_CONTROL_BUTTON`,
`UI_CONTROL_TOGGLE`, `UI_CONTROL_NUMBER`, `UI_CONTROL_STRING`, or `UI_CONTROL_SELECT`. Values bind with callbacks or
directly to a config var.

```cpp
UiControlDesc control = UI_CONTROL_DESC_INIT;
control.kind = UI_CONTROL_TOGGLE;
control.label = "Enable rainbows";
control.help_rml = "Shown in the help pane while focused.";
control.binding = UI_BINDING_CONFIG_VAR;
control.config_var = myBoolVar;  // from svc_config->register_var
svc_ui->pane_add_control(mod_ctx, leftPane, &control, nullptr);
```

`UI_BINDING_CONFIG_VAR` wires persistence, change notifications, and the modified indicator automatically. The var
type must match the control: `TOGGLE` = bool, `NUMBER` and `SELECT` = int, `STRING` = string. Float vars are not
bindable; use callbacks and convert. `help_rml` and `SELECT` option lists render in a help pane, so `SELECT` controls
are only available inside window tabs.

**Windows:** `window_push` pushes a tabbed two-pane window onto the document stack and shows it. Each tab's `build`
receives the window handle plus fresh left and right pane handles on every activation. The optional per-tab `update`
runs each frame while that tab is active. `on_closed` fires when the window is destroyed. `desc.rcss` optionally styles
that window's document only; custom windows carry the owning mod's id as a `mod-id` attribute on the window root, so
scoped RCSS can target your specific mod's windows (e.g. `window[mod-id="com.example.mod"]`).

```cpp
UiTabDesc tabs[1] = {UI_TAB_DESC_INIT};
tabs[0].title = "Options";
tabs[0].build = build_options_tab;

UiWindowDesc desc = UI_WINDOW_DESC_INIT;
desc.tabs = tabs;
desc.tab_count = 1;
desc.on_closed = options_window_closed;
UiWindowHandle window = 0;
svc_ui->window_push(mod_ctx, &desc, &window);
```

**Dialogs:** `dialog_push` shows a modal dialog. `variant` picks the style, `icon` optionally overrides the variant's
default icon, and actions become buttons. After an action's `on_pressed` returns, the dialog closes unless the action
sets `keep_open`. A `keep_open` action can close it later (or immediately) with `dialog_close`. Cancel fires
`on_dismiss` if present and always closes. `dialog_set_body`, `dialog_set_icon`, and `dialog_add_action` mutate a live
dialog.

**Menu bar tabs:** `register_menu_tab` adds a tab to the in-game menu bar. `on_selected` fires when the user activates
the tab: typically you'd push a window from it. The tab is removed by `unregister_menu_tab`, or automatically when the
mod is disabled.

**Custom styles:** `register_styles(scope, rcss, &handle)` applies an RCSS stylesheet to every document of a scope:
existing documents restyle immediately, and future ones pick it up when created. `register_styles_file(scope, path,
&handle)` reads the sheet from your bundle's `res/` directory. Scopes are `UI_SCOPE_PRELAUNCH`, `UI_SCOPE_WINDOW`,
`UI_SCOPE_MENU_BAR`, `UI_SCOPE_OVERLAY`, `UI_SCOPE_TOUCH_CONTROLS`, and `UI_SCOPE_GRAPHICS_TUNER`. Sheets apply after
host styles and may override them. Scope selectors tightly (use `[mod-id="..."]`!), especially for `UI_SCOPE_WINDOW`,
unless changing host UI is intentional.

### GfxService (`mods/svc/gfx.h`)

**Requires `add_mod(... FEATURES webgpu)`**

Direct WebGPU access at various stages of the rendering pipeline. Mods use the `wgpu*` C API (via `webgpu/webgpu.h`) for
custom draws and compute dispatches. Mods must manage their own WebGPU state, including pipelines and bind groups.

```cpp
IMPORT_SERVICE(GfxService, svc_gfx);

GfxDeviceInfo info = GFX_DEVICE_INFO_INIT;
svc_gfx->get_device_info(mod_ctx, &info);
```

`register_stage_hook` runs a game-thread callback during frame recording. The public stages are:

- `GFX_STAGE_SCENE_BEGIN`: world camera window after camera/projection/light setup
- `GFX_STAGE_SCENE_AFTER_TERRAIN`: after terrain/shadow lists, before object and translucent lists
- `GFX_STAGE_SCENE_AFTER_OPAQUE`: after sky/terrain/object opaque lists, before translucent lists
- `GFX_STAGE_FRAME_BEFORE_HUD`: 3D scene and wipe are complete, before 2D/HUD lists
- `GFX_STAGE_FRAME_AFTER_HUD`: full game scene, including HUD

Inside a stage callback, record work with `push_draw`, stream per-frame data with `push_verts`, `push_indices`,
`push_uniform`, or `push_storage`, snapshot the current frame with `resolve_pass`, and use `create_pass`/`resolve_pass`
for temporary offscreen passes. Draw callbacks run later on the render worker thread with the live
`WGPURenderPassEncoder`; they may use only their `GfxDrawContext` handles and raw `wgpu*` calls. Compute callbacks
registered with `register_compute_type` follow the same worker-thread rule and run on the frame command encoder.

All WGPU handles from the service are borrowed. Resolved target views are valid for the current frame only. GPU objects
created by a mod are owned by that mod and should be released in `mod_shutdown`.

### CameraService (`mods/svc/camera.h`)

Converts a game view provided by a render callback into WebGPU-convention camera data. Matrix fields are column-major
`float[16]` values using the matrix * column-vector convention (transpose of the game's row-major `Mtx`/`Mtx44` layout),
ready to copy into WGSL `mat4x4f` uniforms.

```cpp
IMPORT_SERVICE(CameraService, svc_camera);

CameraInfo camera = CAMERA_INFO_INIT;
if (svc_camera->get_camera(mod_ctx, game_view, &camera) == MOD_OK) {
    // camera.view_from_world, camera.proj_from_view, camera.eye, ...
}
```

`get_camera` returns `MOD_UNAVAILABLE` while the view is not a valid perspective camera, such as before the
first in-game frame. Projection matrices match the renderer's WebGPU clip convention and renderer depth convention
(reversed-Z by default).

---

## Hooking Game Functions

**Requires `add_mod(... FEATURES game)`**

Mods may hook the vast majority of game functions, including file-local static, private and virtual functions.
`mods/hook.hpp` provides typed helpers over the hook service:

```cpp
#include "mods/hook.hpp"
#include "mods/svc/hook.h"

IMPORT_SERVICE(HookService, svc_hook);

DEFINE_HOOK(&daAlink_c::posMove, LinkPosMove);
DEFINE_HOOK(&daAlink_c::execute, LinkExecute);
```

Every hook target must be **declared** at namespace scope with `DEFINE_HOOK` (a target you can name in C++) or
`DEFINE_HOOK_SYMBOL` (a symbol name).

### Pre-hooks

Run before the original. Return `HOOK_SKIP_ORIGINAL` to cancel it (post-hooks still run).

```cpp
HookAction on_pos_move_pre(ModContext*, void* args, void* retval, void* userdata) {
    daAlink_c* link = dusk::mods::arg<daAlink_c*>(args, 0);  // arg 0 is `this`
    if (link->shape_angle.y > 10000) {
        return HOOK_SKIP_ORIGINAL;
    }
    return HOOK_CONTINUE;
}

dusk::mods::hook_add_pre<LinkPosMove>(svc_hook, on_pos_move_pre);
```

### Post-hooks

Run after the original (or after a replace-hook, or after a cancelled original). `retval` points to the return value,
if any.

```cpp
void on_pos_move_post(ModContext*, void* args, void* retval, void* userdata) { ... }

dusk::mods::hook_add_post<LinkPosMove>(svc_hook, on_pos_move_post);
```

### Replace-hooks

Substitute the original entirely. Call through to it via the declaration's `g_orig` if needed:

```cpp
void on_execute_replace(ModContext*, void* args, void* retval, void*) {
    int result = LinkExecute::g_orig(dusk::mods::arg<daAlink_c*>(args, 0));
    if (retval != nullptr) {
        *static_cast<int*>(retval) = result;
    }
}

dusk::mods::hook_replace<LinkExecute>(svc_hook, on_execute_replace);
```

By default a second replace-hook on the same function is a conflict; `HookOptions` (`replace_policy`, `priority`,
`userdata`) controls this and callback ordering. Multiple mods can attach pre/post hooks to the same function
independently.

### Hooking by name

Functions you can't name in C++ (file-local statics, private class members, anything not in a header) can be hooked by
symbol name instead. You must supply the signature along with the name.

```cpp
DEFINE_HOOK_SYMBOL("daAlink_hookshotAtHitCallBack",
    void(fopAc_ac_c*, dCcD_GObjInf*, fopAc_ac_c*, dCcD_GObjInf*), HookshotHit);

dusk::mods::hook_add_pre<HookshotHit>(svc_hook, on_hookshot_hit_pre);
...
HookshotHit::g_orig(link, atObjInf, target, tgObjInf);  // call through to the original
```

Class member functions must include `Class*` as the first argument.

Two spellings work on every platform:

- **Display names** (`daAlink_c::posMove`, `fapGm_Before`): the qualified name with no parameter list. They carry no
  signature, so overload sets (and file-local statics sharing a name) return `MOD_CONFLICT`.
- **Decorated names** (`_ZN9daAlink_c7posMoveEv` / `?posMove@daAlink_c@@...`): the platform's mangled spelling in
  dlsym convention (no Mach-O leading underscore). The escape hatch for overloads.

Installing fails with `MOD_UNAVAILABLE` when it didn't resolve (missing, ambiguous, or no symbol manifest). Unlike
`DEFINE_HOOK`, the signature is **not** compiler-checked: a mismatched signature will corrupt the
call.

### Reading and writing arguments

`args` is an array of pointers to the arguments. For member functions, index 0 is `this`; parameters follow in
declaration order.

```cpp
T  value = dusk::mods::arg<T>(args, n);      // copy
T& ref   = dusk::mods::arg_ref<T>(args, n);  // read/write reference
```

```cpp
DEFINE_HOOK(fopAcM_createItem, CreateItem);

// fpc_ProcID fopAcM_createItem(..., int itemNo, ...): turn heart drops into green rupees
HookAction on_create_item_pre(ModContext*, void* args, void*, void*) {
    int& itemNo = dusk::mods::arg_ref<int>(args, 1);
    if (itemNo == dItemNo_HEART_e) {
        itemNo = dItemNo_GREEN_RUPEE_e;
    }
    return HOOK_CONTINUE;
}

dusk::mods::hook_add_pre<CreateItem>(svc_hook, on_create_item_pre);
```

For reference parameters (e.g. `const cXyz& pos`), `arg_ref<cXyz>` yields a direct reference.

---

## Asset Overlays

Files placed under `overlay/` in the `.dusk` archive override game files at the corresponding path, equivalent to
replacing files in the .iso. This requires no code: an archive with just `mod.json` and `overlay/` is a complete mod.

Files placed under `textures/` register as texture replacements, and act just like the user's general
`texture_replacements/` directory: Dolphin-style naming, matched by texture hash
(`tex1_{w}x{h}_{texhash}[_{tluthash}]_{fmt}.dds|.png`, `$` as a hash wildcard). Subdirectories are scanned recursively;
only the filename needs to match.

Both mechanisms are tied to the mod's lifecycle: disabling the mod removes its overrides (files revert to the disc
contents on their next open; added files stop existing), and reloading serves the new bundle's content. However, game
data the engine already read stays as-is until it is loaded again, which may require a scene change or, in the worst
case, a full restart. Texture replacements usually take effect immediately.

If multiple sources replace the same file or texture, the last one wins: runtime registrations override static
`textures/` or `overlay/` files, and later-loaded mods override earlier ones. Cross-mod conflicts log warnings.
**All** mod-provided texture replacements override the user's `texture_replacements/`.

To configure overlays and texture replacements at runtime instead, see [OverlayService](#overlayservice-modssvcoverlayh)
and [TextureService](#textureservice-modssvctextureh).

---

## Runtime Lifecycle

Mods can be disabled, re-enabled, and reloaded at runtime without restarting the game (the enabled state persists as the
`mod.<escaped id>.enabled` config var). Write your mod assuming this happens:

- **Disable** calls `mod_shutdown`, removes your hooks, services, overlays, and texture replacements (both static and
  runtime-registered), and unloads your library.
- **Enable** and **Reload** load a *fresh copy* of your library, imports are re-resolved, and `mod_initialize` runs
  again. You never see a second `mod_initialize` on the same image, so just make `mod_shutdown` release anything the
  loader doesn't manage for you (threads, files, game-side state you mutated).
- **Reload** additionally re-reads the `.dusk` from disk, picking up a rebuilt library and changed assets. This is the
  fast iteration loop during development: rebuild, click Reload.

**Dependents restart too.** Disabling or reloading a mod that exports services shuts down the mods importing them
first (in reverse dependency order) and brings them back afterward. A mod whose *required* provider is disabled stays
suspended and resumes automatically when the provider returns. Mods with an *optional* import of a disabled provider
restart with that import null.

**One caution for hooks:** lifecycle changes are applied between frames, which is safe for hooks on functions
that return every frame (effectively everything you'd normally hook). Avoid hooking a function that stays on
the stack for the whole session (e.g. the outermost main loop); a mod that does cannot be safely unloaded.

---

## Error Handling

Service calls report failure through `ModResult` return values (`MOD_OK`, `MOD_UNAVAILABLE`,
`MOD_INVALID_ARGUMENT`, ...). Lifecycle exports additionally receive a `ModError*`: fill it (e.g. with
`dusk::mods::set_error(error, code, "message")`) and return the code, and the loader disables the mod and shows the
message to the user.

```cpp
MOD_EXPORT ModResult mod_initialize(ModError* error) {
    if (!load_my_data()) {
        return dusk::mods::set_error(error, MOD_ERROR, "failed to load data");
    }
    return MOD_OK;
}
```

Throwing exceptions out of lifecycle functions also disables the mod (they are caught by the loader), but prefer
explicit results.

---

## Advanced

### Exporting Services

Mods may export services of their own, permitting framework mods and cross-mod integration. Define the interface in a
header both mods share:

```cpp
// my_mod_api.h
#include "mods/api.h"

#define MY_MOD_SERVICE_ID "com.example.my_mod.api"
#define MY_MOD_SERVICE_MAJOR 1u
#define MY_MOD_SERVICE_MINOR 0u

typedef struct MyModService {
    ServiceHeader header;
    ModResult (*do_thing)(ModContext* ctx, int value);
} MyModService;

#ifdef __cplusplus
#include "mods/service.hpp"
template <>
struct dusk::mods::ServiceTraits<MyModService> {
    static constexpr const char* id = MY_MOD_SERVICE_ID;
    static constexpr uint16_t major_version = MY_MOD_SERVICE_MAJOR;
    static constexpr uint16_t minor_version = MY_MOD_SERVICE_MINOR;
};
#endif
```

**Provider:**

```cpp
ModResult do_thing(ModContext* ctx, int value) { ... }

constexpr MyModService g_service{
    .header = SERVICE_HEADER(MyModService, MY_MOD_SERVICE_MAJOR, MY_MOD_SERVICE_MINOR),
    .do_thing = do_thing,
};
EXPORT_SERVICE(g_service);
```

**Consumer:**

```cpp
IMPORT_SERVICE(MyModService, svc_my_mod);
// or IMPORT_OPTIONAL_SERVICE if the dependency is optional

svc_my_mod->do_thing(mod_ctx, 42);
```

The loader registers all exports before resolving any imports, so declaration order between mods doesn't matter. Note
that the `ctx` a provider receives identifies the *calling* mod.

#### Dependencies between mods

Service imports are also dependency declarations: the loader initializes mods in dependency order, so by the time your
`mod_initialize` runs, every mod you import services from (required *or* optional) has already finished its own
`mod_initialize`. This includes deferred services: a service the provider publishes during its initialization resolves
into your import slot just like a static export.

Consequences of that contract:

- If a provider fails to load, every mod that *requires* one of its services is disabled too, with an error naming the
  provider. Optional imports of a failed provider simply resolve to `NULL`.
- Mods whose **required** imports form a cycle all fail to load. If the cycle runs through an **optional** import, the
  loader breaks it there: the optional import still resolves, but its provider may not be initialized yet when you run.
- `svc_host->get_service(...)` is outside this system. It sees whatever is published at call time and gives no
  initialization-order guarantee, which also makes it the escape hatch for intentionally cyclic designs.

Mods shut down in reverse initialization order, so services you import remain safe to call from `mod_shutdown`.

Rules for providers:

- Service IDs are global and use reverse-DNS names (e.g. `com.mydomain.mod.service`)
- Every function pointer covered by your declared minor version must be populated.
- Within a major version, only append fields; never reorder, remove, or repurpose them. Breaking changes require a major
  bump (which is, in effect, a new service).
- Only one provider per `(id, major)` pair may be registered; duplicates are load errors.

For services whose construction can't happen at static-init time, declare the export with `EXPORT_DEFERRED_SERVICE(...)`
and publish the pointer later via `svc_host->publish_service(...)`. Consumers can fetch services dynamically with
`svc_host->get_service(...)`; prefer manifest imports whenever possible, since they give the loader dependency
information and fail fast with good errors.

### Native Runtime Libraries

`RUNTIME_LIBRARIES` passed to `add_mod` are packaged beside the mod's native module in `lib/<platform>/`. Dusklight
extracts the whole directory before loading the mod, so libraries linked by the mod resolve normally. The SDK links the
mod itself with `$ORIGIN` on Linux and `@loader_path` on Apple platforms; runtime libraries with their own non-system
dependencies must also be built with origin-relative lookup paths. On Windows, Dusklight uses an isolated DLL search
rooted at this directory.

```cmake
add_mod(my_mod
        SOURCES src/mod.cpp
        MOD_JSON mod.json
        RUNTIME_LIBRARIES "${VENDOR_RUNTIME_LIBRARY}")
```

SDKs that load plugins by directory can pass them the absolute runtime path from the current HostService:

```cpp
IMPORT_SERVICE(HostService, svc_host);

const char* nativeDir = svc_host->native_dir(mod_ctx);  // read-only
```

Libraries loaded explicitly by the mod remain its responsibility: stop their threads and unload them during
`mod_shutdown`. Do not write into `native_dir`; use `mod_dir` for writable state. Native library namespaces are
process-wide on some platforms, so two mods cannot safely assume that incompatible libraries with the same filename
will remain isolated.
