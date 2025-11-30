#ifndef D_MAP_D_MAP_PATH_H
#define D_MAP_D_MAP_PATH_H

#include "d/d_drawlist.h"

class dDrawPath_c : public dDlst_base_c {
public:
    struct line_class {
        /* 0x00 */ u8 field_0x0;
        /* 0x01 */ u8 field_0x1;
        /* 0x02 */ u8 mDataNum;
        /* 0x03 */ u8 field_0x3;
        /* 0x04 */ u16* mpData;
    };  // Size: 0x8

    struct poly_class {
        /* 0x00 */ u8 field_0x0;
        /* 0x01 */ u8 mDataNum;
        /* 0x04 */ u16* mpData;
    };  // Size: 0x8

    struct group_class {
        /* 0x00 */ u8 mSwbit;
        /* 0x01 */ u8 field_0x1;
        /* 0x02 */ u8 mLineNum;
        /* 0x03 */ u8 field_0x3;
        /* 0x04 */ u8 mPolyNum;
        /* 0x08 */ dDrawPath_c::line_class* mpLine;
        /* 0x0C */ u8 field_0xc[4];
        /* 0x10 */ dDrawPath_c::poly_class* mpPoly;
    };  // Size: 0x14

    struct floor_class {
        /* 0x0 */ s8 mFloorNo;
        /* 0x1 */ u8 mGroupNum;
        /* 0x4 */ dDrawPath_c::group_class* mpGroup;
    };  // Size: 0x8

    struct room_class {
        /* 0x0 */ u8 mFloorNum;
        /* 0x4 */ dDrawPath_c::floor_class* mpFloor;
        /* 0x8 */ f32* mpFloatData;  // might be Vec or cXyz instead
    };

    struct layer_data {
        /* 0x0 */ room_class* mRooms[0x2][0x40];
    };

    void rendering(dDrawPath_c::group_class const*);
    void rendering(dDrawPath_c::floor_class const*);

    virtual ~dDrawPath_c() {}
    virtual bool isDrawType(int) { return true; }
    virtual const GXColor* getColor(int) = 0;
    virtual const GXColor* getLineColor(int param_0, int) { return (GXColor*)getColor(param_0); }
    virtual int getLineWidth(int) = 0;
    virtual bool isSwitch(dDrawPath_c::group_class const*) = 0;
    virtual bool isRenderingFloor(int) = 0;
    virtual room_class* getFirstRoomPointer() = 0;
    virtual room_class* getNextRoomPointer() = 0;
    virtual void drawPath();
    virtual void rendering(dDrawPath_c::line_class const*);
    virtual void rendering(dDrawPath_c::poly_class const*);
    virtual void rendering(dDrawPath_c::room_class const*);
};

class dDrawPathWithNormalPattern_c : public dDrawPath_c {
public:
    virtual ~dDrawPathWithNormalPattern_c() {}
};

class dRenderingMap_c : public dDrawPathWithNormalPattern_c {
public:
    void makeResTIMG(ResTIMG*, u16, u16, u8*, u8*, u16) const;
    void renderingMap();

    virtual ~dRenderingMap_c() {}
    virtual void beforeDrawPath() = 0;
    virtual void afterDrawPath() = 0;

    virtual void preDrawPath() = 0;
    virtual void postDrawPath() = 0;
    virtual bool isDrawPath() = 0;
    virtual void preRenderingMap() = 0;
    virtual void postRenderingMap() = 0;
};

class dRenderingFDAmap_c : public dRenderingMap_c {
public:
    dRenderingFDAmap_c() {
        field_0x4 = NULL;
        field_0x8 = 0.0f;
        field_0xc = 0.0f;
        mPosX = 0.0f;
        mPosZ = 0.0f;
        mCmPerTexel = 0.0f;
        mTexWidth = 0;
        mTexHeight = 0;
        field_0x20 = 0;
        field_0x22 = 0;
    }

    void setTevSettingNonTextureDirectColor() const;
    void setTevSettingIntensityTextureToCI() const;
    void drawBack() const;
    void renderingDecoration(dDrawPath_c::line_class const*);

    virtual ~dRenderingFDAmap_c() {}
    virtual void preRenderingMap();
    virtual void postRenderingMap();
    virtual const GXColor* getBackColor() const = 0;
    virtual const GXColor* getDecoLineColor(int, int);
    virtual s32 getDecorationLineWidth(int);

    bool isDrawAreaCheck(const Vec& param_0) {
        return (param_0.x >= mPosX - field_0x8 * 2.0f &&
                param_0.x <= mPosX + field_0x8 * 2.0f) &&
               (param_0.z >= mPosZ - field_0xc * 2.0f &&
                param_0.z <= mPosZ + field_0xc * 2.0f);
    }

    /* 0x04 */ u8* field_0x4;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 mPosX;
    /* 0x14 */ f32 mPosZ;
    /* 0x18 */ f32 mCmPerTexel;
    /* 0x1C */ u16 mTexWidth;
    /* 0x1E */ u16 mTexHeight;
    /* 0x20 */ u16 field_0x20;
    /* 0x22 */ u16 field_0x22;
};  // Size: 0x24

struct dMpath_n {
    class dTexObjAggregate_c {
    public:
        void create();
        void remove();
        ~dTexObjAggregate_c() { remove(); };

        GXTexObj* getTexObjPointer(int i_no) { return mp_texObj[i_no]; }

        dTexObjAggregate_c() {
            for (int i = 0; i < 7; i++) {
                mp_texObj[i] = NULL;
            }
        }

        /* 0x0 */ GXTexObj* mp_texObj[7];
    };

    static dTexObjAggregate_c m_texObjAgg;
};

STATIC_ASSERT(sizeof(dMpath_n::dTexObjAggregate_c) == 28);

struct dMpath_RGB5A3_s {
    u16 color;
};

struct dMpath_RGB5A3_palDt_s {
    /* 0x0 */ dMpath_RGB5A3_s field_0x0;
    /* 0x2 */ dMpath_RGB5A3_s field_0x2;
    /* 0x4 */ dMpath_RGB5A3_s field_0x4;
    /* 0x6 */ dMpath_RGB5A3_s field_0x6;
};

#endif /* D_MAP_D_MAP_PATH_H */
