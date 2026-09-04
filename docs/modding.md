# Dusklight Mod API

Mods are `.dusk` bundles: zip archives that can contain code (native libraries or scripts), resources, DVD overlay
files, and texture replacements. Mods may be enabled, disabled and reloaded at runtime.

There are three types of mods:

- **Asset-only mods**: Mods that contain no code, and may replace files on the game disc ("overlays") and provide
  replacement textures (texture packs).
- **Native mods (C++)**: Fully-featured, with the ability to interop with game code and hook functions. Must be compiled
  for every supported platform. (See [mod-template](https://github.com/TwilitRealm/mod-template))
- **Script mods (Luau)**: Simple, widely-compatible, but can only access provided services.

## Table of Contents

1. [mod.json](#modjson)
2. [Asset-only Mods](#asset-only-mods)
3. [Native Mods (C++)](#native-mods)
4. [Script Mods (Luau)](#script-mods)
5. [Services](#services)
6. [Built-in Services](#built-in-services)
7. [Hooking Game Functions](#hooking-game-functions)
8. [Asset Overlays](#asset-overlays)
9. [Runtime Lifecycle](#runtime-lifecycle)
10. [Error Handling](#error-handling)
11. [Advanced](#advanced)

---

## mod.json

Every mod starts with a single file, a `mod.json`:

```json
{
  "id": "com.example.my_mod",
  "name": "My Mod",
  "version": "1.0.0",
  "author": "Your Name",
  "description": "A short description shown in the mod manager.",
  "icon": "res/icon.png",
  "banner": "res/banner.png"
}
```

`id` is required: a unique, stable identifier (reverse-DNS style; periods, underscores, and lowercase alphanumerics).
Everything else is optional but recommended.

`icon` and `banner` are bundle paths to PNG images that display in the in-game mod manager and mod website. A square
icon (1:1), and a banner (~3.5:1, minimum 800px width). If omitted, `res/icon.png` and `res/banner.png` are used
automatically when present.

Simply create a zip file with a `mod.json`, and rename it to `.dusk`. That's it!

---

## Asset-only Mods

```
my_mod.dusk
├── mod.json
├── res/       (optional bundled resources)
├── overlay/   (optional game file overrides)
└── textures/  (optional texture replacements)
```

Place files in `overlay/` to replace the disc version of that file. Examples:

- `overlay/Movie/demo_movie98_00.thp`: Replaces `Movie/demo_movie98_00.thp` on disc.
- `overlay/res/Object/Kmdl/archive/bmwr/al.bmd`: Replaces `archive/bmwr/al.bmd` _within_ `res/Object/Kmdl.arc` without
  overwriting the entire `.arc`.

Place textures in `textures/` to automatically register them as texture replacements when active. These follow the same
[Dolphin-compatible naming scheme](#textureservice-modssvctextureh) as the user `<data>/texture_replacements/`
directory. Directories are scanned recursively. Examples:

- `textures/tex1_256x128_e6a4c7be9bf48305_14.dds`: Replaces the Hero's Clothes texture.

Simply zip the `mod.json` and adjacent folders, then rename to `.dusk`. Then, copy the `.dusk` into the user mods
folder:

- Windows: `%APPDATA%\TwilitRealm\Dusklight\mods`
- Linux: `~/.local/share/TwilitRealm/Dusklight/mods`
- macOS: `~/Library/Application Support/TwilitRealm/Dusklight/mods`

---

## Native Mods

Mods built with [mod-template](https://github.com/TwilitRealm/mod-template) are native C++ mods. Native mods are very
powerful and can interact with and [hook](#hooking-game-functions) game code directly. All features are available to
native mods.

Example C++ mod:

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

When native mods are loaded, they get dynamically linked by the operating system to the running game process. The mod
exports lifecycle functions that Dusklight calls into (`mod_initialize`, `mod_update`, `mod_shutdown`), and the mod
communicates with the host via **services**: plain C APIs, individually versioned. Dusklight exports several built-in
services, and mods may export services of their own, permitting framework mods and cross-mod integration.

Beyond services, native mods have full access to the original game's code: include game headers, call directly into any
public function, read and write data fields, and hook the vast majority of game functions.

### Quick Start (Native Mods)

Create a repository from
the [mod-template](https://github.com/new?template_name=mod-template&template_owner=TwilitRealm),
a self-contained CMake project that uses the Dusklight mod SDK. It includes a GitHub Actions CI workflow that builds the
mod for every supported platform.

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
cmake_minimum_required(VERSION 3.26)
project(my_mod CXX)

if (NOT DUSKLIGHT_VERSION)
    set(DUSKLIGHT_VERSION "76b56cd8b81809fce0a5c2a44e2f6d437591132f")
endif ()
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/FetchDusklight.cmake")
add_subdirectory("${DUSKLIGHT_DIR}/sdk" dusklight-sdk EXCLUDE_FROM_ALL)

add_mod(my_mod
        FEATURES game fmt      # remove game for service-only mods; add webgpu for GfxService
        SOURCES src/mod.cpp
        MOD_JSON mod.json
        RES_DIR res            # mod resources, including icon.png and banner.png
        OVERLAY_DIR overlay    # game file overlays; remove if unused
        TEXTURES_DIR textures  # texture replacements; remove if unused
)
```

Available features:

- `fmt`: Provides the header-only `{fmt}` library and the formatted logging helpers in
  [`mods/svc/log.hpp`](../sdk/include/mods/svc/log.hpp).
- `game`: Allows calling into and hooking game code. Mods that **only** use services may omit it, providing a wider
  range of compatibility with Dusklight versions and a slightly faster build process.
- `webgpu`: Allows importing the WebGPU API
  ([`webgpu/webgpu.h`](https://github.com/webgpu-native/webgpu-headers/blob/main/webgpu.h)). Must be enabled when using
  [GfxService](#gfxservice-modssvcgfxh).

Building produces `my_mod.dusk` in `build/mods/`. Copy the `.dusk` into the user mods folder:

- Windows: `%APPDATA%\TwilitRealm\Dusklight\mods`
- Linux: `~/.local/share/TwilitRealm/Dusklight/mods`
- macOS: `~/Library/Application Support/TwilitRealm/Dusklight/mods`

Passing `--mods <dir>` on the command line replaces the user directory with one of your choosing.

---

## Script Mods

For simpler use cases where direct game code access and hooks aren't necessary, Luau mods are also supported.
Luau mods do not need to be compiled, and are always supported on all platforms. However, they can only use services
that are provided as Luau modules.

Mods can utilize Luau to add UI elements to their mod panel, use configuration variables, and dynamically swap out
models/textures at runtime (e.g. to switch model variants or disable certain replacements) while remaining widely
compatible.

Example mod structure:

```
my_luau_mod.dusk
├── mod.json
└── res/
    ├── main.luau
    └── lib/util.luau
```

Set the `runtime` to `dev.twilitrealm.luau@1.0` in `mod.json`:

```json
{
  "id": "com.example.my_luau_mod",
  "name": "My Script Mod",
  "version": "1.0.0",
  "runtime": "dev.twilitrealm.luau@1.0"
}
```

`res/main.luau` runs once when the mod activates. Register optional update and shutdown callbacks through
`dusklight.host`:

```lua
local host = require("dusklight.host")
host.on_update(function()
    -- Runs every frame
end)
host.on_shutdown(function()
    -- Runs on mod deactivation
end)
```

Require script modules with `./` or `../` paths relative to the requiring file. The runtime appends `.luau` and rejects
paths that escape `res/`.

The runtime provides these modules:

- `dusklight.log`: `write`, `trace`, `debug`, `info`, `warn`, and `error`.
- `dusklight.host`: host `version`, mod metadata and directories, lifecycle callbacks, and `fail`.
- `dusklight.config`: bool, integer, float, and string variables with `get`, `set`, and subscriptions.
- `dusklight.resource`: binary-safe reads from the mod's `res/` tree.
- `dusklight.overlay`: file and copied-buffer overlays with removable handles.
- `dusklight.texture`: encoded-file and raw-data replacements with unregisterable handles.
- `dusklight.ui`: Mods panels, controls, lists, windows, dialogs, styles, menu tabs, toasts, and clipboard access.

For completion and type checking, add `sdk/luau/dusklight.d.luau` to the `luau-lsp.types.definitionFiles` setting.

Configurable overlay example:

```lua
local config = require("dusklight.config")
local overlay = require("dusklight.overlay")

local hood = config.register({ name = "hood", type = "bool", default = true })
local current

local function apply(enabled)
    if current then current:remove() end
    current = overlay.add_file("/res/Object/Alink.arc",
        enabled and "res/alink_hood.arc" or "res/alink_nohood.arc")
end

hood:subscribe(apply)
apply(hood:get())
```

To create a script mod, copy `sdk/luau/template`, edit its manifest and source, then zip the `mod.json` and adjacent
folders and rename to `.dusk`. Copy the `.dusk` into the user mods folder:

- Windows: `%APPDATA%\TwilitRealm\Dusklight\mods`
- Linux: `~/.local/share/TwilitRealm/Dusklight/mods`
- macOS: `~/Library/Application Support/TwilitRealm/Dusklight/mods`

**Restrictions:** The Luau VM has no raw filesystem, network, or game-code access. Each script mod has a 64 MiB memory
limit. Calls are interrupted after 250 ms for updates and UI callbacks or 5 seconds for lifecycle calls. An uncaught
error, timeout, or memory exhaustion fails and disables the mod.

---

## Services

A service is a struct of C function pointers with a version header. Import services at file scope, and the loader
resolves them before initializing the mod:

```cpp
IMPORT_SERVICE(LogService, svc_log);              // required, latest minor version
IMPORT_SERVICE_VERSION(LogService, svc_log, 0);   // required, minimum minor version 0 (for backwards compatibility)
IMPORT_OPTIONAL_SERVICE(SomeService, svc_maybe);  // may be null
```

A service should be imported in only **one** file (usually your `mod.cpp`). Other files may simply use `svc_log` or
`mods::log::` after including the appropriate header.

Each service is individually versioned, and there may be multiple major versions of a service provided at once,
allowing backwards compatibility with older mods while still changing services fundamentally if necessary. A **major**
bump is a breaking change, treated as a different service entirely. For **additive** changes, a service appends new
functions to the end of the struct without breaking existing callers and simply bumps the minor version.

`IMPORT_SERVICE` and `IMPORT_OPTIONAL_SERVICE` require the latest minor version compiled against, guaranteeing that
every function is present. If a mod doesn't use (or may operate without) functions added in later minor versions, and
wants to remain compatible with older Dusklight versions, it may use `IMPORT_SERVICE_VERSION` or
`IMPORT_OPTIONAL_SERVICE_VERSION` to require an older minor version. It can then check `SERVICE_HAS` at runtime to see
if a newer function is present (i.e. running on a new enough Dusklight version).

---

## Built-in Services

### LogService ([`mods/svc/log.h`](../sdk/include/mods/svc/log.h))

**C++**

```cpp
IMPORT_SERVICE(LogService, svc_log);

svc_log->info(mod_ctx, "spawned the thing");
svc_log->warn(mod_ctx, "that looks wrong");
svc_log->error(mod_ctx, "very bad");
svc_log->write(mod_ctx, LOG_LEVEL_DEBUG, "verbose details");
```

**Luau**

```lua
local log = require("dusklight.log")
log.info("spawned the thing")
```

Messages appear in the console prefixed with your mod ID. Messages are plain UTF-8 strings and are copied before the
call returns. C++ mods can enable `add_mod(... FEATURES fmt)` and use the formatted logging helpers in
[`mods/svc/log.hpp`](../sdk/include/mods/svc/log.hpp):

```cpp
#include <mods/svc/log.hpp>

mods::log::info("spawned actor {} at ({}, {})", actorName, x, y);
mods::log::warn("health is down to {:.1f}%", healthPercent);
```

### ResourceService ([`mods/svc/resource.h`](../sdk/include/mods/svc/resource.h))

Loads files from the `res/` tree of your `.dusk` archive. Paths are relative to `res/` (pass `"config.txt"`, not
`"res/config.txt"`); absolute paths and `..` are rejected.

**C++**

```cpp
IMPORT_SERVICE(ResourceService, svc_resource);

ResourceBuffer buf = RESOURCE_BUFFER_INIT;
if (svc_resource->load(mod_ctx, "config.txt", &buf) == MOD_OK) {
    // buf.data / buf.size
    svc_resource->free(mod_ctx, &buf);
}
```

**Luau**

```lua
local resource = require("dusklight.resource")
local contents = resource.load("config.txt")
```

Missing files return `MOD_UNAVAILABLE`. Always `free` what you `load`. The bundle is read-only; use
`HostService::data_dir` for persistent storage.

### FileService ([`mods/svc/file.h`](../sdk/include/mods/svc/file.h))

Provides file and folder pickers, file I/O, exports and folder enumeration.

A location is an opaque UTF-8 string returned by `pick_*`, `export_file`, `join`, or `create_child`.
Save it and pass it back to the service. Never parse it or manually append path segments.

```cpp
#include "mods/svc/file.hpp"

IMPORT_SERVICE(FileService, svc_file);

mods::file::PickOptions options;
options.filters.push_back({"Audio", "wav;ogg"});
mods::file::pick_file(options, [](mods::file::PickResult result) {
    if (result.status == MOD_OK && !result.locations.empty()) {
        save_location(result.locations.front());
    }
});
```

Use `check` before reopening a saved location because removable storage or an access grant may no longer be available.
`open` provides seekable streaming I/O. `read_all` allocates the entire file and should only be used when the file is
small. Folder locations support `list` and child resolution through `join`. Only one picker can be open at a time.

`create_child` never replaces an existing file and returns `MOD_CONFLICT` if one exists. Use the returned location;
document providers may adjust the requested name. `write_all` is a convenience function over
`open`/`write`/`flush`/`close`.

```cpp
std::string location;
if (mods::file::create_child(folder, "report.txt", location) == MOD_OK) {
    mods::file::write_all(location, report);
}

mods::file::export_file(location, "report.txt", [](mods::file::PickResult result) {
    if (result.status == MOD_OK) {
        remember_export_destination(result.locations.front());
    }
});
```

`export_file` copies an existing file to a user-selected destination and returns the destination location in its
callback. Mod-owned persistent files belong in `HostService::data_dir`.

### HttpService ([`mods/svc/http.h`](../sdk/include/mods/svc/http.h))

Asynchronous HTTPS requests supporting HTTP/2 and TLS 1.2+. C++ mods should use the helpers in
[`mods/svc/http.hpp`](../sdk/include/mods/svc/http.hpp):

```cpp
#include "mods/svc/http.hpp"

IMPORT_SERVICE(HttpService, svc_http);

mods::http::Pending pendingRequest;

void fetch_manifest() {
    mods::http::Request request{
        .url = "https://example.com/manifest.json",
        .maxBodyBytes = 256 * 1024,
    };
    pendingRequest = mods::http::request(request, [](mods::http::Response response) {
        if (!response.ok()) {
            handle_fetch_error(response.error, response.statusCode);
            return;
        }

        std::string manifest{response.body.begin(), response.body.end()};
        use_manifest(manifest);
    });
    if (!pendingRequest) {
        handle_start_error(pendingRequest.result());
    }
}
```

Keep the returned `Pending` alive until completion. Dropping it or calling `cancel` requests cancellation. Callbacks run
on the game thread.

`Response::ok()` requires a 2xx status. Other HTTP statuses are valid responses, not transport errors, so always check
`statusCode`. In-memory responses default to a 1 MiB limit; set `maxBodyBytes` to increase it if needed.

For large responses, set `downloadPath` to an absolute path in the calling mod's `HostService::data_dir` or
`HostService::mod_dir`. The response is streamed to disk instead of loaded in memory. On success, the callback receives
an empty `body` and the final path in `downloadPath`. Check `Response::ok()` before using the file.
`Pending::progress()` reports download progress when the server provides a total size.

### WebSocketService ([`mods/svc/websocket.h`](../sdk/include/mods/svc/websocket.h))

WebSocket client connections with text or binary messages. Secure `wss://` URLs are supported everywhere. Insecure
`ws://` is limited to `localhost`, `127.0.0.1`, and `[::1]`.

```cpp
#include "mods/svc/websocket.hpp"

IMPORT_SERVICE(WebSocketService, svc_websocket);

mods::ws::Connection connection;

MOD_EXPORT ModResult mod_initialize(ModError*) {
    connection = mods::ws::connect({.url = "wss://example.com/events"});
    return connection ? MOD_OK : connection.result();
}

MOD_EXPORT ModResult mod_update(ModError*) {
    mods::ws::Event event;
    while (mods::ws::poll(event)) {
        if (event.type == WEBSOCKET_EVENT_MESSAGE) {
            consume(event.data);
        } else if (event.type == WEBSOCKET_EVENT_CLOSED) {
            schedule_reconnect(event.error);
        }
    }
    return MOD_OK;
}
```

Keep the `Connection` alive and drain `mods::ws::poll()` regularly, usually on every `mod_update` tick. A connection
emits an (optional) `OPEN` event, zero to many `MESSAGE` events, and exactly one `CLOSED` event.

**Restrictions:** A mod may only open four connections at once. Messages have a 1 MiB limit by default, and may request
up to 16 MiB. Unread data is limited to 16 MiB and the outbound queue to 4 MiB. `send` returns `MOD_CONFLICT` when the
outbound queue is full. Dropping the `Connection` or deactivating the mod attempts to gracefully close with code 1001,
until the close deadline expires.

### NetService ([`mods/svc/net.h`](../sdk/include/mods/svc/net.h))

Asynchronous raw TCP and UDP networking. Endpoints can be `tcp://host:port` or `udp://host:port`. TCP connections and
`resolve` accept hostnames. Listeners and UDP endpoints require IP literals.

```cpp
#include "mods/svc/net.hpp"

IMPORT_SERVICE(NetService, svc_net);

mods::net::BindOutcome bound;
mods::net::Socket listener;
mods::net::Socket client;

MOD_EXPORT ModResult mod_initialize(ModError*) {
    listener = mods::net::listen("tcp://127.0.0.1:0", &bound);
    client = mods::net::connect(bound.local);
    return listener && client ? MOD_OK : MOD_ERROR;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    mods::net::Event event;
    while (mods::net::poll(event)) {
        if (event.type == NET_EVENT_ACCEPTED) {
            remember_client(mods::net::adopt(event.accepted));
        } else if (event.type == NET_EVENT_STREAM_DATA) {
            consume(event.data);
        }
    }
    return MOD_OK;
}
```

`send` and `send_to` copy the payload and return `MOD_CONFLICT` when the socket's outbound queue is full. `stats`
reports queued bytes, traffic, dropped inbound datagrams, and asynchronous UDP send failures.

**Restrictions:** A mod may only have 32 streams, 4 listeners, 4 UDP sockets, and 8 DNS resolutions active at once.

### HostService ([`mods/svc/host.h`](../sdk/include/mods/svc/host.h))

Mod metadata and runtime interaction with the loader.

**C++**

```cpp
IMPORT_SERVICE(HostService, svc_host);

// Temporary mod data directory, wiped on startup
const char* cacheDir = svc_host->mod_dir(mod_ctx);

// Persistent mod data directory
const char* dataDir = nullptr;
if (svc_host->data_dir(mod_ctx, &dataDir) == MOD_OK) {
    // ...
}

// Report an error and disable the mod
svc_host->fail(mod_ctx, MOD_ERROR, "something unrecoverable happened");
```

**Luau**

```lua
local host = require("dusklight.host")
local dataDir = host.data_dir()
host.on_update(function()
    -- Runs every frame
end)
host.on_shutdown(function()
    -- Runs on mod deactivation
end)
host.fail("something unrecoverable happened")
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

### HookService ([`mods/svc/hook.h`](../sdk/include/mods/svc/hook.h))

Installs hooks on game functions and resolves symbols by name. You'll rarely call it directly; use the typed helpers in
[`mods/svc/hook.hpp`](../sdk/include/mods/svc/hook.hpp) described in
[Hooking Game Functions](#hooking-game-functions).

### OverlayService ([`mods/svc/overlay.h`](../sdk/include/mods/svc/overlay.h))

Registers DVD file overlays at runtime: the dynamic counterpart to the static `overlay/` directory (see
[Asset Overlays](#asset-overlays)). Overlay a disc path with a file from your bundle, a file within an archive,
or with a caller-owned buffer (copied on registration).

**C++**

```cpp
IMPORT_SERVICE(OverlayService, svc_overlay);

OverlayHandle handle = 0;
svc_overlay->add_file(mod_ctx, "/Movie/demo_movie98_00.thp", "res/replacement.thp", &handle); // Replaces the demo movie
svc_overlay->add_file(mod_ctx, "/res/Object/Kmdl/archive/bmwr/al.bmd", "res/link_model.bmd", &handle); // Replaces link's model
svc_overlay->add_buffer(mod_ctx, "/generated.txt", data, size, nullptr);
svc_overlay->remove(mod_ctx, handle);
```

**Luau**

```lua
local overlay = require("dusklight.overlay")
local replacement = overlay.add_file("/Movie/demo_movie98_00.thp", "res/replacement.thp")
replacement:remove()
```

`disc_path` must be absolute (leading `/`) and is matched against the disc case-insensitively. Paths that don't exist
on the disc are added as new files. Changes are applied at the next frame boundary, and data the game already read
stays in memory until the file is re-read: sometimes a scene reload, and in the worst case, a full restart.

Dusklight reloads core archive files during scene transitions so modifications to Link, Midna or other globally-loaded
data get refreshed without a full restart.

See [Asset Overlays](#asset-overlays) for priority and conflict handling.

### TextureService ([`mods/svc/texture.h`](../sdk/include/mods/svc/texture.h))

Registers texture replacements at runtime: the dynamic counterpart to the static `textures/` directory (see
[Asset Overlays](#asset-overlays)). Two forms: raw texel data with an explicit key, or an encoded `.dds`/`.png` from
your bundle whose filename encodes the key.

**C++**

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

**Luau**

```lua
local texture = require("dusklight.texture")
local replacement = texture.register_file("res/tex1_32x32_$_6.png")
replacement:unregister()
```

Filenames use the same Dolphin-style convention as the user's `texture_replacements` directory:
`tex1_{w}x{h}_{texhash}[_{tluthash}]_{fmt}.dds|.png`, where hashes may be `$` (wildcard). `_mipN` sidecar files next to
a registered file are picked up automatically. Files are decoded lazily on first use by the renderer; raw data is copied
at registration. Registrations follow your mod's lifecycle.

See [Asset Overlays](#asset-overlays) for priority and conflict handling.

### ConfigService ([`mods/svc/config.h`](../sdk/include/mods/svc/config.h))

Persistent, mod-scoped configuration variables. Each var is stored in the user's `config.json` under
`mod.<escaped mod id>.<name>` (escaping: `.` → `_`, `_` → `__`, so `com.example.my_mod` becomes `com_example_my__mod`),
next to the host's own settings.

**C++**

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

**Luau**

```lua
local config = require("dusklight.config")
local speed = config.register({ name = "speedMultiplier", type = "float", default = 1.0 })
speed:subscribe(function(value, previous)
    -- React to the new value.
end)
speed:set(2.0)
```

Types: `CONFIG_VAR_BOOL` (`bool`), `CONFIG_VAR_INT` (`int64_t`), `CONFIG_VAR_FLOAT` (`double`), `CONFIG_VAR_STRING`
(UTF-8; `get_string` copies into a caller buffer, pass a `NULL` buffer with size 0 to query the length). Accessors are
typed and must match the registration.

Change callbacks fire on the game thread whenever the value changes at runtime (your own `set_*` calls included).
Writes that store the same value are silent. Values applied from `config.json` or `--cvar` at registration do
**not** fire callbacks; read the value after `register_var` for the starting state.

### SaveService ([`mods/svc/save.h`](../sdk/include/mods/svc/save.h))

Stores named binary blobs for each save slot. Blob names are scoped to the calling mod, and each mod may store up to
`SAVE_BLOB_BUDGET_BYTES` per slot. The service copies data passed to `set_blob`.

```cpp
IMPORT_SERVICE(SaveService, svc_save);

struct MySaveData {
    uint32_t version;
    uint32_t counter;
};

MySaveData state{1, 42};
svc_save->set_blob(mod_ctx, "state", &state, sizeof(state));

MySaveData loaded{};
size_t loadedSize = sizeof(loaded);
if (svc_save->get_blob(mod_ctx, "state", &loaded, &loadedSize) == MOD_OK &&
    loadedSize == sizeof(loaded)) {
    apply_state(loaded);
}
```

`set_blob`, `get_blob`, and `delete_blob` operate on the current slot, which is available after creating or loading a
save and unavailable at file select. Blob changes are written with the next game save. File-select copy and erase
operations update the blob data as well. Use `peek_blob` to read the calling mod's data from any slot; it uses the same
buffer contract as `get_blob`. Pass a `NULL` buffer to either read function to query the blob size.

`observe_saves` registers callbacks for new, loaded, and written saves. New-save callbacks run after the slot's blobs
are cleared. Observers are removed automatically when the mod is detached, so the output handle is only needed for
manual unregistration. Save callbacks run on the game thread.

### StageService ([`mods/svc/stage.h`](../sdk/include/mods/svc/stage.h))

Allows making changes to a stage's "stage info" (contents of .dzs/.dzr files).
(Currently only supports editing actor nodes.)

```cpp
IMPORT_SERVICE(StageService, svc_stage);

stage_actor_data_class record = {
    "carry00",
    0xFF000000,
    cXyz(0.0f, 0.0f, 0.0f),
    csXyz(0, 0, 0),
    0,
};

StageActorHandle handle{};
svc_stage->patch_actor(mod_ctx, "F_SP102", 0, -1, record_crc, &record, sizeof(record), &handle);
```

```
StageActorHandle handle{};
svc_stage->delete_actor(mod_ctx, "F_SP102", 0, -1, record_crc, &handle);
```

Patch or remove actors from the original actor list as the room loads.
Given records must be of either `stage_actor_data_class` or `stage_tgsc_data_class` types.
`record_crc` is the CRC-32 of the unmodified original record used to identify the record to replace or remove.

```
stage_actor_data_class record = {
    "carry00",
    0xFF000000,
    cXyz(0.0f, 0.0f, 0.0f),
    csXyz(0, 0, 0),
    0,
};

StageActorHandle handle{};
svc_stage->add_actor(mod_ctx, "F_SP102", 0, -1, &record, sizeof(record), &handle);
```

Add a new actor to the actor list as the room loads.
Given records must be of either `stage_actor_data_class` or `stage_tgsc_data_class` types.

Stage names may contain up to 8 characters. For patches and deletions, room `0xff` and layer `-1` match any room or
layer; additions require a specific room. Edits are removed when the mod is detached. If multiple mods edit the same
record, the later-loaded mod wins.

### UiService ([`mods/svc/ui.h`](../sdk/include/mods/svc/ui.h))

Integrate seamlessly with Dusklight's UI system: add controls and buttons to your mod's detail pane in the Mods window,
create custom windows and modal dialogs, apply custom RCSS stylesheets (anywhere!), and add menu bar tabs.

**Mod panel:** Registers or replaces the panel rendered in your mod's detail pane; `build` runs every time the detail
content is rebuilt, and `update` runs every frame while that mod is selected. While your mod is selected, the detail
pane carries your mod's id as a `mod-id` attribute (like custom window roots), so scoped RCSS can target it (e.g.
`[mod-id="com.example.mod"]`).

**C++**

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

**Luau**

```lua
local ui = require("dusklight.ui")
ui.register_mods_panel({
    build = function(panel)
        panel:add_section("Status")
        panel:add_text("running")
    end,
})
```

Element setters must match the element kind: `elem_set_text`/`elem_set_rml` on text rows, and `elem_set_progress` on
progress bars. `elem_set_class` sets or clears an RCSS class on any element handle, for styling via scoped or
per-window RCSS. A non-`MOD_OK` result from `build`/`update` fails your mod, as do exceptions thrown from any UI
callback.

**Controls:** `pane_add_control` adds an input row described by a `UiControlDesc`: `UI_CONTROL_BUTTON`,
`UI_CONTROL_GROUP`, `UI_CONTROL_TOGGLE`, `UI_CONTROL_NUMBER`, `UI_CONTROL_STRING`, `UI_CONTROL_SELECT`, or
`UI_CONTROL_COLOR`. Values bind with callbacks or directly to a config var.

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
type must match the control: `TOGGLE` = bool, `NUMBER` and `SELECT` = int, `STRING` and `COLOR` = string. Float vars
are not bindable; use callbacks and convert. `help_rml` and `SELECT` option lists render in a help pane, so `SELECT`
controls are only available inside window tabs.

`pane_add_group` adds a category button to a window tab's left pane. Focusing the button clears the paired right pane
and calls the group's build callback with that pane, which is useful for organizing related controls without adding
more tabs.

**Lists:** `pane_add_list` adds a scrollable virtualized list of items that can be efficiently updated and filtered.
Keys must be unique and remain stable across replacements.

```cpp
UiListHandle locationList = 0;

void replace_locations(std::string_view query) {
    std::vector<UiListItem> items;
    for (uint64_t i = 0; i < locations.size(); ++i) {
        if (matches(locations[i], query)) {
            UiListItem item = UI_LIST_ITEM_INIT;
            item.key = i;
            item.label = locations[i].c_str();
            items.push_back(item);
        }
    }
    svc_ui->list_set_items(mod_ctx, locationList, items.data(), items.size());
}

void set_filter(ModContext*, void*, const UiControlValue* value) {
    replace_locations(value->string_value);
}

bool location_selected(ModContext*, UiListHandle, uint64_t key, void*) {
    return selected_locations.contains(key);
}

UiControlDesc filter = UI_CONTROL_DESC_INIT;
filter.kind = UI_CONTROL_STRING;
filter.label = "Filter";
filter.get = get_filter;
filter.set = set_filter;
filter.string_set_mode = UI_STRING_SET_ON_CHANGE; /* invoke `set` while typing */
svc_ui->pane_add_control(mod_ctx, pane, &filter, nullptr);

UiListDesc list = UI_LIST_DESC_INIT;
/* items may be passed as a part of list creation, or set afterwards */
list.on_pressed = location_pressed;
list.is_selected = location_selected;
svc_ui->pane_add_list(mod_ctx, pane, &list, &locationList);
replace_locations(""); /* calls `list_set_items` */
```

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
default icon, and actions become buttons. The optional `build` callback allows you to add controls to a pane between
the body and actions. It uses the same text, progress, and control builders as panels.

```cpp
ModResult build_dialog(ModContext*, UiElementHandle pane, void*, ModError*) {
    UiControlDesc input = UI_CONTROL_DESC_INIT;
    input.kind = UI_CONTROL_STRING;
    input.label = "Name";
    input.get = get_name;
    input.set = set_name;
    return svc_ui->pane_add_control(mod_ctx, pane, &input, nullptr);
}

UiDialogAction action = UI_DIALOG_ACTION_INIT;
action.label = "Save";
action.on_pressed = save;
action.is_disabled = is_save_disabled;

UiDialogDesc dialog = UI_DIALOG_DESC_INIT;
dialog.title = "New Preset";
dialog.body_rml = "Choose a name for the preset.";
dialog.actions = &action;
dialog.action_count = 1;
dialog.build = build_dialog;
svc_ui->dialog_push(mod_ctx, &dialog, nullptr);
```

After an action's `on_pressed`, the dialog closes unless the action sets `keep_open`. It can then be closed later
(or immediately) with `dialog_close`. Cancel fires `on_dismiss` and always closes. `dialog_set_body` and
`dialog_set_icon` mutate a live dialog.

**Toasts:** `push_toast` enqueues a notification. Titles and bodies accept RML. The optional `type` is applied as an
RCSS class; `warning` uses the built-in warning appearance, and mods can define their own types. A duration of 0 uses
the default of 5 seconds.

Toasts have a `mod-id` attribute, so `UI_SCOPE_OVERLAY` styles can use selectors such as
`toast[mod-id="com.example.randomizer"].success`.

```cpp
UiToastDesc toast = UI_TOAST_DESC_INIT;
toast.type = "success";
toast.title_rml = "Randomizer";
toast.body_rml = "<span>Seed loaded successfully.</span>";
toast.duration_ms = 3000;
svc_ui->push_toast(mod_ctx, &toast);
```

**Menu bar tabs:** `register_menu_tab` adds a tab to the in-game menu bar. `on_selected` fires when the user activates
the tab: typically you'd push a window from it. The tab is removed by `unregister_menu_tab`, or automatically when the
mod is disabled.

**Custom styles:** `register_styles(scope, rcss, &handle)` applies an RCSS stylesheet to every document of a scope:
existing documents restyle immediately, and future ones pick it up when created. `register_styles_file(scope, path,
&handle)` reads the sheet from your bundle's `res/` directory. Scopes are `UI_SCOPE_PRELAUNCH`, `UI_SCOPE_WINDOW`,
`UI_SCOPE_MENU_BAR`, `UI_SCOPE_OVERLAY`, `UI_SCOPE_TOUCH_CONTROLS`, and `UI_SCOPE_GRAPHICS_TUNER`. Sheets apply after
host styles and may override them. Scope selectors tightly (use `[mod-id="..."]`!), especially for `UI_SCOPE_WINDOW`,
unless changing host UI is intentional.

### WindowService ([`mods/svc/window.h`](../sdk/include/mods/svc/window.h))

Allows creating new windows that can be rendered to via `GfxService`.

```cpp
IMPORT_SERVICE(WindowService, svc_window);

WindowDesc desc = WINDOW_DESC_INIT;
desc.title = "My auxiliary view";
desc.on_event = on_window_event;
WindowHandle window = 0;
svc_window->create_window(mod_ctx, &desc, &window);
```

Window callbacks run on the game thread. A close event is only a request; call `destroy_window` when the mod is ready to
close it. A window attached to a GfxService present target cannot be destroyed until that target is unregistered. Only
one present target may be attached to a WindowService window at a time.

New windows are hidden by default so a mod can finish attaching graphics before calling `show_window`.

### GfxService ([`mods/svc/gfx.h`](../sdk/include/mods/svc/gfx.h))

**Requires `add_mod(... FEATURES webgpu)`**

Direct WebGPU access at various stages of the rendering pipeline. Mods use the `wgpu*` C API (via
[`webgpu/webgpu.h`](https://github.com/webgpu-native/webgpu-headers/blob/main/webgpu.h)) for custom draws and compute
dispatches. Mods must manage their own WebGPU state, including pipelines and bind groups.

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

#### External presentation

GfxService supports external presentation ("present targets") backed by either a WindowService window (via
`register_window_present_target`) or a plain `WGPUSurface` (via `register_present_target`).

```cpp
GfxPresentTargetDesc target_desc = GFX_PRESENT_TARGET_DESC_INIT;
target_desc.render = render_auxiliary_view;
GfxPresentTargetHandle target = 0;
svc_gfx->register_window_present_target(mod_ctx, window, &target_desc, &target);

// From a stage callback:
svc_gfx->push_present(mod_ctx, target, &payload, sizeof(payload));
```

For WindowService windows, the surface is automatically reconfigured on window size changes.
For plain `WBPUSurface`s, `resize_present_target` must be used to resize.

To create a `WGPUSurface` manually, `GfxDeviceInfo` holds the `WGPUInstance` and `WGPUAdapter` which can be used with
`wgpuInstanceCreateSurface` and a chained `WGPUSurfaceSource*` struct.

`push_present` must be called every frame from a GfxService stage callback. If surface was lost, `push_present` returns
`MOD_ERROR`. Unregister and re-register the target before trying again.

### CameraService ([`mods/svc/camera.h`](../sdk/include/mods/svc/camera.h))

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

Camera operators allow overriding the main camera. When an operator callback returns true, its values replace the camera
state for the current frame. Register and unregister using `register_camera_operator` / `unregister_camera_operator`.

### GameModeService ([`mods/svc/game_mode.h`](../sdk/include/mods/svc/game_mode.h))

Allows a mod to register a game mode with callbacks for key gameplay and save lifecycle events. Registered game modes
appear in the prelaunch menu. Game modes may use a unique set of saves by configuring `save_name`; leave it empty to use
the vanilla `gczelda2` save.

```cpp
IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(HookService, svc_hook);
IMPORT_SERVICE(GameModeService, svc_game_mode);

DEFINE_HOOK(fopAcM_createItem, CreateItem);

#define MY_GAME_MODE_ID "game-mode-id"

static HookAction my_function_hook(ModContext* ctx, void* args, void*, void*) {
    // If we wish to have this hook only run while the gamemode is registered, we need to hook the function from the
    // gamemode's onActivatedFunction, and uninstall the hook during the onDeactivatedFunction. Example below.
    // Alternatively, check with `svc_game_mode->is_active(mod_ctx, MY_GAME_MODE_ID, &active) == MOD_OK && active`.
    return HOOK_CONTINUE;
}

ModResult on_game_mode_activated(void*, ModError* outError) {
    // Setup the gamemode, Add any hooks that are gamemode specific
    // Overlay any files that are gamemode specific
    ModResult result = mods::hook_add_pre<CreateItem>(svc_hook, my_function_hook);
    if (result != MOD_OK) {
        return mods::set_error(outError, result, "failed to install fopAcM_createItem hook");
    }
    return MOD_OK;
}

ModResult on_game_mode_deactivated(void*, ModError* outError) {
    // Uninstall any hooks that are gamemode specific
    // Remove any file overlays that are gamemode specific
    ModResult result = mods::hook_uninstall<CreateItem>();
    if (result != MOD_OK) {
        return mods::set_error(outError, result, "failed to uninstall fopAcM_createItem hook");
    }
    return MOD_OK;
}

ModResult on_save_loaded(void*, ModError*) {
    // This function will be invoked by the game as a save is loaded
    return MOD_OK;
}

const GameModeDesc gameModeDesc = {
    .struct_size = sizeof(GameModeDesc),
    .game_mode_id = MY_GAME_MODE_ID,
    .full_name = "My Game Mode",
    .save_name = "my-unique-save",
    .user_data = nullptr,
    .on_activated = on_game_mode_activated,
    .on_deactivated = on_game_mode_deactivated,
    .on_save_loaded = on_save_loaded,
};
svc_game_mode->register_game_mode(mod_ctx, &gameModeDesc);
```

A game mode can also open UI for per-save settings when creating a new file. The state begins as
`GAME_MODE_STATE_PENDING` and remains valid until the mod selects `PROCEED` or `RETURN`.

```cpp
IMPORT_SERVICE(GameModeService, svc_game_mode);
IMPORT_SERVICE(UiService, svc_ui);

ModResult on_new_save_select(void*, GameModeNewSaveState* state, ModError* outError) {
    static GameModeNewSaveState* newSaveState;
    static UiWindowHandle windowHandle;

    newSaveState = state;

    UiTabDesc tabs[1]{};

    tabs[0].struct_size = sizeof(UiTabDesc);
    tabs[0].title = "Play";
    tabs[0].build = [](ModContext* ctx, UiWindowHandle, UiElementHandle leftPane, UiElementHandle rightPane, void*, ModError*) {
        UiControlDesc desc = UI_CONTROL_DESC_INIT;
        desc.kind = UI_CONTROL_BUTTON;
        desc.label = "Play";
        desc.help_rml = "Play Button";
        desc.on_pressed = [](ModContext* ctx, void* userdata) {
            *newSaveState = GAME_MODE_STATE_PROCEED;
            svc_ui->window_close(ctx, *static_cast<UiWindowHandle*>(userdata));
        };
        desc.user_data = &windowHandle;
        svc_ui->pane_add_control(mod_ctx, leftPane, &desc, nullptr);
        return MOD_OK;
    };

    UiWindowDesc desc = UI_WINDOW_DESC_INIT;
    desc.tabs = tabs;
    desc.tab_count = 1;
    desc.on_closed = [](ModContext *, UiWindowHandle, void *userdata) {
        // If closing the window through backing out, return to file select
        if (*newSaveState == GAME_MODE_STATE_PENDING) {
            *newSaveState = GAME_MODE_STATE_RETURN;
        }
    };

    ModResult result = svc_ui->window_push(mod_ctx, &desc, &windowHandle);
    if (result != MOD_OK) {
        return mods::set_error(outError, result, "failed to open new-save settings");
    }
    return MOD_OK;
}

const GameModeDesc gameModeDesc = {
    .struct_size = sizeof(GameModeDesc),
    .game_mode_id = "my-game-mode-id",
    .full_name = "My Game Mode",
    .save_name = "my-unique-save",
    .on_new_save_select = on_new_save_select,
};
svc_game_mode->register_game_mode(mod_ctx, &gameModeDesc);

```

### ActorService (`mods/svc/actor.h`)

A service that manages registering and creating custom actors. These actors will be run by the game as if they are part of the engine. These actors can be created by the game either by its 16-bit actor name, or a 7-character long name that can
be loaded by a stage.

```cpp
#include "mods/svc/actor.h"
IMPORT_SERVICE(ActorService, svc_actor);

class myActor_c : public fopAc_ac_c {};

int myActor_Create(void* i_this) {
    // Ran several times, until it returns cPhs_COMPLEATE_e to allow for async loading
    return cPhs_COMPLEATE_e;
}

int myActor_Delete(void* i_this) {
    // Free resources here
    return 1;
}

int myActor_Execute(void* i_this) {
    // Ran once per game tick
    return 1;
}

int myActor_IsDelete(void* i_this) {
    // Returns 1 when the actor can be deleted
    return 1;
}

int myActor_Draw(void* i_this) {
    // Code to draw the actor
    return 1;
}

s16 actor_name; // The process name that can be used by the game to load the actor
ActorHandle actor_handle;
ActorProfileDesc profDesc = {
    .name = "AUnique", // The name used by the stage loader to load the actor with.
                       // It has a character limit of 7 and must be unique among active
                       // mod actors. Matching a game actor name overrides stage lookup.
    .priority_group = 7, // When, relative to other actors _Execute should run
                         // See: mods/svc/actor.h
    .process_size = sizeof(myActor_c),
    .draw_priority = fpcDwPi_OBJ_LBOX_e, // Defines when the actor should be drawn relative
                                         // to other actors (see f_pc_draw_priority.h)
    .status = fopAcStts_CULL_e  | fopAcStts_UNK_0x4000_e | fopAcStts_UNK_0x40000_e,
    .group = fopAc_ACTOR_e, // Can be fopAc_ACTOR_e, fopAc_PLAYER_e, fopAc_ENEMY_e, or fopAc_NPC_e
    .cull_type = fopAc_CULLBOX_CUSTOM_e,
    .create_function = myActor_Create,
    .delete_function = myActor_Delete,
    .execute_function = myActor_Execute,
    .is_delete_function = myActor_IsDelete,
    .draw_function = myActor_Draw,
};
svc_actor->register_actor(mod_ctx, &profDesc, &actor_name, &actor_handle);

// Spawn the actor at the player's position
fopAc_ac_c* plr = dComIfGp_getPlayer(0);
if (plr) {
    ActorSpawnParams spawnParams = {
        .parameters = 0,
        .argument = 0,
        .room_num = fopAcM_GetRoomNo(plr),
        .position = {plr->current.pos.x, plr->current.pos.y, plr->current.pos.z},
        .angle = {plr->current.angle.x, plr->current.angle.y, plr->current.angle.z},
        .scale = {1.0f, 1.0f, 1.0f}
    };
    ActorId created_actor_id;
    svc_actor->create_actor(mod_ctx, actor_name, &spawnParams, &created_actor_id);
}
```

See `mods/custom_actor_demo` for a more complete example.

---

## Hooking Game Functions

**Requires `add_mod(... FEATURES game)`**

Mods may hook the vast majority of game functions, including file-local static, private and virtual functions.
[`mods/svc/hook.hpp`](../sdk/include/mods/svc/hook.hpp) provides typed helpers over the hook service:

```cpp
#include "mods/svc/hook.hpp"

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
    daAlink_c* link = mods::arg<daAlink_c*>(args, 0);  // arg 0 is `this`
    if (link->shape_angle.y > 10000) {
        return HOOK_SKIP_ORIGINAL;
    }
    return HOOK_CONTINUE;
}

mods::hook::add_pre<LinkPosMove>(on_pos_move_pre);
```

### Post-hooks

Run after the original (or after a replace-hook, or after a cancelled original). `retval` points to the return value,
if any.

```cpp
void on_pos_move_post(ModContext*, void* args, void* retval, void* userdata) { ... }

mods::hook::add_post<LinkPosMove>(on_pos_move_post);
```

### Replace-hooks

Substitute the original entirely. Call through to it via the declaration's `g_orig` if needed:

```cpp
void on_execute_replace(ModContext*, void* args, void* retval, void*) {
    int result = LinkExecute::g_orig(mods::arg<daAlink_c*>(args, 0));
    if (retval != nullptr) {
        *static_cast<int*>(retval) = result;
    }
}

mods::hook::replace<LinkExecute>(on_execute_replace);
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

mods::hook::add_pre<HookshotHit>(on_hookshot_hit_pre);
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
T  value = mods::arg<T>(args, n);      // copy
T& ref   = mods::arg_ref<T>(args, n);  // read/write reference
```

```cpp
DEFINE_HOOK(fopAcM_createItem, CreateItem);

// fpc_ProcID fopAcM_createItem(..., int itemNo, ...): turn heart drops into green rupees
HookAction on_create_item_pre(ModContext*, void* args, void*, void*) {
    int& itemNo = mods::arg_ref<int>(args, 1);
    if (itemNo == dItemNo_HEART_e) {
        itemNo = dItemNo_GREEN_RUPEE_e;
    }
    return HOOK_CONTINUE;
}

mods::hook::add_pre<CreateItem>(on_create_item_pre);
```

For reference parameters (e.g. `const cXyz& pos`), `arg_ref<cXyz>` yields a direct reference.

---

## Asset Overlays

Files placed under `overlay/` in the `.dusk` archive override game files at the corresponding path, equivalent to
replacing files in the .iso. This requires no code: an archive with just `mod.json` and `overlay/` is a complete mod.
To replace a file within an `.arc` archive, replace the archive suffix with a directory and place the replacement at
its path within the archive.

- `overlay/Audiores/Stream/menu_select.ast` replaces the main title's audio stream.
- `overlay/res/Layout/main2D/main2d/timg/midona64.bti` replaces Midna's UI icon inside `main2D.arc`.

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
`mods::set_error(error, code, "message")`) and return the code, and the loader disables the mod and shows the
message to the user.

```cpp
MOD_EXPORT ModResult mod_initialize(ModError* error) {
    if (!load_my_data()) {
        return mods::set_error(error, MOD_ERROR, "failed to load data");
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
struct mods::ServiceTraits<MyModService> {
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
`mod_shutdown`. Do not write into `native_dir`; use `data_dir` for persistent storage or `mod_dir` for temporary
(session) storage. Native library namespaces are process-wide on some platforms, so two mods cannot safely assume that
incompatible libraries with the same filename will remain isolated.
