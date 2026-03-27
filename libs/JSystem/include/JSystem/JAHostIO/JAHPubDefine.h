#ifndef JAHPUBDEFINE_H
#define JAHPUBDEFINE_H

#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTAssert.h"

template <class T>
class JAHSingletonBase {
public:
    JAHSingletonBase() { sInstance = (T*)this; }
    virtual ~JAHSingletonBase() { sInstance = NULL; }

    static T* newInstance() {
        // clang-format off
        JUT_ASSERT(82, sInstance==NULL);
        // clang-format on
        if (!sInstance) {
            sInstance = JKR_NEW T();
        }
        // clang-format off
        JUT_ASSERT(85, sInstance!=NULL);
        // clang-format on
        return sInstance;
    }

    static T* getIns() {
        JUT_ASSERT(110, sInstance);
        return sInstance;
    }

    static T* sInstance;
};

#endif /* JAHPUBDEFINE_H */
