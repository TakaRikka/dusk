#ifndef NW4HBM_UT_LIST_H
#define NW4HBM_UT_LIST_H

#include <revolution/types.h>
#include "../db/assert.h"


namespace nw4hbm {
    namespace ut {

        typedef struct Link {
            /* 0x00 */ void* prevObject;
            /* 0x04 */ void* nextObject;
        } Link;  // size = 0x08

        typedef struct List {
            /* 0x00 */ void* headObject;
            /* 0x04 */ void* tailObject;
            /* 0x08 */ u16 numObjects;
            /* 0x0A */ u16 offset;
        } List;  // size = 0x0C

        void List_Init(List* list, u16 offset);
        void List_Append(List* list, void* object);
        void List_Prepend(List* list, void* object);
        void List_Insert(List* list, void* target, void* object);
        void List_Remove(List* list, void* object);

        void* List_GetNext(const List* list, const void* object);
        void* List_GetPrev(const List* list, const void* object);
        void* List_GetNth(const List* list, u16 index);

        static void* List_GetFirst(const List* list) {
            return List_GetNext(list, NULL);
        }
        static void* List_GetLast(const List* list) {
            return List_GetPrev(list, NULL);
        }

        inline u16 List_GetSize(const List* list) {
            NW4R_ASSERT_CHECK_NULL(207, list);
            return list->numObjects;
        }

    }  // namespace ut
}  // namespace nw4hbm

#endif
