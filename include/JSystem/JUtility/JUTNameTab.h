#ifndef JUTNAMETAB_H
#define JUTNAMETAB_H

#include <dolphin/types.h>

/**
* @ingroup jsystem-jutility
* 
*/
struct ResNTAB {
    u16 mEntryNum;
    u16 mPad0;
    struct Entry {
        u16 mKeyCode;
        u16 mOffs;
    } mEntries[1];

    inline const char* getName(u32 index) const {
        return ((const char*)mEntries) + mEntries[index].mOffs - 4;
    }
};

/**
* @ingroup jsystem-jutility
* 
*/
class JUTNameTab {
public:
    JUTNameTab();
    JUTNameTab(const ResNTAB* pNameTable);
    virtual ~JUTNameTab() {};

    void setResource(const ResNTAB* pNameTable);
    s32 getIndex(char const*) const;
    const char* getName(u16 index) const;
    u16 calcKeyCode(char const* pName) const;
    const ResNTAB* getResNameTable() const { return mNameTable; }

private:
    /* 0x4 */ const ResNTAB* mNameTable;
    /* 0x8 */ const char* mpStrData;
    /* 0xC */ u16 mNameNum;
};

#endif /* JUTNAMETAB_H */
