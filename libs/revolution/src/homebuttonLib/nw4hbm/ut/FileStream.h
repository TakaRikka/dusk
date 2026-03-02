#ifndef NW4HBM_UT_FILE_STREAM_H
#define NW4HBM_UT_FILE_STREAM_H

#include "IOStream.h"

namespace nw4hbm {
    namespace ut {

        class FileStream : public IOStream {
        public:
            FileStream() {}

            /* 0x08 */ NW4HBM_UT_RUNTIME_TYPEINFO;
            /* 0x0C */ virtual ~FileStream() {}
            /* 0x40 */ virtual u32 GetSize() const = 0;
            /* 0x44 */ virtual void Seek(s32 offset, u32 origin);
            /* 0x48 */ virtual void Cancel();
            /* 0x4C */ virtual bool CancelAsync(IOStreamCallback pCallback, void* pCallbackArg);
            /* 0x50 */ virtual bool CanSeek() const = 0;
            /* 0x54 */ virtual bool CanCancel() const = 0;
            /* 0x58 */ virtual u32 Tell() const = 0;

        protected:
            class FilePosition {
            public:
                FilePosition() : mFileSize(0), mPosition(0) {}

                u32 GetFileSize() const { return mFileSize; }
                void SetFileSize(u32 size) { mFileSize = size; }

                u32 Tell() const { return mPosition; }

                u32 Skip(s32 offset);
                u32 Append(s32 offset);
                void Seek(s32 offset, u32 origin);

            private:
                /* 0x00 */ u32 mFileSize;
                /* 0x04 */ u32 mPosition;
            };  // size = 0x08
        };  // size = 0x14

    }  // namespace ut
}  // namespace nw4hbm

#endif
