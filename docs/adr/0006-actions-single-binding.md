# ActionService uses one Binding per Action

ActionService today stores four Bindings per Action (one per PAD port), with Settings and `actions_mod` exposing a Port picker. Dusklight is single-player and uses the primary player slot (`PAD_1`) almost exclusively; other ports are debug-only, so per-port Action config is noise and a source of wrong-slot binds.

We collapse to **one Binding per Action**, evaluated against the primary player (gamepad buttons on `PAD_1`’s assigned controller; keyboard globally). Poll, callback, capture, and host helpers drop the `port` parameter; Settings Input and `actions_mod` show a single rebind row. Persistence moves to unsuffixed keys (`actions.<id>.<name>`, `actions.host.<name>`); existing `…_portN` keys are orphaned.

ActionService remains **1.0** — the surface is pre-production, so we rewrite the ABI in place rather than major-bumping. Vanilla controller-config multi-port is unchanged.
