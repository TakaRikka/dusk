#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTNameTab.h"
#include "string.h"

JUTNameTab::JUTNameTab() {
    setResource(NULL);
}

JUTNameTab::JUTNameTab(const ResNTAB* pNameTable) {
    setResource(pNameTable);
}

void JUTNameTab::setResource(const ResNTAB* pNameTable) {
    mNameTable = pNameTable;

    if (pNameTable != NULL) {
        mNameNum = pNameTable->mEntryNum;
        mpStrData = (const char*)(pNameTable->mEntries + mNameNum);
    } else {
        mNameNum = 0;
        mpStrData = 0;
    }
}

s32 JUTNameTab::getIndex(const char* pName) const {
    JUT_ASSERT(101, mNameTable != NULL);

    const ResNTAB::Entry* pEntry = mNameTable->mEntries;
    u16 keyCode = calcKeyCode(pName);

    for (u16 i = 0; i < mNameNum; i++) {
        if (
            pEntry->mKeyCode == keyCode &&
            strcmp((mNameTable->mEntries[i].mOffs + ((const char*)mNameTable)), pName) == 0
        ) {
            return i;
        }
        pEntry++;
    }

    return -1;
}

const char* JUTNameTab::getName(u16 index) const {
    JUT_ASSERT(138, mNameTable != NULL);
    if (index < mNameNum)
        return ((const char*)mNameTable) + mNameTable->mEntries[index].mOffs;
    return NULL;
}

u16 JUTNameTab::calcKeyCode(const char* pName) const {
    u32 keyCode = 0;
    while (*pName)
        keyCode = (keyCode * 3) + *pName++;
    return keyCode;
}
