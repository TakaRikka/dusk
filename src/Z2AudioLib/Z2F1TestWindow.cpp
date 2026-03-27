#include "Z2AudioLib/Z2F1TestWindow.h"
#include "JSystem/JAHostIO/JAHioMessage.h"
#include "JSystem/JAudio2/JASAudioThread.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "Z2AudioLib/Z2Audience.h"

f32 sF1_Diff = 0.01f;
f32 sF1_i = 0.1f;
f32 sF1_Geta = 0.2f;
s32 sF1_Frame = 200;
f32 sF1_Pitch_Width = 0.2f;
f32 sF1_Pan_Width;

Z2F1TestWindow::Z2F1TestWindow() : JAWWindow("Z2F1TestWindow", 500, 450) {}

void Z2F1TestWindow::onDraw(JAWGraphContext*) {}

void Z2F1TestWindow::frameWork() {}

void Z2F1TestWindow::calcF1() {}

void Z2F1TestWindow::onPadProc(const JUTGamePad&) {}

f32 Z2F1TestWindow::tau(f32) {}

f32 Z2F1TestWindow::calcThreshold(int) {}

f32 Z2F1TestWindow::getRawX() const {
    return field_0x3f0;
}

f32 Z2F1TestWindow::calcNoise() {
    field_0x3f0 = field_0x3f4;
    field_0x3f4 = tau(field_0x3f0);
    return field_0x3f0;
}

void Z2F1TestWindow::setValue(f32 param_1, f32 param_2) {
    field_0x3f0 = param_1;
    field_0x3f4 = param_1;
    field_0xbd4 = param_2;
}

void Z2F1TestWindow::onTrigY(const JUTGamePad&) {
    for (u16 i = 0; i < 500; ++i)
        field_0x3f8[i] = 0.0f;

    field_0x3f0 = tau(sF1_i);
    field_0x3f4 = tau(field_0x3f0);
    field_0xbd4 = calcThreshold(sF1_Frame);
}

void Z2F1TestWindow::onTrigMenu(const JUTGamePad&) {
    if (field_0xbd0)
        field_0xbd0 = 0;
    else
        field_0xbd0 = 1;
}

Z2F1SwingTestNode::Z2F1SwingTestNode()
    : JAHFrameNode("1/fゆらぎテスト"), field_0xc48(-1, NULL), field_0xcd0(-1, NULL) {
    appendVirNode(&field_0xc48);
    appendVirNode(&field_0xcd0);
    mModulateVolume = 0;
    mModulatePitch = 0;
    mModulatePan = 0;
    mModulateDolby = 0;
    field_0xd5c = 0.0f;
    field_0xd60 = 0.0f;
    field_0xd64 = 0.0f;
    field_0xd68 = 0.0f;
}

void Z2F1SwingTestNode::message(JAHControl& ctrl) {
    ctrl.makeComment("--1/fゆらぎテスト--", 0, 5, 0);
    ctrl.indent(1);
    ctrl.makeSlider("Diff値", &sF1_Diff, 0.0, 0.1, 0);
    ctrl.makeSlider("i値", &sF1_i, 0.0f, 1.0f, 0);
    ctrl.makeSlider("Geta値", &sF1_Geta, 0.0f, 1.0f, 0);
    ctrl.makeSlider("frame値", &sF1_Frame, 2, 200, 0);
    ctrl.returnY(1);
    ctrl.makeCheckBox("ボリューム変調", &mModulateVolume, 1, false, 0);
    ctrl.makeCheckBox("ピッチ変調", &mModulatePitch, 1, false, 0);
    ctrl.makeSlider("Pitch ゆらぎ幅", &sF1_Pitch_Width, 0.0f, 1.0f, 0);
    ctrl.makeCheckBox("パン変調", &mModulatePan, 1, false, 0);
    ctrl.makeSlider("パン移動幅", &sF1_Pan_Width, 0.0f, 1.0f, 0);
    ctrl.makeCheckBox("ドルビー変調", &mModulateDolby, 1, false, 0);
}

void Z2F1SwingTestNode::onFrame() {
    f32 fVar1 = 1.0;
    f32 fVar2 = getValue() * (1.0f - sF1_Geta) + sF1_Geta;
    f32 fVar3 = getValue2() * (1.0f - sF1_Geta) + sF1_Geta;
    if (field_0xc48.unk40.isSoundAttached() && field_0xcd0.unk40.isSoundAttached()) {
        fVar1 = 0.5f;
        field_0xc48.unk40->getAuxiliary().moveVolume(fVar2 * fVar1, 0);
        field_0xcd0.unk40->getAuxiliary().moveVolume(fVar3 * fVar1, 0);
    } else if (field_0xc48.unk40.isSoundAttached()) {
        fVar1 = 1.0f;
        field_0xc48.unk40->getAuxiliary().moveVolume(fVar2 * fVar1, 0);
    } else if (field_0xcd0.unk40.isSoundAttached()) {
        fVar1 = 1.0f;
        field_0xcd0.unk40->getAuxiliary().moveVolume(fVar3 * fVar1, 0);
    }

    if (field_0xc48.unk40.isSoundAttached()) {
        if (mModulateVolume)
            field_0xc48.unk40->getAuxiliary().moveVolume(fVar3 * fVar1, 0);
        else
            field_0xc48.unk40->getAuxiliary().moveVolume(0.5f, 0);

        if (mModulatePitch)
            field_0xc48.unk40->getAuxiliary().movePitch(
                getValue2() * sF1_Pitch_Width + (1.0f - sF1_Pitch_Width), 0);
        else
            field_0xc48.unk40->getAuxiliary().movePitch(1.0f, 0);

        if (mModulatePan)
            field_0xc48.unk40->getAuxiliary().movePan(0.5f - getValue2() * sF1_Pan_Width / 2, 0);
        else
            field_0xc48.unk40->getAuxiliary().movePan(0.5f, 0);

        if (mModulateDolby)
            field_0xc48.unk40->getAuxiliary().moveDolby(fVar3, 0);
        else
            field_0xc48.unk40->getAuxiliary().moveDolby(0.0f, 0);
    } else if (field_0xcd0.unk40.isSoundAttached()) {
        if (mModulateVolume)
            field_0xcd0.unk40->getAuxiliary().moveVolume(fVar3 * fVar1, 0);
        else
            field_0xcd0.unk40->getAuxiliary().moveVolume(0.5f, 0);

        if (mModulatePitch)
            field_0xcd0.unk40->getAuxiliary().movePitch(
                getValue2() * sF1_Pitch_Width + (1.0f - sF1_Pitch_Width), 0);
        else
            field_0xcd0.unk40->getAuxiliary().movePitch(1.0f, 0);

        if (mModulatePan)
            field_0xcd0.unk40->getAuxiliary().movePan(0.5f + getValue2() * sF1_Pan_Width / 2, 0);
        else
            field_0xcd0.unk40->getAuxiliary().movePan(0.5f, 0);

        if (mModulateDolby)
            field_0xcd0.unk40->getAuxiliary().moveDolby(fVar3, 0);
        else
            field_0xcd0.unk40->getAuxiliary().moveDolby(0.0f, 0);
    }
}

f32 Z2F1TestWindow::getValue2() {
    return field_0x3f8[field_0xbcc];
}

f32 Z2F1TestWindow::getValue() {
    return field_0x3f8[field_0xbc8];
}

Z2AudSettingView::Z2AudSettingView() : JAWWindow("Z2AudSettingView", 400, 300) {}

void Z2AudSettingView::onDraw(JAWGraphContext* param_1) {
    const Z2AudioCamera* this_00 = Z2GetAudience()->getAudioCamera(0);
    Z2Audience3DSetting* pfVar2 = Z2GetAudience()->getSetting();
    int l = 0;
    param_1->print(0, l++, "<SETTING>");
    param_1->print(0, l++, "far  dist    %.3f", pfVar2->field_0x0[0]);
    param_1->print(0, l++, "near dist    %.3f", pfVar2->field_0x3c);
    param_1->print(0, l++, "frontDolbyZ  %.3f", pfVar2->field_0x44);
    param_1->print(0, l++, "behindDolbyZ %.3f", pfVar2->field_0x48);
    param_1->print(0, l++, "centerDolby  %.3f", pfVar2->field_0x4c);
    ++l;
    param_1->print(0, l++, "<AUD CAMERA>");
    param_1->print(0, l++, "CamDist %.3f", this_00->getCamDist());
    param_1->print(0, l++, "vel.x        %.3f", this_00->getVel()->x);
    param_1->print(0, l++, "vel.y        %.3f", this_00->getVel()->y);
    param_1->print(0, l++, "vel.z        %.3f", this_00->getVel()->z);
    param_1->print(0, l++, "TargetVolume %.3f", this_00->getTargetVolume());
    param_1->print(0, l++, "VolCenterZ   %.3f", this_00->getVolCenterZ());
    param_1->print(0, l++, "DolbyCenterZ %.3f", this_00->getDolbyCenterZ());
    param_1->print(0, l++, "getFovySin   %.3f", this_00->getFovySin());
}

void Z2DSPCheck::onDraw(JAWGraphContext* param_1) {
    u8* puVar1 = (u8*)JKRHeap::getAltAramStartAdr();
    DCInvalidateRange(puVar1, 0x20);
    int l = 0;
    param_1->print(0, l++, "<DISP INFO>");
    param_1->print(0, l++, "VARAM_TOP     %08X", puVar1);
    param_1->print(0, l++, "DSP state     %04X", *(u16*)(puVar1 + 0));
    param_1->print(0, l++, "DSP channel   %04X", *(u8*)(puVar1 + 2));
    param_1->print(0, l++, "DSP sub-fr    %04X", *(u8*)(puVar1 + 3));
    param_1->print(0, l++, "DSP cmd wr    %04X", *(u16*)(puVar1 + 4));
    param_1->print(0, l++, "DSP sub-rd    %04X", *(u16*)(puVar1 + 6));
    param_1->print(0, l++, "DSP cmd rest  %04X", *(u16*)(puVar1 + 8));
    l++;
    param_1->print(0, l++, "<CPU AUDIO THREAD INFO>");
    param_1->print(0, l++, "DSPSyncCount   %2d", JASAudioThread::getDSPSyncCount());
}
