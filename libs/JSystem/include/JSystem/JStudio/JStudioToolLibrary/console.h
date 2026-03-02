#ifndef JSTUDIOTOOLLIBRARY_CONSOLE_H
#define JSTUDIOTOOLLIBRARY_CONSOLE_H

#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JHostIO/JORReflexible.h"
#include "JSystem/JUtility/TColor.h"

namespace JStudioToolLibrary {
    struct JORValPrpEvent {};
    struct JORStrValPrpEvent {};

    struct TConsole {
        TConsole(const char*, JORReflexible*);

        u32 memory_allocate_available(JKRHeap*, double, u32*);
        bool memory_isAllocatable(JKRHeap*);
        bool memory_isAllocatable(JKRHeap*, u32);
        bool memory_isAllocatable_available(JKRHeap*, double, u32*);
        bool memory_allocate(JKRHeap*, u32);

        void control_enable(const u32*, bool);
        void control_enable(u32, bool);
        void control_setStyle(u32, u32);
        void control_generateRadioButton(JORMContext*, u32, const char*, const char*, int);
        void control_generateSelectList(JORMContext*, u32, const char*, uint, const char*, int);
        void control_generateLabel(JORMContext*, u32, const char*);
        void control_getRadioButton(const JORValPrpEvent*);
        void control_isCheckBox(const JORValPrpEvent*);
        void control_getSelectList(const JORValPrpEvent*);
        void control_generateEditBox_float(JORMContext*, u32, const char*, const f32&);
        void control_generateEditBox_uint(JORMContext*, u32, const char*, const uint&);
        void control_generateTitle(JORMContext*, const char*) const;
        void control_generateSeparator(JORMContext*, const char*);
        void control_setCheckBox(u32, bool);
        void control_generateRadioButton_style(JORMContext*, u32, u32, const char*, const char**, int);
        void control_setRadioButton(u32, int);
        void control_generateSelectList_style(JORMContext*, u32, u32, const char*, uint, const char**, int);
        void control_setSelectList(u32, int);
        void control_generateLabel_style(JORMContext*, u32, u32, const char*);
        void control_generateEditBox_style(JORMContext*, u32, u32, const char*, const char*);
        void control_generateEditBox_style_int(JORMContext*, u32, u32, const char*, const int&);
        void control_generateEditBox_style_uint(JORMContext*, u32, u32, const char*, const uint&);
        void control_generateEditBox_style_float(JORMContext*, u32, u32, const char*, const f32&);
        void control_generateEditBox_style_RGBA(JORMContext*, u32, u32, const char*, JUtility::TColor);
        void control_generateEditBox_int(JORMContext*, u32, const char*, const int&);
        void control_setEditBox(u32, const char*);
        void control_setEditBox_f(u32, const char*, ...);
        void control_setEditBox_f_va(u32, const char*, va_list);
        void control_setEditBox_int(u32, const int&);
        void control_setEditBox_uint(u32, const uint&);
        void control_setEditBox_float(u32, const f32&);
        void control_setEditBox_RGBA(u32, const JUtility::TColor&);
        void control_replyEditBox_int(const JORStrValPrpEvent*, int*);
        void control_replyEditBox_uint(const JORStrValPrpEvent*, uint*);
        void control_replyEditBox_float(const JORStrValPrpEvent*, f32*);
        void control_replyEditBox_float_not(const JORStrValPrpEvent*, f32*, const f32&);
        void control_replyEditBox_float_greaterEqual(const JORStrValPrpEvent*, f32*, const f32&);
        void control_replyEditBox_RGBA(const JORStrValPrpEvent*, JUtility::TColor*);
        void control_forceEditBox(u32, const char*, const char*);

        void control_replyEditBox_float_range(const JORStrValPrpEvent*, f32*, const f32&, const f32&);
        void control_replyEditBox_uint_range(const JORStrValPrpEvent*, uint*, const uint&, const uint&);

        bool parseValue_int(const char*, int*);
        bool parseValue_uint(const char*, uint*);
        bool parseValue_float(const char*, f32*);
        bool parseValue_double(const char*, double*);
        bool parseValue_RGBA(const char*, JUtility::TColor*);
        bool parseValueList_float(const char*, char, f32*, f32*);

        int openMessageBox(uint, const char*);
        int openMessageBox_f(uint, const char*, ...);
        int openMessageBox_f_va(uint, const char*, va_list);
        bool openBrowser(const char*);

        void toValue_style_(u32);
        void getValueString_int(char*, u32, int);
        void getValueString_uint(char*, u32, uint);
        void getValueString_float(char*, u32, f32);
        void getValueString_RGBA(char*, u32, const JUtility::TColor&);

        /* 0x0 */ char* szTitle_;
        /* 0x4 */ JORReflexible* pReflexible_;
    };
}

#endif
