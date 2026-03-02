#ifndef _CHARPIPELINE_FILECACHE_H_
#define _CHARPIPELINE_FILECACHE_H_

#include <charPipeline/structures.h>

#ifdef __cplusplus
extern "C" {
#endif

#define	DS_AUTO_PURGE 0
#define	DS_NO_PURGE 1

typedef struct {
    DSLink Link;
    void (*Free)(Ptr* data);
    char* Name;
    Ptr Data;
    u16 ReferenceCount;
} DSCacheNode, *DSCacheNodePtr;

typedef struct {
    u8 PurgeFlag;
    DSList CacheNodeList;
} DSCache, *DSCachePtr;

extern u8 DOCacheInitialized;
extern DSCache DODisplayCache;

DSCacheNodePtr DSAddCacheNode(DSCachePtr cache, char* name, Ptr data, Ptr OSFreeFunc);
void DSEmptyCache(DSCachePtr cache);
Ptr DSGetCacheObj(DSCachePtr cache, char* name);
void DSInitCache(DSCachePtr cache);
void DSPurgeCache(DSCachePtr cache);
void DSReleaseCacheObj(DSCachePtr cache, Ptr data);
void DSSetCachePurgeFlag(DSCachePtr cache, u8 purgeFlag);
void CSHInitDisplayCache(void);

#ifdef __cplusplus
}
#endif

#endif
