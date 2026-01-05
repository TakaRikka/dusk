#ifndef JASGADGET_H
#define JASGADGET_H

#include "JSystem/JUtility/JUTAssert.h"
#include <string>

/**
 * @ingroup jsystem-jaudio
 * 
 */
template<class T>
class JASGlobalInstance {
public:
    JASGlobalInstance(T* inst) {
        sInstance = inst;
    }

    JASGlobalInstance(bool setInstance) {
        if (setInstance) {
            JUT_ASSERT(186, sInstance == NULL);
            sInstance = (T*)this;
        }
    }

    ~JASGlobalInstance() {
        if (sInstance == (T*)this) {
            sInstance = NULL;
        }
    }

    static T* getInstance() { return sInstance; }

    static T* sInstance;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
template<class T>
class JASPtrTable {
public:
    JASPtrTable(T** param_0, u32 size) {
        mTable = param_0;
        mSize = size;
        memset(mTable, 0, size * 4);
    }
    T* get(u32 index) {
        if (index >= mSize) {
            return NULL;
        }
        return mTable[index];
    }
    T* get(u32 index) const {
        if (index >= mSize) {
            return NULL;
        }
        return mTable[index];
    }
    void set(u32 index, T* value) {
        JUT_ASSERT(229, index < mSize);
        mTable[index] = value;
    }

private:
    /* 0x00 */ T** mTable;
    /* 0x04 */ u32 mSize;
};

/**
 * @ingroup jsystem-jaudio
 * 
 */
template<class T, size_t N>
class JASPtrArray : public JASPtrTable<T> {
public:
    JASPtrArray() : JASPtrTable<T>(mArray, N) {}

private:
    /* 0x08 */ T* mArray[N];
};

#endif /* JASGADGET_H */
