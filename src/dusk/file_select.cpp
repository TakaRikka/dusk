#include "file_select.hpp"

#include <memory>
#include <string_view>

#include <SDL3/SDL_dialog.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_stdinc.h>

#if defined(__ANDROID__) || defined(ANDROID)
#include <SDL3/SDL_system.h>
#include <jni.h>
#endif

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#if defined(__APPLE__) && !TARGET_OS_IOS && !TARGET_OS_TV && !TARGET_OS_MACCATALYST
#define USE_MACOS_FOLDER_DIALOG 1
#else
#define USE_MACOS_FOLDER_DIALOG 0
#endif

#if defined(__APPLE__) && TARGET_OS_IOS && !TARGET_OS_MACCATALYST
#define USE_IOS_DIALOG 1
#include "ios/FileSelectDialog.h"
#else
#define USE_IOS_DIALOG 0
#endif

#if USE_MACOS_FOLDER_DIALOG
namespace dusk {
bool ShowMacOSFolderSelect(
    FileCallback callback, void* userdata, SDL_Window* window, const char* default_location);
}  // namespace dusk
#endif

namespace dusk {
namespace {

std::string fallback_display_name(std::string_view path) {
    if (path.empty()) {
        return {};
    }

    std::string pathString(path);
    while (pathString.size() > 1 && (pathString.back() == '/' || pathString.back() == '\\')) {
        pathString.pop_back();
    }

    const std::size_t slash = pathString.find_last_of("/\\");
    if (slash == std::string::npos || slash + 1 >= pathString.size()) {
        return pathString;
    }
    return pathString.substr(slash + 1);
}

#if defined(__ANDROID__) || defined(ANDROID)
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

std::string android_display_name(std::string_view path) {
    auto* env = static_cast<JNIEnv*>(SDL_GetAndroidJNIEnv());
    if (env == nullptr) {
        return {};
    }

    jobject activity = static_cast<jobject>(SDL_GetAndroidActivity());
    if (activity == nullptr || clear_pending_exception(env)) {
        if (activity != nullptr) {
            env->DeleteLocalRef(activity);
        }
        return {};
    }

    jclass activityClass = env->GetObjectClass(activity);
    if (activityClass == nullptr || clear_pending_exception(env)) {
        env->DeleteLocalRef(activity);
        return {};
    }

    jmethodID getDisplayName = env->GetMethodID(
        activityClass, "getDisplayNameForUri", "(Ljava/lang/String;)Ljava/lang/String;");
    env->DeleteLocalRef(activityClass);
    if (getDisplayName == nullptr || clear_pending_exception(env)) {
        env->DeleteLocalRef(activity);
        return {};
    }

    jstring uri = env->NewStringUTF(std::string(path).c_str());
    if (uri == nullptr || clear_pending_exception(env)) {
        env->DeleteLocalRef(activity);
        return {};
    }

    auto* displayName = static_cast<jstring>(env->CallObjectMethod(activity, getDisplayName, uri));
    env->DeleteLocalRef(uri);
    env->DeleteLocalRef(activity);
    if (displayName == nullptr || clear_pending_exception(env)) {
        return {};
    }

    std::string result = to_string(env, displayName);
    env->DeleteLocalRef(displayName);
    return result;
}

struct AndroidFolderDialogState {
    FileCallback callback;
    void* userdata;
    std::string path;
    std::string error;
};

void onAndroidFolderDialogFinished(void* userdata) {
    std::unique_ptr<AndroidFolderDialogState> state(
        static_cast<AndroidFolderDialogState*>(userdata));

    const char* path = state->path.empty() ? nullptr : state->path.c_str();
    const char* error = state->error.empty() ? nullptr : state->error.c_str();
    state->callback(state->userdata, path, error);
}

bool show_android_folder_select(AndroidFolderDialogState* state) {
    auto* env = static_cast<JNIEnv*>(SDL_GetAndroidJNIEnv());
    if (env == nullptr) {
        return false;
    }

    jobject activity = static_cast<jobject>(SDL_GetAndroidActivity());
    if (activity == nullptr || clear_pending_exception(env)) {
        if (activity != nullptr) {
            env->DeleteLocalRef(activity);
        }
        return false;
    }

    jclass activityClass = env->GetObjectClass(activity);
    if (activityClass == nullptr || clear_pending_exception(env)) {
        env->DeleteLocalRef(activity);
        return false;
    }

    jmethodID showFolderDialog =
        env->GetMethodID(activityClass, "showFolderDialog", "(J)Z");
    env->DeleteLocalRef(activityClass);
    if (showFolderDialog == nullptr || clear_pending_exception(env)) {
        env->DeleteLocalRef(activity);
        return false;
    }

    const jboolean shown = env->CallBooleanMethod(
        activity, showFolderDialog, reinterpret_cast<jlong>(state));
    env->DeleteLocalRef(activity);
    if (clear_pending_exception(env)) {
        return false;
    }

    return shown == JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_dev_twilitrealm_dusk_DuskActivity_nativeFolderDialogResult(
    JNIEnv* env, jclass, jlong userdata, jstring path, jstring error) {
    auto* state = reinterpret_cast<AndroidFolderDialogState*>(userdata);
    if (state == nullptr) {
        return;
    }

    state->path = to_string(env, path);
    state->error = to_string(env, error);

    if (!SDL_RunOnMainThread(&onAndroidFolderDialogFinished, state, false)) {
        onAndroidFolderDialogFinished(state);
    }
}
#endif

#if USE_IOS_DIALOG
struct IOSDialogCallbackState {
    FileCallback callback;
    void* userdata;
};

void onIOSDialogFinished(void* userdata, const char* path, const char* error) {
    std::unique_ptr<IOSDialogCallbackState> state(static_cast<IOSDialogCallbackState*>(userdata));

    if (error != nullptr) {
        state->callback(state->userdata, nullptr, error);
        return;
    }

    if (path == nullptr) {
        state->callback(state->userdata, nullptr, nullptr);
        return;
    }

    state->callback(state->userdata, path, nullptr);
}
#else
struct SDLDialogCallbackState {
    FileCallback callback;
    void* userdata;
};

void onSDLDialogFinished(void* userdata, const char* const* filelist, [[maybe_unused]] int filter) {
    std::unique_ptr<SDLDialogCallbackState> state(static_cast<SDLDialogCallbackState*>(userdata));

    if (filelist == nullptr) {
        state->callback(state->userdata, nullptr, SDL_GetError());
        return;
    }

    if (filelist[0] == nullptr) {
        state->callback(state->userdata, nullptr, nullptr);
        return;
    }

    state->callback(state->userdata, filelist[0], nullptr);
}
#endif

}  // namespace

void ShowFileSelect(FileCallback callback, void* userdata, SDL_Window* window,
    const SDL_DialogFileFilter* filters, int nfilters, const char* default_location,
    bool allow_many) {
    if (callback == nullptr) {
        return;
    }

#if USE_IOS_DIALOG
    auto state = std::make_unique<IOSDialogCallbackState>();
    state->callback = callback;
    state->userdata = userdata;

    Dusk_iOS_ShowFileSelect(&onIOSDialogFinished, state.release(), window, filters, nfilters,
        default_location, allow_many);
#else
    auto state = std::make_unique<SDLDialogCallbackState>();
    state->callback = callback;
    state->userdata = userdata;

    SDL_ShowOpenFileDialog(&onSDLDialogFinished, state.release(), window, filters, nfilters,
        default_location, allow_many);
#endif
}

void ShowFolderSelect(
    FileCallback callback, void* userdata, SDL_Window* window, const char* default_location) {
    if (callback == nullptr) {
        return;
    }

#if USE_IOS_DIALOG
    callback(userdata, nullptr, "Folder selection is not supported on this platform");
#elif USE_MACOS_FOLDER_DIALOG
    ShowMacOSFolderSelect(callback, userdata, window, default_location);
#elif defined(__ANDROID__) || defined(ANDROID)
    auto state = std::make_unique<AndroidFolderDialogState>();
    state->callback = callback;
    state->userdata = userdata;

    if (show_android_folder_select(state.get())) {
        state.release();
        return;
    }

    callback(userdata, nullptr, "Folder selection is not supported on this platform");
#else
    auto state = std::make_unique<SDLDialogCallbackState>();
    state->callback = callback;
    state->userdata = userdata;

    SDL_ShowOpenFolderDialog(
        &onSDLDialogFinished, state.release(), window, default_location, false);
#endif
}

std::string display_name_for_path(std::string_view path) {
#if defined(__ANDROID__) || defined(ANDROID)
    if (path.starts_with("content:") || path.starts_with("file:")) {
        std::string displayName = android_display_name(path);
        if (!displayName.empty()) {
            return displayName;
        }
    }
#endif
    return fallback_display_name(path);
}
}  // namespace dusk
