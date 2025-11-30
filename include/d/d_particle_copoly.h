#ifndef D_PARTICLE_D_PARTICLE_COPOLY_H
#define D_PARTICLE_D_PARTICLE_COPOLY_H

#include "d/d_com_inf_game.h"

class dBgS_Acch;
class dKy_tevstr_c;
class JPABaseEmitter;

/**
 * @brief Particle effect management class for center-based effects
 * 
 * This class handles particle effects, particularly those related to water interactions.
 * It manages emitters, water offsets, and ripple effects with a center-based configuration.
 * 
 * @details The class maintains:
 * - Center-based particle emitters
 * - Water and ripple offset values
 * - Effect type information
 * - Background collision checking capabilities
 * 
 * @note This serves as the base class for dPaPoT_c and dPaPoF_c which extend the 
 * functionality to handle two and four particle emitters respectively.
 */
class dPaPo_c {
public:
    void init(dBgS_Acch*, f32, f32);
    int checkWaterIn(cXyz*, cXyz const*, dKy_tevstr_c const*, u32, s8, f32, f32);
    void setEffect(u32*, int, dKy_tevstr_c const*, cXyz const*, u32, u32,
                                  cXyz const*, csXyz const*, s8, u8*, u8*);
    int setEffectCenter(dKy_tevstr_c const*, cXyz const*, u32, u32, cXyz const*,
                                       csXyz const*, cXyz const*, s8, f32, f32);
    void clearID(u32*, u8*);

    void clearCenterID() { clearID(field_0x8[0], &field_0x36); }
    void changeWaterOffset(f32 i_offset) { mWaterOffset = i_offset; }
    void changeRippleOffset(f32 i_offset) { mRippleOffset = i_offset; }
    u8 getCenterEffType(int i_idx) const { return mCenterEffType[i_idx]; }
    JPABaseEmitter* getCenterEmitter(int param_0, int param_1) {
        return dComIfGp_particle_getEmitter(field_0x8[param_0][param_1]);
    }

    /* 0x00 */ u32 field_0x0[2];
    /* 0x08 */ u32 field_0x8[2][4];
    /* 0x28 */ dBgS_Acch* field_0x28;
    /* 0x2C */ f32 mWaterOffset;
    /* 0x30 */ f32 mRippleOffset;
    /* 0x34 */ u8 mCenterEffType[2];
    /* 0x36 */ u8 field_0x36;
    /* 0x37 */ u8 field_0x37;
};

class dPaPoT_c : public dPaPo_c {
public:
    int setEffectTwo(dKy_tevstr_c const*, cXyz const*, u32, u32, cXyz const*,
                                    cXyz const*, cXyz const*, csXyz const*, cXyz const*, s8, f32,
                                    f32);
    void clearTwoAllID();

    void clearLeftID() { clearID(mLeftEmitter[0], &field_0x7c); }
    void clearRightID() { clearID(mRightEmitter[0], &field_0x7d); }
    u8 getRightEffType(int i_idx) const { return mRightEffType[i_idx]; }
    u8 getLeftEffType(int i_idx) const { return mLeftEffType[i_idx]; }
    JPABaseEmitter* getRightEmitter(int param_0, int param_1) {
        return dComIfGp_particle_getEmitter(mRightEmitter[param_0][param_1]);
    }
    JPABaseEmitter* getLeftEmitter(int param_0, int param_1) {
        return dComIfGp_particle_getEmitter(mLeftEmitter[param_0][param_1]);
    }

    typedef u8 (dPaPoT_c::*effTypeFunc)(int) const;
    typedef JPABaseEmitter* (dPaPoT_c::*emitterFunc)(int, int);

    u8 getTypeTwo(int param_0, int param_1) const {
        return (this->*m_typeTwoData[param_0])(param_1);
    }
    JPABaseEmitter* getEmitterTwo(int param_0, int param_1, int param_2) {
        return (this->*m_emitterTwoData[param_0])(param_1, param_2);
    }

    static effTypeFunc m_typeTwoData[];
    static emitterFunc m_emitterTwoData[];

    /* 0x38 */ u32 mLeftEmitter[2][4];
    /* 0x58 */ u32 mRightEmitter[2][4];
    /* 0x78 */ u8 mLeftEffType[2];
    /* 0x7A */ u8 mRightEffType[2];
    /* 0x7C */ u8 field_0x7c;
    /* 0x7D */ u8 field_0x7d;
};

class dPaPoF_c : public dPaPoT_c {
public:
    int setEffectFour(dKy_tevstr_c const*, cXyz const*, u32, u32, cXyz const*,
                                     cXyz const*, cXyz const*, cXyz const*, cXyz const*,
                                     csXyz const*, cXyz const*, s8, f32, f32);
    void clearFourAllID();

    void clearBLeftID() { clearID(mBackLeftEmitter[0], &field_0xc4); }
    void clearBRightID() { clearID(mBackRightEmitter[0], &field_0xc5); }
    u8 getBackRightEffType(int i_idx) const { return mBackRightEffType[i_idx]; }
    u8 getBackLeftEffType(int i_idx) const { return mBackLeftEffType[i_idx]; }
    JPABaseEmitter* getBackRightEmitter(int param_0, int param_1) {
        return dComIfGp_particle_getEmitter(mBackRightEmitter[param_0][param_1]);
    }
    JPABaseEmitter* getBackLeftEmitter(int param_0, int param_1) {
        return dComIfGp_particle_getEmitter(mBackLeftEmitter[param_0][param_1]);
    }

    typedef u8 (dPaPoF_c::*effTypeFunc)(int) const;
    typedef JPABaseEmitter* (dPaPoF_c::*emitterFunc)(int, int);

    u8 getTypeFour(int param_0, int param_1) const {
        return (this->*m_typeFourData[param_0])(param_1);
    }
    JPABaseEmitter* getEmitterFour(int param_0, int param_1, int param_2) {
        return (this->*m_emitterFourData[param_0])(param_1, param_2);
    }

    static effTypeFunc m_typeFourData[];
    static emitterFunc m_emitterFourData[];

    /* 0x80 */ u32 mBackLeftEmitter[2][4];
    /* 0xA0 */ u32 mBackRightEmitter[2][4];
    /* 0xC0 */ u8 mBackLeftEffType[2];
    /* 0xC2 */ u8 mBackRightEffType[2];
    /* 0xC4 */ u8 field_0xc4;
    /* 0xC5 */ u8 field_0xc5;
};

#endif /* D_PARTICLE_D_PARTICLE_COPOLY_H */
