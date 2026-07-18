# Public game helpers

Headers in this directory provide port-specific types and utilities used by ordinary game
headers. Their corresponding implementations live in `src/helpers/`.

These helpers **must not** depend on internal `src/dusk/` declarations in their public interface.
Unlike the internal `dusk::` namespace, they are exposed to mods that use the `game` feature
and therefore must remain ABI stable within `GameService` major versions.

APIs _specifically_ for mod use do not belong here; instead they belong in mod services,
which are individually versioned, can provide backwards compatibility, and are designed to
keep track of per-mod runtime state.
