#include "IOStream.h"

#include "../db/assert.h"

namespace nw4hbm {
    namespace ut {

        NW4HBM_UT_GET_RUNTIME_TYPEINFO(IOStream);

        s32 IOStream::Read(void* pDst, u32 size) {
            #pragma unused(pDst)
            #pragma unused(size)
            #pragma unused(pCallback)
            #pragma unused(pCallbackArg)
            NW4R_DB_ASSERTMSG(41, CanRead(), "Stream don't support READ function\n");
            return 0;
        }

        bool IOStream::ReadAsync(void* pDst, u32 size, IOStreamCallback pCallback,
                                 void* pCallbackArg) {
            #pragma unused(pDst)
            #pragma unused(size)
            #pragma unused(pCallback)
            #pragma unused(pCallbackArg)
            NW4R_DB_ASSERTMSG(62, CanRead(), "Stream don't support READ function\n");
            NW4R_DB_ASSERTMSG(63, CanAsync(), "Stream don't support ASYNC function\n");
            return false;
        }

        void IOStream::Write(const void* pSrc, u32 size) {
            #pragma unused(pSrc)
            #pragma unused(size)
            NW4R_DB_ASSERTMSG(82, CanWrite(), "Stream don't support WRITE function\n");
        }

        bool IOStream::WriteAsync(const void* pSrc, u32 size, IOStreamCallback pCallback,
                                  void* pCallbackArg) {
            #pragma unused(pSrc)
            #pragma unused(size)
            #pragma unused(pCallback)
            #pragma unused(pCallbackArg)
            NW4R_DB_ASSERTMSG(102, CanWrite(), "Stream don't support WRITE function\n");
            NW4R_DB_ASSERTMSG(103, CanAsync(), "Stream don't support ASYNC function\n");
            return false;
        }

        bool IOStream::IsBusy() const {
            NW4R_DB_ASSERTMSG(142, CanAsync(), "Stream don't support ASYNC function\n");
            return false;
        }

    }  // namespace ut
}  // namespace nw4hbm
