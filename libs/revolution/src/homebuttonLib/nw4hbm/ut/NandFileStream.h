#ifndef NW4HBM_UT_NAND_FILE_STREAM_H
#define NW4HBM_UT_NAND_FILE_STREAM_H

#include "FileStream.h"

#include <revolution/nand.h>
#include "global.h"

namespace nw4hbm {
    namespace ut {

        class NandFileStream : public FileStream {
        public:
            NandFileStream(const char* path, u32 mode);
            NandFileStream(const NANDFileInfo* info, u32 mode, bool enableClose);

            bool Open(const char* path, u32 mode);
            bool Open(const NANDFileInfo* info, u32 mode, bool enableClose) NO_INLINE;

            /* 0x08 */ NW4HBM_UT_RUNTIME_TYPEINFO;
            /* 0x0C */ virtual ~NandFileStream();
            /* 0x10 */ virtual void Close();
            /* 0x14 */ virtual s32 Read(void* pDst, u32 size);
            /* 0x18 */ virtual bool ReadAsync(void* pDst, u32 size, IOStreamCallback pCallback,
                                              void* pCallbackArg);
            /* 0x1C */ virtual void Write(const void* pSrc, u32 size);
            /* 0x20 */ virtual bool WriteAsync(const void* pSrc, u32 size,
                                               IOStreamCallback pCallback, void* pCallbackArg);
            /* 0x44 */ virtual void Seek(s32 offset, u32 origin);

            // the order is important
            /* 0x24 */ virtual bool IsBusy() const { return mIsBusy; }
            /* 0x58 */ virtual u32 Tell() const { return mFilePosition.Tell(); }
            /* 0x40 */ virtual u32 GetSize() const { return mFilePosition.GetFileSize(); }
            /* 0x28 */ virtual bool CanAsync() const { return true; }
            /* 0x50 */ virtual bool CanSeek() const { return true; }
            /* 0x2C */ virtual bool CanRead() const { return mCanRead; }
            /* 0x30 */ virtual bool CanWrite() const { return mCanWrite; }
            /* 0x54 */ virtual bool CanCancel() const { return false; }
            /* 0x34 */ virtual u32 GetOffsetAlign() const { return 1; }
            /* 0x38 */ virtual u32 GetSizeAlign() const { return 32; }
            /* 0x3C */ virtual u32 GetBufferAlign() const { return 32; }

        private:
            typedef struct NandFileStreamInfo {
                /* 0x000 */ NANDCommandBlock nandBlock;
                /* 0x0B8 */ NANDFileInfo nandInfo;
                /* 0x144 */ NandFileStream* stream;
            } NandFileStreamInfo;

            static void NandAsyncCallback_(s32 result, NANDCommandBlock* pBlock);

            void Initialize_();
            bool ReadAsyncImpl(void* buf, u32 length, IOStreamCallback pCallback,
                               void* pCallbackArg);
            void ReadAsyncSetArgs(IOStreamCallback pCallback, void* pCallbackArg);

            /* 0x000 (base) */
            /* 0x014 */ FilePosition mFilePosition;
            /* 0x01C */ NandFileStreamInfo mFileInfo;
            /* 0x164 */ bool mCanRead;
            /* 0x165 */ bool mCanWrite;
            /* 0x166 */ volatile bool mIsBusy;
            /* 0x167 */ bool mCloseOnDestroyFlg;
            /* 0x168 */ bool mCloseEnableFlg;
        };  // size = 0x16C

    }  // namespace ut
}  // namespace nw4hbm

#endif
