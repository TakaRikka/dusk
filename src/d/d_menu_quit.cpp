#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_menu_quit.h"
#include "d/d_lib.h"

dMq_HIO_c g_mqHIO;

dMq_HIO_c::~dMq_HIO_c() {}

dMq_HIO_c::dMq_HIO_c() {}

void dMq_HIO_c::genMessage(JORMContext*) {}

typedef void (dMenu_Quit_c::*moveProc)();
static moveProc move_process[] = {
    &dMenu_Quit_c::select1_proc,
    &dMenu_Quit_c::select2_proc,
    &dMenu_Quit_c::messageChange,
};

// stripped, but based on asserts it was likely the constructor goes here
dMenu_Quit_c::dMenu_Quit_c() {
    JUT_ASSERT(0, mStick != NULL);
    JUT_ASSERT(0, mpScrnExplain != NULL);
}

dMenu_Quit_c::~dMenu_Quit_c() {
    delete mStick;
    delete mpScrnExplain;
    mpScrnExplain = NULL;
}

void dMenu_Quit_c::select1_init() {
    field_0x14 = 1;
    field_0x1c = 1;
    msgTxtSet(0x4B2, false);  // "Continue?"
    field_0x12 = 0;
}

void dMenu_Quit_c::select1_proc() {
    bool isSelect = YesNoSelect();
    if (isSelect) {
        if (field_0x14 == 1 || field_0x14 == 2) {
            field_0x10 = 0;
            field_0x11 = 1;
        } else {
            select2_init();
        }
    }
}

void dMenu_Quit_c::select2_init() {
    field_0x14 = 0;
    field_0x1c = 1;
    msgTxtSet(0x4B3, false);  // "Really quit?"
    field_0x12 = 1;
}

void dMenu_Quit_c::select2_proc() {
    bool isSelect = YesNoSelect();
    if (isSelect) {
        if (field_0x14 == 1) {
            mDoAud_bgmStop(30);
            dComIfGp_setNextStage("name", 0, 0, 0);
        } else {
            select1_init();
        }
    }
}

void dMenu_Quit_c::messageChange() {
    bool rt;

    if (mpScrnExplain->getStatus() != dMsgScrnExplain_c::STATUS_WAIT_e) {
        if (mpScrnExplain->getStatus() == dMsgScrnExplain_c::STATUS_MOVE_e || mpScrnExplain->getStatus() == dMsgScrnExplain_c::STATUS_MOVE_SELECT_e) {
            mpScrnExplain->onForceSelect();
        }

        mpScrnExplain->move();
    } else {
        if (field_0x1c == 1) {
            rt = mpScrnExplain->openExplain(mMsgId, 1, field_0x14 != 0 ? 0 : 1, 0xFF, field_0x20);
        } else {
            rt = mpScrnExplain->openExplain(mMsgId, 0, 0, 0xFF, true);
        }

        if (rt == 1) {
            field_0x12 = field_0x13;
        }
    }
}

bool dMenu_Quit_c::YesNoSelect() {
    bool rt = false;
    if (mpScrnExplain->getStatus() == dMsgScrnExplain_c::STATUS_WAIT_e) {
        rt = true;
        field_0x1c = 0;
    }

    if (field_0x22 == 4) {
        switch (mpScrnExplain->getSelectCursor()) {
        case 0:
            field_0x14 = 1;
            break;
        case 1:
            field_0x14 = 0;
            break;
        case 0xFF:
            field_0x14 = 2;
            break;
        }
    }

    if (field_0x22 == 4 && mpScrnExplain->getStatus() == dMsgScrnExplain_c::STATUS_CLOSE_e) {
        if (field_0x12 == 0) {
            mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);
        } else if (field_0x12 == 1) {
            if (field_0x14 == 1) {
                mDoAud_seStart(Z2SE_QUIT_GAME, NULL, 0, 0);
            } else {
                mDoAud_seStart(Z2SE_SY_CURSOR_CANCEL, NULL, 0, 0);
            }
        }
    }

    return rt;
}

void dMenu_Quit_c::msgTxtSet(u16 i_msgId, bool param_1) {
    param_1 = true;

    if (i_msgId != 0xFFFF) {
        bool rt;
        if (field_0x1c == 1) {
            rt = mpScrnExplain->openExplain(i_msgId, 1, field_0x14 != 0 ? 0 : 1, 0xFF, param_1);
        } else {
            rt = mpScrnExplain->openExplain(i_msgId, 0, 0, 0xFF, true);
        }

        if (!rt) {
            field_0x20 = param_1;
            mMsgId = i_msgId;
            field_0x13 = field_0x12;
            field_0x12 = 2;
        }
    }

    field_0x21 = 0;
}

void dDlst_MenuQuitExplain_c::draw() {
    mpExplain->draw((J2DOrthoGraph*)dComIfGp_getCurrentGrafPort());
}
