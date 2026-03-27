#include <JSystem/JAHostIO/JAHUAsnData.h>
#include "JSystem/JAHostIO/JAHioMessage.h"
#include "JSystem/JAudio2/JAUSoundTable.h"
#include "JSystem/JHostIO/JORServer.h"

u8 JAHUAsn::getAttribute(u32 param_1) {
    u8 a = (param_1 & 0x800) >> 11;
    u8 b = (param_1 & 0xC0000000) >> 24;
    return a | (b << 4);
}

u16 JAHUAsn::getSubNum(s32 param_1, s32 param_2) {
    JAUSoundNameTable* table = JASGlobalInstance<JAUSoundNameTable>::getInstance();
    int num = table->getNumItems_inGroup(param_1, param_2);
    int uVar2 = (u16)(num / unk0);
    if (num % unk0 > 0)
        uVar2++;
    return uVar2 <= 64 ? (u16)uVar2 : (u16)64;
}

u16 JAHUAsn::getDataNum(s32 param_1, s32 param_2, s32 param_3) {
    JAUSoundNameTable* table = JASGlobalInstance<JAUSoundNameTable>::getInstance();
    int uVar1 = getSubNum(param_1, param_2);
    int iVar2 = table->getNumItems_inGroup(param_1, param_2);

    if (iVar2 == 0)
        return 0;

    if (iVar2 % unk0 == 0)
        return unk0;

    if (param_3 == uVar1 - 1)
        return iVar2 % unk0;

    return unk0;
}

const char* JAHUAsn::getName(s32 param_1, s32 param_2, s32 param_3, s32 param_4) {
    JAUSoundNameTable* table = JASGlobalInstance<JAUSoundNameTable>::getInstance();
    return table->getName((param_1 << 24) + (param_2 << 16) + (param_4 + param_3 * unk0));
}

u32 JAHUAsn::getDataId(s32 param_1, s32 param_2, s32 param_3, s32 param_4) {
    return (param_1 << 24) + (param_2 << 16) + (param_4 + param_3 * unk0);
}

void JAHUAsn::createComboBoxCategory(JAHControl& control) {
    int totalItems = 0;

    JAUSoundNameTable* table = JASGlobalInstance<JAUSoundNameTable>::getInstance();
    for (u32 i = 0; i < 3; ++i) {
        for (u32 j = 0; j < table->getNumGroups_inSection(i); ++j) {
            if (table->getNumItems_inGroup(i, j) > 0) {
                const char* name = table->getGroupName((i << 24) + (j << 16));
                control.getContext()->genComboBoxItem(name, totalItems);
                ++totalItems;
            }
        }
    }
}

void JAHUAsn::createComboBoxSub(JAHControl& control, s32 param_2, s32 param_3) {
    int uVar1 = getSubNum(param_2, param_3);
    for (int i = 0; i < uVar1; ++i) {
        char buffer[4];
        sprintf(buffer, "%2x\0", i);
        control.getContext()->genComboBoxItem(buffer, i);
    }
}

void JAHUAsn::createComboBoxData(JAHControl& control, s32 param_2, s32 param_3, s32 param_4) {
    int uVar1 = getDataNum(param_2, param_3, param_4);
    for (int i = 0; i < uVar1; ++i) {
        const char* name = getName(param_2, param_3, param_4, i);
        control.getContext()->genComboBoxItem(name, i);
    }
}

void JAHUAsn::checkAndUpdateNameBox(s32* param_1, s32* param_2, s32* param_3, s32* param_4,
                                    s32 param_5, s32 param_6, s32 param_7, JORReflexible* param_8) {
    if (*param_1 != param_5 || *param_2 != param_6 || *param_3 != param_7) {
        if (*param_2 != param_6 || *param_1 != param_5) {
            *param_3 = 0;
            __updateNameBoxSub(*param_1, *param_2, param_3, param_8);
        }

        if (*param_3 != param_7 || *param_2 != param_6 || *param_1 != param_5) {
            *param_4 = 0;
            __updateNameBoxData(*param_1, *param_2, *param_3, param_4, param_8);
        }
    }
}

void JAHUAsn::updateNameBoxAll(s32* param_1, s32* param_2, s32* param_3, s32* param_4,
                               JORReflexible* param_5) {
    __updateNameBoxCategory(param_2, param_5);
    __updateNameBoxSub(*param_1, *param_2, param_3, param_5);
    __updateNameBoxData(*param_1, *param_2, *param_3, param_4, param_5);
}

void JAHUAsn::__updateNameBoxCategory(s32* param_1, JORReflexible* param_2) {
    JORMContext* ctx = attachJORMContext(8);
    ctx->startUpdateNode(param_2);
    ctx->updateComboBox(2, param_1, 0);
    ctx->endUpdateNode();
    releaseJORMContext(ctx);
}

void JAHUAsn::__updateNameBoxSub(s32 param_1, s32 param_2, s32* param_3, JORReflexible* param_4) {
    JORMContext* ctx = attachJORMContext(8);
    ctx->startUpdateNode(param_4);
    ctx->removeComboBoxItem(5, param_3, 0);
    int num = getSubNum(param_1, param_2);
    for (int i = 0; i < num; ++i) {
        char buf[4];
        sprintf(buf, "%2x\0", i);
        ctx->addComboBoxItem(1, param_3, buf, i, 0);
    }
    ctx->updateComboBox(2, param_3, 0);
    ctx->endUpdateNode();
    releaseJORMContext(ctx);
}

void JAHUAsn::__updateNameBoxData(s32 param_1, s32 param_2, s32 param_3, s32* param_4,
                                  JORReflexible* param_5) {
    JORMContext* ctx = attachJORMContext(8);
    ctx->startUpdateNode(param_5);
    ctx->removeComboBoxItem(5, param_4, 0);
    ctx->endUpdateNode();
    releaseJORMContext(ctx);

    ctx = attachJORMContext(8);
    ctx->startUpdateNode(param_5);
    int num = getDataNum(param_1, param_2, param_3);
    for (int i = 0; i < num; ++i) {
        ctx->addComboBoxItem(1, param_4, getName(param_1, param_2, param_3, i), i, 0);
    }
    ctx->updateComboBox(2, param_4, 0);
    ctx->endUpdateNode();
    releaseJORMContext(ctx);
}

u32 JAHUAsn::__GetSubNo(s32 param_1) {
    return param_1 / unk0;
}

u32 JAHUAsn::__GetDataNo(s32 param_1) {
    return param_1 % unk0;
}

u8 JAHUAsn::getCategory(u32 param_1) {
    return (param_1 & 0xF0000) >> 0x10;
}

u8 JAHUAsn::getSub(u32 param_1) {
    int perCat = getDataPerCat(param_1);
    return __GetSubNo(perCat);
}

u16 JAHUAsn::getDataPerCat(u32 param_1) {
    return param_1;
}

u16 JAHUAsn::getDataPerSub(u32 param_1) {
    u16 perCat = getDataPerCat(param_1);
    return __GetDataNo(perCat);
}

u32 JAHUAsn::getNextId(u32 param_1, bool param_2) {
    JUT_ASSERT(490, 0L);
    return param_1;
}
