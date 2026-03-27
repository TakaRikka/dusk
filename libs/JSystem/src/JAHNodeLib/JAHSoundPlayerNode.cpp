#include "JSystem/JAHNodeLib/JAHSoundPlayerNode.h"
#include "JSystem/JAHostIO/JAHUpdate.h"
#include "JSystem/JAHostIO/JAHioMessage.h"
#include "JSystem/JAudio2/JAISoundStarter.h"

void JAHPlayBox::onCurrentNodeFrame() {
    JAHSoundPlayerNode* master = (JAHSoundPlayerNode*)getMaster();
    if (!master)
        return;
    u8 b = master->getContMode() & 2;
    if (!b) {
        JAHUSeBox::onCurrentNodeFrame();
    } else if (unk48 != unk85) {
        if (unk48) {
            JAHUpdate::buttonOff(getMaster(), unk4C);
            JAHUpdate::buttonOn(getMaster(), unk50);
        } else {
            JAHUpdate::buttonOn(getMaster(), unk4C);
            JAHUpdate::buttonOff(getMaster(), unk50);
        }
    }

    unk85 = unk48;
}

u8 JAHSoundPlayerNode::getContMode() const {
    return unk7C;
}

JAHSoundPlayerNode::JAHSoundPlayerNode() : JAHFrameNode("SOUND PLAYER"), unk122(16) {
    unk70 = 30;
    unk74 = 60;
    unk78 = 10;
    unk7C = 0;
    unk7D = 0;
    unk7E = 1;
    unk80 = 0;
    unk84 = 0;
    unk7F = 2;
    unk120 = 0;
    appendVirNode(&unk98);
}

JAHPlayBox::JAHPlayBox() : JAHUSeBox(-1, NULL) {
    unk85 = 0;
}

void JAHSoundPlayerNode::message(JAHControl& param_1) {
    param_1.returnY(1);
    param_1.makeCheckBox("ID順に連続再生する", &unk7C, 2, false, 0);
    param_1.returnY(1);
    param_1.makeComment("--連続再生モード設定--", 0, 5, 0);
    param_1.indent(1);
    param_1.makeSlider("最大再生時間(1=0.1sec)", &unk70, 0, 60000, 0);
    param_1.makeSlider("空白時間(1=0.1sec)", &unk78, 0, 6000, 0);
    param_1.returnY(1);
    param_1.startRadioButton("ダミーデータの挙動", &unk7E, 2, 0);
    param_1.makeRadioButtonItem("スキップする", 1);
    param_1.makeRadioButtonItem("再生する", 0);
    param_1.endRadioButton();
    param_1.startRadioButton("トリガーSEまたはBGM系の挙動", &unk7D, 2, 0);
    param_1.makeRadioButtonItem("フル再生", 0);
    param_1.makeRadioButtonItem("最大再生時間以上はフェードアウト", 1);
    param_1.endRadioButton();
    param_1.returnY(1);
    param_1.makeSlider("フェードアウト時間(単位：ゲームフレーム)", &unk74, 0, 3600, 0);
}

void JAHSoundPlayerNode::onCurrentNodeFrame() {
    if (!(unk7C & 2))
        return;

    u8 attr = unk122.getAttribute(unk98.getId());

    switch (unk98.unk48) {
    case true:
        switch (unk7F) {
        case 0:
            if (!unk98.unk40) {
                unk90 = OSGetTime();
                unk84 = 0;
                unk7F = 2;
                break;
            }

            if (unk80 > unk70) {
                unk84 = 0;
                unk7F = 2;
            } else {
                JASGlobalInstance<JAISoundStarter>::getInstance()->startSound(unk98.getId(),
                                                                              &unk98.unk40, NULL);
                unk80 = OSTicksToMilliseconds(OSGetTime() - unk88) / 100;
            }

            if (unk7D == 1) {
                if (unk80 > unk70) {
                    unk98.unk40->stop(unk74);
                    unk84 = 0;
                    unk90 = OSGetTime();
                    unk7F = 1;
                } else {
                    unk80 = OSTicksToMilliseconds(OSGetTime() - unk88) / 100;
                }
            }
            break;

        case 1:
            if (!unk98.unk40) {
                unk7F = 2;
                unk90 = OSGetTime();
            }
            break;

        case 2:
            if (unk120) {
                if (unk84 <= unk78) {
                    if (unk84 < 0)
                        JASGlobalInstance<JAISoundStarter>::getInstance()->startSound(
                            unk98.getId(), &unk98.unk40, NULL);

                    unk84 = OSTicksToMilliseconds(OSGetTime() - unk90) / 100;
                    break;
                }

                u32 id = unk122.getNextId(unk98.getId(), unk7E != 0 ? true : false);
                unk98.setId(id, true, this);
            }

            JASGlobalInstance<JAISoundStarter>::getInstance()->startSound(unk98.getId(),
                                                                          &unk98.unk40, NULL);
            unk7F = 0;
            unk80 = 0;
            unk88 = OSGetTime();
            unk84 = 0;
            break;
        }
        break;

    case false:
        if (unk120) {
            unk80 = 0;
            unk84 = 0;
            unk7F = 2;
        }
        break;
    }

    unk120 = unk98.unk48;
}
