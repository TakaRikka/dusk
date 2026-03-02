#include "TextWriterBase.h"

#include "CharWriter.h"
#include "inlines.h"

//! TODO: remove this
#define NO_THIS_ASSERT
#include "Font.h"

#include "../db/assert.h"

namespace nw4hbm {
    namespace ut {

        template <typename charT>
        u32 TextWriterBase<charT>::mFormatBufferSize = 0x100;
        template <typename charT>
        charT* TextWriterBase<charT>::mFormatBuffer;
        template <typename charT>
        TagProcessorBase<charT> TextWriterBase<charT>::mDefaultTagProcessor;

        template <typename charT>
        TextWriterBase<charT>::TextWriterBase()
            : mCharSpace(0.0f), mLineSpace(0.0f), mTabWidth(4), mDrawFlag(0),
              mTagProcessor(&mDefaultTagProcessor) {}

        template <typename charT>
        TextWriterBase<charT>::~TextWriterBase() {}

        template <typename charT>
        void TextWriterBase<charT>::SetLineHeight(f32 lineHeight) {
            const Font* font = GetFont();
            int linefeed = font ? font->GetLineFeed() : 0;

            mLineSpace = lineHeight - linefeed * GetScaleV();
        }

        template <typename charT>
        f32 TextWriterBase<charT>::GetLineHeight() const {
            NW4HBM_ASSERT_VALID_PTR(241, this);
            const Font* font = GetFont();
            int linefeed = font ? font->GetLineFeed() : 0;

            return mLineSpace + linefeed * GetScaleV();
        }

        template <typename charT>
        void TextWriterBase<charT>::SetLineSpace(f32 lineSpace) {
            NW4HBM_ASSERT_VALID_PTR(261, this);
            mLineSpace = lineSpace;
        }

        template <typename charT>
        void TextWriterBase<charT>::SetCharSpace(f32 charSpace) {
            NW4HBM_ASSERT_VALID_PTR(278, this);
            mCharSpace = charSpace;
        }

        template <typename charT>
        f32 TextWriterBase<charT>::GetLineSpace() const {
            return mLineSpace;
        }

        template <typename charT>
        f32 TextWriterBase<charT>::GetCharSpace() const {
            NW4HBM_ASSERT_VALID_PTR(312, this);
            return mCharSpace;
        }

        template <typename charT>
        void TextWriterBase<charT>::SetTabWidth(int tabWidth) {
            mTabWidth = tabWidth;
        }

        template <typename charT>
        int TextWriterBase<charT>::GetTabWidth() const {
            NW4HBM_ASSERT_VALID_PTR(346, this);
            return mTabWidth;
        }

        template <typename charT>
        void TextWriterBase<charT>::SetDrawFlag(u32 flags) {
            mDrawFlag = flags;
        }

        template <typename charT>
        u32 TextWriterBase<charT>::GetDrawFlag() const {
            return mDrawFlag;
        }

        template <typename charT>
        void TextWriterBase<charT>::SetTagProcessor(TagProcessorBase<charT>* tagProcessor) {
            NW4HBM_ASSERT_VALID_PTR(114, this);
            NW4HBM_ASSERT_VALID_PTR(115, tagProcessor);
            mTagProcessor = tagProcessor;
        }

        template <typename charT>
        void TextWriterBase<charT>::ResetTagProcessor() {
            mTagProcessor = &mDefaultTagProcessor;
        }

        template <typename charT>
        TagProcessorBase<charT>& TextWriterBase<charT>::GetTagProcessor() const {
            NW4HBM_ASSERT_VALID_PTR(151, this);
            return *mTagProcessor;
        }

        template <typename charT>
        f32 TextWriterBase<charT>::CalcFormatStringWidth(const charT* format, ...) const {
            Rect rect;
            std::va_list vargs;

            va_start(vargs, format);
            CalcVStringRect(&rect, format, vargs);
            va_end(vargs);

            return rect.GetWidth();
        }

        template <typename charT>
        f32 TextWriterBase<charT>::CalcFormatStringHeight(const charT* format, ...) const {
            Rect rect;
            std::va_list vargs;

            va_start(vargs, format);
            CalcVStringRect(&rect, format, vargs);
            va_end(vargs);

            return rect.GetHeight();
        }

        template <typename charT>
        void TextWriterBase<charT>::CalcFormatStringRect(Rect* pRect, const charT* format,
                                                         ...) const {
            std::va_list vargs;

            va_start(vargs, format);
            CalcVStringRect(pRect, format, vargs);
            va_end(vargs);
        }

        template <typename charT>
        f32 TextWriterBase<charT>::AdjustCursor(float* xOrigin, float* yOrigin, const charT* str,
                                                int length) {
            f32 textWidth = 0.0f;
            f32 textHeight = 0.0f;

            if (!IsDrawFlagSet(0x333, 0x300) && !IsDrawFlagSet(0x333, 0x0)) {
                Rect textRect;

                CalcStringRect(&textRect, str, length);
                textWidth = textRect.left + textRect.right;
                textHeight = textRect.top + textRect.bottom;
            }

            if (IsDrawFlagSet(0x30, 0x10)) {
                *xOrigin -= textWidth / 2.0f;
            } else if (IsDrawFlagSet(0x30, 0x20)) {
                *xOrigin -= textWidth;
            }

            if (IsDrawFlagSet(0x300, 0x100)) {
                *yOrigin -= textHeight / 2.0f;
            } else if (IsDrawFlagSet(0x300, 0x200)) {
                *yOrigin -= textHeight;
            }

            if (IsDrawFlagSet(0x3, 0x1)) {
                f32 width = CalcLineWidth(str, length);
                f32 offset = (textWidth - width) / 2.0f;
                SetCursorX(*xOrigin + offset);
            } else if (IsDrawFlagSet(0x3, 0x2)) {
                f32 width = CalcLineWidth(str, length);
                f32 offset = textWidth - width;
                SetCursorX(*xOrigin + offset);
            } else {
                SetCursorX(*xOrigin);
            }

            if (IsDrawFlagSet(0x300, 0x300)) {
                SetCursorY(*yOrigin);
            } else {
                SetCursorY(*yOrigin + GetFontAscent());
            }

            return textWidth;
        }

        template <typename charT>
        f32 TextWriterBase<charT>::PrintImpl(const charT* str, int length) {
            NW4HBM_ASSERT_VALID_PTR(1055, this);
            NW4HBM_ASSERT_VALID_PTR(1056, str);
            NW4HBM_ASSERT_VALID_PTR(1057, GetFont());
            NW4R_ASSERT_MIN(1058, length, 0);
            f32 xOrigin = GetCursorX();
            f32 yOrigin = GetCursorY();
            f32 orgCursorX = xOrigin;
            f32 orgCursorY = yOrigin;
            f32 xCursorAdj = 0.0f;
            f32 yCursorAdj = 0.0f;
            f32 textWidth = 0.0f;
            bool bCharSpace = false;

            textWidth = AdjustCursor(&xOrigin, &yOrigin, str, length);
            xCursorAdj = orgCursorX - GetCursorX();
            yCursorAdj = orgCursorY - GetCursorY();

            PrintContext<charT> context = {this, str, xOrigin, yOrigin, 0};
            CharStrmReader reader = GetFont()->GetCharStrmReader();
            reader.Set(str);

            u16 code = reader.Next();

            while (static_cast<const charT*>(reader.GetCurrentPos()) - str <= length) {
                if (code < ' ') {  // C0 control characters under space
                    context.str = static_cast<const charT*>(reader.GetCurrentPos());
                    context.flags = 0;
                    context.flags |= bCharSpace ? FALSE : TRUE;

                    Operation operation = mTagProcessor->Process(code, &context);

                    if (operation == OPERATION_NEXT_LINE) {
                        NW4HBM_ASSERT_VALID_PTR(1097, context.str);

                        if (IsDrawFlagSet(0x3, 0x1)) {
                            int remain = length - (context.str - str);
                            f32 width = CalcLineWidth(context.str, remain);
                            f32 offset = (textWidth - width) / 2.0f;

                            SetCursorX(context.xOrigin + offset);
                        } else if (IsDrawFlagSet(0x3, 0x2)) {
                            int remain = length - (context.str - str);
                            f32 width = CalcLineWidth(context.str, remain);
                            f32 offset = textWidth - width;

                            SetCursorX(context.xOrigin + offset);
                        } else {
                            f32 width = GetCursorX() - context.xOrigin;

                            textWidth = Max(textWidth, width);
                            SetCursorX(context.xOrigin);
                        }
                        bCharSpace = false;
                    } else if (operation == OPERATION_NO_CHAR_SPACE) {
                        bCharSpace = false;
                    } else if (operation == OPERATION_CHAR_SPACE) {
                        bCharSpace = true;
                    } else if (operation == OPERATION_END_DRAW) {
                        break;
                    }

                    NW4HBM_ASSERT_VALID_PTR(1137, context.str);
                    reader.Set(context.str);
                } else {
                    f32 baseY = GetCursorY();

                    if (bCharSpace) {
                        MoveCursorX(GetCharSpace());
                    }

                    bCharSpace = true;

                    {
                        const Font* pFont = GetFont();
                        f32 adj = -pFont->GetBaselinePos() * GetScaleV();

                        MoveCursorY(adj);
                    }

                    CharWriter::Print(code);
                    SetCursorY(baseY);
                }

                code = reader.Next();
            }

            if (IsDrawFlagSet(0x300, 0x100) || IsDrawFlagSet(0x300, 0x200)) {
                SetCursorY(orgCursorY);
            } else {
                MoveCursorY(yCursorAdj);
            }

            return textWidth;
        }

        static void ut_TextWriterBase_dummy(void* buffer, int size) {
            NW4HBM_ASSERT_VALID_PTR(0, buffer);
            NW4R_ASSERT_MIN(0, size, 0);
        }

        template <typename charT>
        void TextWriterBase<charT>::CalcStringRectImpl(Rect* pRect, const charT* str, int length) {
            NW4HBM_ASSERT_VALID_PTR(1010, this);
            NW4HBM_ASSERT_VALID_PTR(1011, pRect);
            NW4HBM_ASSERT_VALID_PTR(1012, str);
            NW4R_ASSERT_MIN(1013, length, 0);
            int remain = length;
            const charT* pos = str;

            pRect->left = 0.0;
            pRect->right = 0.0;
            pRect->top = 0.0;
            pRect->bottom = 0.0;

            SetCursor(0.0f, 0.0f);

            do {
                Rect rect;
                int read = CalcLineRectImpl(&rect, pos, remain);

                pos += read;
                remain -= read;

                pRect->left = Min(pRect->left, rect.left);
                pRect->top = Min(pRect->top, rect.top);
                pRect->right = Max(pRect->right, rect.right);
                pRect->bottom = Max(pRect->bottom, rect.bottom);
            } while (remain > 0);
        }

        template <typename charT>
        void TextWriterBase<charT>::ut_TextWriterBase_unused1(Rect* pRect, const charT* str,
                                                              int length) {
            void* buffer = 0;
            int size = 0;

            NW4HBM_ASSERT_VALID_PTR(0, this);
            NW4HBM_ASSERT_VALID_PTR(0, pRect);
            NW4HBM_ASSERT_VALID_PTR(0, str);
            NW4R_ASSERT_MIN(0, length, 0);
            NW4HBM_ASSERT_VALID_PTR(0, buffer);
            NW4R_ASSERT_MIN(0, size, 1);
        }

        template <typename charT>
        int TextWriterBase<charT>::CalcLineRectImpl(Rect* pRect, const charT* str, int length) {
            NW4HBM_ASSERT_VALID_PTR(893, this);
            NW4HBM_ASSERT_VALID_PTR(894, pRect);
            NW4HBM_ASSERT_VALID_PTR(895, str);
            NW4R_ASSERT_MIN(896, length, 0);
            PrintContext<charT> context = {this, str, 0.0f, 0.0f, 0};

            const Font* font = GetFont();
            f32 x = 0.0f;
            bool bCharSpace = false;

            NW4HBM_ASSERT_VALID_PTR(902, font);
            CharStrmReader reader = font->GetCharStrmReader();

            pRect->left = 0.0f;
            pRect->right = 0.0f;
            pRect->top = Min(0.0f, GetLineHeight());
            pRect->bottom = Max(0.0f, GetLineHeight());

            reader.Set(str);

            for (u16 code = reader.Next();
                 static_cast<const charT*>(reader.GetCurrentPos()) - str <= length;
                 code = reader.Next())
            {
                if (code < ' ') {
                    Operation operation;
                    Rect rect(x, 0.0f, 0.0f, 0.0f);

                    context.str = static_cast<const charT*>(reader.GetCurrentPos());
                    context.flags = 0;
                    context.flags |= bCharSpace ? FALSE : TRUE;

                    SetCursorX(x);

                    operation = mTagProcessor->CalcRect(&rect, code, &context);

                    NW4HBM_ASSERT_VALID_PTR(936, context.str);
                    reader.Set(context.str);

                    pRect->left = Min(pRect->left, rect.left);
                    pRect->top = Min(pRect->top, rect.top);
                    pRect->right = Max(pRect->right, rect.right);
                    pRect->bottom = Max(pRect->bottom, rect.bottom);

                    x = GetCursorX();

                    if (operation == OPERATION_END_DRAW) {
                        return length;
                    } else if (operation == OPERATION_NO_CHAR_SPACE) {
                        bCharSpace = false;
                    } else if (operation == OPERATION_CHAR_SPACE) {
                        bCharSpace = true;
                    } else if (operation == OPERATION_NEXT_LINE) {
                        break;
                    }
                } else {
                    if (bCharSpace) {
                        x += GetCharSpace();
                    }

                    bCharSpace = true;

                    if (IsWidthFixed()) {
                        x += GetFixedWidth();
                    } else {
                        x += GetFont()->GetCharWidth(code) * GetScaleH();
                    }

                    pRect->left = Min(pRect->left, x);
                    pRect->right = Max(pRect->right, x);
                }
            }

            return static_cast<const charT*>(reader.GetCurrentPos()) - str;
        }

        template <typename charT>
        f32 TextWriterBase<charT>::CalcLineWidth(const charT* str, int length) {
            NW4HBM_ASSERT_VALID_PTR(861, this);
            NW4HBM_ASSERT_VALID_PTR(862, str);
            NW4R_ASSERT_MIN(863, length, 0);
            Rect rect;
            TextWriterBase<charT> myCopy(*this);

            myCopy.SetCursor(0.0f, 0.0f);
            myCopy.CalcLineRectImpl(&rect, str, length);
            return rect.GetWidth();
        }

        template <typename charT>
        charT* TextWriterBase<charT>::GetBuffer() {
            return mFormatBuffer;
        }

        template <typename charT>
        u32 TextWriterBase<charT>::GetBufferSize() {
            return mFormatBufferSize;
        }

        template <typename charT>
        charT* TextWriterBase<charT>::SetBuffer(u32 size) {
            charT* oldBuffer = mFormatBuffer;

            mFormatBuffer = NULL;
            mFormatBufferSize = size;

            return oldBuffer;
        }

        template <typename charT>
        charT* TextWriterBase<charT>::SetBuffer(charT* buffer, u32 size) {
            charT* oldBuffer = mFormatBuffer;

            mFormatBuffer = buffer;
            mFormatBufferSize = size;

            return oldBuffer;
        }

        template <typename charT>
        f32 TextWriterBase<charT>::Print(const charT* str) {
            NW4HBM_ASSERT_VALID_PTR(733, this);
            NW4HBM_ASSERT_VALID_PTR(734, str);
            return Print(str, StrLen(str));
        }

        template <typename charT>
        f32 TextWriterBase<charT>::Print(const charT* str, int length) {
            NW4HBM_ASSERT_VALID_PTR(705, this);
            NW4HBM_ASSERT_VALID_PTR(706, str);
            NW4R_ASSERT_MIN(707, length, 0);
            TextWriterBase<charT> myCopy(*this);

            f32 width = myCopy.PrintImpl(str, length);
            SetCursor(myCopy.GetCursorX(), myCopy.GetCursorY());

            return width;
        }

        template <typename charT>
        f32 TextWriterBase<charT>::VPrintf(const charT* format, std::va_list args) {
            NW4HBM_ASSERT_VALID_PTR(678, this);
            NW4HBM_ASSERT_VALID_PTR(679, format);

            // i did not know about alloca before this TU so thank you kiwi
            charT* buffer =
                mFormatBuffer ? mFormatBuffer : static_cast<charT*>(__alloca(mFormatBufferSize));

            int length = VSNPrintf(buffer, mFormatBufferSize, format, args);
            f32 width = Print(buffer, length);
            return width;
        }

        template <typename charT>
        f32 TextWriterBase<charT>::Printf(const charT* format, ...) {
            NW4HBM_ASSERT_VALID_PTR(650, this);
            NW4HBM_ASSERT_VALID_PTR(651, format);
            std::va_list vargs;

            va_start(vargs, format);
            f32 width = VPrintf(format, vargs);
            va_end(vlist);

            return width;
        }

        template <typename charT>
        void TextWriterBase<charT>::CalcStringRect(Rect* pRect, const charT* str) const {
            CalcStringRect(pRect, str, StrLen(str));
        }

        template <typename charT>
        void TextWriterBase<charT>::CalcStringRect(Rect* pRect, const charT* str,
                                                   int length) const {
            NW4HBM_ASSERT_VALID_PTR(548, this);
            NW4HBM_ASSERT_VALID_PTR(549, pRect);
            NW4HBM_ASSERT_VALID_PTR(550, str);
            NW4R_ASSERT_MIN(551, length, 0);
            TextWriterBase<charT> myCopy(*this);

            myCopy.CalcStringRectImpl(pRect, str, length);
        }

        template <typename charT>
        f32 TextWriterBase<charT>::CalcStringHeight(const charT* str) const {
            return CalcStringHeight(str, StrLen(str));
        }

        template <typename charT>
        f32 TextWriterBase<charT>::CalcStringHeight(const charT* str, int length) const {
            Rect rect;

            CalcStringRect(&rect, str, length);
            return rect.GetHeight();
        }

        template <typename charT>
        f32 TextWriterBase<charT>::CalcStringWidth(const charT* str) const {
            return CalcStringWidth(str, StrLen(str));
        }

        template <typename charT>
        f32 TextWriterBase<charT>::CalcStringWidth(const charT* str, int length) const {
            Rect rect;

            CalcStringRect(&rect, str, length);
            return rect.GetWidth();
        }

        template <typename charT>
        void TextWriterBase<charT>::CalcVStringRect(Rect* pRect, const charT* format,
                                                    std::va_list args) const {
            NW4HBM_ASSERT_VALID_PTR(0, this);
            NW4HBM_ASSERT_VALID_PTR(0, format);
            NW4HBM_ASSERT_VALID_PTR(0, pRect);
            charT* buffer =
                mFormatBuffer ? mFormatBuffer : static_cast<charT*>(__alloca(mFormatBufferSize));

            int length = VSNPrintf(buffer, mFormatBufferSize, format, args);
            CalcStringRect(pRect, buffer, length);
        }

        template class TextWriterBase<char>;
        template class TextWriterBase<wchar_t>;

    }  // namespace ut
}  // namespace nw4hbm
