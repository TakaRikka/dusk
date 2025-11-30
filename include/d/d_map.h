#ifndef D_MAP_D_MAP_H
#define D_MAP_D_MAP_H

#include "d/d_map_path_dmap.h"

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

struct dMap_HIO_prm_res_dst_s {
    static dMap_prm_res_s* m_res;
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

    static const int PALETTE_NUMBER = 50;

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
    void _remove();
    void getMapMinMaxXZ(int, f32*, f32*, f32*, f32*);
    void getPack(int, f32*, f32*);
    void calcMapCenterXZ(int, f32*, f32*);
    void calcMapCmPerTexel(int, f32*);
    int getDispType() const;
    void _move(f32, f32, int, f32);
    void _draw();

    virtual ~dMap_c() { _remove(); }
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

    bool isDraw() const { return renderingDAmap_c::isDraw(); }

    ResTIMG* getResTIMGPointer() { return mResTIMG; }
    f32 getTopEdgePlus() { return mTopEdgePlus; }
    f32 getPackPlusZ() { return mPackPlusZ; }
    f32 getPackZ() { return mPackZ; }
    f32 getTexelPerCm() { return 1.0f / field_0x58; }
    u16 getTexSizeY() { return mTexSizeY; }
    f32 getRightEdgePlus() { return mRightEdgePlus; }
    f32 getPackX() { return mPackX; }

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
};  // Size: 0x94

#endif /* D_MAP_D_MAP_H */
