#ifndef NW4HBM_UT_DVD_LOCKED_FILE_STREAM_H
#define NW4HBM_UT_DVD_LOCKED_FILE_STREAM_H

#include "DvdFileStream.h"
#include <revolution/os.h>

namespace nw4hbm {
    namespace ut {

        class DvdLockedFileStream : public DvdFileStream {
        public:
            explicit DvdLockedFileStream(s32 entrynum);
            DvdLockedFileStream(const DVDFileInfo* info, bool close);

            /* 0x08 */ NW4HBM_UT_RUNTIME_TYPEINFO;
            /* 0x0C */ virtual ~DvdLockedFileStream();
            /* 0x14 */ virtual s32 Read(void* pDst, u32 size);
            /* 0x28 */ virtual bool CanAsync() const { return false; }

            /* 0x18 */ virtual bool ReadAsync(void* pDst, u32 size, IOStreamCallback pCallback,
                                              void* pCallbackArg) {
                return false;
            }

            /* 0x5C */ virtual s32 Peek(void* pDst, u32 size);
            /* 0x60 */ virtual bool PeekAsync(void* pDst, u32 size, IOStreamCallback pCallback,
                                              void* pCallbackArg) {
                return false;
            }

        private:
            static void InitMutex_();

        private:
            /* 0x00 (base) */
            /* 0x6F */ bool mCancelFlag;

            static bool sInitialized;
            static OSMutex sMutex;
        };

    }  // namespace ut
}  // namespace nw4hbm

#endif
