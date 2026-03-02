#ifndef NW4HBM_LYT_TEXTBOX_H
#define NW4HBM_LYT_TEXTBOX_H

#include "common.h"
#include "pane.h"

#include "../ut/RuntimeTypeInfo.h"
#include "../ut/TagProcessorBase.h"
#include "../ut/WideTagProcessor.h"
#include "../ut/WideTextWriter.h"

namespace nw4hbm {
namespace lyt {

class TextBox : public Pane {
public:
    TextBox(u16 allocStrLen, const wchar_t* str, const ut::Font* pFont);
    TextBox(const res::TextBox* pBlock, const ResBlockSet& resBlockSet);

    /* 0x08 */ virtual ~TextBox();
    /* 0x0C */ NW4HBM_UT_RUNTIME_TYPEINFO;
    /* 0x18 */ virtual void DrawSelf(const DrawInfo& drawInfo);
    /* 0x24 */ virtual ut::Color GetVtxColor(u32 idx) const;
    /* 0x28 */ virtual void SetVtxColor(u32 idx, ut::Color value);
    /* 0x34 */ virtual u8 GetVtxColorElement(u32 idx) const;
    /* 0x38 */ virtual void SetVtxColorElement(u32 idx, u8 value);
    /* 0x64 */ virtual void AllocStringBuffer(u16 size);
    /* 0x68 */ virtual void FreeStringBuffer();
    /* 0x6C */ virtual u16 SetString(const wchar_t* str, u16 dstIdx = 0);
    /* 0x70 */ virtual u16 SetString(const wchar_t* str, u16 dstIdx, u16 strLen);

    const Size& GetFontSize() const { return mFontSize; }

    void SetFontSize(const Size& fontSize) { mFontSize = fontSize; }
    void SetTagProcessor(ut::WideTagProcessor* pTagProcessor) { mpTagProcessor = pTagProcessor; }

    u16 GetStringBufferLength() const;

    f32 GetTextMagH() const;
    f32 GetTextMagV() const;

    u8 GetTextPositionH() const { return detail::GetHorizontalPosition(mTextPosition); }
    u8 GetTextPositionV() const { return detail::GetVerticalPosition(mTextPosition); }

    const ut::Color GetTextColor(u32 type) const {
        NW4HBM_ASSERT(95, type < TEXTCOLOR_MAX);
        return mTextColors[type];
    }

    void SetTextColor(u32 type, ut::Color value) {
        NW4HBM_ASSERT(96, type < TEXTCOLOR_MAX);
        mTextColors[type] = value;
    }

    void SetTextPositionH(u8 pos) { detail::SetHorizontalPosition(&mTextPosition, pos); }
    void SetTextPositionV(u8 pos) { detail::SetVerticalPosition(&mTextPosition, pos); }

    const ut::Font* GetFont() const;
    void SetFont(const ut::Font* pFont);

    void Init(u16 allocStrLen);

    const ut::Rect GetTextDrawRect(const DrawInfo& drawInfo) const;
    const ut::Rect GetTextDrawRect(ut::WideTextWriter* pWriter) const;

private:
    /* 0x00 (base) */
    /* 0xD4 */ wchar_t* mTextBuf;
    /* 0xD8 */ ut::Color mTextColors[TEXTCOLOR_MAX];
    /* 0xE0 */ const ut::Font* mpFont;
    /* 0xE4 */ Size mFontSize;
    /* 0xEC */ f32 mLineSpace;
    /* 0xF0 */ f32 mCharSpace;
    /* 0xF4 */ ut::WideTagProcessor* mpTagProcessor;
    /* 0xF8 */ u16 mTextBufBytes;
    /* 0xFA */ u16 mTextLen;
    /* 0xFC */ u8 mTextPosition;

    /* 0xFD */ struct {
        u8 allocFont : 1;
    } mTextBoxFlag;
};
} // namespace lyt
} // namespace nw4hbm

#endif
