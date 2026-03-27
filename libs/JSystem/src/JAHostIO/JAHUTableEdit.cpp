#include "JSystem/JAHostIO/JAHUTableEdit.h"
#include "JSystem/JAHostIO/JAHUAsnData.h"
#include "JSystem/JAHostIO/JAHUpdate.h"
#include "JSystem/JAHostIO/JAHioMessage.h"
#include "JSystem/JAHostIO/JAHioUtil.h"
#include "JSystem/JAudio2/JAISoundStarter.h"
#include "JSystem/JAudio2/JAUSoundTable.h"

JAHUSeBox::JAHUSeBox(u32 param_1, u32* param_2) : JAHVirtualNode(NULL) {
    unk4C = getVirNodeNum() | 0x12000000;
    unk50 = getVirNodeNum() | 0x13000000;
    unk54 = getVirNodeNum() | 0x14000000;
    unk5C = param_2;
    mAsnCalc = NULL;

    mAsnCalc = new JAHUAsn(0x10);
    JUT_ASSERT(611, mAsnCalc);

    unk48 = false;
    if (param_1 == -1)
        unk58 = mAsnCalc->getDataId(0, 0, 0, 0);
    else
        unk58 = param_1;

    unk84 = 0;
}

void JAHUSeBox::trigPlayButton() {
    unk48 = true;
    JAHUpdate::buttonOff(getMaster(), unk4C);
    JAHUpdate::buttonOn(getMaster(), unk50);
}

void JAHUSeBox::trigStopButton() {
    unk48 = false;
    JAHUpdate::buttonOn(getMaster(), unk4C);
    JAHUpdate::buttonOff(getMaster(), unk50);
    soundStop();
}

void JAHUSeBox::soundStop() {
    if (unk40)
        unk40->stop();
}

void JAHUSeBox::onCurrentNodeFrame() {
    if (unk48) {
        soundPlay();
        unk48 = false;
    }

    if (unk44 && !unk40) {
        JAHUpdate::buttonOn(getMaster(), unk4C);
        JAHUpdate::buttonOff(getMaster(), unk50);
    } else if (!unk44 && unk40) {
        JAHUpdate::buttonOff(getMaster(), unk4C);
        JAHUpdate::buttonOn(getMaster(), unk50);
    }
}

void JAHUSeBox::soundPlay() {
    JAISoundStarter* starter = JASGlobalInstance<JAISoundStarter>::getInstance();
    if (starter) {
        starter->startSound(unk58, &unk40, 0);
    }
}

void JAHUSeBox::message(JAHControl& control) {
    if (!unk84 && JASGlobalInstance<JAUSoundNameTable>::getInstance()) {
        if (unk58 == -1) {
            unk64 = 0;
            unk68 = 0;
            unk6C = 0;
            unk70 = 0;
            unk74 = 0;
            unk78 = 0;
            unk7C = 0;
            unk80 = 0;
            unk58 = 0;
        } else {
            unk64 = unk58 >> 24;
            unk68 = mAsnCalc->getCategory(unk58);
            unk6C = mAsnCalc->getSub(unk58);
            unk70 = mAsnCalc->getDataPerSub(unk58);
            unk74 = unk64;
            unk78 = unk68;
            unk7C = unk6C;
            unk80 = unk70;
        }

        unk84 = true;
    }

    control.startComboBoxL(NULL, &unk68, 0, 200, 0);
    mAsnCalc->createComboBoxCategory(control);
    control.endComboBox();
    control.startComboBoxL(NULL, &unk6C, 0, 0x2d, 0);
    mAsnCalc->createComboBoxSub(control, unk64, unk68);
    control.endComboBox();
    control.startComboBoxL(NULL, &unk70, 0, 200, 0);
    mAsnCalc->createComboBoxData(control, unk64, unk68, unk6C);
    control.endComboBox();
    control.makeCommentL(JAHioUtil::getString("%08x", unk58), unk54, 0x37, 0);
    control.makeButtonL("＞", unk4C, 0x32, 0);
    control.makeButtonL("■", unk50, 0x32, 0x40000000);
    control.returnY(1);
}

void JAHUSeBox::nodeEvent(JAH_N_Event) {}

void JAHUSeBox::propertyEvent(JAH_P_Event param_1, u32 param_2) {
    if (param_1 == 0) {
        unk74 = unk64;
        unk78 = unk68;
        unk7C = unk6C;
        unk80 = unk70;
        return;
    }

    if (param_2 != unk4C && param_2 != unk50 &&
        ((unk7C == unk6C && unk80 == unk70) || unk68 != unk78))
    {
        JAUSoundNameTable* table = JASGlobalInstance<JAUSoundNameTable>::getInstance();
        int groups = table->getNumGroups_inSection(0);
        if (unk68 >= groups) {
            groups += table->getNumGroups_inSection(1);
            if (unk68 < groups) {
                unk64 = 1;
                unk68 -= table->getNumGroups_inSection(0);
            } else {
                unk64 = 2;
                unk68 -= groups;
            }
        } else {
            unk64 = 0;
        }
    }

    mAsnCalc->checkAndUpdateNameBox(&unk64, &unk68, &unk6C, &unk70, unk74, unk78, unk7C,
                                    getMaster());
    if (checkNameSetChange()) {
        unk58 = mAsnCalc->getDataId(unk64, unk68, unk6C, unk70);
        if (unk5C != NULL)
            *unk5C = unk58;
        char buf[9];
        sprintf(buf, "%08x", unk58);
        JORMContext* ctx = attachJORMContext(8);
        ctx->startUpdateNode(getMaster());
        ctx->updateLabel(2, unk54, buf);
        ctx->endUpdateNode();
        releaseJORMContext(ctx);
    }

    if (param_2 == unk4C) {
        trigPlayButton();
    } else if (param_2 == unk50) {
        trigStopButton();
    }
}

u8 JAHUSeBox::checkNameSetChange() {
    u8 iVar1 = 0;

    if (unk74 != unk64)
        iVar1++;

    if (unk78 != unk68)
        iVar1++;

    if (unk7C != unk6C)
        iVar1++;

    if (unk80 != unk70)
        iVar1++;

    return iVar1;
}

void JAHUSeBox::updateNameSet(JORReflexible* param_1) {
    mAsnCalc->updateNameBoxAll(&unk64, &unk68, &unk6C, &unk70, param_1);
    JORMContext* ctx = attachJORMContext(8);
    ctx->startUpdateNode(param_1);
    ctx->updateLabel(2, unk54, JAHioUtil::getString("%08x", unk58));
    ctx->endUpdateNode();
    releaseJORMContext(ctx);
}

void JAHUSeBox::setId(u32 param_1, bool param_2, JORReflexible* param_3) {
    unk58 = param_1;
    unk64 = unk58 >> 24;
    unk68 = (param_1 >> 16) & 0xff;
    unk6C = mAsnCalc->getSub(param_1);
    unk70 = mAsnCalc->getDataPerSub(param_1);
    if (param_2)
        updateNameSet(param_3);
}
