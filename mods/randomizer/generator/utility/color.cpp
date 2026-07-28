#include "../utility/color.hpp"
#include "../utility/string.hpp"

#include <cmath>

HSV RGBToHSV(const double& r, const double& g, const double& b) {
    double min, max, delta;

    HSV out;

    min = r < g ? r : g;
    min = min < b ? min : b;

    max = r > g ? r : g;
    max = max > b ? max : b;

    out.V = max;
    delta = max - min;

    if(max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
        out.S = (delta / max);          // s
    } else {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.S = 0.0;
    }

    if (delta < 0.001) {
        out.H = 0.0;
    } else {
        if (r >= max)  {
            out.H = (g - b) / delta;        // between yellow & magenta
        } else if (g >= max) {
            out.H = 2.0 + (b - r) / delta;  // between cyan & yellow
        } else {
            out.H = 4.0 + (r - g) / delta;  // between magenta & cyan
        }
            
        out.H *= 60.0;                      // degrees

        if(out.H < 0.0) {
            out.H += 360.0;
        }
    }
    return out;
}

HSV RGBToHSV(RGBA<double> color) {
    return RGBToHSV(color.R, color.G, color.B);
}

RGBA<double> HSVToRGB(const HSV& hsv) {
    double      hh, p, q, t, ff;
    long        i;
    RGBA<double> out;

    if(hsv.S <= 0.0) { 
        out.R = hsv.V;
        out.G = hsv.V;
        out.B = hsv.V;
        return out;
    }
    hh = hsv.H;
    if (hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = hsv.V * (1.0 - hsv.S);
    q = hsv.V * (1.0 - (hsv.S * ff));
    t = hsv.V * (1.0 - (hsv.S * (1.0 - ff)));

    switch(i) {
    case 0:
        out.R = hsv.V;
        out.G = t;
        out.B = p;
        break;
    case 1:
        out.R = q;
        out.G = hsv.V;
        out.B = p;
        break;
    case 2:
        out.R = p;
        out.G = hsv.V;
        out.B = t;
        break;
    case 3:
        out.R = p;
        out.G = q;
        out.B = hsv.V;
        break;
    case 4:
        out.R = t;
        out.G = p;
        out.B = hsv.V;
        break;
    case 5:
    default:
        out.R = hsv.V;
        out.G = p;
        out.B = q;
        break;
    }
    return out;   
}

HSV color16BitToHSV(const uint16_t& color) {
    double r = (color & 0xF800) >> 11;
    double g = (color & 0x07E0) >> 5;
    double b = (color & 0x001F);
    return RGBToHSV(r / 31.0, g / 63.0, b / 31.0);
}

uint16_t colorHSVTo16Bit(const HSV& hsv) {
    auto colorRGB = HSVToRGB(hsv);

    uint16_t color565 = 0;
    color565 |= uint16_t(round(colorRGB.R * 31.0)) << 11;
    color565 |= uint16_t(round(colorRGB.G * 63.0)) << 5;
    color565 |= uint16_t(round(colorRGB.B * 31.0));

    return color565;
}

uint16_t hexColorStrTo16Bit(const std::string& hexColor) {
    auto hex = std::stoi(hexColor, nullptr, 16);

    double r = ((hex & 0xFF0000) >> 16) / 255.0f;
    double g = ((hex & 0x00FF00) >> 8) / 255.0f;
    double b = (hex & 0x0000FF) / 255.0f;

    auto colorHSV = RGBToHSV(r, g, b);
    return colorHSVTo16Bit(colorHSV);
}

RGBA<double> hexColorStrToRGB(const std::string& hexColor) {
    auto hex = std::stoi(hexColor, nullptr, 16);

    double r = ((hex & 0xFF0000) >> 16) / 255.0f;
    double g = ((hex & 0x00FF00) >> 8) / 255.0f;
    double b = (hex & 0x0000FF) / 255.0f;

    return RGBA<double>(r, g, b, 1);
}

std::string RGBToHexColorStr(const RGBA<double>& color) {
    
    int c = 0;

    c |= int(color.R * 255) << 16;
    c |= int(color.G * 255) << 8;
    c |= int(color.B * 255);

    return randomizer::utility::str::intToHex(c, 6, false);
}

bool isValidHexColor(const std::string& hexColor) {
    return hexColor.find_first_not_of("0123456789ABCDEFabcdef") == std::string_view::npos && hexColor.length() == 6;
}

// Takes 16-bit base, replacement, and current colors.
// Outputs what the new 16-bit color in place of the current color should
// be based on the difference between the base and replacement colors 
uint16_t colorExchange(const uint16_t& baseColor, const uint16_t& replacementColor, const uint16_t& curColor) {

    // Translate 16-bit colors into HSV color space
    auto baseColorHSV = color16BitToHSV(baseColor);
    auto replacementColorHSV = color16BitToHSV(replacementColor);
    auto curColorHSV = color16BitToHSV(curColor);

    // Calculate difference between base and replacement colors
    double sChange = replacementColorHSV.S - baseColorHSV.S;
    double vChange = replacementColorHSV.V - baseColorHSV.V;

    // Prevent issues when recoloring black/white/grey parts of a texture where the base color is not black/white/grey.
    if (curColorHSV.S == 0.0) {
        curColorHSV.S = baseColorHSV.S;
    }

    // Create new color from current color based on difference between base and replacement
    HSV newColorHSV;
    newColorHSV.H = replacementColorHSV.H;
    newColorHSV.S = curColorHSV.S + sChange;
    newColorHSV.V = curColorHSV.V + vChange;
    
    newColorHSV.S = std::max(0.0, std::min(1.0, newColorHSV.S));
    newColorHSV.V = std::max(0.0, std::min(1.0, newColorHSV.V));

    return colorHSVTo16Bit(newColorHSV);
}

std::string HSVShiftColor(const std::string& hexColor, const int& hShift, const int& vShift) {
    auto colorRGB = hexColorStrToRGB(hexColor);
    auto colorHSV = RGBToHSV(colorRGB);
    int h = colorHSV.H;
    int s = round(colorHSV.S * 100);
    int v = round(colorHSV.V * 100);

    h += hShift;
    h %= 360;

    auto origV = v;
    v += vShift;
    if (v < 0) {
        v = 0;
    }
    if (v > 100) {
        v = 100;
    }
    if (v < 30 && origV >= 30) {
        v = 30;
    }
    if (v > 90 and origV <= 90) {
        v = 90;
    }

    auto vDiff = v - origV;

    // Instead of shifting saturation separately, we simply make it relative to the value shift.
    // As value increases we want saturation to decrease and vice versa.
    // This is because bright colors look bad if they are too saturated, and dark colors look bland if they aren't saturated enough.
    auto origS = s;
    if (origS < 15 && vShift > 0) {
        // For colors that were originally very unsaturated, we want saturation to increase regardless of which direction value is shifting in.
        if (origV < 30) {
            // Very dark, nearly black. Needs extra saturation for the change to be noticeable.
            s += (vShift * 2);
        } else {
            // Not that dark, probably grey or whitish.
            s += vShift;
        }
    } else {
        s -= vDiff;
    }

    if (s < 0) {
        s = 0;
    }
    if (s > 100) {
        s = 100;
    }
    if (s < 5 && origS >= 5) {
        s = 5;
    }
    if (s > 80 && origS <= 80) {
        s = 80;
    }

    auto newColorHSV = HSV(h, s / 100.0f, v / 100.0f);
    auto newColorRGB = HSVToRGB(newColorHSV);
    return RGBToHexColorStr(newColorRGB);
}

std::pair<int, int> get_random_h_and_v_shifts_for_custom_color(const std::string& hexColor) {
    auto colorRGB = hexColorStrToRGB(hexColor);
    auto colorHSV = RGBToHSV(colorRGB);

    int s = round(colorHSV.S * 100);
    int v = round(colorHSV.V * 100);

    int minVShift = -40;
    int maxVShift = 40;

    if (s < 10) {
        // For very unsaturated colors, we want to limit the range of value
        // randomization to exclude results that wouldn't change anything anyway.
        // This effectively stops white and black from having a 50% chance to not change at all.
        minVShift = std::max(-40, 0-v);
        maxVShift = std::min(40, 100-v);
    }

    auto hShift = rand() % 360;
    auto vShift = (rand() % (maxVShift - minVShift)) + minVShift;

    return {hShift, vShift};
}
