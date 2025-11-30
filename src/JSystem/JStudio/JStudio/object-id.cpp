//
// object-id
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio/object-id.h"
#include "JSystem/JUtility/JUTAssert.h"

bool JStudio::object::TIDData::isEqual(JStudio::object::TIDData const& a,
                                       JStudio::object::TIDData const& b) {
    u32 aSize = a.getIDSize();
    u32 bSize = b.getIDSize();
    if (aSize != bSize) {
        return false;
    }

    const u8* aStr32 = a.getID();
    const u8* bStr32 = b.getID();
    if (aStr32 == bStr32) {
        // they point to the same id
        return true;
    }

    u32 u = aSize;
    for (; u >= 4; u -= 4, aStr32 += 4, bStr32 += 4) {
        if (*(u32*)aStr32 != *(u32*)bStr32) {
            return false;
        }
    }

    const u8* aStrBytes = aStr32;
    const u8* bStrBytes = bStr32;
    JUT_ASSERT(46, u<4);
    switch (u) {
    case 3:
        if (*aStrBytes != *bStrBytes) {
            return false;
        }
        aStrBytes++;
        bStrBytes++;
    case 2:
        if (*aStrBytes != *bStrBytes) {
            return false;
        }
        aStrBytes++;
        bStrBytes++;
    case 1:
        if (*aStrBytes != *bStrBytes) {
            return false;
        }
    }

    return true;
}
