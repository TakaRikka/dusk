#include "Z2AudioLib/Z2WaveArcLoader.h"
#include "JSystem/JAudio2/JAUSectionHeap.h"
#include "JSystem/JAudio2/JASWaveInfo.h"

#include <bitset>

static f32 cStatusBar_X1 = 8.0f;
static f32 cStatusBar_X2 = 16.0f;
static f32 cWaveBar_X1 = 30.0f;
static f32 cWaveBar_X2 = 80.0f;
static f32 cBar_Y = 5.0f;
static f32 cBarHeight = 310.0f;

static u8 lbl_8074A738 = 10;
static u8 lbl_8074A739 = 23;

static u32 cWaveArcListSize = 10;

Z2WaveArcLoader::Z2WaveArcLoader() :
    JAWWindow("Z2WaveArcLoader", 420, 365)
{
    setWindowColor(0, 0, 0x50, 0xFF);

    mpWaveBank = NULL;
    mpWaveArc = NULL;
    mWaveUsedSize = 0;
    mTotalUsedSize = 0;
    mBankNo = 0;
    mArcCount = 0;
    field_0x408 = 0;
    field_0x40c = 0;
    field_0x410 = 0;
    field_0x414 = 0;
    mIsLoadTail = false;

    checkWaveBank();
    checkWaveArc();
}

Z2WaveArcLoader::~Z2WaveArcLoader() {}

// nonmatching with tree iterator stuff
void Z2WaveArcLoader::onDraw(JAWGraphContext* graf) {
    int cursorX = 8;
    JASHeap* rootheap = JASWaveArcLoader::getRootHeap();
    u32 aramsize = JASKernel::getAramSize();

    mWaveUsedSize = 0;

    for (u32 i = 0; i < 255; i++) {
        JASWaveBank* bank = JASGlobalInstance<JAUSectionHeap>::getInstance()->getWaveBankTable().getWaveBank(i);
        if (bank != NULL) {
            u32 arcCount = bank->getArcCount();
            for (int j = 0; j < arcCount; j++) {
                JASWaveArc* arc = bank->getWaveArc(j);
                if (arc != NULL && arc->getStatus() == 2) {
                    mWaveUsedSize += arc->getFileSize();
                }
            }
        }
    }

    u32 otherUsedSize = mTotalUsedSize - mWaveUsedSize;
    u32 totalFree = rootheap->getTotalFreeSize();
    u32 linearFreeSize = rootheap->getFreeSize();
    u32 sp54 = totalFree - linearFreeSize;

    graf->color(JUtility::TColor(0x32, 0x96, 0xFF, 0xFF), JUtility::TColor(0xC8, 0xC8, 0xFF, 0xFF));
    graf->print(cursorX, 0, "ARAM STATUS\n");

    graf->color(0xFF, 0xFF, 0xFF, 0xFF);
    graf->print(cursorX, 1, " total used    : %8d\n", mTotalUsedSize);

    graf->color(0xFF, 0x40, 0x40, 0xFF);
    graf->print(cursorX, 2, "  wave used    : %8d\n", mWaveUsedSize);

    graf->color(0xFF, 0xFF, 0x40, 0xFF);
    graf->print(cursorX, 3, "  other used   : %8d\n", otherUsedSize);

    graf->color(0x40, 0xFF, 0x40, 0xFF);
    graf->print(cursorX, 4, "  linear free  : %8d\n", linearFreeSize);

    graf->color(0xFF, 0xFF, 0xFF, 0xFF);
    graf->print(cursorX, 5, " total free    : %8d\n", totalFree);
    graf->print(cursorX, 6, " ------------------------\n");
    graf->print(cursorX, 7, " aram size     : %8d\n", aramsize);

    graf->color(0xFF, 0xFF, 0x20, 0xFF);
    f32 var_f30 = cBar_Y;
    f32 var_f31 = var_f30 + (((f32)otherUsedSize / (f32)aramsize) * cBarHeight);
    graf->fillBox(JGeometry::TBox2<f32>(cStatusBar_X1, var_f30, cStatusBar_X2, var_f31));

    graf->color(0xFF, 0x20, 0x20, 0xFF);
    var_f30 = var_f31;
    var_f31 += ((f32)mWaveUsedSize / (f32)aramsize) * cBarHeight;
    graf->fillBox(JGeometry::TBox2<f32>(cStatusBar_X1, var_f30, cStatusBar_X2, var_f31));

    graf->color(0x20, 0xFF, 0x20, 0xFF);
    var_f30 = var_f31;
    var_f31 += ((f32)linearFreeSize / (f32)aramsize) * cBarHeight;
    graf->fillBox(JGeometry::TBox2<f32>(cStatusBar_X1, var_f30, cStatusBar_X2, var_f31));

    graf->color(0x20, 0x20, 0xFF, 0xFF);
    var_f30 = var_f31;
    var_f31 += ((f32)sp54 / (f32)aramsize) * cBarHeight;
    graf->fillBox(JGeometry::TBox2<f32>(cStatusBar_X1, var_f30, cStatusBar_X2, var_f31));

    graf->color(0x40, 0x40, 0xFF, 0xFF);
    graf->fillBox(JGeometry::TBox2<f32>(cWaveBar_X1, cBar_Y, cWaveBar_X2, cBar_Y + cBarHeight));

    void* pbase = rootheap->getBase();
    u32 heapSize = rootheap->getSize();
    int sp48 = 0;

    mTotalUsedSize = 0;

    for (JSUTreeIterator<JASHeap> i = rootheap->getFirstChild(); (int)i != (int)rootheap->getEndChild(); ++i) {
        intptr_t sp40 = (char*)i->getBase() - (char*)pbase;
        u32 sp3C = i->getSize();
        mTotalUsedSize += sp3C;

        int sp38 = (sp40 * cBarHeight) / heapSize;
        int sp34 = (sp3C * cBarHeight) / heapSize;
        JGeometry::TBox2<f32> spD0(cWaveBar_X1, cBar_Y + (f32)sp38, cWaveBar_X2, (f32)sp34 + (cBar_Y + (f32)sp38));
        u8 sp9 = 0xFF;
        sp9 = sp48 % 2 ? (u8)0xC0 : (u8)0x80;
        graf->color(0xFF, sp9, 0x40, 0xFF);
        graf->fillBox(spD0);

        if (mpWaveArc != NULL) {
            if (mpWaveArc->getHeap() == i.getObject()) {
                graf->color(0xFF, 0xFF, 0xFF, 0xFF);
                graf->lineWidth(15);
                graf->drawFrame(spD0);
            }
        }

        sp48++;
    }

    graf->color(JUtility::TColor(0xC8, 0xC8, 0xFF, 0xFF), JUtility::TColor(0x32, 0x96, 0xFF, 0xFF));
    if (mIsLoadTail) {
        graf->print(cursorX, 9, "WAVE BANK    LOAD:Tail");
    } else {
        graf->print(cursorX, 9, "WAVE BANK    LOAD:Linear");
    }

    int var_r26 = lbl_8074A738;

    if (mpWaveBank == NULL) {
        graf->color(0x64, 0x64, 0x64, 0xFF);
        graf->print(cursorX, var_r26, " ws:%3d Not Registerd", mBankNo);

        graf->color(0xFF, 0xFF, 0xFF, 0xFF);
        graf->print(cursorX, var_r26, ">");
    } else {
        graf->color(0xFF, 0xFF, 0xFF, 0xFF);
        if (field_0x414 < 2) {
            graf->print(cursorX, field_0x414 + var_r26, ">");
        } else {
            graf->print(cursorX, field_0x414 + (var_r26 + 1), ">");
        }

        graf->print(cursorX, var_r26++, " ws:%3d", mBankNo);
        graf->print(cursorX, var_r26++, " aw:%3d-%3d /%3d", field_0x40c, field_0x410, mArcCount - 1);

        graf->color(0xFF, 0xA0, 0x64, 0xFF);
        graf->print(cursorX, var_r26++, "      aw_status  aw_size");

        graf->color(0xFF, 0xFF, 0xFF, 0xFF);

        for (int i = field_0x40c; i < field_0x410 + 1; i++) {
            JASWaveArc* arc = mpWaveBank->getWaveArc(i);
            if (arc == NULL) {
                break;
            }

            switch (arc->getStatus()) {
            case 0:
                graf->color(0x64, 0x64, 0x64, 0xFF);
                graf->print(cursorX, var_r26, "%3d:  NOT_LOAD  %8d", i, arc->getFileSize());
                break;
            case 1:
                graf->color(0xFF, 0x64, 0x64, 0xFF);
                graf->print(cursorX, var_r26, "%3d:  WAIT_LOAD %8d", i, arc->getFileSize());
                break;
            case 2:
                graf->color(0xFF, 0xFF, 0xC8, 0xFF);
                graf->print(cursorX, var_r26, "%3d:  LOAD      %8d", i, arc->getFileSize());
                break;
            }

            var_r26++;
        }
    }

    var_r26 = lbl_8074A739;
    graf->color(0x96, 0x96, 0xE1, 0xFF);
    graf->print(cursorX, var_r26++, "-----------------");
    graf->print(cursorX, var_r26++, " A ->  LOAD WAVE");
    graf->print(cursorX, var_r26++, " B -> ERASE WAVE");
    graf->print(cursorX, var_r26++, "-----------------");
}

void Z2WaveArcLoader::checkWaveBank() {
    JAUSectionHeap* sectionHeap = JASGlobalInstance<JAUSectionHeap>::getInstance();
    const JAUSectionHeap::TSectionHeapData& heapData = sectionHeap->getSectionHeapData();
    const JAUSection::TSectionData& sectionData = sectionHeap->getSectionData();

    if (sectionData.registeredWaveBankTables.test(mBankNo)) {
        mpWaveBank = heapData.waveBankTable.getWaveBank(mBankNo);
        mArcCount = mpWaveBank->getArcCount();
    } else {
        mpWaveBank = NULL;
        mArcCount = 0;
    }

    if (mBankNo == 1) {
        mIsLoadTail = true;
    } else {
        mIsLoadTail = false;
    }
}

void Z2WaveArcLoader::checkWaveArc() {
    field_0x40c = field_0x408 * cWaveArcListSize;
    if (field_0x40c > mArcCount - 1) {
        field_0x408--;
        field_0x40c = field_0x408 * cWaveArcListSize;
    }

    field_0x410 = (field_0x408 + 1) * cWaveArcListSize - 1;

    if (field_0x410 > mArcCount - 1) {
        field_0x410 = mArcCount - 1;
    }

    if (field_0x414 > 1) {
        if ((field_0x40c + field_0x414) - 2 > field_0x410) {
            field_0x414 = (field_0x410 - field_0x40c) + 2;
        }

        mpWaveArc = mpWaveBank->getWaveArc(field_0x40c + field_0x414 - 2);
    } else {
        mpWaveArc = NULL;
    }
}

void Z2WaveArcLoader::onKeyUp(const JUTGamePad&) {
    if (mpWaveBank == NULL) {
        field_0x414 = 0;
    } else {
        if (field_0x414 != 0) {
            field_0x414--;
        } else {
            field_0x414 = 11;
        }

        checkWaveArc();
    }
}

void Z2WaveArcLoader::onKeyDown(const JUTGamePad&) {
    if (mpWaveBank == NULL) {
        field_0x414 = 0;
    } else {
        if (field_0x414 < 11) {
            field_0x414++;
        } else {
            field_0x414 = 0;
        }

        checkWaveArc();
    }
}

void Z2WaveArcLoader::onKeyLeft(const JUTGamePad&) {
    if (field_0x414 == 0) {
        if (mBankNo != 0) {
            mBankNo--;
        } else {
            mBankNo = 0;
        }

        checkWaveBank();
    } else if (mpWaveBank != NULL) {
        if (field_0x408 != 0) {
            field_0x408--;
        } else {
            field_0x408 = 0;
        }

        checkWaveArc();
    }
}

void Z2WaveArcLoader::onKeyRight(const JUTGamePad&) {
    if (field_0x414 == 0) {
        if (mBankNo < 255) {
            mBankNo++;
        } else {
            mBankNo = 255;
        }

        checkWaveBank();
    } else if (mpWaveBank != NULL) {
        if (field_0x408 < 255) {
            field_0x408++;
        } else {
            field_0x408 = 255;
        }

        checkWaveArc();
    }
}

void Z2WaveArcLoader::onTrigA(const JUTGamePad&) {
    if (mpWaveArc != NULL && mpWaveArc->getStatus() == 0) {
        if (mIsLoadTail) {
            mpWaveArc->loadTail(NULL);
        } else {
            mpWaveArc->load(NULL);
        }
    }
}

void Z2WaveArcLoader::onTrigB(const JUTGamePad&) {
    if (mpWaveArc != NULL && mpWaveArc->getStatus() == 2) {
        mpWaveArc->erase();
    }
}

void Z2WaveArcLoader::onTrigZ(const JUTGamePad&) {
    mIsLoadTail = !mIsLoadTail;
}
