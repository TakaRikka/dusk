#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_debug_viewer.h"
#include "d/d_debug_pad.h"
#include "d/d_com_inf_game.h"
#include "f_ap/f_ap_game.h"
#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JUtility/JUTReport.h"

J3DPacket* l_drawPacketList[1000];
int l_drawPacketListNum;

J3DPacket* dDbVw_setDrawPacketList(J3DPacket* i_packet, int i_bufferType) {
    if (i_packet == NULL) {
        return NULL;
    }

    if (l_drawPacketListNum >= 1000) {
        delete (void*)i_packet;
        return NULL;
    }

    l_drawPacketList[l_drawPacketListNum++] = i_packet;
    j3dSys.getDrawBuffer(i_bufferType)->entryImm(i_packet, 0);
    return i_packet;
}

void dDbVw_deleteDrawPacketList() {
    for (int i = 0; i < l_drawPacketListNum; i++) {
        delete (void*)l_drawPacketList[i];
    }

    l_drawPacketListNum = 0;
}

void dDbVw_drawCube8p(int i_bufferType, cXyz* i_points, const GXColor& i_color) {
    mDoExt_cube8pPacket* packet = new mDoExt_cube8pPacket(i_points, i_color);
    dDbVw_setDrawPacketList(packet, i_bufferType);
}

void dDbVw_drawCube(int i_bufferType, cXyz& i_pos, cXyz& i_size, csXyz& i_angle, const GXColor& i_color) {
    mDoExt_cubePacket* packet = new mDoExt_cubePacket(i_pos, i_size, i_angle, i_color);
    dDbVw_setDrawPacketList(packet, i_bufferType);
}

void dDbVw_drawTriangle(int i_bufferType, cXyz* i_points, const GXColor& i_color, u8 i_clipZ) {
    mDoExt_trianglePacket* packet = new mDoExt_trianglePacket(i_points, i_color, i_clipZ);
    dDbVw_setDrawPacketList(packet, i_bufferType);
}

void dDbVw_drawQuad(int i_bufferType, cXyz* i_points, const GXColor& i_color, u8 i_clipZ) {
    mDoExt_quadPacket* packet = new mDoExt_quadPacket(i_points, i_color, i_clipZ);
    dDbVw_setDrawPacketList(packet, i_bufferType);
}

void dDbVw_drawQuad(int i_bufferType, cXyz* param_1, cXyz* param_2, cXyz* i_pos, s16 i_rotY, const GXColor& i_color, u8 i_clipZ) {
    cXyz points[4];

    points[0] = *param_1;
    points[2] = *param_2;

    points[1].x = param_2->x;
    points[1].y = param_1->y;
    points[1].z = param_1->z;

    points[3].x = param_1->x;
    points[3].y = param_1->y;
    points[3].z = param_2->z;

    mDoMtx_stack_c::transS(*i_pos);
    mDoMtx_stack_c::YrotM(i_rotY);

    for (int i = 0; i < 4; i++) {
        mDoMtx_stack_c::multVec(&points[i], &points[i]);
    }

    mDoExt_quadPacket* packet = new mDoExt_quadPacket(points, i_color, i_clipZ);
    dDbVw_setDrawPacketList(packet, i_bufferType);
}

void dDbVw_drawLine(int i_bufferType, cXyz& i_start, cXyz& i_end, const GXColor& i_color, u8 i_clipZ, u8 i_width) {
    mDoExt_linePacket* packet = new mDoExt_linePacket(i_start, i_end, i_color, i_clipZ, i_width);
    dDbVw_setDrawPacketList(packet, i_bufferType);
}

void dDbVw_drawArrow(int i_bufferType, cXyz& i_pos, cXyz& param_2, const GXColor& i_color, u8 i_clipZ, u8 i_width) {
    mDoExt_ArrowPacket* packet = new mDoExt_ArrowPacket(i_pos, param_2, i_color, i_clipZ, i_width);
    dDbVw_setDrawPacketList(packet, i_bufferType);
}

void dDbVw_drawPoint(int i_bufferType, cXyz& i_pos, const GXColor& i_color, u8 i_clipZ, u8 i_width) {
    mDoExt_pointPacket* packet = new mDoExt_pointPacket(i_pos, i_color, i_clipZ, i_width);
    dDbVw_setDrawPacketList(packet, i_bufferType);
}

void dDbVw_drawCircle(int i_bufferType, cXyz& i_pos, f32 i_radius, const GXColor& i_color, u8 i_clipZ, u8 i_width) {
    mDoExt_circlePacket* packet = new mDoExt_circlePacket(i_pos, i_radius, i_color, i_clipZ, i_width);
    dDbVw_setDrawPacketList(packet, i_bufferType);
}

void dDbVw_drawSphere(int i_bufferType, cXyz& i_pos, f32 i_size, const GXColor& i_color, u8 i_clipZ) {
    mDoExt_spherePacket* packet = new mDoExt_spherePacket(i_pos, i_size, i_color, i_clipZ);
    dDbVw_setDrawPacketList(packet, i_bufferType);
}

void dDbVw_drawCylinder(int i_bufferType, cXyz& i_pos, f32 i_radius, f32 i_height, const GXColor& i_color, u8 i_clipZ) {
    mDoExt_cylinderPacket* packet = new mDoExt_cylinderPacket(i_pos, i_radius, i_height, i_color, i_clipZ);
    dDbVw_setDrawPacketList(packet, i_bufferType);
}

void dDbVw_drawCylinderM(int i_bufferType, Mtx i_mtx, const GXColor& i_color, u8 i_clipZ) {
    mDoExt_cylinderMPacket* packet = new mDoExt_cylinderMPacket(i_mtx, i_color, i_clipZ);
    dDbVw_setDrawPacketList(packet, i_bufferType);
}

static bool reportOK() {
    return !dComIfG_isDebugMode() && !dDebugPad.Active();
}

static JUtility::TColor ShadowDarkColor(0, 0, 0, 0x80);
static JUtility::TColor ShadowLightColor(0xFF, 0xFF, 0xFF, 0x80);

int dDbVw_Report(int x, int y, char const* string, ...) {
    UNUSED(string);
    char buffer[256];

    if (reportOK()) {
        va_list list;
        va_start(list, string);
        vsnprintf(buffer, sizeof(buffer), string, list);
        va_end(list);
    
        JUTDbPrint::getManager()->flush();

        if (g_HIO.mColor & 0xF0F0F000) {
            JUTDbPrint::getManager()->setCharColor(ShadowDarkColor);
        } else {
            JUTDbPrint::getManager()->setCharColor(ShadowLightColor);
        }

        JUTReport(x + 2, y + 2, buffer);
        JUTDbPrint::getManager()->flush();
    
        JUTDbPrint::getManager()->setCharColor(g_HIO.mColor);
        JUTReport(x, y, buffer);
        JUTDbPrint::getManager()->flush();
        return 1;
    }

    return 0;
}
