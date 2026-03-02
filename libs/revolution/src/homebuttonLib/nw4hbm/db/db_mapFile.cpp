#include <revolution/dvd.h>
#include <revolution/os.h>
#include "assert.h"
#include "mapFile.h"

#include "global.h"

typedef u8 GetCharFunc(u8 const* buf);

namespace nw4hbm {
    namespace db {
        static u8 GetCharOnMem_(const u8* buf);
        static u8 GetCharOnDvd_(u8 const* buf);

        static u8* SearchNextLine_(u8* buf, s32 lines);
        static u8* SearchNextSection_(u8* buf);
        static u8* SearchParam_(u8* lineTop, u32 argNum, u8 splitter);

        static u32 XStrToU32_(u8 const* str);
        static u32 CopySymbol_(u8 const* buf, u8* str, u32 strLenMax, u8 splitter);

        static bool QuerySymbolToMapFile_(u8* buf, OSModuleInfo const* moduleInfo, u32 address,
                                          u8* strBuf, u32 strBufSize);
        static bool QuerySymbolToSingleMapFile_(MapFile* pMapFile, u32 address, u8* strBuf,
                                                u32 strBufSize) NO_INLINE;
    }  // namespace db
}  // namespace nw4hbm

namespace nw4hbm {
    namespace db {
        static u8 sMapBuf[0x200];
        static s32 sMapBufOffset = -1;
        static DVDFileInfo sFileInfo;
        static u32 sFileLength;
        static MapFile* sMapFileList;
        static GetCharFunc* GetCharPtr_;
    }  // namespace db
}  // namespace nw4hbm

namespace nw4hbm {
    namespace db {
        bool MapFile_Exists(void) {
            return sMapFileList ? true : false;
        }

        static u8 GetCharOnMem_(u8 const* buf) {
            return *buf;
        }

        static s32 GetSize(s32 offset, u32 length) {
            if (offset + ARRAY_SIZE(sMapBuf) >= length) {
                return OSRoundUp32B(length - offset);
            }

            return ARRAY_SIZE(sMapBuf);
        }

        static u8 GetCharOnDvd_(u8 const* buf) {
            s32 address = (u32)buf & ~0x80000000;
            s32 offset = address - sMapBufOffset;

            if (address >= sFileLength) {
                return 0;
            }

            if (sMapBufOffset < 0 || offset < 0 || ARRAY_SIZE(sMapBuf) <= offset) {
                s32 len;
                s32 size;

                sMapBufOffset = OSRoundDown32B(address);
                offset = address - sMapBufOffset;
                size = GetSize(sMapBufOffset, sFileLength);

                BOOL enabled = OSEnableInterrupts();

                len = DVDReadAsyncPrio(&sFileInfo, sMapBuf, size, sMapBufOffset, NULL, 2);

                while (DVDGetCommandBlockStatus(&sFileInfo.cb)) {}

                OSRestoreInterrupts(enabled);

                if (len <= 0) {
                    return 0;
                }
            }

            return sMapBuf[offset];
        }

        void dummyString() {
            u8* buffer;
            NW4HBM_ASSERT_CHECK_NULL(0, buffer);

            u8* mapDataBuf;
            NW4HBM_ASSERT_CHECK_NULL(0, mapDataBuf);

            MapFile* pMapFile;
            NW4HBM_ASSERT_CHECK_NULL(0, pMapFile);

            NW4HBM_ASSERT(0, sMapFileList->moduleInfo != NULL);

            u8* filePath;
            NW4HBM_ASSERT_CHECK_NULL(0, filePath);

            NW4HBM_ASSERT(0, pMapFile->fileEntry >= 0);
        }

        static u8* SearchNextLine_(u8* buf, s32 lines) {
            u8 c;

            NW4HBM_ASSERT_CHECK_NULL(361, GetCharPtr_);

            if (buf == NULL) {
                return NULL;
            }

            for (; (c = (*GetCharPtr_)(buf)) != '\0'; buf++) {
                if (c == '\n') {
                    if (--lines <= 0) {
                        return buf + 1;
                    }
                }
            }

            return NULL;
        }

        static u8* SearchNextSection_(u8* buf) {
            NW4HBM_ASSERT_CHECK_NULL(397, GetCharPtr_);

            do {
                buf = SearchNextLine_(buf, 1);

                if (!buf) {
                    return NULL;
                }
            } while ((*GetCharPtr_)(buf) != '.');

            return buf;
        }

        static u8* SearchParam_(u8* lineTop, u32 argNum, u8 splitter) {
            int inArg = 0;
            u8* buf = lineTop;

            NW4HBM_ASSERT_CHECK_NULL(432, GetCharPtr_);

            if (buf == NULL) {
                return NULL;
            }

            while (true) {
                u8 c = (*GetCharPtr_)(buf);

                if (c == '\0' || c == '\n') {
                    return 0;
                }

                if (inArg) {
                    if (c == splitter) {
                        inArg = 0;
                    }
                } else if (c != splitter) {
                    if (!argNum--) {
                        return buf;
                    }

                    inArg = 1;
                }

                buf++;
            }

            return 0;
        }

        static u32 XStrToU32_(u8 const* str) {
            u32 val = 0;

            NW4HBM_ASSERT_CHECK_NULL(486, str);
            NW4HBM_ASSERT_CHECK_NULL(487, GetCharPtr_);

            while (true) {
                u32 num;
                u8 c;

                c = (*GetCharPtr_)(str);

                if ('0' <= c && c <= '9') {
                    num = static_cast<u32>(c - '0');
                } else if ('a' <= c && c <= 'z') {
                    num = static_cast<u32>(c - ('a' - 10));  // ?
                } else if ('A' <= c && c <= 'Z') {
                    num = static_cast<u32>(c - ('A' - 10));  // What's the - 10 for
                } else {
                    return val;
                }

                if (val >= 0x10000000) {
                    return 0;
                }

                val = num + (val << 4);
                str++;
            }

            return 0;
        }

        static u32 CopySymbol_(const u8* buf, u8* str, u32 strLenMax, u8 splitter) {
            u32 cnt = 0;

            NW4HBM_ASSERT_CHECK_NULL(544, buf);
            NW4HBM_ASSERT_CHECK_NULL(545, str);
            NW4HBM_ASSERT_CHECK_NULL(546, GetCharPtr_);

            while (true) {
                u8 c = (*GetCharPtr_)(buf++);

                if (c == splitter || c == '\0' || c == '\n') {
                    *str = '\0';
                    return cnt;
                }

                *str = c;
                str++;
                cnt++;

                if (cnt >= strLenMax - 1) {
                    *str = '\0';
                    return cnt;
                }
            }

            return 0;
        }

        static bool QuerySymbolToMapFile_(u8* buf, OSModuleInfo const* moduleInfo, u32 address,
                                          u8* strBuf, u32 strBufSize) {
            OSSectionInfo* sectionInfo = NULL;
            u32 sectionCnt;

            NW4HBM_ASSERT_CHECK_NULL(602, strBuf);
            NW4HBM_ASSERT(603, strBufSize > 0);

            if (moduleInfo) {
                sectionInfo = reinterpret_cast<OSSectionInfo*>(moduleInfo->sectionInfoOffset);
                sectionCnt = moduleInfo->numSections;
            }

            do {
                u32 offset = 0;

                buf = SearchNextSection_(buf);
                buf = SearchNextLine_(buf, 3);

                if (sectionInfo) {
                    offset = sectionInfo->offset;

                    if (address < offset) {
                        goto get_next_section_info;
                    }

                    if (address >= offset + sectionInfo->size) {
                        goto get_next_section_info;
                    }
                }

                while (true) {
                    u8* param;
                    u32 startAddr;
                    u32 size;

                    buf = SearchNextLine_(buf, 1);
                    if (!buf) {
                        return false;
                    }

                    param = SearchParam_(buf, 1, ' ');
                    if (!param) {
                        break;
                    }

                    size = XStrToU32_(param);
                    param = SearchParam_(buf, 2, ' ');
                    if (!param) {
                        break;
                    }

                    startAddr = XStrToU32_(param);
                    if (!startAddr) {
                        continue;
                    }

                    startAddr = startAddr + offset;
                    if (address < startAddr || startAddr + size <= address) {
                        continue;
                    }

                    param = SearchParam_(buf, 5, ' ');
                    if (!param) {
                        *strBuf = '\0';
                        return true;
                    }

                    if ((*GetCharPtr_)(param) == '.') {
                        continue;
                    }

                    CopySymbol_(param, strBuf, strBufSize, ' ');
                    return true;
                }

            get_next_section_info:
                if (sectionInfo) {
                    if (!--sectionCnt) {
                        return false;
                    }

                    sectionInfo++;
                }
            } while (true);

            return false;
        }

        static bool QuerySymbolToSingleMapFile_(MapFile* pMapFile, u32 address, u8* strBuf,
                                                u32 strBufSize) {
            NW4HBM_ASSERT_CHECK_NULL(723, pMapFile);
            NW4HBM_ASSERT_CHECK_NULL(724, strBuf);

            if (pMapFile->mapBuf) {
                GetCharPtr_ = &GetCharOnMem_;
                return QuerySymbolToMapFile_(pMapFile->mapBuf, pMapFile->moduleInfo, address,
                                             strBuf, strBufSize);
            }

            if (pMapFile->fileEntry >= 0) {
                bool ret;

                if (DVDFastOpen(pMapFile->fileEntry, &sFileInfo)) {
                    sFileLength = sFileInfo.length;
                    GetCharPtr_ = &GetCharOnDvd_;
                    ret = QuerySymbolToMapFile_((u8*)(0x80000000), pMapFile->moduleInfo, address, strBuf, strBufSize);

                    DVDClose(&sFileInfo);
                    return ret;
                }
            }

            *strBuf = '\0';
            return false;
        }

        bool MapFile_QuerySymbol(u32 address, u8* strBuf, u32 strBufSize) {
            MapFile* pMap;
            for (pMap = sMapFileList; pMap; pMap = pMap->next) {
                if (QuerySymbolToSingleMapFile_(pMap, address, strBuf, strBufSize)) {
                    return true;
                }
            }

            return false;
        }

    }  // namespace db
}  // namespace nw4hbm
