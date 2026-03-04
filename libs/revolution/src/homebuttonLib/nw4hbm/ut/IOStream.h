#ifndef NW4HBM_UT_IO_STREAM_H
#define NW4HBM_UT_IO_STREAM_H

#include <revolution/types.h>

#include "RuntimeTypeInfo.h"

namespace nw4hbm {
    namespace ut {

        class IOStream {
        public:
            typedef void (*IOStreamCallback)(s32 result, IOStream* pStream, void* pCallbackArg);

            IOStream() : mAvailable(false), mCallback(NULL), mArg(NULL) {}

            /* 0x08 */ NW4HBM_UT_RUNTIME_TYPEINFO;
            /* 0x0C */ virtual ~IOStream() {}
            /* 0x10 */ virtual void Close() = 0;
            /* 0x14 */ virtual s32 Read(void* pDst, u32 size);
            /* 0x18 */ virtual bool ReadAsync(void* pDst, u32 size, IOStreamCallback pCallback,
                                              void* pCallbackArg);
            /* 0x1C */ virtual void Write(const void* pSrc, u32 size);
            /* 0x20 */ virtual bool WriteAsync(const void* pSrc, u32 size,
                                               IOStreamCallback pCallback, void* pCallbackArg);
            /* 0x24 */ virtual bool IsBusy() const;
            /* 0x28 */ virtual bool CanAsync() const = 0;
            /* 0x2C */ virtual bool CanRead() const = 0;
            /* 0x30 */ virtual bool CanWrite() const = 0;
            /* 0x34 */ virtual u32 GetOffsetAlign() const { return 1; }
            /* 0x38 */ virtual u32 GetSizeAlign() const { return 1; }
            /* 0x3C */ virtual u32 GetBufferAlign() const { return 1; }

            bool IsAvailable() const { return mAvailable; }

        protected:
            /* 0x00 (vtable) */
            /* 0x04 */ bool mAvailable;
            /* 0x08 */ s32 mAsyncResult;
            /* 0x0C */ IOStreamCallback mCallback;
            /* 0x10 */ void* mArg;
        };  // size = 0x14

    };  // namespace ut
};  // namespace nw4hbm

#endif
