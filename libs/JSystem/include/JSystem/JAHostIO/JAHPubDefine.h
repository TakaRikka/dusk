#ifndef JAHPUBDEFINE_H
#define JAHPUBDEFINE_H

#include "JSystem/JUtility/JUTAssert.h"

template<class T>
class JAHSingletonBase {
public:
    JAHSingletonBase() { sInstance = (T*)this; }
    virtual ~JAHSingletonBase() { sInstance = NULL; }

    static T* newInstance() {
        JUT_ASSERT(82, sInstance==NULL);
        if (!sInstance) {
            sInstance = new T();
        }
        JUT_ASSERT(85, sInstance!=NULL);
        return sInstance;
    }

    static T* getIns() {
        JUT_ASSERT(110, sInstance);
        return sInstance;
    }

    static T* sInstance;
};

#endif /* JAHPUBDEFINE_H */
