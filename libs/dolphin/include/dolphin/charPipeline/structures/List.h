#ifndef _CHARPIPELINE_STRUCTURES_LIST_H_
#define _CHARPIPELINE_STRUCTURES_LIST_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    Ptr Prev;
    Ptr Next;
} DSLink, *DSLinkPtr;

typedef struct {
    u32 Offset;
    Ptr Head;
    Ptr Tail;
} DSList, *DSListPtr;

void DSInitList(DSListPtr list, Ptr obj, DSLinkPtr link);
void DSInsertListObject(DSListPtr list, Ptr cursor, Ptr obj);
void DSRemoveListObject(DSListPtr list, Ptr obj);
void DSAttachList(DSListPtr baseList, DSListPtr attachList);
void* DSNextListObj(DSListPtr list, Ptr obj);

#ifdef __cplusplus
}
#endif

#endif
