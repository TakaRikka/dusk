#pragma once

/**
 * File originally copied from console TPR with permission from isaac
 * https://github.com/zsrtp/libtp_rel/blob/master/include/util/texture_utils.h
 */

#include "mod.hpp"

#include <gx.h>

#include <list>
#include <unordered_map>

uint32_t get_image_data_size(
    uint32_t format, uint32_t width, uint32_t height, uint32_t mipmapCount);

void recolor_texture(
    TextureReplacementData& replacementData, GXColor color, std::vector<u8>& newTextureDataOut);

std::unordered_map<ConfigVarHandle, std::list<TextureReplacementData>>& get_texture_replacements();
