#!/usr/bin/env python3
"""Offline source audit for Android-only storage and JNI fail-closed boundaries."""

import pathlib


def main() -> int:
    root = pathlib.Path(__file__).parents[2]
    source = (root / "platforms/android/app/src/main/java/com/twilitrealm/dusk/DuskSecretStore.java").read_text(
            encoding="utf-8")
    required = (
        "new AtomicFile(base)",
        "atomic.openRead()",
        "requireRegularOrAbsent(backup)",
        "if (!base.exists() && !backup.exists())",
        "store.deleteEntry(alias)",
        "store.containsAlias(alias)",
    )
    assert all(marker in source for marker in required)
    native = (root / "src/dusk/mods/svc/secret_storage_android.cpp").read_text(encoding="utf-8")
    assert "== nullptr || clear_exception(env)" not in native
    assert "!= nullptr && clear_exception(env)" not in native
    assert "const bool allocationFailed = clear_exception(env);" in native
    assert "const bool callFailed = clear_exception(env);" in native
    assert "wipe_java_bytes(env, payload);" in native
    assert """if (copyFailed) {
            wipe_java_bytes(env, result);
            env->DeleteLocalRef(result);""" in native
    assert """if (bytesValue != nullptr) {
                wipe_java_bytes(env, bytesValue);
                env->DeleteLocalRef(bytesValue);""" in native
    assert """if (payloadFailed) {
            if (payload != nullptr) {
                wipe_java_bytes(env, payload);
                env->DeleteLocalRef(payload);
            }
            env->DeleteLocalRef(resultClass);
            env->DeleteLocalRef(result);
            return SECRET_STORAGE_UNAVAILABLE;""" in native
    assert """if (code != SECRET_STORAGE_OK && payload != nullptr) {
            wipe_java_bytes(env, payload);
            env->DeleteLocalRef(payload);
            env->DeleteLocalRef(resultClass);
            env->DeleteLocalRef(result);
            return code;""" in native
    assert '"put", "(Landroid/content/Context;[B[B[B)I"' in native
    assert "activity, mod, keyBytes, bytesValue" in native
    assert '"remove", "(Landroid/content/Context;[B[B)I"' in native
    assert "activity, mod, keyBytes);" in native
    assert not list((root / "platforms/android/app/src/debug").rglob("*Bootstrap*"))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
