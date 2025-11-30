#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_save_HIO.h"
#include "d/d_com_inf_game.h"

dSvBit_childSwPerfectionHIO_c::dSvBit_childSwPerfectionHIO_c() {
    for (int i = 0; i < 0x80; i++) {
        field_0x5[i] = 0;
        field_0x85[i] = 0;
    }
}

void dSvBit_childSwPerfectionHIO_c::init() {
    for (int i = 0; i < 0x80; i++) {
        field_0x5[i] = dComIfGs_isSaveSwitch(i);
        field_0x85[i] = field_0x5[i];
    }
}

dSvBit_childSwDungeonHIO_c::dSvBit_childSwDungeonHIO_c() {
    for (int i = 0; i < 0x40; i++) {
        field_0x5[i] = 0;
        field_0x45[i] = 0;
    }
}

void dSvBit_childSwDungeonHIO_c::init() {
    for (int i = 0; i < 0x40; i++) {
        field_0x5[i] = dComIfGs_isSaveDunSwitch(i);
        field_0x45[i] = field_0x5[i];
    }
}

dSvBit_childSwZoneHIO_c::dSvBit_childSwZoneHIO_c() {
    for (int i = 0; i < 0x20; i++) {
        field_0x5[i] = 0;
        field_0x25[i] = 0;
    }
}

void dSvBit_childSwZoneHIO_c::init() {
    for (int i = 0; i < 0x20; i++) {
        field_0x5[i] = 0;
        field_0x25[i] = field_0x5[i];
    }
}

dSvBit_childSwOneZoneHIO_c::dSvBit_childSwOneZoneHIO_c() {
    for (int i = 0; i < 0x10; i++) {
        field_0x5[i] = 0;
        field_0x15[i] = 0;
    }
}

void dSvBit_childSwOneZoneHIO_c::init() {
    for (int i = 0; i < 0x10; i++) {
        field_0x5[i] = 0;
        field_0x15[i] = field_0x5[i];
    }
}

dSvBit_childItPerfectionHIO_c::dSvBit_childItPerfectionHIO_c() {
    for (int i = 0; i < 0x20; i++) {
        field_0x5[i] = 0;
        field_0x25[i] = 0;
    }
}

void dSvBit_childItPerfectionHIO_c::init() {
    for (int i = 0; i < 0x20; i++) {
        field_0x5[i] = dComIfGs_isSaveItem(i);
        field_0x25[i] = field_0x5[i];
    }
}

dSvBit_childItDungeonHIO_c::dSvBit_childItDungeonHIO_c() {
    for (int i = 0; i < 0x80; i++) {
        field_0x5[i] = 0;
        field_0x85[i] = 0;
    }
}

void dSvBit_childItDungeonHIO_c::init() {
    for (int i = 0; i < 0x80; i++) {
        field_0x5[i] = dComIfGs_isSaveDunItem(i);
        field_0x85[i] = field_0x5[i];
    }
}

dSvBit_childItZoneHIO_c::dSvBit_childItZoneHIO_c() {
    for (int i = 0; i < 0x20; i++) {
        field_0x5[i] = 0;
        field_0x25[i] = 0;
    }
}

void dSvBit_childItZoneHIO_c::init() {
    for (int i = 0; i < 0x20; i++) {
        field_0x5[i] = 0;
        field_0x25[i] = field_0x5[i];
    }
}

dSvBit_childItOneZoneHIO_c::dSvBit_childItOneZoneHIO_c() {
    for (int i = 0; i < 0x10; i++) {
        field_0x5[i] = 0;
        field_0x15[i] = 0;
    }
}

void dSvBit_childItOneZoneHIO_c::init() {
    for (int i = 0; i < 0x10; i++) {
        field_0x5[i] = 0;
        field_0x15[i] = field_0x5[i];
    }
}

dSvBit_childTbPerfectionHIO_c::dSvBit_childTbPerfectionHIO_c() {
    for (int i = 0; i < 0x40; i++) {
        field_0x5[i] = 0;
        field_0x45[i] = 0;
    }
}

void dSvBit_childTbPerfectionHIO_c::init() {
    for (int i = 0; i < 0x40; i++) {
        field_0x5[i] = dComIfGs_isTbox(i);
        field_0x45[i] = field_0x5[i];
    }
}

dSvBit_childDarknessHIO_c::dSvBit_childDarknessHIO_c() {
    for (int i = 0; i < 6; i++) {
        TW[i] = 0;
        field_0xB[i] = 0;
    }
}

void dSvBit_childDarknessHIO_c::init() {
    for (int i = 0; i < 6; i++) {
        TW[i] = dComIfGs_isDarkClearLV(i);
        field_0xB[i] = TW[i];
    }
}

dSvBit_childTransformHIO_c::dSvBit_childTransformHIO_c() {
    for (int i = 0; i < 4; i++) {
        WF[i] = 0;
        field_0x9[i] = 0;
    }
}

void dSvBit_childTransformHIO_c::init() {
    for (int i = 0; i < 4; i++) {
        WF[i] = dComIfGs_isTransformLV(i);
        field_0x9[i] = WF[i];
    }
}

dSvBit_childOtherHIO_c::dSvBit_childOtherHIO_c() {
    mDropNum = 0;
    field_0x6 = 0;
    for (int i = 0; i < 4; i++) {
        mLightVessel[i] = 0;
        field_0xb[i] = 0;
    }
}

void dSvBit_childOtherHIO_c::init() {
    mDropNum = dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea());
    field_0x6 = mDropNum;
    for (int i = 0; i < 4; i++) {
        mLightVessel[i] = dComIfGs_isLightDropGetFlag(i);
        field_0xb[i] = mLightVessel[i];
    }
}

void dSvBit_HIO_c::init() {
    mSwitch.init();
    mItem.init();
    mTreasure.init();
    mDarkness.init();
    mTransform.init();
    mOther.init();
                                   /* Main Event - Get shadow crystal (can now transform) */
    mTransformFlag = field_0x49d = dComIfGs_isEventBit(dSv_event_flag_c::M_077);
}

dSvBit_HIO_c g_save_bit_HIO;
