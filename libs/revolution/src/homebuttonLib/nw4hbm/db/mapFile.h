#ifndef NW4R_DB_MAPFILE_H
#define NW4R_DB_MAPFILE_H

#include <revolution/os.h>

namespace nw4hbm {
    namespace db {
        struct MapFile {
            /* 0x00 */ u8* mapBuf;
            /* 0x04 */ OSModuleInfo* moduleInfo;
            /* 0x08 */ s32 fileEntry;
            /* 0x0C */ MapFile* next;
        }; // size = 0x10

        bool MapFile_Exists();
        bool MapFile_QuerySymbol(u32 address, u8* strBuf, u32 strBufSize);
    }  // namespace db
}  // namespace nw4r

#endif
