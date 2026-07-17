# ActionService ships; InputService deferred

Mods need rebindable Actions through a versioned Service ABI. We ship **ActionService** (SDK feature `actions`) as a built-in Service over the host `dusk::actions` core. A separate **InputService** for SDL3-shaped device access was considered as a peer Service with its own major and SDK feature, but is **deferred** and is not part of this stack — neither ABI, host implementation, nor `FEATURES input` wiring ships here.

ActionService observes input and manages Bindings; it does not perform game or mod side effects on the mod's behalf. Game thread only. Mods enable `FEATURES actions` for the ActionService headers and the SDL3 types used by Binding values.
