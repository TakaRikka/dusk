#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DManage.h"
#include "JSystem/JSupport/JSUInputStream.h"
#include "string.h"

void* J2DDataManage::get(char const* name) {
    for (J2DataManageLink* link = mList; link != NULL; link = link->mNext) {
        if (strcmp(link->mName, name) == 0) {
            return link->mData;
        }
    }
    return NULL;
}

void* J2DDataManage::get(JSUInputStream* inputStream) {
    inputStream->skip(1);
    u8 nameLen = inputStream->readU8();

    if (nameLen == 0) {
        return NULL;
    } else {
        char nameBuffer[257];
        inputStream->read(&nameBuffer, nameLen);
        nameBuffer[nameLen] = 0;
        return this->get(nameBuffer);
    }
}

s8* J2DResReference::getResReference(u16 idx) const {
    if (mCount <= idx || idx == 0xFFFF) {
        return NULL;
    } else {
        return (((s8*)this) + mOffsets[idx]);
    }
}

char* J2DResReference::getName(u16 idx) const {
    static char p_name[257];

    s8* resRef = this->getResReference(idx);

    if (resRef == NULL) {
        p_name[0] = 0;
        return p_name;
    } else {
        s8 first = resRef[0];

        switch (first) {
        case 2:
        case 3:
            for (s32 i = 0; i < resRef[1]; i++) {
                p_name[i] = resRef[2 + i];
            }
            p_name[resRef[1]] = 0;
            break;
        case 4:
            s32 pos = resRef[1] + 1;
            for (; pos >= 2; pos--) {
                if (resRef[pos] == '\\' || resRef[pos] == '/') {
                    break;
                }
            }

            s32 i = 0;
            pos++;
            for (; pos < resRef[1] + 2; i++, pos++) {
                p_name[i] = resRef[pos];
            }
            p_name[i] = 0;

            break;
        default:
            p_name[0] = 0;
        }
        return p_name;
    }
}
