#ifndef D_MAP_D_MAP_H
#define D_MAP_D_MAP_H

#include "d/d_map_path_dmap.h"
#include "JSystem/JHostIO/JORReflexible.h"

struct dMap_HIO_prm_other_s {
    /* 0x00 */ u8 field_0x0;
    /* 0x04 */ f32 field_0x4;
    /* 0x08 */ u8 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ u8 field_0x10;
    /* 0x11 */ u8 field_0x11;
    /* 0x12 */ u8 field_0x12;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ u8 field_0x1c;
    /* 0x1D */ u8 field_0x1d;
    /* 0x1E */ u8 field_0x1e;
    /* 0x1F */ u8 field_0x1f;
    /* 0x20 */ u8 field_0x20;
    /* 0x21 */ u8 field_0x21;
    /* 0x22 */ u8 field_0x22;
    /* 0x23 */ u8 field_0x23;
    /* 0x24 */ u8 field_0x24[0x2C - 0x24];
    /* 0x2C */ u8 field_0x2c;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ u8 field_0x34;
    /* 0x35 */ u8 field_0x35;
    /* 0x36 */ s16 field_0x36;
    /* 0x38 */ u8 field_0x38;
    /* 0x38 */ s16 field_0x3a;
    /* 0x3C */ u8 field_0x3c;
    /* 0x3D */ u8 field_0x3d;
    /* 0x3E */ s16 field_0x3e;
    /* 0x40 */ u8 field_0x40;
    /* 0x41 */ u8 field_0x41[0x44 - 0x41];
    /* 0x44 */ f32 field_0x44[17];
    /* 0x88 */ u8 field_0x88;
    /* 0x89 */ u8 field_0x89;
    /* 0x8A */ u8 field_0x8a;
    /* 0x8C */ f32 field_0x8c[17];
    /* 0xD0 */ u8 field_0xd0[0x114 - 0xD0];
    /* 0x114 */ dTres_c::typeGroupData_c field_0x114[1];
    /* 0x130 */ u8 field_0x130[0x2f0 - 0x130];
};

struct dMap_prm_res_s {
    /* 0x000 */ dMpath_RGB5A3_palDt_s palette_data[51];
    /* 0x198 */ u8 field_0x198;
    /* 0x199 */ u8 field_0x199;
    /* 0x19A */ u8 field_0x19a;
    /* 0x19B */ u8 field_0x19b;
    /* 0x19C */ u8 field_0x19c;
    /* 0x19D */ u8 field_0x19d;
    /* 0x19E */ u8 field_0x19e;
    /* 0x19F */ u8 field_0x19f;
    /* 0x1A0 */ u8 field_0x1a0;
    /* 0x1A1 */ u8 field_0x1a1;
    /* 0x1A2 */ u8 field_0x1a2;
    /* 0x1A3 */ u8 field_0x1a3;
    /* 0x1A4 */ u8 field_0x1a4;
    /* 0x1A5 */ u8 field_0x1a5;
    /* 0x1A6 */ u8 field_0x1a6;
    /* 0x1A7 */ u8 field_0x1a7;
    /* 0x1A8 */ u8 field_0x1a8;
    /* 0x1A9 */ u8 field_0x1a9;
    /* 0x1AA */ s16 field_0x1aa;
    /* 0x1AC */ s16 field_0x1ac;
    /* 0x1AE */ s16 field_0x1ae;
    /* 0x1B0 */ s16 field_0x1b0;
    /* 0x1B2 */ s16 field_0x1b2;
    /* 0x1B4 */ s16 field_0x1b4;
    /* 0x1B6 */ s16 field_0x1b6;
    /* 0x1B8 */ f32 cursor_size;
};

struct dMap_prm_hio_s {
    /* 0x00 */ GXColor field_0x0[51];
    /* 0xCC */ u8 field_0xcc[0x24];
    /* 0xF0 */ dMap_HIO_prm_other_s field_0xf0;
};

struct dMap_HIO_prm_res_src_s {
    static const u8 m_other[0x2f0];
};

struct dMap_HIO_prm_res_dst_s {
    static dMap_prm_res_s* m_res;
    static dMap_HIO_prm_other_s m_other;
};

class renderingAmap_c : public renderingPlusDoorAndCursor_c {
public:
    int getDispType() const;
    bool isDrawOutSideTrim();
    int getOutSideBlackLineNumber();
    bool isOutSideBlackLine();
    int getLineWidthSub(int);
    int getStayType() const;
    renderingAmap_c() {}

    virtual void draw();
    virtual ~renderingAmap_c() {}
    virtual GXColor* getLineColor(int, int);
    virtual int getLineWidth(int);
    virtual void drawPath();
    virtual void rendering(dDrawPath_c::line_class const*);
    virtual void rendering(dDrawPath_c::poly_class const*);
    virtual void rendering(dDrawPath_c::room_class const*);
    virtual void beforeDrawPath();
    virtual void afterDrawPath();
    virtual const GXColor* getDecoLineColor(int, int);
    virtual s32 getDecorationLineWidth(int);
    virtual int getIconGroupNumber(u8) const;
    virtual bool hasMap() const;
    virtual bool isRendAllRoom() const;
    virtual bool isRendDoor() const;
    virtual bool isCheckFloor() const;
    virtual bool isDrawIconSingle2(dTres_c::data_s const*, bool, bool, int) const;
    virtual bool isRendRestart() const;
    virtual bool isRendCursor() const;
    virtual bool isRendIcon() const { return true; }
    virtual f32 getIconSize(u8) const;
    virtual f32 getPlayerCursorSize();
    virtual f32 getRestartCursorSize();
    virtual void setAmapPaletteColor(int, u8, u8, u8, u8) = 0;
    virtual bool isSpecialOutline() = 0;

#if DEBUG
    static const int PALETTE_NUMBER = 51;
#else
    static const int PALETTE_NUMBER = 50;
#endif

private:
    /* 0x34 */ s32 m_outSideBlackLineCnt;
    /* 0x38 */ s32 field_0x38;
    /* 0x3C */ s32 field_0x3c;
};  // Size: 0x40

class dMap_c : public renderingAmap_c {
public:
    void copyPalette();
    void copyPalette(int, int, f32);
    void setMapPaletteColorAlphaPer(int, f32);
    void setMapPaletteColorAlphaPer(int, int, f32);
    void resCopy();
    dMap_c(int, int, int, int);
    void changeTextureSize(int, int, int);
    void _remove();
    void getMapMinMaxXZ(int, f32*, f32*, f32*, f32*);
    void getPack(int, f32*, f32*);
    void calcMapCenterXZ(int, f32*, f32*);
    void calcMapCmPerTexel(int, f32*);
    int getDispType() const;
    void _move(f32, f32, int, f32);
    void _draw();

    virtual ~dMap_c() {
#if DEBUG
        m_mySelfPointer = NULL;
#endif
        _remove();
    }
    virtual bool isDrawType(int);
    virtual const GXColor* getColor(int);
    virtual bool isRendAllRoom() const;
    virtual int getRoomNoSingle();
    virtual bool isDrawRoom(int, int) const;
    virtual bool isDrawRoomIcon(int, int) const;
    virtual const Vec* getIconPosition(dTres_c::typeGroupData_c*) const;
    virtual dTres_c::typeGroupData_c* getFirstData(u8);
    virtual dTres_c::typeGroupData_c* getNextData(dTres_c::typeGroupData_c*);
    virtual void setAmapPaletteColor(int, u8, u8, u8, u8);
    virtual bool isSpecialOutline();

#if DEBUG
    virtual bool isSwitch(const dDrawPath_c::group_class*);
    virtual void beforeDrawPath();
#endif

    bool isDraw() const { return renderingDAmap_c::isDraw(); }

    ResTIMG* getResTIMGPointer() { return mResTIMG; }
    f32 getTopEdgePlus() { return mTopEdgePlus; }
    f32 getPackPlusZ() { return mPackPlusZ; }
    f32 getPackZ() const { return mPackZ; }
    f32 getTexelPerCm() const { return 1.0f / field_0x58; }
    u16 getTexSizeY() const { return mTexSizeY; }
    f32 getRightEdgePlus() { return mRightEdgePlus; }
    f32 getPackX() const { return mPackX; }
    int getStayRoomNo() const { return mStayRoomNo; }
    f32 getCenterZ() const { return mCenterZ; }

    static dMap_c* m_mySelfPointer;

private:
    /* 0x40 */ dMap_prm_res_s* m_res_src;
    /* 0x44 */ dMap_prm_res_s* m_res;
    /* 0x48 */ ResTIMG* mResTIMG;
    /* 0x4C */ u8* mImage_p;
    /* 0x50 */ f32 mCenterX;
    /* 0x54 */ f32 mCenterZ;
    /* 0x58 */ f32 field_0x58;
    /* 0x5C */ f32 mPackX;
    /* 0x60 */ f32 mPackZ;
    /* 0x64 */ f32 field_0x64;
    /* 0x68 */ f32 mPackPlusZ;
    /* 0x6C */ f32 mRightEdgePlus;
    /* 0x70 */ f32 mTopEdgePlus;
    /* 0x74 */ int field_0x74;
    /* 0x78 */ u16 mTexSizeX;
    /* 0x7A */ u16 mTexSizeY;
    /* 0x7C */ int mStayRoomNo;
    /* 0x80 */ int field_0x80;
    /* 0x84 */ int field_0x84;
    /* 0x88 */ int field_0x88;
    /* 0x8C */ u8 field_0x8c;
    /* 0x8D */ u8 field_0x8d;
    /* 0x8E */ u8 field_0x8e;
    /* 0x8F */ u8 field_0x8f;
    /* 0x90 */ u8 field_0x90;
    /* 0x91 */ u8 field_0x91;
};  // Size: 0x94

class dMap_HIO_list_c : public dMpath_HIO_n::hioList_c {
public:
    virtual void copySrcToHio();
    virtual void copyHioToDst();
    virtual void copyBufToHio(const char*);
};

class dMap_HIO_c : public dMpath_HIO_file_base_c  {
public:
    dMap_HIO_c();
    virtual void listenPropertyEvent(const JORPropertyEvent*);
    virtual void genMessage(JORMContext*);
    virtual u32 addString(char* param_1, u32 param_2, u32 param_3) { return mList.addString(param_1, param_2, param_3); }
    virtual u32 addData(char* param_1, u32 param_2, u32 param_3) {
        UNUSED(param_2);
        UNUSED(param_3);
        memcpy(param_1, dMap_HIO_prm_res_dst_s::m_res, 0x1bc);
        return 0x1bc;
    }
    virtual void copyReadBufToData(const char* param_1, s32 param_2) {
        UNUSED(param_2);
        mList.copyBufToHio(param_1);
    }
    virtual u32 addStringBinary(char* param_1, u32 param_2, u32 param_3) {
        return mList.addStringBinary(param_1, param_2, param_3);
    }

    /* 0x04 */ u8 field_0x4[0x08 - 0x04];
    /* 0x08 */ dMap_HIO_list_c mList;

    static dMap_HIO_c* mMySelfPointer;
    static const u8 l_listData[];
    static const dMpath_HIO_n::list_s l_list;
    static dMap_prm_res_s* m_res_src_p;
    static dMap_prm_hio_s m_prm_hio;
};

#endif /* D_MAP_D_MAP_H */
