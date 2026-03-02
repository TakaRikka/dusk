#ifndef NW4R_DB_DIRECTPRINT_H
#define NW4R_DB_DIRECTPRINT_H

#include <revolution/gx.h>

namespace nw4hbm {
    namespace db {
        void DirectPrint_Init();
        bool DirectPrint_IsActive();
        void DirectPrint_EraseXfb(int posh, int posv, int sizeh, int sizev);
        void DirectPrint_ChangeXfb(void* framebuf, u16 width, u16 height);
        void DirectPrint_StoreCache();
        void DirectPrint_DrawString(int posh, int posv, bool turnOver, const char* format, ...);
        void DirectPrint_SetColor(u8 r, u8 g, u8 b);

        namespace detail {
            void DirectPrint_DrawStringToXfb(int posh, int posv, const char* format, va_list vargs, bool turnOver, bool backErase);
            void* DirectPrint_SetupFB(const GXRenderModeObj* rmode);
        }
    }  // namespace db
}  // namespace nw4r

#endif
