#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_menu_fmap2D.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_lib.h"
#include "d/d_menu_fmap.h"
#include "d/d_meter2_info.h"
#include "d/d_meter_HIO.h"
#include "d/d_msg_class.h"
#include "d/d_msg_scrn_explain.h"
#include "m_Do/m_Do_graphic.h"
#include "d/actor/d_a_midna.h"

dMenu_Fmap2DBack_c::dMenu_Fmap2DBack_c() {
    dMeter2Info_setMapDrugFlag(0);

    for (int i = 0; i < 8; i++) {
        mpAreaTex[i] = NULL;
        mRegionMinMapX[i] = 0.0;
        mRegionMinMapY[i] = 0.0;
        mRegionMapSizeX[i] = 0.0;
        mRegionMapSizeY[i] = 0.0;
        field_0xf0c[i] = 0.0;
        field_0xf2c[i] = 0.0;
        field_0xf4c[i] = 0.0;
        field_0xf6c[i] = 0.0;
        mRegionOriginX[i] = 0.0;
        mRegionOriginZ[i] = 0.0;
        mRegionTexData[i].mMinX = 0.0;
        mRegionTexData[i].mMinZ = 0.0;
        mRegionTexData[i].mMaxX = 0.0;
        mRegionTexData[i].mMaxZ = 0.0;
        mRegionTexData[i].mScale = 0.0;
        mRegionTextureReadNum[i] = 0xff;
        field_0x1230[i] = 0xff;
        mRegionScrollMinX[i] = 0.0;
        mRegionScrollMinZ[i] = 0.0;
        mRegionScrollMaxX[i] = 0.0;
        mRegionScrollMaxZ[i] = 0.0;
        mRegionScrollMapMinX[i] = 0.0;
        mRegionScrollMapMinY[i] = 0.0;
        mRegionScrollMapMaxX[i] = 0.0;
        mRegionScrollMapMaxY[i] = 0.0;
    }

    for (int i = 0; i < 20; i++) {
        mStageOriginX[i] = 0.0;
        mStageOriginZ[i] = 0.0;
        mStageOriginPathX[i] = 0.0;
        mStageOriginPathZ[i] = 0.0;
    }

    mpStages = NULL;
    mTexMinX = 0.0;
    mTexMinZ = 0.0;
    mTexMaxX = 0.0;
    mTexMaxZ = 0.0;
    mAllMapScaleRate = 0.0;
    mWorldMinX = 0.0;
    mWorldMinZ = 0.0;
    mWorldMaxX = 0.0;
    mWorldMaxZ = 0.0;
    field_0x1184 = 0.0;
    field_0x1188 = 0.0;
    mRenderingPosX = 0.0;
    mRenderingPosZ = 0.0;
    mRenderingScale = 1.0;
    field_0xf98 = 1.0;
    mMapZoomRate = 0.0;
    mZoom = getAllMapZoomRate();
    field_0xfa4 = 1.0;
    field_0xfa8 = 0.0;
    mpBackTex = NULL;
    mpSpotTexture = NULL;
    mTransX = 0.0;
    mTransZ = 0.0;
    mAlphaRate = 0.0;
    mpMenuFmapMap = NULL;
    mpMeterHaihai = NULL;
    field_0x122d = 0;
    field_0x122e = false;
    mSpotNum = 0;
    mStageDataNum = 0;
    mStageTransX = 0.0;
    mStageTransZ = 0.0;
    field_0xfd0 = 0.0;
    field_0xfd4 = 0.0;
    field_0xfd8 = 0.0;
    field_0xfdc = 0.0;
    control_xpos = 0.0;
    control_ypos = 0.0;
    field_0x11b4 = 0.0;
    field_0x11b8 = 0.0;
    field_0x11bc = 0.0;
    field_0x11c0 = 0.0;
    field_0x11c4 = 0.0;
    field_0x11c8 = 0.0;
    field_0x11cc = 0.0;
    field_0x11d0 = 0.0;
    mRegionFlag = 0;
    mMapDrawFlag = false;
    field_0x1238 = 0;
    field_0x1218 = 0;
    field_0x121a = 0;
    mSelectRegion = 0xff;
    mRegionCursor = 0;
    field_0x1228 = 0;
    mSpotCursor = 0;
    field_0x122a = 0;
    field_0x122b = 1;
    field_0xfb0 = 0.0;
    field_0xfb4 = 0.0;
    field_0xfb8 = 1.0;
    mArrowPos3DX = mArrowPos3DZ = mArrowAlpha = 0.0;
    field_0x11a4 = field_0x11a8 = 0.0;
    field_0x11ac = field_0x11b0 = 0.0;
    mArrowPos2DX = mArrowPos2DY = 0.0;
    mSpotTextureFadeAlpha = 0.0;
    field_0x11dc = 0.0;
    field_0x11e0 = 360.0;
    mBackAlpha = 0.0;
    field_0x11e8 = 0.0;
    field_0x11ec = 0.0;

    mSelectedRegionBlack.set(0, 0, 0, 0);
    mSelectedRegionWhite.set(255, 255, 255, 255);
    mUnselectedRegionBlack.set(0, 0, 0, 0);
    mUnselectedRegionWhite.set(255, 255, 255, 255);
    mUnreachedRegionBlack.set(0, 0, 0, 0);
    mUnreachedRegionWhite.set(0, 0, 0, 255);
    field_0x1208.set(0, 0, 0, 0);
    field_0x120c.set(0, 0, 0, 255);

    field_0x1210 = 1.0;
    field_0x1214 = 1.0;
    mArrowDrawFlag = true;
    
#if DEBUG
    mAllPathShowFlag = g_fmapHIO.mDisplayAllPaths;
#else
    mAllPathShowFlag = false;
#endif

    initiate(dComIfGp_getFmapResArchive());

    mpBackScreen = new J2DScreen();
    JUT_ASSERT(238, mpBackScreen != NULL);

    bool fg = mpBackScreen->setPriority("zelda_map_screen_stone_base.blo", 0x20000,
                                        dComIfGp_getFmapResArchive());
    JUT_ASSERT(242, fg != false);

    dPaneClass_showNullPane(mpBackScreen);

    mpBaseScreen = new J2DScreen();
    JUT_ASSERT(248, mpBaseScreen != NULL)

    fg = mpBaseScreen->setPriority("zelda_map_screen_base.blo", 0x20000,
                                   dComIfGp_getFmapResArchive());
    JUT_ASSERT(252, fg != false);

    dPaneClass_showNullPane(mpBaseScreen);

    void* r = JKRGetNameResource("zelda_map_screen_base.btk", dComIfGp_getFmapResArchive());
    mpBaseAnm = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(r);
    mpBaseAnm->searchUpdateMaterialID(mpBaseScreen);

    mAnmFrame = 0.0;

    mpBaseRoot = new CPaneMgrAlphaMorf(mpBaseScreen, 'ROOT', 2, NULL);
    JUT_ASSERT(264, mpBaseRoot != NULL);

    mpMapArea = new CPaneMgr(mpBaseScreen, 'center_n', 0, NULL);
    JUT_ASSERT(269, mpMapArea != NULL);

    mpMapBlack = new CPaneMgr(mpBaseScreen, 'map_blak', 0, NULL);
    JUT_ASSERT(273, mpMapBlack != NULL);

    mpMeterHaihai = new dMeterHaihai_c(1);
    JUT_ASSERT(277, mpMeterHaihai != NULL);

    ResTIMG* backTexResource =
        (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
    mpBackTex = new J2DPicture(backTexResource);
    JUT_ASSERT(282, mpBackTex != NULL);

    mpBackTex->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
    mpBackTex->setAlpha(0);

    mpPointScreen = new J2DScreen();
    JUT_ASSERT(293, mpPointScreen != NULL);

    mpPointScreen->setPriority("zelda_field_map_point_cursor.blo", 0x20000,
                                    dComIfGp_getFmapResArchive());
    JUT_ASSERT(297, fg != false);

    dPaneClass_showNullPane(mpPointScreen);

    mpPointParent = new CPaneMgr(mpPointScreen, 'f_po_n', 2, NULL);
    JUT_ASSERT(301, mpPointParent != NULL);

    mpPointParent->scale(g_fmapHIO.mCursorScale, g_fmapHIO.mCursorScale);

    static const u64 cur_tag[4] = {'f_po_ru', 'f_po_rd', 'f_po_ld', 'f_po_lu'};

    for (int i = 0; i < 4; i++) {
        J2DPicture* picture = static_cast<J2DPicture*>(mpPointScreen->search(cur_tag[i]));
        picture->setBlackWhite(g_fmapHIO.mCursorBlack, g_fmapHIO.mCursorWhite);
    }

    dMeter2Info_set2DWidth(getMapScissorAreaSizeX());
    dMeter2Info_set2DHeight(getMapScissorAreaSizeY());
    dMeter2Info_set2DPosH(getMapScissorAreaLX());
    dMeter2Info_set2DPosV(getMapScissorAreaLY());
    setDebugData(NULL, NULL, 0);
}

dMenu_Fmap2DBack_c::~dMenu_Fmap2DBack_c() {
    delete mpBackScreen;
    mpBackScreen = NULL;

    delete mpBaseScreen;
    mpBaseScreen = NULL;

    delete mpBaseAnm;
    mpBaseAnm = NULL;

    delete mpBaseRoot;
    mpBaseRoot = NULL;

    delete mpMapArea;
    mpMapArea = NULL;

    delete mpMapBlack;
    mpMapBlack = NULL;

    delete mpMeterHaihai;
    mpMeterHaihai = NULL;

    delete mpBackTex;
    mpBackTex = NULL;

    delete mpPointScreen;
    mpPointScreen = NULL;

    delete mpPointParent;
    mpPointParent = NULL;

    for (int i = 0; i < 8; i++) {
        if (mpAreaTex[i] != NULL) {
            delete mpAreaTex[i];
            mpAreaTex[i] = NULL;
        }
    }

    deleteSpotPane();
    dMeter2Info_setMapDrugFlag(0);
}

void dMenu_Fmap2DBack_c::draw() {
    calcBlink();

    J2DGrafContext* grafPort = dComIfGp_getCurrentGrafPort();

    grafPort->setup2D();

    u32 scissorLeft, scissorTop, scissorWidth, scissorHeight;
    GXGetScissor(&scissorLeft, &scissorTop, &scissorWidth, &scissorHeight);

    grafPort->scissor(mTransX, 0.0f, FB_WIDTH, FB_HEIGHT);
    grafPort->setScissor();

    mpBackTex->setBlackWhite(field_0x1208, field_0x120c);
    mpBackTex->setAlpha(mAlphaRate * 255.0f * g_fmapHIO.mBackgroundAlpha);
    mpBackTex->draw(mTransX + mDoGph_gInf_c::getMinXF(),
                    mTransZ + mDoGph_gInf_c::getMinYF(), mDoGph_gInf_c::getWidthF(),
                    mDoGph_gInf_c::getHeightF(), false, false, false);

    mpBackScreen->draw(mTransX, mTransZ, grafPort);
    mpBaseScreen->draw(mTransX, mTransZ, grafPort);

    Mtx mtx;
    Vec vec1 = mpMapArea->getGlobalVtx(&mtx, 0, false, 0);

    Vec vec2 = mpMapArea->getGlobalVtx(&mtx, 3, false, 0);

    f32 width = mDoGph_gInf_c::getWidthF() / FB_WIDTH;
    f32 height = mDoGph_gInf_c::getHeightF() / FB_HEIGHT;

    grafPort->scissor(mTransX + ((vec1.x - mDoGph_gInf_c::getMinXF()) / width),
                      mTransZ + (vec1.y / height), (vec2.x - vec1.x) / width,
                      (vec2.y - vec1.y) / height);

    grafPort->setScissor();

    field_0x11dc = (mTransX + getMapScissorAreaLX()) + (getMapScissorAreaSizeRealX() * 0.5f);

    if (g_fmapHIO.mRangeCheck && g_fmapHIO.mRangeCheckDrawPriority) {
        drawDebugRegionArea();
    }

    regionTextureDraw();

    if (g_fmapHIO.mRangeCheck && g_fmapHIO.mRangeCheckDrawPriority) {
        drawDebugStageArea();
    }

    if (mMapDrawFlag && mpSpotTexture) {
        if (g_fmapHIO.mDisplayRegionGrid && g_fmapHIO.mRegionGridWidth > 0.0f) {
            regionGridDraw();
        }

        stageTextureDraw();

        if (g_fmapHIO.mDisplayStageOrigin) {
            stageOriginDraw();
        }
    }

    if (g_fmapHIO.mDisplayRegionOrigin) {
        regionOriginDraw();
    }

    if (g_fmapHIO.mDisplayWorldGrid && g_fmapHIO.mWorldGridWidth > 0.0f) {
        worldGridDraw();
    }

    if (g_fmapHIO.mDisplayWorldOrigin) {
        worldOriginDraw();
    }

    if (g_fmapHIO.mScrollRangeDisplay) {
        scrollAreaDraw();
    }

    blinkMove(30);
    moveLightDropAnime();
    setCenterPosX(field_0x11dc, 1);
    drawIcon(mTransX, mTransZ, mAlphaRate, field_0xfa8 * mSpotTextureFadeAlpha);

    if (g_fmapHIO.mRangeCheck && !g_fmapHIO.mRangeCheckDrawPriority) {
        drawDebugRegionArea();
    }

    grafPort->scissor(scissorLeft, scissorTop, scissorWidth, scissorHeight);
    grafPort->setScissor();

    if (isArrowDrawFlag()) {
        f32 fVar2 = 0.0f;
        f32 fVar3 = 0.0f;
        f32 fVar4, fVar5;

        if (mMapZoomRate > 1.0f && mMapZoomRate < 2.0f) {
            f32 tmp = 2.0f - mMapZoomRate;
            if (field_0x11a4 != 0.0f) {
                fVar2 = field_0x11a4 * tmp;
            }
            if (field_0x11a8 != 0.0f) {
                fVar3 = field_0x11a8 * tmp;
            }
            fVar4 = field_0x11ac;
            fVar5 = field_0x11b0;
        } else {
            fVar4 = mStageTransX;
            fVar5 = mStageTransZ;
        }

        calcAllMapPos2D((mArrowPos3DX + control_xpos + fVar2) - fVar4,
                        (mArrowPos3DZ + control_ypos + fVar3) - fVar5, &mArrowPos2DX,
                        &mArrowPos2DY);

        field_0x11e0 -= g_fmapHIO.mCursorSpeed;

        if (field_0x11e0 < 0.0f) {
            field_0x11e0 += 360.0f;
        }

        mpPointParent->getPanePtr()->rotate(mpPointParent->getSizeX() / 2.0f,
                                            mpPointParent->getSizeY() / 2.0f, ROTATE_Z,
                                            field_0x11e0);

        if (g_fmapHIO.mCursorDebugON) {
            mpPointParent->scale(g_fmapHIO.mCursorScale, g_fmapHIO.mCursorScale);

            static const u64 cur_tag[4] = {'f_po_ru', 'f_po_rd', 'f_po_ld', 'f_po_lu'};

            for (int i = 0; i < 4; i++) {
                J2DPicture* picture = static_cast<J2DPicture*>(mpPointScreen->search(cur_tag[i]));
                picture->setBlackWhite(g_fmapHIO.mCursorBlack, g_fmapHIO.mCursorWhite);
            }
        }

        mpPointParent->setAlphaRate(mArrowAlpha * mSpotTextureFadeAlpha);
        mpPointParent->translate(mArrowPos2DX + mTransX, mArrowPos2DY + mTransZ);
        mpPointScreen->draw(0.0f, 0.0f, grafPort);
    }

    if (g_fmapHIO.mDisplayReferenceArea) {
        J2DDrawFrame(mTransX + getMapAreaGlobalPosX(), mTransZ + getMapAreaGlobalPosY(),
                     getMapAreaSizeX(), getMapAreaSizeY(), JUtility::TColor(255, 255, 255, 255), 6);
        J2DFillBox(getMapAreaGlobalCenterPosX() - 3.0f, getMapAreaGlobalCenterPosY() - 3.0f, 6.0f,
                   6.0f, JUtility::TColor(255, 255, 255, 255));
    }

    if (field_0x122d) {
        mpMeterHaihai->drawHaihai(field_0x122d);
        field_0x122d = 0;
    }

    if (g_fmapHIO.mRangeCheck && !g_fmapHIO.mRangeCheckDrawPriority) {
        drawDebugStageArea();
    }

    mpBackTex->setBlackWhite(field_0x1208, field_0x120c);
    mpBackTex->setAlpha((mAlphaRate * 255.0f) * g_fmapHIO.mBackgroundFrontAlpha * mBackAlpha);
    mpBackTex->draw(mTransX + mDoGph_gInf_c::getMinXF(),
                    mTransZ + mDoGph_gInf_c::getMinYF(), mDoGph_gInf_c::getWidthF(),
                    mDoGph_gInf_c::getHeightF(), false, false, false);
}

void dMenu_Fmap2DBack_c::setRegionTexData(u8 i_areaType, ResTIMG* i_timg, f32 i_originX,
                                          f32 i_originZ, f32 i_posX, f32 i_posZ, f32 i_scale,
                                          f32 i_scrollMinX, f32 i_scrollMinZ, f32 i_scrollMaxX,
                                          f32 i_scrollMaxZ) {
    if (i_scale == 0.0f) {
        i_scale = 100.0f;
    }

    mpAreaTex[i_areaType] = new J2DPicture(i_timg);
    JUT_ASSERT(763, mpAreaTex[i_areaType] != NULL);

    mRegionOriginX[i_areaType] = i_originX;
    mRegionOriginZ[i_areaType] = i_originZ;
    mRegionTexData[i_areaType].mScale = i_scale * 100.0f;

    f32 width = i_timg->width;
    f32 height = i_timg->height;

    f32 min_x = (mRegionOriginX[i_areaType] + i_posX) -
                ((width * 0.5f) * mRegionTexData[i_areaType].mScale);
    f32 min_z = (mRegionOriginZ[i_areaType] + i_posZ) -
                ((height * 0.5f) * mRegionTexData[i_areaType].mScale);
    f32 max_x = (mRegionOriginX[i_areaType] + i_posX) +
                ((width * 0.5f) * mRegionTexData[i_areaType].mScale);
    f32 max_z = (mRegionOriginZ[i_areaType] + i_posZ) +
                ((height * 0.5f) * mRegionTexData[i_areaType].mScale);

    mRegionTexData[i_areaType].mMinX = min_x;
    mRegionTexData[i_areaType].mMinZ = min_z;
    mRegionTexData[i_areaType].mMaxX = max_x;
    mRegionTexData[i_areaType].mMaxZ = max_z;

    mRegionScrollMinX[i_areaType] = i_scrollMinX;
    mRegionScrollMinZ[i_areaType] = i_scrollMinZ;
    mRegionScrollMaxX[i_areaType] = i_scrollMaxX;
    mRegionScrollMaxZ[i_areaType] = i_scrollMaxZ;

    bool first = false;

    mTexMinX = 0.0f;
    mTexMinZ = 0.0f;
    mTexMaxX = 0.0f;
    mTexMaxZ = 0.0f;

    bool bVar14 = false;
    for (int i = 0; i < 8; i++) {
        if (mpAreaTex[i] != NULL) {
            if (!first) {
                mTexMinX = mRegionTexData[i].mMinX;
                mTexMinZ = mRegionTexData[i].mMinZ;
                mTexMaxX = mRegionTexData[i].mMaxX;
                mTexMaxZ = mRegionTexData[i].mMaxZ;
                first = true;
            } else {
                if (mTexMinX > mRegionTexData[i].mMinX) {
                    mTexMinX = mRegionTexData[i].mMinX;
                }
                if (mTexMinZ > mRegionTexData[i].mMinZ) {
                    mTexMinZ = mRegionTexData[i].mMinZ;
                }
                if (mTexMaxX < mRegionTexData[i].mMaxX) {
                    mTexMaxX = mRegionTexData[i].mMaxX;
                }
                if (mTexMaxZ < mRegionTexData[i].mMaxZ) {
                    mTexMaxZ = mRegionTexData[i].mMaxZ;
                }
            }
        }
        if ((!bVar14) && (field_0x1230[i] == 0xff)) {
            field_0x1230[i] = i_areaType;
            bVar14 = true;
        }
    }
}

void dMenu_Fmap2DBack_c::setRegionTexData(u8 i_areaType, f32 i_originX, f32 i_originZ, f32 i_posX,
                                          f32 i_posZ, f32 i_scale, f32 i_scrollMinX,
                                          f32 i_scrollMinZ, f32 i_scrollMaxX, f32 i_scrollMaxZ) {
    if (i_scale == 0.0f) {
        i_scale = 100.0f;
    }

    mRegionOriginX[i_areaType] = i_originX;
    mRegionOriginZ[i_areaType] = i_originZ;
    mRegionTexData[i_areaType].mScale = i_scale * 100.0f;

    f32 width = mpAreaTex[i_areaType]->getTexture(0)->getTexInfo()->width;
    f32 height = mpAreaTex[i_areaType]->getTexture(0)->getTexInfo()->height;

    f32 min_x = (mRegionOriginX[i_areaType] + i_posX) -
                ((width * 0.5f) * mRegionTexData[i_areaType].mScale);
    f32 min_z = (mRegionOriginZ[i_areaType] + i_posZ) -
                ((height * 0.5f) * mRegionTexData[i_areaType].mScale);
    f32 max_x = (mRegionOriginX[i_areaType] + i_posX) +
                ((width * 0.5f) * mRegionTexData[i_areaType].mScale);
    f32 max_z = (mRegionOriginZ[i_areaType] + i_posZ) +
                ((height * 0.5f) * mRegionTexData[i_areaType].mScale);

    mRegionTexData[i_areaType].mMinX = min_x;
    mRegionTexData[i_areaType].mMinZ = min_z;
    mRegionTexData[i_areaType].mMaxX = max_x;
    mRegionTexData[i_areaType].mMaxZ = max_z;

    mRegionScrollMinX[i_areaType] = i_scrollMinX;
    mRegionScrollMinZ[i_areaType] = i_scrollMinZ;
    mRegionScrollMaxX[i_areaType] = i_scrollMaxX;
    mRegionScrollMaxZ[i_areaType] = i_scrollMaxZ;

    bool first = false;

    mTexMinX = 0.0f;
    mTexMinZ = 0.0f;
    mTexMaxX = 0.0f;
    mTexMaxZ = 0.0f;

    for (int i = 0; i < 8; i++) {
        if (mpAreaTex[i] != NULL) {
            if (!first) {
                mTexMinX = mRegionTexData[i].mMinX;
                mTexMinZ = mRegionTexData[i].mMinZ;
                mTexMaxX = mRegionTexData[i].mMaxX;
                mTexMaxZ = mRegionTexData[i].mMaxZ;
                first = true;
            } else {
                if (mTexMinX > mRegionTexData[i].mMinX) {
                    mTexMinX = mRegionTexData[i].mMinX;
                }
                if (mTexMinZ > mRegionTexData[i].mMinZ) {
                    mTexMinZ = mRegionTexData[i].mMinZ;
                }
                if (mTexMaxX < mRegionTexData[i].mMaxX) {
                    mTexMaxX = mRegionTexData[i].mMaxX;
                }
                if (mTexMaxZ < mRegionTexData[i].mMaxZ) {
                    mTexMaxZ = mRegionTexData[i].mMaxZ;
                }
            }
        }
    }
}

void dMenu_Fmap2DBack_c::calcAllMapPos() {
    bool set = false;
    calcAllMapScaleRate();
    for (int i = 0; i < 8; i++) {
        if (mpAreaTex[i] != NULL) {
            f32 min_x, min_y, max_x, max_y;

            calcAllMapPos2DFirst(mRegionTexData[i].mMinX, mRegionTexData[i].mMinZ, &min_x, &min_y);
            calcAllMapPos2DFirst(mRegionTexData[i].mMaxX, mRegionTexData[i].mMaxZ, &max_x, &max_y);
            mRegionMinMapX[i] = min_x;
            mRegionMinMapY[i] = min_y;
            mRegionMapSizeX[i] = max_x - min_x;
            mRegionMapSizeY[i] = max_y - min_y;

            calcAllMapPos2DFirst(mRegionScrollMinX[i], mRegionScrollMinZ[i],
                                 &mRegionScrollMapMinX[i], &mRegionScrollMapMinY[i]);
            calcAllMapPos2DFirst(mRegionScrollMaxX[i], mRegionScrollMaxZ[i],
                                 &mRegionScrollMapMaxX[i], &mRegionScrollMapMaxY[i]);

            onShowRegionFlag(i);

            if (!set) {
                set = true;
                calcRenderingScale();
                calcRenderingPos();
            }
        }
    }
}

f32 dMenu_Fmap2DBack_c::calcAllMapScaleRate() {
    f32 width = mTexMaxX - mTexMinX;
    f32 height = mTexMaxZ - mTexMinZ;
    f32 size = width;
    if (size < height) {
        size = height;
    }
    f32 v = getMapAreaSizeX();
    mAllMapScaleRate = v / size;

    return mAllMapScaleRate;
}

void dMenu_Fmap2DBack_c::calcAllMapPos2DFirst(f32 i_texPosX, f32 i_texPosZ, f32* o_posX,
                                              f32* o_posY) {
    f32 midTexX = (mTexMaxX + mTexMinX) * 0.5f;
    f32 midTexZ = (mTexMaxZ + mTexMinZ) * 0.5f;
    f32 relativePosX = i_texPosX - midTexX;
    f32 relativePosZ = i_texPosZ - midTexZ;
    f32 pos_x = (mAllMapScaleRate * relativePosX);
    f32 pos_y = (mAllMapScaleRate * relativePosZ);

    *o_posX = pos_x + getMapAreaGlobalCenterPosX();
    *o_posY = pos_y + getMapAreaGlobalCenterPosY();
}

void dMenu_Fmap2DBack_c::calcAllMapPos2D(f32 param_0, f32 param_1, f32* o_posX, f32* o_posY) {
    calcAllMapPos2D(param_0, param_1, o_posX, o_posY, field_0xf0c[mRegionCursor],
                    field_0xf2c[mRegionCursor], mZoom);
}

void dMenu_Fmap2DBack_c::calcAllMapPos2D(char const* param_0, f32 param_1, f32 param_2,
                                         f32* o_posX, f32* o_posY) {
    f32 tmp1, tmp2;
    calcOffset(mRegionCursor, param_0, &tmp1, &tmp2);
    calcAllMapPos2D(param_1 + tmp1, param_2 + tmp2, o_posX, o_posY, field_0xf0c[mRegionCursor],
                    field_0xf2c[mRegionCursor], mZoom);
}

void dMenu_Fmap2DBack_c::calcAllMapPos2D(f32 param_0, f32 param_1, f32* outX, f32* outZ,
                                         f32 param_4, f32 param_5, f32 i_zoom) {
    f32 unusedX = (mTexMaxX - mTexMinX) / getMapAreaSizeX();
    f32 unusedY = (mTexMaxZ - mTexMinZ) / getMapAreaSizeY();
    f32 unusedMax = unusedX;
    if (unusedX < unusedY) {
        unusedMax = unusedY;
    }
    f32 regionMidX = (mRegionTexData[mRegionCursor].mMinX + mRegionTexData[mRegionCursor].mMaxX) * 0.5f;
    f32 regionMidY = (mRegionTexData[mRegionCursor].mMinZ + mRegionTexData[mRegionCursor].mMaxZ) * 0.5f;
    f32 f25 = mRegionMinMapX[mRegionCursor] + param_4 + (mRegionMapSizeX[mRegionCursor] * i_zoom) * 0.5f;
    f32 f24 = mRegionMinMapY[mRegionCursor] + param_5 + (mRegionMapSizeY[mRegionCursor] * i_zoom) * 0.5f;

    f32 scale = getPathScale(i_zoom) / getMapScissorAreaSizeRealY();
    f32 offsX = (param_0 - regionMidX) / scale;
    f32 offsY = (param_1 - regionMidY) / scale;
    *outX = offsX + f25;
    *outZ = offsY + f24;
}

void dMenu_Fmap2DBack_c::calcAllMapPosWorld(f32 param_0, f32 param_1, f32* outX, f32* outZ) {
    f32 midX = (mRegionTexData[mRegionCursor].mMinX + mRegionTexData[mRegionCursor].mMaxX) * 0.5f;
    f32 midY = (mRegionTexData[mRegionCursor].mMinZ + mRegionTexData[mRegionCursor].mMaxZ) * 0.5f;
    f32 tmp3 = (mRegionMinMapX[mRegionCursor] + field_0xf0c[mRegionCursor]) +
               mRegionMapSizeX[mRegionCursor] * mZoom * 0.5f;
    f32 tmp4 = (mRegionMinMapY[mRegionCursor] + field_0xf2c[mRegionCursor]) +
               mRegionMapSizeY[mRegionCursor] * mZoom * 0.5f;
    f32 tmp3_ = param_0 - tmp3;
    f32 tmp4_ = param_1 - tmp4;
    f32 tmp5 = field_0xf98 / getMapScissorAreaSizeRealY();

    *outX = midX + (tmp3_ * tmp5);
    *outZ = midY + (tmp4_ * tmp5);
}

void dMenu_Fmap2DBack_c::setSpotPane(ResTIMG* i_timg) {
    if (mpSpotTexture == NULL) {
        mpSpotTexture = new J2DPicture(i_timg);
        JUT_ASSERT(1169, mpSpotTexture != NULL);
    }
}

void dMenu_Fmap2DBack_c::deleteSpotPane() {
    if (mpSpotTexture != NULL) {
        delete mpSpotTexture;
        mpSpotTexture = NULL;
    }
}

void dMenu_Fmap2DBack_c::zoomMapCalc(f32 i_zoom) {
    mMapZoomRate = i_zoom;
    field_0xfa8 = i_zoom;
    mZoom = getAllMapZoomRate() +
                  (i_zoom * (getRegionMapZoomRate(mRegionCursor) - getAllMapZoomRate()));
    calcRenderingScale();

    f32 centerX = getMapAreaGlobalCenterPosX();
    f32 centerY = getMapAreaGlobalCenterPosY();
    if (g_fmapHIO.field_0x308 == false) {
        f32 region_centerX = mRegionMinMapX[mRegionCursor] + (mRegionMapSizeX[mRegionCursor] * 0.5f);
        f32 region_centerY = mRegionMinMapY[mRegionCursor] + (mRegionMapSizeY[mRegionCursor] * 0.5f);
        f32 v = region_centerX + (i_zoom * (centerX - region_centerX));
        f32 v2 = region_centerY + (i_zoom * (centerY - region_centerY));
        field_0xf0c[mRegionCursor] =
            v - (mRegionMapSizeX[mRegionCursor] * mZoom * 0.5f) - mRegionMinMapX[mRegionCursor];
        field_0xf2c[mRegionCursor] =
            v2 - (mRegionMapSizeY[mRegionCursor] * mZoom * 0.5f) - mRegionMinMapY[mRegionCursor];
    } else {
        f32 dVar12 = (mRegionScrollMapMinX[mRegionCursor] + mRegionScrollMapMaxX[mRegionCursor]) * 0.5f;
        f32 dVar11 = (mRegionScrollMapMinY[mRegionCursor] + mRegionScrollMapMaxY[mRegionCursor]) * 0.5f;

        f32 tmp3 = (mRegionMinMapX[mRegionCursor] + mRegionMapSizeX[mRegionCursor] * 0.5f) - dVar12;
        f32 tmp = (mRegionMinMapY[mRegionCursor] + mRegionMapSizeY[mRegionCursor] * 0.5f) - dVar11;

        f32 tmp2 = (dVar12 + (i_zoom * (centerX - dVar12)));
        f32 tmp2_ = (dVar11 + (i_zoom * (centerY - dVar11)));
        
        field_0xf0c[mRegionCursor] =
            ((tmp2 + (tmp3 * mZoom)) - mRegionMapSizeX[mRegionCursor] * mZoom * 0.5f) -
            mRegionMinMapX[mRegionCursor];

        field_0xf2c[mRegionCursor] =
            ((tmp2_ + tmp * mZoom) - mRegionMapSizeY[mRegionCursor] * mZoom * 0.5f) -
            mRegionMinMapY[mRegionCursor];
    }

    for (int i = 0; i < 8; i++) {
        if (i != mRegionCursor) {
            field_0xf0c[i] =
                field_0xf0c[mRegionCursor] +
                (mRegionMinMapX[i] - mRegionMinMapX[mRegionCursor]) * (mZoom - getAllMapZoomRate());
            field_0xf2c[i] =
                field_0xf2c[mRegionCursor] +
                (mRegionMinMapY[i] - mRegionMinMapY[mRegionCursor]) * (mZoom - getAllMapZoomRate());
        }
    }

    if (i_zoom == 1.0f) {
        for (int i = 0; i < 8; i++) {
            field_0xf4c[i] = field_0xf0c[i];
            field_0xf6c[i] = field_0xf2c[i];
        }
    }

    calcRenderingPos();
    setSpotCursor(mSpotCursor);

    field_0xfb8 =
        ((mRegionMapSizeX[mRegionCursor] + (i_zoom * (getMapAreaSizeX() - mRegionMapSizeX[mRegionCursor]))) /
         getMapAreaSizeX());
}

void dMenu_Fmap2DBack_c::zoomMapCalc2(f32 param_0) {
    f32 spotZoomRate = getSpotMapZoomRate();
    f32 regionZoomRate = getRegionMapZoomRate(mRegionCursor);
    mZoom = regionZoomRate + (param_0 * (spotZoomRate - regionZoomRate));

    if (mArrowPos3DX + control_xpos < mRegionScrollMinX[mRegionCursor]) {
        if (field_0x11a4 == 0.0f) {
            field_0x11a4 = (mArrowPos3DX + control_xpos) - mRegionScrollMinX[mRegionCursor];
        }
        control_xpos = mRegionScrollMinX[mRegionCursor] - mArrowPos3DX;
    } else {
        if (mArrowPos3DX + control_xpos > mRegionScrollMaxX[mRegionCursor]) {
            if (field_0x11a4 == 0.0f) {
                field_0x11a4 = (mArrowPos3DX + control_xpos) - mRegionScrollMaxX[mRegionCursor];
            }
            control_xpos = mRegionScrollMaxX[mRegionCursor] - mArrowPos3DX;
        }
    }

    if (mArrowPos3DZ + control_ypos < mRegionScrollMinZ[mRegionCursor]) {
        if (field_0x11a8 == 0.0f) {
            field_0x11a8 = (mArrowPos3DZ + control_ypos) - mRegionScrollMinZ[mRegionCursor];
        }
        control_ypos = mRegionScrollMinZ[mRegionCursor] - this->mArrowPos3DZ;
    } else {
        if (mArrowPos3DZ + control_ypos > mRegionScrollMaxZ[mRegionCursor]) {
            if (field_0x11a8 == 0.0f) {
                field_0x11a8 = (mArrowPos3DZ + control_ypos) - mRegionScrollMaxZ[mRegionCursor];
            }
            control_ypos = mRegionScrollMaxZ[mRegionCursor] - mArrowPos3DZ;
        }
    }

    calcRenderingScale();

    mMapZoomRate = param_0 + 1.0f;
    f32 centerX = getMapAreaGlobalCenterPosX();
    f32 centerY = getMapAreaGlobalCenterPosY();
    if (g_fmapHIO.field_0x308 == false) {
        field_0xf0c[mRegionCursor] = (centerX - (mRegionMapSizeX[mRegionCursor] * mZoom * 0.5f)) -
                                    mRegionMinMapX[mRegionCursor];
        field_0xf2c[mRegionCursor] = (centerY - (mRegionMapSizeY[mRegionCursor] * mZoom * 0.5f)) -
                                    mRegionMinMapY[mRegionCursor];
    } else {
        f32 x = (mRegionScrollMapMinX[mRegionCursor] + mRegionScrollMapMaxX[mRegionCursor]) * 0.5f;
        f32 y = (mRegionScrollMapMinY[mRegionCursor] + mRegionScrollMapMaxY[mRegionCursor]) * 0.5f;

        f32 tmp2 = (mRegionMinMapX[mRegionCursor] + (mRegionMapSizeX[mRegionCursor] * 0.5f)) - x;
        f32 tmp1 = (mRegionMinMapY[mRegionCursor] + (mRegionMapSizeY[mRegionCursor] * 0.5f)) - y;

        field_0xf0c[mRegionCursor] =
            ((centerX + (tmp2 * mZoom)) - mRegionMapSizeX[mRegionCursor] * mZoom * 0.5f) -
            mRegionMinMapX[mRegionCursor];
        field_0xf2c[mRegionCursor] =
            ((centerY + (tmp1 * mZoom)) - mRegionMapSizeY[mRegionCursor] * mZoom * 0.5f) -
            mRegionMinMapY[mRegionCursor];
    }

    for (int i = 0; i < 8; i++) {
        if (i != mRegionCursor) {
            field_0xf0c[i] =
                field_0xf0c[mRegionCursor] +
                (mRegionMinMapX[i] - mRegionMinMapX[mRegionCursor]) * (mZoom - getAllMapZoomRate());
            field_0xf2c[i] =
                field_0xf2c[mRegionCursor] +
                (mRegionMinMapY[i] - mRegionMinMapY[mRegionCursor]) * (mZoom - getAllMapZoomRate());
        }
    }

    scrollCalc(param_0);

    if (param_0 == 1.0f) {
        field_0x11b4 = (mArrowPos3DX + control_xpos) - mStageTransX;
        field_0x11b8 = (mArrowPos3DZ + control_ypos) - mStageTransZ;
        field_0x11a4 = 0.0f;
        field_0x11a8 = 0.0f;
    }
}

void dMenu_Fmap2DBack_c::zoomMapCalcHIO() {
    if (mZoom != getSpotMapZoomRate()) {
        f32 spotZoomRate = getSpotMapZoomRate();
        f32 regionZoomRate = getRegionMapZoomRate(mRegionCursor);
        mZoom = getSpotMapZoomRate();
        calcRenderingScale();

        f32 centerX = getMapAreaGlobalCenterPosX();
        f32 centerY = getMapAreaGlobalCenterPosY();
        if (g_fmapHIO.field_0x308 == false) {
            field_0xf0c[mRegionCursor] =
                (centerX - (mRegionMapSizeX[mRegionCursor] * mZoom * 0.5f)) -
                mRegionMinMapX[mRegionCursor];
            field_0xf2c[mRegionCursor] =
                (centerY - (mRegionMapSizeY[mRegionCursor] * mZoom * 0.5f)) -
                mRegionMinMapY[mRegionCursor];
        } else {
            f32 x =
                (mRegionScrollMapMinX[mRegionCursor] + mRegionScrollMapMaxX[mRegionCursor]) * 0.5f;
            f32 y =
                (mRegionScrollMapMinY[mRegionCursor] + mRegionScrollMapMaxY[mRegionCursor]) * 0.5f;

            f32 tmp2 =
                ((mRegionMinMapX[mRegionCursor] + mRegionMapSizeX[mRegionCursor] * 0.5f) - x);
            f32 tmp1 =
                ((mRegionMinMapY[mRegionCursor] + mRegionMapSizeY[mRegionCursor] * 0.5f) - y);

            field_0xf0c[mRegionCursor] = ((centerX + (tmp2 * mZoom)) -
                                         mRegionMapSizeX[mRegionCursor] * mZoom * 0.5f) -
                                        mRegionMinMapX[mRegionCursor];
            field_0xf2c[mRegionCursor] = ((centerY + (tmp1 * mZoom)) -
                                         mRegionMapSizeY[mRegionCursor] * mZoom * 0.5f) -
                                        mRegionMinMapY[mRegionCursor];
        }

        for (int i = 0; i < 8; i++) {
            if (i != mRegionCursor) {
                field_0xf0c[i] =
                    field_0xf0c[mRegionCursor] + (mRegionMinMapX[i] - mRegionMinMapX[mRegionCursor]) *
                                                    (mZoom - getAllMapZoomRate());
                field_0xf2c[i] =
                    field_0xf2c[mRegionCursor] + (mRegionMinMapY[i] - mRegionMinMapY[mRegionCursor]) *
                                                    (mZoom - getAllMapZoomRate());
            }
        }

        scrollCalc(1.0f);
    }
}

void dMenu_Fmap2DBack_c::scrollCalc(f32 param_0) {
    f32 sp40, sp3C;
    calcAllMapPos2D(mArrowPos3DX + control_xpos, mArrowPos3DZ + control_ypos, &sp40, &sp3C,
                    field_0xf4c[mRegionCursor], field_0xf6c[mRegionCursor],
                    getRegionMapZoomRate(mRegionCursor));

    f32 sp38 = getMapAreaGlobalCenterPosX();
    f32 sp34 = getMapAreaGlobalCenterPosY();

    f32 sp30 = sp40 + (param_0 * (sp38 - sp40));
    f32 sp2C = sp3C + (param_0 * (sp34 - sp3C));
    f32 sp28, sp24;
    calcAllMapPosWorld(sp30, sp2C, &sp28, &sp24);

    mStageTransX = (mArrowPos3DX + control_xpos) - sp28;
    mStageTransZ = (mArrowPos3DZ + control_ypos) - sp24;

    field_0x11ac = mStageTransX;
    field_0x11b0 = mStageTransZ;

    if ((((field_0x11a4 != 0.0f) || (field_0x11a8 != 0.0f)) && (mMapZoomRate > 1.0f)) &&
        (mMapZoomRate < 2.0f))
    {
        f32 local_40 = 2.0f - mMapZoomRate;

        //!@bug local_44 or local_48 can be uninitialized if field_0x11a4 or field_0x11a8 is not 0
        f32 local_44, local_48;

        if (field_0x11a4 != 0.0f) {
            local_44 = field_0x11a4 * local_40;
        }

        if (field_0x11a8 != 0.0f) {
            local_48 = field_0x11a8 * local_40;
        }

        calcAllMapPos2D(mArrowPos3DX + control_xpos + local_44,
                        mArrowPos3DZ + control_ypos + local_48, &sp40, &sp3C,
                        field_0xf4c[mRegionCursor], field_0xf6c[mRegionCursor],
                        getRegionMapZoomRate(mRegionCursor));

        f32 sp14 = sp40 + (param_0 * (sp38 - sp40));
        f32 sp10 = sp3C + (param_0 * (sp34 - sp3C));
        f32 spC, sp8;
        calcAllMapPosWorld(sp14, sp10, &spC, &sp8);

        field_0x11ac = (mArrowPos3DX + control_xpos + local_44) - spC;
        field_0x11b0 = (mArrowPos3DZ + control_ypos + local_48) - sp8;
    }

    if (param_0 == 0.0f) {
        field_0xfd0 = field_0xfd4 = 0.0f;
        field_0xfd8 = field_0xfdc = 0.0f;
    } else if (param_0 == 1.0f) {
        field_0xfd0 = mStageTransX;
        field_0xfd4 = mStageTransZ;
    }

    calcRenderingPos();
}

void dMenu_Fmap2DBack_c::mapBlink(s16* param_0) {
    *param_0 += (s16)1;
    if (*param_0 >= 30) {
        *param_0 -= (s16)30;
    }
}

void dMenu_Fmap2DBack_c::setSpotCursor(u8 param_0) {
    this->mSpotCursor = param_0;

    f32 tmp1, tmp2;
    getStageOriginPath(param_0, &tmp1, &tmp2);
    calcAllMapPos2D(tmp1, tmp2, &field_0x1184, &field_0x1188);
}

void dMenu_Fmap2DBack_c::allmap_move2(STControl* param_0) {
    f32 sp48, sp44;
    calcAllMapPosWorld(getMapScissorAreaLX(), getMapScissorAreaLY(), &sp48, &sp44);

    f32 sp40, sp3C;
    calcAllMapPosWorld(getMapScissorAreaLX() + getMapScissorAreaSizeRealX(),
                       getMapScissorAreaLY() + getMapScissorAreaSizeRealY(), &sp40, &sp3C);
    int r26 = 0;
    if (r26 == 0) {
        f32 spC = g_fmapHIO.mScrollSpeedSlowBound < g_fmapHIO.mScrollSpeedFastBound ?
                               g_fmapHIO.mScrollSpeedSlowBound :
                               g_fmapHIO.mScrollSpeedFastBound;
        f32 sp8 = g_fmapHIO.mScrollSpeedSlowBound > g_fmapHIO.mScrollSpeedFastBound ?
                               g_fmapHIO.mScrollSpeedSlowBound :
                               g_fmapHIO.mScrollSpeedFastBound;

        f32 stickValue = param_0->getValueStick();
        if (stickValue >= spC) {
            s16 angle = param_0->getAngleStick();
            f32 local_68 = (mTexMaxX - mTexMinX);
            f32 zoomRate = local_68 / getAllMapZoomRate();
            f32 sp24;
            if (stickValue < sp8) {
                sp24 =  g_fmapHIO.mScrollSpeedWorldSlow;
            } else {
                sp24 = g_fmapHIO.mScrollSpeedWorldFast;
            }

            f32 speed = (sp24 / 100.0f) * zoomRate;
            f32 delta_y = speed * cM_ssin(angle);
            f32 delta_x = speed * cM_scos(angle);

            control_xpos = control_xpos + delta_y;
            control_ypos = control_ypos + delta_x;
        }
    }
    if (mArrowPos3DX + control_xpos < sp48) {
        control_xpos = sp48 - mArrowPos3DX;
    }

    if (mArrowPos3DX + control_xpos > sp40) {
        control_xpos = sp40 - mArrowPos3DX;
    }

    if (mArrowPos3DZ + control_ypos < sp44) {
        control_ypos = sp44 - mArrowPos3DZ;
    }

    if (mArrowPos3DZ + control_ypos > sp3C) {
        control_ypos = sp3C - mArrowPos3DZ;
    }

    calcRenderingScale();
    calcRenderingPos();
    calcAllMapPos();

    f32 sp14, sp10;
    calcAllMapPos2D((mArrowPos3DX + control_xpos) - mStageTransX,
                    (mArrowPos3DZ + control_ypos) - mStageTransZ, &sp14, &sp10);

    mSelectRegion = 0xff;
    for (int i = 7; i >= 0; i--) {
        int val = field_0x1230[i];
        if (val != 0xff && val != 7) {
            u8 pixel = dMeter2Info_getPixel(
                sp14, sp10, (mRegionMinMapX[val] + field_0xf0c[val] + mTransX),
                (mRegionMinMapY[val] + field_0xf2c[val] + mTransZ), (mRegionMapSizeX[val] * mZoom),
                (mRegionMapSizeY[val] * mZoom), mpAreaTex[val]->getTexture(0)->getTexInfo());
            if (pixel) {
                mSelectRegion = val;
            }
        }
        if (mSelectRegion != 0xff)
            break;
    }
}

void dMenu_Fmap2DBack_c::setIcon2DPos(u8 param_0, f32 param_1, f32 param_2, f32 param_3, u8 param_4,
                                      bool param_5) {
    f32 local_68, local_6c;

    f32 sp30 = mMapZoomRate;
    u8 uVar9 = 1;
    if (param_0 == 17) {
        uVar9 = 2;

        f32 dVar11 = 0.0f;
        f32 dVar10 = 0.0f;
        f32 sp2C, sp28;
        calcAllMapPosWorld(getMapScissorAreaCenterPosX(), getMapScissorAreaCenterPosY(), &sp2C,
                           &sp28);

        f32 sp24, sp20;
        calcAllMapPos2D(sp2C + mStageTransX, sp28 + mStageTransZ, &sp24, &sp20);

        dVar11 = getMapScissorAreaCenterPosX() - sp24;
        dVar10 = getMapScissorAreaCenterPosY() - sp20;

        if (mRegionCursor + 1 == dComIfGp_getNowLevel()) {
            calcAllMapPos2D(param_1 - mStageTransX, param_2 - mStageTransZ, &local_68, &local_6c);
            f32 tmp2 = (dVar11 + (mRegionMinMapX[mRegionCursor] + field_0xf0c[mRegionCursor])) +
                       ((mRegionMapSizeX[mRegionCursor] * mZoom) / 2.0f);
            f32 tmp1 = (dVar10 + (mRegionMinMapY[mRegionCursor] + field_0xf2c[mRegionCursor])) +
                       ((mRegionMapSizeY[mRegionCursor] * mZoom) / 2.0f);

            field_0x11e8 = local_68 - tmp2;
            field_0x11ec = local_6c - tmp1;
        } else {
            f32 tmp2 = (dVar11 + (mRegionMinMapX[dComIfGp_getNowLevel() - 1] +
                                  field_0xf0c[dComIfGp_getNowLevel() - 1])) +
                       ((mRegionMapSizeX[dComIfGp_getNowLevel() - 1] * mZoom) / 2.0f);
            f32 tmp1 = (dVar10 + (mRegionMinMapY[dComIfGp_getNowLevel() - 1] +
                                  field_0xf2c[dComIfGp_getNowLevel() - 1])) +
                       ((mRegionMapSizeY[dComIfGp_getNowLevel() - 1] * mZoom) / 2.0f);
            local_68 = tmp2 + (field_0x11e8 * mZoom);
            local_6c = tmp1 + (field_0x11ec * mZoom);
        }
    } else {
        calcAllMapPos2D(param_1 - mStageTransX, param_2 - mStageTransZ, &local_68, &local_6c);
    }

    if (!param_5) {
        if (param_0 == 1) {
            sp30 = (mMapZoomRate * 0.5f) + 0.5f;
            uVar9 = 2;
        }

        if (param_4 != 0) {
            uVar9 = 3;
        }
        setIconInfo(param_0, local_68, local_6c, 1.0f, param_3, sp30, uVar9);
    }
}

void dMenu_Fmap2DBack_c::setIcon2DPos(u8 param_0, char const* param_1, f32 param_2, f32 param_3,
                                      f32 param_4, u8 param_5, bool param_6) {
    setIcon2DPos(param_0, mRegionCursor, param_1, param_2, param_3, param_4, param_5, param_6);
}

void dMenu_Fmap2DBack_c::setIcon2DPos(u8 param_0, u8 param_1, char const* param_2, f32 param_3,
                                      f32 param_4, f32 param_5, u8 param_6, bool param_7) {
    f32 tmp1, tmp2;
    calcOffset(param_1, param_2, &tmp1, &tmp2);

    setIcon2DPos(param_0, param_3 + tmp1, param_4 + tmp2, param_5, param_6, param_7);
}

int dMenu_Fmap2DBack_c::isShowRegion(int param_0) {
    if (param_0 == 7) {
        return 1;
    }

    if (g_fmapHIO.mAllRegionsUnlocked) {
        if (isShowRegionFlag(param_0)) {
            return 1;
        }
    } else {
        if (g_fmapHIO.mDebugRegionBits[param_0] && isShowRegionFlag(param_0)) {
            return 1;
        }
        if (param_0 == 6) {
            if (isShowRegionFlag(3) && dComIfGs_isRegionBit(4)) {
                return 1;
            }
            if (dComIfGp_getNowLevel() == 4) {
                return 1;
            }
            return 0;
        }
        if (isShowRegionFlag(param_0) && dComIfGs_isRegionBit(param_0 + 1)) {
            return 1;
        }
        if (param_0 + 1 == dComIfGp_getNowLevel()) {
            return 1;
        }
    }

    return 0;
}

f32 dMenu_Fmap2DBack_c::getMapAreaGlobalPosX() {
    return g_fmapHIO.mMapTopLeftPosX;
}

f32 dMenu_Fmap2DBack_c::getMapAreaGlobalPosY() {
    return g_fmapHIO.mMapTopLeftPosY;
}

f32 dMenu_Fmap2DBack_c::getMapAreaGlobalCenterPosX() {
    return getMapAreaGlobalPosX() + (getMapAreaSizeX() / 2.0f);
}

f32 dMenu_Fmap2DBack_c::getMapAreaGlobalCenterPosY() {
    return getMapAreaGlobalPosY() + (getMapAreaSizeY() / 2.0f);
}

f32 dMenu_Fmap2DBack_c::getMapAreaSizeX() {
    return g_fmapHIO.mMapScale;
}

f32 dMenu_Fmap2DBack_c::getMapAreaSizeY() {
    return g_fmapHIO.mMapScale;
}

f32 dMenu_Fmap2DBack_c::getMapScissorAreaLX() {
    Mtx mtx;
    Vec vec = mpMapArea->getGlobalVtx(mpMapArea->getPanePtr(), &mtx, 0, false, 0);
    return vec.x;
}

f32 dMenu_Fmap2DBack_c::getMapScissorAreaLY() {
    Mtx mtx;
    Vec vec = mpMapArea->getGlobalVtx(mpMapArea->getPanePtr(), &mtx, 0, false, 0);
    return vec.y;
}

f32 dMenu_Fmap2DBack_c::getMapScissorAreaSizeX() {
    f32 scale = mpMapArea->getScaleX();
    J2DPane* pane;
    for (pane = mpMapArea->getPanePtr()->getParentPane(); pane != NULL;
         pane = pane->getParentPane())
    {
        scale *= pane->getScaleX();
    }

    return scale * mpMapArea->getSizeX();
}

f32 dMenu_Fmap2DBack_c::getMapScissorAreaSizeRealX() {
#if PLATFORM_GCN
    return getMapScissorAreaSizeX();
#else
    return getMapScissorAreaSizeX() * mDoGph_gInf_c::getScale();
#endif
}

f32 dMenu_Fmap2DBack_c::getMapScissorAreaCenterPosX() {
    return getMapScissorAreaLX() + (getMapScissorAreaSizeRealX() / 2.0f);
}

f32 dMenu_Fmap2DBack_c::getMapScissorAreaCenterPosY() {
    return getMapScissorAreaLY() + (getMapScissorAreaSizeRealY() / 2.0f);
}

f32 dMenu_Fmap2DBack_c::getMapScissorAreaSizeY() {
    f32 scale = mpMapArea->getScaleY();
    J2DPane* pane;
    for (pane = mpMapArea->getPanePtr()->getParentPane(); pane != NULL;
         pane = pane->getParentPane())
    {
        scale *= pane->getScaleY();
    }

    return scale * mpMapArea->getSizeY();
}

f32 dMenu_Fmap2DBack_c::getMapScissorAreaSizeRealY() {
    return getMapScissorAreaSizeY();
}

void dMenu_Fmap2DBack_c::calcRenderingPos() {
    calcAllMapPosWorld(getMapScissorAreaCenterPosX(), getMapScissorAreaCenterPosY(), &mRenderingPosX,
                       &mRenderingPosZ);

    mRenderingPosX += mStageTransX;
    mRenderingPosZ += mStageTransZ;
}

void dMenu_Fmap2DBack_c::calcRenderingScale() {
    f32 xScale = (mTexMaxX - mTexMinX) / getMapAreaSizeX();
    f32 yScale = (mTexMaxZ - mTexMinZ) / getMapAreaSizeY();

    f32 scale = xScale;
    if (xScale < yScale) {
        scale = yScale;
    }

    mRenderingScale = scale / mZoom;
    field_0xf98 = mRenderingScale * getMapScissorAreaSizeRealY();
}

f32 dMenu_Fmap2DBack_c::getPathScale(f32 i_zoom) {
    JUT_ASSERT(2204, i_zoom != 0.0f);

    f32 xScale = (mTexMaxX - mTexMinX) / getMapAreaSizeX();
    f32 yScale = (mTexMaxZ - mTexMinZ) / getMapAreaSizeY();

    f32 scale = xScale;
    if (xScale < yScale) {
        scale = yScale;
    }

    return (scale * getMapScissorAreaSizeRealY()) / i_zoom;
}

f32 dMenu_Fmap2DBack_c::getAllMapZoomRate() {
    return 1.0f;
}

f32 dMenu_Fmap2DBack_c::getRegionMapZoomRate(int i_regionNo) {
    f32 tmp1, tmp2;

    if (g_fmapHIO.field_0x308 == false) {
        tmp1 = getMapAreaSizeX() / mRegionMapSizeX[i_regionNo];
        tmp2 = getMapAreaSizeY() / mRegionMapSizeY[i_regionNo];
    } else {
        tmp1 = getMapAreaSizeX() /
            (mRegionScrollMapMaxX[mRegionCursor] - mRegionScrollMapMinX[mRegionCursor]);
        tmp2 = getMapAreaSizeY() /
            (mRegionScrollMapMaxY[mRegionCursor] - mRegionScrollMapMinY[mRegionCursor]);
    }

    if (tmp1 < tmp2) {
        field_0x122e = false;
        return tmp1;
    } else {
        field_0x122e = true;
        return tmp2;
    }
}

f32 dMenu_Fmap2DBack_c::getSpotMapZoomRate() {
    f32 tmp4;
    f32 tmp1 = mTexMaxX - mTexMinX;
    f32 tmp2 = mTexMaxZ - mTexMinZ;
    
    f32 tmp3 = tmp1;
    if (tmp3 < tmp2) {
        tmp3 = tmp2;
    }
    tmp4 = tmp3 / g_fmapHIO.mRegionZoomRange;
    return tmp4;
}

void dMenu_Fmap2DBack_c::getStageOriginPath(u8 param_0, f32* param_1, f32* param_2) {
    for (int i = 0; i < 20; i++) {
        if (i == param_0) {
            *param_1 = mStageOriginPathX[i];
            *param_2 = mStageOriginPathZ[i];
            break;
        }
    }
}

void dMenu_Fmap2DBack_c::calcOffset(u8 i_regionNo, char const* i_stageName,
                                    f32* o_offsetX, f32* o_offsetZ) {
    if (mpStages != NULL) {
        Stage_c::data* data = mpStages->mData;
        for (int j = 0, i = 0; i < mStageDataNum; i++) {
            if (i_regionNo + 1 == data[i].mRegionNo) {
                if (strcmp(i_stageName, data[i].mName) == 0) {
                    *o_offsetX = mRegionOriginX[i_regionNo] + data[i].mOffsetX;
                    *o_offsetZ = mRegionOriginZ[i_regionNo] + data[i].mOffsetZ;
                    return;
                } else {
                    j++;
                }
            }
        }
    }

    *o_offsetX = mRegionOriginX[i_regionNo];
    *o_offsetZ = mRegionOriginZ[i_regionNo];
}

void dMenu_Fmap2DBack_c::regionTextureDraw() {
    f32 local_84, local_88;
    f32 dVar14 = 0.0f;
    f32 dVar13 = 0.0f;
    calcAllMapPosWorld(getMapScissorAreaCenterPosX(), getMapScissorAreaCenterPosY(), &local_84,
                       &local_88);

    f32 local_8c, local_90;
    calcAllMapPos2D(local_84 + mStageTransX, local_88 + mStageTransZ, &local_8c, &local_90);

    dVar14 = getMapScissorAreaCenterPosX() - local_8c;
    dVar13 = getMapScissorAreaCenterPosY() - local_90;

    s32 uVar9 = mRegionCursor;

    s32 local_94;
    if (mMapZoomRate == 0.0f) {
        local_94 = mSelectRegion;
    } else {
        local_94 = mRegionCursor;
    }

    if (g_fmapHIO.mDrawBackground == false) {
        return;
    }

    for (int iVar8 = 0; iVar8 < 8; iVar8++) {
        s32 uVar10 = field_0x1230[iVar8];
        if (uVar10 != 0xff && mpAreaTex[uVar10] != NULL) {
            f32 fVar3;
            if (isShowRegion(uVar10)) {
                if (uVar10 != local_94) {
                    mpAreaTex[uVar10]->setBlackWhite(mUnselectedRegionBlack, mUnselectedRegionWhite);
                    fVar3 = field_0x1214;

                } else {
                    mpAreaTex[uVar10]->setBlackWhite(mSelectedRegionBlack, mSelectedRegionWhite);
                    fVar3 = field_0x1210;
                }
            } else {
                mpAreaTex[uVar10]->setBlackWhite(mUnreachedRegionBlack, mUnreachedRegionWhite);
                fVar3 = 0.5f;
            }

            mpAreaTex[uVar10]->setAlpha(mAlphaRate * 255.0f * fVar3 * field_0xfa4);

            if (uVar10 != uVar9) {
                bool b = 0;
                f32 v = mTransX + (dVar14 + (mRegionMinMapX[uVar10] + field_0xf0c[uVar10]));
                mpAreaTex[uVar10]->draw(
                    v, mTransZ + (dVar13 + (mRegionMinMapY[uVar10] + field_0xf2c[uVar10])),
                    mRegionMapSizeX[uVar10] * mZoom, mRegionMapSizeY[uVar10] * mZoom, b, false,
                    false);
            } else {
                bool b = 0;
                f32 v = mTransX + (dVar14 + (mRegionMinMapX[uVar9] + field_0xf0c[uVar9]));
                mpAreaTex[uVar9]->draw(
                    v, mTransZ + (dVar13 + (mRegionMinMapY[uVar9] + field_0xf2c[uVar9])),
                    mRegionMapSizeX[uVar9] * mZoom, mRegionMapSizeY[uVar9] * mZoom, b, false,
                    false);
            }
        }
    }
}

void dMenu_Fmap2DBack_c::stageTextureDraw() {
    if (mAllPathShowFlag) {
        mpSpotTexture->setAlpha(mAlphaRate * 255.0f * mSpotTextureFadeAlpha);
    } else {
        mpSpotTexture->setAlpha(mAlphaRate * 255.0f * field_0xfa8 * mSpotTextureFadeAlpha);
    }

    mpSpotTexture->draw(mTransX + getMapScissorAreaLX(), mTransZ + getMapScissorAreaLY(),
                        getMapScissorAreaSizeRealX(), getMapScissorAreaSizeRealY(), false, false,
                        false);
}

void dMenu_Fmap2DBack_c::worldGridDraw() {
    f32 local_74, local_78;

    f32 xPos, yPos;
    f32 dVar9 = -mStageTransX;
    f32 dVar8 = -mStageTransZ;
    calcAllMapPos2D(dVar9, dVar8, &local_74, &local_78);

    J2DDrawLine(local_74, mDoGph_gInf_c::getMinYF(), local_74,
                mDoGph_gInf_c::getMinYF() + mDoGph_gInf_c::getHeightF(),
                JUtility::TColor(255, 255, 255, 255), 6);

    xPos = dVar9 - g_fmapHIO.mWorldGridWidth;
    while (true) {
        calcAllMapPos2D(xPos, dVar8, &local_74, &local_78);
        if (local_74 >= getMapScissorAreaLX()) {
            J2DDrawLine(local_74, mDoGph_gInf_c::getMinYF(), local_74,
                        mDoGph_gInf_c::getMinYF() + mDoGph_gInf_c::getHeightF(),
                        JUtility::TColor(255, 255, 255, 255), 6);
            xPos -= g_fmapHIO.mWorldGridWidth;
        } else {
            break;
        }
    }

    xPos = dVar9 + g_fmapHIO.mWorldGridWidth;
    while (true) {
        calcAllMapPos2D(xPos, dVar8, &local_74, &local_78);
        if (local_74 <= getMapScissorAreaLX() + getMapScissorAreaSizeRealX()) {
            J2DDrawLine(local_74, mDoGph_gInf_c::getMinYF(), local_74,
                        mDoGph_gInf_c::getMinYF() + mDoGph_gInf_c::getHeightF(),
                        JUtility::TColor(255, 255, 255, 255), 6);
            xPos += g_fmapHIO.mWorldGridWidth;
        } else {
            break;
        }
    }

    J2DDrawLine(mDoGph_gInf_c::getMinXF(), local_78,
                mDoGph_gInf_c::getMinXF() + mDoGph_gInf_c::getWidthF(), local_78,
                JUtility::TColor(255, 255, 255, 255), 6);

    yPos = dVar8 - g_fmapHIO.mWorldGridWidth;
    while (true) {
        calcAllMapPos2D(dVar9, yPos, &local_74, &local_78);
        if (local_78 >= getMapScissorAreaLY()) {
            J2DDrawLine(mDoGph_gInf_c::getMinXF(), local_78,
                        mDoGph_gInf_c::getMinXF() + mDoGph_gInf_c::getWidthF(), local_78,
                        JUtility::TColor(255, 255, 255, 255), 6);
            yPos -= g_fmapHIO.mWorldGridWidth;
        } else {
            break;
        }
    }

    yPos = dVar8 + g_fmapHIO.mWorldGridWidth;
    while (true) {
        calcAllMapPos2D(dVar9, yPos, &local_74, &local_78);
        if (local_78 <= getMapScissorAreaLY() + getMapScissorAreaSizeRealY()) {
            J2DDrawLine(mDoGph_gInf_c::getMinXF(), local_78,
                        mDoGph_gInf_c::getMinXF() + mDoGph_gInf_c::getWidthF(), local_78,
                        JUtility::TColor(255, 255, 255, 255), 6);
            yPos += g_fmapHIO.mWorldGridWidth;
        } else {
            break;
        }
    }
}

void dMenu_Fmap2DBack_c::regionGridDraw() {
    f32 local_74, local_78;

    f32 xPos, yPos;
    f32 dVar9 = mRegionOriginX[mRegionCursor] - mStageTransX;
    f32 dVar8 = mRegionOriginZ[mRegionCursor] - mStageTransZ;
    calcAllMapPos2D(dVar9, dVar8, &local_74, &local_78);

    J2DDrawLine(local_74, mDoGph_gInf_c::getMinYF(), local_74,
                mDoGph_gInf_c::getMinYF() + mDoGph_gInf_c::getHeightF(),
                JUtility::TColor(180, 0, 0, 255), 6);

    xPos = dVar9 - g_fmapHIO.mRegionGridWidth;
    while (true) {
        calcAllMapPos2D(xPos, dVar8, &local_74, &local_78);
        if (local_74 >= getMapScissorAreaLX()) {
            J2DDrawLine(local_74, mDoGph_gInf_c::getMinYF(), local_74,
                        mDoGph_gInf_c::getMinYF() + mDoGph_gInf_c::getHeightF(),
                        JUtility::TColor(180, 0, 0, 255), 6);
            xPos -= g_fmapHIO.mRegionGridWidth;
        } else {
            break;
        }
    }

    xPos = dVar9 + g_fmapHIO.mRegionGridWidth;
    while (true) {
        calcAllMapPos2D(xPos, dVar8, &local_74, &local_78);
        if (local_74 <= getMapScissorAreaLX() + getMapScissorAreaSizeRealX()) {
            J2DDrawLine(local_74, mDoGph_gInf_c::getMinYF(), local_74,
                        mDoGph_gInf_c::getMinYF() + mDoGph_gInf_c::getHeightF(),
                        JUtility::TColor(180, 0, 0, 255), 6);
            xPos += g_fmapHIO.mRegionGridWidth;
        } else {
            break;
        }
    }

    J2DDrawLine(mDoGph_gInf_c::getMinXF(), local_78,
                mDoGph_gInf_c::getMinXF() + mDoGph_gInf_c::getWidthF(), local_78,
                JUtility::TColor(180, 0, 0, 255), 6);

    yPos = dVar8 - g_fmapHIO.mRegionGridWidth;
    while (true) {
        calcAllMapPos2D(dVar9, yPos, &local_74, &local_78);
        if (local_78 >= getMapScissorAreaLY()) {
            J2DDrawLine(mDoGph_gInf_c::getMinXF(), local_78,
                        mDoGph_gInf_c::getMinXF() + mDoGph_gInf_c::getWidthF(), local_78,
                        JUtility::TColor(180, 0, 0, 255), 6);
            yPos -= g_fmapHIO.mRegionGridWidth;
        } else {
            break;
        }
    }

    yPos = dVar8 + g_fmapHIO.mRegionGridWidth;
    while (true) {
        calcAllMapPos2D(dVar9, yPos, &local_74, &local_78);
        if (local_78 <= getMapScissorAreaLY() + getMapScissorAreaSizeRealY()) {
            J2DDrawLine(mDoGph_gInf_c::getMinXF(), local_78,
                        mDoGph_gInf_c::getMinXF() + mDoGph_gInf_c::getWidthF(), local_78,
                        JUtility::TColor(180, 0, 0, 255), 6);
            yPos += g_fmapHIO.mRegionGridWidth;
        } else {
            break;
        }
    }
}

// needed to make sdata2 match
static f32 dummy(int param_0) {
    return param_0;
}

void dMenu_Fmap2DBack_c::worldOriginDraw() {
    f32 local_44, local_48;
    calcAllMapPos2D(-mStageTransX, -mStageTransZ, &local_44, &local_48);

    J2DDrawLine(mDoGph_gInf_c::getMinXF(), local_48 - local_44 + mDoGph_gInf_c::getMinXF(),
                mDoGph_gInf_c::getMinXF() + mDoGph_gInf_c::getWidthF(),
                local_48 - local_44 + (mDoGph_gInf_c::getMinXF() + mDoGph_gInf_c::getWidthF()),
                JUtility::TColor(0, 255, 0, 255), 6);
    
    J2DDrawLine(mDoGph_gInf_c::getMinXF(), local_48 + local_44 + -mDoGph_gInf_c::getMinXF(),
                mDoGph_gInf_c::getMinXF() + mDoGph_gInf_c::getWidthF(),
                local_48 + local_44 + -(mDoGph_gInf_c::getMinXF() + mDoGph_gInf_c::getWidthF()),
                JUtility::TColor(0, 255, 0, 255), 6);
}

void dMenu_Fmap2DBack_c::scrollAreaDraw() {
    for (int i = 0; i < 8; i++) {
        if (mMapZoomRate != 0.0f && i != mRegionCursor) {
            continue;
        }

        f32 x_min, z_min, x_max, z_max;
        if (g_fmapHIO.mScrollRangeDebugON) {
            x_min = g_fmapHIO.mRegionScrollRangeMinX[i];
            z_min = g_fmapHIO.mRegionScrollRangeMinZ[i];
            x_max = g_fmapHIO.mRegionScrollRangeMaxX[i];
            z_max = g_fmapHIO.mRegionScrollRangeMaxZ[i];
        } else {
            x_min = mRegionScrollMinX[i];
            z_min = mRegionScrollMinZ[i];
            x_max = mRegionScrollMaxX[i];
            z_max = mRegionScrollMaxZ[i];
        }

        f32 local_4c, local_50, local_54, local_58;
        calcAllMapPos2D(x_min - mStageTransX, z_min - mStageTransZ, &local_4c, &local_50);
        calcAllMapPos2D(x_max - mStageTransX, z_max - mStageTransZ, &local_54, &local_58);

        J2DDrawLine(local_4c, local_50, local_4c, local_58,
                    JUtility::TColor(255, 255, 255, 255), 6);
        J2DDrawLine(local_54, local_50, local_54, local_58,
                    JUtility::TColor(255, 255, 255, 255), 6);
        J2DDrawLine(local_4c, local_50, local_54, local_50,
                    JUtility::TColor(255, 255, 255, 255), 6);
        J2DDrawLine(local_4c, local_58, local_54, local_58,
                    JUtility::TColor(255, 255, 255, 255), 6);
    }
}

void dMenu_Fmap2DBack_c::regionOriginDraw() {
    for (int i = 0; i < 8; i++) {
        if (!mpAreaTex[i]) {
            continue;
        }

        f32 center_x, center_y;
        calcAllMapPos2D(mRegionOriginX[i] - mStageTransX, mRegionOriginZ[i] - mStageTransZ,
                        &center_x, &center_y);
        J2DFillBox(center_x - 3.0f, center_y - 3.0f, 6.0f, 6.0f, JUtility::TColor(255, 0, 0, 255));
    }
}

void dMenu_Fmap2DBack_c::stageOriginDraw() {
    if (!mpStages) {
        return;
    }

    dMenuMapCommon_c::Stage_c::data* stage_data = mpStages->mData;

    for (int i = 0; i < mStageDataNum; i++) {
        if (mRegionCursor + 1 == stage_data[i].mRegionNo) {
            f32 center_x, center_y;
            f32 v1 = mRegionOriginX[mRegionCursor] + stage_data[i].mOffsetX - mStageTransX;
            f32 v2 = mRegionOriginZ[mRegionCursor] + stage_data[i].mOffsetZ - mStageTransZ;
            calcAllMapPos2D(v1, v2, &center_x, &center_y);
            J2DFillBox(center_x - 3.0f, center_y - 3.0f, 6.0f, 6.0f,
                       JUtility::TColor(0, 0, 255, 255));
        }
    }
}

void dMenu_Fmap2DBack_c::calcBlink() {
    f32 t;
    int i;
    if (mMapZoomRate <= 1.0f) {
        t = mMapZoomRate;
        i = 0;
    } else {
        t = mMapZoomRate - 1.0f;
        i = 1;
    }

    mSelectedRegionBlack.r = g_fmapHIO.mMapBlink[i].mSelectedRegion.mBlack.r
                        + t * (g_fmapHIO.mMapBlink[i + 1].mSelectedRegion.mBlack.r
                                - g_fmapHIO.mMapBlink[i].mSelectedRegion.mBlack.r);
    mSelectedRegionBlack.g = g_fmapHIO.mMapBlink[i].mSelectedRegion.mBlack.g
                        + t * (g_fmapHIO.mMapBlink[i + 1].mSelectedRegion.mBlack.g
                                - g_fmapHIO.mMapBlink[i].mSelectedRegion.mBlack.g);
    mSelectedRegionBlack.b = g_fmapHIO.mMapBlink[i].mSelectedRegion.mBlack.b
                        + t * (g_fmapHIO.mMapBlink[i + 1].mSelectedRegion.mBlack.b
                                - g_fmapHIO.mMapBlink[i].mSelectedRegion.mBlack.b);
    mSelectedRegionBlack.a = g_fmapHIO.mMapBlink[i].mSelectedRegion.mBlack.a
                        + t * (g_fmapHIO.mMapBlink[i + 1].mSelectedRegion.mBlack.a
                                - g_fmapHIO.mMapBlink[i].mSelectedRegion.mBlack.a);
    mSelectedRegionWhite.r = g_fmapHIO.mMapBlink[i].mSelectedRegion.mWhite.r
                        + t * (g_fmapHIO.mMapBlink[i + 1].mSelectedRegion.mWhite.r
                                - g_fmapHIO.mMapBlink[i].mSelectedRegion.mWhite.r);
    mSelectedRegionWhite.g = g_fmapHIO.mMapBlink[i].mSelectedRegion.mWhite.g
                        + t * (g_fmapHIO.mMapBlink[i + 1].mSelectedRegion.mWhite.g
                                - g_fmapHIO.mMapBlink[i].mSelectedRegion.mWhite.g);
    mSelectedRegionWhite.b = g_fmapHIO.mMapBlink[i].mSelectedRegion.mWhite.b
                        + t * (g_fmapHIO.mMapBlink[i + 1].mSelectedRegion.mWhite.b
                                - g_fmapHIO.mMapBlink[i].mSelectedRegion.mWhite.b);
    mSelectedRegionWhite.a = g_fmapHIO.mMapBlink[i].mSelectedRegion.mWhite.a
                        + t * (g_fmapHIO.mMapBlink[i + 1].mSelectedRegion.mWhite.a
                                - g_fmapHIO.mMapBlink[i].mSelectedRegion.mWhite.a);
    mUnselectedRegionBlack.r = g_fmapHIO.mMapBlink[i].mUnselectedRegion.mBlack.r
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnselectedRegion.mBlack.r
                                - g_fmapHIO.mMapBlink[i].mUnselectedRegion.mBlack.r);
    mUnselectedRegionBlack.g = g_fmapHIO.mMapBlink[i].mUnselectedRegion.mBlack.g
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnselectedRegion.mBlack.g
                                - g_fmapHIO.mMapBlink[i].mUnselectedRegion.mBlack.g);
    mUnselectedRegionBlack.b = g_fmapHIO.mMapBlink[i].mUnselectedRegion.mBlack.b
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnselectedRegion.mBlack.b
                                - g_fmapHIO.mMapBlink[i].mUnselectedRegion.mBlack.b);
    mUnselectedRegionBlack.a = g_fmapHIO.mMapBlink[i].mUnselectedRegion.mBlack.a
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnselectedRegion.mBlack.a
                                - g_fmapHIO.mMapBlink[i].mUnselectedRegion.mBlack.a);
    mUnselectedRegionWhite.r = g_fmapHIO.mMapBlink[i].mUnselectedRegion.mWhite.r
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnselectedRegion.mWhite.r
                                - g_fmapHIO.mMapBlink[i].mUnselectedRegion.mWhite.r);
    mUnselectedRegionWhite.g = g_fmapHIO.mMapBlink[i].mUnselectedRegion.mWhite.g
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnselectedRegion.mWhite.g
                                - g_fmapHIO.mMapBlink[i].mUnselectedRegion.mWhite.g);
    mUnselectedRegionWhite.b = g_fmapHIO.mMapBlink[i].mUnselectedRegion.mWhite.b
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnselectedRegion.mWhite.b
                                - g_fmapHIO.mMapBlink[i].mUnselectedRegion.mWhite.b);
    mUnselectedRegionWhite.a = g_fmapHIO.mMapBlink[i].mUnselectedRegion.mWhite.a
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnselectedRegion.mWhite.a
                                - g_fmapHIO.mMapBlink[i].mUnselectedRegion.mWhite.a);
    mUnreachedRegionBlack.r = g_fmapHIO.mMapBlink[i].mUnreachedRegionBlack.r
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnreachedRegionBlack.r
                                - g_fmapHIO.mMapBlink[i].mUnreachedRegionBlack.r);
    mUnreachedRegionBlack.g = g_fmapHIO.mMapBlink[i].mUnreachedRegionBlack.g
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnreachedRegionBlack.g
                                - g_fmapHIO.mMapBlink[i].mUnreachedRegionBlack.g);
    mUnreachedRegionBlack.b = g_fmapHIO.mMapBlink[i].mUnreachedRegionBlack.b
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnreachedRegionBlack.b
                                - g_fmapHIO.mMapBlink[i].mUnreachedRegionBlack.b);
    mUnreachedRegionBlack.a = g_fmapHIO.mMapBlink[i].mUnreachedRegionBlack.a
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnreachedRegionBlack.a
                                - g_fmapHIO.mMapBlink[i].mUnreachedRegionBlack.a);
    mUnreachedRegionWhite.r = g_fmapHIO.mMapBlink[i].mUnreachedRegionWhite.r
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnreachedRegionWhite.r
                                - g_fmapHIO.mMapBlink[i].mUnreachedRegionWhite.r);
    mUnreachedRegionWhite.g = g_fmapHIO.mMapBlink[i].mUnreachedRegionWhite.g
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnreachedRegionWhite.g
                                - g_fmapHIO.mMapBlink[i].mUnreachedRegionWhite.g);
    mUnreachedRegionWhite.b = g_fmapHIO.mMapBlink[i].mUnreachedRegionWhite.b
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnreachedRegionWhite.b
                                - g_fmapHIO.mMapBlink[i].mUnreachedRegionWhite.b);
    mUnreachedRegionWhite.a = g_fmapHIO.mMapBlink[i].mUnreachedRegionWhite.a
                        + t * (g_fmapHIO.mMapBlink[i + 1].mUnreachedRegionWhite.a
                                - g_fmapHIO.mMapBlink[i].mUnreachedRegionWhite.a);
    field_0x1208.r = g_fmapHIO.mMapBlink[i].field_0x20.r
                        + t * (g_fmapHIO.mMapBlink[i + 1].field_0x20.r
                                - g_fmapHIO.mMapBlink[i].field_0x20.r);
    field_0x1208.g = g_fmapHIO.mMapBlink[i].field_0x20.g
                        + t * (g_fmapHIO.mMapBlink[i + 1].field_0x20.g
                                - g_fmapHIO.mMapBlink[i].field_0x20.g);
    field_0x1208.b = g_fmapHIO.mMapBlink[i].field_0x20.b
                        + t * (g_fmapHIO.mMapBlink[i + 1].field_0x20.b
                                - g_fmapHIO.mMapBlink[i].field_0x20.b);
    field_0x1208.a = g_fmapHIO.mMapBlink[i].field_0x20.a
                        + t * (g_fmapHIO.mMapBlink[i + 1].field_0x20.a
                                - g_fmapHIO.mMapBlink[i].field_0x20.a);
    field_0x120c.r = g_fmapHIO.mMapBlink[i].field_0x24.r
                        + t * (g_fmapHIO.mMapBlink[i + 1].field_0x24.r
                                - g_fmapHIO.mMapBlink[i].field_0x24.r);
    field_0x120c.g = g_fmapHIO.mMapBlink[i].field_0x24.g
                        + t * (g_fmapHIO.mMapBlink[i + 1].field_0x24.g
                                - g_fmapHIO.mMapBlink[i].field_0x24.g);
    field_0x120c.b = g_fmapHIO.mMapBlink[i].field_0x24.b
                        + t * (g_fmapHIO.mMapBlink[i + 1].field_0x24.b
                                - g_fmapHIO.mMapBlink[i].field_0x24.b);
    field_0x120c.a = g_fmapHIO.mMapBlink[i].field_0x24.a
                        + t * (g_fmapHIO.mMapBlink[i + 1].field_0x24.a
                                - g_fmapHIO.mMapBlink[i].field_0x24.a);

    f32 selected_max, selected_min, unselected_max, unselected_min;
    f32 tmp_selected_min = g_fmapHIO.mMapBlink[i].mSelectedRegion.mMin / 255.0f;
    f32 tmp_selected_max = g_fmapHIO.mMapBlink[i].mSelectedRegion.mMax / 255.0f;
    f32 tmp_unselected_min = g_fmapHIO.mMapBlink[i].mUnselectedRegion.mMin / 255.0f;
    f32 tmp_unselected_max = g_fmapHIO.mMapBlink[i].mUnselectedRegion.mMax / 255.0f;
                            
    f32 sp20 = g_fmapHIO.mMapBlink[i + 1].mSelectedRegion.mMin / 255.0f;
    f32 sp1C = g_fmapHIO.mMapBlink[i + 1].mSelectedRegion.mMax / 255.0f;
    f32 sp18 = g_fmapHIO.mMapBlink[i + 1].mUnselectedRegion.mMin / 255.0f;
    f32 sp14 = g_fmapHIO.mMapBlink[i + 1].mUnselectedRegion.mMax / 255.0f;

    selected_max = tmp_selected_max + t * (sp1C - tmp_selected_max);
    selected_min = tmp_selected_min + t * (sp20 - tmp_selected_min);

    s16 selected_blink_speed = g_fmapHIO.mMapBlink[i].mSelectedRegion.mBlinkSpeed +
                               t * (g_fmapHIO.mMapBlink[i + 1].mSelectedRegion.mBlinkSpeed -
                                    g_fmapHIO.mMapBlink[i].mSelectedRegion.mBlinkSpeed);

    unselected_max = tmp_unselected_max + t * (sp14 - tmp_unselected_max);
    unselected_min = tmp_unselected_min + t * (sp18 - tmp_unselected_min);

    s16 unselected_blink_speed = g_fmapHIO.mMapBlink[i].mUnselectedRegion.mBlinkSpeed +
                                 t * (g_fmapHIO.mMapBlink[i + 1].mUnselectedRegion.mBlinkSpeed -
                                      g_fmapHIO.mMapBlink[i].mUnselectedRegion.mBlinkSpeed);

    field_0x1218++;
    if (field_0x1218 >= selected_blink_speed) {
        field_0x1218 = 0;
    }

    field_0x121a++;
    if (field_0x121a >= unselected_blink_speed) {
        field_0x121a = 0;
    }

    f32 t_selected = 0.0f;
    f32 t_unselected = 0.0f;

    if (selected_blink_speed > 0.0f) {
        if (field_0x1218 < selected_blink_speed * 0.5f) {
            t_selected = field_0x1218 / (selected_blink_speed * 0.5f);
        } else {
            t_selected = (selected_blink_speed - field_0x1218) / (selected_blink_speed * 0.5f);
        }
    }

    if (unselected_blink_speed > 0.0f) {
        if (field_0x121a < unselected_blink_speed * 0.5f) {
            t_unselected = field_0x121a / (unselected_blink_speed * 0.5f);
        } else {
            t_unselected = (unselected_blink_speed - field_0x121a) / (unselected_blink_speed * 0.5f);
        }
    }

    field_0x1210 = selected_min + t_selected * (selected_max - selected_min);
    field_0x1214 = unselected_min + t_unselected * (unselected_max - unselected_min);
}

void dMenu_Fmap2DBack_c::calcBackAlpha(bool param_0) {
    if (param_0) {
        if (mBackAlpha != 1.0f) {
            cLib_addCalc2(&mBackAlpha, 1.0f, 0.4f, 0.5f);
            if (std::fabs(mBackAlpha - 1.0f) < 0.1f) {
                mBackAlpha = 1.0f;
            }
        }
    } else {
        if (mBackAlpha != 0.0f) {
            cLib_addCalc2(&mBackAlpha, 0.0f, 0.4f, 0.5f);
            if (std::fabs(mBackAlpha) < 0.1f) {
                mBackAlpha = 0.0f;
            }
        }
    }
}

void dMenu_Fmap2DBack_c::btkAnimeLoop(f32 i_step) {
    if (mpBaseAnm) {
        mAnmFrame += i_step;
        if (mAnmFrame >= mpBaseAnm->getFrameMax()) {
            mAnmFrame -= mpBaseAnm->getFrameMax();
        }
        mpBaseAnm->setFrame(mAnmFrame);
    } else {
        mAnmFrame = 0.0f;
    }

    mpBaseScreen->search('gold00_0')->setAnimation(mpBaseAnm);
    mpBaseScreen->search('gold00_1')->setAnimation(mpBaseAnm);
    mpBaseScreen->animation();
}

void dMenu_Fmap2DBack_c::setBaseBackAlpha(u8 param_0) {
    mpMapBlack->setAlpha(param_0);
}

void dMenu_Fmap2DBack_c::regionMapMove(STControl* i_stick) {
    f32 min_x, min_y, max_x, max_y;
    calcAllMapPosWorld(getMapScissorAreaLX(), getMapScissorAreaLY(), &min_x, &min_y);
    calcAllMapPosWorld(getMapScissorAreaLX() + getMapScissorAreaSizeRealX(),
                       getMapScissorAreaLY() + getMapScissorAreaSizeRealY(), &max_x, &max_y);
    int r27 = 0;
    if (r27 == 0) {
        f32 slow_bound = g_fmapHIO.mScrollSpeedSlowBound < g_fmapHIO.mScrollSpeedFastBound ?
                             g_fmapHIO.mScrollSpeedSlowBound :
                             g_fmapHIO.mScrollSpeedFastBound;
        f32 fast_bound = g_fmapHIO.mScrollSpeedSlowBound > g_fmapHIO.mScrollSpeedFastBound ?
                             g_fmapHIO.mScrollSpeedSlowBound :
                             g_fmapHIO.mScrollSpeedFastBound;

        f32 stick_value = i_stick->getValueStick();
        if (stick_value >= slow_bound) {
            s16 angle = i_stick->getAngleStick();
            f32 local_68 = mTexMaxX - mTexMinX;
            f32 spot_zoom = getSpotMapZoomRate();
            f32 region_zoom = getRegionMapZoomRate(mRegionCursor);
            f32 zoom = region_zoom + (mMapZoomRate - 1.0f) * (spot_zoom - region_zoom);
            f32 local_78 = local_68 / zoom;
            f32 base_speed;
            if (stick_value < fast_bound) {
                base_speed = g_fmapHIO.mScrollSpeedRegionSlow;
            } else {
                base_speed = g_fmapHIO.mScrollSpeedRegionFast;
            }
            f32 speed = base_speed / 100.0f * local_78;
            f32 speed_y = speed * cM_ssin(angle);
            f32 speed_x = speed * cM_scos(angle);
            control_xpos += speed_y;
            control_ypos += speed_x;
        }
    }

    if (mArrowPos3DX + control_xpos < min_x) {
        control_xpos = min_x - mArrowPos3DX;
    }
    if (mArrowPos3DX + control_xpos > max_x) {
        control_xpos = max_x - mArrowPos3DX;
    }
    if (mArrowPos3DZ + control_ypos < min_y) {
        control_ypos = min_y - mArrowPos3DZ;
    }
    if (mArrowPos3DZ + control_ypos > max_y) {
        control_ypos = max_y - mArrowPos3DZ;
    }

    calcRenderingScale();
    calcRenderingPos();

    f32 pos_x, pos_y;
    calcAllMapPos2D(mArrowPos3DX + control_xpos - mStageTransX,
                    mArrowPos3DZ + control_ypos - mStageTransZ, &pos_x, &pos_y);

    mSelectRegion = 0xff;
    int region = mRegionCursor;
    if (region != 0xff && region != 7) {
        u8 pix = dMeter2Info_getPixel(
            pos_x, pos_y, mRegionMinMapX[region] + field_0xf0c[region] + mTransX,
            mRegionMinMapY[region] + field_0xf2c[region] + mTransZ, mRegionMapSizeX[region] * mZoom,
            mRegionMapSizeY[region] * mZoom, mpAreaTex[region]->getTexture(0)->getTexInfo());
        if (pix) {
            mSelectRegion = region;
        }
    }
}

void dMenu_Fmap2DBack_c::stageMapMove(STControl* i_stick, u8 param_1, bool param_2) {
    UNUSED(param_1);
    
    f32 slow_bound = g_fmapHIO.mScrollSpeedSlowBound < g_fmapHIO.mScrollSpeedFastBound ?
    g_fmapHIO.mScrollSpeedSlowBound : g_fmapHIO.mScrollSpeedFastBound;
    f32 fast_bound = g_fmapHIO.mScrollSpeedSlowBound > g_fmapHIO.mScrollSpeedFastBound ?
    g_fmapHIO.mScrollSpeedSlowBound : g_fmapHIO.mScrollSpeedFastBound;
    
    f32  min_x, min_y, max_x, max_y;
    f32 stick_value = i_stick->getValueStick();
    bool bVar6 = false;
    f32 trans_x = mStageTransX;
    f32 trans_z = mStageTransZ;

    if (stick_value >= slow_bound && param_2 && field_0x1238 != 2) {
        bVar6 = true;
        s16 angle = i_stick->getAngleStick();
        f32 local_68 = mTexMaxX - mTexMinX;
        f32 spot_zoom = getSpotMapZoomRate();
        f32 region_zoom = getRegionMapZoomRate(mRegionCursor);
        f32 zoom = region_zoom + (mMapZoomRate - 1.0f) * (spot_zoom - region_zoom);
        f32 local_78 = local_68 / zoom;
        f32 base_speed;
        if (stick_value < fast_bound) {
            base_speed = g_fmapHIO.mScrollSpeedRegionZoomSlow;
        } else {
            base_speed = g_fmapHIO.mScrollSpeedRegionZoomFast;
        }
        f32 speed = base_speed / 100.0f * local_78;
        f32 speed_x = speed * cM_ssin(angle);
        f32 speed_z = speed * cM_scos(angle);
        mStageTransX += speed_x;
        mStageTransZ += speed_z;
    } else if (!param_2) {
        return;
    }
    f32 center_x, center_y;
    calcAllMapPosWorld(getMapAreaGlobalCenterPosX(), getMapAreaGlobalCenterPosY(),
                       &center_x, &center_y);
    calcAllMapPosWorld(getMapScissorAreaLX(), getMapScissorAreaLY(), &min_x, &min_y);

    if (g_fmapHIO.mScrollRangeDebugON) {
        min_x = g_fmapHIO.mRegionScrollRangeMinX[mRegionCursor];
        min_y = g_fmapHIO.mRegionScrollRangeMinZ[mRegionCursor];
        max_x = g_fmapHIO.mRegionScrollRangeMaxX[mRegionCursor];
        max_y = g_fmapHIO.mRegionScrollRangeMaxZ[mRegionCursor];
    } else {
        min_x = mRegionScrollMinX[mRegionCursor];
        min_y = mRegionScrollMinZ[mRegionCursor];
        max_x = mRegionScrollMaxX[mRegionCursor];
        max_y = mRegionScrollMaxZ[mRegionCursor];
    }

    if (center_x < min_x - mStageTransX) {
        mStageTransX = -(center_x - min_x);
    }
    if (center_x > max_x - mStageTransX) {
        mStageTransX = -(center_x - max_x);
    }
    if (center_y < min_y - mStageTransZ) {
        mStageTransZ = -(center_y - min_y);
    }
    if (center_y > max_y - mStageTransZ) {
        mStageTransZ = -(center_y - max_y);
    }

    if (bVar6 && (trans_x != mStageTransX || trans_z != mStageTransZ)) {
        Z2GetAudioMgr()->seStartLevel(Z2SE_SY_MAP_SCROLL, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }

    field_0xfd8 = mStageTransX - field_0xfd0;
    field_0xfdc = mStageTransZ - field_0xfd4;
    control_xpos = mStageTransX + (field_0x11b4 - mArrowPos3DX);
    control_ypos = mStageTransZ + (field_0x11b8 - mArrowPos3DZ);

    calcRenderingScale();
    calcRenderingPos();

    field_0x122d = 0;
    if (mStageTransX > -(center_x - min_x)) {
        field_0x122d |= (u8)1;
    }
    if (mStageTransX < -(center_x - max_x)) {
        field_0x122d |= (u8)4;
    }
    if (mStageTransZ > -(center_y - min_y)) {
        field_0x122d |= (u8)8;
    }
    if (mStageTransZ < -(center_y - max_y)) {
        field_0x122d |= (u8)2;
    }

    mpMeterHaihai->_execute(0);
}

void dMenu_Fmap2DBack_c::setAllAlphaRate(f32 i_rate, bool i_init) {
    mAlphaRate = i_rate;
    if (i_init) {
        mpBaseRoot->setBackupAlpha();
    }
    mpBaseRoot->setAlphaMorfRate(i_rate);
}

void dMenu_Fmap2DBack_c::drawDebugStageArea() {
    JUtility::TColor colors[6] = {
        JUtility::TColor(255, 255, 0, 50),
        JUtility::TColor(255, 0, 255, 50),
        JUtility::TColor(0, 255, 255, 50),
        JUtility::TColor(255, 0, 0, 50),
        JUtility::TColor(0, 255, 0, 50),
        JUtility::TColor(0, 0, 255, 50),
    };

    if (mpFmapMap && mpRegionData) {
        for (int i = 0; i < mDoGph_gInf_c::getWidthF(); i += g_fmapHIO.mRangeCheckInterval + 1) {
            for (int j = 0; j < mDoGph_gInf_c::getHeightF(); j += g_fmapHIO.mRangeCheckInterval + 1)
            {
                int stage_no;
                mpFmapMap->getPointStagePathInnerNo(mpRegionData,
                                                    i - mDoGph_gInf_c::getWidthF() / 2.0f,
                                                    j - mDoGph_gInf_c::getHeightF() / 2.0f,
                                                    field_0x1244, &stage_no, NULL);
                if (stage_no >= 0) {
                    f32 v = i + mDoGph_gInf_c::getMinXF();
                    f32 v2 = j;
                    J2DFillBox(v - 3.0f, v2 - 3.0f, 6.0f, 6.0f, colors[stage_no % 6]);
                }
            }
        }
    }
}

void dMenu_Fmap2DBack_c::drawDebugRegionArea() {
    if (mZoom != getAllMapZoomRate()) {
    }

    JUtility::TColor colors[7] = {
        JUtility::TColor(255, 255, 0, 50),
        JUtility::TColor(255, 0, 255, 50),
        JUtility::TColor(0, 255, 255, 50),
        JUtility::TColor(255, 0, 0, 50),
        JUtility::TColor(0, 255, 0, 50),
        JUtility::TColor(0, 0, 255, 50),
        JUtility::TColor(255, 255, 255, 50),
    };

    for (int i = 0; i < getMapScissorAreaSizeRealX(); i += g_fmapHIO.mRangeCheckInterval + 1) {
        for (int j = 0; j < getMapScissorAreaSizeRealY(); j += g_fmapHIO.mRangeCheckInterval + 1) {
            u8 u = 0;
            for (int k = 7; k >= 0; k--) {
                int region = field_0x1230[k];
                if (region == 0xff || region == 7) continue;
                f32 pos_x = i + getMapScissorAreaLX();
                f32 pos_y = j + getMapScissorAreaLY();
                u = dMeter2Info_getPixel(
                    pos_x, pos_y, mRegionMinMapX[region] + field_0xf0c[region] + mTransX,
                    mRegionMinMapY[region] + field_0xf2c[region] + mTransZ,
                    mRegionMapSizeX[region] * mZoom, mRegionMapSizeY[region] * mZoom,
                    mpAreaTex[region]->getTexture(0)->getTexInfo());
                if (u) {
                    J2DFillBox(pos_x - 3.0f, pos_y - 3.0f, 6.0f, 6.0f, colors[region]);
                    break;
                }
            }
        }
    }
}

void dMenu_Fmap2DBack_c::setArrowPos3D(u8 i_regionNo, char const* i_stageName, f32 i_posX,
                                       f32 i_posZ) {
    f32 offset_x = 0.0f;
    f32 offset_z = 0.0f;

    if (i_stageName) {
        calcOffset(i_regionNo, i_stageName, &offset_x, &offset_z);
    } else {
        i_posX = 0.5f * (mRegionTexData[i_regionNo].mMinX + mRegionTexData[i_regionNo].mMaxX);
        i_posZ = 0.5f * (mRegionTexData[i_regionNo].mMinZ + mRegionTexData[i_regionNo].mMaxZ);
    }

    mArrowPos3DX = i_posX + offset_x;
    mArrowPos3DZ = i_posZ + offset_z;
    control_xpos = 0.0f;
    control_ypos = 0.0f;
}

void dMenu_Fmap2DBack_c::setArrowPos3DOffset(u8 i_regionNo, char const* i_stageName, f32 i_posX,
                                             f32 i_posZ, f32 param_4) {
    f32 offset_x, offset_z, pos2d_x, pos2d_y;
    
    if (param_4 != 0.0f) {
        calcOffset(i_regionNo, i_stageName, &offset_x, &offset_z);
        f32 v = i_posX + offset_x - getRegionOriginX(i_regionNo);
        f32 v2 = i_posZ + offset_z - getRegionOriginZ(i_regionNo);
        calcAllMapPos2D(v, v2, &pos2d_x, &pos2d_y);
        pos2d_y -= param_4;
        calcAllMapPosWorld(pos2d_x, pos2d_y, &i_posX, &i_posZ);
        i_posX += getRegionOriginX(i_regionNo);
        i_posZ += getRegionOriginZ(i_regionNo);
    }

    mArrowPos3DX = i_posX;
    mArrowPos3DZ = i_posZ;
    control_xpos = 0.0f;
    control_ypos = 0.0f;
}

void dMenu_Fmap2DBack_c::calcDrawPriority() {
    u8 region;
    u8 dVar2 = mRegionCursor;
    u8 dVar3 = 6;

    for (int i = 7; i >= 0; i--) {
        if (field_0x1230[i] != 0xff) {
            if (field_0x1230[i] == mRegionCursor) {
                field_0x1230[i] = dVar2;
                break;
            } else {
                u8 tmp = field_0x1230[i];
                field_0x1230[i] = dVar2;
                dVar2 = tmp;
            }
        }
    }

    if (mRegionCursor != 6) {
        for (int i = 7; i >= 0; i--) {
            if (field_0x1230[i] != 0xff) {
                if (field_0x1230[i] == 6) {
                    field_0x1230[i] = dVar3;
                    break;
                } else {
                    u8 tmp = field_0x1230[i];
                    field_0x1230[i] = dVar3;
                    dVar3 = tmp;
                }
            }
        }
    }

    if (field_0x1230[0] != 7) {
        u8 dVar4 = field_0x1230[0];
        field_0x1230[0] = 7;
        for (int i = 1; i < 8; i++) {
            if (field_0x1230[i] != 0xff) {
                if (field_0x1230[i] == 7) {
                    field_0x1230[i] = dVar4;
                    break;
                } else {
                    u8 tmp = field_0x1230[i];
                    field_0x1230[i] = dVar4;
                    dVar4 = tmp;
                }
            }
        }
    }
}

void dMenu_Fmap2DBack_c::setArrowPosAxis(f32 i_posX, f32 i_posZ) {
    mArrowPos3DX = i_posX;
    mArrowPos3DZ = i_posZ;
    control_xpos = 0.0f;
    control_ypos = 0.0f;
}

dMenu_Fmap2DTop_c::dMenu_Fmap2DTop_c(JKRExpHeap* i_heap, STControl* i_stick) {
    mpHeap = i_heap;
    mTransX = 0.0f;
    mTransY = 0.0f;
    mpPortalBin = NULL;
    mpScrnExplain = NULL;

    mpTitleScreen = new J2DScreen();
    JUT_ASSERT(3872, mpTitleScreen != NULL);
#if PLATFORM_GCN
    bool fg = mpTitleScreen->setPriority("zelda_map_screen_title.blo", 0x1020000,
                                         dComIfGp_getFmapResArchive());
#else
    bool fg = mpTitleScreen->setPriority("zelda_map_screen_title_revo.blo", 0x1020000,
                                         dComIfGp_getFmapResArchive());
#endif
    JUT_ASSERT(3877, fg != false);

    dPaneClass_showNullPane(mpTitleScreen);
    mpTitleRoot = new CPaneMgrAlphaMorf(mpTitleScreen, 'ROOT', 2, NULL);
    JUT_ASSERT(3881, mpTitleRoot != NULL);
#if PLATFORM_SHIELD
    for (int i = 0; i < 2; i++) {
        mpArrowLAlpha[i] = NULL;
        mpArrowRAlpha[i] = NULL;
    }
    mpDpadAlpha = NULL;
    mpAnalogStickAlpha = NULL;
    mpAnalogStick = NULL;
    mpDpad = NULL;
    mpTitleScreen->search('tri_Null')->hide();
#endif
#if PLATFORM_GCN
    mpArrowLAlpha[0] = new CPaneMgrAlpha(mpTitleScreen, 'yaji_04', 0, NULL);
    mpArrowLAlpha[1] = new CPaneMgrAlpha(mpTitleScreen, 'yaji_05', 0, NULL);
    mpArrowRAlpha[0] = new CPaneMgrAlpha(mpTitleScreen, 'yaji_06', 0, NULL);
    mpArrowRAlpha[1] = new CPaneMgrAlpha(mpTitleScreen, 'yaji_07', 0, NULL);
    
    mpDpadAlpha = new CPaneMgrAlpha(mpTitleScreen, 'juji_c_n', 2, NULL);
    mpDpadAlpha->setAlphaRate(0.0f);
    mpAnalogStickAlpha = new CPaneMgrAlpha(mpTitleScreen, 'as_n', 2, NULL);
    mpAnalogStickAlpha->setAlphaRate(0.0f);
    mpAnalogStick = new CPaneMgr(mpTitleScreen, 'as_n', 0, NULL);
    mpDpad = new CPaneMgr(mpTitleScreen, 'juji_c_n', 0, NULL);
#endif
#if PLATFORM_SHIELD
    mpButtonA = new CPaneMgr(mpTitleScreen, 'abtn_n', 2, NULL);
    JUT_ASSERT(3935, mpButtonA != NULL);
    mpButtonB = new CPaneMgr(mpTitleScreen, 'bbtn_n', 2, NULL);
    JUT_ASSERT(3938, mpButtonB != NULL);
    mpButtonZ = new CPaneMgr(mpTitleScreen, 'j_scal_n', 2, NULL);
    JUT_ASSERT(3941, mpButtonZ != NULL);
    mpButtonTextA = new CPaneMgr(mpTitleScreen, 'a_text_n', 2, NULL);
    JUT_ASSERT(3953, mpButtonTextA != NULL);
    mpButtonTextB = new CPaneMgr(mpTitleScreen, 'b_text_n', 2, NULL);
    JUT_ASSERT(3956, mpButtonTextB != NULL);
    mpButtonTextZ = new CPaneMgr(mpTitleScreen, 'z_text_n', 2, NULL);
    JUT_ASSERT(3959, mpButtonTextZ != NULL);
#else
    mpButtonA = new CPaneMgr(mpTitleScreen, 'abtn_n1', 2, NULL);
    JUT_ASSERT(3935, mpButtonA != NULL);
    mpButtonB = new CPaneMgr(mpTitleScreen, 'bbtn_n1', 2, NULL);
    JUT_ASSERT(3938, mpButtonB != NULL);
    mpButtonZ = new CPaneMgr(mpTitleScreen, 'zbtn_n1', 2, NULL);
    JUT_ASSERT(3941, mpButtonZ != NULL);
    mpButtonTextA = new CPaneMgr(mpTitleScreen, 'a_text_n', 2, NULL);
    JUT_ASSERT(3953, mpButtonTextA != NULL);
    mpButtonTextB = new CPaneMgr(mpTitleScreen, 'b_text_n', 2, NULL);
    JUT_ASSERT(3956, mpButtonTextB != NULL);
    mpButtonTextZ = new CPaneMgr(mpTitleScreen, 'z_text_n', 2, NULL);
    JUT_ASSERT(3959, mpButtonTextZ != NULL);
#endif
    mpButtonA->setAlphaRate(0.0f);
    mpButtonB->setAlphaRate(0.0f);
    mpButtonZ->setAlphaRate(0.0f);
    mpButtonTextA->setAlphaRate(0.0f);
    mpButtonTextB->setAlphaRate(0.0f);
    mpButtonTextZ->setAlphaRate(0.0f);

    mpNamePane = new CPaneMgr(mpTitleScreen, 'name_n', 0, NULL);
    JUT_ASSERT(3970, mpNamePane != NULL);
    mpSubPane = new CPaneMgr(mpTitleScreen, 'sub_n_n', 0, NULL);
    JUT_ASSERT(3974, mpSubPane != NULL);

    if (dMeter2Info_getMapStatus() == 9) {
        mpNamePane->hide();
        mpSubPane->hide();
    }
    mpContPane = new CPaneMgr(mpTitleScreen, 'cont_n', 0, NULL);
    JUT_ASSERT(3984, mpContPane != NULL);

    mTitlePosX = mTitlePosY = 0.0f;
    mTitleScale = 1.0f;
    mSubTitlePosX = mSubTitlePosY = 0.0f;
    mSubTitleScale = 1.0f;
    mAreaMovementPosX = mAreaMovementPosY = 0.0f;
    mAreaMovementScale = 1.0f;
    mClosingPosX = mClosingPosY = 0.0f;
    mClosingScale = 1.0f;
    mDoIconPosX = mDoIconPosY = 0.0f;
    mDoIconScale = 1.0f;

    OSInitFastCast();
#if PLATFORM_GCN
    mpAnm = static_cast<J2DAnmTextureSRTKey*>(J2DAnmLoaderDataBase::load(
        JKRFileLoader::getGlbResource("zelda_map_screen_title.btk", dComIfGp_getFmapResArchive())));
#else
    void* v = JKRGetNameResource("zelda_map_screen_title_revo.btk", dComIfGp_getFmapResArchive());
    mpAnm = static_cast<J2DAnmTextureSRTKey*>(J2DAnmLoaderDataBase::load(v));
#endif
    mpAnm->searchUpdateMaterialID(mpTitleScreen);

    mAnmFrame = 0.0f;

    static const u64 area_name[3] = {'i_name_s', 'i_name' ,'i_name1'};
    static const u64 farea_name[3] = {'f_name_1', 'f_name3', 'f_name2'};
    for (int i = 0; i < 3; i++) {
#if VERSION == VERSION_GCN_JPN
        static_cast<J2DTextBox*>(mpTitleScreen->search(area_name[i]))
            ->setFont(mDoExt_getRubyFont());
        static_cast<J2DTextBox*>(mpTitleScreen->search(area_name[i]))->setString(0x40, "");
        mpTitleScreen->search(farea_name[i])->hide();
#else
        static_cast<J2DTextBox*>(mpTitleScreen->search(farea_name[i]))
            ->setFont(mDoExt_getRubyFont());
        static_cast<J2DTextBox*>(mpTitleScreen->search(farea_name[i]))->setString(0x40, "");
        mpTitleScreen->search(area_name[i])->hide();
#endif
    }
#if PLATFORM_GCN
    static const u64 sfont_name[7] = {
        'sfont00', 'sfontl0', 'sfontl1', 'sfontl2', 'sfontb0', 'sfontb1', 'sfontb2'
    };
    static const u64 ffont_name[7] = {
        'ffont00', 'ffontl0', 'ffontl1', 'ffontl2', 'ffontb0', 'ffontb3', 'ffontb4'
    };
#else
    static const u64 sfont_name[7] = {
        'sfont00', 'sfontl0', 'sfontl1', 'sfontl2', 'sfontb0', 'sfontb1', 'sfontb2'
    };
    static const u64 ffont_name[7] = {
        'ffont01', 'ffontl3', 'ffontl4', 'ffontl5', 'ffontb3', 'ffontb4', 'ffontb5'
    };
#endif
    for (int i = 0; i < 7; i++) {
#if VERSION == VERSION_GCN_JPN
        static_cast<J2DTextBox*>(mpTitleScreen->search(sfont_name[i]))
            ->setFont(mDoExt_getRubyFont());
        static_cast<J2DTextBox*>(mpTitleScreen->search(sfont_name[i]))->setString(0x40, "");
        mpTitleScreen->search(ffont_name[i])->hide();
#else
        static_cast<J2DTextBox*>(mpTitleScreen->search(ffont_name[i]))
            ->setFont(mDoExt_getRubyFont());
        static_cast<J2DTextBox*>(mpTitleScreen->search(ffont_name[i]))->setString(0x40, "");
        mpTitleScreen->search(sfont_name[i])->hide();
#endif
    }
    setTitleNameString(0x3e0);

    static const u64 cont_zt[5] = {'cont_zt', 'cont_zt1', 'cont_zt2', 'cont_zt3', 'cont_zt4'};
    static const u64 font_zt[5] = {'font_zt1', 'font_zt2', 'font_zt3', 'font_zt4', 'font_zt5'};
    for (int i = 0; i < 5; i++) {
#if VERSION == VERSION_GCN_JPN
        static_cast<J2DTextBox*>(mpTitleScreen->search(cont_zt[i]))
            ->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpTitleScreen->search(cont_zt[i]))->setString(0x20, "");
        mpTitleScreen->search(font_zt[i])->hide();
#else
        static_cast<J2DTextBox*>(mpTitleScreen->search(font_zt[i]))
            ->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpTitleScreen->search(font_zt[i]))->setString(0x20, "");
        mpTitleScreen->search(cont_zt[i])->hide();
#endif
    }
    setZButtonString(0x529, 0xff);
#if PLATFORM_GCN
    static const u64 cont_bt[5] = {'cont_bt1', 'cont_bt2', 'cont_bt3', 'cont_bt4', 'cont_bt'};
    static const u64 font_bt[5] = {'font_bt1', 'font_bt2', 'font_bt3', 'font_bt4', 'font_bt5'};
#else
    static const u64 cont_bt[5] = {'cont_bt1', 'cont_bt2', 'cont_bt3', 'cont_bt4', 'cont_bt8'};
    static const u64 font_bt[5] = {'font_bt1', 'font_bt2', 'font_bt3', 'font_bt4', 'font_bt5'};
#endif
    for (int i = 0; i < 5; i++) {
#if VERSION == VERSION_GCN_JPN
        static_cast<J2DTextBox*>(mpTitleScreen->search(cont_bt[i]))
            ->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpTitleScreen->search(cont_bt[i]))->setString(0x20, "");
        mpTitleScreen->search(font_bt[i])->hide();
#else
        static_cast<J2DTextBox*>(mpTitleScreen->search(font_bt[i]))
            ->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpTitleScreen->search(font_bt[i]))->setString(0x20, "");
        mpTitleScreen->search(cont_bt[i])->hide();
#endif
    }
    setBButtonString(0x522, 0xff);

    static const u64 cont_at[5] = {'cont_at', 'cont_at1', 'cont_at2', 'cont_at3', 'cont_at4'};
    static const u64 font_at[5] = {'font_at1', 'font_at2', 'font_at3', 'font_at4', 'font_at5'};
    for (int i = 0; i < 5; i++) {
#if VERSION == VERSION_GCN_JPN
        static_cast<J2DTextBox*>(mpTitleScreen->search(cont_at[i]))
            ->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpTitleScreen->search(cont_at[i]))->setString(0x20, "");
        mpTitleScreen->search(font_at[i])->hide();
#else
        static_cast<J2DTextBox*>(mpTitleScreen->search(font_at[i]))
            ->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpTitleScreen->search(font_at[i]))->setString(0x20, "");
        mpTitleScreen->search(cont_at[i])->hide();
#endif
    }
    setAButtonString(0x527, 0xff);

#if PLATFORM_GCN
    static const u64 juji_c[5] = {'juji_c00', 'juji_c01', 'juji_c02', 'juji_c03', 'juji_c04'};
    static const u64 fuji_c[5] = {'fuji_c00', 'fuji_c01', 'fuji_c02', 'fuji_c03', 'fuji_c04'};
    for (int i = 0; i < 5; i++) {
#if VERSION == VERSION_GCN_JPN
        static_cast<J2DTextBox*>(mpTitleScreen->search(juji_c[i]))
            ->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpTitleScreen->search(juji_c[i]))->setString(0x20, "");
        mpTitleScreen->search(fuji_c[i])->hide();
#else
        static_cast<J2DTextBox*>(mpTitleScreen->search(fuji_c[i]))
            ->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpTitleScreen->search(fuji_c[i]))->setString(0x20, "");
        mpTitleScreen->search(juji_c[i])->hide();
#endif
    }
    setCrossLRString(0x3f9);

    static const u64 ast_c[5] = {'ast_00', 'ast_01', 'ast_02', 'ast_03', 'ast_04'};
    static const u64 fst_c[5] = {'fst_00', 'fst_01', 'fst_02', 'fst_03', 'fst_04'};
    for (int i = 0; i < 5; i++) {
#if VERSION == VERSION_GCN_JPN
        static_cast<J2DTextBox*>(mpTitleScreen->search(ast_c[i]))
            ->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpTitleScreen->search(ast_c[i]))->setString(0x20, "");
        mpTitleScreen->search(fst_c[i])->hide();
#else
        static_cast<J2DTextBox*>(mpTitleScreen->search(fst_c[i]))
            ->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpTitleScreen->search(fst_c[i]))->setString(0x20, "");
        mpTitleScreen->search(ast_c[i])->hide();
#endif
    }
    set3DStickString(0x524);
#endif

    setHIO(true);
}

dMenu_Fmap2DTop_c::~dMenu_Fmap2DTop_c() {
    deleteExplain();
    delete mpTitleScreen;
    mpTitleScreen = NULL;
    delete mpTitleRoot;
    if (mpTitleRoot != NULL) {
    }
    delete mpButtonA;
    mpButtonA = NULL;
    delete mpButtonB;
    mpButtonB = NULL;
    delete mpButtonZ;
    mpButtonZ = NULL;
    delete mpButtonTextA;
    mpButtonTextA = NULL;
    delete mpButtonTextB;
    mpButtonTextB = NULL;
    delete mpButtonTextZ;
    mpButtonTextZ = NULL;
    if (mpDpadAlpha != NULL) {
        delete mpDpadAlpha;
        mpDpadAlpha = NULL;
    }
    if (mpAnalogStickAlpha != NULL) {
        delete mpAnalogStickAlpha;
        mpAnalogStickAlpha = NULL;
    }
    delete mpNamePane;
    mpNamePane = NULL;
    delete mpSubPane;
    mpSubPane = NULL;
    if (mpAnalogStick != NULL) {
        delete mpAnalogStick;
        mpAnalogStick = NULL;
    }
    if (mpDpad != NULL) {
        delete mpDpad;
        mpDpad = NULL;
    }
    delete mpContPane;
    mpContPane = NULL;
    for (int i = 0; i < 2; i++) {
        if (mpArrowLAlpha[i] != NULL) {
            delete mpArrowLAlpha[i];
            if (mpArrowLAlpha[i] != NULL) {
            }
        }
        if (mpArrowRAlpha[i] != NULL) {
            delete mpArrowRAlpha[i];
            if (mpArrowRAlpha[i] != NULL) {
            }
        }
    }
    delete mpAnm;
    mpAnm = NULL;
}

void dMenu_Fmap2DTop_c::_execute() {
    switch (mAlphaButtonA) {
    case ALPHA_MIN:
        setAlphaAnimeMax(mpButtonA);
        setAlphaAnimeMin(mpButtonTextA);
        break;
    case ALPHA_MID:
        setAlphaAnimeMid(mpButtonA);
        setAlphaAnimeMid(mpButtonTextA);
        break;
    default:
        setAlphaAnimeMax(mpButtonA);
        setAlphaAnimeMax(mpButtonTextA);
    }

    switch (mAlphaButtonB) {
    case ALPHA_MIN:
        setAlphaAnimeMax(mpButtonB);
        setAlphaAnimeMin(mpButtonTextB);
        break;
    case ALPHA_MID:
        setAlphaAnimeMid(mpButtonB);
        setAlphaAnimeMid(mpButtonTextB);
        break;
    default:
        setAlphaAnimeMax(mpButtonB);
        setAlphaAnimeMax(mpButtonTextB);
    }

    switch (mAlphaButtonZ) {
    case ALPHA_MIN:
        setAlphaAnimeMax(mpButtonZ);
        setAlphaAnimeMin(mpButtonTextZ);
        break;
    case ALPHA_MID:
        setAlphaAnimeMid(mpButtonZ);
        setAlphaAnimeMid(mpButtonTextZ);
        break;
    default:
        setAlphaAnimeMax(mpButtonZ);
        setAlphaAnimeMax(mpButtonTextZ);
    }

    if (mpDpadAlpha) {
        switch (mAlphaDpad) {
        case ALPHA_MIN:
            setAlphaAnimeMin(mpDpadAlpha);
            break;
        case ALPHA_MID:
            setAlphaAnimeMid(mpDpadAlpha);
            break;
        default:
            setAlphaAnimeMax(mpDpadAlpha);
            break;
        }
    }

    if (mpAnalogStickAlpha) {
        switch (mAlphaAnalogStick) {
        case ALPHA_MIN:
            setAlphaAnimeMin(mpAnalogStickAlpha);
            break;
        case ALPHA_MID:
            setAlphaAnimeMid(mpAnalogStickAlpha);
            break;
        default:
            setAlphaAnimeMax(mpAnalogStickAlpha);
            break;
        }
    }

    setHIO(false);
}

void dMenu_Fmap2DTop_c::setAllAlphaRate(f32 i_rate, bool i_init) {
    mAlphaRate = i_rate;
    if (i_init) {
        mpTitleRoot->setBackupAlpha();
    }
    mpTitleRoot->setAlphaMorfRate(i_rate);
}

void dMenu_Fmap2DTop_c::draw() {
    u32 scissor_left, scissor_top, scissor_width, scissor_height;
    J2DOrthoGraph* ctx = static_cast<J2DOrthoGraph*>(dComIfGp_getCurrentGrafPort());
    ctx->setup2D();
    GXGetScissor(&scissor_left, &scissor_top, &scissor_width, &scissor_height);
    ctx->scissor(mTransX, 0.0f, FB_WIDTH, FB_HEIGHT);
    ctx->setScissor();
    mpTitleScreen->draw(mTransX, mTransY, ctx);
    ctx->scissor(scissor_left, scissor_top, scissor_width, scissor_height);
    ctx->setScissor();
    if (mpScrnExplain) {
        mpScrnExplain->draw(ctx);
    }
}

void dMenu_Fmap2DTop_c::btkAnimeLoop(J2DAnmTextureSRTKey* i_anm, f32 i_delta) {
    if (i_anm) {
        mAnmFrame += i_delta;
        if (mAnmFrame >= i_anm->getFrameMax()) {
            mAnmFrame -= i_anm->getFrameMax();
        }
        i_anm->setFrame(mAnmFrame);
    } else {
        mAnmFrame = 0.0f;
    }

    mpTitleScreen->search('spot_0')->setAnimation(i_anm);
    mpTitleScreen->animation();
}

void dMenu_Fmap2DTop_c::setMoyaAlpha(u8 i_alpha) {
    mpTitleScreen->search('spot_0')->setAlpha(i_alpha);
}

void dMenu_Fmap2DTop_c::setTitleNameString(u32 param_0) {
#if VERSION == VERSION_GCN_JPN
    static const u64 sfont_name[7] = {
        'sfont00', 'sfontl0', 'sfontl1', 'sfontl2', 'sfontb0', 'sfontb1', 'sfontb2'
    };
#define setTitleNameString_font_name sfont_name
#elif PLATFORM_GCN
    static const u64 ffont_name[7] = {
        'ffont00', 'ffontl0', 'ffontl1', 'ffontl2', 'ffontb0', 'ffontb3', 'ffontb4'
    };
#define setTitleNameString_font_name ffont_name
#else
    static const u64 ffont_name[7] = {
        'ffont01', 'ffontl3', 'ffontl4', 'ffontl5', 'ffontb3', 'ffontb4', 'ffontb5'
    };
#define setTitleNameString_font_name ffont_name
#endif
    for (int i = 0; i < 7; i++) {
        if (param_0 == 0) {
            strcpy(((J2DTextBox*)(mpTitleScreen->search(setTitleNameString_font_name[i])))
                       ->getStringPtr(),
                   "");
        } else {
            dMeter2Info_getStringKanji(
                param_0,
                ((J2DTextBox*)(mpTitleScreen->search(setTitleNameString_font_name[i])))
                    ->getStringPtr(),
                NULL);
        }
    }
}

void dMenu_Fmap2DTop_c::setAreaNameString(u32 param_0) {
#if VERSION == VERSION_GCN_JPN
    static const u64 iarea_name[3] = {'i_name_s', 'i_name', 'i_name1'};
#define setAreaNameString_area_name iarea_name
#else
    static const u64 farea_name[3] = {'f_name_1', 'f_name3', 'f_name2'};
#define setAreaNameString_area_name farea_name
#endif
    for (int i = 0; i < 3; i++) {
        if (param_0 == 0) {
            strcpy(((J2DTextBox*)(mpTitleScreen->search(setAreaNameString_area_name[i])))
                       ->getStringPtr(),
                   "");
        } else {
            dMeter2Info_getStringKanji(
                param_0,
                ((J2DTextBox*)(mpTitleScreen->search(setAreaNameString_area_name[i])))
                    ->getStringPtr(),
                NULL);
        }
    }
}

void dMenu_Fmap2DTop_c::setZButtonString(u32 param_0, u8 i_alpha) {
    if (param_0 == 0x529 && ((daMidna_c*)daPy_py_c::getMidnaActor())->checkPortalObjRide()) {
        param_0 = 0x533;
    }

#if VERSION == VERSION_GCN_JPN
    static const u64 cont_zt[5] = {'cont_zt', 'cont_zt1', 'cont_zt2', 'cont_zt3', 'cont_zt4'};
    #define setZButtonString_font_zt cont_zt
#else
    static const u64 font_zt[5] = {'font_zt1', 'font_zt2', 'font_zt3', 'font_zt4', 'font_zt5'};
    #define setZButtonString_font_zt font_zt
#endif
    if (param_0 == 0 || !isWarpAccept()) {
#if PLATFORM_GCN
        mAlphaButtonZ = ALPHA_MID;
#else
        mAlphaButtonZ = ALPHA_MIN;
#endif
    } else {
        for (int i = 0; i < 5; i++) {
            dMeter2Info_getStringKanji(
                param_0,
                ((J2DTextBox*)(mpTitleScreen->search(setZButtonString_font_zt[i])))->getStringPtr(),
                NULL);
        }

        if (i_alpha == ALPHA_DEFAULT) {
            if (checkPlayerWarpAccept()) {
                mAlphaButtonZ = ALPHA_MAX;
            } else {
                mAlphaButtonZ = ALPHA_MID;
            }
        } else {
            mAlphaButtonZ = i_alpha;
        }
    }
}

void dMenu_Fmap2DTop_c::setBButtonString(u32 param_0, u8 i_alpha) {
#if VERSION == VERSION_GCN_JPN
    static const u64 cont_bt[5] = {'cont_bt1', 'cont_bt2', 'cont_bt3', 'cont_bt4', 'cont_bt'};
#define setBButtonString_font_bt cont_bt
#else
    static const u64 font_bt[5] = {'font_bt1', 'font_bt2', 'font_bt3', 'font_bt4', 'font_bt5'};
#define setBButtonString_font_bt font_bt
#endif
    if (param_0 == 0) {
        mAlphaButtonB = ALPHA_MIN;
    } else {
        for (int i = 0; i < 5; i++) {
            dMeter2Info_getStringKanji(
                param_0,
                ((J2DTextBox*)(mpTitleScreen->search(setBButtonString_font_bt[i])))->getStringPtr(),
                NULL);
        }

        if (i_alpha == ALPHA_DEFAULT) {
            mAlphaButtonB = ALPHA_MAX;
        } else {
            mAlphaButtonB = i_alpha;
        }
    }
}

void dMenu_Fmap2DTop_c::setAButtonString(u32 param_0, u8 i_alpha) {
#if VERSION == VERSION_GCN_JPN
    static const u64 cont_at[5] = {'cont_at', 'cont_at1', 'cont_at2', 'cont_at3', 'cont_at4'};
#define setAButtonString_font_at cont_at
#else
    static const u64 font_at[5] = {'font_at1', 'font_at2', 'font_at3', 'font_at4', 'font_at5'};
#define setAButtonString_font_at font_at
#endif
    if (param_0 == 0) {
        mAlphaButtonA = ALPHA_MIN;
    } else {
        for (int i = 0; i < 5; i++) {
            dMeter2Info_getStringKanji(
                param_0,
                ((J2DTextBox*)(mpTitleScreen->search(setAButtonString_font_at[i])))->getStringPtr(),
                NULL);
        }

        if (i_alpha == ALPHA_DEFAULT) {
            mAlphaButtonA = ALPHA_MAX;
        } else {
            mAlphaButtonA = i_alpha;
        }
    }
}

void dMenu_Fmap2DTop_c::setCrossLRString(u32 param_0) {
#if PLATFORM_GCN || (VERSION == VERSION_SHIELD)
#if VERSION == VERSION_GCN_JPN
    static const u64 juji_c[5] = {'juji_c00', 'juji_c01', 'juji_c02', 'juji_c03', 'juji_c04'};
#else
    static const u64 juji_c[5] = {'fuji_c00', 'fuji_c01', 'fuji_c02', 'fuji_c03', 'fuji_c04'};
#endif
    if (param_0 == 0) {
        for (int i = 0; i < 5; i++) {
            J2DTextBox* text_box = static_cast<J2DTextBox*>(mpTitleScreen->search(juji_c[i]));
            strcpy(text_box->getStringPtr(), "");
        }
        mpTitleScreen->search('juy_sha0')->show();
        mAlphaDpad = 1;
    } else {
        for (int i = 0; i < 5; i++) {
            J2DTextBox* text_box = static_cast<J2DTextBox*>(mpTitleScreen->search(juji_c[i]));
            dMeter2Info_getStringKanji(param_0, text_box->getStringPtr(), NULL);
        }
        mpTitleScreen->search('juy_sha0')->show();
        mAlphaDpad = 2;
    }
#endif
}

void dMenu_Fmap2DTop_c::set3DStickString(u32 param_0) {
#if PLATFORM_GCN || (VERSION == VERSION_SHIELD)
#if VERSION == VERSION_GCN_JPN
    static const u64 ast_c[5] = {'ast_00', 'ast_01', 'ast_02', 'ast_03', 'ast_04'};
#else
    static const u64 ast_c[5] = {'fst_00', 'fst_01', 'fst_02', 'fst_03', 'fst_04'};
#endif
    if (param_0 == 0) {
        for (int i = 0; i < 5; i++) {
            J2DTextBox* text_box = static_cast<J2DTextBox*>(mpTitleScreen->search(ast_c[i]));
            strcpy(text_box->getStringPtr(), "");
        }
        mpTitleScreen->search('as_sha0')->show();
        mAlphaAnalogStick = 1;
    } else {
        for (int i = 0; i < 5; i++) {
            J2DTextBox* text_box = static_cast<J2DTextBox*>(mpTitleScreen->search(ast_c[i]));
            dMeter2Info_getStringKanji(param_0, text_box->getStringPtr(), NULL);
        }
        mpTitleScreen->search('as_sha0')->show();
        mAlphaAnalogStick = 2;
    }
#endif
}

void dMenu_Fmap2DTop_c::createExplain(JKRExpHeap* i_heap, STControl* i_stick) {
    if (!mpScrnExplain) {
        JKRHeap* heap = mDoExt_setCurrentHeap(i_heap);
        mpScrnExplain = new dMsgScrnExplain_c(i_stick, 0, false, 1);
        JUT_ASSERT(4743, mpScrnExplain != NULL);
        mDoExt_setCurrentHeap(heap);
    }
}

void dMenu_Fmap2DTop_c::deleteExplain() {
    if (mpScrnExplain) {
        delete mpScrnExplain;
        mpScrnExplain = NULL;
    }
}

void dMenu_Fmap2DTop_c::setArrowAlphaRatio(u8 i_mask, f32 i_rate) {
    if (i_mask & ARROW_LEFT) {
        for (int i = 0; i < 2; i++) {
            if (mpArrowLAlpha[i]) {
                mpArrowLAlpha[i]->setAlphaRate(i_rate);
            }
        }
    }

    if (i_mask & ARROW_RIGHT) {
        for (int i = 0; i < 2; i++) {
            if (mpArrowRAlpha[i]) {
                mpArrowRAlpha[i]->setAlphaRate(i_rate);
            }
        }
    }
}

void dMenu_Fmap2DTop_c::setAlphaAnimeMin(CPaneMgrAlpha* i_pane) {
    s16 timer = i_pane->getAlphaTimer();
    if (timer > 0 || i_pane->getAlphaRate() != 0.0f) {
        timer--;
        if (timer < 0) {
            timer = 0;
        }
        i_pane->alphaAnimeStart(timer);
        i_pane->setAlphaRate(timer / 5.0f);
    }
}

void dMenu_Fmap2DTop_c::setAlphaAnimeMid(CPaneMgrAlpha* i_pane) {
    s16 timer = i_pane->getAlphaTimer();
    if (timer != 3 || i_pane->getAlphaRate() != 0.25f) {
        if (timer > 3) {
            timer--;
        } else if (timer < 3) {
            timer++;
        }
        i_pane->alphaAnimeStart(timer);
        i_pane->setAlphaRate(timer / 6.0f * 0.5f);
    }
}

void dMenu_Fmap2DTop_c::setAlphaAnimeMax(CPaneMgrAlpha* i_pane) {
    s16 timer = i_pane->getAlphaTimer();
    if (timer < 5 || i_pane->getAlphaRate() != 1.0f) {
        timer++;
        if (timer > 5) {
            timer = 5;
        }
        i_pane->alphaAnimeStart(timer);
        i_pane->setAlphaRate(timer / 5.0f);
    }
}

bool dMenu_Fmap2DTop_c::checkPlayerWarpAccept() {
    if (checkWarpAcceptCannon() == FALSE) {
        return FALSE;
    } else if (checkWarpAcceptRegion4() == FALSE) {
        return FALSE;
    } else {
        return daPy_getLinkPlayerActorClass()->checkAcceptDungeonWarpAlink(0);
    }
}

bool dMenu_Fmap2DTop_c::checkWarpAcceptRegion(int i_region) {
    if (g_fmapHIO.mAllRegionsUnlocked) {
        return true;
    }

    if (mpPortalBin) {
        dMenu_Fmap_portal_data_c* portal_data = (dMenu_Fmap_portal_data_c*)mpPortalBin;
        dMenu_Fmap_portal_data_c::data* portals = portal_data->mData;
        for (int i = 0; i < portal_data->mCount; i++) {
            if (i_region + 1 == portals[i].mRegionNo
                && dComIfGs_isStageSwitch(portals[i].mStageNo, portals[i].mSwitchNo))
            {
                return true;
            }
        }
    }

    return false;
}

bool dMenu_Fmap2DTop_c::checkWarpAcceptRegion4() {
    if (dComIfGp_getNowLevel() == 4 && !checkWarpAcceptRegion(3)) {
        return false;
    }

        /* dSv_event_flag_c::M_071 - Cutscene - [cutscene: 20] Zant appears (during Midna's desperate hour) */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[104])
            /* dSv_event_flag_c::F_0250 - Cutscene - [cutscene: 21] reunion with Zelda / Midna revived (Hyrule Castle barrier appears) */
        && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[250]))
    {
        return false;
    }

    return true;
}

bool dMenu_Fmap2DTop_c::checkWarpAcceptCannon() {
        /* dSv_event_flag_c::F_0786 - Kakariko Village - Forced conversation with Shad when he sees the sky cannon */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[786])
            /* dSv_event_flag_c::F_0785 - Kakariko Village - Shad leaves after attempting to warp sky cannon */
        && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[785]))
    {
        return false;
    }

    return true;
}

void dMenu_Fmap2DTop_c::setHIO(bool param_0) {
    if (mTitlePosX != g_fmapHIO.mTitlePosX || mTitlePosY != g_fmapHIO.mTitlePosY) {
        mTitlePosX = g_fmapHIO.mTitlePosX;
        mTitlePosY = g_fmapHIO.mTitlePosY;
        mpNamePane->paneTrans(mTitlePosX, mTitlePosY);
    }

    if (mTitleScale != g_fmapHIO.mTitleScale) {
        mTitleScale = g_fmapHIO.mTitleScale;
        mpNamePane->scale(mTitleScale, mTitleScale);
    }

    if (mSubTitlePosX != g_fmapHIO.mSubTitlePosX || mSubTitlePosY != g_fmapHIO.mSubTitlePosY) {
        mSubTitlePosX = g_fmapHIO.mSubTitlePosX;
        mSubTitlePosY = g_fmapHIO.mSubTitlePosY;
        mpSubPane->paneTrans(mSubTitlePosX, mSubTitlePosY);
    }

    if (mSubTitleScale != g_fmapHIO.mSubTitleScale) {
        mSubTitleScale = g_fmapHIO.mSubTitleScale;
        mpSubPane->scale(mSubTitleScale, mSubTitleScale);
    }

    if (mpAnalogStick) {
        if (mAreaMovementPosX != g_fmapHIO.mAreaMovementPosX
            || mAreaMovementPosY != g_fmapHIO.mAreaMovementPosY)
        {
            mAreaMovementPosX = g_fmapHIO.mAreaMovementPosX;
            mAreaMovementPosY = g_fmapHIO.mAreaMovementPosY;
            mpAnalogStick->paneTrans(mAreaMovementPosX, mAreaMovementPosY);
        }

        if (mAreaMovementScale != g_fmapHIO.mAreaMovementScale) {
            mAreaMovementScale = g_fmapHIO.mAreaMovementScale;
            mpAnalogStick->scale(mAreaMovementScale, mAreaMovementScale);
        }
    }

    if (mpDpad) {
        if (mClosingPosX != g_fmapHIO.mClosingPosX || mClosingPosY != g_fmapHIO.mClosingPosY) {
            mClosingPosX = g_fmapHIO.mClosingPosX;
            mClosingPosY = g_fmapHIO.mClosingPosY;
            mpDpad->paneTrans(mClosingPosX, mClosingPosY);
        }

        if (mClosingScale != g_fmapHIO.mClosingScale) {
            mClosingScale = g_fmapHIO.mClosingScale;
            mpDpad->scale(mClosingScale, mClosingScale);
        }
    }

    if (mDoIconPosX != g_fmapHIO.mDoIconPosX || mDoIconPosY != g_fmapHIO.mDoIconPosY) {
        mDoIconPosX = g_fmapHIO.mDoIconPosX;
        mDoIconPosY = g_fmapHIO.mDoIconPosY;
        mpContPane->paneTrans(mDoIconPosX, mDoIconPosY);
    }

    if (mDoIconScale != g_fmapHIO.mDoIconScale) {
        mDoIconScale = g_fmapHIO.mDoIconScale;
        mpContPane->scale(mDoIconScale, mDoIconScale);
    }

    if (g_fmapHIO.mButtonDebugON || param_0) {
        mpButtonZ->paneTrans(g_fmapHIO.mButtonZPosX, g_fmapHIO.mButtonZPosY);
        mpButtonZ->scale(g_fmapHIO.mButtonZScale, g_fmapHIO.mButtonZScale);
        mpButtonA->paneTrans(g_fmapHIO.mButtonAPosX, g_fmapHIO.mButtonAPosY);
        mpButtonA->scale(g_fmapHIO.mButtonAScale, g_fmapHIO.mButtonAScale);
        mpButtonB->paneTrans(g_fmapHIO.mButtonBPosX, g_fmapHIO.mButtonBPosY);
        mpButtonB->scale(g_fmapHIO.mButtonBScale, g_fmapHIO.mButtonBScale);
        mpButtonTextZ->paneTrans(g_fmapHIO.mButtonZTextPosX, g_fmapHIO.mButtonZTextPosY);
        mpButtonTextZ->scale(g_fmapHIO.mButtonZTextScale, g_fmapHIO.mButtonZTextScale);
        mpButtonTextA->paneTrans(g_fmapHIO.mButtonATextPosX, g_fmapHIO.mButtonATextPosY);
        mpButtonTextA->scale(g_fmapHIO.mButtonATextScale, g_fmapHIO.mButtonATextScale);
        mpButtonTextB->paneTrans(g_fmapHIO.mButtonBTextPosX, g_fmapHIO.mButtonBTextPosY);
        mpButtonTextB->scale(g_fmapHIO.mButtonBTextScale, g_fmapHIO.mButtonBTextScale);
    }
}

bool dMenu_Fmap2DTop_c::isWarpAccept() {
         /* dSv_event_flag_c::M_021 - Main Event - First portal warp */
    if ((dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[54])
        && dMeter2Info_getMapStatus() != 9 && dMeter2Info_getMapStatus() != 7
        && dMeter2Info_getMapStatus() != 8) || g_fmapHIO.mPortalWarpON)
    {
        return true;
    }
    return false;
}
