#include "Z2AudioLib/Z2SoundPlayer.h"
#include "Z2AudioLib/Z2AudioMgr.h"
#include "Z2AudioCS/Z2AudioCS.h"
#include "JSystem/JUtility/JUTGamePad.h"

Z2SoundPlayer::Z2SoundPlayer(const char* projectName)
    : JAWWindow("Player for JAudio2", 500, 140),
      field_0x48c(field_0x46c, 8)
{
    field_0x3ed = false;
    field_0x3ee = false;
    field_0x3ef = false;
    field_0x3f0 = false;

    m_name = projectName;
    field_0x3f8 = 0;
    field_0x3fc = 0;
    field_0x3fa = 0;
    field_0x3fe = 0;
    field_0x400 = 0;
    m_portNum = 0;
    m_portVal = 0;
    field_0x446 = 0;

    m_portNames[0] = "CMD_PORT";
    m_portNames[1] = "END_PORT";
    m_portNames[2] = "STA_PORT";
    m_portNames[3] = "WAIT_PORT";
    m_portNames[4] = "NUM_PORT";
    m_portNames[5] = "PORT_5";
    m_portNames[6] = "MAP_PORT";
    m_portNames[7] = "NOTE_PORT";
    m_portNames[8] = "SE_SELECT_PORT";
    m_portNames[9] = "BGM_STATUS_PORT";
    m_portNames[10] = "BGM_PORT2";
    m_portNames[11] = "BGM_PORT3";
    m_portNames[12] = "PORT12";
    m_portNames[13] = "PORT13";
    m_portNames[14] = "FILTER_PORT";
    m_portNames[15] = "FX_PORT";

    u8 r30 = 0;
    field_0x448[r30++] = &field_0x3f8;
    field_0x448[r30++] = &field_0x3fa;
    field_0x448[r30++] = &field_0x3fc;
    field_0x448[r30++] = &field_0x3fe;
    field_0x448[r30++] = &field_0x400;
    field_0x448[r30++] = &m_portNum;
    field_0x448[r30] = &m_portVal;

    m_cursorY = 0;
    m_cursorMax = 7;
    field_0x4a0 = 0;
    field_0x4a4 = 1.0f;
    field_0x4a8 = 0.5f;
    field_0x4ac = 1.0f;
    field_0x4b0 = 0.0f;
    field_0x4b4 = 0.0f;
    field_0x4b8 = 1.0f;

    mp_subBgmHandle = Z2GetSeqMgr()->getSubBgmHandle();
}

void Z2SoundPlayer::onDraw(JAWGraphContext* graf) {
    JAUSoundNameTable* soundTable = JASGlobalInstance<JAUSoundNameTable>::getInstance();
    
    int var_r27 = 0;
    if (field_0x3ed) {
        var_r27 = 2;
    }

    u8 cursorY = 0;

    graf->color(JUtility::TColor(0, 0x7F, 0xFF, 0xFF), JUtility::TColor(0, 0xFF, 0x7F, 0xFF));
    graf->print(1, cursorY++, "Project: %s", m_name);

    graf->color(0xFF >> (u8)var_r27, 0xFF >> (u8)var_r27, 0xFF >> (u8)var_r27, 0xFF);
    graf->print(0, m_cursorY + 1, ">");

    JUtility::TColor sp28(0xFF >> (u8)var_r27, 0xFF >> (u8)var_r27, 0xFF >> (u8)var_r27, 0xFF);
    JUtility::TColor sp24(0, 0xFF >> (u8)var_r27, 0, 0xFF);

    onDrawSoundItem(graf, soundTable, cursorY++,
        sp28,
        sp24,
        "SEQ NUM ",
        1, 0, field_0x3f8
    );

    graf->color(sp28);
    const char* cateName;
    if (soundTable != NULL) {
        cateName = soundTable->getGroupName(JAISoundID(0, field_0x3fa & 0xFF, 0));
    } else {
        cateName = "";
    }
    graf->print(1, cursorY++, "SE CATE  %03x %s", field_0x3fa, cateName);

    onDrawSoundItem(graf, soundTable, cursorY++,
        sp28,
        sp24,
        "SE NUM  ",
        0, field_0x3fa & 0xFF, field_0x3fc
    );
    onDrawSoundItem(graf, soundTable, cursorY++,
        sp28,
        sp24,
        "STRM NUM",
        2, 0, field_0x3fe
    );

    graf->color(sp28);
    graf->print(1, cursorY++, "CORE_SPK %03x %s", field_0x400, Z2AudioCS::getName(field_0x400));

    graf->color(field_0x446 + 0xC0, 0xC0, 0xC0, 0xFF);
    graf->print(1, cursorY++, "PORT NUM %03x %s", m_portNum, m_portNames[m_portNum]);
    graf->print(1, cursorY++, "PORT VAL %03x", m_portVal);

    graf->color(0, 0x50, 0xC8, 0xFF);
    if (field_0x3ee) {
        graf->print(1, cursorY, "DOLBY ON\n");
    }

    graf->color(0xC8, 0x50, 0, 0xFF);
    if (field_0x3f0) {
        graf->print(10, cursorY, "LEVEL SE CALL");
    }

    graf->color(0xFF, 0, 0, 0xFF);
    graf->print(0x16, cursorY, "%s", field_0x3ed ? "PAUSE ON" : "");

    graf->color(0, 0xC8, (field_0x4a0 * 200) / 30, 0xFF);
    if (field_0x3ef) {
        graf->print(0x16, cursorY, "RESET %d\n", field_0x4a0);
    }
}

void Z2SoundPlayer::onTrigA(const JUTGamePad& pad) {
    switch (m_cursorY) {
    case 0: {
        JAISoundID sp24(1, 0, (u16)field_0x3f8);
        switch (sp24) {
        case 0x100000a:
        case 0x100000b:
        case 0x100000f:
        case 0x1000012:
        case 0x1000014:
        case 0x100001b:
        case 0x100001c:
        case 0x1000024:
            Z2GetSoundMgr()->startSound(sp24, mp_subBgmHandle, NULL);
            break;
        default:
            Z2GetSoundMgr()->startSound(sp24, &field_0x494, NULL);
            break;
        }
        break;
    }
    case 1:
    case 2:
        if (!field_0x3f0) {
            JAISoundHandle* handle = field_0x48c.getFreeHandle();
            JAISoundID sp20(0, field_0x3fa & 0xFF, (u16)field_0x3fc);
            if (handle != NULL) {
                Z2GetSoundStarter()->startSound(sp20, handle, NULL);
                if (*handle) {
                    Z2GetSoundStarter()->setPortData(handle, m_portNum, m_portVal, -1);
                }
            }
        }
        break;
    case 3:
        Z2GetSoundMgr()->startSound(JAISoundID(2, 0, (u16)field_0x3fe), &field_0x49c, NULL);
        break;
    case 4:
        if (!field_0x3f0) {
            Z2AudioCS::start(field_0x400, 0);
        }
        break;
    case 5:
    case 6:
        field_0x446 = 64;
        if (m_portNum < 9) {
            JAISoundHandle* handle = field_0x48c.getFreeHandle();
            JAISoundID sp1C(0, field_0x3fa & 0xFF, (u16)field_0x3fc);
            if (handle != NULL) {
                Z2GetSoundStarter()->startSound(sp1C, handle, NULL);
                if (*handle) {
                    Z2GetSoundStarter()->setPortData(handle, m_portNum, m_portVal, -1);
                }
            }
        } else {
            if (field_0x494) {
                Z2GetSoundStarter()->setPortData(&field_0x494, m_portNum, m_portVal, -1);
            }

            if (mp_subBgmHandle->isSoundAttached()) {
                Z2GetSoundStarter()->setPortData(mp_subBgmHandle, m_portNum, m_portVal, -1);
            }
        }
        break;
    }
}

void Z2SoundPlayer::onTrigB(const JUTGamePad& pad) {
    switch (m_cursorY) {
    case 0: {
        if (field_0x494) {
            field_0x494->stop(0);
        }

        field_0x494.releaseSound();

        if (mp_subBgmHandle->isSoundAttached()) {
            (*mp_subBgmHandle)->stop();
        }
        break;
    }
    case 1:
    case 2: {
        JAISoundHandle* handle = field_0x48c.getHandleSoundID(JAISoundID(0, field_0x3fa & 0xFF, (u16)field_0x3fc));
        if (handle) {
            (*handle)->stop();
            handle->releaseSound();
        }
        break;
    }
    case 3:
        if (field_0x49c) {
            field_0x49c->stop();
        }

        field_0x49c.releaseSound();
        break;
    case 4:
        Z2AudioCS::stop(0);
        break;
    case 5:
    case 6:
        m_portVal = 0;
        field_0x446 = 0;

        if (field_0x494) {
            Z2GetSoundStarter()->setPortData(&field_0x494, m_portNum, m_portVal, -1);
        }

        if (mp_subBgmHandle->isSoundAttached()) {
            Z2GetSoundStarter()->setPortData(mp_subBgmHandle, m_portNum, m_portVal, -1);
        }
        break;
    }
}

void Z2SoundPlayer::frameWork() {
    switch (m_cursorY) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        break;
    }

    if (field_0x3ef) {
        field_0x4a0++;

        if (Z2GetAudioMgr()->hasReset()) {
            Z2GetAudioMgr()->resetRecover();
            field_0x3ef = false;
            OS_REPORT("RESET END \n");
        }
    }
}

void Z2SoundPlayer::onPadProc(const JUTGamePad& pad) {
    if (field_0x3f0 && pad.testButton(PAD_BUTTON_A)) {
        switch (m_cursorY) {
        case 2: {
            JAISoundID sp14(0, field_0x3fa & 0xFF, (u16)field_0x3fc);
            JAISoundHandle* handle = field_0x48c.getHandleSoundID(sp14);
            if (handle == NULL) {
                handle = field_0x48c.getFreeHandle();
            }

            if (handle != NULL) {
                Z2GetAudioMgr()->startLevelSound(sp14, handle, NULL);
            }

            if (*handle) {
                Z2GetSoundStarter()->setPortData(handle, m_portNum, m_portVal, -1);
            }
            break;
        }
        case 4:
            Z2AudioCS::startLevel(field_0x400, 0);
            break;
        }
    }
}

void Z2SoundPlayer::onTrigX(const JUTGamePad& pad) {
    switch (m_cursorY) {
    case 0: {
        if (field_0x494) {
            field_0x494->pause(!field_0x494->isPaused());
        }

        if (mp_subBgmHandle->isSoundAttached()) {
            (*mp_subBgmHandle)->pause(!(*mp_subBgmHandle)->isPaused());
        }
        break;
    }
    case 1:
    case 2: {
        JAISoundHandle* handle = field_0x48c.getHandleSoundID(JAISoundID(0, field_0x3fa & 0xFF, (u16)field_0x3fc));
        if (handle) {
            (*handle)->pause(!(*handle)->isPaused());
        }
        break;
    }
    case 3:
        if (field_0x49c) {
            (field_0x49c)->pause(!(field_0x49c)->isPaused());
        }
        break;
    case 4:
        Z2AudioCS::stop(0);
        break;
    }
}

void Z2SoundPlayer::onTrigY(const JUTGamePad& pad) {
    if (!field_0x3ef) {
        Z2GetAudioMgr()->resetProcess(30, false);
        field_0x3ef = true;
        field_0x4a0 = 0;
    }
}

void Z2SoundPlayer::onTrigZ(const JUTGamePad& pad) {
    bool* pvar = &field_0x3f0;
    bool newValue = *pvar ^ 1;
    *pvar = newValue;
}

void Z2SoundPlayer::onTrigL(const JUTGamePad& pad) {
    bool* pvar = &field_0x3ee;
    bool newValue = *pvar ^ 1;
    *pvar = newValue;

    if (field_0x3ee) {
        Z2GetAudioMgr()->setOutputMode(2);
    } else {
        Z2GetAudioMgr()->setOutputMode(1);
    }
}

void Z2SoundPlayer::onKeyLeft(const JUTGamePad& pad) {
    int moveMax = getCursorMoveMax(pad);
    int menuMax = getMenuNumberMax();

    if (moveMax >= menuMax) {
        moveMax = 1;
    }

    if (*field_0x448[m_cursorY] > moveMax - 1) {
        *field_0x448[m_cursorY] -= (s16)moveMax;
    } else {
        *field_0x448[m_cursorY] = (*field_0x448[m_cursorY] + menuMax) - moveMax;
    }

    if (m_cursorY == 1) {
        correctSeNumber();
    }
}

void Z2SoundPlayer::onKeyRight(const JUTGamePad& pad) {
    int moveMax = getCursorMoveMax(pad);
    int menuMax = getMenuNumberMax();

    if (moveMax >= menuMax) {
        moveMax = 1;
    }

    if (*field_0x448[m_cursorY] < menuMax - moveMax) {
        *field_0x448[m_cursorY] += (s16)moveMax;
    } else {
        *field_0x448[m_cursorY] = (*field_0x448[m_cursorY] + moveMax) - menuMax;
    }

    if (m_cursorY == 1) {
        correctSeNumber();
    }
}

void Z2SoundPlayer::onKeyUp(const JUTGamePad& pad) {
    if (m_cursorY != 0) {
        m_cursorY--;
    } else {
        m_cursorY = m_cursorMax - 1;
    }
}

void Z2SoundPlayer::onKeyDown(const JUTGamePad& pad) {
    if (m_cursorY < m_cursorMax - 1) {
        m_cursorY++;
    } else {
        m_cursorY = 0;
    }
}

void Z2SoundPlayer::onKeyMenu(const JUTGamePad& pad) {
    bool* pvar = &field_0x3ed;
    bool newValue = *pvar ^ 1;
    *pvar = newValue;

    Z2GetSoundMgr()->pauseAllGameSound(field_0x3ed);
}

u32 Z2SoundPlayer::getCursorMoveMax(const JUTGamePad& pad) {
    u32 num = 1;

    if (pad.testButton(PAD_TRIGGER_Z) && pad.testButton(PAD_BUTTON_X)) {
        num = 128;
    } else if (pad.testButton(PAD_TRIGGER_Z)) {
        num = 16;
    } else if (pad.testButton(PAD_BUTTON_X)) {
        num = 8;
    }

    return num;
}

int Z2SoundPlayer::getMenuNumberMax() {
    JAUSoundTable* soundTable = JASGlobalInstance<JAUSoundTable>::getInstance();
    JUT_ASSERT(550, soundTable);

    int num = 0;
    switch (m_cursorY) {
    case 0:
        num = soundTable->getNumItems_inGroup(1, 0);
        break;
    case 1:
        num = soundTable->getNumGroups_inSection(0);
        break;
    case 2:
        num = soundTable->getNumItems_inGroup(0, field_0x3fa);
        break;
    case 3:
        num = soundTable->getNumItems_inGroup(2, 0);
        break;
    case 4:
        num = Z2AudioCS::getNumOfSound();
        break;
    case 5:
        num = 16;
        break;
    case 6:
        num = 128;
        break;
    }

    return num;
}

void Z2SoundPlayer::correctSeNumber() {
    JAUSoundTable* soundTable = JASGlobalInstance<JAUSoundTable>::getInstance();
    JUT_ASSERT(594, soundTable);

    int num = soundTable->getNumItems_inGroup(0, field_0x3fa);
    if (num == 0) {
        field_0x3fc = 0;
    } else if (field_0x3fc >= num) {
        field_0x3fc = num - 1;
    }
}

void Z2SoundPlayer::onDrawSoundItem(JAWGraphContext* graf, JAUSoundNameTable* nameTable, int cursorY,
                                    const JUtility::TColor& color0, const JUtility::TColor& color1,
                                    const char* label, u32 sectionID, u32 groupID, u32 waveID)
{
    static const char szNoEntry[] = "(NO ENTRY)";

    const char* soundName = "";
    if (nameTable != NULL) {
        soundName = nameTable->getName(JAISoundID(sectionID, groupID, waveID));
        if (*soundName == 0) {
            soundName = szNoEntry;
        }
    } else {
        soundName = "";
    }

    // !@bug comparing strings directly instead of using strcmp
    if (soundName == szNoEntry) {
        graf->color(color1);
    } else {
        graf->color(color0);
    }

    graf->print(1, cursorY, "%s %03x %s", label, waveID, soundName);
}
