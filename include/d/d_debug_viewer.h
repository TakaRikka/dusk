#ifndef D_DEBUG_VIEWER_H
#define D_DEBUG_VIEWER_H

#include <dolphin/gx.h>
#include "SSystem/SComponent/c_xyz.h"
#include "SSystem/SComponent/c_sxyz.h"

class J3DPacket;

J3DPacket* dDbVw_setDrawPacketList(J3DPacket* i_packet, int i_bufferType);
void dDbVw_deleteDrawPacketList();

void dDbVw_drawCube8p(int i_bufferType, cXyz* i_points, const GXColor& i_color);
void dDbVw_drawCube(int i_bufferType, cXyz& i_pos, cXyz& i_size, csXyz& i_angle, const GXColor& i_color);
void dDbVw_drawTriangle(int i_bufferType, cXyz* i_points, const GXColor& i_color, u8 i_clipZ);
void dDbVw_drawQuad(int i_bufferType, cXyz* i_points, const GXColor& i_color, u8 i_clipZ);
void dDbVw_drawQuad(int i_bufferType, cXyz* param_1, cXyz* param_2, cXyz* i_pos, s16 i_rotY, const GXColor& i_color, u8 i_clipZ);
void dDbVw_drawLine(int i_bufferType, cXyz& i_start, cXyz& i_end, const GXColor& i_color, u8 i_clipZ, u8 i_width);
void dDbVw_drawArrow(int i_bufferType, cXyz& i_pos, cXyz& param_2, const GXColor& i_color, u8 i_clipZ, u8 i_width);
void dDbVw_drawPoint(int i_bufferType, cXyz& i_pos, const GXColor& i_color, u8 i_clipZ, u8 i_width);
void dDbVw_drawCircle(int i_bufferType, cXyz& i_pos, f32 i_radius, const GXColor& i_color, u8 i_clipZ, u8 i_width);
void dDbVw_drawSphere(int i_bufferType, cXyz& i_pos, f32 i_size, const GXColor& i_color, u8 i_clipZ);
void dDbVw_drawCylinder(int i_bufferType, cXyz& i_pos, f32 i_radius, f32 i_height, const GXColor& i_color, u8 i_clipZ);
void dDbVw_drawCylinderM(int i_bufferType, Mtx i_mtx, const GXColor& i_color, u8 i_clipZ);
void dDbVw_drawCube8pXlu(cXyz* i_points, const GXColor& i_color);

inline void dDbVw_drawCube8pOpa(cXyz* i_points, const GXColor& i_color) {
    dDbVw_drawCube8p(0, i_points, i_color);
}

inline void dDbVw_drawCube8pXlu(cXyz* i_points, const GXColor& i_color) {
    dDbVw_drawCube8p(1, i_points, i_color);
}

inline void dDbVw_drawCubeOpa(cXyz& i_pos, cXyz& i_size, csXyz& i_angle, const GXColor& i_color) {
    dDbVw_drawCube(0, i_pos, i_size, i_angle, i_color);
}

inline void dDbVw_drawCubeXlu(cXyz& i_pos, cXyz& i_size, csXyz& i_angle, const GXColor& i_color) {
    dDbVw_drawCube(1, i_pos, i_size, i_angle, i_color);
}

inline void dDbVw_drawTriangleOpa(cXyz* i_points, const GXColor& i_color, u8 i_clipZ) {
    dDbVw_drawTriangle(0, i_points, i_color, i_clipZ);
}

inline void dDbVw_drawTriangleXlu(cXyz* i_points, const GXColor& i_color, u8 i_clipZ) {
    dDbVw_drawTriangle(1, i_points, i_color, i_clipZ);
}

inline void dDbVw_drawQuadOpa(cXyz* i_points, const GXColor& i_color, u8 i_clipZ) {
    dDbVw_drawQuad(0, i_points, i_color, i_clipZ);
}

inline void dDbVw_drawQuadXlu(cXyz* i_points, const GXColor& i_color, u8 i_clipZ) {
    dDbVw_drawQuad(1, i_points, i_color, i_clipZ);
}

inline void dDbVw_drawQuadOpa(cXyz* param_1, cXyz* param_2, cXyz* i_pos, s16 i_rotY, const GXColor& i_color, u8 i_clipZ) {
    dDbVw_drawQuad(0, param_1, param_2, i_pos, i_rotY, i_color, i_clipZ);
}

inline void dDbVw_drawQuadXlu(cXyz* param_1, cXyz* param_2, cXyz* i_pos, s16 i_rotY, const GXColor& i_color, u8 i_clipZ) {
    dDbVw_drawQuad(1, param_1, param_2, i_pos, i_rotY, i_color, i_clipZ);
}

inline void dDbVw_drawLineOpa(cXyz& i_start, cXyz& i_end, const GXColor& i_color, u8 i_clipZ, u8 i_width) {
    dDbVw_drawLine(0, i_start, i_end, i_color, i_clipZ, i_width);
}

inline void dDbVw_drawLineXlu(cXyz& i_start, cXyz& i_end, const GXColor& i_color, u8 i_clipZ, u8 i_width) {
    dDbVw_drawLine(1, i_start, i_end, i_color, i_clipZ, i_width);
}

inline void dDbVw_drawArrowOpa(cXyz& i_pos, cXyz& param_2, const GXColor& i_color, u8 i_clipZ, u8 i_width) {
    dDbVw_drawArrow(0, i_pos, param_2, i_color, i_clipZ, i_width);
}

inline void dDbVw_drawArrowXlu(cXyz& i_pos, cXyz& param_2, const GXColor& i_color, u8 i_clipZ, u8 i_width) {
    dDbVw_drawArrow(1, i_pos, param_2, i_color, i_clipZ, i_width);
}

inline void dDbVw_drawPointOpa(cXyz& i_pos, const GXColor& i_color, u8 i_clipZ, u8 i_width) {
    dDbVw_drawPoint(0, i_pos, i_color, i_clipZ, i_width);
}

inline void dDbVw_drawPointXlu(cXyz& i_pos, const GXColor& i_color, u8 i_clipZ, u8 i_width) {
    dDbVw_drawPoint(1, i_pos, i_color, i_clipZ, i_width);
}

inline void dDbVw_drawCircleOpa(cXyz& i_pos, f32 i_radius, const GXColor& i_color, u8 i_clipZ, u8 i_width) {
    dDbVw_drawCircle(0, i_pos, i_radius, i_color, i_clipZ, i_width);
}

inline void dDbVw_drawCircleXlu(cXyz& i_pos, f32 i_radius, const GXColor& i_color, u8 i_clipZ, u8 i_width) {
    dDbVw_drawCircle(1, i_pos, i_radius, i_color, i_clipZ, i_width);
}

inline void dDbVw_drawSphereOpa(cXyz& i_pos, f32 i_size, const GXColor& i_color, u8 i_clipZ) {
    dDbVw_drawSphere(0, i_pos, i_size, i_color, i_clipZ);
}

inline void dDbVw_drawSphereXlu(cXyz& i_pos, f32 i_size, const GXColor& i_color, u8 i_clipZ) {
    dDbVw_drawSphere(1, i_pos, i_size, i_color, i_clipZ);
}

inline void dDbVw_drawCylinderOpa(cXyz& i_pos, f32 i_radius, f32 i_height, const GXColor& i_color, u8 i_clipZ) {
    dDbVw_drawCylinder(0, i_pos, i_radius, i_height, i_color, i_clipZ);
}

inline void dDbVw_drawCylinderXlu(cXyz& i_pos, f32 i_radius, f32 i_height, const GXColor& i_color, u8 i_clipZ) {
    dDbVw_drawCylinder(1, i_pos, i_radius, i_height, i_color, i_clipZ);
}

inline void dDbVw_drawCylinderMOpa(Mtx i_mtx, const GXColor& i_color, u8 i_clipZ) {
    dDbVw_drawCylinderM(0, i_mtx, i_color, i_clipZ);
}

inline void dDbVw_drawCylinderMXlu(Mtx i_mtx, const GXColor& i_color, u8 i_clipZ) {
    dDbVw_drawCylinderM(1, i_mtx, i_color, i_clipZ);
}

int dDbVw_Report(int x, int y, char const* string, ...);

#endif /* D_DEBUG_VIEWER_H */
