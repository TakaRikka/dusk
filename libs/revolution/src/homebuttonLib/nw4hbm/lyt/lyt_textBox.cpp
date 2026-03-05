#include "../ut/CharStrmReader.h"
#include "../ut/Font.h"
#include "../ut/ResFont.h"

#include "common.h"
#include "layout.h"
#include "material.h"
#include "pane.h"
#include "textBox.h"

#include <new>

namespace nw4hbm {
    namespace lyt {
        NW4HBM_UT_GET_DERIVED_RUNTIME_TYPEINFO(TextBox, Pane);
    }
}  // namespace nw4hbm

namespace {
    using namespace nw4hbm;

    u8 ClampColor(s16 colVal);

    ut::Color GetColor(const GXColorS10& src) {
        GXColor dst;

        dst.r = ClampColor(src.r);
        dst.g = ClampColor(src.g);
        dst.b = ClampColor(src.b);
        dst.a = ClampColor(src.a);

        return dst;
    }

    u8 ClampColor(s16 value) {
        return value < 0 ? 0 : (value > 255 ? 255 : value);
    }

    template <typename T>
    int CalcLineRectImpl(ut::Rect* pRect, ut::TextWriterBase<T>* pTextWriter, const T* str,
                         int length, f32 maxWidth, bool* pbOver) {
        NW4HBM_ASSERT_VALID_PTR(71, pTextWriter);
        NW4HBM_ASSERT_VALID_PTR(72, pRect);
        NW4HBM_ASSERT_VALID_PTR(73, str);
        NW4R_ASSERT_MIN(74, length, 0);
        ut::PrintContext<T> context = {pTextWriter, str, 0.0f, 0.0f, 0};
        const ut::Font* font = pTextWriter->GetFont();

        f32 x = 0.0f;
        bool bCharSpace = false;

        NW4HBM_ASSERT_VALID_PTR(83, font);
        ut::CharStrmReader reader = font->GetCharStrmReader();
        const T* prStrPos = static_cast<const T*>(reader.GetCurrentPos());

        pRect->left = 0.0f;
        pRect->right = 0.0f;
        pRect->top = ut::Min(0.0f, pTextWriter->GetLineHeight());
        pRect->bottom = ut::Max(0.0f, pTextWriter->GetLineHeight());

        *pbOver = false;

        reader.Set(str);

        ut::Rect prMaxRect = *pRect;

        for (u16 code = reader.Next();
             static_cast<const T*>(reader.GetCurrentPos()) - str <= length;
             prStrPos = static_cast<const T*>(reader.GetCurrentPos()), code = reader.Next(),
                 prMaxRect = *pRect)
        {
            if ((int)code < L' ') {
                ut::Operation operation;
                ut::Rect rect(x, 0.0f, 0.0f, 0.0f);

                context.str = static_cast<const T*>(reader.GetCurrentPos());
                context.flags = 0;
                context.flags |= bCharSpace ? 0 : 1;

                pTextWriter->SetCursorX(x);

                operation = pTextWriter->GetTagProcessor().CalcRect(&rect, code, &context);

                NW4HBM_ASSERT_VALID_PTR(123, context.str);
                reader.Set(context.str);

                pRect->left = ut::Min(pRect->left, rect.left);
                pRect->top = ut::Min(pRect->top, rect.top);
                pRect->right = ut::Max(pRect->right, rect.right);
                pRect->bottom = ut::Max(pRect->bottom, rect.bottom);

                x = pTextWriter->GetCursorX();

                if (pRect->GetWidth() > maxWidth) {
                    *pbOver = true;
                    break;
                }

                if (operation == ut::OPERATION_END_DRAW) {
                    return length;
                } else if (operation == ut::OPERATION_NO_CHAR_SPACE) {
                    bCharSpace = false;
                } else if (operation == ut::OPERATION_CHAR_SPACE) {
                    bCharSpace = true;
                } else if (operation == ut::OPERATION_NEXT_LINE) {
                    goto end_draw;
                }
            } else {
                if (bCharSpace) {
                    x += pTextWriter->GetCharSpace();
                }

                bCharSpace = true;

                if (pTextWriter->IsWidthFixed()) {
                    x += pTextWriter->GetFixedWidth();
                } else {
                    x += pTextWriter->GetFont()->GetCharWidth(code) * pTextWriter->GetScaleH();
                }

                pRect->left = ut::Min(pRect->left, x);
                pRect->right = ut::Max(pRect->right, x);

                if (pRect->GetWidth() > maxWidth) {
                    *pbOver = true;
                    goto end_draw;
                }
            }
        }

    end_draw:
        if (*pbOver && prStrPos) {
            *pRect = prMaxRect;
            return prStrPos - str;
        } else {
            return static_cast<const T*>(reader.GetCurrentPos()) - str;
        }
    }

    template <typename T>
    void CalcStringRectImpl(ut::Rect* pRect, ut::TextWriterBase<T>* pTextWriter, const T* str,
                            int length, f32 maxWidth) {
        NW4HBM_ASSERT_VALID_PTR(218, pTextWriter);
        NW4HBM_ASSERT_VALID_PTR(219, pRect);
        NW4HBM_ASSERT_VALID_PTR(220, str);
        NW4R_ASSERT_MIN(221, length, 0);
        int remain = length;
        const T* pos = str;

        pRect->left = 0.0f;
        pRect->right = 0.0f;
        pRect->top = 0.0f;
        pRect->bottom = 0.0f;
        pTextWriter->SetCursor(0.0f, 0.0f);

        do {
            ut::Rect rect;
            bool bOver;
            int read = CalcLineRectImpl(&rect, pTextWriter, pos, remain, maxWidth, &bOver);

            if (bOver) {
                CalcLineRectImpl(&rect, pTextWriter, L"\n", 1, maxWidth, &bOver);
            }

            pos += read;
            remain -= read;

            pRect->left = ut::Min(pRect->left, rect.left);
            pRect->top = ut::Min(pRect->top, rect.top);
            pRect->right = ut::Max(pRect->right, rect.right);
            pRect->bottom = ut::Max(pRect->bottom, rect.bottom);
        } while (remain > 0);
    }

    template <typename T>
    int CalcLineStrNum(f32* pWidth, ut::TextWriterBase<T>* pTextWriter, const T* str, int length,
                       f32 maxWidth, bool* pbOver) {
        NW4HBM_ASSERT_VALID_PTR(275, pTextWriter);
        NW4HBM_ASSERT_VALID_PTR(276, str);
        NW4R_ASSERT_MIN(277, length, 0);

        ut::Rect rect;
        ut::TextWriterBase<T> myCopy = *pTextWriter;
        myCopy.SetCursor(0.0f, 0.0f);

        int ret = CalcLineRectImpl(&rect, &myCopy, str, length, maxWidth, pbOver);

        *pWidth = rect.GetWidth();
        return ret;
    }

    template <typename T>
    void CalcStringRect(ut::Rect* pRect, ut::TextWriterBase<T>* pTextWriter, const T* str,
                        int length, f32 maxWidth) {
        NW4HBM_ASSERT_VALID_PTR(311, pTextWriter);
        NW4HBM_ASSERT_VALID_PTR(312, pRect);
        NW4HBM_ASSERT_VALID_PTR(313, str);
        NW4R_ASSERT_MIN(314, length, 0);
        ut::TextWriterBase<T> myCopy = *pTextWriter;
        CalcStringRectImpl(pRect, &myCopy, str, length, maxWidth);
    }

}  // namespace

namespace nw4hbm {
    namespace lyt {

        TextBox::TextBox(u16 allocStrLen, const wchar_t* str, const ut::Font* pFont) : Pane() {
            Init(allocStrLen);
            SetString(str);
            SetFont(pFont);
        }

        TextBox::TextBox(const res::TextBox* pBlock, const ResBlockSet& resBlockSet)
            : Pane(pBlock) {
            u16 allocStrBufLen = pBlock->textBufBytes / sizeof(wchar_t);
            if (allocStrBufLen) {
                allocStrBufLen = allocStrBufLen - 1;
            }

            Init(allocStrBufLen);

            if (pBlock->textStrBytes >= 2 && mTextBuf != NULL) {
                const wchar_t* pBlockText =
                    detail::ConvertOffsToPtr<wchar_t>(pBlock, pBlock->textStrOffset);
                const u16 resStrLen = pBlock->textStrBytes / sizeof(wchar_t) - 1;

                SetString(pBlockText, 0, resStrLen);
            }

            for (int i = 0; i < (int)ARRAY_SIZE(mTextColors); i++) {
                mTextColors[i] = pBlock->textCols[i];
            }

            mFontSize = pBlock->fontSize;
            mTextPosition = pBlock->textPosition;
            mCharSpace = pBlock->charSpace;
            mLineSpace = pBlock->lineSpace;

            NW4HBM_ASSERT_CHECK_NULL(395, resBlockSet.pFontList);
            NW4HBM_ASSERT(396, pBlock->fontIdx < resBlockSet.pFontList->fontNum);

            const res::Font* fonts = detail::ConvertOffsToPtr<res::Font>(
                resBlockSet.pFontList, sizeof(*resBlockSet.pFontList));

            const char* fontName =
                detail::ConvertOffsToPtr<char>(fonts, fonts[pBlock->fontIdx].nameStrOffset);

            if (ut::Font* pFont = resBlockSet.pResAccessor->GetFont(fontName)) {
                mpFont = pFont;
            } else if (void* fontRes =
                           resBlockSet.pResAccessor->GetResource('font', fontName, NULL))
            {
                if (void* pMemFont = Layout::AllocMemory(sizeof(ut::ResFont))) {
                    ut::ResFont* pResFont = new (pMemFont) ut::ResFont();
                    bool bSuccess = pResFont->SetResource(fontRes);

                    if (!bSuccess) {
                        NW4R_DB_ASSERTMSG(410, false, "Fail to load ResFont.");
                    }

                    mpFont = pResFont;
                    mTextBoxFlag.allocFont = true;
                }
            }

            if (void* pMemMaterial = Layout::AllocMemory(sizeof(Material))) {
                NW4HBM_ASSERT_CHECK_NULL(420, resBlockSet.pMaterialList);

                const u32* matOffsTbl = detail::ConvertOffsToPtr<u32>(
                    resBlockSet.pMaterialList, sizeof(*resBlockSet.pMaterialList));

                const res::Material* pResMaterial = detail::ConvertOffsToPtr<res::Material>(
                    resBlockSet.pMaterialList, matOffsTbl[pBlock->materialIdx]);

                mpMaterial = new (pMemMaterial) Material(pResMaterial, resBlockSet);
            }
        }

        void TextBox::Init(u16 allocStrLen) {
            mTextBuf = NULL;
            mTextBufBytes = 0;
            mTextLen = 0;

            mpFont = NULL;
            mFontSize = Size(0.0f, 0.0f);

            SetTextPositionH(HORIZONTALPOSITION_CENTER);
            SetTextPositionV(VERTICALPOSITION_CENTER);

            mLineSpace = 0.0f;
            mCharSpace = 0.0f;
            mpTagProcessor = NULL;

            memset(&mTextBoxFlag, 0, sizeof(mTextBoxFlag));

            if (allocStrLen != 0) {
                AllocStringBuffer(allocStrLen);
            }
        }

        TextBox::~TextBox() {
            SetFont(NULL);

            if (mpMaterial != NULL && !mpMaterial->IsUserAllocated()) {
                mpMaterial->~Material();
                Layout::FreeMemory(mpMaterial);
                mpMaterial = NULL;
            }

            FreeStringBuffer();
        }

        ut::Color TextBox::GetVtxColor(u32 idx) const {
            NW4HBM_ASSERT(467, idx < VERTEXCOLOR_MAX);
            return GetTextColor(idx / 2);
        }

        void TextBox::SetVtxColor(u32 idx, ut::Color value) {
            NW4HBM_ASSERT(478, idx < VERTEXCOLOR_MAX);
            SetTextColor(idx / 2, value);
        }

        u8 TextBox::GetVtxColorElement(u32 idx) const {
            NW4HBM_ASSERT(486, idx < ANIMTARGET_VERTEXCOLOR_MAX);
            return reinterpret_cast<const u8*>(mTextColors + idx / 8)[idx % 4];
        }

        void TextBox::SetVtxColorElement(u32 idx, u8 value) {
            NW4HBM_ASSERT(494, idx < ANIMTARGET_VERTEXCOLOR_MAX);
            reinterpret_cast<u8*>(mTextColors + idx / 8)[idx % 4] = value;
        }

        const ut::Rect TextBox::GetTextDrawRect(const DrawInfo& drawInfo) const {
            ut::WideTextWriter writer;

            writer.SetFont(*mpFont);
            writer.SetFontSize(mFontSize.width, mFontSize.height);
            writer.SetLineSpace(mLineSpace);
            writer.SetCharSpace(mCharSpace);

            if (mpTagProcessor) {
                writer.SetTagProcessor(mpTagProcessor);
            }

            ut::Rect rect = GetTextDrawRect(&writer);

            if (drawInfo.IsYAxisUp()) {
                rect.top = -rect.top;
                rect.bottom = -rect.bottom;
            }

            return rect;
        }

        void TextBox::DrawSelf(const DrawInfo& drawInfo) {
            if (mTextBuf == NULL || mpFont == NULL || mpMaterial == NULL) {
                return;
            }

            LoadMtx(drawInfo);

            ut::WideTextWriter writer;
            writer.SetFont(*mpFont);
            writer.SetFontSize(mFontSize.width, mFontSize.height);
            writer.SetLineSpace(mLineSpace);
            writer.SetCharSpace(mCharSpace);

            ut::Color topCol = detail::MultipleAlpha(mTextColors[TEXTCOLOR_TOP], mGlbAlpha);
            ut::Color btmCol = detail::MultipleAlpha(mTextColors[TEXTCOLOR_BOTTOM], mGlbAlpha);
            writer.SetGradationMode(topCol != btmCol ? ut::CharWriter::GRADMODE_V :
                                                       ut::CharWriter::GRADMODE_NONE);

            writer.SetTextColor(topCol, btmCol);

            ut::Color minCol = GetColor(mpMaterial->GetTevColor(TEVCOLOR_REG0));
            ut::Color maxCol = GetColor(mpMaterial->GetTevColor(TEVCOLOR_REG1));
            writer.SetColorMapping(minCol, maxCol);

            if (mpTagProcessor) {
                writer.SetTagProcessor(mpTagProcessor);
            }

            writer.SetupGX();

            ut::Rect textRect = GetTextDrawRect(&writer);

            f32 hMag = GetTextMagH();
            wchar_t* strPos = mTextBuf;
            f32 textWidth = textRect.GetWidth();

            writer.SetCursor(textRect.left, textRect.top);

            for (int remain = mTextLen; remain > 0;) {
                f32 lineWidth;
                bool bOver;
                int lineStrNum =
                    CalcLineStrNum(&lineWidth, &writer, strPos, remain, mSize.width, &bOver);
                f32 textPosX = hMag * (textWidth - lineWidth);

                writer.SetCursorX(textRect.left + textPosX);
                writer.Print(strPos, lineStrNum);

                if (bOver) {
                    writer.Print(L"\n");
                }

                strPos += lineStrNum;
                remain -= lineStrNum;
            }
        }

        u16 TextBox::GetStringBufferLength() const {
            if (mTextBufBytes == 0) {
                return 0;
            } else {
                NW4HBM_ASSERT(605, mTextBufBytes >= sizeof(wchar_t));
            }

            return mTextBufBytes / sizeof(wchar_t) - 1;
        }

        void TextBox::AllocStringBuffer(u16 minLen) {
            if (minLen == 0) {
                return;
            }

            u16 allocBytes = sizeof(wchar_t) * (minLen + 1);
            if (allocBytes <= mTextBufBytes) {
                return;
            }

            FreeStringBuffer();

            mTextBuf = static_cast<wchar_t*>(Layout::AllocMemory(allocBytes));
            if (mTextBuf != NULL) {
                mTextBufBytes = allocBytes;
            }
        }

        void TextBox::FreeStringBuffer() {
            if (mTextBuf != NULL) {
                Layout::FreeMemory(mTextBuf);
                mTextBuf = NULL;
                mTextBufBytes = 0;
            }
        }

        u16 TextBox::SetString(const wchar_t* str, u16 dstIdx) {
            return SetString(str, dstIdx, std::wcslen(str));
        }

        u16 TextBox::SetString(const wchar_t* str, u16 dstIdx, u16 strLen) {
            if (mTextBuf == NULL) {
                NW4R_DB_WARNING(708, false, "mTextBuf is NULL.\n");
                return 0;
            }

            u16 bufLen = GetStringBufferLength();
            if (dstIdx >= bufLen) {
                NW4R_DB_WARNING(716, false, "dstIdx is out of range.\n");
                return 0;
            }

            const u16 cpLen = ut::Min<u16>(strLen, bufLen - dstIdx);

            if (cpLen < strLen) {
                NW4R_DB_WARNING(721, false, "%d character(s) droped.\n", strLen - cpLen);
            }

            std::memcpy(&mTextBuf[dstIdx], str, sizeof(wchar_t) * cpLen);

            mTextLen = dstIdx + cpLen;
            mTextBuf[mTextLen] = L'\0';

            return cpLen;
        }

        const ut::Font* TextBox::GetFont() const {
            return mpFont;
        }

        void TextBox::SetFont(const ut::Font* pFont) {
            if (mTextBoxFlag.allocFont) {
                NW4HBM_ASSERT_CHECK_NULL(775, mpFont);
                mpFont->~Font();
                Layout::FreeMemory(const_cast<ut::Font*>(mpFont));
                mTextBoxFlag.allocFont = false;
            }

            mpFont = pFont;

            if (mpFont) {
                SetFontSize(Size(mpFont->GetWidth(), mpFont->GetHeight()));
            } else {
                SetFontSize(Size(0.0f, 0.0f));
            }
        }

        const ut::Rect TextBox::GetTextDrawRect(ut::WideTextWriter* pWriter) const {
            ut::Rect textRect;

            pWriter->SetCursor(0.0f, 0.0f);
            CalcStringRect(&textRect, pWriter, mTextBuf, mTextLen, mSize.width);

            math::VEC2 basePt = Pane::GetVtxPos();

            textRect.MoveTo(basePt.x + (mSize.width - textRect.GetWidth()) * GetTextMagH(),
                            basePt.y + (mSize.height - textRect.GetHeight()) * GetTextMagV());

            return textRect;
        }

        f32 TextBox::GetTextMagH() const {
            f32 hMag = 0.0f;

            switch (GetTextPositionH()) {
            default:
            case HORIZONTALPOSITION_LEFT: {
                hMag = 0.0f;
                break;
            }
            case HORIZONTALPOSITION_CENTER: {
                hMag = 0.5f;
                break;
            }
            case HORIZONTALPOSITION_RIGHT: {
                hMag = 1.0f;
                break;
            }
            }

            return hMag;
        }

        f32 TextBox::GetTextMagV() const {
            f32 vMag = 0.0f;

            switch (GetTextPositionV()) {
            default:
            case HORIZONTALPOSITION_LEFT: {
                vMag = 0.0f;
                break;
            }
            case HORIZONTALPOSITION_CENTER: {
                vMag = 0.5f;
                break;
            }
            case HORIZONTALPOSITION_RIGHT: {
                vMag = 1.0f;
                break;
            }
            }

            return vMag;
        }
    }  // namespace lyt
}  // namespace nw4hbm
