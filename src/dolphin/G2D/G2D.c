#include <dolphin/dolphin.h>
#include <dolphin/gx.h>
#include <dolphin/G2D.h>
#include "sdk_math.h"

static G2DGlob glob;

void G2DInitSprite(G2DSprite *sprite) {
    f32 rInvWidth;
    f32 rInvHeight;

    rInvWidth = 1.0f / (f32)GXGetTexObjWidth(sprite->to);
    rInvHeight = 1.0f / (f32)GXGetTexObjHeight(sprite->to);
    sprite->rS0 = (0.5f + (f32)sprite->nTlcS) * rInvWidth;
    sprite->rS1 = rInvWidth * (((f32)sprite->nTlcS + (f32)sprite->nWidth) - 0.5f);
    sprite->rT0 = (0.5f + (f32)sprite->nTlcT) * rInvHeight;
    sprite->rT1 = rInvHeight * (((f32)sprite->nTlcT + (f32)sprite->nHeight) - 0.5f);
}

void G2DDrawSprite(G2DSprite* sprite, G2DPosOri* po) {
    f32 rOX, rOY;
    f32 rWX, rWY;
    f32 rHX, rHY;
    f32 rRelX, rRelY;
    
    GXClearVtxDesc();
    GXLoadTexObj(sprite->to, GX_TEXMAP0);
    
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    rOX = 0.5f * po->rOriX;
    rOY = 0.5f * po->rOriY;

    rWX = (f32)sprite->nWidth * rOX;
    rWY = (f32)sprite->nWidth * rOY;
    rHX = (f32)sprite->nHeight * rOX;
    rHY = (f32)sprite->nHeight * rOY;

    rRelX = po->rPosX - glob.poCam.rPosX;
    rRelY = po->rPosY - glob.poCam.rPosY;
    if (rRelX >= glob.rHalfX) {
        rRelX -= glob.rWorldX;
    }
    if (rRelX < -glob.rHalfX) {
        rRelX += glob.rWorldX;
    }
    if (rRelY >= glob.rHalfY) {
        rRelY -= glob.rWorldY;
    }
    if (rRelY < -glob.rHalfY) {
        rRelY += glob.rWorldY;
    }
    
    rRelX += glob.poCam.rPosX;
    rRelY += glob.poCam.rPosY;
    
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    
    GXPosition2f32(rWY + (rRelX - rHX), (rRelY - rHY) - rWX);
    GXTexCoord2f32(sprite->rS0, sprite->rT1);
    
    GXPosition2f32(rWY + (rRelX + rHX), (rRelY + rHY) - rWX);
    GXTexCoord2f32(sprite->rS0, sprite->rT0);
    
    GXPosition2f32((rRelX + rHX) - rWY, rWX + (rRelY + rHY));
    GXTexCoord2f32(sprite->rS1, sprite->rT0);
    
    GXPosition2f32((rRelX - rHX) - rWY, rWX + (rRelY - rHY));
    GXTexCoord2f32(sprite->rS1, sprite->rT1);
    
    GXEnd();
}

static inline void FillSection(G2DLayer* layer, s8* aSortBuffer, s32* nScanLine, s32 nEvent, s16* nIdx,
                               s32* nL, s32* nR, f32* rLeft, f32* rRight, f32 rStep0, f32 rStep1, s32 nMapX,
                               s32 nMapY) {
    s32 nHMask;
    s32 nVMask;
    s32 nI;
    s32 nJ;
    s32 nK;
    s32 nM;
    s16 nMaterial;
    s16* pAddr;

    nHMask = (1 << layer->nHS) - 1;
    nVMask = (1 << layer->nVS) - 1;
    if (layer->nBPI == 1) {
        u8 nTile;
        if (layer->bWrap) {
            for (; *nScanLine <= nEvent; (*nScanLine)++) {
                nJ = *nScanLine - 1;
                nK = (nVMask & (nJ + nMapY)) << layer->nHS;
                for (nI = *nL; nI <= *nR; nI++) {
                    nTile = ((u8*)layer->map)[nK + (nHMask & (nI + nMapX))];
                    nMaterial = layer->tileDesc[nTile].nMaterial;
                    pAddr = (s16*)&layer->matDesc[nMaterial].nReserved;
                    pAddr[1]++;

                    if (*pAddr != *nIdx) {
                        *nIdx += 2;
                        *(s16*)&aSortBuffer[*pAddr] = *pAddr - *nIdx;
                    }
                    *(s16*)&aSortBuffer[*nIdx] = nTile;
                    *nIdx += 2;
                    aSortBuffer[(*nIdx)++] = nI;
                    aSortBuffer[(*nIdx)++] = nJ;
                    *pAddr = *nIdx;
                }
                *rLeft += rStep0;
                *rRight += rStep1;
                *nL = floor(*rLeft);
                *nR = floor(*rRight);
            }
        } else {
            for (; *nScanLine <= nEvent; (*nScanLine)++) {
                nJ = *nScanLine - 1 + nMapY;
                if (nJ < 0) {
                    nK = 0;
                }
                else if ( nJ > nVMask ) {
                    nK = nVMask << layer->nHS;
                }
                else {
                    nK = nJ << layer->nHS;
                }

                nM = *nR + nMapX;
                for (nI = *nL + nMapX; nI <= nM; nI++) {
                    if ( nI < 0 ) {
                        nTile = ((u8*)layer->map)[nK];
                    }
                    else if ( nI > nHMask ) {
                        nTile = ((u8*)layer->map)[nK + nHMask];
                    }
                    else {
                        nTile = ((u8*)layer->map)[nK + nI];
                    }

                    nMaterial = layer->tileDesc[nTile].nMaterial;
                    pAddr = (s16*)&layer->matDesc[nMaterial].nReserved;
                    pAddr[1]++;

                    if (*pAddr != *nIdx) {
                        *nIdx += 2;
                        *(s16*)&aSortBuffer[*pAddr] = *pAddr - *nIdx;
                    }
                    *(s16*)&aSortBuffer[*nIdx] = nTile;
                    *nIdx += 2;
                    aSortBuffer[(*nIdx)++] = nI - nMapX;
                    aSortBuffer[(*nIdx)++] = nJ - nMapY;
                    *pAddr = *nIdx;
                }

                *rLeft += rStep0;
                *rRight += rStep1;
                *nL = floor(*rLeft);
                *nR = floor(*rRight);
            }
        }
    } else {
        u16 nTile;
        if (layer->bWrap) {
            for (; *nScanLine <= nEvent; (*nScanLine)++) {
                nJ = *nScanLine - 1;
                nK = (nVMask & (nJ + nMapY)) << layer->nHS;
                for (nI = *nL; nI <= *nR; nI++) {
                    nTile = ((u16*)layer->map)[nK + (nHMask & (nI + nMapX))];
                    nMaterial = layer->tileDesc[nTile].nMaterial;
                    pAddr = (s16*)&layer->matDesc[nMaterial].nReserved;
                    pAddr[1]++;

                    if (*pAddr != *nIdx) {
                        *nIdx += 2;
                        *(s16*)&aSortBuffer[*pAddr] = *pAddr - *nIdx;
                    }
                    *(s16*)&aSortBuffer[*nIdx] = nTile;
                    *nIdx += 2;
                    aSortBuffer[(*nIdx)++] = nI;
                    aSortBuffer[(*nIdx)++] = nJ;
                    *pAddr = *nIdx;
                }

                *rLeft += rStep0;
                *rRight += rStep1;
                *nL = floor(*rLeft);
                *nR = floor(*rRight);
            }
        } else {
            for (; *nScanLine <= nEvent; (*nScanLine)++) {
                nJ = *nScanLine - 1 + nMapY;
                if (nJ < 0) {
                    nK = 0;
                }
                else if ( nJ > nVMask ) {
                    nK = nVMask << layer->nHS;
                }
                else {
                    nK = nJ << layer->nHS;
                }

                nM = *nR + nMapX;
                for (nI = *nL + nMapX; nI <= nM; nI++) {
                    if ( nI < 0 ) {
                        nTile = ((u16*)layer->map)[nK];
                    }
                    else if ( nI > nHMask ) {
                        nTile = ((u16*)layer->map)[nK + nHMask];
                    }
                    else {
                        nTile = ((u16*)layer->map)[nK + nI];
                    }

                    nMaterial = layer->tileDesc[nTile].nMaterial;
                    pAddr = (s16*)&layer->matDesc[nMaterial].nReserved;
                    pAddr[1]++;

                    if (*pAddr != *nIdx) {
                        *nIdx += 2;
                        *(s16*)&aSortBuffer[*pAddr] = *pAddr - *nIdx;
                    }
                    *(s16*)&aSortBuffer[*nIdx] = nTile;
                    *nIdx += 2;
                    aSortBuffer[(*nIdx)++] = nI - nMapX;
                    aSortBuffer[(*nIdx)++] = nJ - nMapY;
                    *pAddr = *nIdx;
                }

                *rLeft += rStep0;
                *rRight += rStep1;
                *nL = floor(*rLeft);
                *nR = floor(*rRight);
            }
        }
    }
}

void G2DDrawLayer(G2DLayer* layer, s8* aSortBuffer) {
    s16* pAddr;
    s16 aCount0;
    s16 aCount1;
    s16 aCount2;
    f32 rInvTileWidth;
    f32 rInvTileHeight;
    s16 nIdx;
    s32 nI;
    s32 nJ;
    s32 nK;
    s32 nL;
    s32 nR;
    f32 rX;
    f32 rY;
    f32 rTlcX;
    f32 rTrcX;
    f32 rBlcX;
    f32 rBrcX;
    f32 rTlcY;
    f32 rTrcY;
    f32 rBlcY;
    f32 rBrcY;
    s32 nScanLine;
    f32 rLeft;
    f32 rRight;
    f32 rLeftY;
    f32 rRightY;
    f32 rStep0;
    f32 rStep1;
    f32 rMid;
    s32 nEvent0;
    s32 nEvent1;
    s32 nEvent2;
    f32 rFrcX;
    f32 rFrcY;
    s32 nMapX;
    s32 nMapY;
    s32 nLocalMapX;
    s32 nLocalMapY;
    f32 rCamOriX;
    f32 rCamOriY;
    s16 nTile;
    s16 nMaterial;
    f32 rRatio;

    aCount0 = 0;
    aCount1 = 0;
    aCount2 = 0;
    rInvTileWidth = 1.0f / (f32)layer->nTileWidth;
    rInvTileHeight = 1.0f / (f32)layer->nTileHeight;

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    for (nI = 0; nI < layer->nNumMaterials; nI++) {
        pAddr = (s16*)&layer->matDesc[nI];
        pAddr[0] = nI << 1;
        pAddr[1] = 0;
    }
    nIdx = (nI - 1) << 1;

    rFrcX = glob.poCam.rPosX * rInvTileWidth;
    rFrcY = glob.poCam.rPosY * rInvTileHeight;
    nMapX = (s32)rFrcX;
    nMapY = (s32)rFrcY;
    rFrcX -= nMapX;
    rFrcY -= nMapY;

    rCamOriX = glob.poCam.rOriX;
    rCamOriY = glob.poCam.rOriY;
    if (rCamOriX < 0.0001 && rCamOriX > -0.0001) {
        rCamOriX = 0.0001f;
    }
    else if (rCamOriY < 0.0001 && rCamOriY > -0.0001) {
        rCamOriY = 0.0001f;
    }

    rX = 0.5f * (f32)glob.nViewportWidth;
    rY = 0.5f * (f32)glob.nViewportHeight;
    rTlcX = rFrcX + (rInvTileWidth * ((rY * rCamOriX) + (rX * rCamOriY)));
    rTlcY = (rFrcY + (rInvTileHeight * ((rY * rCamOriY) - (rX * rCamOriX))));
    rTrcX = rFrcX + (rInvTileWidth * ((rY * rCamOriX) - (rX * rCamOriY)));
    rTrcY = (rFrcY + (rInvTileHeight * ((rY * rCamOriY) + (rX * rCamOriX))));
    rBlcX = rFrcX - (rInvTileWidth * ((rY * rCamOriX) - (rX * rCamOriY)));
    rBlcY = rFrcY - (rInvTileHeight * ((rY * rCamOriY) + (rX * rCamOriX)));
    rBrcX = rFrcX - (rInvTileWidth * ((rY * rCamOriX) + (rX * rCamOriY)));
    rBrcY = rFrcY - (rInvTileHeight * ((rY * rCamOriY) - (rX * rCamOriX)));

    if (rCamOriY < 0.0f) {
        if (rCamOriX >= 0.0f) {
            nScanLine = (s32)floor(rTlcY) + 1;
            rY = nScanLine - rTlcY;
            rLeft = rTlcX;
            rLeftY = rBlcY;
            rRightY = rTrcY;
            nEvent2 = (s32)floor(rBrcY);
            rStep0 = rCamOriX / rCamOriY;
            rStep1 = -rCamOriY / rCamOriX;
        }
        else {
            nScanLine = (s32)floor(rTrcY) + 1;
            rY = nScanLine - rTrcY;
            rLeft = rTrcX;
            rLeftY = rTlcY;
            rRightY = rBrcY;
            nEvent2 = (s32)floor(rBlcY);
            rStep0 = -rCamOriY / rCamOriX;
            rStep1 = rCamOriX / rCamOriY;
        }
    } else {
        if (rCamOriX >= 0.0f) {
            nScanLine = (s32)floor(rBlcY) + 1;
            rY = nScanLine - rBlcY;
            rLeft = rBlcX;
            rLeftY = rBrcY;
            rRightY = rTlcY;
            nEvent2 = (s32)floor(rTrcY);
            rStep0 = -rCamOriY / rCamOriX;
            rStep1 = rCamOriX / rCamOriY;
        } else {
            nScanLine = (s32)floor(rBrcY) + 1;
            rY = nScanLine - rBrcY;
            rLeft = rBrcX;
            rLeftY = rTrcY;
            rRightY = rBlcY;
            nEvent2 = (s32)floor(rTlcY);
            rStep0 = rCamOriX / rCamOriY;
            rStep1 = -rCamOriY / rCamOriX;
        }
    }

    rRatio = (f32)layer->nTileHeight / (f32)layer->nTileWidth;
    rStep0 *= rRatio;
    rStep1 *= rRatio;
    rRight = rLeft + (rY * rStep1);
    rLeft = rLeft + (rY * rStep0);

    if (rLeftY < rRightY) {
        nEvent0 = (s32)floor(rLeftY);
        nEvent1 = (s32)floor(rRightY);
        rMid = rStep1;
    } else {
        nEvent0 = (s32)floor(rRightY);
        nEvent1 = (s32)floor(rLeftY);
        rMid = rStep0;
    }

    nL = (s32)floor(rLeft);
    nR = (s32)floor(rRight);
    nLocalMapX = nMapX;
    nLocalMapY = nMapY;

    if (!layer->bWrap) {
        f32 rInvTileWidth = 1.0f / (f32)layer->nTileWidth;
        f32 rInvTileHeight = 1.0f / (f32)layer->nTileHeight;
        f32 rLocalPosX = glob.poCam.rPosX;
        f32 rLocalPosY = glob.poCam.rPosY;
        f32 rSplitX = 0.5f * (glob.rWorldX + (f32)(layer->nTileWidth * (1 << layer->nHS)));
        f32 rSplitY = 0.5f * (glob.rWorldY + (f32)(layer->nTileHeight * (1 << layer->nVS)));
        
        if (rLocalPosX >= rSplitX) {
            rLocalPosX -= glob.rWorldX;
        }
        
        if (rLocalPosY >= rSplitY) {
            rLocalPosY -= glob.rWorldY;
        }

        rFrcX = rLocalPosX * rInvTileWidth;
        rFrcY = rLocalPosY * rInvTileHeight;
        
        nLocalMapX = floor(rFrcX);
        nLocalMapY = floor(rFrcY);
    }

    FillSection(layer, aSortBuffer, &nScanLine, nEvent0, &nIdx, &nL, &nR, &rLeft, &rRight, rStep0, rStep1, nLocalMapX, nLocalMapY);

    pAddr = (s16*)&layer->matDesc[0].nReserved;
    aCount0 = pAddr[1];
    pAddr = (s16*)&layer->matDesc[1].nReserved;
    aCount1 = pAddr[1];
    pAddr = (s16*)&layer->matDesc[2].nReserved;
    aCount2 = pAddr[1];

    if ((f32)nScanLine > rLeftY) {
        rLeft -= rStep0 * ((f32)nScanLine - rLeftY);
        nL = (s32)floor(rLeft);
        rLeft += rStep1 * (((f32)nScanLine - rLeftY) - 1.0f);
        rLeftY = 1000.0f;
    }

    if ((f32)nScanLine > rRightY) {
        rRight -= rStep1 * ((f32)nScanLine - rRightY);
        nR = (s32)floor(rRight);
        rRight += rStep0 * (((f32)nScanLine - rRightY) - 1.0f);
        rRightY = 1000.0f;
    }

    FillSection(layer, aSortBuffer, &nScanLine, nEvent1, &nIdx, &nL, &nR, &rLeft, &rRight, rMid, rMid, nLocalMapX, nLocalMapY);

    pAddr = (s16*)&layer->matDesc[0].nReserved;
    aCount0 = pAddr[1];
    pAddr = (s16*)&layer->matDesc[1].nReserved;
    aCount1 = pAddr[1];
    pAddr = (s16*)&layer->matDesc[2].nReserved;
    aCount2 = pAddr[1];

    if ((f32)nScanLine > rLeftY) {
        rLeft -= rStep0 * ((f32)nScanLine - rLeftY);
        nL = (s32)floor(rLeft);
        rLeft += rStep1 * (((f32)nScanLine - rLeftY) - 1.0f);
    }

    if ((f32)nScanLine > rRightY) {
        rRight -= rStep1 * ((f32)nScanLine - rRightY);
        nR = (s32)floor(rRight);
        rRight += rStep0 * (((f32)nScanLine - rRightY) - 1.0f);
    }

    FillSection(layer, aSortBuffer, &nScanLine, nEvent2 + 1, &nIdx, &nL, &nR, &rLeft, &rRight, rStep1, rStep0, nLocalMapX, nLocalMapY);

    pAddr = (s16*)&layer->matDesc[0].nReserved;
    aCount0 = pAddr[1];
    pAddr = (s16*)&layer->matDesc[1].nReserved;
    aCount1 = pAddr[1];
    pAddr = (s16*)&layer->matDesc[2].nReserved;
    aCount2 = pAddr[1];

    for (nMaterial = 0; nMaterial < layer->nNumMaterials; nMaterial++) {
        pAddr = (s16*)&layer->matDesc[nMaterial].nReserved;
        if (!pAddr[1]) {
            continue;
        }
        
        switch (layer->matDesc[nMaterial].nCategory) {
            case G2D_CTG_EMPTY:
                continue;
            
            case G2D_CTG_RGBA_INDEX8: {
                GXClearVtxDesc();
                
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
                
                GXSetNumTexGens(0);
                GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
                GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
                GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
                GXSetArray(GX_VA_CLR0, layer->matDesc[nMaterial].clut, 4);
                
                GXSetNumChans(1);
                GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_VTX, GX_SRC_VTX, 1, GX_DF_NONE, GX_AF_NONE);
                
                nIdx = nMaterial << 1;
                GXBegin(GX_QUADS, GX_VTXFMT0, pAddr[1] << 2);

                for (nK = pAddr[1]; nK--; ) {
                    f32 rI, rJ;
                    u8 nCI;
                    
                    nTile = *((s16*)&aSortBuffer[nIdx]);
                    if (nTile < 0) {
                        nIdx -= nTile;
                        nTile = *((s16*)&aSortBuffer[nIdx]);
                    }
                    nIdx += 2;
                    nI = aSortBuffer[nIdx++];
                    nJ = aSortBuffer[nIdx++];

                    rI = (f32)layer->nTileWidth * (f32)(nI + nMapX);
                    rJ = (f32)layer->nTileHeight * (f32)(nJ + nMapY);
                    nCI = layer->tileDesc[nTile].nCI;

                    GXPosition2f32(rI + (f32)layer->nTileWidth, rJ);
                    GXColor1x8(nCI);

                    GXPosition2f32(rI + (f32)layer->nTileWidth, rJ + (f32)layer->nTileHeight);
                    GXColor1x8(nCI);

                    GXPosition2f32(rI, rJ + (f32)layer->nTileHeight);
                    GXColor1x8(nCI);

                    GXPosition2f32(rI, rJ);
                    GXColor1x8(nCI);
                }
                
                GXEnd();
                break;
            }

            case G2D_CTG_RGB_DIRECT: {
                GXClearVtxDesc();
                
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
                
                GXSetNumTexGens(0);
                GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
                GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0);
                GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
                
                GXSetNumChans(1);
                GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_VTX, GX_SRC_VTX, 1, GX_DF_NONE, GX_AF_NONE);
                
                nIdx = nMaterial << 1;
                GXBegin(GX_QUADS, GX_VTXFMT0, pAddr[1] << 2);

                for (nK = pAddr[1]; nK--; ) {
                    f32 rI, rJ;
                    u8 nR, nG, nB;
                    
                    nTile = *((s16*)&aSortBuffer[nIdx]);
                    if (nTile < 0) {
                        nIdx -= nTile;
                        nTile = *((s16*)&aSortBuffer[nIdx]);
                    }
                    
                    nIdx += 2;
                    nI = aSortBuffer[nIdx++];
                    nJ = aSortBuffer[nIdx++];

                    rI = (f32)layer->nTileWidth * (f32)(nI + nMapX);
                    rJ = (f32)layer->nTileHeight * (f32)(nJ + nMapY);
                    nR = layer->tileDesc[nTile].nS;
                    nG = layer->tileDesc[nTile].nT;
                    nB = layer->tileDesc[nTile].nCI;

                    GXPosition2f32(rI + (f32)layer->nTileWidth, rJ);
                    GXColor3u8(nR, nG, nB);
                    
                    GXPosition2f32(rI + (f32)layer->nTileWidth, rJ + (f32)layer->nTileHeight);
                    GXColor3u8(nR, nG, nB);
                    
                    GXPosition2f32(rI, rJ + (f32)layer->nTileHeight);
                    GXColor3u8(nR, nG, nB);
                    
                    GXPosition2f32(rI, rJ);
                    GXColor3u8(nR, nG, nB);
                }
                
                GXEnd();
                break;
            }

            case G2D_CTG_TEXTURE: {
                f32 rInvTileWidth = 1.0f / (f32)GXGetTexObjWidth(layer->matDesc[nMaterial].to);
                f32 rInvTileHeight = 1.0f / (f32)GXGetTexObjHeight(layer->matDesc[nMaterial].to);
                
                f32 rWidth = (f32)layer->nTileWidth * rInvTileWidth;
                f32 rHeight = (f32)layer->nTileHeight * rInvTileHeight;
                
                f32 rS0 = 0.0f;
                f32 rT0 = 0.0f;
                f32 rS1 = rWidth;
                f32 rT1 = rHeight;

                GXClearVtxDesc();
                
                GXLoadTexObj(layer->matDesc[nMaterial].to, GX_TEXMAP0);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
                
                GXSetNumTexGens(1);
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
                GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

                if ( layer->matDesc[nMaterial].color ) {
                    GXSetNumChans(1);
                    GXSetChanMatColor(GX_COLOR0A0, *layer->matDesc[nMaterial].color);
                    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, 1, GX_DF_NONE, GX_AF_NONE);
                    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
                    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
                } else {
                    GXSetNumChans(0);
                    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
                    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                }

                nIdx = nMaterial << 1;
                GXBegin(GX_QUADS, GX_VTXFMT0, pAddr[1] << 2);

                for (nK = pAddr[1]; nK--; ) {
                    f32 rI;
                    f32 rJ;
                    f32 rS;
                    f32 rT;
                    
                    nTile = *((s16*)&aSortBuffer[nIdx]);
                    if (nTile < 0) {
                        nIdx -= nTile;
                        nTile = *((s16*)&aSortBuffer[nIdx]);
                    }

                    nIdx += 2;
                    nI = aSortBuffer[nIdx++];
                    nJ = aSortBuffer[nIdx++];

                    rS = rS0 + (rWidth * (f32)layer->tileDesc[nTile].nS);
                    rT = rT0 + (rHeight * (f32)layer->tileDesc[nTile].nT);
                    rI = (f32)layer->nTileWidth * (f32)(nI + nMapX);
                    rJ = (f32)layer->nTileHeight * (f32)(nJ + nMapY);
                    
                    GXPosition2f32(rI + (f32)layer->nTileWidth, rJ);
                    GXTexCoord2f32(rS + rS1, rT);
                    
                    GXPosition2f32(rI + (f32)layer->nTileWidth, rJ + (f32)layer->nTileHeight);
                    GXTexCoord2f32(rS + rS1, rT + rT1);
                    
                    GXPosition2f32(rI, rJ + (f32)layer->nTileHeight);
                    GXTexCoord2f32(rS, rT + rT1);
                    
                    GXPosition2f32(rI, rJ);
                    GXTexCoord2f32(rS, rT);
                }
                
                GXEnd();
                break;
            }
        }
    }
}

void G2DSetCamera(G2DPosOri* po) {
    Mtx mView;
    Vec vPos;
    Vec vUp;
    Vec vAt;
    f32 rX;
    f32 rY;
    
    glob.poCam = *po;
    
    vUp.x = po->rOriX;
    vUp.y = po->rOriY;
    vUp.z = 0.0f;
    
    rX = (((640 - glob.nViewportWidth) >> 1) - glob.nViewportTlcX);
    rY = (((448 - glob.nViewportHeight) >> 1) - glob.nViewportTlcY);
    
    vPos.x = po->rPosX - (vUp.x * rY) - (vUp.y * rX);
    vPos.y = po->rPosY + (vUp.x * rX) - (vUp.y * rY);
    vPos.z = -300.0f;

    vAt.x = vPos.x;
    vAt.y = vPos.y;
    vAt.z = 0.0f;

    MTXLookAt(mView, &vPos, &vUp, &vAt);
    GXLoadPosMtxImm(mView, GX_PNMTX0);
}

void G2DInitWorld(u32 nWorldX, u32 nWorldY) {
    Mtx44 mProjection;
    
    glob.rWorldX = (f32)nWorldX;
    glob.rWorldY = (f32)nWorldY;
    glob.rHalfX = (f32)(nWorldX >> 1);
    glob.rHalfY = (f32)(nWorldY >> 1);
    GXSetZMode(0, GX_ALWAYS, 1);
    MTXOrtho(mProjection, 224.0f, -224.0f, -320.0f, 320.0f, 100.0f, 1000.0f);
    GXSetProjection(mProjection, GX_ORTHOGRAPHIC);
}

void G2DSetViewport(u16 nLeft, u16 nTop, u16 nWidth, u16 nHeight) {
    glob.nViewportTlcX = nLeft;
    glob.nViewportTlcY = nTop;
    glob.nViewportWidth = nWidth;
    glob.nViewportHeight = nHeight;
    GXSetScissor(nLeft, nTop, nWidth, nHeight);
}
