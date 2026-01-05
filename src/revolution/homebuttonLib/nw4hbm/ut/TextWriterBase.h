#ifndef NW4HBM_UT_TEXT_WRITER_BASE_H
#define NW4HBM_UT_TEXT_WRITER_BASE_H

#include <revolution/types.h>

#include "CharWriter.h"
#include "Rect.h"

#include <string>
#include <stdio>

#include "TagProcessorBase.h"

namespace nw4hbm {
    namespace ut {

        struct Rect;
        template <typename>
        class TagProcessorBase;

        template <typename T>
        class TextWriterBase : public CharWriter {
        public:
            enum DrawFlag {
                // Align text lines
                DRAWFLAG_ALIGN_TEXT_BASELINE = 0,
                DRAWFLAG_ALIGN_TEXT_CENTER = (1 << 0),
                DRAWFLAG_ALIGN_TEXT_RIGHT = (1 << 1),

                // Align text block (horizontal)
                DRAWFLAG_ALIGN_H_BASELINE = 0,
                DRAWFLAG_ALIGN_H_CENTER = (1 << 4),
                DRAWFLAG_ALIGN_H_RIGHT = (1 << 5),

                // Align text block (vertical)
                DRAWFLAG_ALIGN_V_BASELINE = 0,
                DRAWFLAG_ALIGN_V_CENTER = (1 << 8),
                DRAWFLAG_ALIGN_V_TOP = (1 << 9),

                // Mask constants
                DRAWFLAG_MASK_ALIGN_TEXT = DRAWFLAG_ALIGN_TEXT_BASELINE |
                                           DRAWFLAG_ALIGN_TEXT_CENTER | DRAWFLAG_ALIGN_TEXT_RIGHT,

                DRAWFLAG_MASK_ALIGN_H =
                    DRAWFLAG_ALIGN_H_BASELINE | DRAWFLAG_ALIGN_H_CENTER | DRAWFLAG_ALIGN_H_RIGHT,

                DRAWFLAG_MASK_ALIGN_V =
                    DRAWFLAG_ALIGN_V_BASELINE | DRAWFLAG_ALIGN_V_CENTER | DRAWFLAG_ALIGN_V_TOP,
            };

            TextWriterBase();
            ~TextWriterBase();

            void SetLineHeight(f32 lineHeight);
            f32 GetLineHeight() const;

            void SetLineSpace(f32 lineSpace);
            void SetCharSpace(f32 charSpace);

            f32 GetLineSpace() const;
            f32 GetCharSpace() const;

            void SetTabWidth(int tabWidth);
            int GetTabWidth() const;

            void SetDrawFlag(u32 flags);
            u32 GetDrawFlag() const;

            void SetTagProcessor(TagProcessorBase<T>* tagProcessor);
            void ResetTagProcessor();
            TagProcessorBase<T>& GetTagProcessor() const;

            f32 CalcFormatStringWidth(const T* format, ...) const;
            f32 CalcFormatStringHeight(const T* format, ...) const;

            void CalcFormatStringRect(Rect* pRect, const T* format, ...) const;
            void CalcVStringRect(Rect* pRect, const T* format, va_list args) const;

            f32 CalcStringWidth(const T* str, int length) const;
            f32 CalcStringWidth(const T* str) const;

            f32 CalcStringHeight(const T* str, int length) const;
            f32 CalcStringHeight(const T* str) const;

            void CalcStringRect(Rect* pRect, const T* str, int length) const;
            void CalcStringRect(Rect* pRect, const T* str) const;

            f32 Printf(const T* format, ...);
            f32 VPrintf(const T* format, va_list args);
            f32 Print(const T* str, int length);
            f32 Print(const T* str);

            static T* SetBuffer(T* buf, u32 size);
            static T* SetBuffer(u32 size);

            static u32 GetBufferSize();
            static T* GetBuffer();

            static int VSNPrintf(T* buffer, u32 count, const T* format, va_list arg) {
                return sizeof(T) == sizeof(char) ?
                           std::vsnprintf((char*)buffer, count, (const char*)format, arg) :
                           std::vswprintf((wchar_t*)buffer, count, (const wchar_t*)format, arg);
            }
            static int StrLen(const T* str) {
                return sizeof(T) == sizeof(char) ? std::strlen((const char*)str) :
                                                   std::wcslen((const wchar_t*)str);
            }

            void ut_TextWriterBase_unused1(Rect* pRect, const T* str, int length);

            f32 CalcLineWidth(const T* str, int length);
            int CalcLineRectImpl(Rect* pRect, const T* str, int length);
            void CalcStringRectImpl(Rect* pRect, const T* str, int length);
            f32 PrintImpl(const T* str, int length);
            f32 AdjustCursor(f32* xOrigin, f32* yOrigin, const T* str, int length);

            bool IsDrawFlagSet(u32 mask, u32 flag) const { return (mDrawFlag & mask) == flag; }

        private:
            /* 0x4C */ f32 mCharSpace;
            /* 0x50 */ f32 mLineSpace;
            /* 0x54 */ int mTabWidth;
            /* 0x58 */ u32 mDrawFlag;
            /* 0x5C */ TagProcessorBase<T>* mTagProcessor;

        private:
            static T* mFormatBuffer;
            static u32 mFormatBufferSize;
            static TagProcessorBase<T> mDefaultTagProcessor;
            static const int DEFAULT_FORMAT_BUFFER_SIZE = 256;
            static const u32 DRAWFLAG_MASK_ALL =
                DRAWFLAG_MASK_ALIGN_TEXT | DRAWFLAG_MASK_ALIGN_H | DRAWFLAG_MASK_ALIGN_V;
        };  // size = 0x60

    }  // namespace ut
}  // namespace nw4hbm

#endif
