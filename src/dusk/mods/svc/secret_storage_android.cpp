#include "secret_storage_android.hpp"

#include <SDL3/SDL_system.h>
#include <jni.h>

#include <limits>
#include <string>
#include <vector>

namespace dusk::mods::svc {
namespace {

constexpr const char* kStoreClass = "dev.twilitrealm.dusk.DuskSecretStore";

bool clear_exception(JNIEnv* env) {
    if (env == nullptr || !env->ExceptionCheck()) {
        return false;
    }
    env->ExceptionClear();
    return true;
}

jclass load_dusk_class(JNIEnv* env, jobject activity, const char* name) {
    if (env == nullptr || activity == nullptr) {
        return nullptr;
    }
    jclass activityClass = env->GetObjectClass(activity);
    const bool activityClassFailed = clear_exception(env);
    if (activityClass == nullptr || activityClassFailed) {
        return nullptr;
    }
    const jmethodID getClassLoader =
        env->GetMethodID(activityClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
    const bool getClassLoaderFailed = clear_exception(env);
    env->DeleteLocalRef(activityClass);
    if (getClassLoader == nullptr || getClassLoaderFailed) {
        return nullptr;
    }
    jobject loader = env->CallObjectMethod(activity, getClassLoader);
    const bool loaderFailed = clear_exception(env);
    if (loader == nullptr || loaderFailed) {
        return nullptr;
    }
    jclass loaderClass = env->FindClass("java/lang/ClassLoader");
    const bool loaderClassFailed = clear_exception(env);
    if (loaderClass == nullptr || loaderClassFailed) {
        env->DeleteLocalRef(loader);
        return nullptr;
    }
    const jmethodID loadClass =
        env->GetMethodID(loaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    const bool loadClassFailed = clear_exception(env);
    env->DeleteLocalRef(loaderClass);
    if (loadClass == nullptr || loadClassFailed) {
        env->DeleteLocalRef(loader);
        return nullptr;
    }
    jstring className = env->NewStringUTF(name);
    const bool classNameFailed = clear_exception(env);
    if (className == nullptr || classNameFailed) {
        env->DeleteLocalRef(loader);
        return nullptr;
    }
    auto* result = static_cast<jclass>(env->CallObjectMethod(loader, loadClass, className));
    const bool resultFailed = clear_exception(env);
    env->DeleteLocalRef(className);
    env->DeleteLocalRef(loader);
    if (result == nullptr || resultFailed) {
        if (result != nullptr) {
            env->DeleteLocalRef(result);
        }
        return nullptr;
    }
    return result;
}

void wipe_java_bytes(JNIEnv* env, jbyteArray value) {
    if (env == nullptr || value == nullptr) {
        return;
    }
    const jsize size = env->GetArrayLength(value);
    const bool sizeFailed = clear_exception(env);
    if (sizeFailed || size <= 0) {
        return;
    }
    std::vector<jbyte> zeros(static_cast<size_t>(size), 0);
    env->SetByteArrayRegion(value, 0, size, zeros.data());
    static_cast<void>(clear_exception(env));
}

jbyteArray bytes(JNIEnv* env, const uint8_t* data, const size_t size) {
    if (env == nullptr || (data == nullptr && size != 0) ||
        size > static_cast<size_t>(std::numeric_limits<jsize>::max())) {
        return nullptr;
    }
    auto* result = env->NewByteArray(static_cast<jsize>(size));
    const bool allocationFailed = clear_exception(env);
    if (result == nullptr || allocationFailed) {
        return nullptr;
    }
    if (size != 0) {
        env->SetByteArrayRegion(result, 0, static_cast<jsize>(size),
            reinterpret_cast<const jbyte*>(data));
        const bool copyFailed = clear_exception(env);
        if (copyFailed) {
            wipe_java_bytes(env, result);
            env->DeleteLocalRef(result);
            return nullptr;
        }
    }
    return result;
}

SecretStorageResult result_code(const jint code) {
    return code >= SECRET_STORAGE_OK && code <= SECRET_STORAGE_IO_ERROR
        ? static_cast<SecretStorageResult>(code)
        : SECRET_STORAGE_UNAVAILABLE;
}

class AndroidSecretStorageBackend final : public SecretStorageBackend {
public:
    SecretStorageResult get(
        const std::string_view modId, const std::string_view key, std::vector<uint8_t>& value) override {
        value.clear();
        JNIEnv* env = static_cast<JNIEnv*>(SDL_GetAndroidJNIEnv());
        if (env == nullptr) {
            return SECRET_STORAGE_UNAVAILABLE;
        }
        const bool environmentFailed = clear_exception(env);
        if (environmentFailed) {
            return SECRET_STORAGE_UNAVAILABLE;
        }
        jobject activity = static_cast<jobject>(SDL_GetAndroidActivity());
        const bool activityFailed = clear_exception(env);
        if (activity == nullptr || activityFailed) {
            if (activity != nullptr) env->DeleteLocalRef(activity);
            return SECRET_STORAGE_UNAVAILABLE;
        }
        jclass store = load_dusk_class(env, activity, kStoreClass);
        if (store == nullptr) {
            env->DeleteLocalRef(activity);
            return SECRET_STORAGE_UNAVAILABLE;
        }
        const jmethodID method = env->GetStaticMethodID(store, "get",
            "(Landroid/content/Context;[B[B)Ldev/twilitrealm/dusk/DuskSecretStore$Result;");
        const bool methodFailed = clear_exception(env);
        jbyteArray mod = bytes(env, reinterpret_cast<const uint8_t*>(modId.data()), modId.size());
        jbyteArray keyBytes = bytes(env, reinterpret_cast<const uint8_t*>(key.data()), key.size());
        if (method == nullptr || methodFailed || mod == nullptr || keyBytes == nullptr) {
            if (mod != nullptr) env->DeleteLocalRef(mod);
            if (keyBytes != nullptr) env->DeleteLocalRef(keyBytes);
            env->DeleteLocalRef(store);
            env->DeleteLocalRef(activity);
            return SECRET_STORAGE_UNAVAILABLE;
        }
        jobject result = env->CallStaticObjectMethod(store, method, activity, mod, keyBytes);
        const bool callFailed = clear_exception(env);
        env->DeleteLocalRef(mod);
        env->DeleteLocalRef(keyBytes);
        env->DeleteLocalRef(store);
        env->DeleteLocalRef(activity);
        if (result == nullptr || callFailed) {
            if (result != nullptr) env->DeleteLocalRef(result);
            return SECRET_STORAGE_UNAVAILABLE;
        }
        jclass resultClass = env->GetObjectClass(result);
        const bool resultClassFailed = clear_exception(env);
        if (resultClass == nullptr || resultClassFailed) {
            env->DeleteLocalRef(result);
            return SECRET_STORAGE_UNAVAILABLE;
        }
        const jfieldID codeField = env->GetFieldID(resultClass, "code", "I");
        const bool codeFieldFailed = clear_exception(env);
        const jfieldID valueField = env->GetFieldID(resultClass, "value", "[B");
        const bool valueFieldFailed = clear_exception(env);
        if (codeField == nullptr || codeFieldFailed || valueField == nullptr || valueFieldFailed) {
            env->DeleteLocalRef(resultClass);
            env->DeleteLocalRef(result);
            return SECRET_STORAGE_UNAVAILABLE;
        }
        const jint rawCode = env->GetIntField(result, codeField);
        const bool codeFailed = clear_exception(env);
        if (codeFailed) {
            env->DeleteLocalRef(resultClass);
            env->DeleteLocalRef(result);
            return SECRET_STORAGE_UNAVAILABLE;
        }
        const SecretStorageResult code = result_code(rawCode);
        auto* payload = static_cast<jbyteArray>(env->GetObjectField(result, valueField));
        const bool payloadFailed = clear_exception(env);
        if (payloadFailed) {
            if (payload != nullptr) {
                wipe_java_bytes(env, payload);
                env->DeleteLocalRef(payload);
            }
            env->DeleteLocalRef(resultClass);
            env->DeleteLocalRef(result);
            return SECRET_STORAGE_UNAVAILABLE;
        }
        if (code == SECRET_STORAGE_OK && payload == nullptr) {
            env->DeleteLocalRef(resultClass);
            env->DeleteLocalRef(result);
            return SECRET_STORAGE_UNAVAILABLE;
        }
        if (code != SECRET_STORAGE_OK && payload != nullptr) {
            wipe_java_bytes(env, payload);
            env->DeleteLocalRef(payload);
            env->DeleteLocalRef(resultClass);
            env->DeleteLocalRef(result);
            return code;
        }
        if (code == SECRET_STORAGE_OK && payload != nullptr) {
            const jsize size = env->GetArrayLength(payload);
            const bool sizeFailed = clear_exception(env);
            if (sizeFailed || size < 0 || static_cast<size_t>(size) > SECRET_STORAGE_VALUE_MAX_SIZE) {
                wipe_java_bytes(env, payload);
                env->DeleteLocalRef(payload);
                env->DeleteLocalRef(resultClass);
                env->DeleteLocalRef(result);
                return SECRET_STORAGE_CORRUPT;
            }
            value.resize(static_cast<size_t>(size));
            if (size != 0) {
                env->GetByteArrayRegion(payload, 0, size, reinterpret_cast<jbyte*>(value.data()));
            }
            const bool copyFailed = clear_exception(env);
            if (copyFailed) {
                wipe_secret_bytes(value.data(), value.size());
                value.clear();
                wipe_java_bytes(env, payload);
                env->DeleteLocalRef(payload);
                env->DeleteLocalRef(resultClass);
                env->DeleteLocalRef(result);
                return SECRET_STORAGE_UNAVAILABLE;
            }
            wipe_java_bytes(env, payload);
            env->DeleteLocalRef(payload);
        }
        env->DeleteLocalRef(resultClass);
        env->DeleteLocalRef(result);
        return code;
    }

    SecretStorageResult put(
        const std::string_view modId, const std::string_view key, const uint8_t* data, const size_t size) override {
        return call_put(modId, key, data, size);
    }

    SecretStorageResult remove(const std::string_view modId, const std::string_view key) override {
        return call_remove(modId, key);
    }

private:
    SecretStorageResult call_put(const std::string_view modId, const std::string_view key,
        const uint8_t* value, const size_t size) {
        JNIEnv* env = static_cast<JNIEnv*>(SDL_GetAndroidJNIEnv());
        if (env == nullptr) {
            return SECRET_STORAGE_UNAVAILABLE;
        }
        const bool environmentFailed = clear_exception(env);
        if (environmentFailed) {
            return SECRET_STORAGE_UNAVAILABLE;
        }
        jobject activity = static_cast<jobject>(SDL_GetAndroidActivity());
        const bool activityFailed = clear_exception(env);
        if (activity == nullptr || activityFailed) {
            if (activity != nullptr) env->DeleteLocalRef(activity);
            return SECRET_STORAGE_UNAVAILABLE;
        }
        jclass store = load_dusk_class(env, activity, kStoreClass);
        jbyteArray mod = bytes(env, reinterpret_cast<const uint8_t*>(modId.data()), modId.size());
        jbyteArray keyBytes = bytes(env, reinterpret_cast<const uint8_t*>(key.data()), key.size());
        jbyteArray bytesValue = bytes(env, value, size);
        jmethodID method = nullptr;
        bool methodFailed = false;
        if (store != nullptr) {
            method = env->GetStaticMethodID(store, "put", "(Landroid/content/Context;[B[B[B)I");
            methodFailed = clear_exception(env);
        }
        if (store == nullptr || mod == nullptr || keyBytes == nullptr ||
            bytesValue == nullptr || method == nullptr || methodFailed)
        {
            if (bytesValue != nullptr) {
                wipe_java_bytes(env, bytesValue);
                env->DeleteLocalRef(bytesValue);
            }
            if (keyBytes != nullptr) env->DeleteLocalRef(keyBytes);
            if (mod != nullptr) env->DeleteLocalRef(mod);
            if (store != nullptr) env->DeleteLocalRef(store);
            env->DeleteLocalRef(activity);
            return SECRET_STORAGE_UNAVAILABLE;
        }
        const jint code = env->CallStaticIntMethod(store, method, activity, mod, keyBytes, bytesValue);
        const bool callFailed = clear_exception(env);
        wipe_java_bytes(env, bytesValue);
        if (bytesValue != nullptr) env->DeleteLocalRef(bytesValue);
        env->DeleteLocalRef(keyBytes);
        env->DeleteLocalRef(mod);
        env->DeleteLocalRef(store);
        env->DeleteLocalRef(activity);
        return callFailed ? SECRET_STORAGE_UNAVAILABLE : result_code(code);
    }

    SecretStorageResult call_remove(const std::string_view modId, const std::string_view key) {
        JNIEnv* env = static_cast<JNIEnv*>(SDL_GetAndroidJNIEnv());
        if (env == nullptr) {
            return SECRET_STORAGE_UNAVAILABLE;
        }
        const bool environmentFailed = clear_exception(env);
        if (environmentFailed) {
            return SECRET_STORAGE_UNAVAILABLE;
        }
        jobject activity = static_cast<jobject>(SDL_GetAndroidActivity());
        const bool activityFailed = clear_exception(env);
        if (activity == nullptr || activityFailed) {
            if (activity != nullptr) env->DeleteLocalRef(activity);
            return SECRET_STORAGE_UNAVAILABLE;
        }
        jclass store = load_dusk_class(env, activity, kStoreClass);
        jbyteArray mod = bytes(env, reinterpret_cast<const uint8_t*>(modId.data()), modId.size());
        jbyteArray keyBytes = bytes(env, reinterpret_cast<const uint8_t*>(key.data()), key.size());
        jmethodID method = nullptr;
        bool methodFailed = false;
        if (store != nullptr) {
            method = env->GetStaticMethodID(store, "remove", "(Landroid/content/Context;[B[B)I");
            methodFailed = clear_exception(env);
        }
        if (store == nullptr || mod == nullptr || keyBytes == nullptr || method == nullptr || methodFailed)
        {
            if (keyBytes != nullptr) env->DeleteLocalRef(keyBytes);
            if (mod != nullptr) env->DeleteLocalRef(mod);
            if (store != nullptr) env->DeleteLocalRef(store);
            env->DeleteLocalRef(activity);
            return SECRET_STORAGE_UNAVAILABLE;
        }
        const jint code = env->CallStaticIntMethod(store, method, activity, mod, keyBytes);
        const bool callFailed = clear_exception(env);
        env->DeleteLocalRef(keyBytes);
        env->DeleteLocalRef(mod);
        env->DeleteLocalRef(store);
        env->DeleteLocalRef(activity);
        return callFailed ? SECRET_STORAGE_UNAVAILABLE : result_code(code);
    }
};

}  // namespace

std::unique_ptr<SecretStorageBackend> make_android_secret_storage_backend() {
    return std::make_unique<AndroidSecretStorageBackend>();
}

}  // namespace dusk::mods::svc
