#include <charPipeline/structures/List.h>

void DSInitList(DSListPtr list, Ptr obj, DSLinkPtr link) {
    list->Head = NULL;
    list->Tail = NULL;
    list->Offset = (Ptr)link - obj;
}

void DSInsertListObject(DSListPtr list, Ptr cursor, Ptr obj) {
    DSLinkPtr link;
    DSLinkPtr linkNext;
    DSLinkPtr linkPrev;

    link = (DSLinkPtr)(obj + list->Offset);
    if (list->Head) {
        if (!cursor) {
            linkPrev = (DSLinkPtr)(list->Tail + list->Offset);
            linkPrev->Next = obj;
            link->Prev = list->Tail;
            link->Next = NULL;
            list->Tail = obj;
        } else {
            linkNext = (DSLinkPtr)(cursor + list->Offset);
            if (cursor == list->Head) {
                list->Head = obj;
                link->Next = cursor;
                linkNext->Prev = obj;
            } else {
                linkPrev = (DSLinkPtr)(linkNext->Prev + list->Offset);
                link->Next = cursor;
                link->Prev = linkNext->Prev;
                linkNext->Prev = obj;
                linkPrev->Next = obj;
            }
        }
    } else {
        list->Tail = obj;
        list->Head = obj;
        link->Next = link->Prev = NULL;
    }
}

void DSRemoveListObject(DSListPtr list, Ptr obj) {
    DSLinkPtr link = (DSLinkPtr)(obj + list->Offset);

    if (obj) {
        if (link->Prev) {
            ((DSLinkPtr)(link->Prev + list->Offset))->Next = link->Next;
        } else {
            list->Head = link->Next;
        }

        if (link->Next) {
            ((DSLinkPtr)(link->Next + list->Offset))->Prev = link->Prev;
        } else {
            list->Tail = link->Prev;
        }

        link->Prev = NULL;
        link->Next = NULL;
    }
}

void DSAttachList(DSListPtr baseList, DSListPtr attachList) {
    DSLinkPtr link;
    DSLinkPtr linkPrev;

    if (baseList->Offset == attachList->Offset && (attachList->Head || attachList->Tail)) {
        linkPrev = (DSLinkPtr)(attachList->Head + attachList->Offset);
        if (baseList->Head) {
            link = (DSLinkPtr)(baseList->Tail + baseList->Offset);
            link->Next = attachList->Head;
            linkPrev->Prev = baseList->Tail;
            baseList->Tail = attachList->Tail;
            return;
        }
        baseList->Head = attachList->Head;
        baseList->Tail = attachList->Tail;
        linkPrev;  // needed to match
    }
}

void* DSNextListObj(DSListPtr list, Ptr obj) {
    if (!list) {
        return NULL;
    }
    if (!obj) {
        return list->Head;
    }

    return ((DSLinkPtr)(obj + list->Offset))->Next;
}
