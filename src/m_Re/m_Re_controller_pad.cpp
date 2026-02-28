#include "m_Re/m_Re_controller_pad.h"

#include "JSystem/JKernel/JKRHeap.h"
#include "SSystem/SComponent/c_lib.h"
#include "SSystem/SComponent/c_m3d.h"
#include "Z2AudioCS/Z2AudioCS.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_graphic.h"
#include <revolution/types.h>
#include <cstring>

bool l_reset_requested;

mReCPd::Pad mReCPd::m_pad[WPAD_MAX_CONTROLLERS];
WPADInfo mReCPd::m_pad_info[WPAD_MAX_CONTROLLERS];
mReCPd::motorWave_t mReCPd::m_motorWave[WPAD_MAX_CONTROLLERS];
int mReCPd::m_cal_value;

JKRHeap* wpad_heap;

f32 l_dif_y_tbl[5] = {-0.3f, -0.15f, 0.0f, 0.15f, 0.3f};

void mReCPd::Pad::Acc::clean() {
    for (int i = 0; i < ARRAY_SIZE(field_0x0); i++) {
        field_0x0[i].x = field_0x0[i].y = field_0x0[i].z = 0.0f;
        field_0x5a0[i].x = field_0x5a0[i].y = field_0x5a0[i].z = 0.0f;
    }
    field_0xb40 = 0;
    m_swing = 0;
    field_0xb54 = 0.0f;
    m_swingVec.x = 0.0f;
    m_swingVec.y = 0.0f;
    field_0xb50 = 0;
    field_0xb51 = 1;
    PSMTXIdentity(field_0xb68);
    field_0xb98 = 0.0f;
    field_0xb9c = 0;
    field_0xbac = 0;
    field_0xba0 = 0.0f;
    field_0xba4 = 0.0f;
    field_0xbb0 = 0.0f;
    field_0xbb4 = 0.0f;
    field_0xbbc = 0;
    field_0xbc0 = 0;
}

void mReCPd::Pad::Acc::changeStts(u32 param_0, bool param_1) {
    if (param_1 != 0) {
        if (field_0xb40 & param_0) {
            m_swing &= ~param_0;
        } else {
            m_swing |= param_0;
        }
        field_0xb40 |= param_0;
        return;
    }
    field_0xb40 &= ~param_0;
    m_swing &= ~param_0;
}

bool cM3d_IsZero(f32 f);

// NONMATCHING - cM3d_IsZero inlining
void mReCPd::Pad::Acc::calcSwingVec() {
    Vec sp2C;
    Vec sp20;
    Vec sp14;
    Vec sp08;

    if (field_0xb51 != 0) {
        u32 var_r0 = 0;
        u32 var_r30 = 0;
        sp2C.z = 0.0f;
        sp2C.y = 0.0f;
        sp2C.x = 0.0f;
        field_0xb50 = 0;
        for (int i = 1; i < 30; i++) {
            if (field_0x0[i].z > 1.1f) {
                sp20 = field_0x5a0[i];
                sp14 = field_0x5a0[i - 1];
                PSVECSubtract(&sp20, &sp14, &sp08);
                PSVECScale(&sp08, &sp08, PSVECMag(&sp08));
                PSVECAdd(&sp2C, &sp08, &sp2C);
                var_r0 = 1;
            } else if (var_r0 == 1) {
                var_r30 = 1;
                break;
            }
        }

        if (var_r30 == 1 && cM3d_IsZero(PSVECSquareMag(&sp2C)) == 0) {
            sp2C.z = 0.0f;
            PSVECNormalize(&sp2C, &sp2C);
            m_swingVec.x = sp2C.x;
            m_swingVec.y = sp2C.y;
            field_0xb50 = 1;
        }
    }
}

void mReCPd::Pad::Acc::calcSttsRem() {
    f32 var_f25 = 8.0f;
    f32 var_f24 = -8.0f;
    f32 var_f23 = 8.0f;
    f32 var_f22 = -8.0f;
    f32 var_f21 = 8.0f;
    f32 var_f20 = -8.0f;
    f32 var_f19 = 8.0f;
    f32 var_f18 = -8.0f;
    f32 var_f17 = 8.0f;
    f32 var_f16 = -8.0f;
    f32 var_f15 = 8.0f;
    f32 var_f14 = -8.0f;

    bool x_over_2 = false;
    bool x_under_2 = false;
    bool y_over_2 = false;
    bool y_under_2 = false;
    bool z_over = false;
    bool z_under = false;
    bool var_r28 = false;
    bool var_r27 = false;
    bool var_r26 = false;
    bool var_r25 = false;
    bool var_r24 = false;
    bool x_under = false;
    bool x_over = false;
    bool y_under = false;
    bool y_over = false;
    bool z_over_2 = false;
    bool z_under_2 = false;
    int var_r17 = 0;

    field_0xb54 = 0.0f;

    Vec sp14;
    Vec sp8;
    for (int i = 0; i < 30; i++) {
        sp14 = field_0x5a0[i];
        sp8 = field_0x0[i];

        if (sp14.x > 0.63f) {
            x_over = true;
            if (x_under) {
                x_under = false;
            }
            x_over_2 = true;
            if (x_under_2 == true) {
                var_r17 = i;
                var_r28 = true;
                break;
            }
        }

        if (sp14.x < -0.63f) {
            x_under = true;
            if (x_over) {
                x_over = false;
            }
            x_under_2 = true;
            if (x_over_2 == true) {
                var_r17 = i;
                var_r27 = true;
                break;
            }
        }

        if (sp14.y > 0.7f) {
            y_over = true;
            if (y_under) {
                y_under = false;
            }
            y_over_2 = true;
            if (y_under_2 == true) {
                var_r17 = i;
                var_r25 = true;
                break;
            }
        }

        if (sp14.y < -0.4f) {
            y_under = true;
            if (y_over) {
                y_over = false;
            }
            y_under_2 = true;
            if (y_over_2 == true) {
                var_r17 = i;
                var_r26 = true;
                break;
            }
        }

        if (sp14.z > 0.28f) {
            z_over = true;
            if (z_under == true) {
                var_r17 = i;
                break;
            }
            z_over_2 = true;
            if (z_under_2) {
                z_under_2 = false;
            }
        }

        if (sp14.z < -0.28f) {
            z_under = true;
            if (z_over == true) {
                var_r17 = i;
                var_r24 = true;
                break;
            }
            z_under_2 = true;
            if (z_over_2) {
                z_over_2 = false;
            }
        }

        if (var_f25 > sp14.x) {
            var_f25 = sp14.x;
        }
        if (var_f24 < sp14.x) {
            var_f24 = sp14.x;
        }
        if (var_f23 > sp14.y) {
            var_f23 = sp14.y;
        }
        if (var_f22 < sp14.y) {
            var_f22 = sp14.y;
        }
        if (var_f21 > sp14.z) {
            var_f21 = sp14.z;
        }
        if (var_f20 < sp14.z) {
            var_f20 = sp14.z;
        }
        if (var_f19 > sp8.x) {
            var_f19 = sp8.x;
        }
        if (var_f18 < sp8.x) {
            var_f18 = sp8.x;
        }
        if (var_f17 > sp8.y) {
            var_f17 = sp8.y;
        }
        if (var_f16 < sp8.y) {
            var_f16 = sp8.y;
        }
        if (var_f15 > sp8.z) {
            var_f15 = sp8.z;
        }
        if (var_f14 < sp8.z) {
            var_f14 = sp8.z;
        }

        f32 temp_f1_2 = sp14.z * sp14.z + (sp14.x * sp14.x + sp14.y * sp14.y);
        if (field_0xb54 < temp_f1_2) {
            field_0xb54 = temp_f1_2;
        }
    }

    f32 temp_f2 = 0.0f;
    f32 temp_f1_3 = 0.0f;
    f32 temp_f0 = 0.0f;
    f32 var_f14_2 = 0.0f;
    f32 var_f15_2 = 0.0f;
    f32 var_f16_2 = 0.0f;

    for (int i = 0; i < 0x25; i++) {
        temp_f2 += field_0x0[i].x * field_0x0[i].x;
        temp_f1_3 += field_0x0[i].y * field_0x0[i].y;
        temp_f0 += field_0x0[i].z * field_0x0[i].z;
        var_f14_2 += field_0x5a0[i].x * field_0x5a0[i].x;
        var_f15_2 += field_0x5a0[i].y * field_0x5a0[i].y;
        var_f16_2 += field_0x5a0[i].z * field_0x5a0[i].z;
    }

    var_f20 = 0.0f;
    var_f19 = 0.0f;
    var_f18 = 0.0f;
    for (int i = 0; i < 29; i++) {
        var_f20 += cM3d_VectorProduct2d(field_0x5a0[0].x, field_0x5a0[0].y,
                                        field_0x5a0[i].x, field_0x5a0[i].y,
                                        field_0x5a0[i + 1].x, field_0x5a0[i + 1].y);
        var_f19 += cM3d_VectorProduct2d(field_0x5a0[0].x, field_0x5a0[0].z,
                                        field_0x5a0[i].x, field_0x5a0[i].z,
                                        field_0x5a0[i + 1].x, field_0x5a0[i + 1].z);
        var_f18 += cM3d_VectorProduct2d(field_0x5a0[0].z, field_0x5a0[0].y,
                                        field_0x5a0[i].z, field_0x5a0[i].y,
                                        field_0x5a0[i + 1].z, field_0x5a0[i + 1].y);
    }

    if (var_r17 < 0x25 && !(field_0xb40 & 0x100) && var_r27 == true && !(field_0xb40 & 0x8)) {
        if (field_0xb40 & 0x1) {
            m_swing &= ~0x1;
        } else {
            m_swing |= 0x1;
            if (field_0xbc0 > 0) {
                field_0xbc0 = 0;
            }
            field_0xbbc = 0x14;
        }
        field_0xb40 |= 0x1;
    } else {
        field_0xb40 &= ~0x1;
        m_swing &= ~0x1;
    }

    if (var_r17 < 0x25 && !(field_0xb40 & 0x100) && var_r28 == true && !(field_0xb40 & 0x8)) {
        if (field_0xb40 & 0x2) {
            m_swing &= ~0x2;
        } else {
            m_swing |= 0x2;
            if (field_0xbbc > 0) {
                field_0xbbc = 0;
            }
            field_0xbc0 = 0x14;
        }
        field_0xb40 |= 0x2;
    } else {
        field_0xb40 &= ~0x2;
        m_swing &= ~0x2;
    }

    bool var_r3_2 = y_over_2 == 1 && (int)y_under_2 == 0 && var_r24 == true && var_f17 < -1.3f;
    if (var_r3_2) {
        var_r24 = false;
    }

    if (var_r17 < 0x25 && !(field_0xb40 & 0x100) && (var_r25 == true || var_r3_2)) {
        if (field_0xb40 & 0x8) {
            m_swing &= ~0x8;
        } else {
            m_swing |= 0x8;
        }
        field_0xb40 |= 0x8;
    } else {
        field_0xb40 &= ~0x8;
        m_swing &= ~0x8;
    }

    if (var_r17 < 0x25 && !(field_0xb40 & 0x100) && var_r26 == true) {
        if (field_0xb40 & 0x4) {
            m_swing &= ~0x4;
        } else {
            m_swing |= 0x4;
        }
        field_0xb40 |= 0x4;
    } else {
        field_0xb40 &= ~0x4;
        m_swing &= ~0x4;
    }

    changeStts(0x10,
        var_f14_2 < 0.5f * var_f16_2 &&
        var_f15_2 < 0.45f * var_f16_2 &&
        var_f16_2 > 12.0f &&
        var_r17 < 0x25 &&
        var_r24);
    calcSwingVec();
}

void mReCPd::Pad::Acc::calcSttsFs() {
    f32 var_f25 = 8.0f;
    f32 var_f24 = -8.0f;
    f32 var_f23 = 8.0f;
    f32 var_f22 = -8.0f;
    f32 var_f21 = 8.0f;
    f32 var_f20 = -8.0f;
    f32 var_f19 = 8.0f;
    f32 var_f18 = -8.0f;
    f32 var_f17 = 8.0f;
    f32 var_f16 = -8.0f;
    f32 var_f15 = 8.0f;
    f32 var_f14 = -8.0f;

    bool x_over_2 = false;
    bool x_under_2 = false;
    bool y_over_2 = false;
    bool y_under_2 = false;
    bool z_over = false;
    bool z_under = false;
    bool var_r28 = false;
    bool var_r27 = false;
    bool var_r26 = false;
    bool var_r25 = false;
    bool var_r24 = false;
    bool x_under = false;
    bool x_over = false;
    bool y_under = false;
    bool y_over = false;
    bool z_over_2 = false;
    bool z_under_2 = false;

    int var_r17 = 0;

    field_0xb54 = 0.0f;

    Vec sp14;
    Vec sp8;

    for (int i = 0; i < 30; i++) {
        sp14 = field_0x5a0[i];
        sp8 = field_0x0[i];

        if (sp8.x > 0.5f) {
            x_over = true;
            if (x_under) {
                x_under = false;
            }
            x_over_2 = true;
            if (x_under_2 == true) {
                var_r17 = i;
                var_r28 = true;
                break;
            }
        }

        if (sp8.x < -0.5f) {
            x_under = true;
            if (x_over) {
                x_over = false;
            }
            x_under_2 = true;
            if (x_over_2 == true) {
                var_r17 = i;
                var_r27 = true;
                break;
            }
        }

        if (sp14.y > 0.7f) {
            y_over = true;
            if (y_under) {
                y_under = false;
            }
            y_over_2 = true;
            if (y_under_2 == true) {
                var_r25 = true;
            }
        }

        if (sp14.y < -0.4f) {
            y_under = true;
            if (y_over) {
                y_over = false;
            }
            y_under_2 = true;
            if (y_over_2 == true) {
                var_r26 = true;
            }
        }

        if (sp14.z > 0.28f) {
            z_over = true;
            if (z_under == true) {
                var_r17 = i;
                break;
            }
            z_over_2 = true;
            if (z_under_2) {
                z_under_2 = false;
            }
        }

        if (sp14.z < -0.28f) {
            z_under = true;
            if (z_over == true) {
                var_r17 = i;
                var_r24 = true;
                break;
            }
            z_under_2 = true;
            if (z_over_2) {
                z_over_2 = false;
            }
        }

        if (var_f25 > sp14.x) {
            var_f25 = sp14.x;
        }
        if (var_f24 < sp14.x) {
            var_f24 = sp14.x;
        }
        if (var_f23 > sp14.y) {
            var_f23 = sp14.y;
        }
        if (var_f22 < sp14.y) {
            var_f22 = sp14.y;
        }
        if (var_f21 > sp14.z) {
            var_f21 = sp14.z;
        }
        if (var_f20 < sp14.z) {
            var_f20 = sp14.z;
        }
        if (var_f19 > sp8.x) {
            var_f19 = sp8.x;
        }
        if (var_f18 < sp8.x) {
            var_f18 = sp8.x;
        }
        if (var_f17 > sp8.y) {
            var_f17 = sp8.y;
        }
        if (var_f16 < sp8.y) {
            var_f16 = sp8.y;
        }
        if (var_f15 > sp8.z) {
            var_f15 = sp8.z;
        }
        if (var_f14 < sp8.z) {
            var_f14 = sp8.z;
        }

        f32 temp_f1_2 = sp14.x * sp14.x + sp14.y * sp14.y + sp14.z * sp14.z;
        if (field_0xb54 < temp_f1_2) {
            field_0xb54 = temp_f1_2;
        }
    }

    f32 var_f16_2 = var_f16 - var_f17;

    f32 var_f18_2 = 0.0f;
    f32 var_f6 = 0.0f;
    f32 var_f17_2 = 0.0f;
    f32 var_f19_2 = 0.0f;
    f32 var_f7 = 0.0f;
    f32 var_f20_2 = 0.0f;
    for (int i = 0; i < 37; i++) {
        var_f18_2 += field_0x0[i].x * field_0x0[i].x;
        var_f6 += field_0x0[i].y * field_0x0[i].y;
        var_f17_2 += field_0x0[i].z * field_0x0[i].z;
        var_f19_2 += field_0x5a0[i].x * field_0x5a0[i].x;
        var_f7 += field_0x5a0[i].y * field_0x5a0[i].y;
        var_f20_2 += field_0x5a0[i].z * field_0x5a0[i].z;
    }

    f32 var_f23_2 = 0.0f;
    f32 var_f21_2 = 0.0f;
    f32 var_f22_2 = 0.0f;
    for (int i = 0; i < 29; i++) {
        var_f23_2 += cM3d_VectorProduct2d(field_0x5a0[0].x, field_0x5a0[0].y,
                             field_0x5a0[i].x, field_0x5a0[i].y,
                             field_0x5a0[i + 1].x, field_0x5a0[i + 1].y);
        var_f21_2 += cM3d_VectorProduct2d(field_0x5a0[0].x, field_0x5a0[0].z,
                                          field_0x5a0[i].x, field_0x5a0[i].z,
                                          field_0x5a0[i + 1].x, field_0x5a0[i + 1].z);
        var_f22_2 += cM3d_VectorProduct2d(field_0x5a0[0].z, field_0x5a0[0].y,
                                          field_0x5a0[i].z, field_0x5a0[i].y,
                                          field_0x5a0[i + 1].z, field_0x5a0[i + 1].y);
    }

    bool var_r14_3 = false;
    if (var_r17 < 0x25 &&
        var_f19_2 > 0.5f * var_f20_2 &&
        !(field_0xb40 & 0x100) &&
        var_r27 == 1 &&
        !(field_0xb40 & 0x8)) {
        if (field_0xb40 & 0x1) {
            m_swing &= ~0x1;
        } else {
            m_swing |= 0x1;
            if (field_0xbc0 > 0) {
                var_r14_3 = true;
                field_0xbc0 = 0;
            }
            field_0xbbc = 0x14;
        }
        field_0xb40 |= 0x1;
    } else {
        field_0xb40 &= ~0x1;
        m_swing &= ~0x1;
    }

    bool var_r16_2 = false;
    if (var_r17 < 0x25 &&
        var_f19_2 > 0.5f * var_f20_2 &&
        !(field_0xb40 & 0x100) &&
        var_r28 == true &&
        !(field_0xb40 & 0x8)) {
        if (field_0xb40 & 0x2) {
            m_swing &= ~0x2;
        } else {
            m_swing |= 0x2;
            if (field_0xbbc > 0) {
                var_r16_2 = true;
                field_0xbbc = 0;
            }
            field_0xbc0 = 0x14;
        }
        field_0xb40 |= 0x2;
    } else {
        field_0xb40 &= ~0x2;
        m_swing &= ~0x2;
    }

    if (!(field_0xb40 & 0x100) &&
        (var_r25 == 1 ||
         (y_over_2 == 1 && y_under_2 == 0 && var_r24 == 1 && var_f16_2 > 2.5f)) &&
        fabsf(var_f21_2) < 3.0f &&
        fabsf(var_f22_2) > 3.0f &&
        var_f22_2 > 0.0f) {
        if (field_0xb40 & 8) {
            m_swing &= ~0x8;
        } else {
            m_swing |= 8;
        }
        field_0xb40 |= 8;
    } else {
        field_0xb40 &= ~0x8;
        m_swing &= ~0x8;
    }

    if (!(field_0xb40 & 0x100) &&
        var_r26 == 1 &&
        fabsf(var_f21_2) < 3.0f &&
        fabsf(var_f22_2) > 3.0f &&
        var_f22_2 < 0.0f) {
        if (field_0xb40 & 0x4) {
            m_swing &= ~0x4;
        } else {
            m_swing |= 4;
        }
        field_0xb40 |= 4;
    } else {
        field_0xb40 &= ~0x4;
        m_swing &= ~0x4;
    }

    changeStts(0x10, var_f18_2 < 0.5f * var_f17_2 &&
        var_r17 < 0x25 &&
        ((field_0xb40 & 0x100 && field_0xb98 > 0.28f) || var_r24 != 0));
    bool temp_r5 = 0.5f * (var_f15 + var_f14) < -0.7f;
    changeStts(0x100, temp_r5);
    int temp_r0 = field_0xbbc - 1;
    field_0xbbc = temp_r0;
    if (temp_r0 < 0) {
        field_0xbbc = 0;
    }

    int temp_r0_2 = field_0xbc0 - 1;
    field_0xbc0 = temp_r0_2;
    if (temp_r0_2 < 0) {
        field_0xbc0 = 0;
    }

    if (var_r14_3) {
        if (field_0xb40 & 0x80) {
            m_swing &= ~0x80;
        } else {
            m_swing |= 0x80;
        }
        field_0xb40 |= 0x80;
    } else {
        field_0xb40 &= ~0x80;
        m_swing &= ~0x80;
    }

    if (var_r16_2 != 0) {
        if (field_0xb40 & 0x40) {
            m_swing &= ~0x40;
        } else {
            m_swing |= 0x40;
        }
        field_0xb40 |= 0x40;
    } else {
        field_0xb40 &= ~0x40;
        m_swing &= ~0x40;
    }
}

void mReCPd::Pad::clean() {
    field_0x554 = 0;
    field_0x0 = 0;
    m_stick3D.x = m_stick3D.y = 0.0f;
    m_stickValue = 0.0f;
    m_stickAngle = 0;
    m_stick3DAngle = 0;
    cLib_memSet(&field_0x4, 0, sizeof(field_0x4));
    cLib_memSet(&field_0x558, 0, sizeof(field_0x558));
    m_remAcc.clean();
    m_FSAcc.clean();
    field_0x1dd4 = 0;
    field_0x1d78.x = field_0x1d78.y = 0.0f;
    field_0x1d80.x = field_0x1d80.y = 0.0f;
    m_dpd_2d_pos.x = m_dpd_2d_pos.y = 0.0f;
    m_dpd_ratio_pos.x = m_dpd_ratio_pos.y = 0.0f;
    field_0x1d98 = field_0x1d9c = 0.0f;
    m_dpd_play_box_pos.x = m_dpd_play_box_pos.y = 0.0f;
    field_0x1db0.x = field_0x1db0.y = 0.0f;
    field_0x1db8 = field_0x1dbc = 0.0f;
    field_0x1da8.x = field_0x1da8.y = 0.0f;
    field_0x1dc0 = 0.0f;
    field_0x1dc4 = 0;
    field_0x1dc8 = 0;
    m_swingBlock = 0;
    field_0x1dd0 = 0;
    field_0x1dd8 = 0;
    field_0x1de0 = 0;
    field_0x1de4 = 0;
    field_0x1dec = 0;
    field_0x1df4 = 0;
    dev_type = WPAD_DEV_INITIALIZING;
    field_0x1dfc = -1;
    field_0x1e00 = 0;
    field_0x1e04 = 0;
}

void mReCPd::Pad::cleanOneTime() {
#if PLATFORM_SHIELD
    field_0x1ddd = 0;
    field_0x1de8 = 0;
    field_0x1df0 = 0;
#else
    field_0x1df0 = 0;
    field_0x1ddd = 0;
    field_0x1de8 = 0;
#endif
    field_0x1ddc = false;
    field_0x1e05 = 0;
}

void mReCPd::Pad::copyRemAcc() {
    for (int i = 0x77 - field_0x0; i >= 0; i--) {
        m_remAcc.field_0x0[i + field_0x0] = m_remAcc.field_0x0[i];
        m_remAcc.field_0x5a0[i + field_0x0] = m_remAcc.field_0x5a0[i];
    }

    f32 var_f30 = -8.0f;
    int var_r27 = 0;
    for (int i = 0; i < field_0x0; i++) {
        m_remAcc.field_0x0[i] = field_0x4[i].acc;
        if (var_f30 < m_remAcc.field_0x0[i].y) {
            var_f30 = m_remAcc.field_0x0[i].y;
            var_r27 = 1;
        }
        PSMTXMultVec(m_remAcc.field_0xb68, &m_remAcc.field_0x0[i], &m_remAcc.field_0x5a0[i]);
        m_remAcc.field_0x5a0[i].y += 1.0f;
    }

    if (var_r27 != 0) {
        m_remAcc.field_0xb98 = var_f30;
    }
}

void mReCPd::Pad::copyFSAcc() {
    for (int i = 0x77 - field_0x0; i >= 0; i--) {
        m_FSAcc.field_0x0[i + field_0x0] = m_FSAcc.field_0x0[i];
        m_FSAcc.field_0x5a0[i + field_0x0] = m_FSAcc.field_0x5a0[i];
    }

    f32 var_f30 = -8.0f;
    int var_r27 = 0;
    for (int i = 0; i < field_0x0; i++) {
        m_FSAcc.field_0x0[i] = field_0x4[i].ex_status.fs.acc;
        if (var_f30 < m_FSAcc.field_0x0[i].y) {
            var_f30 = m_FSAcc.field_0x0[i].y;
            var_r27 = 1;
        }
        PSMTXMultVec(m_FSAcc.field_0xb68, &m_FSAcc.field_0x0[i], &m_FSAcc.field_0x5a0[i]);
        m_FSAcc.field_0x5a0[i].y += 1.0f;
    }

    if (var_r27 != 0) {
        m_FSAcc.field_0xb98 = var_f30;
    }
}

void mReCPd::Pad::cleanTrig() {
    m_remAcc.m_swing = 0;
    m_FSAcc.m_swing = 0;

    for (int i = 0; i < 10; i++) {
        field_0x4[i].trig = 0;
    }
}

void mReCPd::Pad::cleanHold() {
    for (int i = 0; i < 10; i++) {
        field_0x4[i].hold = 0;
    }
}

void mReCPd::Pad::cleanRelease() {
    for (int i = 0; i < 10; i++) {
        field_0x4[i].release = 0;
    }
}

void mReCPd::Pad::clearUpDown() {
    if (field_0x4[0].trig & 0x1 || field_0x4[0].trig & 0x2 ||
        field_0x4[0].hold & 0x1 || field_0x4[0].hold & 0x2) {
        field_0x4[0].trig &= ~0xC;
        field_0x4[0].hold = field_0x4[0].hold & ~0xC;
    }
}

void mReCPd::Pad::calcDpdBox(Vec2* arg0, Vec2* arg1, f32 arg2, f32 arg3) {
    Vec2 sp8;

    f32 x_min;
    f32 x_max;
    x_max = arg1->x;
    if (x_max > 0.0f) {
        x_min = 0.0f;
    } else {
        x_min = x_max;
        x_max = 0.0f;
    }

    f32 y_min;
    f32 y_max;
    y_max = arg1->y;
    if (y_max > 0.0f) {
        y_min = 0.0f;
    } else {
        y_min = y_max;
        y_max = 0.0f;
    }

    sp8.x = (arg1->x * -arg3) / arg1->y;
    sp8.y = -arg3 + 0.001f;
    if (x_min < sp8.x && x_max > sp8.x && y_min < sp8.y && y_max > sp8.y &&
        -arg2 <= sp8.x && arg2 >= sp8.x && -arg3 <= sp8.y && arg3 >= sp8.y) {
        arg0->x = sp8.x;
        arg0->y = sp8.y;
        return;
    }

    sp8.x = arg2 - 0.001f;
    sp8.y = (arg1->y * arg2) / arg1->x;
    if (x_min < sp8.x && x_max > sp8.x && y_min < sp8.y && y_max > sp8.y &&
        -arg2 <= sp8.x && arg2 >= sp8.x && -arg3 <= sp8.y && arg3 >= sp8.y) {
        *arg0 = sp8;
        return;
    }

    sp8.x = (arg1->x * arg3) / arg1->y;
    sp8.y = arg3 - 0.001f;
    if (x_min < sp8.x && x_max > sp8.x && y_min < sp8.y && y_max > sp8.y &&
        -arg2 <= sp8.x && arg2 >= sp8.x && -arg3 <= sp8.y && arg3 >= sp8.y) {
        *arg0 = sp8;
        return;
    }

    sp8.x = 0.001f + -arg2;
    sp8.y = (arg1->y * -arg2) / arg1->x;
    *arg0 = sp8;
}

// NONMATCHING - JMAFastSqrt inlining
void mReCPd::Pad::calcDpdOld() {
    Vec2 sp8;

    field_0x1db0 = field_0x1da8;
    sp8 = field_0x1db0;
    if (field_0x0 != 0) {
        bool var_r30 = false;
        for (int i = 0; i < field_0x0; i++) {
            if ((s32)(u32)field_0x4[i].dpd_valid_fg == 1 || (s32)(u32)field_0x4[i].dpd_valid_fg == 2) {
                var_r30 = true;
                sp8 = field_0x4[i].pos;
                field_0x1d78 = sp8;
                field_0x1d80 = field_0x4[i].vec;
                break;
            }
        }

        if (var_r30 == 0) {
            sp8.x = sp8.x + field_0x1d80.x;
            sp8.y += field_0x1d80.y;
        }
    }

    f32 temp_f2 = mDoGph_gInf_c::getHeightF() / mDoGph_gInf_c::getWidthF();
    if (-1.0f < sp8.x && 1.0f > sp8.x && -temp_f2 < sp8.y && temp_f2 > sp8.y) {
        field_0x1da8 = sp8;
    } else {
        calcDpdBox(&field_0x1da8, &sp8, 1.0f, temp_f2);
        field_0x1d80.x = field_0x1d80.y = 0.0f;
    }

    field_0x1db8 = field_0x1da8.x - field_0x1db0.x;
    field_0x1dbc = field_0x1da8.y - field_0x1db0.y;
    field_0x1dc0 = JMAFastSqrt(field_0x1db8 * field_0x1db8 + field_0x1dbc * field_0x1dbc);
}

void mReCPd::enableDpd(u32 param_0) {
    KPADEnableDPD(param_0);
}

void mReCPd::disableDpd(u32 param_0) {
    KPADDisableDPD(param_0);
}

void mReCPd::Pad::calcDpd2DPos() {
    Vec2 sp08 = field_0x1da8;
    sp08.x += 1.0f;
    f32 ratio = mDoGph_gInf_c::getWidthF() / mDoGph_gInf_c::getHeightF();
    int diff_x = (mDoGph_gInf_c::getMaxX() - mDoGph_gInf_c::getMinX()) >> 1;
    m_dpd_2d_pos.x = diff_x * sp08.x + mDoGph_gInf_c::getMinX();
    int diff_y = (mDoGph_gInf_c::getMaxY() - mDoGph_gInf_c::getMinY()) >> 1;
    m_dpd_2d_pos.y = diff_y + (ratio * (diff_y * sp08.y) + mDoGph_gInf_c::getMinY());
}

void mReCPd::Pad::calcDpdRatioPos() {
    f32 ratio = mDoGph_gInf_c::getWidthF() / mDoGph_gInf_c::getHeightF();
    m_dpd_ratio_pos.x = field_0x1da8.x;
    m_dpd_ratio_pos.y = field_0x1da8.y * ratio;
}

void mReCPd::Pad::calcDpdPlayCirPos() {
    Vec2 sp28 = field_0x1da8;

    f32 mag = JMAFastSqrt(sp28.x * sp28.x + sp28.y * sp28.y);
    if (mag <= 0.3f) {
        field_0x1d98 = 0.0f;
        field_0x1d9c = 0.0f;
        return;
    }

    Vec2 sp20;
    sp20.x = sp28.x / mag;
    sp20.y = sp28.y / mag;

    Vec2 sp18;
    sp18.x = fabsf(sp28.x / mag);
    sp18.y = fabsf(sp20.y);

    f32 ratio = mDoGph_gInf_c::getHeightF() / mDoGph_gInf_c::getWidthF();

    f32 var_f2;
    if (cM3d_IsZero(sp18.x)) {
        var_f2 = (mag - 0.3f) / (ratio - 0.3f);
    } else if (sp18.y / sp18.x > ratio) {
        Vec2 sp10;
        sp10.x = ratio * (sp18.x / sp18.y);
        sp10.y = ratio;
        f32 mag2 = JMAFastSqrt(sp10.x * sp10.x + sp10.y * sp10.y);
        var_f2 = (mag - 0.3f) / (mag2 - 0.3f);
        if (var_f2 > 1.0f) {
            var_f2 = 1.0f;
        }
    } else {
        Vec2 sp08;
        sp08.x = 1.0f;
        sp08.y = sp18.y / sp18.x;
        f32 mag2 = JMAFastSqrt(sp08.x * sp08.x + sp08.y * sp08.y);
        var_f2 = (mag - 0.3f) / (mag2 - 0.3f);
        if (var_f2 > 1.0f) {
            var_f2 = 1.0f;
        }
    }

    field_0x1d98 = sp20.x * var_f2;
    field_0x1d9c = sp20.y * var_f2;
}

void mReCPd::Pad::calcDpdPlayBoxPos() {
    Vec2 sp10 = m_dpd_ratio_pos;
    Vec2 sp8 = sp10;
    if (sp8.x < 0.0f) {
        sp8.x *= -1.0f;
    }
    if (sp8.y < 0.0f) {
        sp8.y *= -1.0f;
    }

    f32 max;
    if (sp8.x <= 0.5f && sp8.y <= 0.5f) {
        max = 0.0f;
    } else {
        sp8.x = sp8.x - 0.5f;
        sp8.y = sp8.y - 0.5f;
        if (sp8.y > sp8.x) {
            max = sp8.y / 0.5f;
        } else {
            max = sp8.x / 0.5f;
        }
    }

    f32 mag = JMAFastSqrt(sp10.x * sp10.x + sp10.y * sp10.y);
    if (cM3d_IsZero(max) || cM3d_IsZero(mag)) {
        mag = 1.0f;
    }

    m_dpd_play_box_pos.x = sp10.x * max / mag;
    m_dpd_play_box_pos.y = sp10.y * max / mag;
}

void mReCPd::onConnect(u32 i_padNo) {
    m_pad[i_padNo].field_0x1dd4 = 1;
    m_pad[i_padNo].field_0x1e04 = 1;
}

// NONMATCHING: stack restore order
static void l_mReCPd_extensionCallback(s32 i_padNo, s32 param_1) {
    Z2AudioCS::extensionProcess(i_padNo, param_1);
    mReCPd::m_pad[i_padNo].field_0x1dec = param_1;
    mReCPd::m_pad[i_padNo].field_0x1df0 = 1;
}

void mReCPd::setExtensionCallback() {
    WPADSetExtensionCallback(0, l_mReCPd_extensionCallback);
}

/*static*/ void l_mReCPd_connectCallback(s32 param_0, s32 param_1) {
    switch (param_1) {
    case 0:
        mReCPd::m_pad[param_0].field_0x1e05 = 1;
        WPADSetExtensionCallback(0, l_mReCPd_extensionCallback);
        break;
    case -1:
        mReCPd::m_pad[param_0].field_0x1e05 = 0;
        break;
    }
}

/*static*/ void l_mReCPd_connectCallbackForDisconnect(s32 param_0, s32 param_1) {
    if (param_1 == 0) {
        mReCPd::m_pad[param_0].field_0x1e05 = 1;
    }
}

void mReCPd::onGetInfoAsync(u32 i_padNo) {
    m_pad[i_padNo].field_0x1ddd = 1;
    m_pad[i_padNo].field_0x1de0 = 1800;
}

static BOOL wpad_ok() {
    OSThread* thread = OSGetCurrentThread();
    if (thread != NULL && thread->state == 2) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void* wpad_alloc(u32 size) {
    if (wpad_ok()) {
        return wpad_heap->alloc(size, 0);
    }
    return FALSE;
}

// NONMATCHING - string reference is wrong, probably needs -ipa program
static BOOL wpad_free(void* ptr) {
    if (wpad_ok()) {
        wpad_heap->free(ptr);
        return TRUE;
    } else {
        OSReport_Warning("wpad_free %08x failed.\n", ptr);
        return FALSE;
    }
}

void mReCPd::create() {
    memset(m_pad_info, 0, 0x60);
    wpad_heap = JKRHeap::sRootHeap2;
    WPADRegisterAllocator(wpad_alloc, wpad_free);
    KPADInit();
    KPADSetObjInterval(0.2f);

    for (int i = 0; i < 4; i++) {
        KPADSetPosParam(i, 0.05f, 0.08f);
        KPADSetAccParam(i, 0.1f, 0.55f);
    }

    for (int i = 0; i < 4; i++) {
        m_pad[i].clean();
        m_pad[i].cleanOneTime();
        m_pad[i].field_0x1dd0 = i;
    }

    l_reset_requested = false;
    m_cal_value = NULL;

    WPADSetConnectCallback(0, l_mReCPd_connectCallback);
    WPADSetConnectCallback(1, l_mReCPd_connectCallbackForDisconnect);
    WPADSetConnectCallback(2, l_mReCPd_connectCallbackForDisconnect);
    WPADSetConnectCallback(3, l_mReCPd_connectCallbackForDisconnect);
    WPADSetExtensionCallback(0, l_mReCPd_extensionCallback);
    WPADSetAcceptConnection(1);
    KPADSetFSStickClamp(0xF, 0x4E);

#if PLATFORM_SHIELD
    WPADSetAutoSleepTime(0);
#endif
}

void mReCPd::startMoterWave(motorWave_t* wave, u8* param_1, bool param_2) {
    wave->field_0x4 = ((param_1[0] << 8) & 0xFF00) | param_1[1];
    wave->field_0x0 = param_1 + 2;
    wave->field_0x6 = 0;
    wave->field_0x8 = param_2;
}

void mReCPd::startMoterWave(int wave_idx, u8* param_1, bool param_2) {
    startMoterWave(&m_motorWave[wave_idx], param_1, param_2);
}

void mReCPd::stopMoterWave(motorWave_t* wave) {
    if (wave->field_0x0 == 0) {
        return;
    }

    wave->field_0x6 = wave->field_0x4;
    wave->field_0x8 = 0;
}

void mReCPd::stopMoterWave(int wave_idx) {
    stopMoterWave(&m_motorWave[wave_idx]);
}

void mReCPd::updateMoterWave() {
    motorWave_t* wave = &m_motorWave[0];
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++, wave++) {

        if (wave->field_0x0 == NULL) {
            break;
        }

        if (wave->field_0x6 >= wave->field_0x4) {
            if (wave->field_0x8 == 0) {
                wave->field_0x0 = NULL;
                WPADControlMotor(i, 0);
                continue;
            }

            wave->field_0x6 = 0;
        }

        int temp1 = 0x80 >> (wave->field_0x6 & 0x7);
        int temp2 = wave->field_0x0[wave->field_0x6 >> 3];
        int temp_r4 = temp1 & temp2;
        WPADControlMotor(i, temp_r4 != 0);
        wave->field_0x6++;
    }
}

void mReCPd::stopMoter() {
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        WPADControlMotor(i, 0);
    }
}

BOOL mReCPd::getLowBat(u32 chan) {
    return m_pad_info[chan].lowBat;
}

void mReCPd::setLowBat(u32 chan, BOOL low_bat) {
    m_pad_info[chan].lowBat = low_bat;
}

void mReCPd::onLowBatChk(u32 chan) {
    m_pad[chan].field_0x1ddc = true;
}

void mReCPd_getInfoAsync(s32 chan, s32 param_1) {
    mReCPd::onGetInfoAsync(chan);
}

void mReCPd::procNoData(u32 chan) {
    m_pad[chan].cleanTrig();

    KPADStatus* status = &m_pad[chan].field_0x4[0];
    if (m_pad[chan].field_0x554 > 5) {
        m_pad[chan].field_0x554 = 6;
        status->ex_status.fs.stick.x = 0.0f;
        status->ex_status.fs.stick.y = 0.0f;
    }

    m_pad[chan].field_0x0 = 1;
    f32 temp_x;
    f32 temp_y;
    f32 mult = 1.0f / 1.1f;
    temp_x = status->pos.x * mult;
    temp_y = status->pos.y * mult;
    status->pos.x = temp_x + status->vec.x;
    status->pos.y = temp_y + status->vec.y;
}

// NONMATCHING - r17/r18 regswap, weird m_pad load issue
void mReCPd::read() {
    if (m_pad[0].field_0x1e05 != 0) {
        if (m_pad[0].field_0x1dd4 == 0) {
            Z2AudioCS::connect(0);
            onConnect(0);
        }
    } else {
        if (m_pad[0].field_0x1dd4 != 0) {
            Z2AudioCS::disconnect(0);
            m_pad[0].clean();
        }
    }

    for (int i = 1; i < WPAD_MAX_CONTROLLERS; i++) {
        if (m_pad[i].field_0x1e05 != 0) {
            if (m_pad[i].field_0x1dd4 == 0) {
                onConnect(i);
            }
            m_pad[i].field_0x1e05 = 0;
        }
    }

    if (m_pad[0].field_0x1dd4 != 0) {
        if (WPADGetAcceptConnection() != 0 && m_pad[0].field_0x1dd8 < 300) {
            WPADSetAcceptConnection(FALSE);
        } else {
            if (WPADGetAcceptConnection() != 1) {
                if (m_pad[0].field_0x1dd8 >= 300) {
                    WPADSetAcceptConnection(TRUE);
                }
            }
        }

        if (m_pad[0].field_0x1ddc != 0) {
            if ((m_pad[0].field_0x1e04 != 0 && m_pad[0].field_0x1e00 > 60) || m_pad[0].field_0x1e00 > 120) {
                m_pad[0].field_0x1e00 = 0;
                m_pad[0].field_0x1e04 = 0;

                if (WPADGetInfoAsync(0, &m_pad_info[0], mReCPd_getInfoAsync) != 0) {
                    onGetInfoAsync(0);
                }
            } else {
                m_pad[0].field_0x1e00++;
            }
        } else {
            m_pad[0].field_0x1e00 = 0;
        }

        if (m_pad[0].field_0x1de0 <= 0) {
            m_pad[0].field_0x1de0 = 0;
            m_pad_info[0].lowBat = FALSE;
        } else {
            m_pad[0].field_0x1de0--;
        }
    } else {
        if (WPADGetAcceptConnection() != TRUE) {
            WPADSetAcceptConnection(TRUE);
        }
    }

    for (int i = 1; i < WPAD_MAX_CONTROLLERS; i++) {
        if (m_pad[i].field_0x1dd4 != 0) {
            m_pad[i].clean();
            WPADDisconnect(i);
        }
    }

    Pad* pad = &m_pad[0];
    u32* dev_type;
    for (u32 i = 0; i < WPAD_MAX_CONTROLLERS; i++, pad++) {
        dev_type = &m_pad[i].dev_type;
        m_pad[i].field_0x1dfc = WPADProbe(i, dev_type);
        if (m_pad[i].field_0x1dfc == 0 && *dev_type == WPAD_DEV_CORE) {
            if (++m_pad[i].field_0x1df4 >= 20) {
                m_pad[i].field_0x1df4 = 20;
            }
        } else {
            m_pad[i].field_0x1df4 = 0;
        }

        m_pad[i].field_0x1de4 = m_pad[i].field_0x1dec;

        if (m_pad[i].field_0x1df0 != 0) {
            m_pad[i].field_0x1de8 = 1;
        }
        KPADSetSensorHeight(i, l_dif_y_tbl[m_cal_value + 2]);
        KPADSetPosParam(i, 0.05f, 0.08f);

        daPy_py_c* player = static_cast<daPy_py_c*>(dComIfGp_getPlayer(0));
        if (player != NULL && player->checkWolfHowlDemoMode()) {
            KPADSetAccParam(i, 0.1f, 0.35f);
        } else {
            KPADSetAccParam(i, 0.1f, 0.55f);
        }

        // not sure if this aggregate is real, but it's the only way I can find to account for the
        // extra stack space without breaking codegen
        struct {
            KPADStatus unused_0x0[RECPD_SAMPLING_BUF_COUNT];
            KPADStatus sampling_bufs[RECPD_SAMPLING_BUF_COUNT];
        } unk_buf_struct;
        int read_count = KPADRead(i, unk_buf_struct.sampling_bufs,
                                  ARRAY_SIZE(unk_buf_struct.sampling_bufs));
        m_pad[i].field_0x0 = 0;
        for (int j = 0; j < read_count; j++) {
            if (unk_buf_struct.sampling_bufs[j].wpad_err == WPAD_ERR_OK) {
                m_pad[i].field_0x4[m_pad[i].field_0x0] = unk_buf_struct.sampling_bufs[j];
                m_pad[i].field_0x0++;
            }
        }

        if (m_pad[i].field_0x0 == 0) {
            m_pad[i].field_0x554++;
            procNoData(i);
        } else {
            m_pad[i].field_0x554 -= 2;
            if (m_pad[i].field_0x554 > 0) {
                procNoData(i);
            } else {
                m_pad[i].field_0x554 = 0;
                m_pad[i].field_0x558 = m_pad[i].field_0x4[0];
            }
        }

        if (m_pad[i].field_0x0 != 0) {
            if (m_pad[i].field_0x1de4 != 1) {
                for (int j = 0; j < m_pad[i].field_0x0; j++) {
                    KPADStatus* status = &m_pad[i].field_0x4[j];
                    status->hold &= ~0x6000;
                    status->trig &= ~0x6000;
                    status->release &= ~0x6000;
                    status->ex_status.fs.stick.x = 0.0f;
                    status->ex_status.fs.stick.y = 0.0f;
                    status->ex_status.fs.acc.x = 0.0f;
                    status->ex_status.fs.acc.y = 0.0f;
                    status->ex_status.fs.acc_value = 0.0f;
                    status->ex_status.fs.acc_speed = 0.0f;
                }
            }

            for (int j = 0; j < m_pad[i].field_0x0; j++) {
                KPADStatus* status = &m_pad[i].field_0x4[j];
                status->pos.x *= 1.1f;
                status->pos.y *= 1.1f;
            }

            if (--m_pad[i].m_swingBlock < 0) {
                m_pad[i].m_swingBlock = 0;
            }

            if (getKPADAccValueMax(i) > 1.3f) {
                m_pad[i].m_swingBlock = 10;
            }

            Pad::Acc* rem_acc = &m_pad[i].m_remAcc;
            calcUnderVec(rem_acc, getKPADAccValueMax(i));
            m_pad[i].copyRemAcc();
            rem_acc->calcSttsRem();
            m_pad[i].calcDpdOld();
            m_pad[i].calcDpd2DPos();
            m_pad[i].calcDpdRatioPos();
            m_pad[i].calcDpdPlayCirPos();
            m_pad[i].calcDpdPlayBoxPos();

            if (*dev_type == WPAD_DEV_FS) {
                KPADEXStatus& ex_status = m_pad[i].field_0x4[0].ex_status;

                m_pad[i].m_stick3D = ex_status.fs.stick;

                m_pad[i].m_stick3D.x *= -1.0f;
                Vec2& stick = ex_status.fs.stick;
                m_pad[i].m_stickValue = JMAFastSqrt(stick.x * stick.x + stick.y * stick.y);
                m_pad[i].m_stickValue *= 1.0005003f;
                if (m_pad[i].m_stickValue > 1.0f) {
                    m_pad[i].m_stickValue = 1.0f;
                }

                if (m_pad[i].m_stickValue > 0.0f) {
                    if (ex_status.fs.stick.y == 0.0f) {
                        if (ex_status.fs.stick.x > 0.0f) {
                            m_pad[i].m_stickAngle = 0x4000;
                        } else {
                            m_pad[i].m_stickAngle = -0x4000;
                        }
                    } else {
                        m_pad[i].m_stickAngle =
                            RAD2S((f32)atan2(ex_status.fs.stick.x, -ex_status.fs.stick.y));
                    }
                }

                m_pad[i].m_stick3DAngle = m_pad[i].m_stickAngle;
                m_pad[i].m_stick3DAngle *= -1;

                Pad::Acc* fs_acc = &m_pad[i].m_FSAcc;
                calcUnderVec(fs_acc, getKPADFSAccValueMax(i));
                m_pad[i].copyFSAcc();
                fs_acc->calcSttsFs();
            } else {
                m_pad[i].m_stick3D.x = 0.0f;
                m_pad[i].m_stick3D.y = 0.0f;
                m_pad[i].m_stickValue = 0.0f;
            }
        }

        if (read_count != 0) {
            m_pad[i].field_0x1dd8 = 0;
        } else if (++m_pad[i].field_0x1dd8 > 300) {
            m_pad[i].field_0x1dd8 = 300;
            m_pad[i].cleanHold();
            m_pad[i].cleanRelease();

            for (u32 j = 0; j < ARRAY_SIZE(m_pad[i].field_0x4); j++) {
                m_pad[i].field_0x4[j].ex_status.fs.stick.x = 0.0f;
                m_pad[i].field_0x4[j].ex_status.fs.stick.y = 0.0f;
                m_pad[i].field_0x4[j].ex_status.fs.acc.x = 0.0f;
                m_pad[i].field_0x4[j].ex_status.fs.acc.y = 0.0f;
                m_pad[i].field_0x4[j].ex_status.fs.acc_value = 0.0f;
                m_pad[i].field_0x4[j].ex_status.fs.acc_speed = 0.0f;
            }

            m_pad[i].m_stick3D.x = 0.0f;
            m_pad[i].m_stick3D.y = 0.0f;
            m_pad[i].m_stickValue = 0.0f;
            m_pad[i].cleanTrig();
        }

        m_pad[i].clearUpDown();
    }

    if (OSGetResetButtonState() != 0) {
        l_reset_requested = true;
    } else {
        if (l_reset_requested == true) {
            mDoRst_resetCallBack(-1, NULL);
        }
        l_reset_requested = false;
    }
}

// NONMATCHING - float regalloc
f32 mReCPd::calibrateDist(int param_0) {
    f32 var_f31 = mDoGph_gInf_c::getWidthF() / (param_0 / (6.0f / 25.0f));
    f32 max = 10000.0f;
    f32 min = 0.0f;
    f32 temp_f1_0 = WPADGetDpdSensitivity() + 1.0f;
    f32 var_f23 = (temp_f1_0 - 0.5f) / 50.0f;
    f32 temp_f1_1 = 1.0f - fabsf(l_dif_y_tbl[m_cal_value + 2]);

    for (int i = 0; i < 50; i++) {
        f32 var_f22 = i * var_f23;
        f32 var_f26 = cM_atan2f(var_f31, var_f22);
        f32 temp_f1_2 = cM_atan2f(temp_f1_1 * 0.2f / 2.0f, var_f22);
        f32 var_f0_0 = fabsf(var_f26 - (DEG_TO_RAD(21) - temp_f1_2));
        if (max > var_f0_0) {
            max = var_f0_0;
            min = var_f22;
        }
    }

    if (min < 1.0f) {
        min = 1.0f;
    }

    return min;
}

Vec2& mReCPd::getDpd2DPos(u32 chan) {
    return m_pad[chan].m_dpd_2d_pos;
}

Vec2& mReCPd::getDpdRatioPos(u32 chan) {
    return m_pad[chan].m_dpd_ratio_pos;
}

Vec2& mReCPd::getDpdPlayBoxPos(u32 chan) {
    return m_pad[chan].m_dpd_play_box_pos;
}

BOOL mReCPd::chkDpdOk(u32 chan) {
    return TRUE;
}

BOOL mReCPd::chkDpdValid(u32 chan) {
    BOOL result = TRUE;
    BOOL valid = m_pad[chan].field_0x4[0].dpd_valid_fg;
    if (valid != 1 && valid != 2) {
        result = FALSE;
    }
    return result;
}

BOOL mReCPd::chkDpdPosIn(u32 chan) {
    Pad* pad = &m_pad[chan];

    if (pad->field_0x0 == 0) {
        return 0;
    }

    if (!chkDpdValid(chan)) {
        return 0;
    }

    f32 ratio = mDoGph_gInf_c::getHeightF() / mDoGph_gInf_c::getWidthF();
    Vec2 pos = pad->field_0x4[0].pos;

    if (pos.x < -1.0f) {
        return 0;
    }
    if (pos.x > 1.0f) {
        return 0;
    }
    if (pos.y < -ratio) {
        return 0;
    }
    if (pos.y > ratio) {
        return 0;
    }

    return chkDpdValid(chan);
}

void mReCPd::getKPADAcc(u32 chan, Vec* param_1, int param_2) {
    if (m_pad[chan].field_0x0 == 0) {
        param_1->x = 0.0f;
        param_1->y = 0.0f;
        param_1->z = 0.0f;
    } else {
        if (param_2 >= m_pad[chan].field_0x0) {
            param_2 = m_pad[chan].field_0x0 - 1;
        }
        *param_1 = m_pad[chan].field_0x4[param_2].acc;
    }
}

s16 mReCPd::getAngleXy(u32 chan) {
    f32 x = m_pad[chan].field_0x4[0].acc.x;
    f32 y = m_pad[chan].field_0x4[0].acc.y;
    if (x > 1.0f) {
        x = 1.0f;
    } else if (x < -1.0f) {
        x = -1.0f;
    }
    if (y > 1.0f) {
        y = 1.0f;
    } else if (y < -1.0f) {
        y = -1.0f;
    }
    return cM_atan2s(x, -y);
}

s16 mReCPd::getAngleZy(u32 chan) {
    f32 z = m_pad[chan].field_0x4[0].acc.z;
    f32 y = m_pad[chan].field_0x4[0].acc.y;
    if (z > 1.0f) {
        z = 1.0f;
    } else if (z < -1.0f) {
        z = -1.0f;
    }
    if (y > 1.0f) {
        y = 1.0f;
    } else if (y < -1.0f) {
        y = -1.0f;
    }
    return cM_atan2s(z, -y);
}

f32 mReCPd::getKPADAccValue(u32 chan, int param_1) {
    int temp_r3 = m_pad[chan].field_0x0;
    if (temp_r3 == 0) {
        return 0.0f;
    }

    int status_idx = param_1;
    if (status_idx >= temp_r3) {
        status_idx = temp_r3 - 1;
    }

    return m_pad[chan].field_0x4[status_idx].acc_value;
}

f32 mReCPd::getKPADAccValueMax(u32 chan) {
    f32 max = 0.0f;
    if (m_pad[chan].field_0x0 == 0) {
        return max;
    }

    for (int i = 0; i < m_pad[chan].field_0x0; i++) {
        f32 cur_val = getKPADAccValue(chan, i);
        if (max < cur_val) {
            max = cur_val;
        }
    }

    return max;
}

f32 mReCPd::getKPADAccSpeed(u32 chan, int param_1) {
    int temp_r3 = m_pad[chan].field_0x0;
    if (temp_r3 == 0) {
        return 0.0f;
    }

    int status_idx = param_1;
    if (status_idx >= temp_r3) {
        status_idx = temp_r3 - 1;
    }

    return m_pad[chan].field_0x4[status_idx].acc_speed;
}

f32 mReCPd::getKPADAccSpeedMax(u32 chan) {
    f32 max = 0.0f;
    if (m_pad[chan].field_0x0 == 0) {
        return max;
    }

    for (int i = 0; i < m_pad[chan].field_0x0; i++) {
        f32 cur_val = getKPADAccSpeed(chan, i);
        if (max < cur_val) {
            max = cur_val;
        }
    }

    return max;
}

f32 mReCPd::getKPADFSAccValue(u32 chan, int param_1) {
    int temp_r3 = m_pad[chan].field_0x0;
    if (temp_r3 == 0) {
        return 0.0f;
    }

    int status_idx = param_1;
    if (status_idx >= temp_r3) {
        status_idx = temp_r3 - 1;
    }

    return m_pad[chan].field_0x4[status_idx].ex_status.fs.acc_value;
}

f32 mReCPd::getKPADFSAccValueMax(u32 chan) {
    f32 max = 0.0f;
    if (m_pad[chan].field_0x0 == 0) {
        return max;
    }

    for (int i = 0; i < m_pad[chan].field_0x0; i++) {
        f32 cur_val = getKPADFSAccValue(chan, i);
        if (max < cur_val) {
            max = cur_val;
        }
    }

    return max;
}

s16 mReCPd::getFSStickAngle3DRev(u32 chan) {
    return m_pad[chan].m_stick3DAngle;
}

void mReCPd::calcUnderVec(Pad::Acc* acc_p, f32 param_1) {
    JMAFastSqrt(acc_p->field_0x0[0].x * acc_p->field_0x0[0].x +
                acc_p->field_0x0[0].y * acc_p->field_0x0[0].y +
                acc_p->field_0x0[0].z * acc_p->field_0x0[0].z);

    if (param_1 <= 1.1f && acc_p->field_0x0[0].y < 0.35f) {
        int temp_r0 = acc_p->field_0xb64 + 1;
        acc_p->field_0xb64 = temp_r0;
        if (temp_r0 >= 6) {
            acc_p->field_0xb64 = 6;
            acc_p->field_0xb58 = acc_p->field_0x0[0];

            Vec sp20 = {0.0f, -1.0f, 0.0f};
            if (cM3d_IsZero(PSVECSquareMag(&acc_p->field_0xb58)) == 0) {
                Vec sp14;
                Vec sp8 = acc_p->field_0xb58;
                PSVECNormalize(&sp8, &sp8);
                PSVECCrossProduct(&sp8, &sp20, &sp14);
                f32 dot = PSVECDotProduct(&sp8, &sp20);
                if (cM3d_IsZero(PSVECSquareMag(&sp14)) == 0) {
                    PSVECNormalize(&sp14, &sp14);
                    PSMTXRotAxisRad(acc_p->field_0xb68, &sp14, acos(dot));
                    return;
                }
                PSMTXIdentity(acc_p->field_0xb68);
                return;
            }
            PSMTXIdentity(acc_p->field_0xb68);
        }
    } else {
        acc_p->field_0xb64 = 0;
    }
}

u32 mReCPd::getHoldB(u32 i_padNo) {
    return m_pad[i_padNo].field_0x4[0].hold & WPAD_BUTTON_B;
}

u32 mReCPd::getHoldMinus(u32 i_padNo) {
    if (chkSwingBlock(i_padNo)) {
        return 0;
    } else {
        return m_pad[i_padNo].field_0x4[0].hold & WPAD_BUTTON_MINUS;
    }
}

u32 mReCPd::getHoldPlus(u32 i_padNo) {
    if (chkSwingBlock(i_padNo)) {
        return 0;
    } else {
        return m_pad[i_padNo].field_0x4[0].hold & WPAD_BUTTON_PLUS;
    }
}

u32 mReCPd::getHoldZ1(u32 i_padNo) {
    return m_pad[i_padNo].field_0x4[0].hold & WPAD_BUTTON_Z;
}

u32 mReCPd::getTrigB(u32 i_padNo) {
    return m_pad[i_padNo].field_0x4[0].trig & WPAD_BUTTON_B;
}

u32 mReCPd::getTrigMinus(u32 i_padNo) {
    if (chkSwingBlock(i_padNo)) {
        return 0;
    } else {
        return m_pad[i_padNo].field_0x4[0].trig & WPAD_BUTTON_MINUS;
    }
}

u32 mReCPd::getTrigHome(u32 i_padNo) {
    if (chkSwingBlock(i_padNo)) {
        return 0;
    } else {
        return m_pad[i_padNo].field_0x4[0].trig & WPAD_BUTTON_HOME;
    }
}

u32 mReCPd::getTrigPlus(u32 i_padNo) {
    if (chkSwingBlock(i_padNo)) {
        return 0;
    } else {
        return m_pad[i_padNo].field_0x4[0].trig & WPAD_BUTTON_PLUS;
    }
}

u32 mReCPd::getTrigZ1(u32 i_padNo) {
    return m_pad[i_padNo].field_0x4[0].trig & WPAD_BUTTON_Z;
}

u32 mReCPd::chkSwingBlock(u32 i_padNo) {
    if (m_pad[i_padNo].m_swingBlock > 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

u32 mReCPd::getTrigSwing(u32 i_padNo) {
    return m_pad[i_padNo].m_remAcc.m_swing;
}

Vec2& mReCPd::getSwingVec(u32 i_padNo) {
    return m_pad[i_padNo].m_remAcc.m_swingVec;
}

u32 mReCPd::chkSimpleProbe(u32 i_padNo) {
    if (m_pad[i_padNo].field_0x1dd4 == 0) {
        return 0;
    } else {
        u32 field_0x1dfc = m_pad[i_padNo].field_0x1dfc;
        u32 field_0x1df8 = m_pad[i_padNo].dev_type;
        if (field_0x1dfc + 3 <= 2) {
            return 0;
        } else if (field_0x1df8 == 0xFD || field_0x1df8 == 0xFF) {
            return 0;
        } else {
            return 1;
        }
    }
}

BOOL mReCPd::chkDevTypeValid(u32 i_padNo) {
    if (!chkSimpleProbe(i_padNo)) {
        return FALSE;
    }

    Pad* pad = &m_pad[i_padNo];
    if (pad->field_0x1df4 < 20 &&
        pad->field_0x1de4 == 0xFF &&
        pad->field_0x1de8 != 0) {
        return FALSE;
    }

    return TRUE;
}

u32 mReCPd::getFSTrigSwing(u32 i_padNo) {
    return m_pad[i_padNo].m_FSAcc.m_swing;
}

void mReCPd::plusCalValue() {
    m_cal_value++;
    if (m_cal_value >= 2) {
        m_cal_value = 2;
    }
}

void mReCPd::minusCalValue() {
    m_cal_value--;
    if (m_cal_value <= -2) {
        m_cal_value = -2;
    }
}
