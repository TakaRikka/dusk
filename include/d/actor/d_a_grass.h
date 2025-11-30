#ifndef D_A_GRASS_H
#define D_A_GRASS_H

#include "f_op/f_op_actor_mng.h"

class dCcMassS_HitInf;
class cCcD_Obj;

class dGrass_data_c {
public:
    void WorkCo(fopAc_ac_c*, u32, int);
    void WorkAt_NoCutAnim(fopAc_ac_c*, u32, int, dCcMassS_HitInf*, cCcD_Obj*);
    void Direction_Set(fopAc_ac_c*, u32, int, dCcMassS_HitInf*, cCcD_Obj*, csXyz*);
    void WorkAt(fopAc_ac_c*, u32, int, dCcMassS_HitInf*, u16);
    void hitCheck(int, u16);
    ~dGrass_data_c() {}
    dGrass_data_c() {
        m_state = 0;
    }

    /* 0x00 */ u8 m_state;
    /* 0x01 */ u8 field_0x01;
    /* 0x02 */ s8 field_0x02;
    /* 0x03 */ u8 m_tableNo;
    /* 0x04 */ u8 m_itemBitNo;
    /* 0x05 */ u8 field_0x05;
    /* 0x06 */ u8 field_0x06[0x08 - 0x06];
    /* 0x08 */ s16 m_addCol;
    /* 0x0C */ cXyz m_pos;
    /* 0x18 */ Mtx m_modelMtx;
    /* 0x48 */ dGrass_data_c* mp_next;
};

class dGrass_room_c {
public:
    void newData(dGrass_data_c*);
    void deleteData();
    dGrass_room_c() {
        mp_data = NULL;
    }

    dGrass_data_c* getData() { return mp_data; }

    /* 0x0 */ dGrass_data_c* mp_data;
};

class dGrass_anm_c {
public:
    dGrass_anm_c() {
        m_state = 0;
    }

    /* 0x00 */ u8 m_state;
    /* 0x02 */ s16 m_angY;
    /* 0x04 */ s16 m_angX;
    /* 0x08 */ Mtx m_animMtx;
};

class dGrass_packet_c : public J3DPacket {
public:
    dGrass_packet_c();
    void calc();
    void update();
    void setData(dGrass_data_c*, int, cXyz&, int, u8, u8, s16, u8);
    dGrass_data_c* newData(cXyz&, int, u8, u8, s16, u8);
    void deleteRoom(int);
    int newAnm();
    void setAnm(int, s16);

    virtual void draw();
    virtual ~dGrass_packet_c();

    typedef void (dGrass_packet_c::*deleteFunc)(int);
    static deleteFunc m_deleteRoom;

    dGrass_anm_c* getAnm() { return m_anm; }
    dGrass_anm_c* getAnm(int i_no) { return &m_anm[i_no]; }

    dGrass_data_c* getData() { return m_data; }

    void deleteAnm(int i_idx) { m_anm[i_idx].m_state = 0; }

    /* 0x00010 */ u16 m_dataNum;
    /* 0x00014 */ dGrass_data_c m_data[1500];
    /* 0x1BD64 */ dGrass_anm_c m_anm[112];
    /* 0x1D5E4 */ dGrass_room_c m_room[64];
    /* 0x1D6E4 */ Vec* mp_pos;
    /* 0x1D6E8 */ Vec* mp_normal;
    /* 0x1D6EC */ GXColor* mp_colors;
    /* 0x1D6F0 */ Vec* mp_texCoords;
    /* 0x1D6F4 */ u8* mp_kusa9q_DL;
    /* 0x1D6F8 */ u32 m_kusa9q_DL_size;
    /* 0x1D6FC */ u8* mp_kusa9q_14_DL;
    /* 0x1D700 */ u32 m_kusa9q_DL_14_size;
    /* 0x1D704 */ u8* mp_Mkusa_9q_DL;
    /* 0x1D708 */ u32 m_Mkusa_9q_DL_size;
    /* 0x1D70C */ u8* mp_Mkusa_9q_cDL;
    /* 0x1D710 */ u32 m_Mkusa_9q_cDL_size;
    /* 0x1D714 */ s16 field_0x1d714;
};  // Size: 0x1D718

STATIC_ASSERT(sizeof(dGrass_packet_c) == 0x1D718);

class dFlower_data_c {
public:
    void WorkCo(fopAc_ac_c*, u32, int);
    void deleteAnm();
    void WorkAt_NoCutAnim(fopAc_ac_c*, u32, int, dCcMassS_HitInf*, cCcD_Obj*);
    void WorkAt(fopAc_ac_c*, u32, int, dCcMassS_HitInf*);
    void hitCheck(fopAc_ac_c*, int);
    ~dFlower_data_c();
    dFlower_data_c();

    /* 0x00 */ u8 m_state;
    /* 0x01 */ s8 field_0x01;
    /* 0x02 */ s8 m_tableNo;
    /* 0x03 */ u8 field_0x03;
    /* 0x04 */ s16 field_0x04;
    /* 0x08 */ cXyz m_pos;
    /* 0x14 */ Mtx m_modelMtx;
    /* 0x44 */ dFlower_data_c* mp_next;
};

class dFlower_room_c {
public:
    void newData(dFlower_data_c*);
    void deleteData();
    dFlower_room_c();

    dFlower_data_c* getData() { return mp_data; }

    /* 0x0 */ dFlower_data_c* mp_data;
};

class dFlower_anm_c {
public:
    dFlower_anm_c();

    /* 0x00 */ u8 m_state;
    /* 0x02 */ s16 m_angY;
    /* 0x04 */ s16 m_angX;
    /* 0x08 */ Mtx m_animMtx;
};

class dFlower_packet_c : public J3DPacket {
public:
    dFlower_packet_c();
    void calc();
    void update();
    void setData(dFlower_data_c*, int, s8, cXyz&, int, s8, s16);
    dFlower_data_c* newData(s8, cXyz&, int, s8, s16);
    void deleteRoom(int);
    int newAnm();
    void setAnm(int, s16);

    virtual void draw();
    virtual ~dFlower_packet_c();

    typedef void (dFlower_packet_c::*deleteFunc)(int);
    static deleteFunc m_deleteRoom;

    dFlower_anm_c* getAnm(int i_idx) { return &m_anm[i_idx]; }
    dFlower_anm_c* getAnm() { return &m_anm[0]; }
    dFlower_data_c* getData() { return m_data; }

    void deleteAnm(int i_idx) { m_anm[i_idx].m_state = 0; }

    void setPlayerCutFlg(int i_flag) { m_playerCutFlg = i_flag != 0; }
    void setPlayerSwordAngY(s16 i_angY) { m_playerSwordAngY = i_angY; }
    void setPlayerSwordMoveAngY(s16 i_angY) { m_playerSwordMoveAngY = i_angY; }
    void setPlayerSwordTop(cXyz& i_pos) { m_playerSwordTop = i_pos; }
    cXyz& getPlayerSwordTop() { return m_playerSwordTop; }

    /* 0x00010 */ u16 m_dataNum;
    /* 0x00014 */ dFlower_data_c m_data[1000];
    /* 0x11954 */ dFlower_anm_c m_anm[72];
    /* 0x12914 */ dFlower_room_c m_room[64];
    /* 0x12A14 */ u8 m_playerCutFlg;
    /* 0x12A16 */ s16 m_playerSwordAngY;
    /* 0x12A18 */ s16 m_playerSwordMoveAngY;
    /* 0x12A1C */ cXyz m_playerSwordTop;
    /* 0x12A28 */ Vec* mp_pos;
    /* 0x12A2C */ GXColor* mp_colors;
    /* 0x12A30 */ Vec* mp_texCoords;
    /* 0x12A34 */ u8* mp_mat2DL;
    /* 0x12A38 */ u32 m_mat2DL_size;
    /* 0x12A3C */ u8* mp_mat2Light4DL;
    /* 0x12A40 */ u32 m_mat2Light4DL_size;
    /* 0x12A44 */ u8* mp_Jhana01DL;
    /* 0x12A48 */ u32 m_Jhana01DL_size;
    /* 0x12A4C */ u8* mp_Jhana01_cDL;
    /* 0x12A50 */ u32 m_Jhana01_cDL_size;
};  // Size: 0x12A54

/**
 * @ingroup actors-unsorted
 * @class daGrass_c
 * @brief Grass
 *
 * @details
 *
 */
class daGrass_c : public fopAc_ac_c {
public:
    bool createGrass();
    void deleteGrass();
    void executeGrass();
    void drawGrass();
    void newGrassData(cXyz&, int, u8, u8, s16, u8);
    bool createFlower();
    void deleteFlower();
    void executeFlower();
    void drawFlower();
    void newFlowerData(s8, cXyz&, int, s8, s16);

    inline int create();

    inline int Delete();
    inline int execute();
    inline int draw();

    static dGrass_packet_c* getGrass() { return m_grass; }
    static dFlower_packet_c* getFlower() { return m_flower; }

    static void deleteRoomGrass(int);
    static void deleteRoomFlower(int);

    static daGrass_c* m_myObj;
    static dGrass_packet_c* m_grass;
    static dFlower_packet_c* m_flower;
};

namespace daGrass_prm {
    inline u8 getKind(daGrass_c* i_this) { return (fopAcM_GetParam(i_this) >> 4) & 3; }
    inline u8 getType(daGrass_c* i_this) { return fopAcM_GetParam(i_this) & 0xF; }
    inline u8 getItemNo(daGrass_c* i_this) { return (fopAcM_GetParam(i_this) >> 0x10) & 0xFF; }
    inline u8 getItemBitNo(daGrass_c* i_this) { return (fopAcM_GetParam(i_this) >> 8) & 0xFF; }
};

struct daGrass_offsetData {
    /* 0x0 */ u8 num;
    /* 0x4 */ csXyz* set_type;
};


#endif /* D_A_GRASS_H */
