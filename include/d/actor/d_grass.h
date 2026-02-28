#ifndef D_GRASS_H
#define D_GRASS_H

#include "JSystem/J3DGraphBase/J3DPacket.h"
#include "SSystem/SComponent/c_xyz.h"

class cCcD_Obj;
class csXyz;
class dCcMassS_HitInf;
class fopAc_ac_c;

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

    static deleteFunc getDeleteRoom() {
        JUT_ASSERT(181, m_deleteRoom != NULL);
        return m_deleteRoom;
    }

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

#endif /* D_GRASS_H */
