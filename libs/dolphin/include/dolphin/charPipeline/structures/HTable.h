#ifndef _CHARPIPELINE_STRUCTURES_HTABLE_H_
#define _CHARPIPELINE_STRUCTURES_HTABLE_H_

#include <charPipeline/structures/List.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef u16 (DSHashFunc)(Ptr);

typedef struct {
    DSList* table;
    u16 tableSize;
    DSHashFunc* hash;
} DSHashTable;

void DSInitHTable(DSHashTable* hTable, u16 size, DSList* listArray, DSHashFunc* hashFunc, Ptr obj, DSLinkPtr link);
void DSInsertHTableObj(DSHashTable* hTable, Ptr obj);
void DSHTableToList(DSHashTable* hTable, DSList* list);
void* DSNextHTableObj(DSHashTable* hTable, Ptr obj);
s32 DSHTableIndex(DSHashTable* hTable, Ptr obj);
void* DSHTableHead(DSHashTable* hTable, s32 index);

#ifdef __cplusplus
}
#endif

#endif
