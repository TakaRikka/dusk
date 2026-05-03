#include "JavaWrapperFuncs.hpp"

#include <SDL3/SDL_system.h>
#include <jni.h>

namespace dusk::android {

jclass activityClass = nullptr;

jmethodID midTakeUriPermissions = nullptr;
jmethodID midCheckUriPermissions = nullptr;

// TODO: this needs to be called again when the app gets minimized and re-opened
void setupMethods() {
    JNIEnv* env = (JNIEnv*)SDL_GetAndroidJNIEnv();
    jobject activity = (jobject)SDL_GetAndroidActivity();
    activityClass = env->GetObjectClass(activity);

    midTakeUriPermissions = env->GetStaticMethodID(activityClass, "takeUriPermissions", "(Ljava/lang/String;)V");
    midCheckUriPermissions = env->GetStaticMethodID(activityClass, "checkUriPermissions", "(Ljava/lang/String;)Z");
}

void takeUriPermissions(const std::string& uri) {
    JNIEnv* env = (JNIEnv*)SDL_GetAndroidJNIEnv();

    jstring juri = env->NewStringUTF(uri.c_str());
    env->CallStaticVoidMethod(activityClass, midTakeUriPermissions, juri);
    env->DeleteLocalRef(juri);
}

bool checkUriPermissions(const std::string& uri) {
    JNIEnv* env = (JNIEnv*)SDL_GetAndroidJNIEnv();

    jstring juri = env->NewStringUTF(uri.c_str());
    auto result = env->CallStaticBooleanMethod(activityClass, midCheckUriPermissions, juri);
    env->DeleteLocalRef(juri);

    return result;
}

}