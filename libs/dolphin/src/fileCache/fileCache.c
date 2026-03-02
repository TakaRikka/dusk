#include <dolphin/os.h>
#include <charPipeline/fileCache.h>

DSCache DODisplayCache;
u8 DOCacheInitialized;

static u8 AllocCacheNode(DSCacheNodePtr* cacheNode, char* name);
static void FreeCacheNode(DSCacheNodePtr* cacheNode);

DSCacheNodePtr DSAddCacheNode(DSCachePtr cache, char* name, Ptr data, Ptr OSFreeFunc) {
    DSCacheNodePtr cacheNode;

    cacheNode = NULL;
    if (!AllocCacheNode(&cacheNode, name)) {
        return NULL;
    }
    Strcpy(cacheNode->Name, name);
    cacheNode->Data = data;
    cacheNode->Free = (void (*)(Ptr *))OSFreeFunc;
    cacheNode->ReferenceCount = 0;
    DSInsertListObject(&cache->CacheNodeList, NULL, (Ptr)cacheNode);
    return cacheNode;
}

static u8 AllocCacheNode(DSCacheNodePtr* cacheNode, char* name) {
    if (*cacheNode) {
        FreeCacheNode(cacheNode);
    }

    *cacheNode = OSAlloc(sizeof(DSCacheNode));
    if (!*cacheNode) {
        return FALSE;
    }

    (*cacheNode)->Name = OSAlloc(Strlen(name) + 1);
    if (!(*cacheNode)->Name) {
        return FALSE;
    }

    return TRUE;
}

void DSEmptyCache(DSCachePtr cache) {
    DSCacheNodePtr cursor;
    DSCacheNodePtr cacheNode;

    cursor = (DSCacheNodePtr)cache->CacheNodeList.Head;
    while (cursor) {
        cacheNode = cursor;
        cursor = (DSCacheNodePtr)cursor->Link.Next;
        DSRemoveListObject(&cache->CacheNodeList, (Ptr)cacheNode);
        FreeCacheNode(&cacheNode);
    }
}

static DSCacheNodePtr FindCacheNode(DSCachePtr cache, char* name, Ptr data) {
    DSCacheNodePtr cacheNode;

    cacheNode = (DSCacheNodePtr)cache->CacheNodeList.Head;
    if (data) {
        while (cacheNode) {
            if (data == cacheNode->Data) {
                return cacheNode;
            }
            cacheNode = (DSCacheNodePtr)cacheNode->Link.Next;
        }
    } else if (name) {
        while (cacheNode) {
            if (Strcmp(name, cacheNode->Name) == 0) {
                return cacheNode;
            }
            cacheNode = (DSCacheNodePtr)cacheNode->Link.Next;
        }
    }
    return NULL;
}

Ptr DSGetCacheObj(DSCachePtr cache, char* name) {
    DSCacheNodePtr cacheNode;

    cacheNode = FindCacheNode(cache, name, NULL);
    if (cacheNode) {
        cacheNode->ReferenceCount++;
        return cacheNode->Data;
    }
    return NULL;
}

static void FreeCacheNode(DSCacheNodePtr* cacheNode) {
    if (*cacheNode) {
        if ((*cacheNode)->Free) {
            (*cacheNode)->Free(&(*cacheNode)->Data);
        }
        OSFree((*cacheNode)->Name);
        OSFree(*cacheNode);
        *cacheNode = NULL;
    }
}

void DSInitCache(DSCachePtr cache) {
    DSCacheNode cacheNode;

    cache->PurgeFlag = DS_AUTO_PURGE;
    DSInitList(&cache->CacheNodeList, (Ptr)&cacheNode, &cacheNode.Link);
}

void DSPurgeCache(DSCachePtr cache) {
    DSCacheNodePtr cursor;
    DSCacheNodePtr cacheNode;

    cursor = (DSCacheNodePtr)cache->CacheNodeList.Head;
    while (cursor) {
        cacheNode = cursor;
        cursor = (DSCacheNodePtr)cursor->Link.Next;
        if (cacheNode->ReferenceCount == 0) {
            DSRemoveListObject(&cache->CacheNodeList, (Ptr)cacheNode);
            FreeCacheNode(&cacheNode);
        }
    }
}

void DSReleaseCacheObj(DSCachePtr cache, Ptr data) {
    DSCacheNodePtr cacheNode;

    cacheNode = FindCacheNode(cache, NULL, data);
    if (cacheNode) {
        if (cacheNode->ReferenceCount != 0) {
            cacheNode->ReferenceCount--;
        }

        if (cacheNode->ReferenceCount == 0 && cache->PurgeFlag == DS_AUTO_PURGE) {
            DSRemoveListObject(&cache->CacheNodeList, (Ptr)cacheNode);
            FreeCacheNode(&cacheNode);
        }
    }
}

void DSSetCachePurgeFlag(DSCachePtr cache, u8 purgeFlag) {
    cache->PurgeFlag = purgeFlag;
}

void CSHInitDisplayCache(void) {
    DSInitCache(&DODisplayCache);
    DOCacheInitialized = TRUE;
}
