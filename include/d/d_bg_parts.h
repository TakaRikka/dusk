#ifndef D_D_BG_PARTS
#define D_D_BG_PARTS

#include "f_op/f_op_actor.h"
#include "d/d_com_inf_game.h"
#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "JSystem/J3DGraphBase/J3DPacket.h"
#include "m_Do/m_Do_ext.h"
#include "d/d_bg_w.h"

class dBgp_c {
public:
    class material_c {
    public:
        material_c() {}

        void draw();
        material_c* getNext() { return mpNext; }
        material_c** getNext_p() { return &mpNext; }

        void setNext(material_c* i_material) {
            mpNext = i_material;
        }

        virtual void drawSimple() = 0;
        virtual bool isSame(material_c*) { return false; }

        virtual void addChild(material_c* i_material) {
            UNUSED(i_material);
            JUT_ASSERT(78, FALSE);
        }

        /* 0x4 */ material_c* mpNext;
    };

    class modelMaterial_c : public material_c {
    public:
        modelMaterial_c() {}

        MtxPtr getMtx() {
            return mpMtx;
        }

        J3DModelData* getModelData() {
            return mpModelData;
        }

        modelMaterial_c* getChild() {
            return mpChild;
        }

        J3DMaterial* getMaterial() {
            return mpMaterial;
        }

        u16 getMaterialID() { return mMaterialID; }

        void setChild(modelMaterial_c* i_child) {
            mpChild = i_child;
        }

        void set(J3DModelData*, J3DMaterial*, Mtx);

        virtual void drawSimple();
        virtual bool isSame(material_c*);
        virtual void addChild(material_c*);

        /* 0x08 */ u16 mMaterialID;
        /* 0x0C */ J3DModelData* mpModelData;
        /* 0x10 */ J3DMaterial* mpMaterial;
        /* 0x14 */ MtxPtr mpMtx;
        /* 0x18 */ modelMaterial_c* mpChild;
    };

    class packet_c : public J3DPacket {
    public:
        packet_c();
        ~packet_c();

        void draw();
        s8 getRoomNo() { return mRoomNo; }
        void setRoomNo(s8 i_roomNo) {
            mRoomNo = i_roomNo;
        }
        void reset();
        void entryMaterial(material_c*, u32);
        void entryModel(modelMaterial_c*);
        void entryShare(modelMaterial_c*);

        static const int MATERIAL_SORT_LEVEL = 2;

        /* 0x10 */ s8 mRoomNo;
        /* 0x14 */ material_c* mpMaterials[64];
    };

    class model_c {
    public:
        model_c() {
            mMaterial = NULL;
        }

        void create(J3DModelData*, Mtx);
        void remove();

        MtxPtr getBaseTRMtx() {
            return mBaseTRMtx;
        }

        modelMaterial_c* getMaterial() {
            return mMaterial;
        }

        /* 0x00 */ u16 mId;
        /* 0x04 */ Mtx mBaseTRMtx;
        /* 0x34 */ modelMaterial_c* mMaterial;
    };

    class share_c {
    public:
        share_c();
        ~share_c() {}

        void set(u16);
        void reset();

        const char* getArcName();
        int execute();
        void draw();
        void entry(packet_c*);
        void cbNormal();
        void cbConveyor();

        u16 getId() { return mId; }
        u16 getCount() { return mCount; }
        bool isUsed() { return mStatus != -1; }
        bool isDraw() { return mStatus == 0; }

        void setCallback(void (share_c::*i_callback)()) {
            mCallback = i_callback;
        }

        /* 0x00 */ s8 mStatus;
        /* 0x01 */ u8 field_0x1;
        /* 0x02 */ u16 mId;
        /* 0x04 */ u16 mCount;
        /* 0x08 */ J3DModelData* mModelData;
        /* 0x0C */ mDoExt_btkAnm mBtk;
        /* 0x24 */ mDoExt_brkAnm mBrk;
        /* 0x3C */ modelMaterial_c* mMaterial;
        /* 0x40 */ void (share_c::*mCallback)();
    };

    // made up names except for `unit_group_class`
    struct unit_class;
    struct unit_group_class;

    struct stage_map_unit_class {
        /* 0x0 */ int num;
        /* 0x4 */ unit_class* entries;
    };

    struct unit_unk_data2_class {
        /* 0x00 */ u8 field_0x0[3];
        /* 0x03 */ u8 field_0x3;
        /* 0x04 */ u8 field_0x4;
        /* 0x05 */ u8 field_0x5;
        /* 0x06 */ s16 angle;
        /* 0x08 */ Vec pos;
        /* 0x14 */ u8 field_0x14;
        /* 0x15 */ u8 field_0x15;
    };

    struct unit_unk_data_class {
        u16 field_0x0;
        u16 field_0x2;
    };

    struct unit_class {
        /* 0x0 */ int num;
        /* 0x4 */ unit_group_class* groups;
        /* 0x8 */ unit_unk_data_class* unk_0x8;
        /* 0xC */ unit_unk_data2_class* unk_0xc;
    };

    struct unit_group_class {
        /* 0x0 */ u8 unk_0x0;
        /* 0x1 */ u8 switchNo;
        /* 0x2 */ u16 unk_0x2;
    };

    dBgp_c();
    ~dBgp_c();

    void create(s8, void*);
    int remove();
    BOOL isSwitch(unit_group_class*);
    void registBg(fopAc_ac_c*);
    void releaseBg();
    int registBg(int, fopAc_ac_c*);
    void releaseBg(int);
    int execute(bool);
    void draw(fopAc_ac_c*);

    void entryPacket() {
        dComIfGd_getOpaListBG()->entryImm(&mPacket, 0);
    }

    void entryModel(model_c* i_model) {
        modelMaterial_c* var_30 = i_model->getMaterial();
        for (int i = var_30->getModelData()->getMaterialNum() - 1; i >= 0; i--) {
            mPacket.entryModel(var_30);
            var_30++;
        }
    }

    static void setPointer(void* i_pointer);

    static u16 getArg0(u8* param_0) {
        return ((param_0[1] & 0xF) << 8) | (param_0[2]);
    }

    static u16 getId(u8* param_0) {
        return (param_0[0] << 4) | ((param_0[1] >> 4) & 0xF);
    }

    static const char* getArcName(u8* param_0) {
        return getArcName(getId(param_0), getArg0(param_0));
    }

    static const char* getArcName(u16, u16);

    static void createShare();
    static void removeShare();
    static void addShare(u16);
    static void cutShare(u16);
    static bool executeShare();
    static void drawShare();
    static void entryShare(packet_c*);

    static JKRSolidHeap* mShareHeap;
    static share_c* mShare;

    /* 0x000 */ void* mPointer;
    /* 0x004 */ char mArcName[8];
    /* 0x00C */ JKRSolidHeap* mHeap;
    /* 0x010 */ JKRSolidHeap* mHeap2;
    /* 0x014 */ dBgW* mBgWork;
    /* 0x018 */ model_c* mModel;
    /* 0x01C */ packet_c mPacket;
    /* 0x130 */ u16 field_0x130;
};

#endif /* D_D_BG_PARTS */
