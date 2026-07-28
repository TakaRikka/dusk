#pragma once

#include "../utility/common.hpp"

template<typename T> requires std::is_arithmetic_v<T>
struct RGBA {
    T R = 0;
    T G = 0;
    T B = 0;
    T A = std::numeric_limits<T>::max();

    RGBA() = default;

    RGBA(const T& val, const T& alpha) :
        R(val),
        G(val),
        B(val),
        A(alpha)
    {}

    RGBA(const T& r_, const T& g_, const T& b_ , const T& a_) :
        R(r_),
        G(g_),
        B(b_),
        A(a_)
    {}
};

using RGBA8 = RGBA<uint8_t>;

template<typename T>
bool readRGBA(std::istream& in, const std::streamoff& offset, RGBA<T>& out) {
    in.seekg(offset, std::ios::beg);

    if(!in.read(reinterpret_cast<char*>(&out.R), sizeof(out.R))) return false;
    if(!in.read(reinterpret_cast<char*>(&out.G), sizeof(out.G))) return false;
    if(!in.read(reinterpret_cast<char*>(&out.B), sizeof(out.B))) return false;
    if(!in.read(reinterpret_cast<char*>(&out.A), sizeof(out.A))) return false;
    
    if constexpr (sizeof(T) > 1) {
        Utility::Endian::toPlatform_inplace(Utility::Endian::Type::Big, out.R);
        Utility::Endian::toPlatform_inplace(Utility::Endian::Type::Big, out.G);
        Utility::Endian::toPlatform_inplace(Utility::Endian::Type::Big, out.B);
        Utility::Endian::toPlatform_inplace(Utility::Endian::Type::Big, out.A);
    }

    return true;
}

template<typename T>
void writeRGBA(std::ostream& out, const RGBA<T>& color) {
    if constexpr (sizeof(T) > 1) {
        T R_BE = Utility::Endian::toPlatform(Utility::Endian::Type::Big, color.R);
        T G_BE = Utility::Endian::toPlatform(Utility::Endian::Type::Big, color.G);
        T B_BE = Utility::Endian::toPlatform(Utility::Endian::Type::Big, color.B);
        T A_BE = Utility::Endian::toPlatform(Utility::Endian::Type::Big, color.A);

        out.write(reinterpret_cast<const char*>(&R_BE), sizeof(R_BE));
        out.write(reinterpret_cast<const char*>(&G_BE), sizeof(G_BE));
        out.write(reinterpret_cast<const char*>(&B_BE), sizeof(B_BE));
        out.write(reinterpret_cast<const char*>(&A_BE), sizeof(A_BE));
        return;
    }
    else {
        out.write(reinterpret_cast<const char*>(&color.R), sizeof(color.R));
        out.write(reinterpret_cast<const char*>(&color.G), sizeof(color.G));
        out.write(reinterpret_cast<const char*>(&color.B), sizeof(color.B));
        out.write(reinterpret_cast<const char*>(&color.A), sizeof(color.A));
    }

    return;    
}

struct HSV {
	double H = 0;
	double S = 0;
	double V = 0;

	HSV() = default;

	HSV(const double& h_, const double& s_, const double& v_) :
		H(h_),
		S(s_),
		V(v_)
	{}
};

HSV RGBToHSV(const double& r, const double& g, const double& b);

HSV RGBToHSV(RGBA<double> color);

RGBA<double> HSVToRGB(const HSV& hsv);

HSV color16BitToHSV(const uint16_t& color);

uint16_t colorHSVTo16Bit(const HSV& hsv);

RGBA<double> hexColorStrToRGB(const std::string& hexColor);

std::string RGBToHexColorStr(const RGBA<double>& color);

uint16_t hexColorStrTo16Bit(const std::string& hexColor);

bool isValidHexColor(const std::string& hexColor);

uint16_t colorExchange(const uint16_t& baseColor, const uint16_t& replacementColor, const uint16_t& curColor);

std::string HSVShiftColor(const std::string& hexColor, const int& hShift, const int& vShift);

std::pair<int, int> get_random_h_and_v_shifts_for_custom_color(const std::string& hexColor);
