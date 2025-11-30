#ifndef J2DMANAGE_H
#define J2DMANAGE_H

#include "dolphin/types.h"

class JSUInputStream;

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DResReference {
    /* 0x00 */ u16 mCount;
    /* 0x02 */ u16 mOffsets[1];

    s8* getResReference(u16) const;
    char* getName(u16) const;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DataManageLink {
    /* 0x00 */ void* mData;
    /* 0x04 */ char* mName;
    /* 0x08 */ J2DataManageLink* mNext;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DDataManage {
private:
    /* 0x00 */ J2DataManageLink* mList;

public:
    void* get(char const*);
    void* get(JSUInputStream*);
};

#endif /* J2DMANAGE_H */
