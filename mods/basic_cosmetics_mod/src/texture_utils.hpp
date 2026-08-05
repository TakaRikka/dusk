#pragma once

/**
 * File originally copied from console TPR with permission from isaac
 * https://github.com/zsrtp/libtp_rel/blob/master/include/util/texture_utils.h
 */

#include <cstdint>

struct ResTIMG;
struct J3DTextureBlock;
class J3DModelFileData;
class mDoDvdThd_mountArchive_c;

ResTIMG* find_tex_header_in_tex_1_section(J3DTextureBlock* tex1Ptr, const char* textureName);

uint32_t swap_index_bits(bool leftIsGreater, uint32_t bits);

void recolor_cmpr_texture(J3DTextureBlock* tex1Ptr, const char* textureName, const uint8_t* rgb);

J3DTextureBlock* find_tex_1_in_bmd(J3DModelFileData* bmdPtr);

void handle_texture_overrides_on_load(mDoDvdThd_mountArchive_c* mountArchive);
