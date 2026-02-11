#ifndef D_MAP_D_MAP_PATH_H
#define D_MAP_D_MAP_PATH_H

#include "d/d_drawlist.h"
#include "JSystem/JHostIO/JORMContext.h"

struct dMpath_RGB5A3_s {
    u16 color;
};

namespace dMpath_ColorCnv_n {
    void convertRGB5A3_To_GXColor(_GXColor&, const dMpath_RGB5A3_s&);
}

struct dMpath_RGB5A3_palDt_s {
    /* 0x0 */ dMpath_RGB5A3_s field_0x0;
    /* 0x2 */ dMpath_RGB5A3_s field_0x2;
    /* 0x4 */ dMpath_RGB5A3_s field_0x4;
    /* 0x6 */ dMpath_RGB5A3_s field_0x6;
};

class dMpath_RGBA_c {
public:
    GXColor mColor;

    dMpath_RGBA_c() {}
    virtual ~dMpath_RGBA_c() {}
    GXColor getGXColor() const { return mColor; }
    void setGXColor(const GXColor&);
    void setRGB5A3_palDt(const dMpath_RGB5A3_palDt_s&);
};

namespace dMpath_HIO_n {
    struct list_s {
        /* 0x00 */ const void* field_0x0;
        /* 0x04 */ u32 field_0x4;
    };

    struct resData_s {};
    struct hioList_s : public list_s, resData_s {};

    // RTTI does not match debug
    class hioList_c : public hioList_s {
    public:
        virtual ~hioList_c() {}
        virtual void copySrcToHio() = 0;
        virtual void copyHioToDst() = 0;
        virtual void copyBufToHio(const char*) = 0;

        void set(const list_s& param_1) { *static_cast<list_s*>(this) = param_1; }
        void gen(JORMContext*);
        void update(JORMContext*);
        u32 addString(char*, u32, u32) const;
        u32 addStringBinary(char*, u32, u32) const;
    };
}

class dMpath_HIO_file_base_c : public JORReflexible {
public:
    virtual ~dMpath_HIO_file_base_c() {}
    virtual u32 addString(char*, u32, u32) = 0;
    virtual u32 addData(char*, u32, u32) = 0;
    virtual void copyReadBufToData(const char*, s32) = 0;
    virtual u32 addStringBinary(char* param_1, u32 param_2, u32 param_3) {
        return addString(param_1, param_2, param_3);
    }

    BOOL writeHostioTextFile(const char*);
    BOOL writeBinaryTextFile(const char*);
    BOOL writeBinaryFile(const char*);
    void binaryDump(const void*, u32);
    bool readBinaryFile(const char*);
};

class dMpath_RGB5A3_c {
public:
    dMpath_RGB5A3_s mColor;

    dMpath_RGB5A3_c(GXColor c) { set(c); }
    virtual ~dMpath_RGB5A3_c() {}
    void set(u8 r, u8 g, u8 b, u8 a) {
        u32 color;
        if (a >= 224) {
            color =  (r & 0xf8) << 7 | (g & 0xf8) << 2 | (b & 0xf8) >> 3 | 0x8000;
        } else {
            color = (r & 0xf0)  << 4 | g & 0xf0 | (b & 0xf0) >> 4 | (a & 0xe0) << 7;
        }
        mColor.color = color;
    }
    void set(const GXColor& c) { set(c.r, c.g, c.b, c.a); }
    const dMpath_RGB5A3_s& getRGB5A3() const { return mColor; }
};

class dMpath_RGB5A3_palDt_c {
public:
    dMpath_RGB5A3_palDt_s field_0x0;

    dMpath_RGB5A3_palDt_c() {}
    virtual ~dMpath_RGB5A3_palDt_c() {}
    dMpath_RGB5A3_palDt_s& getRGB5A3_palDt_s() { return field_0x0; }

    void setGXColor(const GXColor& c) {
        dMpath_RGB5A3_c rgb5a3(c);
        field_0x0.field_0x0 = rgb5a3.getRGB5A3();
        field_0x0.field_0x2 = rgb5a3.getRGB5A3();
        field_0x0.field_0x4 = rgb5a3.getRGB5A3();
        field_0x0.field_0x6 = rgb5a3.getRGB5A3();
    }
};

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

    f32 getCmPerTexel() const { return mCmPerTexel; }
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
        static const int TEX_OBJ_NUMBER = 7;

        void create();
        void remove();
        ~dTexObjAggregate_c() { remove(); };

        GXTexObj* getTexObjPointer(int i_no) {
            JUT_ASSERT(44, i_no >= 0 && i_no < TEX_OBJ_NUMBER);
            return mp_texObj[i_no];
        }

        dTexObjAggregate_c() {
            for (int i = 0; i < TEX_OBJ_NUMBER; i++) {
                mp_texObj[i] = NULL;
            }
        }

        /* 0x0 */ GXTexObj* mp_texObj[TEX_OBJ_NUMBER];
    };

    static dTexObjAggregate_c m_texObjAgg;
};

STATIC_ASSERT(sizeof(dMpath_n::dTexObjAggregate_c) == 28);

#endif /* D_MAP_D_MAP_PATH_H */
