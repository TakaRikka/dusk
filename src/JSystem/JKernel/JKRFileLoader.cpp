#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRFileLoader.h"
#include "string.h"
#include "ctype.h"
#include "global.h"

JKRFileLoader* JKRFileLoader::sCurrentVolume;
JSUList<JKRFileLoader> JKRFileLoader::sVolumeList;

JKRFileLoader::JKRFileLoader(void)
    : mFileLoaderLink(this), mVolumeName(NULL), mVolumeType(0), mMountCount(0) {}

JKRFileLoader::~JKRFileLoader() {
    if (getCurrentVolume() == this) {
        setCurrentVolume(NULL);
    }
}

void JKRFileLoader::unmount(void) {
    s32 count = mMountCount;
    if (mMountCount != 0) {
        count--;
        mMountCount = count;
        if (count == 0) {
            delete this;
        }
    }
}

void* JKRFileLoader::getGlbResource(const char* name) {
    const char* name_reference[1];
    name_reference[0] = name;

    JKRFileLoader* fileLoader = findVolume(name_reference);
    void* resource;
    if (fileLoader == NULL) {
        resource = NULL;
    } else {
        resource = fileLoader->getResource(name_reference[0]);
    }

    return resource;
}

void* JKRFileLoader::getGlbResource(const char* name, JKRFileLoader* fileLoader) {
    void* resource = NULL;
    if (fileLoader) {
        return fileLoader->getResource(0, name);
    }

    JSUList<JKRFileLoader>& volumeList = getVolumeList();
    JSUListIterator<JKRFileLoader> iterator;
    for (iterator = volumeList.getFirst(); iterator != volumeList.getEnd(); ++iterator) {
        resource = iterator->getResource(0, name);
        if (resource)
            break;
    }
    return resource;
}

bool JKRFileLoader::removeResource(void* resource, JKRFileLoader* fileLoader) {
    if (fileLoader) {
        return fileLoader->removeResource(resource);
    }

    JSUList<JKRFileLoader>& volumeList = getVolumeList();
    JSUListIterator<JKRFileLoader> iterator;
    for (iterator = volumeList.getFirst(); iterator != volumeList.getEnd(); ++iterator) {
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

    JSUList<JKRFileLoader>& volumeList = getVolumeList();
    JSUListIterator<JKRFileLoader> iterator;
    for (iterator = volumeList.getFirst(); iterator != volumeList.getEnd(); ++iterator) {
        if (iterator->detachResource(resource)) {
            return true;
        }
    }

    return false;
}

JKRFileLoader* JKRFileLoader::findVolume(const char** volumeName) {
    if (*volumeName[0] != '/') {
        return getCurrentVolume();
    }

    char volumeNameBuffer[0x101];
    *volumeName = fetchVolumeName(volumeNameBuffer, ARRAY_SIZEU(volumeNameBuffer), *volumeName);

    JSUList<JKRFileLoader>& volumeList = getVolumeList();
    JSUListIterator<JKRFileLoader> iterator;
    for (iterator = volumeList.getFirst(); iterator != volumeList.getEnd(); ++iterator) {
        if (strcmp(volumeNameBuffer, iterator->mVolumeName) == 0) {
            return iterator.getObject();
        }
    }

    return NULL;
}


static char rootPath[2] = "/";

const char* JKRFileLoader::fetchVolumeName(char* buffer, s32 bufferSize, const char* path) {
    if (strcmp(path, "/") == 0) {
        strcpy(buffer, rootPath);
        return rootPath;
    }

    path++;
    while (*path != 0 && *path != '/') {
        if (1 < bufferSize) {
            int lower_char;
            int ch = (int)*path;
            if (ch == -1) {
                lower_char = -1;
            } else {
                lower_char = __lower_map[ch & 0xFF];
            }

            *buffer = lower_char;
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
