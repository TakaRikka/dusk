#ifndef D_A_CSTAF_H
#define D_A_CSTAF_H

#include "Z2AudioLib/Z2Creature.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_particle_copoly.h"

/**
 * @ingroup actors-unsorted
 * @class daCstaF_c
 * @brief Dominion Rod Statue
 *
 * @details
 *
 */
class daCstaF_c : public dBgS_MoveBgActor {
public:
    BOOL checkCoverModel();
    int create();
    ~daCstaF_c();
    void setRoomInfo();
    void setMatrix();
    void posMove();
    void setCollision();
    void setAnime();
    BOOL initBrk(u16);
    void initStopBrkBtk();
    void initStartBrkBtk();

    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();

    cXyz& getBallPos() { return m_ballPos; }
    void onPlayerRide() { m_isPlayerRide = true; }
    void onCoverVanishFlg() { m_coverVanishFlg = 1; }

    struct BckTbl {
        u16 idx[4];
    };
    static const BckTbl m_bckIdxTable[];

private:
    /* 0x5A0 */ const char* m_arcName;
    /* 0x5A4 */ request_of_phase_process_class m_phase;
    /* 0x5AC */ J3DModel* mp_model;
    /* 0x5B0 */ mDoExt_McaMorfSO* mp_modelMorf;
    /* 0x5B4 */ J3DModel* mp_coverModel;
    /* 0x5B8 */ J3DAnmTextureSRTKey* m_coverBtk;
    /* 0x5BC */ J3DAnmTevRegKey* m_coverBrk;
    /* 0x5C0 */ mDoExt_btkAnm m_btk;
    /* 0x5D8 */ mDoExt_brkAnm m_brk;
    /* 0x5F0 */ dBgS_AcchCir m_acchCir[4];
    /* 0x6F0 */ dBgS_StatueAcch m_acch;
    /* 0x8C8 */ dCcD_Stts m_cc_stts;
    /* 0x904 */ dCcD_Cyl m_cc_cyl;
    /* 0xA40 */ Z2Creature m_sound;
    /* 0xAD0 */ dPaPo_c m_poly_eff;
    /* 0xB08 */ s8 m_reverb;
    /* 0xB09 */ u8 m_type;
    /* 0xB0A */ u8 m_action;
    /* 0xB0B */ u8 m_swbit;
    /* 0xB0C */ u8 m_isStartBrkBtkInit;
    /* 0xB0D */ u8 m_isPlayerRide;
    /* 0xB0E */ u8 m_mapToolID;
    /* 0xB0F */ u8 m_isStop;
    /* 0xB10 */ u8 m_coverVanishFlg;
    /* 0xB12 */ s16 m_eventId;
    /* 0xB14 */ u8 unk_0xb14[4];
    /* 0xB18 */ u32 m_shadowKey;
    /* 0xB1C */ f32 m_lastGroundH;
    /* 0xB20 */ cXyz m_playerPos;
    /* 0xB2C */ cXyz m_ballPos;
};

#endif /* D_A_CSTAF_H */
