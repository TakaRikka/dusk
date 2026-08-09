#pragma once

/**
 * File originally copied from console TPR with permission from isaac
 * https://github.com/zsrtp/libtp_rel/blob/master/include/util/color_utils.h
 */

#include <gx.h>

#include <cstdint>
#include <string>

// Desaturates an RGB565 color to a u8 gray value (0xFF being white and 0x00
// being black).
uint8_t desaturate_rgb_565(uint16_t rgb565Val);

// Performs an "Overlay" blend of a u8 gray value and a pointer to a u8
// array of {r,g,b}. Returns the result as an RGB565.
uint16_t blend_overlay_rgb_565(uint8_t grayVal, GXColor color);

// Perform overlay blending on a single 8-bit color channel
uint8_t blend_overlay_channel(uint8_t base, uint8_t blend);

bool is_valid_hex_color_str(std::string_view hexStr);

GXColor hex_color_str_to_gx_color(const std::string& hexColorStr);

GXColor get_rainbow_rgb(f32 amplitude);

void update_rainbow_rgb(f32 increment);
