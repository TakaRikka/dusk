#include <revolution/nand.h>
#include <revolution/fs.h>

static const char* USER_DIR_LIST[] = {
    "/meta",
    "/ticket",
    "/title/00010000",
    "/title/00010001",
    "/title/00010003",
    "/title/00010004",
    "/title/00010005",
    "/title/00010006",
    "/title/00010007",
    "/shared2/title",
    NULL,
};

s32 nandCalcUsage(u32* fsBlock, u32* inode, const char* dir_list[]) {
    ISFSError ret = ISFS_ERROR_UNKNOWN;
    *fsBlock = 0;
    *inode = 0;

    while (*dir_list) {
        u32 blk = 0;
        u32 node = 0;
        ret = ISFS_GetUsage((const u8*)*dir_list, &blk, &node);

        if (ret == ISFS_ERROR_OK) {
            *fsBlock += blk;
            *inode += node;
        } else if (ret == ISFS_ERROR_NOEXISTS) {
            ret = ISFS_ERROR_OK;
        } else {
            break;
        }

        ++dir_list;
    }

    return ret;
}

ISFSError nandCalcUserUsage(u32* fsBlock, u32* inode) {
    return nandCalcUsage(fsBlock, inode, USER_DIR_LIST);
}

static u32 nandCheck(const u32 reqBlock, const u32 reqInode, const u32 homeBlock, const u32 homeInode, const u32 userBlock, const u32 userInode) {
    u32 answer = 0;

    if (homeBlock + reqBlock > 0x400) {
        answer |= 1;
    }
    
    if (homeInode + reqInode > 0x21) {
        answer |= 2;
    }

    if (userBlock + reqBlock > 0x4400) {
        answer |= 4;
    }

    if (userInode + reqInode > 0xFA0) {
        answer |= 8;
    }

    return answer;
}

s32 NANDCheck(const u32 fsBlock, const u32 inode, u32* answer) {
    ISFSError ret = ISFS_ERROR_UNKNOWN;
    u32 homeBlocks = 0xFFFFFFFF;
    u32 homeInodes = 0xFFFFFFFF;
    u32 userBlocks = 0xFFFFFFFF;
    u32 userInodes = 0xFFFFFFFF;

    if (!nandIsInitialized()) {
        return NAND_RESULT_FATAL_ERROR;
    }

    ret = ISFS_GetUsage((const u8*)nandGetHomeDir(), &homeBlocks, &homeInodes);

    if (ret != ISFS_ERROR_OK) {
        return nandConvertErrorCode(ret);
    }

    ret = nandCalcUserUsage(&userBlocks, &userInodes);

    if (ret != ISFS_ERROR_OK) {
        return nandConvertErrorCode(ret);
    }

    *answer = nandCheck(fsBlock, inode, homeBlocks, homeInodes, userBlocks, userInodes);
    return NAND_RESULT_OK;
}
