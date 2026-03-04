#ifndef NW4HBM_UT_DVD_FILE_STREAM_H
#define NW4HBM_UT_DVD_FILE_STREAM_H

#include "FileStream.h"

#include <revolution/dvd.h>

namespace nw4hbm {
    namespace ut {

        class DvdFileStream : public FileStream {
        public:
            explicit DvdFileStream(s32 entrynum);
            DvdFileStream(const DVDFileInfo* info, bool close);

            /* 0x08 */ NW4HBM_UT_RUNTIME_TYPEINFO;
            /* 0x0C */ virtual ~DvdFileStream();
            /* 0x10 */ virtual void Close();
            /* 0x14 */ virtual s32 Read(void* pDst, u32 size);
            /* 0x18 */ virtual bool ReadAsync(void* pDst, u32 size, IOStreamCallback pCallback,
                                              void* pCallbackArg);
            /* 0x24 */ virtual bool IsBusy() const { return mIsBusy; }
            /* 0x28 */ virtual bool CanAsync() const { return true; }
            /* 0x2C */ virtual bool CanRead() const { return true; }
            /* 0x30 */ virtual bool CanWrite() const { return false; }
            /* 0x34 */ virtual u32 GetOffsetAlign() const { return 4; }
            /* 0x38 */ virtual u32 GetSizeAlign() const { return 32; }
            /* 0x3C */ virtual u32 GetBufferAlign() const { return 32; }
            /* 0x40 */ virtual u32 GetSize() const { return mFilePosition.GetFileSize(); }
            /* 0x44 */ virtual void Seek(s32 offset, u32 origin);
            /* 0x48 */ virtual void Cancel();
            /* 0x4C */ virtual bool CancelAsync(IOStreamCallback pCallback, void* pCallbackArg);
            /* 0x50 */ virtual bool CanSeek() const { return true; }
            /* 0x54 */ virtual bool CanCancel() const { return true; }
            /* 0x58 */ virtual u32 Tell() const { return mFilePosition.Tell(); }
            /* 0x5C */ virtual s32 Peek(void* pDst, u32 size);
            /* 0x60 */ virtual bool PeekAsync(void* pDst, u32 size, IOStreamCallback pCallback,
                                              void* pCallbackArg);

            bool Open(s32 entrynum);
            bool Open(const DVDFileInfo* info, bool close);
            void SetPriority(s32 priority) { mPriority = priority; }

        private:
            typedef struct DvdFileStreamInfo {
                /* 0x00 */ DVDFileInfo dvdInfo;
                /* 0x3C */ DvdFileStream* stream;
            } DvdFileStreamInfo;

        private:
            static void DvdAsyncCallback_(s32 result, DVDFileInfo* info);
            static void DvdCBAsyncCallback_(s32 result, DVDCommandBlock* pBlock);

            void Initialize_();
            u32 AdjustReadLength_(u32 len);

        private:
            /* 0x00 (base) */
            /* 0x14 */ FilePosition mFilePosition;
            /* 0x1C */ IOStreamCallback mCancelCallback;
            /* 0x20 */ void* mCancelArg;
            /* 0x24 */ volatile bool mIsCanceling;
            /* 0x28 */ DvdFileStreamInfo mFileInfo;
            /* 0x68 */ s32 mPriority;
            /* 0x6C */ volatile bool mIsBusy;
            /* 0x6D */ bool mCloseOnDestroyFlg;
            /* 0x6E */ bool mCloseEnableFlg;
        };

    }  // namespace ut
}  // namespace nw4hbm

#endif
