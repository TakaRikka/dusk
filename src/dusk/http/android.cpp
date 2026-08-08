#include "http.hpp"

#include <SDL3/SDL_system.h>
#include <jni.h>

#include <algorithm>
#include <limits>
#include <string_view>
#include <utility>

namespace dusk::http {
namespace {

constexpr int JavaErrorNone = 0;
constexpr int JavaErrorInvalidUrl = 1;
constexpr int JavaErrorUnsupportedScheme = 2;
constexpr int JavaErrorTimeout = 3;
constexpr int JavaErrorTooLarge = 4;

int timeout_ms(std::chrono::milliseconds timeout) {
    const auto count = std::max<std::chrono::milliseconds::rep>(1, timeout.count());
    return static_cast<int>(
        std::min<std::chrono::milliseconds::rep>(count, std::numeric_limits<int>::max()));
}

jlong max_body_bytes(size_t maxBodyBytes) {
    return static_cast<jlong>(std::min<size_t>(
        maxBodyBytes, static_cast<size_t>(std::numeric_limits<jlong>::max())));
}

bool clear_pending_exception(JNIEnv* env) {
    if (env == nullptr || !env->ExceptionCheck()) {
        return false;
    }
    env->ExceptionClear();
    return true;
}

std::string to_string(JNIEnv* env, jstring value) {
    if (env == nullptr || value == nullptr) {
        return {};
    }

    const char* utf8 = env->GetStringUTFChars(value, nullptr);
    if (utf8 == nullptr) {
        clear_pending_exception(env);
        return {};
    }

    std::string result(utf8);
    env->ReleaseStringUTFChars(value, utf8);
    return result;
}

jstring to_jstring(JNIEnv* env, std::string_view value) {
    if (env == nullptr) {
        return nullptr;
    }
    return env->NewStringUTF(std::string(value).c_str());
}

Error map_java_error(int error) {
    switch (error) {
    case JavaErrorNone:
        return Error::None;
    case JavaErrorInvalidUrl:
        return Error::InvalidUrl;
    case JavaErrorUnsupportedScheme:
        return Error::UnsupportedScheme;
    case JavaErrorTimeout:
        return Error::Timeout;
    case JavaErrorTooLarge:
        return Error::TooLarge;
    default:
        return Error::Network;
    }
}

jclass load_dusk_class(JNIEnv* env, jobject activity, const char* className) {
    jclass activityClass = env->GetObjectClass(activity);
    if (activityClass == nullptr || clear_pending_exception(env)) {
        return nullptr;
    }

    jmethodID getClassLoader =
        env->GetMethodID(activityClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
    env->DeleteLocalRef(activityClass);
    if (getClassLoader == nullptr || clear_pending_exception(env)) {
        return nullptr;
    }

    jobject classLoader = env->CallObjectMethod(activity, getClassLoader);
    if (classLoader == nullptr || clear_pending_exception(env)) {
        return nullptr;
    }

    jclass classLoaderClass = env->FindClass("java/lang/ClassLoader");
    if (classLoaderClass == nullptr || clear_pending_exception(env)) {
        env->DeleteLocalRef(classLoader);
        return nullptr;
    }

    jmethodID loadClass = env->GetMethodID(
        classLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    env->DeleteLocalRef(classLoaderClass);
    if (loadClass == nullptr || clear_pending_exception(env)) {
        env->DeleteLocalRef(classLoader);
        return nullptr;
    }

    jstring javaClassName = env->NewStringUTF(className);
    if (javaClassName == nullptr || clear_pending_exception(env)) {
        env->DeleteLocalRef(classLoader);
        return nullptr;
    }

    auto* loadedClass =
        static_cast<jclass>(env->CallObjectMethod(classLoader, loadClass, javaClassName));
    env->DeleteLocalRef(javaClassName);
    env->DeleteLocalRef(classLoader);
    if (loadedClass == nullptr || clear_pending_exception(env)) {
        return nullptr;
    }

    return loadedClass;
}

jobjectArray make_string_array(JNIEnv* env, const std::vector<Header>& headers, bool names) {
    if (headers.size() > static_cast<size_t>(std::numeric_limits<jsize>::max())) {
        return nullptr;
    }
    jclass stringClass = env->FindClass("java/lang/String");
    if (stringClass == nullptr || clear_pending_exception(env)) {
        return nullptr;
    }

    jobjectArray array =
        env->NewObjectArray(static_cast<jsize>(headers.size()), stringClass, nullptr);
    env->DeleteLocalRef(stringClass);
    if (array == nullptr || clear_pending_exception(env)) {
        return nullptr;
    }

    for (jsize i = 0; i < static_cast<jsize>(headers.size()); ++i) {
        const std::string& value = names ? headers[static_cast<size_t>(i)].name :
                                           headers[static_cast<size_t>(i)].value;
        jstring javaValue = to_jstring(env, value);
        if (javaValue == nullptr || clear_pending_exception(env)) {
            env->DeleteLocalRef(array);
            return nullptr;
        }
        env->SetObjectArrayElement(array, i, javaValue);
        env->DeleteLocalRef(javaValue);
        if (clear_pending_exception(env)) {
            env->DeleteLocalRef(array);
            return nullptr;
        }
    }

    return array;
}

 jbyteArray make_byte_array(JNIEnv* env, const std::string& body) {
    if (body.size() > static_cast<size_t>(std::numeric_limits<jsize>::max())) {
        return nullptr;
    }
    auto* result = env->NewByteArray(static_cast<jsize>(body.size()));
    if (result == nullptr || clear_pending_exception(env)) {
        return nullptr;
    }
    if (!body.empty()) {
        env->SetByteArrayRegion(result, 0, static_cast<jsize>(body.size()),
            reinterpret_cast<const jbyte*>(body.data()));
        if (clear_pending_exception(env)) {
            env->DeleteLocalRef(result);
            return nullptr;
        }
    }
    return result;
}

std::vector<Header> read_headers(JNIEnv* env, jobjectArray names, jobjectArray values) {
    std::vector<Header> headers;
    if (names == nullptr || values == nullptr) {
        return headers;
    }

    const jsize count = std::min(env->GetArrayLength(names), env->GetArrayLength(values));
    headers.reserve(static_cast<size_t>(count));
    for (jsize i = 0; i < count; ++i) {
        auto* name = static_cast<jstring>(env->GetObjectArrayElement(names, i));
        auto* value = static_cast<jstring>(env->GetObjectArrayElement(values, i));
        if (clear_pending_exception(env)) {
            if (name != nullptr) {
                env->DeleteLocalRef(name);
            }
            if (value != nullptr) {
                env->DeleteLocalRef(value);
            }
            headers.clear();
            return headers;
        }

        if (name != nullptr) {
            headers.push_back({
                .name = to_string(env, name),
                .value = to_string(env, value),
            });
        }

        if (name != nullptr) {
            env->DeleteLocalRef(name);
        }
        if (value != nullptr) {
            env->DeleteLocalRef(value);
        }
    }

    return headers;
}

std::string read_body(JNIEnv* env, jbyteArray body) {
    if (body == nullptr) {
        return {};
    }

    const jsize bodySize = env->GetArrayLength(body);
    std::string result(static_cast<size_t>(bodySize), '\0');
    if (bodySize > 0) {
        env->GetByteArrayRegion(body, 0, bodySize, reinterpret_cast<jbyte*>(result.data()));
        if (clear_pending_exception(env)) {
            return {};
        }
    }
    return result;
}

Result result_from_response(JNIEnv* env, jobject response) {
    if (response == nullptr) {
        return {
            .error = Error::Network,
            .message = "Android HTTP request did not return a response",
        };
    }

    jclass responseClass = env->GetObjectClass(response);
    if (responseClass == nullptr || clear_pending_exception(env)) {
        return {
            .error = Error::Network,
            .message = "Failed to inspect Android HTTP response",
        };
    }

    jfieldID errorField = env->GetFieldID(responseClass, "error", "I");
    jfieldID messageField = env->GetFieldID(responseClass, "message", "Ljava/lang/String;");
    jfieldID statusField = env->GetFieldID(responseClass, "statusCode", "I");
    jfieldID headerNamesField =
        env->GetFieldID(responseClass, "headerNames", "[Ljava/lang/String;");
    jfieldID headerValuesField =
        env->GetFieldID(responseClass, "headerValues", "[Ljava/lang/String;");
    jfieldID bodyField = env->GetFieldID(responseClass, "body", "[B");
    env->DeleteLocalRef(responseClass);
    if (errorField == nullptr || messageField == nullptr || statusField == nullptr ||
        headerNamesField == nullptr || headerValuesField == nullptr || bodyField == nullptr ||
        clear_pending_exception(env))
    {
        return {
            .error = Error::Network,
            .message = "Android HTTP response shape was not recognized",
        };
    }

    const int javaError = env->GetIntField(response, errorField);
    auto* message = static_cast<jstring>(env->GetObjectField(response, messageField));
    auto* headerNames = static_cast<jobjectArray>(env->GetObjectField(response, headerNamesField));
    auto* headerValues =
        static_cast<jobjectArray>(env->GetObjectField(response, headerValuesField));
    auto* body = static_cast<jbyteArray>(env->GetObjectField(response, bodyField));
    if (clear_pending_exception(env)) {
        return {
            .error = Error::Network,
            .message = "Failed to read Android HTTP response",
        };
    }

    Response httpResponse{
        .statusCode = static_cast<int>(env->GetIntField(response, statusField)),
        .headers = read_headers(env, headerNames, headerValues),
        .body = read_body(env, body),
    };

    std::string messageString = to_string(env, message);

    if (message != nullptr) {
        env->DeleteLocalRef(message);
    }
    if (headerNames != nullptr) {
        env->DeleteLocalRef(headerNames);
    }
    if (headerValues != nullptr) {
        env->DeleteLocalRef(headerValues);
    }
    if (body != nullptr) {
        env->DeleteLocalRef(body);
    }

    return {
        .error = map_java_error(javaError),
        .message = std::move(messageString),
        .response = std::move(httpResponse),
    };
}

}  // namespace

bool available() noexcept {
    return true;
}

Backend backend() noexcept {
    return Backend::Android;
}

const char* backend_name() noexcept {
    return "Android";
}

Result request(const Request& request) {
    if (request.url.empty()) {
        return {
            .error = Error::InvalidUrl,
            .message = "URL is empty",
        };
    }
    if (!detail::valid_https_url(request.url))
    {
        return {
            .error = Error::UnsupportedScheme,
            .message = "Only https:// URLs are supported",
        };
    }

    auto* env = static_cast<JNIEnv*>(SDL_GetAndroidJNIEnv());
    if (env == nullptr) {
        return {
            .error = Error::Network,
            .message = "Failed to access Android JNI environment",
        };
    }

    jobject activity = static_cast<jobject>(SDL_GetAndroidActivity());
    if (activity == nullptr || clear_pending_exception(env)) {
        if (activity != nullptr) {
            env->DeleteLocalRef(activity);
        }
        return {
            .error = Error::Network,
            .message = "Failed to access Android activity",
        };
    }

    jclass clientClass =
        load_dusk_class(env, activity, "dev.twilitrealm.dusk.DuskHttpClient");
    env->DeleteLocalRef(activity);
    if (clientClass == nullptr) {
        return {
            .error = Error::Network,
            .message = "Failed to load Android HTTP helper",
        };
    }

    jmethodID requestMethod = env->GetStaticMethodID(clientClass, "request",
        "(ILjava/lang/String;[Ljava/lang/String;[Ljava/lang/String;[BIJ)"
        "Ldev/twilitrealm/dusk/DuskHttpClient$Response;");
    if (requestMethod == nullptr || clear_pending_exception(env)) {
        env->DeleteLocalRef(clientClass);
        return {
            .error = Error::Network,
            .message = "Failed to find Android HTTP helper method",
        };
    }

    jstring url = to_jstring(env, request.url);
    jobjectArray headerNames = make_string_array(env, request.headers, true);
    jobjectArray headerValues = make_string_array(env, request.headers, false);
    jbyteArray body = make_byte_array(env, request.body);
    if (url == nullptr || headerNames == nullptr || headerValues == nullptr || body == nullptr ||
        clear_pending_exception(env))
    {
        if (url != nullptr) {
            env->DeleteLocalRef(url);
        }
        if (headerNames != nullptr) {
            env->DeleteLocalRef(headerNames);
        }
        if (headerValues != nullptr) {
            env->DeleteLocalRef(headerValues);
        }
        if (body != nullptr) {
            env->DeleteLocalRef(body);
        }
        env->DeleteLocalRef(clientClass);
        return {
            .error = Error::Network,
            .message = "Failed to prepare Android HTTP request",
        };
    }

    jobject response = env->CallStaticObjectMethod(clientClass, requestMethod,
        request.method == Method::Post ? 1 : 0, url, headerNames, headerValues, body,
        timeout_ms(request.timeout), max_body_bytes(request.maxBodyBytes));
    env->DeleteLocalRef(url);
    env->DeleteLocalRef(headerNames);
    env->DeleteLocalRef(headerValues);
    env->DeleteLocalRef(body);
    env->DeleteLocalRef(clientClass);
    if (clear_pending_exception(env)) {
        return {
            .error = Error::Network,
            .message = "Android HTTP request failed with a Java exception",
        };
    }

    Result result = result_from_response(env, response);
    if (response != nullptr) {
        env->DeleteLocalRef(response);
    }
    return result;
}

Result get(const Request& source) {
    Request request = source;
    request.method = Method::Get;
    request.body.clear();
    return dusk::http::request(request);
}

}  // namespace dusk::http
