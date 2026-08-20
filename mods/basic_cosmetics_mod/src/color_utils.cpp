#include "color_utils.hpp"

uint8_t desaturate_rgb_565(uint16_t rgb565Val) {
    const uint32_t r = (rgb565Val & 0xf800) >> 11;
    const uint32_t g = (rgb565Val & 0x7e0) >> 5;
    const uint32_t b = rgb565Val & 0x1f;

    // Here we are doing a quicker (0.22 * r + 0.72 * g + 0.06 * b) which
    // uses multiplies and shifts rather than division.
    const uint32_t combined = 30480413 * r + 49085341 * g + 8312839 * b;
    uint8_t shifted = (combined >> 24) & 0xff;

    // Check if should round up shifted value.
    if (shifted < 0xff && combined & 0x00800000) {
        shifted += 1;
    }

    return shifted;
}

uint16_t blend_overlay_rgb_565(uint8_t grayVal, GXColor color) {
    uint32_t rTimes255, gTimes255, bTimes255;

    if (grayVal <= 0x7f) {
        const uint32_t grayTimesTwo = 2 * grayVal;

        rTimes255 = grayTimesTwo * color.r;
        gTimes255 = grayTimesTwo * color.g;
        bTimes255 = grayTimesTwo * color.b;
    } else {
        const uint32_t multiplier = 2 * (255 - grayVal);

        rTimes255 = 255 * 255 - multiplier * (255 - color.r);
        gTimes255 = 255 * 255 - multiplier * (255 - color.g);
        bTimes255 = 255 * 255 - multiplier * (255 - color.b);
    }

    // Divide each by 255
    const uint32_t r = (rTimes255 + 1 + (rTimes255 >> 8)) >> 8;
    const uint32_t g = (gTimes255 + 1 + (gTimes255 >> 8)) >> 8;
    const uint32_t b = (bTimes255 + 1 + (bTimes255 >> 8)) >> 8;

    return ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3);
}

// Helper function to perform overlay blending on a single 8-bit color channel
uint8_t blend_overlay_channel(uint8_t base, uint8_t blend) {
    if (base < 128) {
        return static_cast<uint8_t>((2 * base * blend) / 255);
    }

    return static_cast<uint8_t>(255 - (2 * (255 - base) * (255 - blend)) / 255);
}

bool is_valid_hex_color_str(std::string_view hexStr) {
    return hexStr.find_first_not_of("0123456789ABCDEFabcdef") == std::string_view::npos &&
           hexStr.length() == 6;
}

GXColor hex_color_str_to_gx_color(const std::string& hexColorStr) {
    u8 r = std::stoi(hexColorStr.substr(0, 2), nullptr, 16);
    u8 g = std::stoi(hexColorStr.substr(2, 2), nullptr, 16);
    u8 b = std::stoi(hexColorStr.substr(4, 2), nullptr, 16);
    return GXColor{r, g, b};
}

static f32 rainbowPhaseAngle = 0.f;

GXColor get_rainbow_rgb(f32 amplitude) {
    f32 phase_rad = rainbowPhaseAngle * M_PI / 180.0f;

    u8 r_val = (u8)(amplitude * (sinf(phase_rad) + 1.0f) + 0.5f);
    u8 g_val = (u8)(amplitude * (sinf(phase_rad + 2.0f * M_PI / 3.0f) + 1.0f) + 0.5f);
    u8 b_val = (u8)(amplitude * (sinf(phase_rad + 4.0f * M_PI / 3.0f) + 1.0f));
    GXColor rgbColor;
    rgbColor.r = r_val;
    rgbColor.g = g_val;
    rgbColor.b = b_val;
    rgbColor.a = 0xff;
    return rgbColor;
}

void update_rainbow_rgb(f32 increment) {
    rainbowPhaseAngle += increment;
    if (rainbowPhaseAngle >= 360.0f) {
        rainbowPhaseAngle -= 360.0f;
    }
}