#include "dusk/android_frame_rate.hpp"

#if defined(TARGET_ANDROID) || defined(__ANDROID__) || defined(ANDROID)
#include "dusk/settings.h"

#include <SDL3/SDL_system.h>
#include <jni.h>

namespace dusk::android {
namespace {

float preferred_surface_frame_rate() {
    switch (getSettings().game.enableFrameInterpolation.getValue()) {
    case FrameInterpMode::Off:
        return 30.0f;
    case FrameInterpMode::Unlimited:
    default:
        return 0.0f;
    case FrameInterpMode::Capped:
        return static_cast<float>(getSettings().video.maxFrameRate.getValue());
    }
}

bool clear_pending_exception(JNIEnv* env) {
    if (env == nullptr || !env->ExceptionCheck()) {
        return false;
    }
    env->ExceptionClear();
    return true;
}

}  // namespace

void update_surface_frame_rate() {
    auto* env = static_cast<JNIEnv*>(SDL_GetAndroidJNIEnv());
    if (env == nullptr) {
        return;
    }

    jobject activity = static_cast<jobject>(SDL_GetAndroidActivity());
    if (activity == nullptr || clear_pending_exception(env)) {
        if (activity != nullptr) {
            env->DeleteLocalRef(activity);
        }
        return;
    }

    jclass activityClass = env->GetObjectClass(activity);
    if (activityClass == nullptr || clear_pending_exception(env)) {
        env->DeleteLocalRef(activity);
        return;
    }

    jmethodID setPreferredFrameRate =
        env->GetMethodID(activityClass, "setPreferredSurfaceFrameRate", "(F)V");
    env->DeleteLocalRef(activityClass);
    if (setPreferredFrameRate == nullptr || clear_pending_exception(env)) {
        env->DeleteLocalRef(activity);
        return;
    }

    jvalue args[1]{};
    args[0].f = preferred_surface_frame_rate();
    env->CallVoidMethodA(activity, setPreferredFrameRate, args);
    env->DeleteLocalRef(activity);
    clear_pending_exception(env);
}

}  // namespace dusk::android
#else
namespace dusk::android {
void update_surface_frame_rate() {}
}  // namespace dusk::android
#endif
