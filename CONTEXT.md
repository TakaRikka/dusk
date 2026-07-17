# Dusklight

Shared language for Dusklight — the Twilight Princess PC port and its mod platform.

## Language

### Mod platform

**Service**:
A versioned C ABI of function pointers that the host (or a mod) publishes and other mods import. Built-in examples include Camera, Game, and Gfx.
_Avoid_: API module, plugin interface

**ActionService**:
The built-in Service that exposes the host's Actions capabilities to mods: registration, Bindings, poll/callback observation, rebind capture, rebind sessions, and Binding labels. Mods enable the SDK `actions` feature to use it (including the SDL input types needed for Binding values). It observes input and manages Bindings; it does not perform game or mod side effects on the mod's behalf. Game thread only. A separate InputService for raw SDL3 device access is deferred and is not part of this stack.
_Avoid_: keybind API, action dispatcher, Action surface (prefer the Service name), host Actions API (for mod-facing use — prefer ActionService)

**Action**:
A named, rebindable input intent registered with the host. Mods register through ActionService; the host registers its own Actions directly. Its identity is the owning mod's id plus a local name chosen at registration; which control fires it is not part of identity. Host-registered Actions (not a `.dusk` mod) use the `host` config namespace (`actions.host.<name>`) and are the only Actions listed under Settings Input — Open Dusklight Menu (`ALWAYS` delivery; default Binding F1) and Turbo Speed (`GAMEPLAY` delivery, host-applied; default Binding Tab) are first-party examples. Host-registered Actions carry a concrete default Binding (not unbound); a persisted unbound Binding for a host Action is treated as no override and coerced to that default at register. Turbo Speed is observed only through its Action Binding (no parallel hardcoded Tab path). For Open Dusklight Menu, a keyboard Binding replaces keyboard open fallbacks and a gamepad Binding replaces gamepad open fallbacks; the other device class's fallbacks remain. Registration is host-shaped: display name, delivery policy, and a single default Binding; observation is poll and/or callback (no per-port dimension). Gameplay intents that need game behavior (first person, Call Midna, map, minimap) are owned by a first-party Actions mod (rebind via that mod's UI). That mod integrates by hooking game functions with replace-when-bound semantics (unbound → vanilla; bound → Action drives the feature and suppresses the competing vanilla control where applicable); game TUs keep pure vanilla input paths (no PC Action bind branches) and do not call into Actions. Mods are denied direct game-TU integration; host-owned contracts (e.g. touch) may still reach game TUs. Host dusk must not depend on the Actions mod's ids. Live registration ends when the owning mod is disabled, reloaded, or fails; persisted Bindings for that identity remain for the next register. Disabling an owning mod disables its Actions.
_Avoid_: keybind, hotkey, command (when meaning the rebindable intent), ActionBinds (removed host enum — historical name only, not the domain term), factory default (prefer default Binding)

**Delivery policy**:
Whether an Action is delivered only when the host has not captured input for UI (gameplay), or whenever its Binding fires (always) — including while menus or text fields hold focus.
_Avoid_: focus mode, input context, capture mode

**Binding**:
The player-assigned discrete control that currently fires an Action: a tagged value — keyboard (`SDL_Scancode`), gamepad button (`SDL_GamepadButton`), gamepad trigger axis (`SDL_GamepadAxis` LEFT/RIGHT trigger only), or unbound. Each Action has one Binding (not per-Port). Evaluation uses the primary player slot (`PAD_1`) for gamepad buttons and triggers; keyboard is global. Trigger Bindings digitize with that controller's left/right trigger activation zone from Controller Config (same thresholds as emulate-triggers). Bindings are mutable configuration owned and persisted by the host, not identity; mods read and write them through ActionService. Players change mod-owned Actions' Bindings through the owning mod's UI. Actions registered by the host process itself (not a `.dusk` mod) may also be rebound from the host Settings Input category. Multiple Actions may share a Binding; all of them fire (fan-out). Rebind UIs use a rebind session (built on rebind capture) to listen for the next key, gamepad button, or trigger pull; the caller may filter keyboard-only, gamepad-only (buttons and triggers), or either (the default). Escape during capture cancels without firing the capture callback or changing the Binding; unbound is assigned only by explicitly clearing (not by Escape). Reset restores the Action's registration default Binding; that default is readable for UI (choose Clear vs Reset, disable Reset when already at default). A rebind row pairs Rebind with exactly one secondary control — never both: Clear when the registration default is unbound, Reset when it is a concrete Binding.
_Avoid_: keybind (as the registered object), mapping (when Binding is meant), untagged bind int (host ActionBinds legacy), per-Port Binding, Escape-to-clear, Clear-and-Reset together

**Rebind session**:
A host-owned lifecycle for a rebind UI: begin capture for an Action, complete by applying the chosen Binding, or cancel with no Binding change (Escape or explicit cancel). The host uses it directly; mods create and drive sessions through ActionService. Lower-level rebind capture remains available beside sessions.
_Avoid_: capture session (capture is the lower-level primitive), ActionRebindSession (type name — prefer the domain term), rebind dialog
