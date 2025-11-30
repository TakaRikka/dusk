#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio/stb-data-parse.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "dolphin/os.h"

namespace JStudio {
namespace stb {
namespace data {

void TParse_TSequence::getData(TData* pData) const {
    ASSERT(pData != NULL);

    pData->content = NULL;
    pData->next = NULL;
    u32 head = get_head();
    u8 type = head >> 24;
    u32 param = head & 0xffffff;
    pData->type = type;
    pData->param = param;
    if (type != 0) {
        const void* next = (const void*)((int)getRaw() + 4);
        if (type <= 0x7f) {
            pData->next = next;
        } else {
            pData->content = next;
            pData->next = (const void*)((int)next + param);
        }
    }
}

void TParse_TParagraph::getData(TData* pData) const {
    ASSERT(pData != NULL);

    const void* data = getRaw();
    u32 result;
    const void* next = parseVariableUInt_16_32_following(data, &result, &pData->type, NULL);
    pData->param = result;
    if (result == 0) {
        pData->content = NULL;
        pData->next = next;
    } else {
        pData->content = next;
        pData->next = (const void*)((int)next + align_roundUp(result, 4));
    }
}

void TParse_TParagraph_data::getData(TParse_TParagraph_data::TData* pData) const {
    JUT_ASSERT(104, pData!=NULL);
    pData->entrySize = 0;
    pData->entryCount = 0;
    pData->content = NULL;
    pData->next = NULL;
    u8* temp = (u8*)getRaw();
    u8* filedata = temp;
    if (filedata == NULL)
        return;
    u8 r29 = *filedata;
    pData->status = r29 & ~0x8;
    if (!r29)
        return;
    filedata++;
    u32 r28 = 1;
    if (r29 & 8) {
        r28 = *filedata;
        filedata++;
    }
    pData->entryCount = r28;
    pData->content = filedata;

    u8 r27 = r29 & 7;
    if (r27 == 0)
        return;
    u32 dataSize = (gauDataSize_TEParagraph_data)[r27];
    pData->entrySize = dataSize;
    pData->next = filedata + (dataSize * r28);
}

}  // namespace data
}  // namespace stb
}  // namespace JStudio
