#ifndef D_D_MODEL_H
#define D_D_MODEL_H

#include "JSystem/J3DGraphBase/J3DPacket.h"
#include "m_Do/m_Do_mtx.h"

class J3DModelData;
class dKy_tevstr_c;

class dMdl_obj_c {
public:
    dMdl_obj_c() : mpObj(NULL) {}
    MtxP getMtx() { return mMtx; }
    void setMtx(Mtx mtx) { cMtx_copy(mtx, mMtx); }

    /* 0x00 */ Mtx mMtx;
    /* 0x30 */ dMdl_obj_c* mpObj;
};

class dMdl_c : public J3DPacket {
public:
    void create(J3DModelData*, u16, dKy_tevstr_c*);
    void entryObj(dMdl_obj_c*);
    dMdl_c() {
                        mpModelData = NULL;
                        mMaterialId = 0;
                    }

    virtual void draw();
    virtual ~dMdl_c();

    J3DModelData* getModelData() { return mpModelData; }
    u16 getMaterialId() { return mMaterialId; }
    dKy_tevstr_c* getTevstr() { return mpTevstr; }

private:
    /* 0x10 */ J3DModelData* mpModelData;
    /* 0x14 */ dKy_tevstr_c* mpTevstr;
    /* 0x18 */ u16 mMaterialId;
    /* 0x1A */ bool field_0x1a;
    /* 0x1C */ dMdl_obj_c* mpModelObj;
};

class dMdl_mng_c {
public:
    dMdl_mng_c() { field_0x80 = 0; }
    ~dMdl_mng_c() {}

    dMdl_c* search(J3DModelData*, u16, dKy_tevstr_c*);
    dMdl_c* entry(J3DModelData*, u16, dKy_tevstr_c*);
    static dMdl_c* entry(J3DModelData*, u16, int);
    static void create();
    static void remove();
    static void reset();

    static dMdl_mng_c* m_myObj;

private:
    /* 0x00 */ dMdl_c field_0x0[4];
    /* 0x80 */ u8 field_0x80;
};

#endif /* D_D_MODEL_H */
