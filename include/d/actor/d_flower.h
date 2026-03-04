#ifndef D_FLOWER_H
#define D_FLOWER_H

#include "JSystem/J3DGraphBase/J3DPacket.h"
#include "SSystem/SComponent/c_xyz.h"

class cCcD_Obj;
class dCcMassS_HitInf;
class fopAc_ac_c;

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

    static deleteFunc getDeleteRoom() {
        JUT_ASSERT(174, m_deleteRoom != NULL);
        return m_deleteRoom;
    }

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

#endif /* D_FLOWER_H */
