#include <revolution/mem/list.h>

// I've tried inlines but only a macro seems to work
#define GetLink(parent_list, obj) ((MEMLink*)(((u32)(obj))+(parent_list)->offs))

void MEMInitList(MEMList *pList, u16 offs) {
    pList->head = NULL;
    pList->tail = NULL;
    pList->num = 0;
    pList->offs = offs;
}

void MEMAppendListObject(MEMList *pList, void *pObj) {
    MEMLink* link;

    if (pList->head == NULL) {
        link = GetLink(pList, pObj);
        link->next = NULL;
        link->prev = NULL;
        pList->head = pObj;
        pList->tail = pObj;
        pList->num++;
    }
    else {
        link = GetLink(pList, pObj);
        link->prev = pList->tail;
        link->next = NULL;

        GetLink(pList, pList->tail)->next = pObj;
        pList->tail = pObj;
        pList->num++;
    }
}

void* MEMGetNextListObject(MEMList *pList, void *pObj) {
    if (pObj == NULL) {
        return pList->head;
    }

    return GetLink(pList, pObj)->next;
}
