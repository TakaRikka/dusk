#include <revolution/fs.h>
#include <revolution/ipc.h>
#include <cstring>

static IOSFd __fsFd = -1;
static u32 __fsInitialized = FALSE;
static char* __devfs = 0;
static IOSHeapId hId;
static s32 _asynCnt = 0;

#define ROUNDUP(sz) (((u32)(sz) + 31) & ~(u32)(31))

typedef struct isfs_GetAttr {
    IOSUid* ownerId;
    IOSGid* groupId;
    u32* attr;
    u32* ownerAcc;
    u32* groupAcc;
    u32* othersAcc;
} isfs_GetAttr;

typedef struct isfs_GetUsage {
    u32* nblocks;
    u32* ninodes;
} isfs_GetUsage;

typedef struct __isfsCtxt {
    u8 ioBuf[ROUNDUP(256)] ATTRIBUTE_ALIGN(32);
    ISFSCallback cb;
    void* ctxt;
    u32 func;

    union {
        ISFSStats* stats;
        ISFSFileStats* fstats;
        u32* num;
        isfs_GetAttr ga;
        isfs_GetUsage gu;
    } args;
} __isfsCtxt;

static IOSError _FSGetStatsCb(IOSError ret, void* ctxt);
static IOSError _FSReadDirCb(IOSError ret, void* ctxt);
static IOSError _FSGetAttrCb(IOSError ret, void* ctxt);
static IOSError _FSGetUsageCb(IOSError ret, void* ctxt);
static IOSError _FSGetFileStatsCb(IOSError ret, void* ctxt);

ISFSError ISFS_OpenLib(void) {
    ISFSError rc = 0;
    static void* lo = 0, *hi = 0;
    __isfsCtxt* __fsCtxt = 0;

    if (!__fsInitialized) {
        lo = IPCGetBufferLo();
        hi = IPCGetBufferHi();
    }

    __devfs = (char*)ROUNDUP(lo);
    if (!__fsInitialized && ((u32)__devfs + ROUNDUP(64)) > (u32)hi) {
        OSReport("APP ERROR: Not enough IPC arena\n");
        rc = -22;
        goto out;
    }

    strcpy(__devfs, "/dev/fs");
    __fsFd = IOS_Open(__devfs, 0);

    if (__fsFd < 0) {
        rc = __fsFd;
        goto out;
    }

    __fsCtxt = (__isfsCtxt*)((u32)__devfs);

    if (!__fsInitialized && ((u32)__fsCtxt + (16 + 1) *  ROUNDUP(sizeof(__isfsCtxt))) > (u32)hi) {
        OSReport("APP ERROR: Not enough IPC arena\n");
        rc = -22;
        goto out;
    }

    if (!__fsInitialized) {
        IPCSetBufferLo((void*)((u32)__fsCtxt + (17) * ROUNDUP(sizeof(__isfsCtxt))));
        __fsInitialized = TRUE;
    }

    hId = iosCreateHeap(__fsCtxt, 17 * ROUNDUP(sizeof(__isfsCtxt)));

    if (hId < 0) {
        rc = -22;
        goto out;
    }

out:
    return rc;
}

IOSError _isfsFuncCb(IOSError ret, void* ctxt) {
    ISFSError rc = 0;
    __isfsCtxt* _ctxt = (__isfsCtxt*)ctxt;
    rc = ret;

    if (rc >= 0) {
        switch (_ctxt->func) {
        case 1:
            _FSGetStatsCb(ret, ctxt);
            break;
        case 2:
            _FSReadDirCb(ret, ctxt);
            break;
        case 3:
            _FSGetAttrCb(ret, ctxt);
            break;
        case 4:
            _FSGetUsageCb(ret, ctxt);
            break;
        case 5:
            _FSGetFileStatsCb(ret, ctxt);
            break;
        default:
            break;
        }
    }

    _asynCnt = 0;

    if (_ctxt->cb) {
        _ctxt->cb(rc, _ctxt->ctxt);
    }

    if (ctxt) {
        iosFree(hId, ctxt);
    }

    return rc;
}

static IOSError _FSGetStatsCb(IOSError ret, void* ctxt) {
    __isfsCtxt* _ctxt = (__isfsCtxt*)ctxt;
    ISFSError rc = ISFS_ERROR_OK;

    if (ret == 0) {
        memcpy(_ctxt->args.stats, _ctxt->ioBuf, sizeof(*_ctxt->args.stats));
    }

    return rc;
}

s32 ISFS_CreateDir(const u8* dname, u32 dirAttr, u32 ownerAcc, u32 groupAcc, u32 othersAcc) {
    ISFSError rc = ISFS_ERROR_OK;
    ISFSPathAttrArgs* pathAttrArgs;
    __isfsCtxt* blCtxt = 0;
    u32 len;

    if (dname == NULL || __fsFd < 0 || (len = strnlen(dname, 64)) == 64) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    blCtxt = iosAllocAligned(hId, sizeof(*blCtxt), 32);

    if (blCtxt == 0) {
        rc = IOS_ERROR_FAIL_ALLOC;
        goto out;
    }

    pathAttrArgs = (ISFSPathAttrArgs*)blCtxt->ioBuf;
    memcpy(pathAttrArgs->path, dname, len + 1);

    pathAttrArgs->attr = (u8)dirAttr;
    pathAttrArgs->ownerAccess = (u8)ownerAcc;
    pathAttrArgs->groupAccess = (u8)groupAcc;
    pathAttrArgs->othersAccess = (u8)othersAcc;
    rc = IOS_Ioctl(__fsFd, 3, pathAttrArgs, sizeof(*pathAttrArgs), NULL, 0);

out:
    /* they seem to have had a macro for this, and the programmer forgot it did a NULL check */
    if (blCtxt != 0) {
        if (blCtxt != 0) {
            iosFree(hId, blCtxt);
        }
        
    }

    return rc;
}

s32 ISFS_CreateDirAsync(const u8* dname, u32 dirAttr, u32 ownerAcc, u32 groupAcc, u32 othersAcc, ISFSCallback cb, void* fsCtxt) {
    ISFSError rc = ISFS_ERROR_OK;
    u32 len;
    ISFSPathAttrArgs* pathAttrArgs;
    __isfsCtxt* ctxt;

    if (dname == NULL || __fsFd < 0 || (len = strnlen(dname, 64)) == 64) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    ctxt = iosAllocAligned(hId, sizeof(*ctxt), 32);

    if (ctxt == 0) {
        rc = ISFS_ERROR_BUSY;
        goto out;
    }

    ctxt->cb = cb;
    ctxt->ctxt = fsCtxt;
    ctxt->func = 0;
    pathAttrArgs = (ISFSPathAttrArgs*)ctxt->ioBuf;
    memcpy(pathAttrArgs->path, (void*)dname, len + 1);
    pathAttrArgs->attr = dirAttr;
    pathAttrArgs->ownerAccess = ownerAcc;
    pathAttrArgs->groupAccess = groupAcc;
    pathAttrArgs->othersAccess = othersAcc;
    rc = IOS_IoctlAsync(__fsFd, 3, pathAttrArgs, sizeof(*pathAttrArgs), NULL, 0, _isfsFuncCb, ctxt);

out:
    return rc;
}

s32 ISFS_ReadDir(const u8* dname, u8* nameList, u32* num) {
    ISFSError rc = ISFS_ERROR_OK;
    IOSIoVector* v = 0;
    u32 len, numInputs, numOutputs, *numPtr;
    char* dnPtr;
    __isfsCtxt* blCtxt = 0;

    if (dname == NULL || num == NULL || __fsFd < 0 || (u32)nameList & 31 || (len = strnlen(dname, 64)) == 64) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    blCtxt = iosAllocAligned(hId, sizeof(*blCtxt), 32);
    if (blCtxt == 0) {
        rc = IOS_ERROR_FAIL_ALLOC;
        goto out;
    }

    v = (IOSIoVector*)blCtxt->ioBuf;
    dnPtr = (char*)ROUNDUP((u8*)&v[4]);
    memcpy(dnPtr, dname, len + 1);
    v[0].base = (u8*)dnPtr;
    v[0].length = 64;
    numPtr = (u32*)ROUNDUP((u32)dnPtr + 64);
    v[1].base = (u8*)numPtr;
    v[1].length = 4;

    if (nameList != 0) {
        numInputs = 2;
        numOutputs = 2;
        *numPtr = *num;
        v[2].base = nameList;
        v[2].length = *num * 13;
        v[3].base = (u8*)numPtr;
        v[3].length = 4;
    } else {
        numInputs = 1;
        numOutputs = 1;
    }

    rc = IOS_Ioctlv(__fsFd, 4, numInputs, numOutputs, v);

    if (rc != ISFS_ERROR_OK) {
        goto out;
    }

    *num = *numPtr;

out:
    if (blCtxt != 0) {
        if (blCtxt != 0) {
            iosFree(hId, blCtxt);
        }
    }

    return rc;
}

static IOSError _FSReadDirCb(IOSError ret, void* ctxt) {
    ISFSError rc = ISFS_ERROR_OK;
    __isfsCtxt* _ctxt = (__isfsCtxt*)ctxt;

    if (ret == 0) {
        u8* ptr;
        IOSIoVector* v = (IOSIoVector*)_ctxt->ioBuf;
        ptr = (u8*)ROUNDUP((u8*)&v[4]);
        ptr = (u8*)ROUNDUP(ptr + 64);
        *_ctxt->args.num = *(u32 *) ptr;
    }

    return rc;
}

s32 ISFS_ReadDirAsync(const u8* dname, u8* nameList, u32* num, ISFSCallback cb, void* fsCtxt) {
    ISFSError rc = ISFS_ERROR_OK;
    u32 len, numOutputs, numInputs, *numPtr;
    IOSIoVector* v;
    __isfsCtxt* ctxt;
    char* dnPtr;
    
    if (dname == NULL || num == NULL || __fsFd < 0 || (u32)nameList & 31 || (len = strnlen(dname, 64)) == 64) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    ctxt = iosAllocAligned(hId, sizeof(*ctxt), 32);
    if (ctxt == 0) {
        rc = ISFS_ERROR_BUSY;
        goto out;
    }

    ctxt->cb = cb;
    ctxt->ctxt = fsCtxt;
    ctxt->func = 2;
    ctxt->args.num = num;
    v = (IOSIoVector*)ctxt->ioBuf;
    dnPtr = (char*)ROUNDUP((u8*)&v[4]);
    memcpy(dnPtr, dname, len + 1);
    v[0].base = (u8*)dnPtr;
    v[0].length = 64;
    numPtr = (u32*)ROUNDUP((u32)dnPtr + 64);
    v[1].base = (u8*)numPtr;
    v[1].length = 4;

    if (nameList != 0) {
        numInputs = 2;
        numOutputs = 2;
        *numPtr = *num;
        v[2].base = nameList;
        v[2].length = *num * 13;
        v[3].base = (u8*)numPtr;
        v[3].length = 4;
    } else {
        numInputs = 1;
        numOutputs = 1;
    }

    rc = IOS_IoctlvAsync(__fsFd, 4, numInputs, numOutputs, v, _isfsFuncCb, ctxt);

out:
    return rc;
}

s32 ISFS_GetAttr(const u8* name, IOSUid* ownerId, IOSGid* groupId, u32* attr, u32* ownerAcc, u32* groupAcc, u32* othersAcc) {
    ISFSError rc = ISFS_ERROR_OK;
    ISFSPathAttrArgs* pathAttrArgs;
    u8* ptr;
    u32 len;
    __isfsCtxt* blCtxt = 0;

    if (name == NULL || __fsFd < 0 ||
            (len = strnlen(name, 64)) == 64 ||
            ownerId == NULL || groupId == NULL || attr == NULL ||
            ownerAcc == NULL || groupAcc == NULL || othersAcc == NULL) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    blCtxt = iosAllocAligned(hId, sizeof(*blCtxt), 32);
    if (blCtxt == 0) {
        rc = IOS_ERROR_FAIL_ALLOC;
        goto out;
    }

    ptr = (u8*)blCtxt->ioBuf;
    memcpy(ptr, name, len + 1);
    pathAttrArgs = (ISFSPathAttrArgs*)ROUNDUP(ptr + 64);
    rc = IOS_Ioctl(__fsFd, 6, ptr, 64, pathAttrArgs, sizeof(*pathAttrArgs));

    if (rc != IOS_ERROR_OK) {
        goto out;
    }

    *ownerId = pathAttrArgs->ownerId;
    *groupId = pathAttrArgs->groupId;
    *attr = pathAttrArgs->attr;
    *ownerAcc = pathAttrArgs->ownerAccess;
    *groupAcc = pathAttrArgs->groupAccess;
    *othersAcc = pathAttrArgs->othersAccess;

out:
    if (blCtxt != 0) {
        if (blCtxt != 0) {
            iosFree(hId, blCtxt);
        }
    }

    return rc;
}

static IOSError _FSGetAttrCb(IOSError ret, void* ctxt) {
    ISFSError rc = ret;

    if (ret == 0) {
        __isfsCtxt* _ctxt = (__isfsCtxt*)ctxt;
        ISFSPathAttrArgs* pathAttrArgs = (ISFSPathAttrArgs*)ROUNDUP(_ctxt->ioBuf + 64);
    
        *_ctxt->args.ga.ownerId = pathAttrArgs->ownerId;
        *_ctxt->args.ga.groupId = pathAttrArgs->groupId;
        *_ctxt->args.ga.attr = pathAttrArgs->attr; 
        *_ctxt->args.ga.ownerAcc = pathAttrArgs->ownerAccess;
        *_ctxt->args.ga.groupAcc = pathAttrArgs->groupAccess;
        *_ctxt->args.ga.othersAcc = pathAttrArgs->othersAccess;
    }

    return rc;
}

s32 ISFS_GetAttrAsync(const u8* name, IOSUid* ownerId, IOSGid* groupId, u32* attr, u32* ownerAcc, u32* groupAcc, u32* othersAcc, ISFSCallback cb, void* fsCtxt) {
    ISFSError rc = ISFS_ERROR_OK;
    __isfsCtxt *ctxt;
    ISFSPathAttrArgs *pathAttrArgs;
    u8 *ptr;
    u32 len;

    if (name == NULL || __fsFd < 0 ||
        (len = strnlen(name, 64)) == 64 ||
        ownerId == NULL || groupId == NULL || attr == NULL ||
        ownerAcc == NULL || groupAcc == NULL || othersAcc == NULL) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    ctxt =  iosAllocAligned(hId, sizeof(*ctxt), 32);
    if (ctxt == 0) {
        rc = ISFS_ERROR_BUSY;
        goto out;
    }

    ctxt->args.ga.ownerId = ownerId;
    ctxt->args.ga.groupId = groupId;
    ctxt->args.ga.attr = attr; 
    ctxt->args.ga.ownerAcc = ownerAcc;
    ctxt->args.ga.groupAcc = groupAcc;
    ctxt->args.ga.othersAcc = othersAcc;
    ctxt->cb = cb;
    ctxt->ctxt = fsCtxt;
    ctxt->func = 3;

    ptr = (u8*)ctxt->ioBuf;
    memcpy(ptr, name, len + 1);
    pathAttrArgs = (ISFSPathAttrArgs*)ROUNDUP(ptr + 64);

    rc = IOS_IoctlAsync(__fsFd, 6, ptr, 64, pathAttrArgs, sizeof(*pathAttrArgs), _isfsFuncCb, ctxt);
out:
    return rc;
}

s32 ISFS_Delete(const u8* name) {
    ISFSError rc = ISFS_ERROR_OK;
    u32 len;
    __isfsCtxt* blCtxt = 0;

    if (name == NULL || __fsFd < 0 || (len = strnlen(name, 64)) == 64) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    blCtxt = iosAllocAligned(hId, sizeof(*blCtxt), 32);
    if (blCtxt == 0) {
        rc = IOS_ERROR_FAIL_ALLOC;
        goto out;
    }

    memcpy(blCtxt->ioBuf, name, len + 1);
    rc = IOS_Ioctl(__fsFd, 7, blCtxt->ioBuf, 64, NULL, 0);

out:
    if (blCtxt != 0) {
        if (blCtxt != 0) {
            iosFree(hId, blCtxt);
        }
    }

    return rc;
}

s32 ISFS_DeleteAsync(const u8* name, ISFSCallback cb, void* fsCtxt) {
    ISFSError rc = ISFS_ERROR_OK;
    u32 len;
    __isfsCtxt* ctxt;

    if (name == NULL || __fsFd < 0 || (len = strnlen(name, 64)) == 64) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    ctxt = iosAllocAligned(hId, sizeof(*ctxt), 32);
    if (ctxt == 0) {
        rc = ISFS_ERROR_BUSY;
        goto out;
    }

    memcpy(ctxt->ioBuf, name, len + 1);
    ctxt->cb = cb;
    ctxt->ctxt = fsCtxt;
    ctxt->func = 0;
    rc = IOS_IoctlAsync(__fsFd, 7, ctxt->ioBuf, 64, NULL, 0, _isfsFuncCb, ctxt);

out:
    return rc;
}

s32 ISFS_Rename(const u8* oldName, const u8* newName) {
    ISFSError rc = ISFS_ERROR_OK;
    ISFSPathsArgs* pathsArgs;
    u32 oldLen, newLen;
    __isfsCtxt* blCtxt = 0;

    if (oldName == NULL || newName == NULL || __fsFd < 0 ||
        (oldLen = strnlen(oldName, 64)) == 64 || 
        (newLen = strnlen(newName, 64)) == 64) {
            rc = ISFS_ERROR_INVALID;
            goto out;
        }
    
    blCtxt = iosAllocAligned(hId, sizeof(*blCtxt), 32);
    if (blCtxt == 0) {
        rc = IOS_ERROR_FAIL_ALLOC;
        goto out;
    }

    pathsArgs = (ISFSPathsArgs*)blCtxt->ioBuf;
    memcpy(pathsArgs->path1, oldName, oldLen + 1);
    memcpy(pathsArgs->path2, newName, newLen + 1);
    rc = IOS_Ioctl(__fsFd, 8, pathsArgs, sizeof(*pathsArgs), NULL, 0);

out:
    if (blCtxt != 0) {
        if (blCtxt != 0) {
            iosFree(hId, blCtxt);
        }
    }

    return rc;
}

s32 ISFS_RenameAsync(const u8* oldName, const u8* newName, ISFSCallback cb, void* fsCtxt) {
    ISFSError rc = ISFS_ERROR_OK;
    ISFSPathsArgs* pathsArgs;
    u32 oldLen, newLen;
    __isfsCtxt* ctxt;

    if ((oldName == NULL) || newName == NULL || __fsFd < 0 ||
        (oldLen = strnlen(oldName, 64)) == 64 || 
        (newLen = strnlen(newName, 64)) == 64) {
            rc = ISFS_ERROR_INVALID;
            goto out;
        }
    
    ctxt = iosAllocAligned(hId, sizeof(*ctxt), 32);
    if (ctxt == NULL) {
        rc = ISFS_ERROR_BUSY;
        goto out;
    }

    ctxt->cb = cb;
    ctxt->ctxt = fsCtxt;
    ctxt->func = 0;

    pathsArgs = (ISFSPathsArgs*)ctxt->ioBuf;
    memcpy(pathsArgs->path1, oldName, oldLen + 1);
    memcpy(pathsArgs->path2, newName, newLen + 1);
    rc = IOS_IoctlAsync(__fsFd, 8, pathsArgs, sizeof(*pathsArgs), NULL, 0, _isfsFuncCb, ctxt);

out:
    return rc;
}

s32 ISFS_GetUsage(const u8* dname, u32* nblocks, u32* ninodes) {
    ISFSError rc = ISFS_ERROR_OK;
    IOSIoVector* v = 0;
    u32 len, *blkPtr, *inodePtr;
    char* dnPtr;
    __isfsCtxt* blCtxt = 0;

    if (dname == NULL || __fsFd < 0 ||
        nblocks == NULL || ninodes == NULL ||
        (len = strnlen(dname, 64)) == 64) {

        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    blCtxt = iosAllocAligned(hId, sizeof(*blCtxt), 32);
    if (blCtxt == 0) {
        rc = IOS_ERROR_FAIL_ALLOC;
        goto out;
    }

    v = (IOSIoVector*)blCtxt->ioBuf;
    dnPtr = (char*)ROUNDUP((u8*)&v[3]);
    memcpy(dnPtr, dname, len + 1);
    v[0].base = (u8*)dnPtr;
    v[0].length = 64;

    blkPtr = (u32*) ROUNDUP(((u32)dnPtr) + 64);
    inodePtr = (u32*) ROUNDUP(((u32)blkPtr) + 4);
    v[1].base = (u8*) blkPtr;
    v[1].length = 4;
    v[2].base = (u8*)inodePtr;
    v[2].length = 4;

    rc = IOS_Ioctlv(__fsFd,  12, 1, 2, v);
    if (rc != ISFS_ERROR_OK) {
        goto out;
    }

    *nblocks = *blkPtr;
    *ninodes = *inodePtr;

out:
    if (blCtxt != 0) {
        if (blCtxt != 0) {
            iosFree(hId, blCtxt);
        }
    }

    return rc;
}

static IOSError _FSGetUsageCb(IOSError ret, void* ctxt) {
    ISFSError rc = ISFS_ERROR_OK;
    __isfsCtxt* _ctxt = (__isfsCtxt*)ctxt;

    if (ret == 0) {
        u8* ptr;
        IOSIoVector* v = (IOSIoVector*)_ctxt->ioBuf;
        ptr = (u8*)ROUNDUP((u8*)&v[4]);
        ptr = (u8*)ROUNDUP(ptr + 64);
        *_ctxt->args.gu.nblocks = *(u32*)ptr;
        ptr = (u8*)ROUNDUP(ptr + 4);
        *_ctxt->args.gu.ninodes = *(u32*)ptr;
    }

    return rc;
}

s32 ISFS_CreateFile(const u8* fname, u32 fileAttr, u32 ownerAcc, u32 groupAcc, u32 othersAcc) {
    ISFSError rc = ISFS_ERROR_OK;
    ISFSPathAttrArgs* pathAttrArgs;
    u32 len;
    __isfsCtxt* blCtxt = NULL;

    if (fname == NULL || __fsFd < 0 || (len = strnlen(fname, 64)) == 64) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    blCtxt = iosAllocAligned(hId, sizeof(*blCtxt), 32);
    if (blCtxt == 0) {
        rc = IOS_ERROR_FAIL_ALLOC;
        goto out;
    }

    pathAttrArgs = (ISFSPathAttrArgs*)blCtxt->ioBuf;
    memcpy(pathAttrArgs->path, fname, len + 1);
    pathAttrArgs->attr = fileAttr;
    pathAttrArgs->ownerAccess = ownerAcc;
    pathAttrArgs->groupAccess = groupAcc;
    pathAttrArgs->othersAccess = othersAcc;
    rc = IOS_Ioctl(__fsFd, 9, pathAttrArgs, sizeof(*pathAttrArgs), NULL, 0);

out:
    if (blCtxt != 0) {
        if (blCtxt != 0) {
            iosFree(hId, blCtxt);
        }
    }

    return rc;
}

s32 ISFS_CreateFileAsync(const u8* fname, u32 fileAttr, u32 ownerAcc, u32 groupAcc, u32 othersAcc, ISFSCallback cb, void* fsCtxt) {
    ISFSError rc = ISFS_ERROR_OK;
    ISFSPathAttrArgs* pathAttrArgs;
    u32 len;
    __isfsCtxt* ctxt;

    if (fname == NULL || __fsFd < 0 || (len = strnlen(fname, 64)) == 64) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    ctxt = iosAllocAligned(hId, sizeof(*ctxt), 32);
    if (ctxt == 0) {
        rc = ISFS_ERROR_BUSY;
        goto out;
    }

    ctxt->cb = cb;
    ctxt->ctxt = fsCtxt;
    ctxt->func = 0;

    pathAttrArgs = (ISFSPathAttrArgs*)ctxt->ioBuf;
    memcpy(pathAttrArgs->path, fname, len + 1);
    pathAttrArgs->attr = fileAttr;
    pathAttrArgs->ownerAccess = ownerAcc;
    pathAttrArgs->groupAccess = groupAcc;
    pathAttrArgs->othersAccess = othersAcc;
    rc = IOS_IoctlAsync(__fsFd, 9, pathAttrArgs, sizeof(*pathAttrArgs), NULL, 0, _isfsFuncCb, ctxt);

out:
    return rc;
}

IOSFd ISFS_Open(const u8* fname, u32 access) {
    ISFSError rc = ISFS_ERROR_OK;
    u32 len;
    __isfsCtxt* blCtxt = 0;

    if (fname == NULL || (len = strnlen(fname, 64)) == 64) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    blCtxt = iosAllocAligned(hId, sizeof(*blCtxt), 32);
    if (blCtxt == 0) {
        rc = IOS_ERROR_FAIL_ALLOC;
        goto out;
    }

    memcpy(blCtxt->ioBuf, fname, len + 1);
    rc = IOS_Open((const char*)blCtxt->ioBuf, access);

out:
    if (blCtxt != 0) {
        if (blCtxt != 0) {
            iosFree(hId, blCtxt);
        }
    }

    return rc;
}

IOSFd ISFS_OpenAsync(const u8* fname, u32 access, ISFSCallback cb, void* fsCtxt) {
    ISFSError rc = ISFS_ERROR_OK;
    u32 len;
    __isfsCtxt* ctxt;

    if (fname == NULL || (len = strnlen(fname, 64)) == 64) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    ctxt = iosAllocAligned(hId, sizeof(*ctxt), 32);
    if (ctxt == 0) {
        rc = ISFS_ERROR_BUSY;
        goto out;
    }

    ctxt->cb = cb;
    ctxt->ctxt = fsCtxt;
    ctxt->func = 0;
    memcpy(ctxt->ioBuf, fname, len + 1);
    rc = IOS_OpenAsync((const char*)ctxt->ioBuf, access, _isfsFuncCb, ctxt);

out:
    return rc;
}

static IOSError _FSGetFileStatsCb(IOSError ret, void* ctxt) {
    __isfsCtxt* _ctxt = (__isfsCtxt*)ctxt;
    ISFSError rc = ISFS_ERROR_OK;

    if (ret == 0) {
        memcpy(_ctxt->args.fstats, _ctxt->ioBuf, sizeof(*_ctxt->args.fstats));
    }

    return rc;
}

s32 ISFS_GetFileStats(IOSFd fd, ISFSFileStats* stats) {
    ISFSError rc = ISFS_ERROR_OK;
    __isfsCtxt* blCtxt = 0;

    if (stats == NULL || ((u32)stats & 31)) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    blCtxt = iosAllocAligned(hId, sizeof(*blCtxt), 32);
    if (blCtxt == 0) {
        rc = IOS_ERROR_FAIL_ALLOC;
        goto out;
    }

    rc = IOS_Ioctl(fd, 11, NULL, 0, blCtxt->ioBuf, sizeof(*stats));

    if (rc != IOS_ERROR_OK) {
        goto out;
    }

    memcpy(stats, blCtxt->ioBuf, sizeof(*stats));

out:
    if (blCtxt != 0) {
        if (blCtxt != 0) {
            iosFree(hId, blCtxt);
        }
    }

    return rc;
}

s32 ISFS_GetFileStatsAsync(IOSFd fd, ISFSFileStats* stats, ISFSCallback cb, void* fsCtxt) {
    ISFSError rc = ISFS_ERROR_OK;
    __isfsCtxt* ctxt;

    if (stats == NULL || ((u32)stats & 31)) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    ctxt = iosAllocAligned(hId, sizeof(*ctxt), 32);
    if (ctxt == 0) {
        rc = ISFS_ERROR_BUSY;
        goto out;
    }

    ctxt->cb = cb;
    ctxt->ctxt = fsCtxt;
    ctxt->func = 5;
    ctxt->args.fstats = stats;
    rc = IOS_IoctlAsync(fd, 11, NULL, 0, ctxt->ioBuf, sizeof(*stats), _isfsFuncCb, ctxt);

out:
    return rc;
}

s32 ISFS_Seek(IOSFd fd, s32 offset, u32 whence) {
    ISFSError rc = ISFS_ERROR_OK;
    rc = IOS_Seek(fd, offset, whence);
    return rc;
}

s32 ISFS_SeekAsync(IOSFd fd, s32 offset, u32 whence, ISFSCallback cb, void* fsCtxt) {
    ISFSError rc;
    __isfsCtxt* ctxt;

    ctxt = iosAllocAligned(hId, sizeof(*ctxt), 32);
    if (ctxt == 0) {
        rc = ISFS_ERROR_BUSY;
        goto out;
    }

    ctxt->cb = cb;
    ctxt->ctxt = fsCtxt;
    ctxt->func = 0;
    rc = IOS_SeekAsync(fd, offset, whence, _isfsFuncCb, ctxt);

out:
    return rc;
}

s32 ISFS_Read(s32 fd, u8* pBuffer, u32 bufSize) {
    ISFSError rc = ISFS_ERROR_OK;

    /* nullptr check and alignment to 0x20 */
    if (pBuffer == NULL || (u32)pBuffer & 31) {
        rc = ISFS_ERROR_INVALID;
    } else {
        rc = IOS_Read(fd, pBuffer, bufSize);;
    }

    return rc;
}

s32 ISFS_ReadAsync(IOSFd fd, u8* buf, u32 size, ISFSCallback cb, void* fsCtxt) {
    ISFSError rc = ISFS_ERROR_OK;
    __isfsCtxt* ctxt;

    if (buf == NULL || ((u32)buf & 31)) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    ctxt = iosAllocAligned(hId, sizeof(*ctxt), 32);
    if (ctxt == 0) {
        rc = ISFS_ERROR_BUSY;
        goto out;
    }

    ctxt->cb = cb;
    ctxt->ctxt = fsCtxt;
    ctxt->func = 0;
    rc = IOS_ReadAsync(fd, buf, size, _isfsFuncCb, ctxt);

out:
    return rc;
}

s32 ISFS_Write(IOSFd fd, const u8* buf, u32 size) {
    ISFSError rc = ISFS_ERROR_OK;

    if (buf == NULL || ((u32)buf & 31)) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    rc = IOS_Write(fd, (u8*)buf, size);
out:
    return rc;
}

s32 ISFS_WriteAsync(IOSFd fd, const u8* buf, u32 size, ISFSCallback cb, void* fsCtxt) {
    ISFSError rc = ISFS_ERROR_OK;
    __isfsCtxt* ctxt;

    if (buf == NULL || ((u32)buf & (u32)(31))) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    ctxt = iosAllocAligned(hId, sizeof(*ctxt), 32);
    if (ctxt == 0) {
        rc = ISFS_ERROR_BUSY;
        goto out;
    }

    ctxt->cb = cb;
    ctxt->ctxt = fsCtxt;
    ctxt->func = 0;
    rc = IOS_WriteAsync(fd, (void*)buf, size, _isfsFuncCb, ctxt);

out:
    return rc;
}

s32 ISFS_Close(IOSFd fd) {
    ISFSError rc = ISFS_ERROR_OK;
    rc = IOS_Close(fd);
    return rc;
}

s32 ISFS_CloseAsync(IOSFd fd, ISFSCallback cb, void* fsCtxt) {
    ISFSError rc;
    __isfsCtxt* ctxt;

    ctxt = iosAllocAligned(hId, sizeof(*ctxt), 32);

    if (ctxt == 0) {
        rc = ISFS_ERROR_BUSY;
        goto out;
    }

    ctxt->cb = cb;
    ctxt->ctxt = fsCtxt;
    ctxt->func = 0;
    rc = IOS_CloseAsync(fd, _isfsFuncCb, ctxt);

out:
    return rc;
}

s32 ISFS_ShutdownAsync(ISFSCallback cb, void* fsCtxt) {
    __isfsCtxt* ctxt;
    s32 rc = ISFS_ERROR_OK;

    ctxt = iosAllocAligned(hId, sizeof(*ctxt), 32);

    if (__fsFd < 0) {
        rc = ISFS_ERROR_INVALID;
        goto out;
    }

    ctxt->cb = cb;
    ctxt->ctxt = fsCtxt;
    ctxt->func = 0;
    rc = IOS_IoctlAsync(__fsFd, 13, NULL, 0, NULL, 0, _isfsFuncCb, ctxt);

out:
    return rc;
}
