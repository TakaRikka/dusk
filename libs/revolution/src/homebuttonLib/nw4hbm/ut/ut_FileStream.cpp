#include "FileStream.h"

#include "../db/assert.h"
#include "inlines.h"

namespace nw4hbm {
    namespace ut {

        NW4HBM_UT_GET_DERIVED_RUNTIME_TYPEINFO(FileStream, IOStream);

        void FileStream::Seek(s32 offset, u32 origin) {
            NW4R_DB_ASSERTMSG(44, CanSeek(), "Stream don't support SEEK function\n");
        }

        void FileStream::Cancel() {
            NW4R_DB_ASSERTMSG(60, CanCancel(), "Stream don't support CANCEL function\n");
        }

        bool FileStream::CancelAsync(IOStreamCallback pCallback, void* pCallbackArg) {
            #pragma unused(pCallback)
            #pragma unused(pCallbackArg)
            NW4R_DB_ASSERTMSG(78, CanCancel(), "Stream don't support CANCEL function\n");
            NW4R_DB_ASSERTMSG(79, CanAsync(), "Stream don't support ASYNC function\n");
            return true;
        }

        u32 FileStream::FilePosition::Skip(s32 offset) {
            if (offset != 0) {
                s64 newOffset = mPosition + offset;
                mPosition = Clamp<s64>(newOffset, 0, mFileSize);
            }

            return mPosition;
        }

        u32 FileStream::FilePosition::Append(s32 offset) {
            s64 newOffset = mPosition + offset;

            if (newOffset < 0) {
                mPosition = 0;
            } else {
                mPosition = newOffset;
                mFileSize = Max(mPosition, mFileSize);
            }

            return mPosition;
        }

        void FileStream::FilePosition::Seek(s32 offset, u32 origin) {
            switch (origin) {
            case SEEK_BEG: {
                mPosition = 0;
                break;
            }

            case SEEK_END: {
                mPosition = mFileSize;
                break;
            }

            case SEEK_CUR:
            default: {
                break;
            }
            }

            Skip(offset);
        }

    }  // namespace ut
}  // namespace nw4hbm
