#ifndef D_A_OBJ_TOGETRAP_H
#define D_A_OBJ_TOGETRAP_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daTogeTrap_c
 * @brief Blade Trap
 *
 * @details
 *
 */
class daTogeTrap_c : public dBgS_MoveBgActor {
public:
    typedef void (daTogeTrap_c::*modeFunc)();

    enum Mode {
        MODE_WAIT,
        MODE_MOVE_UP,
        MODE_MOVE_DOWN,
        MODE_BREAK,
    };

    void setBaseMtx();
    int CreateHeap();
    int create();
    static void rideCallBack(dBgW*, fopAc_ac_c*, fopAc_ac_c*);
    int Execute(Mtx**);
    void moveMain();
    BOOL playerAreaCheck();
    void init_modeWait();
    void modeWait();
    void init_modeMoveUp();
    void modeMoveUp();
    void init_modeMoveDown();
    void modeMoveDown();
    void init_modeBreak();
    void modeBreak();
    int Draw();
    int Delete();

    u8 getSwBit() { return fopAcM_GetParamBit(this, 0, 8); }

    static dCcD_SrcGObjInf const mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;

private:
    /* 0x5A0 */ dCcD_Stts mStts;
    /* 0x5DC */ dCcD_Cyl mCyls[4];
    /* 0xACC */ request_of_phase_process_class mPhase;
    /* 0xAD4 */ J3DModel* mModel;
    /* 0xAD8 */ J3DModel* mWolfEyeModel;
    /* 0xADC */ u8 mMode;
    /* 0xADD */ u8 field_0xadd;
    /* 0xADE */ u8 mSwBit;
    /* 0xADF */ bool mIsWolfEye;
    /* 0xAE0 */ BOOL mIsPlayerInArea;
};

STATIC_ASSERT(sizeof(daTogeTrap_c) == 0xae4);


class daTogeTrap_HIO_c : public fOpAcm_HIO_entry_c {
public:
    daTogeTrap_HIO_c();
    ~daTogeTrap_HIO_c() {}

    #if DEBUG
    void genMessage(JORMContext* ctx) {
        // Speed
        ctx->genLabel("--- 速 出現時---", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
        // Initial speed
        ctx->genSlider("初速", &mInitialSpeed, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                       0x18);
        // Max
        ctx->genSlider("最大", &mMaxSpeed, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
        // Speed when pulling
        ctx->genLabel("--- 速度 引っ込み時---", 0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
        ctx->genSlider("初速", &mInitialDownSpeed, 0.0, 10000.0, 0, NULL, 0xffff, 0xffff, 0x200,
                       0x18);
        // Max
        ctx->genSlider("最大", &mMaxDownSpeed, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                       0x18);
        // Range
        ctx->genSlider("範囲", &mRange, 0.1f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
        // Spinner speed
        ctx->genSlider("スピナｰ速度", &mSpinnerSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                       0x18);
        // Vibration
        ctx->startComboBox("振動", &mVibration, 0, NULL, 0xffff, 0xffff, 0x100, 0x1a);
        // Strength 1-8
        ctx->genComboBoxItem("強さ１", 1);
        ctx->genComboBoxItem("強さ２", 2);
        ctx->genComboBoxItem("強さ３", 3);
        ctx->genComboBoxItem("強さ４", 4);
        ctx->genComboBoxItem("強さ５", 5);
        ctx->genComboBoxItem("強さ６", 6);
        ctx->genComboBoxItem("強さ７", 7);
        ctx->genComboBoxItem("強さ８", 8);
        ctx->endComboBox();
    }
    #endif

    /* 0x08 */ f32 mRange;
    /* 0x0C */ f32 mInitialSpeed;
    /* 0x10 */ f32 mMaxSpeed;
    /* 0x14 */ f32 mInitialDownSpeed;
    /* 0x18 */ f32 mMaxDownSpeed;
    /* 0x1C */ f32 mSpinnerSpeed;
    /* 0x20 */ u8 mVibration;
};

#endif /* D_A_OBJ_TOGETRAP_H */
