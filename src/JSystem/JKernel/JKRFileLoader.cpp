#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRFileLoader.h"

#define MSL_USE_INLINES 1  // needed to inline tolower call. not inlined elsewhere in the repo

#include <string>
#include <cctype>
#include "global.h"

JKRFileLoader* JKRFileLoader::sCurrentVolume;
JSUList<JKRFileLoader> JKRFileLoader::sVolumeList;

JKRFileLoader::JKRFileLoader(void)
    : mFileLoaderLink(this), mVolumeName(NULL), mVolumeType(0), mMountCount(0) {}

JKRFileLoader::~JKRFileLoader() {
    if (sCurrentVolume == this) {
        sCurrentVolume = NULL;
    }
}

void JKRFileLoader::unmount(void) {
    if (mMountCount != 0) {
        if (--mMountCount == 0) {
            delete this;
        }
    }
}

void* JKRFileLoader::getGlbResource(const char* name) {
    JKRFileLoader* fileLoader = findVolume(&name);
    void* resource;
    if (fileLoader == NULL) {
        return NULL;
    } else {
        resource = fileLoader->getResource(name);
        return resource;
    }
}

void* JKRFileLoader::getGlbResource(const char* name, JKRFileLoader* fileLoader) {
    void* resource = NULL;
    if (fileLoader) {
        resource = fileLoader->getResource(0, name);
    } else {
        for (JSUListIterator<JKRFileLoader> iterator = sVolumeList.getFirst(); iterator != sVolumeList.getEnd(); ++iterator) {
            resource = iterator->getResource(0, name);
            if (resource)
                break;
        }
    }
    return resource;
}

bool JKRFileLoader::removeResource(void* resource, JKRFileLoader* fileLoader) {
    if (fileLoader) {
        return fileLoader->removeResource(resource);
    }

    for (JSUListIterator<JKRFileLoader> iterator = sVolumeList.getFirst(); iterator != sVolumeList.getEnd(); ++iterator) {
        if (iterator->removeResource(resource)) {
            return true;
        }
    }

    return false;
}

bool JKRFileLoader::detachResource(void* resource, JKRFileLoader* fileLoader) {
    if (fileLoader) {
        return fileLoader->detachResource(resource);
    }

    for (JSUListIterator<JKRFileLoader> iterator = sVolumeList.getFirst(); iterator != sVolumeList.getEnd(); ++iterator) {
        if (iterator->detachResource(resource)) {
            return true;
        }
    }

    return false;
}

JKRFileLoader* JKRFileLoader::findVolume(const char** volumeName) {
    if (*volumeName[0] != '/') {
        return sCurrentVolume;
    }

    char volumeNameBuffer[0x101];
    *volumeName = fetchVolumeName(volumeNameBuffer, ARRAY_SIZEU(volumeNameBuffer), *volumeName);

    for (JSUListIterator<JKRFileLoader> iterator = sVolumeList.getFirst(); iterator != sVolumeList.getEnd(); ++iterator) {
        if (strcmp(volumeNameBuffer, iterator->mVolumeName) == 0) {
            return iterator.getObject();
        }
    }

    return NULL;
}

const char* JKRFileLoader::fetchVolumeName(char* buffer, s32 bufferSize, const char* path) {
    static char rootPath[2] = "/";

    if (strcmp(path, "/") == 0) {
        strcpy(buffer, rootPath);
        return rootPath;
    }

    path++;
    while (*path != 0 && *path != '/') {
        if (1 < bufferSize) {
            *buffer = tolower(*path);
            buffer++;
            bufferSize--;
        }
        path++;
    }

    *buffer = '\0';
    if (*path == '\0') {
        path = rootPath;
    }

    return path;
}
