#ifndef NW4HBM_UT_TAG_PROCESSOR_BASE_H
#define NW4HBM_UT_TAG_PROCESSOR_BASE_H

#include <revolution/types.h>

#include "Rect.h"

namespace nw4hbm {
    namespace ut {
        typedef enum Operation {
            /* 0 */ OPERATION_DEFAULT = 0,
            /* 1 */ OPERATION_NO_CHAR_SPACE,
            /* 2 */ OPERATION_CHAR_SPACE,
            /* 3 */ OPERATION_NEXT_LINE,
            /* 4 */ OPERATION_END_DRAW,
            /* 5 */ NUM_OF_OPERATION
        } Operation;

        template <typename>
        class TextWriterBase;

        template <typename T>
        struct PrintContext {
            /* 0x00 */ TextWriterBase<T>* writer;
            /* 0x04 */ const T* str;
            /* 0x08 */ f32 xOrigin;
            /* 0x0C */ f32 yOrigin;
            /* 0x10 */ u32 flags;
        };

        template <typename T>
        class TagProcessorBase {
        public:
            TagProcessorBase();

            /* 0x08 */ virtual ~TagProcessorBase();
            /* 0x0C */ virtual Operation Process(u16 code, PrintContext<T>* context);
            /* 0x10 */ virtual Operation CalcRect(Rect* pRect, u16 code, PrintContext<T>* context);

            void ProcessLinefeed(PrintContext<T>* context);
            void ProcessTab(PrintContext<T>* context);

            /* 0x00 (vtable) */
        };  // size = 0x04

    }  // namespace ut
}  // namespace nw4hbm

#endif
