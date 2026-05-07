#include "JavaWrapperFuncs.hpp"

#include <SDL3/SDL_system.h>
#include <jni.h>

namespace dusk::android {

void takeUriPermissions(const std::string& uri) {
    JNIEnv* env = (JNIEnv*)SDL_GetAndroidJNIEnv();
    jobject activity = (jobject)SDL_GetAndroidActivity();
    jclass activityClass = env->GetObjectClass(activity);
    jmethodID midTakeUriPermissions = env->GetStaticMethodID(activityClass, "takeUriPermissions", "(Ljava/lang/String;)V");

    jstring juri = env->NewStringUTF(uri.c_str());
    env->CallStaticVoidMethod(activityClass, midTakeUriPermissions, juri);
    env->DeleteLocalRef(juri);
}

bool checkUriPermissions(const std::string& uri) {
    JNIEnv* env = (JNIEnv*)SDL_GetAndroidJNIEnv();
    jobject activity = (jobject)SDL_GetAndroidActivity();
    jclass activityClass = env->GetObjectClass(activity);
    jmethodID midCheckUriPermissions = env->GetStaticMethodID(activityClass, "checkUriPermissions", "(Ljava/lang/String;)Z");

    jstring juri = env->NewStringUTF(uri.c_str());
    auto result = env->CallStaticBooleanMethod(activityClass, midCheckUriPermissions, juri);
    env->DeleteLocalRef(juri);

    return result;
}

}