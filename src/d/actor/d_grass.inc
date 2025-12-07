#include "d/actor/d_a_grass.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"
#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "SSystem/SComponent/c_lib.h"
#include "f_op/f_op_overlap_mng.h"
#include <dolphin/gf.h>

#include "d/d_camera.h"
#include "f_op/f_op_camera_mng.h"

const u16 l_M_Hijiki00TEX__width = 31;
const u16 l_M_Hijiki00TEX__height = 31;
#include "assets/l_M_kusa05_RGBATEX.h"  // ALIGN 32

const u16 l_M_kusa05_RGBATEX__width = 31;
const u16 l_M_kusa05_RGBATEX__height = 31;
#include "assets/l_M_Hijiki00TEX.h"  // ALIGN 32

static u8 l_pos[960] = {
    0x3F, 0x4A, 0x56, 0xEF, 0xC2, 0x20, 0x00, 0x00, 0x41, 0xFB, 0x17, 0xE4, 0x41, 0xAA, 0xBB, 0xEA,
    0xC2, 0x20, 0x00, 0x00, 0xC1, 0xB7, 0x03, 0x7A, 0x42, 0x55, 0x8D, 0x6F, 0x43, 0x13, 0x16, 0x3F,
    0xC0, 0xC7, 0x24, 0x07, 0x41, 0xE3, 0xC5, 0x31, 0x43, 0x13, 0x16, 0x3F, 0x42, 0x3A, 0x76, 0x90,
    0xC1, 0xDE, 0xF6, 0x89, 0xC2, 0x20, 0x00, 0x00, 0xC1, 0x4E, 0xFE, 0x02, 0xC2, 0x28, 0x22, 0x86,
    0x43, 0x13, 0x16, 0x3F, 0xC2, 0x07, 0x6E, 0x2C, 0x41, 0x51, 0x51, 0xEE, 0x43, 0x13, 0x16, 0x3F,
    0xC2, 0x54, 0xE3, 0xFA, 0xC1, 0x89, 0xD7, 0x9B, 0x43, 0x13, 0x16, 0x3F, 0x42, 0x47, 0x60, 0x87,
    0xC2, 0x59, 0x1D, 0x0E, 0x43, 0x13, 0x16, 0x3F, 0x40, 0x95, 0x9B, 0x11, 0x3F, 0x3A, 0x78, 0x8E,
    0x43, 0x13, 0x16, 0x3F, 0x41, 0xE0, 0xD9, 0x17, 0xC1, 0xE3, 0x1B, 0x10, 0x43, 0x13, 0x16, 0x3F,
    0x42, 0x5A, 0xDD, 0x48, 0xC1, 0xEE, 0x39, 0x52, 0xC2, 0x20, 0x00, 0x00, 0x41, 0xC4, 0x63, 0x09,
    0x00, 0x00, 0x00, 0x00, 0xC2, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x03, 0x56, 0x8B,
    0xC2, 0x20, 0x00, 0x00, 0x41, 0x08, 0xF1, 0x43, 0x42, 0x14, 0xBE, 0x15, 0x43, 0x13, 0x16, 0x3F,
    0x42, 0x3E, 0x56, 0x15, 0x41, 0x9A, 0x86, 0xFF, 0x43, 0x13, 0x16, 0x3F, 0xC1, 0xA7, 0xC6, 0xC3,
    0x42, 0x6A, 0xD4, 0x7A, 0x43, 0x13, 0x16, 0x3F, 0xC1, 0x97, 0x6D, 0x9D, 0xC1, 0x19, 0xCD, 0xF3,
    0xC2, 0x20, 0x00, 0x00, 0xC2, 0x15, 0xFA, 0x6B, 0x40, 0xF2, 0xA2, 0x21, 0x43, 0x13, 0x16, 0x3F,
    0xC2, 0x72, 0x6C, 0x7F, 0xC1, 0xC9, 0x8C, 0x79, 0x43, 0x13, 0x16, 0x3F, 0xC1, 0x3F, 0xB7, 0x0B,
    0xC2, 0x12, 0xFB, 0xE4, 0x43, 0x13, 0x16, 0x3F, 0xC2, 0x46, 0xC9, 0xEB, 0xC2, 0x69, 0xB2, 0x46,
    0x43, 0x13, 0x16, 0x3F, 0x41, 0x49, 0x97, 0x1C, 0x3E, 0xBA, 0x78, 0x8E, 0x42, 0x0E, 0x6B, 0x09,
    0x41, 0x60, 0xD9, 0x17, 0xC1, 0xE8, 0xAA, 0x31, 0x42, 0x0E, 0x6B, 0x09, 0x42, 0x1E, 0x87, 0x66,
    0xC1, 0xEE, 0x39, 0x52, 0xC1, 0xA0, 0x00, 0x00, 0x41, 0xC4, 0x63, 0x09, 0x00, 0x00, 0x00, 0x00,
    0xC1, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x03, 0x56, 0x8B, 0xC1, 0xA0, 0x00, 0x00,
    0x41, 0x08, 0xF1, 0x43, 0x42, 0x0C, 0x0A, 0x50, 0x42, 0x0E, 0x6B, 0x09, 0x41, 0xE0, 0x92, 0x67,
    0x41, 0x1A, 0x86, 0xFF, 0x42, 0x0E, 0x6B, 0x09, 0xC1, 0x27, 0xC6, 0xC3, 0x42, 0x37, 0x15, 0x83,
    0x42, 0x0E, 0x6B, 0x09, 0xC0, 0xA5, 0xE9, 0xF7, 0xC1, 0x19, 0xCD, 0xF3, 0xC1, 0xA0, 0x00, 0x00,
    0xC2, 0x15, 0xFA, 0x6B, 0xBF, 0x81, 0xF3, 0x8C, 0x42, 0x0E, 0x6B, 0x09, 0xC2, 0x44, 0x33, 0x76,
    0xC1, 0x49, 0x8C, 0x7A, 0x42, 0x0E, 0x6B, 0x09, 0xC0, 0xBF, 0xB7, 0x0B, 0xC1, 0xB9, 0x6F, 0x60,
    0x42, 0x0E, 0x6B, 0x09, 0xC2, 0x2E, 0x62, 0x2B, 0xC2, 0x30, 0x67, 0x78, 0x42, 0x0E, 0x6B, 0x09,
    0x41, 0x94, 0x97, 0x4B, 0x3F, 0x4A, 0x56, 0xEF, 0xC1, 0xA0, 0x00, 0x00, 0x41, 0xFB, 0x17, 0xE4,
    0x41, 0xAA, 0xBB, 0xEA, 0xC1, 0xA0, 0x00, 0x00, 0xC1, 0xB7, 0x03, 0x7A, 0x42, 0x15, 0x75, 0xB2,
    0x41, 0xD4, 0xFE, 0xE0, 0xC1, 0x68, 0xCC, 0x7C, 0x41, 0x6A, 0x17, 0xE7, 0x41, 0xD4, 0xFE, 0xE0,
    0x42, 0x1C, 0x01, 0x41, 0xC1, 0xDE, 0xF6, 0x89, 0xC1, 0xA0, 0x00, 0x00, 0xC1, 0x4E, 0xFE, 0x02,
    0xC2, 0x0B, 0xCE, 0xE5, 0x41, 0xD4, 0xFE, 0xE0, 0xC1, 0xBB, 0x2D, 0xAD, 0x41, 0x89, 0xB2, 0x71,
    0x41, 0xD4, 0xFE, 0xE0, 0xC2, 0x18, 0x32, 0xDB, 0xC1, 0x03, 0x84, 0xE4, 0x41, 0xD4, 0xFE, 0xE0,
    0x42, 0x22, 0x76, 0x3C, 0xC2, 0x24, 0x4C, 0x29, 0x41, 0xD4, 0xFE, 0xE0, 0xC0, 0x84, 0x30, 0x7B,
    0x41, 0x47, 0x75, 0x1E, 0x43, 0x9F, 0x77, 0x6C, 0x42, 0xE0, 0x3F, 0xC6, 0x41, 0x8A, 0x76, 0x75,
    0xC2, 0x05, 0x2D, 0xB7, 0xC1, 0xCF, 0xDC, 0xAC, 0xC1, 0x8C, 0x34, 0xB2, 0xC2, 0x3A, 0xD2, 0x49,
    0x40, 0xA1, 0xFC, 0xF0, 0x43, 0x07, 0x5C, 0xA5, 0x43, 0x37, 0x70, 0xEE, 0x42, 0x80, 0x8E, 0x59,
    0x40, 0xAF, 0x73, 0x3F, 0x40, 0x5B, 0xFC, 0x44, 0x41, 0xA2, 0xC5, 0x58, 0xBE, 0x8F, 0xF1, 0x0F,
    0x41, 0xD6, 0xD6, 0xFB, 0xC0, 0xFF, 0xAA, 0x6F, 0x42, 0x47, 0x87, 0xFB, 0x43, 0x71, 0x58, 0x6A,
    0xC2, 0x6A, 0x7B, 0x21, 0x41, 0x79, 0x6D, 0x34, 0x42, 0x7D, 0x14, 0x66, 0x41, 0xC5, 0x98, 0xA7,
    0xC1, 0x3D, 0x22, 0x18, 0x42, 0x94, 0x1C, 0x80, 0x41, 0xF6, 0xE0, 0x36, 0x42, 0xDF, 0xB0, 0x65,
    0x43, 0xA2, 0xC0, 0xAF, 0xC1, 0x24, 0x3C, 0xD0, 0xC1, 0xD2, 0x1A, 0x2E, 0xC1, 0xD5, 0xC7, 0x38,
    0xC1, 0x71, 0xB4, 0x9D, 0x40, 0x99, 0x06, 0xE6, 0xC2, 0x20, 0x88, 0x2D, 0x41, 0x9D, 0xD0, 0xD9,
    0x41, 0xBB, 0x21, 0x46, 0x43, 0x39, 0xA7, 0xB9, 0xC3, 0x11, 0x8D, 0x20, 0x41, 0x8D, 0xE2, 0x64,
    0x40, 0xB4, 0xD7, 0xAE, 0xC0, 0xDD, 0x14, 0x64, 0xC1, 0x02, 0x49, 0xD6, 0x41, 0xE8, 0x8D, 0x5E,
    0x40, 0xBF, 0xC4, 0x15, 0xC1, 0xD9, 0x0C, 0xDC, 0x43, 0x68, 0x5A, 0x22, 0xC2, 0x4F, 0xE7, 0x5E,
    0x41, 0x82, 0x49, 0x5F, 0x42, 0x5A, 0x19, 0xB6, 0xC1, 0x89, 0xC1, 0xA5, 0x41, 0xB3, 0x90, 0xED,
    0x42, 0x82, 0x9F, 0x28, 0x41, 0x23, 0x0C, 0x02, 0xC1, 0x8D, 0x1C, 0xF7, 0x43, 0x9F, 0x77, 0x6C,
    0xC2, 0xE5, 0xDF, 0x4C, 0xC1, 0xB3, 0xD8, 0xDD, 0xC2, 0x05, 0x2D, 0xB7, 0x41, 0xB9, 0x5E, 0x95,
    0x41, 0x45, 0xA4, 0x94, 0xC2, 0x3A, 0xD2, 0x49, 0xC0, 0xFB, 0xF5, 0x4D, 0xC3, 0x16, 0x18, 0x11,
    0x43, 0x39, 0xA7, 0xB9, 0xC2, 0x07, 0x6F, 0xB1, 0xC1, 0x71, 0x27, 0xE6, 0x40, 0xB4, 0xD7, 0xAE,
    0xC1, 0xAE, 0x78, 0x69, 0xC0, 0x0A, 0xEE, 0xAA, 0x41, 0xE8, 0x8D, 0x5E, 0x40, 0x82, 0x3B, 0x99,
    0xC2, 0x33, 0x95, 0x59, 0x43, 0x5F, 0x75, 0x5E, 0x3D, 0xF4, 0x6A, 0x1A, 0xC1, 0x9E, 0xD4, 0x98,
    0x42, 0x7D, 0x14, 0x66, 0xC1, 0xDD, 0xD4, 0x93, 0x40, 0xF1, 0xCC, 0x38, 0x42, 0x94, 0x1C, 0x80,
    0xC2, 0x07, 0x8E, 0x11, 0xC2, 0xE0, 0xEF, 0x0C, 0x43, 0xA0, 0x12, 0xC6, 0xC1, 0x91, 0xCE, 0x9E,
    0x41, 0x9D, 0x73, 0x77, 0xC2, 0x00, 0x52, 0xDD, 0x41, 0xB1, 0x90, 0xD0, 0xBF, 0xA5, 0x21, 0x04,
    0xC2, 0x35, 0xF7, 0x70, 0xC1, 0x9B, 0xE3, 0x38, 0xC2, 0x84, 0xB6, 0xB8, 0x43, 0x43, 0x0C, 0x16,
    0x42, 0xFA, 0x80, 0xC7, 0xC1, 0xCC, 0x7C, 0xFD, 0x41, 0x7A, 0x99, 0xDC, 0x40, 0x0F, 0xBE, 0x34,
    0x40, 0x31, 0x97, 0xA6, 0x42, 0x1C, 0x52, 0x30, 0xC0, 0x61, 0x26, 0x39, 0x42, 0x41, 0xBD, 0x21,
    0x43, 0x72, 0x8F, 0x21, 0x42, 0x23, 0xF4, 0x30, 0xC1, 0xE5, 0xEB, 0x07, 0x42, 0x80, 0xF7, 0x9F,
    0x40, 0xD8, 0x8F, 0xF3, 0xC1, 0xDC, 0xC3, 0xBB, 0x42, 0x96, 0x89, 0xEC, 0xC1, 0xAA, 0x6C, 0x09,
};

static Vec l_normal = {0.0f, 1.0f, 0.0f};

static GXColor l_color[] = {
    {0xFF, 0xFF, 0xFF, 0x00}, {0xFF, 0xFF, 0xFF, 0x80}, {0xFF, 0xFF, 0xFF, 0x79},
    {0xFF, 0xFF, 0xFF, 0x73}, {0xFF, 0xFF, 0xFF, 0xFF},
};

static u8 l_texCoord[160] = {
    0x3F, 0x86, 0xF6, 0x27, 0x3F, 0x9C, 0x00, 0x00, 0x40, 0x03, 0x7B, 0x14, 0x3F, 0x9C, 0x00, 0x00,
    0x40, 0x03, 0x7B, 0x14, 0x3D, 0x00, 0x00, 0x00, 0x3F, 0x86, 0xF6, 0x27, 0x3D, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3D, 0x00, 0x00, 0x00, 0xBF, 0x80, 0x00, 0x00, 0x3D, 0x00, 0x00, 0x00,
    0xBF, 0x80, 0x00, 0x00, 0x3F, 0x9C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x9C, 0x00, 0x00,
    0x3F, 0x80, 0x00, 0x00, 0x3D, 0x00, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0x3F, 0x9C, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3E, 0xC4, 0xF3, 0xBA, 0xBF, 0x80, 0x00, 0x00, 0x3E, 0xC4, 0xF3, 0xBA,
    0x3F, 0x80, 0x00, 0x00, 0x3E, 0xC4, 0xF3, 0xBA, 0x40, 0x03, 0x7B, 0x14, 0x3E, 0xC4, 0xF3, 0xBA,
    0x3F, 0x86, 0xF6, 0x27, 0x3E, 0xC4, 0xF3, 0xBA, 0x3E, 0xF0, 0x00, 0x00, 0xBF, 0xDC, 0x00, 0x00,
    0x3F, 0x94, 0x00, 0x00, 0x3F, 0x9C, 0x00, 0x00, 0xBD, 0xC0, 0x00, 0x00, 0x3F, 0x9C, 0x00, 0x00,
    0x3F, 0x94, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00, 0xBD, 0xC0, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,
};

#include "assets/l_M_Kusa_9qDL.h"

#include "assets/l_M_Kusa_9q_cDL.h"

#include "assets/l_M_TenGusaDL.h"

#include "assets/l_Tengusa_matDL.h"

l_Tengusa_matDL(l_M_Hijiki00TEX)

#include "assets/l_kusa9q_matDL.h"
l_kusa9q_matDL(l_M_kusa05_RGBATEX)

#include "assets/l_kusa9q_l4_matDL.h"
l_kusa9q_l4_matDL(l_M_kusa05_RGBATEX)

void dGrass_data_c::WorkCo(fopAc_ac_c* i_hitActor, u32 i_massFlg, int i_roomNo) {
    cXyz sp18;

    if (!(dKyw_get_wind_pow() >= 0.6f)) {
        sp18.x = m_pos.x - i_hitActor->current.pos.x;
        sp18.z = m_pos.z - i_hitActor->current.pos.z;

        f32 var_f31 = sp18.abs2XZ();
        if (!(var_f31 > 1600.0f)) {
            sp18.y = m_pos.y - i_hitActor->current.pos.y;

            s16 temp_r26 = cM_atan2s(sp18.x, sp18.z);
            var_f31 = JMAFastSqrt(var_f31);

            dGrass_anm_c* var_r28;
            if (field_0x02 < 16) {
                if (i_hitActor->speedF > 16.0f) {
                    cXyz spC(m_pos.x, 20.0f + m_pos.y, m_pos.z);
                    dKy_tevstr_c* var_r25 = dComIfGp_roomControl_getTevStr(i_roomNo);
                }

                int var_r27 = daGrass_c::getGrass()->newAnm();
                if (var_r27 < 0) {
                    return;
                }

                field_0x02 = var_r27;
                var_r28 = daGrass_c::getGrass()->getAnm(field_0x02);
            } else {
                var_r28 = daGrass_c::getGrass()->getAnm(field_0x02);
            }

            cLib_addCalcAngleS(&var_r28->m_angY, temp_r26, 20, 4000, 1);
            cLib_addCalcAngleS(&var_r28->m_angX, cM_atan2s(40.0f - var_f31, 65.0f), 10, 4000, 1);
            var_r28->m_state = 2;
        }
    }
}

void dGrass_data_c::WorkAt_NoCutAnim(fopAc_ac_c* i_hitActor, u32 i_massFlg, int i_roomNo, dCcMassS_HitInf* i_hitinf, cCcD_Obj* i_hitObj) {
    cXyz spC;
    dCcD_GObjInf* gobj_p = dCcD_GetGObjInf(i_hitObj);

    f32 var_f31;
    s16 var_r26;
    if (i_hitObj == NULL) {
        spC.x = m_pos.x - i_hitActor->current.pos.x;
        spC.y = m_pos.y - i_hitActor->current.pos.y;
        spC.z = m_pos.z - i_hitActor->current.pos.z;

        var_f31 = spC.abs2XZ();
        var_r26 = i_hitActor->shape_angle.y;
    } else {
        spC = *dCcD_GetGObjInf(i_hitObj)->GetAtVecP();
        var_f31 = spC.abs2XZ();

        if (cM3d_IsZero(var_f31)) {
            cCcD_ShapeAttr* shape = i_hitObj->GetShapeAttr();
            if (shape->GetNVec(m_pos, &spC)) {
                PSVECScale(&spC, &spC, 5.0f);
                var_f31 = spC.abs2XZ();
            } else {
                spC.x = m_pos.x - i_hitActor->current.pos.x;
                spC.y = m_pos.y - i_hitActor->current.pos.y;
                spC.z = m_pos.z - i_hitActor->current.pos.z;

                var_f31 = spC.abs2XZ();
            }
        }

        if (cM3d_IsZero(var_f31)) {
            return;
        }

        var_r26 = cM_atan2s(spC.x, spC.z);
    }

    dGrass_anm_c* anm_p;
    if (field_0x02 < 16) {
        int var_r25 = daGrass_c::getGrass()->newAnm();
        if (var_r25 < 0) {
            return;
        }

        field_0x02 = var_r25;
        anm_p = daGrass_c::getGrass()->getAnm(field_0x02);
    } else {
        anm_p = daGrass_c::getGrass()->getAnm(field_0x02);
    }

    anm_p->m_angY = var_r26 * (0.9f + (0.2f * cM_rnd()));

    if (i_hitObj->ChkAtType(AT_TYPE_BOMB)) {
        anm_p->m_angX = cM_atan2s(var_f31, 40.0f) * (2.5f + (0.5f * cM_rnd()));
    } else if (i_hitObj->ChkAtType(AT_TYPE_BOOMERANG)) {
        anm_p->m_angX = cM_atan2s(var_f31, 40.0f) * (1.5f + (0.5f * cM_rnd()));
    } else if (i_hitObj->ChkAtType(AT_TYPE_100) || i_hitObj->ChkAtType(AT_TYPE_LANTERN_SWING)) {
        anm_p->m_angX = 0.05f * (cM_atan2s(var_f31, 40.0f) * (1.5f + cM_rnd()));
    } else if (daPy_getPlayerActorClass()->getCutType() >= daPy_py_c::CUT_TYPE_NM_VERTICAL && daPy_getPlayerActorClass()->getCutType() <= daPy_py_c::CUT_TYPE_FINISH_VERTICAL) {
        anm_p->m_angX = 0.5f * (cM_atan2s(var_f31, 20.0f) * (0.5f + (0.2f * cM_rnd())));
    } else if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_TURN_RIGHT ||
               daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_TURN_LEFT ||
               daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT ||
               daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT)
    {
        anm_p->m_angX = cM_atan2s(var_f31, 40.0f) * (1.5f + (0.3f * cM_rnd()));
    } else {
        anm_p->m_angX = cM_atan2s(var_f31, 40.0f) * (0.9f + (0.2f * cM_rnd()));
    }

    anm_p->m_state = 2;
}

void dGrass_data_c::Direction_Set(fopAc_ac_c* i_hitActor, u32 i_massFlg, int i_roomNo,
                                  dCcMassS_HitInf* i_hitinf, cCcD_Obj* i_hitObj, csXyz* i_rotation) {
    cXyz sp8;

    f32 var_f31;
    s16 var_r27;
    if (i_hitObj == NULL) {
        sp8.x = m_pos.x - i_hitActor->current.pos.x;
        sp8.y = m_pos.y - i_hitActor->current.pos.y;
        sp8.z = m_pos.z - i_hitActor->current.pos.z;

        var_f31 = sp8.abs2XZ();
        var_r27 = i_hitActor->shape_angle.y;
    } else {
        sp8 = *dCcD_GetGObjInf(i_hitObj)->GetAtVecP();
        var_f31 = sp8.abs2XZ();

        if (cM3d_IsZero(var_f31)) {
            cCcD_ShapeAttr* shape = i_hitObj->GetShapeAttr();
            if (shape->GetNVec(m_pos, &sp8)) {
                PSVECScale(&sp8, &sp8, 5.0f);
                var_f31 = sp8.abs2XZ();
            } else {
                sp8.x = m_pos.x - i_hitActor->current.pos.x;
                sp8.y = m_pos.y - i_hitActor->current.pos.y;
                sp8.z = m_pos.z - i_hitActor->current.pos.z;

                var_f31 = sp8.abs2XZ();
            }
        }

        if (cM3d_IsZero(var_f31)) {
            return;
        }

        var_r27 = cM_atan2s(sp8.x, sp8.z);
    }

    i_rotation->y = var_r27 * (0.89999998f + (0.2f * cM_rnd()));
    i_rotation->x = 0.6f * (cM_atan2s(var_f31, 40.0f));
    i_rotation->z = 0;
}

static u8 data_8052790d;

void dGrass_data_c::WorkAt(fopAc_ac_c* i_hitActor, u32 i_massFlg, int i_roomNo, dCcMassS_HitInf* i_hitinf,
                           u16 i_particleID) {
    cCcD_Obj* hit_obj = i_hitinf->GetAtHitObj();

    if (hit_obj != NULL) {
        if (hit_obj->ChkAtType(0) || hit_obj->ChkAtType(AT_TYPE_ARROW) ||
            hit_obj->ChkAtType(AT_TYPE_SLINGSHOT) || hit_obj->ChkAtType(AT_TYPE_20000000) ||
            hit_obj->ChkAtType(AT_TYPE_SHIELD_ATTACK) || hit_obj->ChkAtType(AT_TYPE_BOOMERANG) ||
            hit_obj->ChkAtType(AT_TYPE_100) || hit_obj->ChkAtType(AT_TYPE_LANTERN_SWING) ||
            hit_obj->ChkAtType(AT_TYPE_HOOKSHOT))
        {
            WorkAt_NoCutAnim(i_hitActor, i_massFlg, i_roomNo, i_hitinf, hit_obj);
            return;
        }
    }

    fopAc_ac_c* player_p = dComIfGp_getPlayer(0);
    if (hit_obj->ChkAtType(AT_TYPE_BOMB)) {
        if (i_hitActor != NULL && m_pos.abs(i_hitActor->current.pos) > 200.0f) {
            WorkAt_NoCutAnim(i_hitActor, i_massFlg, i_roomNo, i_hitinf, hit_obj);
            return;
        }
    } else if (daPy_getPlayerActorClass()->getCutType() >= daPy_py_c::CUT_TYPE_NM_RIGHT &&
               daPy_getPlayerActorClass()->getCutType() <= daPy_py_c::CUT_TYPE_FINISH_VERTICAL)
    {
        if (m_pos.abs(player_p->current.pos) > 158.0f) {
            if (m_pos.abs(player_p->current.pos) < 200.0f) {
                WorkAt_NoCutAnim(i_hitActor, i_massFlg, i_roomNo, i_hitinf, hit_obj);
            }
            return;
        }
    } else if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_TURN_RIGHT ||
               daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_TURN_LEFT)
    {
        if (m_pos.abs(player_p->current.pos) > 200.0f) {
            WorkAt_NoCutAnim(i_hitActor, i_massFlg, i_roomNo, i_hitinf, hit_obj);
            return;
        }
    } else if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT ||
               daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT)
    {
        if (m_pos.abs(player_p->current.pos) > 500.0f) {
            WorkAt_NoCutAnim(i_hitActor, i_massFlg, i_roomNo, i_hitinf, hit_obj);
            return;
        }
    } else if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_NM_VERTICAL ||
               daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_NM_STAB)
    {
        if (m_pos.abs(player_p->current.pos) > 158.0f) {
            return;
        }
    }

    if (field_0x02 >= 16) {
        daGrass_c::getGrass()->deleteAnm(field_0x02);
    }

    int itemBitNo = m_itemBitNo;
    if (m_itemBitNo == 0xFF) {
        itemBitNo = -1;
    }

    bool create_direct = false;
    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        create_direct = true;
    }

    fopAcM_createItemFromTable(&m_pos, m_tableNo, itemBitNo, i_roomNo, NULL, 0, NULL, NULL, NULL,
                               create_direct);

    field_0x02 = -1;

    static csXyz ang(0, 0, 0);

    cXyz particle_pos(m_pos.x, m_pos.y + 25.0f, m_pos.z);
    dKy_tevstr_c* tevstr = dComIfGp_roomControl_getTevStr(i_roomNo);

    csXyz particle_rot;
    Direction_Set(i_hitActor, i_massFlg, i_roomNo, i_hitinf, hit_obj, &particle_rot);

    GXColor envcolor;
    envcolor.r = m_addCol >> 8;
    envcolor.g = m_addCol & 0xFF;
    dComIfGp_particle_set(i_particleID, &particle_pos, tevstr, &particle_rot, NULL, 255,
                          dPa_control_c::getLight8EcallBack(), -1, &envcolor, NULL, NULL);

    if (data_8052790d == 0) {
        data_8052790d = 1;
        mDoAud_seStart(JA_SE_LK_CUT_GRASS, &m_pos, 0, dComIfGp_getReverb(i_roomNo));
    }
}

void dGrass_data_c::hitCheck(int i_roomNo, u16 i_particleName) {
    dCcMassS_HitInf hitinf;
    fopAc_ac_c* hit_actor;

    u32 mass_flags = dComIfG_Ccsp()->ChkMass(&m_pos, &hit_actor, &hitinf);
    bool at_hit = (mass_flags & 1) && (hit_actor != NULL && fopAcM_GetName(hit_actor) != PROC_Obj_Carry);

    if (!(mass_flags & 2) && !at_hit) {
        if (field_0x02 >= 16) {
            dGrass_anm_c* anm_p = daGrass_c::getGrass()->getAnm(field_0x02);
            s16 spC = anm_p->m_angY & 0xE000;
            int sp10 = (field_0x02 & 15) & 0xFFFF;
            dGrass_anm_c* temp_r27 = daGrass_c::getGrass()->getAnm(sp10);

            if (anm_p->m_state == 2) {
                mDoAud_seStart(JA_SE_FT_ADD_GRASS, &m_pos, 0, dComIfGp_getReverb(i_roomNo));
                anm_p->m_state = 1;
            }

            if (cLib_addCalcAngleS(&anm_p->m_angX, temp_r27->m_angX, 10, 4400, 400) == 0) {
                if (cLib_addCalcAngleS(&anm_p->m_angY, temp_r27->m_angY, 10, 4400, 400) == 0) {
                    daGrass_c::getGrass()->deleteAnm(field_0x02);
                    field_0x02 &= 15;
                }
            }
        }
    } else {
        if (mass_flags & 2) {
            WorkCo(hit_actor, mass_flags, i_roomNo);
        }

        if (at_hit) {
            WorkAt(hit_actor, mass_flags, i_roomNo, &hitinf, i_particleName);
        }
    }
}

void dGrass_room_c::newData(dGrass_data_c* i_data) {
    i_data->mp_next = mp_data;
    mp_data = i_data;
}

void dGrass_room_c::deleteData() {
    while (mp_data != NULL) {
        mp_data->m_state = 0;

        s8 anm_id = mp_data->field_0x02;
        if (anm_id >= 16) {
            daGrass_c::getGrass()->deleteAnm(anm_id);
            mp_data->field_0x02 = cM_rnd() * 15.0f;
        }

        mp_data = mp_data->mp_next;
    }
}

dGrass_packet_c::dGrass_packet_c() {
    dGrass_data_c* data_p = m_data;
    for (int i = 0; i < 1500; i++) {
        data_p->m_state = 0;
        data_p++;
    }

    m_dataNum = 0;

    dGrass_anm_c* anm_p = m_anm;
    for (int i = 0; i < 112; i++) {
        anm_p->m_state = 0;
        anm_p++;
    }

    s16 unused = 0;
    for (int i = 0; i < 16; i++) {
        setAnm(i, unused);
        unused += 0x1000;
    }

    mp_pos = (Vec*)l_pos;
    mp_normal = (Vec*)&l_normal;
    mp_colors = (GXColor*)l_color;
    mp_texCoords = (Vec*)l_texCoord;
    mp_kusa9q_DL = l_kusa9q_matDL;
    m_kusa9q_DL_size = 0xA0;
    mp_kusa9q_14_DL = l_kusa9q_l4_matDL;
    m_kusa9q_DL_14_size = 0xA0;
    mp_Mkusa_9q_DL = l_M_Kusa_9qDL;
    m_Mkusa_9q_DL_size = 0xC0;
    mp_Mkusa_9q_cDL = l_M_Kusa_9q_cDL;
    m_Mkusa_9q_cDL_size = 0xC0;
    field_0x1d714 = 0;

    OS_REPORT("草群メモリ=%fＫ\n", 117.7734375f);
    
    m_deleteRoom = &dGrass_packet_c::deleteRoom;
}

void dGrass_packet_c::draw() {
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();
    cXyz spB4;
    cXyz spA8;

    j3dSys.reinitGX();
    GXSetNumIndStages(0);
    dKy_setLight_again();
    GXClearVtxDesc();

    static GXVtxDescList l_vtxDescList[] = {
        {GX_VA_POS, GX_INDEX8},
        {GX_VA_NRM, GX_INDEX8},
        {GX_VA_CLR0, GX_INDEX8},
        {GX_VA_TEX0, GX_INDEX8},
        {GX_VA_NULL, GX_NONE},
    };

    static GXVtxAttrFmtList l_vtxAttrFmtList[] = {
        GX_VA_POS,
        GX_POS_XYZ,
        GX_F32,
        0,

        GX_VA_NRM,
        GX_NRM_XYZ,
        GX_F32,
        0,

        GX_VA_CLR0,
        GX_CLR_RGBA,
        GX_RGBA8,
        0,

        GX_VA_TEX0,
        GX_TEX_ST,
        GX_F32,
        0,

        GX_VA_NULL,
        GX_TEX_ST,
        GX_S8,
        0,
    };

    GXSetVtxDescv(l_vtxDescList);
    GXSetVtxAttrFmtv(GX_VTXFMT0, l_vtxAttrFmtList);
    GXSetArray(GX_VA_POS, mp_pos, sizeof(Vec));
    GXSetArray(GX_VA_NRM, mp_normal, sizeof(Vec));
    GXSetArray(GX_VA_CLR0, mp_colors, sizeof(GXColor));
    GXSetArray(GX_VA_TEX0, mp_texCoords, 8);

    GXColorS10 spA0 = {0, 0, 0, 0};
    GXColorS10 sp98 = {0, 0, 0, 0};

    dGrass_room_c* sp54 = m_room;

    f32 var_f31 = g_env_light.getDaytime();
    f32 var_f30;
    f32 var_f29;
    if (var_f31 >= 90.0f && var_f31 < 135.0f) {
        var_f30 = 1.0f - (0.022222223f * (135.0f - var_f31));
        var_f29 = 100.0f - (18.0f * var_f30);
    } else if (var_f31 >= 135.0f && var_f31 < 225.0f) {
        var_f30 = 1.0f - (0.011111111f * (225.0f - var_f31));
        var_f29 = 82.0f - (25.0f * var_f30);
    } else if (var_f31 >= 225.0f && var_f31 < 270.0f) {
        var_f30 = 1.0f - (0.022222223f * (270.0f - var_f31));
        var_f29 = 57.0f - (-25.0f * var_f30);
    } else if (var_f31 >= 270.0f && var_f31 < 315.0f) {
        var_f30 = (1.0f - (0.022222223f * (315.0f - var_f31)));
        var_f29 = 82.0f - (-18.0f * var_f30);
    } else {
        var_f29 = 100.0f;
    }

#if DEBUG
    if (g_kankyoHIO.navy.grass_shine_value != 0.0f) {
        var_f29 = g_kankyoHIO.navy.grass_shine_value;
    }
#endif

    for (int i = 0; i < 64; i++) {
        dGrass_data_c* var_r29 = sp54->getData();
        if (var_r29 != NULL && dComIfGp_roomControl_checkStatusFlag(i, 0x10)) {
            int sp4C = 0;
            int sp48 = 6;
            dKy_tevstr_c* sp44 = dComIfGp_roomControl_getTevStr(i);

            f32 temp_f27 = g_env_light.grass_light_inf_rate * g_env_light.bg_light_influence;
            f32 sp58 = 1.0f - temp_f27;
            temp_f27 += 0.5f * sp58;

            J3DLightInfo* sp40 = sp44->mLights[0].getLightInfo();
            sp98.r = sp40->mColor.r * temp_f27;
            sp98.g = sp40->mColor.g * temp_f27;
            sp98.b = sp40->mColor.b * temp_f27;
            sp98.a = var_f29;

            if (memcmp(dComIfGp_getStartStageName(), "D_MN01", 6) == 0) {
                sp98.r = 0;
                sp98.g = 0x1E;
                sp98.b = 5;
                sp98.a = 0x50;
            }

            GFSetTevColorS10(GX_TEVREG1, sp98);

            if (dComIfGp_roomControl_getStatusRoomDt(i) != NULL) {
                sp48 = dComIfGp_roomControl_getStatusRoomDt(i)->getLightVecInfoNum();
            }

#if DEBUG
            if (g_kankyoHIO.light.m_HOSTIO_setting != 0) {
                sp48 = g_kankyoHIO.dungeonLight.usedLights;
            }
#endif

            if (dKy_SunMoon_Light_Check() == TRUE && sp48 < 2) {
                sp48 = 2;
            }

            for (int j = 0; j < 6; j++) {
                if (kankyo->field_0x0c18[j].field_0x26 == 1) {
                    sp48++;
                }
            }

            if (var_r29->field_0x05 <= 3 || var_r29->field_0x05 >= 10) {
                if (sp48 <= 3) {
                    GXCallDisplayList(mp_kusa9q_14_DL, m_kusa9q_DL_14_size);
                } else {
                    GXCallDisplayList(mp_kusa9q_DL, m_kusa9q_DL_size);
                }
            } else {
                GXCallDisplayList(l_Tengusa_matDL, 0xA0);
            }

            GFSetTevColorS10(GX_TEVREG2, spA0);

            dKy_Global_amb_set(sp44);
            dKy_GfFog_tevstr_set(sp44);
            dKy_setLight_nowroom_grass(sp44->room_no, 0.0f);

            f32 temp1 = 0.001953125f; // fakematch

#if DEBUG
            camera_class* sp34 = dComIfGp_getCamera(0);
            int sp30 = 0;
            if (dCam_getBody() != NULL &&
                (dCam_getBody()->Mode() == 7 || dCam_getBody()->Mode() == 8) &&
                sp34 != NULL &&
                dCam_getBody()->V() < -750.0f &&
                var_r29->m_pos.abs(sp34->lookat.eye) < 200.0f) {
                sp30 = 1;
            }
#endif

            while (var_r29 != NULL) {
                if (var_r29->field_0x05 <= 3 || var_r29->field_0x05 >= 10) {
                    if (sp48 <= 2) {
                        GXCallDisplayList(mp_kusa9q_14_DL, m_kusa9q_DL_14_size);
                    } else {
                        GXCallDisplayList(mp_kusa9q_DL, m_kusa9q_DL_size);
                    }
                } else {
                    GXCallDisplayList(l_Tengusa_matDL, 0xA0);
                }

                if (var_r29->field_0x02 <= -1) {
                    sp98.a = 0;
                } else {
                    sp98.a = var_f29;
                }

                GFSetTevColorS10(GX_TEVREG1, sp98);

                GXColor sp38;

                if (DEBUG && g_kankyoHIO.navy.grass_adjust_ON) {
                    sp38.r = g_kankyoHIO.navy.grass_ambcol.r * 2;
                    sp38.g = g_kankyoHIO.navy.grass_ambcol.g * 2;
                    sp38.b = g_kankyoHIO.navy.grass_ambcol.b * 2;
                } else {
                    sp38.r = (var_r29->m_addCol & 0x1F) * 2;
                    sp38.g = ((var_r29->m_addCol >> 5) & 0x1F) * 2;
                    sp38.b = ((var_r29->m_addCol >> 0xA) & 0x1F) * 2;

                    if (daPy_py_c::checkNowWolfPowerUp()) {
                        f32 var_f26 = g_env_light.bg_amb_col[0].r / 255.0f;
                        f32 var_f1 = (((var_r29->m_addCol & 0x1F) * 2 + 0x10));
                        sp38.r = var_f1 * (var_f26 * 4.0f);

                        var_f26 = g_env_light.bg_amb_col[0].g / 255.0f;
                        f32 var_f1_2 = (((var_r29->m_addCol >> 5) & 0x1F) * 2 + 0x10);
                        sp38.g = var_f1_2 * (4.0f * var_f26);

                        var_f26 = g_env_light.bg_amb_col[0].b / 255.0f;
                        f32 var_f1_3 = (((var_r29->m_addCol >> 10) & 0x1F) * 2 + 0x10);
                        sp38.b = var_f1_3 * (4.0f * var_f26);
                    }

                    f32 sp2C = 1.0f - (((int)var_r29->m_pos.x & 0xFF) * temp1);

                    f32 var_f28 = 1.1f - (((u8)(int)var_r29->m_pos.x & 0xFF) / 2000.0f);
                    var_f28 -= ((int)var_r29->m_pos.z & 0xFF) / 2000.0f;

                    if (var_f28 > 1.0f) {
                        var_f28 = 1.0f;
                    }

                    if (sp38.r == 0x3E) {
                        sp38.r = sp44->AmbCol.r * sp2C;
                    } else {
                        sp38.r = sp38.r * var_f28;
                    }

                    if (sp38.g == 0x3E) {
                        sp38.g = sp44->AmbCol.g * sp2C;
                    } else {
                        sp38.g = sp38.g * var_f28;
                    }

                    if (sp38.b == 0x3E) {
                        sp38.b = sp44->AmbCol.b * sp2C;
                    } else {
                        sp38.b = sp38.b * var_f28;
                    }
                }

                GXSetChanAmbColor(GX_COLOR0A0, sp38);

                if (!cLib_checkBit<u8>(var_r29->field_0x01, 2)) {
                    GXLoadPosMtxImm(var_r29->m_modelMtx, 0);
                    GXLoadNrmMtxImm(j3dSys.getViewMtx(), 0);
                    if (var_r29->field_0x05 <= 3 || var_r29->field_0x05 >= 10) {
                        if (var_r29->field_0x02 < -1) {
                            spA0.a = (-0x100 - ((var_r29->field_0x02 << 8) / 40));
                            GFSetTevColorS10(GX_TEVREG2, spA0);

                            if (var_r29->field_0x02 != -2) {
                                GXCallDisplayList(mp_Mkusa_9q_DL, m_Mkusa_9q_DL_size);
                            } else {
                                GXCallDisplayList(mp_Mkusa_9q_cDL, m_Mkusa_9q_cDL_size);
                            }

                            spA0.a = 0;
                            GFSetTevColorS10(GX_TEVREG2, spA0);
                        } else if (var_r29->field_0x02 >= 0) {
                            GXCallDisplayList(mp_Mkusa_9q_DL, m_Mkusa_9q_DL_size);
                        } else {
                            GXCallDisplayList(mp_Mkusa_9q_cDL, m_Mkusa_9q_cDL_size);
                        }
                    } else if (var_r29->field_0x02 < -1) {
                        spA0.a = (-0x100 - ((var_r29->field_0x02 << 8) / 40));
                        GFSetTevColorS10(GX_TEVREG2, spA0);

                        if (var_r29->field_0x02 != -2) {
                            GXCallDisplayList(l_M_TenGusaDL, 0xC0);
                        } else {
                            GXCallDisplayList(mp_Mkusa_9q_cDL, m_Mkusa_9q_cDL_size);
                        }
 
                        spA0.a = 0;
                        GFSetTevColorS10(GX_TEVREG2, spA0);
                    } else if (var_r29->field_0x02 >= 0) {
                        GXCallDisplayList(l_M_TenGusaDL, 0xC0);
                    } else {
                        GXCallDisplayList(mp_Mkusa_9q_cDL, m_Mkusa_9q_cDL_size);
                    }
                }

                var_r29 = var_r29->mp_next;
            }
        }
        sp54++;
    }

    J3DShape::resetVcdVatCache();
}

void dGrass_packet_c::calc() {
    cXyz* temp_r29 = dKyw_get_wind_vec();
    f32 var_f31 = dKyw_get_wind_pow() * dKyw_get_wind_pow();

    static s16 exec_count_dammy = 0;
    static s16 exec_count_dammy2 = 0;

    if (var_f31 < 0.09f) {
        var_f31 = 0.09f;
    }

    if (memcmp(dComIfGp_getStartStageName(), "D_MN01", 6) == 0 && temp_r29 != NULL) {
        temp_r29->x = 0.0f;
        temp_r29->y = 0.0f;
        temp_r29->z = 0.0f;
        var_f31 = 0.0f;
    }

    if (memcmp(dComIfGp_getStartStageName(), "D_MN54", 6) == 0 && temp_r29 != NULL) {
        temp_r29->x = 0.0f;
        temp_r29->y = 0.0f;
        temp_r29->z = 0.0f;
        var_f31 = 0.01f;
    }

    int sp18;
    dGrass_anm_c* sp14 = getAnm();
    f32 temp_f30 = cM_scos(field_0x1d714);
    field_0x1d714 += (int)(40.0f + cM_rndF(290.0f));

    sp18 = 1.0f + ((1.0f - var_f31) * (2.0f + NREG_F(3)));

    f32 temp1 = 0.0625f;
    for (int i = 0; i < 16; sp14++, i++) {
        f32 temp_f28 = i * temp1;
        f32 temp2 = 500.0f + (500.0f * (i / 16));
        f32 temp_f29 = cLib_maxLimit<f32>(temp2, 2000.0f);
        f32 temp_f27_2 = 1.6f + (0.69999999f * cM_ssin(exec_count_dammy2));

        f32 temp3 = cM_ssin(exec_count_dammy + i * 3050);
        s16 sp8 = 0.69999999f * var_f31 * (temp_f29 + ((1.0f + (-0.2f * temp_f28)) * (10.0f * temp_f29 * (temp3 + temp_f27_2))));

        exec_count_dammy += (int)(s16)(525.0f * var_f31);
        exec_count_dammy2 += (int)(s16)(325.0f * var_f31);

        cLib_addCalcAngleS(&sp14->m_angX, (s16)sp8, sp18, 4000, 1);
        cLib_addCalcAngleS(&sp14->m_angY, (s16)cM_atan2s(temp_r29->x, temp_r29->z), 20, 4000, 1);
    }

    int sp10 = dComIfGp_roomControl_getStayNo();
    dGrass_data_c* var_r30 = m_room[sp10].getData();
    dComIfGs_Grass_hide_Set(0);

    if (var_r30 != NULL) {
        data_8052790d = 0;
        dComIfG_Ccsp()->SetMassAttr(40.0f, 120.0f, 11, 0);

        do {
            u16 spA = 0x89D7;
            if (var_r30->field_0x05 >= 7 && var_r30->field_0x05 <= 9) {
                spA = 0x89D6;
            } else if (var_r30->field_0x05 >= 4 && var_r30->field_0x05 <= 6) {
                spA = 0x89D8;
            }

            if (!cLib_checkBit<u8>(var_r30->field_0x01, 2)) {
                if (var_r30->field_0x02 >= 0) {
                    var_r30->hitCheck(sp10, spA);
                } else if (var_r30->field_0x02 < -2) {
                    var_r30->field_0x02++;
                }
            } else if (var_r30->field_0x02 >= 16) {
                daGrass_c::getGrass()->deleteAnm(var_r30->field_0x02);
                var_r30->field_0x02 = 15.0f * cM_rnd();
            }

            var_r30 = var_r30->mp_next;
        } while (var_r30 != NULL);
    } else {
        dComIfG_Ccsp()->SetMassAttr(40.0f, 40.0f, 11, 0);
    }
}

static f32 checkGroundY(cXyz& param_0) {
    dBgS_GndChk sp8;
    param_0.y += 50.0f;

    bool temp_r3 = fopAcM_gc_c::gndCheck(&param_0);
    f32 temp_f31 = fopAcM_gc_c::getGroundY();

    param_0.y -= 50.0f;

    f32 temp_f30;
    if (!temp_r3) {
        temp_f30 = param_0.y;
        return temp_f30;
    }

    return temp_f31;
}

void dGrass_packet_c::update() {
    static s16 mAngleZ[1500];

    dGrass_anm_c* anm_p = getAnm();
    for (int i = 0; i < 112; i++) {
        mDoMtx_stack_c::YrotS(anm_p->m_angY);
        mDoMtx_stack_c::XrotM(anm_p->m_angX);
        mDoMtx_stack_c::YrotM(-anm_p->m_angY);
        cMtx_copy(mDoMtx_stack_c::get(), anm_p->m_animMtx);
        anm_p++;
    }

    dGrass_data_c* data_p = getData();
    anm_p = getAnm();

    int var_r26 = 0;

    cXyz sp1C;
    f32 far = mDoLib_clipper::getFar();
    mDoLib_clipper::changeFar(far * 1.6363636f);

    for (int i = 0; i < 1500; i++) {
        if (data_p->m_state != 0) {
            if (data_p->m_state == 1 && var_r26 < 30) {
                data_p->m_pos.y = checkGroundY(data_p->m_pos);
                data_p->m_state = 2;
                var_r26++;
            }

            sp1C.set(data_p->m_pos.x, data_p->m_pos.y + 260.0f, data_p->m_pos.z);
            if (mDoLib_clipper::clip(j3dSys.getViewMtx(), sp1C, 260.0f)) {
                cLib_onBit<u8>(data_p->field_0x01, 2);
            } else {
                cLib_offBit<u8>(data_p->field_0x01, 2);

                if (data_p->field_0x02 >= 0) {
                    MtxP mtx = anm_p[data_p->field_0x02].m_animMtx;
                    mtx[0][3] = data_p->m_pos.x;
                    mtx[1][3] = data_p->m_pos.y;
                    mtx[2][3] = data_p->m_pos.z;

                    if (g_env_light.light_init_timer == 1) {
                        mAngleZ[i] = 0;
                    }

                    f32 temp_f2 = (((s16)data_p->m_pos.x * 3535) & 0xFFF) / 4096.0f;
                    temp_f2 = (temp_f2 + ((((s16)data_p->m_pos.z * 3535) & 0xFFF) / 4096.0f)) / 2;

                    f32 var_f31;
                    f32 var_f30;
                    if (data_p->field_0x05 >= 10) {
                        var_f31 = temp_f2 * 0.39999998f + 0.6f;
                        var_f30 = temp_f2 * 0.3f + 0.7f;
                    } else {
                        var_f31 = temp_f2 * 0.3f + 0.7f;
                        var_f30 = (temp_f2 * 0.3f + 0.7f) * 0.8f;
                    }

                    mDoMtx_stack_c::copy(mtx);

                    if (data_p->field_0x05 >= 7 && data_p->field_0x05 <= 9) {
                        mDoMtx_stack_c::ZrotM(mAngleZ[i]);
                        mDoMtx_stack_c::scaleM(var_f31, var_f30 - 0.22f, var_f31);
                        mDoMtx_stack_c::ZrotM(-mAngleZ[i]);

                        s16 ang = ((i + 1) & 0xF) * 5.0f + 162.0f;
                        mAngleZ[i] += ang;
                    } else {
                        if (data_p->field_0x05 >= 10 &&
                            memcmp(dComIfGp_getStartStageName(), "D_MN54", 6) != 0)
                        {
                            mDoMtx_stack_c::ZrotM(mAngleZ[i]);
                        }

                        mDoMtx_stack_c::scaleM(var_f31, var_f30, var_f31);

                        if (data_p->field_0x05 >= 10 &&
                            memcmp(dComIfGp_getStartStageName(), "D_MN54", 6) != 0)
                        {
                            mDoMtx_stack_c::ZrotM(-mAngleZ[i]);
                        }

                        if (data_p->field_0x05 >= 10) {
                            mDoMtx_stack_c::scaleM(2.0f, (i & 3) * 0.1f + 0.64f, 2.0f);
                        }

                        if (data_p->field_0x05 >= 10 &&
                            memcmp(dComIfGp_getStartStageName(), "D_MN54", 6) != 0)
                        {
                            mAngleZ[i] += (int)((((i + 1) & 0xF) * 25.0f + 3200.0f) * (dKyw_get_wind_pow() * dKyw_get_wind_pow() * dKyw_get_wind_pow()));
                        }
                    }

                    cMtx_concat(j3dSys.getViewMtx(), mDoMtx_stack_c::get(), data_p->m_modelMtx);
                } else {
                    mDoMtx_stack_c::transS(data_p->m_pos.x, data_p->m_pos.y, data_p->m_pos.z);
                    mDoMtx_stack_c::YrotM(i * 3535);

                    if (data_p->field_0x02 < -2) {
                        mDoMtx_stack_c::scaleM(1.0f, -data_p->field_0x02 / 40.0f, 1.0f);
                    }

                    f32 scale = ((((s16)data_p->m_pos.x * 3535) & 0xFFF) / 4096.0f) * 0.3f + 0.7f;
                    mDoMtx_stack_c::scaleM(scale, scale, scale);
                    cMtx_concat(j3dSys.getViewMtx(), mDoMtx_stack_c::get(), data_p->m_modelMtx);
                }
            }
        }
        data_p++;
    }

    mDoLib_clipper::resetFar();
    dComIfGd_getListPacket()->entryImm(this, 0);
}

void dGrass_packet_c::setData(dGrass_data_c* i_data, int i_dataNo, cXyz& i_pos, int i_roomNo,
                              u8 i_tableNo, u8 i_itemBitNo, s16 i_addCol, u8 param_7) {
    f32 y_pos;
    if (fopOvlpM_IsPeek()) {
        y_pos = checkGroundY(i_pos);
        i_data->m_state = 2;
    } else {
        y_pos = i_pos.y;
        i_data->m_state = 1;
    }

    i_data->field_0x01 = 2;
    i_data->field_0x02 = cM_rndF(15.0f);
    i_data->m_pos.set(i_pos.x, y_pos, i_pos.z);
    i_data->m_tableNo = i_tableNo;
    i_data->m_itemBitNo = i_itemBitNo;
    i_data->m_addCol = i_addCol;
    i_data->field_0x05 = param_7;

    m_room[i_roomNo].newData(i_data);
    m_dataNum = i_dataNo;
}

dGrass_data_c* dGrass_packet_c::newData(cXyz& i_pos, int i_roomNo, u8 i_tableNo, u8 i_itemBitNo,
                                        s16 i_addCol, u8 param_5) {
    dGrass_data_c* data_p = m_data + m_dataNum;

    for (int i = m_dataNum; i < 1500; i++) {
        if (data_p->m_state == 0) {
            setData(data_p, i, i_pos, i_roomNo, i_tableNo, i_itemBitNo, i_addCol, param_5);
            return data_p;
        }
        data_p++;
    }

    data_p = m_data;
    for (int i = 0; i < m_dataNum; i++) {
        if (data_p->m_state == 0) {
            setData(data_p, i, i_pos, i_roomNo, i_tableNo, i_itemBitNo, i_addCol, param_5);
            return data_p;
        }
        data_p++;
    }

    return NULL;
}

void dGrass_packet_c::deleteRoom(int i_roomNo) {
    m_room[i_roomNo].deleteData();
}

int dGrass_packet_c::newAnm() {
    dGrass_anm_c* anm_p = m_anm + 16;
    for (int i = 16; i < 112; i++) {
        if (anm_p->m_state == 0) {
            anm_p->m_state = 1;
            anm_p->m_angY = 0;
            anm_p->m_angX = 0;
            return i;
        }

        anm_p++;
    }

    return -1;
}

void dGrass_packet_c::setAnm(int i_idx, s16) {
    dGrass_anm_c* anm_p = m_anm + i_idx;

    anm_p->m_state = 1;
    anm_p->m_angY = 0;
    anm_p->m_angX = 0;
}
