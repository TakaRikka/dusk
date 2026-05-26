#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_name.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "d/d_com_inf_game.h"
#include "d/d_lib.h"
#include "m_Do/m_Do_audio.h"
#include "m_Do/m_Do_controller_pad.h"
#include <cstdio>
#include <cstring>

#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "dusk/settings.h"
#include "dusk/version.hpp"
#include "f_op/f_op_msg_mng.h"

static bool isPalOrJpn() {
    return dusk::version::isRegionPal() || dusk::version::isRegionJpn();
}

#if TARGET_PC
static bool useChineseNameKeyboard() {
    return dusk::version::isRegionPal() && dusk::getSettings().game.enableChineseNameKeyboard.getValue();
}

static bool isShiftJisLeadByte(u8 c) {
    return (c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xFC);
}

static int twoByteCode(const char* str) {
    return (static_cast<u8>(str[0]) << 8) | static_cast<u8>(str[1]);
}

static bool isTwoByteNameCharacter(int character) {
    return character > 0xFF;
}

static constexpr int CHINESE_NAME_PAGE_COUNT = 10;
static constexpr int CHINESE_NAME_PAGE_CHARS = 65;
static u8 sChineseNamePage = 0;
static bool sShowingChineseNamePage = false;

static int getActiveNameMenu() {
    return (useChineseNameKeyboard() && sShowingChineseNamePage) ? 1 : 0;
}

static bool isChineseNameMenu(int menu) {
    return useChineseNameKeyboard() && menu == 1;
}
#else
static bool useChineseNameKeyboard() {
    return false;
}
#endif

static const char* l_mojiHira[65] = {
    "あ", "い", "う", "え", "お", "か", "き", "く", "け", "こ", "さ", "し", "す",
    "せ", "そ", "た", "ち", "つ", "て", "と", "な", "に", "ぬ", "ね", "の", "は",
    "ひ", "ふ", "へ", "ほ", "ま", "み", "む", "め", "も", "や", "　", "ゆ", "　",
    "よ", "ら", "り", "る", "れ", "ろ", "わ", "　", "を", "　", "ん", "ぁ", "ぃ",
    "ぅ", "ぇ", "ぉ", "ゃ", "　", "ゅ", "　", "ょ", "っ", "　", "ー", "゛", "゜",
};

static const char* l_mojiHira2[65] = {
    "￥", "￥", "￥", "￥", "￥", "が", "ぎ", "ぐ", "げ", "ご", "ざ", "じ", "ず",
    "ぜ", "ぞ", "だ", "ぢ", "づ", "で", "ど", "￥", "￥", "￥", "￥", "￥", "ば",
    "び", "ぶ", "べ", "ぼ", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
};

static const char* l_mojiHira3[65] = {
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "ぱ",
    "ぴ", "ぷ", "ぺ", "ぽ", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
};

static const char* l_mojikata[65] = {
    "ア", "イ", "ウ", "エ", "オ", "カ", "キ", "ク", "ケ", "コ", "サ", "シ", "ス",
    "セ", "ソ", "タ", "チ", "ツ", "テ", "ト", "ナ", "ニ", "ヌ", "ネ", "ノ", "ハ",
    "ヒ", "フ", "ヘ", "ホ", "マ", "ミ", "ム", "メ", "モ", "ヤ", "　", "ユ", "　",
    "ヨ", "ラ", "リ", "ル", "レ", "ロ", "ワ", "　", "ヲ", "　", "ン", "ァ", "ィ",
    "ゥ", "ェ", "ォ", "ャ", "　", "ュ", "　", "ョ", "ッ", "　", "ー", "゛", "゜",
};

static const char* l_mojikata2[65] = {
    "￥", "￥", "ヴ", "￥", "￥", "ガ", "ギ", "グ", "ゲ", "ゴ", "ザ", "ジ", "ズ",
    "ゼ", "ゾ", "ダ", "ヂ", "ヅ", "デ", "ド", "￥", "￥", "￥", "￥", "￥", "バ",
    "ビ", "ブ", "ベ", "ボ", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
};

static const char* l_mojikata3[65] = {
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "パ",
    "ピ", "プ", "ペ", "ポ", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
    "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥", "￥",
};

static const char* l_mojiEisu[65] = {
    "A", "N", "a", "n", "1", "B", "O", "b", "o", "2", "C", "P", "c", "p", "3", "D", "Q",
    "d", "q", "4", "E", "R", "e", "r", "5", "F", "S", "f", "s", "6", "G", "T", "g", "t",
    "7", "H", "U", "h", "u", "8", "I", "V", "i", "v", "9", "J", "W", "j", "w", "0", "K",
    "X", "k", "x", ",", "L", "Y", "l", "y", ".", "M", "Z", "m", "z", " ",
};

#if TARGET_PC
// The game normally mutates this string list to fill in the real character codes.
// That can't work on a modern platform, so instead I've filled them out ahead of time.
static const char* l_mojiEisuPal_1[65] = {
    "A", "N", "\xC0", "\xCF", "1", "B", "O", "\xC1", "\xD0", "2", "C", "P", "\xC2", "\xD1", "3", "D", "Q",
    "\xC4", "\xD2", "4", "E", "R", "\xC6", "\xD3", "5", "F", "S", "\xC7", "\xD4", "6", "G", "T", "\xC8", "\xD6",
    "7", "H", "U", "\xC9", "\x8C", "8", "I", "V", "\xCA", "\xD9", "9", "J", "W", "\xCB", "\xDA", "0", "K",
    "X", "\xCC", "\xDB", ",", "L", "Y", "\xCD", "\xDC", ".", "M", "Z", "\xCE", "\x2D", " ",
};

static const char* l_mojiEisuPal_2[65] = {
    "a", "n", "\xE0", "\xEF", "1", "b", "o", "\xE1", "\xF0", "2", "c", "p", "\xE2", "\xF1", "3", "d", "q",
    "\xE4", "\xF2", "4", "e", "r", "\xE6", "\xF3", "5", "f", "s", "\xE7", "\xF4", "6", "g", "t", "\xE8",
    "\xF6", "7", "h", "u", "\xE9", "\x9C", "8", "i", "v", "\xEA", "\xF9", "9", "j", "w", "\xEB", "\xFA", "0",
    "k", "x", "\xEC", "\xFB", ",", "l", "y", "\xED", "\xFC", ".", "m", "z", "\xEE", "\xDF", " ",
};

static const char* l_mojiEisuPalChinese_1[65] = {
    "A", "B", "C", "D", "1", "E", "F", "G", "H", "2", "I", "J", "K", "L", "3", "M", "N",
    "O", "P", "4", "Q", "R", "S", "T", "5", "U", "V", "W", "X", "6", "Y", "Z", "a", "b",
    "7", "c", "d", "e", "f", "8", "g", "h", "i", "j", "9", "k", "l", "m", "n", "0", "o",
    "p", "q", "r", ",", "s", "t", "u", "v", ".", "w", "x", "y", "z", " ",
};

static const char* l_mojiEisuPalChinese_2[65] = {
    "A", "B", "C", "D", "1", "E", "F", "G", "H", "2", "I", "J", "K", "L", "3", "M", "N",
    "O", "P", "4", "Q", "R", "S", "T", "5", "U", "V", "W", "X", "6", "Y", "Z", "a", "b",
    "7", "c", "d", "e", "f", "8", "g", "h", "i", "j", "9", "k", "l", "m", "n", "0", "o",
    "p", "q", "r", ",", "s", "t", "u", "v", ".", "w", "x", "y", "z", " ",
};

static const u16 l_mojiZh[CHINESE_NAME_PAGE_COUNT][CHINESE_NAME_PAGE_CHARS] = {
    {
        0x97D1, 0x8D8E, 0x8DC7, 0x8A6C, 0x0000, 0x8E64, 0x8A43, 0x9D66, 0x8F9F, 0x0000, 0x89C1, 0x8FE6, 0x88E4,
        0x95C4, 0x0000, 0x91BD, 0x9B50, 0x88C9, 0x9798, 0x0000, 0x88BE, 0x8965, 0x9456, 0x8ED1, 0x0000, 0x89A9,
        0x8DA8, 0x8CF5, 0x96BE, 0x0000, 0x9142, 0x88C3, 0x8CF6, 0x8EE5, 0x0000, 0x9745, 0x8ED2, 0x8951, 0x8F82,
        0x0000, 0x8B7C, 0x90FB, 0x92DC, 0x8EAB, 0x0000, 0x8EC7, 0x9394, 0x96FB, 0x9572, 0x0000, 0x8FCF, 0x8A93,
        0x89F1, 0x90F9, 0x8ECF, 0x8EB1, 0x8C43, 0x93B2, 0x9854, 0x8FBA, 0x944C, 0x8BE7, 0x8F9E, 0x928E, 0x8FB4
    },
    {
        0x90B8, 0x8BAD, 0x8DB0, 0x905F, 0x0000, 0x9361, 0x9058, 0x89CE, 0x8E52, 0x0000, 0x8CCE, 0x8DB9, 0x9499,
        0x90E1, 0x0000, 0x8FE9, 0x9AC6, 0x91BA, 0x8FAF, 0x0000, 0x9671, 0x89EB, 0x9356, 0x8BF3, 0x0000, 0x926E,
        0x89BA, 0x93B4, 0x8C8A, 0x0000, 0x96C0, 0x8A64, 0x9383, 0x8B62, 0x0000, 0x8ED9, 0x8EA4, 0x8DFA, 0x95F3,
        0x0000, 0x94A0, 0x89E8, 0x8E77, 0x93EC, 0x0000, 0x8E99, 0x8FF5, 0x95D0, 0x9053, 0x0000, 0x90B6, 0x96BD,
        0x9682, 0x9640, 0x8963, 0x94E4, 0x8C6B, 0x8C9E, 0x8CEF, 0x8E87, 0x9492, 0x8BE0, 0x8EB4, 0x96D8, 0x90CE
    },
    {
        0x9085, 0x8F70, 0x978B, 0x9975, 0x0000, 0x9379, 0x9190, 0x89D4, 0x8B5F, 0x0000, 0x8C8E, 0x93FA, 0x90AF,
        0x96E9, 0x0000, 0x88A5, 0x90BC, 0x966B, 0x8FE3, 0x0000, 0x9286, 0x8DB6, 0x8945, 0x914F, 0x0000, 0x8D40,
        0x93E0, 0x8A4F, 0x91E5, 0x0000, 0x8FAC, 0x8D82, 0x92E1, 0x8ED4, 0x0000, 0x925A, 0x9056, 0x8B8C, 0x8A8F,
        0x0000, 0x88F7, 0x8AF7, 0x919C, 0x95C7, 0x0000, 0x89E6, 0x88D7, 0x8DA7, 0x89A4, 0x0000, 0x8D91, 0x89C6,
        0x91B0, 0x95FC, 0x9746, 0x8B40, 0x906C, 0x89F6, 0x95A8, 0x8EF1, 0x8F63, 0x9590, 0x8AED, 0x9195, 0x8A4A
    },
    {
        0x93B9, 0x8BEF, 0x8CEB, 0x9358, 0x0000, 0x8FA4, 0x8E6D, 0x95BA, 0x8F93, 0x0000, 0x8ECD, 0x89CD, 0x97AC,
        0x8FCA, 0x0000, 0x957A, 0x90F2, 0x89B7, 0x8BF8, 0x0000, 0x8A49, 0x8C59, 0x8A8D, 0x9AD0, 0x0000, 0x8B42,
        0x9140, 0x8E73, 0x95BD, 0x0000, 0x8CB4, 0x899C, 0x92F3, 0x8DE2, 0x0000, 0x8CAC, 0x93F2, 0x8CB0, 0x90A2,
        0x0000, 0x8A45, 0x8C85, 0x8FBB, 0x975A, 0x0000, 0x8D87, 0x88FC, 0x89BB, 0x8C60, 0x0000, 0x8A9C, 0x896F,
        0x9067, 0x9197, 0x93FC, 0x8CFB, 0x8F6F, 0x8BE4, 0x9765, 0x8C57, 0x8EB9, 0x8EBE, 0x8F76, 0x8E9B, 0x8AC6
    },
    {
        0x8F7E, 0x8E71, 0x8B8D, 0x8A59, 0x0000, 0x9748, 0x8B53, 0x9467, 0x9476, 0x0000, 0x8AE1, 0x916F, 0x8FD0,
        0x934A, 0x0000, 0x895A, 0x9649, 0x8A7B, 0x96A8, 0x0000, 0x90D7, 0x8EC0, 0x97C0, 0x92F2, 0x0000, 0x8C96,
        0x8DF5, 0x9463, 0x8BB8, 0x0000, 0x91E4, 0x8EA2, 0x984F, 0x9144, 0x0000, 0x91FC, 0x89B1, 0x9051, 0x8AE2,
        0x0000, 0x88A4, 0x8B4F, 0x8E4A, 0x9772, 0x0000, 0x8FD9, 0x8F9B, 0x8F9D, 0x9A46, 0x0000, 0x8AB0, 0x8A4D,
        0x96DA, 0x8EE7, 0x8967, 0x97B7, 0x8F88, 0x8E92, 0x8DB7, 0x88E3, 0x93B6, 0x96AF, 0x9856, 0x9B77, 0x8FAD
    },
    {
        0x8F97, 0x944E, 0x95EA, 0x88C1, 0x0000, 0x9583, 0x88B7, 0x92ED, 0x93AF, 0x0000, 0x94BA, 0x95DB, 0x91B6,
        0x8DAF, 0x0000, 0x8EE6, 0x8E6E, 0x91A9, 0x8E8D, 0x0000, 0x8ACC, 0x8FC1, 0x8C45, 0x92E8, 0x0000, 0x95D4,
        0x96BC, 0x8E9A, 0x8E5E, 0x0000, 0x9349, 0x88EA, 0x90A5, 0x8DDD, 0x0000, 0x9573, 0x97B9, 0x974C, 0x9861,
        0x0000, 0x8E79, 0x88AF, 0x98A2, 0x89E4, 0x0000, 0x88C8, 0x9776, 0x91BC, 0x9788, 0x0000, 0x9770, 0x88CC,
        0x939E, 0x8DEC, 0x98B0, 0x8F41, 0x95AA, 0x8A68, 0x90AC, 0x89EF, 0x89C2, 0x896E, 0x8957, 0x8D48, 0x96E7
    },
    {
        0x945C, 0x8E66, 0x88BF, 0x8C49, 0x0000, 0x96CA, 0x8EA7, 0x95FB, 0x8D73, 0x0000, 0x8A77, 0x8F8A, 0x93BE,
        0x8C82, 0x0000, 0x8E4F, 0x8E81, 0x9285, 0x9399, 0x0000, 0x9594, 0x9378, 0x8BD2, 0x97CD, 0x0000, 0x97A2,
        0x9440, 0x8EA9, 0x93F1, 0x0000, 0x979D, 0x8B4E, 0x8BD0, 0x8A62, 0x0000, 0x97CA, 0x88A9, 0x91CC, 0x90A7,
        0x0000, 0x9396, 0x8E67, 0x935F, 0x98B8, 0x0000, 0x88A1, 0x967B, 0x8B8E, 0x90AB, 0x0000, 0x8D44, 0x8A89,
        0x9B80, 0x8E74, 0x88F6, 0x9752, 0x91B4, 0x8DB1, 0x9152, 0x8E6C, 0x93DF, 0x8E96, 0x918A, 0x9153, 0x955C
    },
    {
        0x8EE0, 0x8B60, 0x975E, 0x8A65, 0x0000, 0x8F64, 0x8C77, 0x9094, 0x90B3, 0x0000, 0x94BD, 0x8AC5, 0x9241,
        0x9F83, 0x0000, 0x91E6, 0x8CFC, 0x8D9F, 0x9676, 0x0000, 0x89F0, 0x8EDD, 0x88D3, 0x8C9A, 0x0000, 0x9DD9,
        0x8C6E, 0x8942, 0x8FEE, 0x0000, 0x8DC5, 0x97A7, 0x889F, 0x917A, 0x0000, 0x9BDF, 0x92CA, 0x9BF3, 0x92F1,
        0x0000, 0x92BC, 0x9357, 0x8CDC, 0x89CA, 0x0000, 0x8FDB, 0x88CA, 0x8FED, 0x95B6, 0x0000, 0x8A9D, 0x8E9F,
        0x9569, 0x8EAE, 0x8A88, 0x8D80, 0x8B79, 0x8AC7, 0x93C1, 0x8C8F, 0x8B81, 0x8AEE, 0x8E61, 0x9848, 0x8C6F
    },
    {
        0x8C8D, 0x90DA, 0x926D, 0x8E5B, 0x0000, 0x8FAB, 0x8C7A, 0x8D5C, 0x8D68, 0x0000, 0x8950, 0x8EE8, 0x8A70,
        0x8AFA, 0x0000, 0x8DAA, 0x8E72, 0x8BE3, 0x8BE6, 0x0000, 0x8A94, 0x95FA, 0x9972, 0x94ED, 0x0000, 0x8AB1,
        0x98F4, 0x954B, 0x90E6, 0x0000, 0x9443, 0x9188, 0x8B8B, 0x8E76, 0x0000, 0x8B54, 0x8CF0, 0x8EF3, 0x8CB2,
        0x0000, 0x8D6A, 0x985A, 0x8BA4, 0x8B52, 0x0000, 0x9DBE, 0x89FC, 0x90B4, 0x94FC, 0x0000, 0x8DC4, 0x8E4C,
        0x8D58, 0x8960, 0x90D8, 0x91C5, 0x8BB3, 0x91AC, 0x8A81, 0x88C0, 0x97E1, 0x905E, 0x8956, 0x969C, 0x8B6B
    },
    {
        0x8E8A, 0x9196, 0x8EA6, 0x90BA, 0x0000, 0x8AAE, 0x8C5D, 0x94AA, 0x895E, 0x0000, 0x89C8, 0x8A92, 0x904D,
        0x9146, 0x0000, 0x8D99, 0x90AE, 0x8DA1, 0x8F57, 0x0000, 0x8D7B, 0x8B69, 0x8C51, 0x9BF6, 0x0000, 0x8D77,
        0x9269, 0x97A5, 0x8BA9, 0x0000, 0x8D5D, 0x897A, 0x8E9D, 0x89B9, 0x0000, 0x88D1, 0x88BA, 0x8A4B, 0x9958,
        0x0000, 0x8F92, 0x8E85, 0x9266, 0x905B, 0x0000, 0x8F4F, 0x8BDF, 0x90E7, 0x8EFC, 0x0000, 0x9166, 0x8B5A,
        0x94BC, 0x8955, 0x90C2, 0x97F1, 0x88B8, 0x8995, 0x8E78, 0x8E6A, 0x8AB4, 0x8959, 0x95D6, 0x899D, 0x89BD
    },
};

static int getChinesePageKey(int idx) {
    switch (idx) {
    case 4:
        return 0;
    case 9:
        return 1;
    case 14:
        return 2;
    case 19:
        return 3;
    case 24:
        return 4;
    case 29:
        return 5;
    case 34:
        return 6;
    case 39:
        return 7;
    case 44:
        return 8;
    case 49:
        return 9;
    default:
        return -1;
    }
}

static const char* getChineseNameKeyboardCell(u16 code) {
    static char buf[3];
    if (code == 0) {
        return " ";
    }

    buf[0] = (code >> 8) & 0xFF;
    buf[1] = code & 0xFF;
    buf[2] = 0;
    return buf;
}

static const char* getPalNameKeyboardCell(const char** eisuSet, int idx) {
    if (!useChineseNameKeyboard() || !sShowingChineseNamePage) {
        return eisuSet[idx];
    }

    return getChinesePageKey(idx) >= 0 ? eisuSet[idx] : getChineseNameKeyboardCell(l_mojiZh[sChineseNamePage][idx]);
}

static const char* getPalNameKeyboardTableCell(bool upper, int idx) {
    if (!useChineseNameKeyboard()) {
        return upper ? l_mojiEisuPal_1[idx] : l_mojiEisuPal_2[idx];
    }

    return upper ? l_mojiEisuPalChinese_1[idx] : l_mojiEisuPalChinese_2[idx];
}
#elif REGION_PAL
static const char* l_mojiEisuPal_1[65] = {
    "A", "N", "AA", "BB", "1", "B", "O", "CC", "DD", "2", "C", "P", "EE", "FF", "3", "D", "Q",
    "GG", "HH", "4", "E", "R", "II", "JJ", "5", "F", "S", "KK", "LL", "6", "G", "T", "MM", "NN",
    "7", "H", "U", "OO", "PP", "8", "I", "V", "QQ", "RR", "9", "J", "W", "SS", "TT", "0", "K",
    "X", "UU", "VV", ",", "L", "Y", "WW", "XX", ".", "M", "Z", "YY", "ZZ", " ",
};

static const char* l_mojiEisuPal_2[65] = {
    "a", "n", "aa", "bb", "1", "b", "o", "cc", "dd", "2", "c", "p", "ee", "ff", "3", "d", "q",
    "gg", "hh", "4", "e", "r", "ii", "jj", "5", "f", "s", "kk", "ll", "6", "g", "t", "mm",
    "nn", "7", "h", "u", "oo", "pp", "8", "i", "v", "qq", "rr", "9", "j", "w", "ss", "tt", "0",
    "k", "x", "uu", "vv", ",", "l", "y", "ww", "xx", ".", "m", "z", "yy", "zz", " ",
};
#endif

#if TARGET_PC
// '　' (full-width space)
#define SPACE_MAYBE_FULL (dusk::version::isRegionJpn() ? '\x81\x40' : ' ')
#elif REGION_JPN
// '　' (full-width space)
#define SPACE_MAYBE_FULL '\x81\x40'
#else
#define SPACE_MAYBE_FULL ' '
#endif

static dNm_HIO_c g_nmHIO;

typedef void (dName_c::*selProcFunc)(void);
static selProcFunc SelProc[9] = {
    &dName_c::MojiSelect,     &dName_c::MojiSelectAnm,  &dName_c::MojiSelectAnm2,
    &dName_c::MojiSelectAnm3, &dName_c::MenuSelect,     &dName_c::MenuSelectAnm,
    &dName_c::MenuSelectAnm2, &dName_c::MenuSelectAnm3, &dName_c::Wait
};

dNm_HIO_c::dNm_HIO_c() {
    mMenuScale = 1.3f;
    mSelCharScale = 1.4f;
    field_0x10 = 10;
}

dName_c::dName_c(J2DPane* pane) {
    nameIn.field_0xc = pane;
    _create();
    init();
}

dName_c::~dName_c() {
    JKR_DELETE(stick);
    JKR_DELETE(nameIn.NameInScr);
    mDoExt_removeMesgFont();

    for (int i = 0; i < 8; i++) {
        JKR_DELETE(mNameCursor[i]);
    }

    for (int i = 0; i < 65; i++) {
        JKR_DELETE(mMojiIcon[i]);
    }

    for (int i = 0; i < 4; i++) {
        if (mMenuIcon[i] != NULL) {
            JKR_DELETE(mMenuIcon[i]);
        }
    }

    JKR_DELETE(mCursorColorKey);
    JKR_DELETE(mCursorTexKey);
    JKR_DELETE(mSelIcon);
    archive->removeResourceAll();
}

void dName_c::_create() {
    stick = JKR_NEW STControl(5, 2, 2, 1, 0.9f, 0.5f, 0, 0x800);
    stick->setFirstWaitTime(5);
    nameIn.font = mDoExt_getMesgFont();
    g_nmHIO.field_0x4 = -1;
    screenSet();

    mNextNameStr[0] = 0;
    mCursorDelay = 1;
    displayInit();
}

void dName_c::init() {
    mCurPos = 0;
    mLastCurPos = 0;
    nameCursorMove();
    mLastCurPos = mCurPos;

    for (int i = 0; i < 4; i++) {
        field_0x30c[i][2] = 0;
    }

    mCharColumn = 0;
    mCharRow = 0;
    mPrevColumn = 0;
    mPrevRow = 0;
    field_0x30c[0][0] = 0;
    field_0x30c[0][1] = 0;
    field_0x30c[0][2] = 1;

    selectCursorMove();
    mSelProc = PROC_MOJI_SELECT;
    field_0x2ac = mSelProc;
    field_0x2ad = mSelProc;
    field_0x2ae = field_0x2ac;
    #if TARGET_PC
    mMojiSet = isPalOrJpn() ? MOJI_HIRA : MOJI_EIGO;
    #elif REGION_PAL || REGION_JPN
    mMojiSet = MOJI_HIRA;
    #else
    mMojiSet = MOJI_EIGO;
    #endif
    mPrevMojiSet = 255;
    #if TARGET_PC
    mSelMenu = isPalOrJpn() ? MENU_HIRA : MENU_END;
    mPrevSelMenu = isPalOrJpn() ? MENU_HIRA : MENU_END;
    #elif REGION_PAL || REGION_JPN
    mSelMenu = MENU_HIRA;
    mPrevSelMenu = MENU_HIRA;
    #else
    mSelMenu = MENU_END;
    mPrevSelMenu = MENU_END;
    #endif
    #if TARGET_PC
    if (useChineseNameKeyboard()) {
        sChineseNamePage = 0;
        sShowingChineseNamePage = false;
    }
    #endif
    mojiListChange();
}

void dName_c::initial() {
    displayInit();

    if (mNextNameStr[0] != 0) {
        NameStrSet();
        mNextNameStr[0] = 0;
    }

    #if TARGET_PC || REGION_PAL || REGION_JPN
    IF_DUSK_BLOCK(isPalOrJpn())
    if (mSelProc == PROC_MOJI_SELECT) {
        int mojiSet_i = getMenuPosIdx(DUSK_IF_ELSE(getActiveNameMenu(), mMojiSet));
        if (mMenuIcon[mojiSet_i] != NULL && mMenuText[mojiSet_i] != NULL) {
            mMenuIcon[mojiSet_i]->scale(g_nmHIO.mMenuScale, g_nmHIO.mMenuScale);
            mMenuText[mojiSet_i]->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));
        }
        if (mPrevMojiSet != 255) {
            int prevMojiSet_i = getMenuPosIdx(mPrevMojiSet);
            if (mMenuIcon[prevMojiSet_i] != NULL && mMenuText[prevMojiSet_i] != NULL) {
                mMenuIcon[prevMojiSet_i]->scale(1.0f, 1.0f);
                mMenuText[prevMojiSet_i]->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
            }
        }
    }
    IF_DUSK_BLOCK_END
    #endif
}

void dName_c::showIcon() {
    Vec pos;

    switch (mSelProc) {
    case PROC_MOJI_SELECT:
        if (mCharColumn != 255 && mCharRow != 255) {
            pos = mMojiIcon[mCharRow + mCharColumn * 5]->getGlobalVtxCenter(false, 0);
            mSelIcon->setPos(pos.x, pos.y, mMojiIcon[mCharRow + mCharColumn * 5]->getPanePtr(),
                             true);
            ((J2DTextBox*)mMojiIcon[mCharRow + mCharColumn * 5]->getPanePtr())
                ->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));
            mSelIcon->setAlphaRate(1.0f);
        }
        break;
    case PROC_MENU_SELECT:
        if (mSelMenu != 255) {
            int menu_i = getMenuPosIdx(mSelMenu);
            if (mMenuIcon[menu_i] == NULL || mMenuText[menu_i] == NULL) {
                break;
            }

            pos = mMenuIcon[menu_i]->getGlobalVtxCenter(false, 0);
            mSelIcon->setPos(pos.x, pos.y, mMenuIcon[menu_i]->getPanePtr(), true);
            mMenuText[menu_i]->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));
            mSelIcon->setAlphaRate(1.0f);
        }
        break;
    }
}

void dName_c::_move() {
    stick->checkTrigger();
    (this->*SelProc[mSelProc])();

    #if TARGET_PC || REGION_PAL || REGION_JPN
    if (IF_DUSK(isPalOrJpn() &&) mDoCPd_c::getTrigY(PAD_1)) {
        mDoAud_seStart(Z2SE_SY_DUMMY, 0, 0, 0);
        mPrevMojiSet = mMojiSet;
        mMojiSet++;
        #if TARGET_PC
        if (useChineseNameKeyboard()) {
            sShowingChineseNamePage = false;
        }
        #endif
        #if REGION_JPN
        if (mMojiSet > MOJI_EIGO) {
        #else
        if (mMojiSet > MOJI_KATA) {
        #endif
            mMojiSet = MOJI_HIRA;
        }
        mojiListChange();
    } else {
    #endif
    #if TARGET_PC || REGION_JPN
    if (IF_DUSK(dusk::version::isRegionJpn() &&) mDoCPd_c::getTrigX(PAD_1)) {
        if (mCurPos != 0) {
            if (mojiChange(mCurPos - 1) == 1) {
                mDoAud_seStart(Z2SE_SY_DUMMY, 0, 0, 0);
            } else {
                mDoAud_seStart(Z2SE_SYS_ERROR, 0, 0, 0);
            }
        }
    } else {
    #endif
#if !TARGET_PC
    if (mDoCPd_c::getTrigRight(PAD_1)) {
        // BUG: this check only fails if the cursor is at exactly 7
        // setMoji allows the cursor to reach 8, which is out of bounds here
        if (mCurPos != 7) {
            mDoAud_seStart(Z2SE_SY_DUMMY, 0, 0, 0);
            mLastCurPos = mCurPos;
            mCurPos++;
            nameCursorMove();
        }
    } else if (mDoCPd_c::getTrigLeft(PAD_1)) {
        if (mCurPos != 0) {
            mDoAud_seStart(Z2SE_SY_DUMMY, 0, 0, 0);
            mLastCurPos = mCurPos;
            mCurPos--;
            nameCursorMove();
        }
    } else
#endif
    if (mDoCPd_c::getTrigB(PAD_1)) {
        if (mCurPos == 0) {
            mDoAud_seStart(Z2SE_SY_MENU_BACK, 0, 0, 0);
            field_0x2ac = mSelProc;
            field_0x2ae = mSelProc;
            mSelProc = PROC_WAIT;
            mIsInputEnd = true;
        } else {
            backSpace();
        }
    } else if (mDoCPd_c::getTrigStart(PAD_1)) {
#define EIGO_OR_END DUSK_IF_ELSE((dusk::version::isRegionPal() ? MENU_EIGO : MENU_END), MENU_END)

        #if REGION_PAL
        if ((mSelProc != PROC_MENU_SELECT || mSelMenu != MENU_EIGO) &&
            (mSelProc == PROC_MENU_SELECT || mSelProc == PROC_MOJI_SELECT))
        {
        #else
        if ((mSelProc != PROC_MENU_SELECT || mSelMenu != EIGO_OR_END) &&
            (mSelProc == PROC_MENU_SELECT || mSelProc == PROC_MOJI_SELECT))
        {
        #endif
            mDoAud_seStart(Z2SE_SY_CURSOR_OPTION, 0, 0, 0);
            mPrevSelMenu = mSelMenu;
            #if REGION_PAL
            mSelMenu = MENU_EIGO;
            #else
            mSelMenu = EIGO_OR_END;
            #endif

            switch (mSelProc) {
            case PROC_MOJI_SELECT:
                mPrevColumn = mCharColumn;
                mPrevRow = mCharRow;
                MojiSelectAnmInit();
                mSelProc = PROC_MOJI_SEL_ANM2;
                break;
            case PROC_MENU_SELECT:
                MenuSelectAnmInit();
                mSelProc = PROC_MENU_SEL_ANM;
                break;
            }
        }
    }
    #if TARGET_PC || REGION_JPN
    }
    #endif
    #if TARGET_PC || REGION_PAL || REGION_JPN
    }
    #endif

    cursorAnm();
}

int dName_c::nameCheck() {
    for (int i = 8, len = 7; i > 0; i--) {
        #if REGION_JPN
        if (mChrInfo[len].mCharacter != ' ' && mChrInfo[len].mCharacter != '\x81\x40') {
        #else
        if (mChrInfo[len].mCharacter != ' ' IF_DUSK(&& (!dusk::version::isRegionJpn() || mChrInfo[len].mCharacter != '\x81\x40'))) {
        #endif
            return len + 1;
        }
        len--;
    }

    return 0;
}

void dName_c::playNameSet(int nameLength) {
    char* str = mInputStr;

    for (int i = 0; i < nameLength; i++) {
        #if TARGET_PC
        if (!isTwoByteNameCharacter(mChrInfo[i].mCharacter) &&
            (!dusk::version::isRegionJpn() || mChrInfo[i].mMojiSet == 2))
        {
            *str = mChrInfo[i].mCharacter;
            str += 1;
        } else {
            *str++ = (mChrInfo[i].mCharacter & 0xff00) >> 8;
            *str++ = mChrInfo[i].mCharacter & 0xff;
        }
        #elif REGION_JPN
        if (mChrInfo[i].mMojiSet == 2) {
            *str = mChrInfo[i].mCharacter;
            str += 1;
        } else {
            *(u16*)str = mChrInfo[i].mCharacter;
            str += 2;
        }
        #else
        *str = mChrInfo[i].mCharacter;
        str++;
        #endif
    }

    *str = 0;
}

void dName_c::cursorAnm() {
    mCurColAnmF += 2;
    if (mCurColAnmF >= mCursorColorKey->getFrameMax()) {
        mCurColAnmF -= mCursorColorKey->getFrameMax();
    }
    mCursorColorKey->setFrame(mCurColAnmF);

    mCurTexAnmF += 2;
    if (mCurTexAnmF >= mCursorTexKey->getFrameMax()) {
        mCurTexAnmF -= mCursorTexKey->getFrameMax();
    }
    mCursorTexKey->setFrame(mCurTexAnmF);

    nameIn.NameInScr->animation();
}

void dName_c::Wait() {}

void dName_c::MojiSelect() {
    if (mDoCPd_c::getTrigA(PAD_1)) {
        selectMojiSet();
    } else if (stick->checkRightTrigger()) {
        mDoAud_seStart(Z2SE_SY_NAME_CURSOR, 0, 0, 0);
        mPrevColumn = mCharColumn;
        mPrevRow = mCharRow;
        mCharColumn++;

        if (mCharColumn > 12) {
            mCharColumn = 0;
        }
        MojiSelectAnmInit();
        mSelProc = PROC_MOJI_SEL_ANM;
    } else if (stick->checkLeftTrigger()) {
        mDoAud_seStart(Z2SE_SY_NAME_CURSOR, 0, 0, 0);
        mPrevColumn = mCharColumn;
        mPrevRow = mCharRow;

        if (mCharColumn == 0) {
            mCharColumn = 12;
        } else {
            mCharColumn--;
        }
        MojiSelectAnmInit();
        mSelProc = PROC_MOJI_SEL_ANM;
    } else if (stick->checkUpTrigger()) {
        mPrevColumn = mCharColumn;
        mPrevRow = mCharRow;
        MojiSelectAnmInit();

        if (mCharRow == 0) {
            mDoAud_seStart(Z2SE_SY_CURSOR_OPTION, 0, 0, 0);
            menuCursorPosSet();
            mSelProc = PROC_MOJI_SEL_ANM2;
        } else {
            mDoAud_seStart(Z2SE_SY_NAME_CURSOR, 0, 0, 0);
            mCharRow--;
            mSelProc = PROC_MOJI_SEL_ANM;
        }
    } else if (stick->checkDownTrigger()) {
        mPrevColumn = mCharColumn;
        mPrevRow = mCharRow;
        MojiSelectAnmInit();
        mCharRow++;

        if (mCharRow > 4) {
            mCharRow = 4;
            mDoAud_seStart(Z2SE_SY_CURSOR_OPTION, 0, 0, 0);
            menuCursorPosSet();
            mSelProc = PROC_MOJI_SEL_ANM2;
        } else {
            mDoAud_seStart(Z2SE_SY_NAME_CURSOR, 0, 0, 0);
            mSelProc = PROC_MOJI_SEL_ANM;
        }
    }
}

void dName_c::MojiSelectAnmInit() {
    mSelIcon->setAlphaRate(0.0f);
    mMojiIcon[mPrevRow + mPrevColumn * 5]->scaleAnimeStart(0);
    ((J2DTextBox*)mMojiIcon[mPrevRow + mPrevColumn * 5]->getPanePtr())
        ->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
}

void dName_c::MojiSelectAnm() {
    if (mMojiIcon[mPrevRow + mPrevColumn * 5]->scaleAnime(mCursorDelay, g_nmHIO.mSelCharScale,
                                                          1.0f, 0) == 1)
    {
        selectCursorMove();
        mSelProc = PROC_MOJI_SELECT;
        field_0x2ad = mSelProc;
    }
}

void dName_c::MojiSelectAnm2() {
    if (mMojiIcon[mPrevRow + mPrevColumn * 5]->scaleAnime(mCursorDelay, g_nmHIO.mSelCharScale,
                                                          1.0f, 0) == 1)
    {
        menuCursorMove2();
        mSelProc = PROC_MENU_SELECT;
        field_0x2ad = mSelProc;
    }
}

void dName_c::MojiSelectAnm3() {}

int dName_c::mojiChange(u8 idx) {
    if (mChrInfo[idx].field_0x3 == 0 || mChrInfo[idx].mMojiSet == MOJI_EIGO ||
        mChrInfo[idx].mCharacter == SJIS('　', 0x8140U))
    {
        return 0;
    }

    if (mChrInfo[idx].mColumn == 4 || mChrInfo[idx].mColumn == 6 || mChrInfo[idx].mColumn == 8 ||
        mChrInfo[idx].mColumn == 9)
    {
        return 0;
    }

    switch (mChrInfo[idx].mColumn) {
    case 0:
    case 10: {
        if (mChrInfo[idx].mCharacter == SJIS('ウ', 0x8345U) || mChrInfo[idx].mCharacter == SJIS('ゥ', 0x8344U) ||
            mChrInfo[idx].mCharacter == SJIS('ヴ', 0x8394U))
        {
            mChrInfo[idx].mCharacter++;

            if (mChrInfo[idx].mCharacter == SJIS('ェ', 0x8346U)) {
                mChrInfo[idx].mCharacter = SJIS('ヴ', 0x8394U);
            }

            if (mChrInfo[idx].mCharacter == SJIS('ヵ', 0x8395U)) {
                mChrInfo[idx].mCharacter = SJIS('ゥ', 0x8344U);
            }
        } else {
            int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? SJIS('ァ', 0x8340U) : SJIS('ぁ', 0x829fU);

            if ((mChrInfo[idx].mCharacter - c) % 2) {
                --mChrInfo[idx].mCharacter;
            } else {
                ++mChrInfo[idx].mCharacter;
            }
        }
        break;
    }
    case 1: {
        int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? SJIS('カ', 0x834aU) : SJIS('か', 0x82a9U);
        c = ((mChrInfo[idx].mCharacter - c) % 2);

        int c2 = c + 1;
        mChrInfo[idx].mCharacter = (mChrInfo[idx].mCharacter - c) + (c2 & 1);
        break;
    }
    case 2: {
        int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? SJIS('サ', 0x8354U) : SJIS('さ', 0x82b3U);
        c = ((mChrInfo[idx].mCharacter - c) % 2);

        int c2 = c + 1;
        mChrInfo[idx].mCharacter = (mChrInfo[idx].mCharacter - c) + (c2 & 1);
        break;
    }
    case 3:
    case 12: {
        if (mChrInfo[idx].mCharacter != (u32)0x815b) {
            if (mChrInfo[idx].mCharacter <= (mChrInfo[idx].mMojiSet != MOJI_HIRA ? SJIS('ヂ', 0x8361U) : SJIS('ぢ', 0x82c0U))) {
                int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? SJIS('タ', 0x835eU) : SJIS('た', 0x82bdU);
                c = ((mChrInfo[idx].mCharacter - c) % 2);

                int c2 = c + 1;
                mChrInfo[idx].mCharacter = (mChrInfo[idx].mCharacter - c) + (c2 & 1);
            } else if (mChrInfo[idx].mCharacter <=
                           (mChrInfo[idx].mMojiSet != MOJI_HIRA ? SJIS('ド', 0x8368U) : SJIS('ど', 0x82c7U)) &&
                       mChrInfo[idx].mCharacter >=
                           (mChrInfo[idx].mMojiSet != MOJI_HIRA ? SJIS('テ', 0x8365U) : SJIS('て', 0x82c4U)))
            {
                int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? SJIS('テ', 0x8365U) : SJIS('て', 0x82c4U);
                c = ((mChrInfo[idx].mCharacter - c) % 2);

                int c2 = c + 1;
                mChrInfo[idx].mCharacter = (mChrInfo[idx].mCharacter - c) + (c2 & 1);
            } else {
                int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? SJIS('ッ', 0x8362U) : SJIS('っ', 0x82c1U);
                int c2 = (mChrInfo[idx].mCharacter - c) % 3;

                int ivar2 = c2 + 1;
                if (ivar2 > 2) {
                    ivar2 = 0;
                }

                mChrInfo[idx].mCharacter = ivar2 + (mChrInfo[idx].mCharacter - c2);
            }
        }
        break;
    }
    case 5: {
        int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? SJIS('ハ', 0x836eU) : SJIS('は', 0x82cdU);
        int c2 = (mChrInfo[idx].mCharacter - c) % 3;

        int ivar2 = c2 + 1;
        if (ivar2 > 2) {
            ivar2 = 0;
        }

        mChrInfo[idx].mCharacter = ivar2 + (mChrInfo[idx].mCharacter - c2);
        break;
    }
    case 7:
    case 11: {
        int c = mChrInfo[idx].mMojiSet != MOJI_HIRA ? SJIS('ャ', 0x8383U) : SJIS('ゃ', 0x82e1U);
        c = ((mChrInfo[idx].mCharacter - c) % 2);

        int c2 = c + 1;
        mChrInfo[idx].mCharacter = (mChrInfo[idx].mCharacter - c) + (c2 & 1);
        break;
    }
    }

    setNameText();
    return 1;
}

void dName_c::selectMojiSet() {
    #if TARGET_PC
    if (useChineseNameKeyboard() && mCharRow == 4) {
        int page = getChinesePageKey(mCharRow + mCharColumn * 5);
        if (page >= 0) {
            mDoAud_seStart(Z2SE_SY_DUMMY, NULL, 0, 0);
            sChineseNamePage = page;
            sShowingChineseNamePage = true;
            mojiListChange();
            return;
        }
    }
    #endif

    #if REGION_JPN
    int moji = getMoji();
    if (moji != -1) {
        if (moji == '゛' || moji == '゜') {
            if (mCurPos != 0) {
                if (checkDakuon(moji, mCurPos - 1) == 1) {
                    mDoAud_seStart(Z2SE_SY_NAME_INPUT, NULL, 0, 0);
                    setDakuon(moji, mCurPos - 1);
                } else {
                    mDoAud_seStart(Z2SE_SYS_ERROR, NULL, 0, 0);
                }
            }
        } else {
            setMoji(moji);
        }
    }
    setNameText();
    #else
    setMoji(getMoji());
    setNameText();
    #endif
}

#if REGION_JPN
int dName_c::checkDakuon(int param_0, u8 param_1) {
    if (mChrInfo[param_1].mMojiSet == MOJI_EIGO) {
        return 0;
    }

    if (param_1 == 0 && mChrInfo[param_1].field_0x3 == 0) {
        return 0;
    }

    if (param_0 == '゜' && mChrInfo[param_1].mColumn != 5) {
        return 0;
    }

    if (param_0 == '゛' &&
        (mChrInfo[param_1].mCharacter == 'ウ' || mChrInfo[param_1].mCharacter == 'ヴ'))
    {
        return 1;
    }

    if (param_0 == '゛' && mChrInfo[param_1].mColumn != 1 && mChrInfo[param_1].mColumn != 2 &&
        mChrInfo[param_1].mColumn != 3 && mChrInfo[param_1].mColumn != 5)
    {
        return 0;
    }

    return 1;
}

int dName_c::setDakuon(int param_1, u8 param_2) {
    int c;

    if (param_1 == '゛') {
        switch (mChrInfo[param_2].mColumn) {
        case 0: {
            c = -1;
            if (mChrInfo[param_2].mCharacter == 'ウ' || mChrInfo[param_2].mCharacter == 'ヴ') {
                c = 4;
                mChrInfo[param_2].mCharacter = 'ヴ';
            }
            break;
        }
        case 1: {
            int c2 = mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'カ' : 'か';
            c = (mChrInfo[param_2].mCharacter - c2) % 2;
            break;
        }
        case 2: {
            int c2 = mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'サ' : 'さ';
            c = (mChrInfo[param_2].mCharacter - c2) % 2;
            break;
        }
        case 3: {
            int c2;
            if (mChrInfo[param_2].mCharacter <=
                ((mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'ヂ' : 'ぢ')))
            {
                c2 = mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'タ' : 'た';
                c = (mChrInfo[param_2].mCharacter - c2) % 2;
            } else {
                if (mChrInfo[param_2].mCharacter <= (mChrInfo[param_2].mMojiSet != 0 ? 'ド' : 'ど'))
                {
                    if (mChrInfo[param_2].mCharacter >=
                        (mChrInfo[param_2].mMojiSet != 0 ? 'テ' : 'て'))
                    {
                        c2 = mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'テ' : 'て';
                        c = (mChrInfo[param_2].mCharacter - c2) % 2;
                        break;
                    }
                }

                c2 = mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'ッ' : 'っ';
                c = (mChrInfo[param_2].mCharacter - c2) % 3;
                if (c == 2) {
                    c = 1;
                } else if (c == 1) {
                    c = 0;
                } else if (c == 0) {
                    c = 3;
                }
            }
            break;
        }
        case 5: {
            int c2 = mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'ハ' : 'は';
            c = (mChrInfo[param_2].mCharacter - c2) % 3;
            break;
        }
        }

        if (c != 1) {
            if (c == 2) {
                mChrInfo[param_2].mCharacter -= 1;
            } else if (c == 0) {
                mChrInfo[param_2].mCharacter += 1;
            } else if (c == 3) {
                mChrInfo[param_2].mCharacter += 2;
            }

            setNameText();

            return 1;
        }
    } else if (param_1 == '゜') {
        int c2 = mChrInfo[param_2].mMojiSet != MOJI_HIRA ? 'ハ' : 'は';
        c = (mChrInfo[param_2].mCharacter - c2) % 3;
        if (c != 2) {
            mChrInfo[param_2].mCharacter = mChrInfo[param_2].mCharacter + (2 - c);
            setNameText();

            return 1;
        }
    }

    return 0;
}
#endif

int dName_c::getMoji() {
    int result = -1;
    const char* moji;
    int idx = mCharRow + mCharColumn * 5;

    #if TARGET_PC
    if (dusk::version::isRegionPal()) {
        switch (mMojiSet) {
        case MOJI_HIRA:
            moji = getPalNameKeyboardCell(useChineseNameKeyboard() ? l_mojiEisuPalChinese_1 : l_mojiEisuPal_1, idx);
            break;
        case MOJI_KATA:
            moji = getPalNameKeyboardCell(useChineseNameKeyboard() ? l_mojiEisuPalChinese_2 : l_mojiEisuPal_2, idx);
            break;
        default:
            abort();
        }
    } else {
        switch (mMojiSet) {
        case MOJI_HIRA:
            moji = l_mojiHira[mCharRow + mCharColumn * 5];
            break;
        case MOJI_KATA:
            moji = l_mojikata[mCharRow + mCharColumn * 5];
            break;
        case MOJI_EIGO:
            moji = l_mojiEisu[mCharRow + mCharColumn * 5];
            break;
        default:
            abort();
        }
    }
    #elif REGION_PAL
    switch (mMojiSet) {
    case MOJI_HIRA:
        moji = l_mojiEisuPal_1[mCharRow + mCharColumn * 5];
        break;
    case MOJI_KATA:
        moji = l_mojiEisuPal_2[mCharRow + mCharColumn * 5];
        break;
    }
    #else
    switch (mMojiSet) {
    case MOJI_HIRA:
        moji = l_mojiHira[mCharRow + mCharColumn * 5];
        break;
    case MOJI_KATA:
        moji = l_mojikata[mCharRow + mCharColumn * 5];
        break;
    case MOJI_EIGO:
        moji = l_mojiEisu[mCharRow + mCharColumn * 5];
        break;
    }
    #endif

    #if TARGET_PC
    if (useChineseNameKeyboard() && sShowingChineseNamePage) {
        if (l_mojiZh[sChineseNamePage][idx] != 0 && isShiftJisLeadByte(*(u8*)moji)) {
            result = twoByteCode(moji);
        } else {
            result = *moji;
        }
    } else if (dusk::version::isRegionJpn()) {
        if (*(u8*)moji >> 4 == 0x8 || *(u8*)moji >> 4 == 0x9) {
            result = *(u16*)moji;
        } else {
            result = *moji;
        }
    } else {
        result = *moji;
    }
    #elif REGION_JPN
    if (*(u8*)moji >> 4 == 0x8 || *(u8*)moji >> 4 == 0x9) {
        result = *(u16*)moji;
    } else {
        result = *(char*)moji;
    }
    #else
    result = *moji;
    #endif

    return result;
}

#if TARGET_PC
#define CHAR_TRUNC(val) (isTwoByteNameCharacter(val) ? val : (dusk::version::isRegionPal() ? val & 0xFF : val))
#elif REGION_PAL
#define CHAR_TRUNC(val) (val & 0xFF)
#else
#define CHAR_TRUNC(val) val
#endif

void dName_c::setMoji(int moji) {
    if (mCurPos == 8 || nameCheck() == 8) {
        mDoAud_seStart(Z2SE_SYS_ERROR, NULL, 0, 0);
    } else {
        mDoAud_seStart(Z2SE_SY_NAME_INPUT, NULL, 0, 0);

        s32 notEmpty = false;
        for (int i = mCurPos; i < 8; i++) {
            if (mChrInfo[i].mCharacter != SPACE_MAYBE_FULL) {
                notEmpty = true;
                break;
            }
        }

        if (notEmpty) {
            if (mChrInfo[7].mCharacter == SPACE_MAYBE_FULL) {
                for (int i = 6; i >= mCurPos; i--) {
                    mChrInfo[i + 1] = mChrInfo[i];
                }

                mChrInfo[mCurPos].mColumn = mCharColumn;
                mChrInfo[mCurPos].mRow = mCharRow;
                mChrInfo[mCurPos].mMojiSet = mMojiSet;
                mChrInfo[mCurPos].field_0x3 = 1;
                mChrInfo[mCurPos].mCharacter = CHAR_TRUNC(moji);

                if (mCurPos != 8) {
                    mLastCurPos = mCurPos;
                    mCurPos++;
                    nameCursorMove();
                }
            }
        } else {
            mChrInfo[mCurPos].mColumn = mCharColumn;
            mChrInfo[mCurPos].mRow = mCharRow;
            mChrInfo[mCurPos].mMojiSet = mMojiSet;
            mChrInfo[mCurPos].field_0x3 = 1;
            mChrInfo[mCurPos].mCharacter = CHAR_TRUNC(moji);

            if (mCurPos != 8) {
                mLastCurPos = mCurPos;
                mCurPos++;
                nameCursorMove();
            }
        }
    }
}


void dName_c::setNameText() {
    for (int i = 0; i < 8; i++) {
        //"\x1bCD\x1bCR\x1bCC[000000]\x1bGM[0]%c\x1bHM\x1bCC[ffffff]\x1bGM[0]%c"
        //"\x1bCD\x1bCR\x1bCC[000000]\x1bGM[0]%c%c\x1bHM\x1bCC[ffffff]\x1bGM[0]%c%c"
        if (mChrInfo[i].field_0x3 != 0) {
            #if TARGET_PC
            if (!isTwoByteNameCharacter(mChrInfo[i].mCharacter) &&
                (!dusk::version::isRegionJpn() || mChrInfo[i].mMojiSet == 2))
            #elif REGION_JPN
            if (mChrInfo[i].mMojiSet == 2)
            #endif
            #if TARGET_PC || REGION_JPN
            {
            #endif
                SAFE_SPRINTF(mNameText[i],
                        "\x1b"
                        "CD\x1b"
                        "CR\x1b"
                        "CC[000000]\x1bGM[0]%c\x1bHM\x1b"
                        "CC[ffffff]\x1bGM[0]%c",
                        CHAR_TRUNC((u8)mChrInfo[i].mCharacter),
                        CHAR_TRUNC((u8)mChrInfo[i].mCharacter)
                );
            #if TARGET_PC || REGION_JPN
            } else {
                SAFE_SPRINTF(mNameText[i],
                        "\x1b"
                        "CD\x1b"
                        "CR\x1b"
                        "CC[000000]\x1bGM[0]%c%c\x1bHM\x1b"
                        "CC[ffffff]\x1bGM[0]%c%c",
                        (mChrInfo[i].mCharacter & 0xff00) >> 8,
                        (mChrInfo[i].mCharacter & 0xff),
                        (mChrInfo[i].mCharacter & 0xff00) >> 8,
                        (mChrInfo[i].mCharacter & 0xff)
                );
            }
            #endif
        }
    }
}

void dName_c::nameCursorMove() {
    if (mCurPos <= 8) {
        u8 position = mCurPos;

        if (position > 7) {
            position = 7;
        }

        if (mLastCurPos != 255 && mLastCurPos < 8) {
            mNameCursor[mLastCurPos]->hide();
        }

        mNameCursor[position]->show();
    }
}

void dName_c::selectCursorMove() {
    int idx;
    #if TARGET_PC
    if (dusk::version::isRegionPal()) {
        if (useChineseNameKeyboard()) {
            if (sShowingChineseNamePage) {
                idx = MENU_KATA;
            } else if (mCharColumn < 3) {
                idx = MENU_HIRA;
            } else if (mCharColumn < 8) {
                idx = MENU_KATA;
            } else {
                idx = MENU_EIGO;
            }
        } else {
            if (mCharColumn < 3) {
                idx = 0;
            } else if (mCharColumn < 6) {
                idx = 1;
            } else if (mCharColumn >= 6) {
                idx = 2;
            }
        }
    } else if (dusk::version::isRegionJpn()) {
        if (mCharColumn < 3) {
            idx = 0;
        } else if (mCharColumn < 6) {
            idx = 1;
        } else if (mCharColumn < 8) {
            idx = 2;
        } else if (mCharColumn >= 8) {
            idx = 3;
        }
    } else {
        idx = 3;
    }
    #elif REGION_PAL
    if (mCharColumn < 3) {
        idx = 0;
    } else if (mCharColumn < 6) {
        idx = 1;
    } else if (mCharColumn >= 6) {
        idx = 2;
    }
    #elif REGION_JPN
    if (mCharColumn < 3) {
        idx = 0;
    } else if (mCharColumn < 6) {
        idx = 1;
    } else if (mCharColumn < 8) {
        idx = 2;
    } else if (mCharColumn >= 8) {
        idx = 3;
    }
    #else
    idx = 3;
    #endif
    field_0x30c[idx][0] = mCharColumn;
    field_0x30c[idx][1] = mCharRow;
    field_0x30c[idx][2] = 1;

    mMojiIcon[mCharRow + mCharColumn * 5]->getPanePtr()->scale(g_nmHIO.mSelCharScale,
                                                               g_nmHIO.mSelCharScale);
    ((J2DTextBox*)mMojiIcon[mCharRow + mCharColumn * 5]->getPanePtr())
        ->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));
    
    #if TARGET_PC
    nameWide();
    #endif

    Vec pos = mMojiIcon[mCharRow + mCharColumn * 5]->getGlobalVtxCenter(false, 0);
    mSelIcon->setPos(pos.x, pos.y, mMojiIcon[mCharRow + mCharColumn * 5]->getPanePtr(), true);
    mSelIcon->setAlphaRate(1.0f);
}

void dName_c::menuCursorPosSet() {
    mPrevSelMenu = mSelMenu;
    #if TARGET_PC
    if (dusk::version::isRegionPal()) {
        if (useChineseNameKeyboard()) {
            if (sShowingChineseNamePage) {
                mSelMenu = MENU_KATA;
            } else if (mCharColumn < 3) {
                mSelMenu = MENU_HIRA;
            } else if (mCharColumn < 8) {
                mSelMenu = MENU_KATA;
            } else {
                mSelMenu = MENU_EIGO;
            }
            return;
        }

        if (mCharColumn < 3) {
            mSelMenu = MENU_HIRA;
        } else if (mCharColumn < 6) {
            mSelMenu = MENU_KATA;
        } else if (mCharColumn >= 6) {
            mSelMenu = MENU_EIGO;
        }
    } else if (dusk::version::isRegionJpn()) {
        if (mCharColumn < 3) {
            mSelMenu = MENU_HIRA;
            return;
        }
        if (mCharColumn < 6) {
            mSelMenu = MENU_KATA;
            return;
        }
        if (mCharColumn < 8) {
            mSelMenu = MENU_EIGO;
            return;
        }
        if (mCharColumn >= 8) {
            mSelMenu = MENU_END;
            return;
        }
    } else {
        mSelMenu = MENU_END;
    }
    #elif REGION_PAL
    if (mCharColumn < 3) {
        mSelMenu = MENU_HIRA;
    } else if (mCharColumn < 6) {
        mSelMenu = MENU_KATA;
    } else if (mCharColumn >= 6) {
        mSelMenu = MENU_EIGO;
    }
    #elif REGION_JPN
    if (mCharColumn < 3) {
        mSelMenu = MENU_HIRA;
        return;
    }
    if (mCharColumn < 6) {
        mSelMenu = MENU_KATA;
        return;
    }
    if (mCharColumn < 8) {
        mSelMenu = MENU_EIGO;
        return;
    }
    if (mCharColumn >= 8) {
        mSelMenu = MENU_END;
        return;
    }
    #else
    mSelMenu = MENU_END;
    #endif
}

void dName_c::MenuSelect() {
    #if TARGET_PC || REGION_PAL || REGION_JPN
    if (isPalOrJpn() && stick->checkRightTrigger()) {
        mDoAud_seStart(Z2SE_SY_CURSOR_OPTION, NULL, 0, 0);
        mPrevSelMenu = mSelMenu;
        mSelMenu++;
        #if TARGET_PC
        if (useChineseNameKeyboard()) {
            if (mSelMenu > MENU_EIGO) {
                mSelMenu = MENU_HIRA;
            }
        } else
        #endif
        #if REGION_PAL
        if (mSelMenu > MENU_EIGO) {
        #else
        if (mSelMenu > EIGO_OR_END) {
        #endif
            mSelMenu = MENU_HIRA;
        }
        MenuSelectAnmInit();
        mSelProc = PROC_MENU_SEL_ANM;
    } else if (isPalOrJpn() && stick->checkLeftTrigger()) {
        mDoAud_seStart(Z2SE_SY_CURSOR_OPTION, NULL, 0, 0);
        mPrevSelMenu = mSelMenu;
        if (mSelMenu == MENU_HIRA) {
            #if REGION_JPN
            mSelMenu = MENU_END;
            #else
            mSelMenu = dusk::version::isRegionJpn() ? MENU_END : MENU_EIGO;
            #endif
        } else {
            mSelMenu--;
        }
        #if TARGET_PC
        if (useChineseNameKeyboard() && mSelMenu > MENU_EIGO) {
            mSelMenu = MENU_EIGO;
        }
        #endif
        MenuSelectAnmInit();
        mSelProc = PROC_MENU_SEL_ANM;
    } else {
    #else
    if (!stick->checkRightTrigger() && !stick->checkLeftTrigger()) {
    #endif
        if (stick->checkUpTrigger()) {
            mDoAud_seStart(Z2SE_SY_NAME_CURSOR, NULL, 0, 0);
            mPrevSelMenu = mSelMenu;
            selectCursorPosSet(4);
            MenuSelectAnmInit();
            mSelProc = PROC_MENU_SEL_ANM2;
        } else if (stick->checkDownTrigger()) {
            mDoAud_seStart(Z2SE_SY_NAME_CURSOR, NULL, 0, 0);
            mPrevSelMenu = mSelMenu;
            selectCursorPosSet(0);
            MenuSelectAnmInit();
            mSelProc = PROC_MENU_SEL_ANM2;
        } else if (mDoCPd_c::getTrigA(PAD_1)) {
            #if TARGET_PC
            if (useChineseNameKeyboard() ? mSelMenu == MENU_EIGO :
            #endif
            #if REGION_PAL
                mSelMenu == MENU_EIGO
            #else
                mSelMenu == EIGO_OR_END
            #endif
            #if TARGET_PC
            )
            #endif
            {
                if (nameCheck() != 0) {
                    mDoAud_seStart(Z2SE_SY_NAME_OK, NULL, 0, 0);
                } else {
                    mDoAud_seStart(Z2SE_SYS_ERROR, NULL, 0, 0);
                }
            } else {
                mDoAud_seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0);
            }
            menuAbtnSelect();
        } else if (mDoCPd_c::getTrigStart(PAD_1)) {
            #if TARGET_PC
            if (useChineseNameKeyboard() ? mSelMenu == MENU_EIGO :
            #endif
            #if REGION_PAL
                mSelMenu == MENU_EIGO
            #else
                mSelMenu == EIGO_OR_END
            #endif
            #if TARGET_PC
            )
            #endif
            {
                if (nameCheck() != 0) {
                    mDoAud_seStart(Z2SE_SY_NAME_OK, NULL, 0, 0);
                } else {
                    mDoAud_seStart(Z2SE_SYS_ERROR, NULL, 0, 0);
                }
                menuAbtnSelect();
            }
        }
    }
}

void dName_c::MenuSelectAnmInit() {
    mSelIcon->setAlphaRate(0.0f);

    int prevMenu_i = getMenuPosIdx(mPrevSelMenu);
    if (mMenuIcon[prevMenu_i] != NULL) {
        mMenuIcon[prevMenu_i]->scaleAnimeStart(0);
    }
}

void dName_c::MenuSelectAnm() {
    int prevMenu_i = getMenuPosIdx(mPrevSelMenu);

    if (mMenuIcon[prevMenu_i] == NULL ||
        mMenuIcon[prevMenu_i]->scaleAnime(mCursorDelay, g_nmHIO.mMenuScale, 1.0f, 0) == 1)
    {
        if (mMenuText[prevMenu_i] != NULL) {
            mMenuText[prevMenu_i]->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
        }
        menuCursorMove();
        mSelProc = PROC_MENU_SELECT;
        field_0x2ad = mSelProc;
    }
}

void dName_c::MenuSelectAnm2() {
    int prevMenu_i = getMenuPosIdx(mPrevSelMenu);
    int mojiSet_i = getMenuPosIdx(DUSK_IF_ELSE(getActiveNameMenu(), mMojiSet));

    bool canMove = true;
    if (prevMenu_i != mojiSet_i && mMenuIcon[prevMenu_i] != NULL) {
        canMove = mMenuIcon[prevMenu_i]->scaleAnime(mCursorDelay, g_nmHIO.mMenuScale, 1.0f, 0);
    }

    if (canMove == true) {
        if (prevMenu_i != mojiSet_i) {
            if (mMenuText[prevMenu_i] != NULL) {
                mMenuText[prevMenu_i]->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
            }
            #if TARGET_PC || REGION_PAL || REGION_JPN
            IF_DUSK_BLOCK(isPalOrJpn())
            if (mMenuIcon[mojiSet_i] != NULL && mMenuText[mojiSet_i] != NULL) {
                mMenuIcon[mojiSet_i]->scale(g_nmHIO.mMenuScale, g_nmHIO.mMenuScale);
                mMenuText[mojiSet_i]->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));
            }
            IF_DUSK_BLOCK_END
            #endif
        }
        selectCursorMove();
        mSelProc = PROC_MOJI_SELECT;
        field_0x2ad = mSelProc;
    }
}

void dName_c::MenuSelectAnm3() {}

void dName_c::menuAbtnSelect() {
#if TARGET_PC
    if (useChineseNameKeyboard() && isChineseNameMenu(mSelMenu)) {
        mPrevMojiSet = mMojiSet;
        mMojiSet = MOJI_KATA;
        sShowingChineseNamePage = true;
        mojiListChange();
        return;
    }

    if (dusk::version::isRegionPal() && mSelMenu == MENU_EIGO) {
        goto pal_eigo;
    }
#endif
    switch (mSelMenu) {
    case MENU_HIRA:
    case MENU_KATA:
    #if !REGION_PAL
    case MENU_EIGO:
    #endif
        #if TARGET_PC
        if (useChineseNameKeyboard()) {
            sShowingChineseNamePage = false;
        }
        #endif
        if (mSelMenu != mMojiSet) {
            mPrevMojiSet = mMojiSet;
            mMojiSet = mSelMenu;
            mojiListChange();
        #if TARGET_PC
        } else if (useChineseNameKeyboard()) {
            mojiListChange();
        #endif
        }
        break;
    #if REGION_PAL
    case MENU_EIGO:
    #else
    case MENU_END:
    #endif
        IF_DUSK(pal_eigo:)
        int nameNum = nameCheck();
        if (nameNum != 0) {
            playNameSet(nameNum);
            field_0x2ac = mSelProc;
            field_0x2ae = mSelProc;
            mSelProc = PROC_WAIT;
            mIsInputEnd = 2;
        } else {
            mDoAud_seStart(Z2SE_SY_DUMMY, NULL, 0, 0);
        }
        break;
    }
}

void dName_c::backSpace() {
    if (mCurPos != 0) {
        mDoAud_seStart(Z2SE_SY_NAME_DELETE, NULL, 0, 0);

        if (mCurPos == 8 && mChrInfo[7].mCharacter != SPACE_MAYBE_FULL) {
            mChrInfo[7].mColumn = 7;
            mChrInfo[7].mRow = 1;
            #if TARGET_PC
            mChrInfo[7].mMojiSet = isPalOrJpn() ? MOJI_HIRA : MOJI_EIGO;
            #elif REGION_PAL || REGION_JPN
            mChrInfo[7].mMojiSet = MOJI_HIRA;
            #else
            mChrInfo[7].mMojiSet = MOJI_EIGO;
            #endif
            mChrInfo[7].field_0x3 = 1;
            mChrInfo[7].mCharacter = SPACE_MAYBE_FULL;
        } else {
            for (int i = mCurPos - 1; i < 7; i++) {
                mChrInfo[i] = mChrInfo[i + 1];
            }
            mChrInfo[7].mColumn = 7;
            mChrInfo[7].mRow = 1;
#if TARGET_PC
            mChrInfo[7].mMojiSet = isPalOrJpn() ? MOJI_HIRA : MOJI_EIGO;
#elif REGION_PAL || REGION_JPN
            mChrInfo[7].mMojiSet = MOJI_HIRA;
            #else
            mChrInfo[7].mMojiSet = MOJI_EIGO;
            #endif
            mChrInfo[7].field_0x3 = 1;
            mChrInfo[7].mCharacter = SPACE_MAYBE_FULL;
        }

        setNameText();
        mLastCurPos = mCurPos;
        mCurPos--;
        nameCursorMove();
    }
}

void dName_c::mojiListChange() {
    #if TARGET_PC
    const char** mojiSet;
    if (dusk::version::isRegionPal()) {
        switch (mMojiSet) {
        case MOJI_HIRA:
            mojiSet = useChineseNameKeyboard() ? l_mojiEisuPalChinese_1 : l_mojiEisuPal_1;
            break;
        case MOJI_KATA:
            mojiSet = useChineseNameKeyboard() ? l_mojiEisuPalChinese_2 : l_mojiEisuPal_2;
            break;
        }
    } else {
        switch (mMojiSet) {
        case MOJI_HIRA:
            mojiSet = l_mojiHira;
            break;
        case MOJI_KATA:
            mojiSet = l_mojikata;
            break;
        case MOJI_EIGO:
            mojiSet = l_mojiEisu;
            break;
        }
    }
    #elif REGION_PAL
    char** mojiSet;

    switch (mMojiSet) {
    case MOJI_HIRA:
        mojiSet = l_mojiEisuPal_1;
        break;
    case MOJI_KATA:
        mojiSet = l_mojiEisuPal_2;
        break;
    }
    #else
    const char** mojiSet;

    switch (mMojiSet) {
    case MOJI_HIRA:
        mojiSet = l_mojiHira;
        break;
    case MOJI_KATA:
        mojiSet = l_mojikata;
        break;
    case MOJI_EIGO:
        mojiSet = l_mojiEisu;
        break;
    }
    #endif

    char buf[74];
    for (int i = 0; i < 65; i++) {
        #if TARGET_PC
        const char* moji = dusk::version::isRegionPal() ? getPalNameKeyboardCell(mojiSet, i) : mojiSet[i];
        #else
        const char* moji = mojiSet[i];
        #endif

        SAFE_STRCPY(buf, "\x1B"
                    "CD"
                    "\x1B"
                    "CR"
                    "\x1B"
                    "CC[000000]"
                    "\x1B"
                    "GM[0]");
        SAFE_STRCAT(buf, moji);
        SAFE_STRCAT(buf, "\x1B"
                    "HM"
                    "\x1B"
                    "CC[ffffff]"
                    "\x1B"
                    "GM[0]");
        SAFE_STRCAT(buf, moji);
        SAFE_STRCPY(mMojiText[i], buf);
    }

    #if TARGET_PC || REGION_PAL || REGION_JPN
    IF_DUSK_BLOCK(isPalOrJpn())
    if (mSelProc == PROC_MOJI_SELECT) {
        int mojiSet_i = getMenuPosIdx(DUSK_IF_ELSE(getActiveNameMenu(), mMojiSet));
        if (mMenuIcon[mojiSet_i] != NULL && mMenuText[mojiSet_i] != NULL) {
            mMenuIcon[mojiSet_i]->scale(g_nmHIO.mMenuScale, g_nmHIO.mMenuScale);
            mMenuText[mojiSet_i]->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));
        }
        if (mPrevMojiSet != 255) {
            int prevMojiSet_i = getMenuPosIdx(mPrevMojiSet);
            if (mMenuIcon[prevMojiSet_i] != NULL && mMenuText[prevMojiSet_i] != NULL) {
                mMenuIcon[prevMojiSet_i]->scale(1.0f, 1.0f);
                mMenuText[prevMojiSet_i]->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
            }
        }
    }
    IF_DUSK_BLOCK_END
    #endif
}

void dName_c::menuCursorMove() {
    int menu_i = getMenuPosIdx(mSelMenu);
    if (mMenuIcon[menu_i] == NULL || mMenuText[menu_i] == NULL) {
        mSelIcon->setAlphaRate(0.0f);
        return;
    }

    mMenuIcon[menu_i]->scale(g_nmHIO.mMenuScale, g_nmHIO.mMenuScale);
    mMenuText[menu_i]->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));

    Vec pos = mMenuIcon[menu_i]->getGlobalVtxCenter(false, 0);
    mSelIcon->setPos(pos.x, pos.y, mMenuIcon[menu_i]->getPanePtr(), true);
    mSelIcon->setAlphaRate(1.0f);
}

void dName_c::menuCursorMove2() {
    int menu_i = getMenuPosIdx(mSelMenu);
    int mojiSet_i = getMenuPosIdx(DUSK_IF_ELSE(getActiveNameMenu(), mMojiSet));

    if (mMenuIcon[menu_i] == NULL || mMenuText[menu_i] == NULL) {
        mSelIcon->setAlphaRate(0.0f);
        return;
    }

    if (menu_i != mojiSet_i) {
        mMenuIcon[menu_i]->scale(g_nmHIO.mMenuScale, g_nmHIO.mMenuScale);
        mMenuText[menu_i]->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));
        #if TARGET_PC || REGION_PAL || REGION_JPN
        IF_DUSK_BLOCK(isPalOrJpn())
        if (mMenuIcon[mojiSet_i] != NULL && mMenuText[mojiSet_i] != NULL) {
            mMenuIcon[mojiSet_i]->scale(1.0f, 1.0f);
            mMenuText[mojiSet_i]->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
        }
        IF_DUSK_BLOCK_END
        #endif
    }

    Vec pos = mMenuIcon[menu_i]->getGlobalVtxCenter(false, 0);
    mSelIcon->setPos(pos.x, pos.y, mMenuIcon[menu_i]->getPanePtr(), true);
    mSelIcon->setAlphaRate(1.0f);
}

void dName_c::selectCursorPosSet(int row) {
    if (field_0x30c[mSelMenu][2] == 1) {
        mCharColumn = field_0x30c[mSelMenu][0];
        mCharRow = row;
    } else {
        switch (mSelMenu) {
        case MENU_HIRA:
            mCharColumn = 0;
            break;
        case MENU_KATA:
            mCharColumn = 3;
            break;
        case MENU_EIGO:
            #if TARGET_PC
            mCharColumn = dusk::version::isRegionPal() ? 8 : 6;
            #elif REGION_PAL
            mCharColumn = 8;
            #else
            mCharColumn = 6;
            #endif
            break;
        case MENU_END:
            #if TARGET_PC
            mCharColumn = useChineseNameKeyboard() ? 9 : 8;
            #else
            mCharColumn = 8;
            #endif
            break;
        }

        mCharRow = row;
        field_0x30c[mSelMenu][0] = mCharColumn;
        field_0x30c[mSelMenu][1] = mCharRow;
        field_0x30c[mSelMenu][2] = 1;
    }
}

#if TARGET_PC
void dName_c::nameWide() {
    // Resize Select Icon
    #if TARGET_PC
    if (mSelIcon) {
        mSelIcon->refreshAspectScale(mDoGph_gInf_c::hudAspectScaleUp);
    }
    #endif

    // List of Characters Box
    static u64 l_tagName[65] = {
        MULTI_CHAR('m_00_0'), MULTI_CHAR('m_00_1'), MULTI_CHAR('m_00_2'), MULTI_CHAR('m_00_3'), MULTI_CHAR('m_00_4'), MULTI_CHAR('m_01_0'), MULTI_CHAR('m_01_1'), MULTI_CHAR('m_01_2'), MULTI_CHAR('m_01_3'),
        MULTI_CHAR('m_01_4'), MULTI_CHAR('m_02_0'), MULTI_CHAR('m_02_1'), MULTI_CHAR('m_02_2'), MULTI_CHAR('m_02_3'), MULTI_CHAR('m_02_4'), MULTI_CHAR('m03_0'),  MULTI_CHAR('m03_1'),  MULTI_CHAR('m03_2'),
        MULTI_CHAR('m03_3'),  MULTI_CHAR('m03_4'),  MULTI_CHAR('m_04_0'), MULTI_CHAR('m_04_1'), MULTI_CHAR('m_04_2'), MULTI_CHAR('m_04_3'), MULTI_CHAR('m_04_4'), MULTI_CHAR('m_05_0'), MULTI_CHAR('m_05_1'),
        MULTI_CHAR('m_05_2'), MULTI_CHAR('m_05_3'), MULTI_CHAR('m_05_4'), MULTI_CHAR('m_06_0'), MULTI_CHAR('m_06_1'), MULTI_CHAR('m_06_2'), MULTI_CHAR('m_06_3'), MULTI_CHAR('m_06_4'), MULTI_CHAR('m_07_0'),
        MULTI_CHAR('m_07_1'), MULTI_CHAR('m_07_2'), MULTI_CHAR('m_07_3'), MULTI_CHAR('m_07_4'), MULTI_CHAR('m_08_0'), MULTI_CHAR('m_08_1'), MULTI_CHAR('m_08_2'), MULTI_CHAR('m_08_3'), MULTI_CHAR('m_08_4'),
        MULTI_CHAR('m_09_0'), MULTI_CHAR('m_09_1'), MULTI_CHAR('m_09_2'), MULTI_CHAR('m_09_3'), MULTI_CHAR('m_09_4'), MULTI_CHAR('m_10_0'), MULTI_CHAR('m_10_1'), MULTI_CHAR('m_10_2'), MULTI_CHAR('m_10_3'),
        MULTI_CHAR('m_10_4'), MULTI_CHAR('m_11_0'), MULTI_CHAR('m_11_1'), MULTI_CHAR('m_11_2'), MULTI_CHAR('m_11_3'), MULTI_CHAR('m_11_4'), MULTI_CHAR('m12_0'),  MULTI_CHAR('m12_1'),  MULTI_CHAR('m12_2'),
        MULTI_CHAR('m12_3'),  MULTI_CHAR('m12_4'),
    };

    for (u32 i = 0; i < 65; i++) {
        nameIn.NameInScr->search(l_tagName[i])->scale(mDoGph_gInf_c::hudAspectScaleDown, 1.0f);
    }

    // "END" Text
    nameIn.NameInScr->search(MULTI_CHAR('p_end_2'))->scale(mDoGph_gInf_c::hudAspectScaleDown, 1.0f);
    nameIn.NameInScr->search(MULTI_CHAR('p_end_1'))->scale(mDoGph_gInf_c::hudAspectScaleDown, 1.0f);
    nameIn.NameInScr->search(MULTI_CHAR('p_end_0'))->scale(mDoGph_gInf_c::hudAspectScaleDown, 1.0f);

    // Letters being typed
    static u64 l_nameTagName[8] = {
        MULTI_CHAR('name_00'), MULTI_CHAR('name_01'), MULTI_CHAR('name_02'), MULTI_CHAR('name_03'), MULTI_CHAR('name_04'), MULTI_CHAR('name_05'), MULTI_CHAR('name_06'), MULTI_CHAR('name_07'),
    };

    for (u32 i = 0; i < 8; i++) {
        nameIn.NameInScr->search(l_nameTagName[i])->scale(mDoGph_gInf_c::hudAspectScaleDown, 1.0f);
    }

    // Underscores when typing below letters
    static u64 l_nameCurTagName[8] = {
        MULTI_CHAR('s__n_00'), MULTI_CHAR('s__n_01'), MULTI_CHAR('s__n_02'), MULTI_CHAR('s__n_03'), MULTI_CHAR('s__n_04'), MULTI_CHAR('s__n_05'), MULTI_CHAR('s__n_06'), MULTI_CHAR('s__n_07'),
    };

    for (u32 i = 0; i < 8; i++) {
        nameIn.NameInScr->search(l_nameCurTagName[i])
            ->scale(mDoGph_gInf_c::hudAspectScaleDown, 1.0f);
    }
}
#endif

void dName_c::_draw() {
    #if TARGET_PC
    nameWide();
    #endif

    dComIfGd_set2DOpa(&nameIn);
    dComIfGd_set2DOpa(mSelIcon);
}

void dName_c::screenSet() {
    static u64 l_cur0TagName[8] = {
        's_00', 's_01', 's_02', 's_03', 's_04', 's_05', 's_06', 's_07',
    };
    static u64 l_cur1TagName[8] = {
        's_0r', MULTI_CHAR('s_01r'), MULTI_CHAR('s_02r'), MULTI_CHAR('s_03r'), MULTI_CHAR('s_04r'), MULTI_CHAR('s_05r'), MULTI_CHAR('s_06r'), MULTI_CHAR('s_07r'),
    };
#if REGION_JPN
    static u64 l_menu_icon_tag[4] = {
        MULTI_CHAR('j_hira_n'),
        MULTI_CHAR('j_kata_n'),
        MULTI_CHAR('j_eigo_n'),
        MULTI_CHAR('j_end_n'),
    };
    static u64 l_menu_tag[5][3] = {
        MULTI_CHAR('m_hira_0'),  MULTI_CHAR('m_hira_1'),  MULTI_CHAR('m_hira_s'),  MULTI_CHAR('m_kata_0'), MULTI_CHAR('m_kata_1'), MULTI_CHAR('m_kata_s'),
        MULTI_CHAR('m_eigo_0'), MULTI_CHAR('m_eigo_1'), MULTI_CHAR('m_eigo_s'), MULTI_CHAR('j_end_0'), MULTI_CHAR('j_end_1'), MULTI_CHAR('j_end_s'),
    };
    static u32 l_menu_msg[4] = {
        0x386,
        0x387,
        0x388,
        0x38A,
    };
    #else
    static u64 l_menu_icon_tag[4] = {
        MULTI_CHAR('p_ABC_n'),
        MULTI_CHAR('p_abc_n'),
        MULTI_CHAR('j_eigo_n'),
        MULTI_CHAR('p_end_n'),
    };
    static u64 l_menu_tag[5][3] = {
        MULTI_CHAR('p_ABC_0'),  MULTI_CHAR('p_ABC_1'),  MULTI_CHAR('p_ABC_2'),  MULTI_CHAR('p_abc_0'), MULTI_CHAR('p_abc_1'), MULTI_CHAR('p_abc_2'),
        MULTI_CHAR('m_eigo_0'), MULTI_CHAR('m_eigo_1'), MULTI_CHAR('m_eigo_2'), MULTI_CHAR('p_end_0'), MULTI_CHAR('p_end_1'), MULTI_CHAR('p_end_2'),
    };
    static u32 l_menu_msg[4] = {
        0x38B,
        0x38C,
        0x388,
        0x38E,
    };
#endif
    static u64 l_tagName[65] = {
        MULTI_CHAR('m_00_0'), MULTI_CHAR('m_00_1'), MULTI_CHAR('m_00_2'), MULTI_CHAR('m_00_3'), MULTI_CHAR('m_00_4'), MULTI_CHAR('m_01_0'), MULTI_CHAR('m_01_1'), MULTI_CHAR('m_01_2'), MULTI_CHAR('m_01_3'),
        MULTI_CHAR('m_01_4'), MULTI_CHAR('m_02_0'), MULTI_CHAR('m_02_1'), MULTI_CHAR('m_02_2'), MULTI_CHAR('m_02_3'), MULTI_CHAR('m_02_4'), MULTI_CHAR('m03_0'),  MULTI_CHAR('m03_1'),  MULTI_CHAR('m03_2'),
        MULTI_CHAR('m03_3'),  MULTI_CHAR('m03_4'),  MULTI_CHAR('m_04_0'), MULTI_CHAR('m_04_1'), MULTI_CHAR('m_04_2'), MULTI_CHAR('m_04_3'), MULTI_CHAR('m_04_4'), MULTI_CHAR('m_05_0'), MULTI_CHAR('m_05_1'),
        MULTI_CHAR('m_05_2'), MULTI_CHAR('m_05_3'), MULTI_CHAR('m_05_4'), MULTI_CHAR('m_06_0'), MULTI_CHAR('m_06_1'), MULTI_CHAR('m_06_2'), MULTI_CHAR('m_06_3'), MULTI_CHAR('m_06_4'), MULTI_CHAR('m_07_0'),
        MULTI_CHAR('m_07_1'), MULTI_CHAR('m_07_2'), MULTI_CHAR('m_07_3'), MULTI_CHAR('m_07_4'), MULTI_CHAR('m_08_0'), MULTI_CHAR('m_08_1'), MULTI_CHAR('m_08_2'), MULTI_CHAR('m_08_3'), MULTI_CHAR('m_08_4'),
        MULTI_CHAR('m_09_0'), MULTI_CHAR('m_09_1'), MULTI_CHAR('m_09_2'), MULTI_CHAR('m_09_3'), MULTI_CHAR('m_09_4'), MULTI_CHAR('m_10_0'), MULTI_CHAR('m_10_1'), MULTI_CHAR('m_10_2'), MULTI_CHAR('m_10_3'),
        MULTI_CHAR('m_10_4'), MULTI_CHAR('m_11_0'), MULTI_CHAR('m_11_1'), MULTI_CHAR('m_11_2'), MULTI_CHAR('m_11_3'), MULTI_CHAR('m_11_4'), MULTI_CHAR('m12_0'),  MULTI_CHAR('m12_1'),  MULTI_CHAR('m12_2'),
        MULTI_CHAR('m12_3'),  MULTI_CHAR('m12_4'),
    };
    static u64 l_nameTagName[8] = {
        MULTI_CHAR('name_00'), MULTI_CHAR('name_01'), MULTI_CHAR('name_02'), MULTI_CHAR('name_03'), MULTI_CHAR('name_04'), MULTI_CHAR('name_05'), MULTI_CHAR('name_06'), MULTI_CHAR('name_07'),
    };
    static u64 l_nameCurTagName[8] = {
        MULTI_CHAR('s__n_00'), MULTI_CHAR('s__n_01'), MULTI_CHAR('s__n_02'), MULTI_CHAR('s__n_03'), MULTI_CHAR('s__n_04'), MULTI_CHAR('s__n_05'), MULTI_CHAR('s__n_06'), MULTI_CHAR('s__n_07'),
    };

    nameIn.NameInScr = JKR_NEW J2DScreen();
    JUT_ASSERT(0, nameIn.NameInScr != NULL);

    archive = dComIfGp_getNameResArchive();
    nameIn.NameInScr->setPriority("zelda_player_name.blo", 0x100000, archive);
    dPaneClass_showNullPane(nameIn.NameInScr);
    nameIn.field_0x10 = nameIn.NameInScr->search(MULTI_CHAR('name_n'));

    void* bpk = JKRGetNameResource("zelda_player_name.bpk", archive);
    JUT_ASSERT(0, bpk != NULL);
    mCursorColorKey = (J2DAnmColorKey*)J2DAnmLoaderDataBase::load(bpk);
    mCursorColorKey->searchUpdateMaterialID(nameIn.NameInScr);

    void* btk = JKRGetNameResource("zelda_player_name.btk", archive);
    JUT_ASSERT(0, btk != NULL);
    mCursorTexKey = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(btk);
    mCursorTexKey->searchUpdateMaterialID(nameIn.NameInScr);

    J2DPane* panes0[8];
    J2DPane* panes1[8];
    for (int i = 0; i < 8; i++) {
        panes0[i] = nameIn.NameInScr->search(l_cur0TagName[i]);
        panes1[i] = nameIn.NameInScr->search(l_cur1TagName[i]);

        panes0[i]->setAnimation(mCursorTexKey);
        panes0[i]->setAnimation(mCursorColorKey);
        panes1[i]->setAnimation(mCursorTexKey);
        panes1[i]->setAnimation(mCursorColorKey);
    }

    #if REGION_JPN
    nameIn.NameInScr->search(MULTI_CHAR('pal_n'))->hide();
    mMenuPane = nameIn.NameInScr->search(MULTI_CHAR('jpn_n'));
    mMenuPane->show();

    nameIn.NameInScr->search(MULTI_CHAR('p_ABC_n'))->scale(0.0f, 0.0f);
    nameIn.NameInScr->search(MULTI_CHAR('p_abc_n'))->scale(0.0f, 0.0f);
    nameIn.NameInScr->search(MULTI_CHAR('p_end_n'))->scale(0.0f, 0.0f);
    #else
    nameIn.NameInScr->search(MULTI_CHAR('jpn_n'))->hide();
    mMenuPane = nameIn.NameInScr->search(MULTI_CHAR('pal_n'));
    mMenuPane->show();

    nameIn.NameInScr->search(MULTI_CHAR('j_hira_n'))->scale(0.0f, 0.0f);
    nameIn.NameInScr->search(MULTI_CHAR('j_kata_n'))->scale(0.0f, 0.0f);
    nameIn.NameInScr->search(MULTI_CHAR('j_eigo_n'))->scale(0.0f, 0.0f);
    nameIn.NameInScr->search(MULTI_CHAR('j_end_n'))->scale(0.0f, 0.0f);
    #endif

    J2DTextBox* menuPane[3];
    for (int i = 0; i < 4; i++) {
        mMenuIcon[i] = NULL;
        mMenuText[i] = NULL;
        #if !REGION_JPN
        if (i == 2) {
        } else {
        #endif
            mMenuIcon[i] = JKR_NEW CPaneMgr(nameIn.NameInScr, l_menu_icon_tag[i], 1, NULL);

            char buf[16];
            if (useChineseNameKeyboard() && i == 1) {
                strcpy(buf, "\x92\x86\x95\xB6");
            } else {
                fopMsgM_messageGet(buf, l_menu_msg[i]);
            }

            for (int j = 0; j < 3; j++) {
                menuPane[j] = (J2DTextBox*)nameIn.NameInScr->search(l_menu_tag[i][j]);
                if (menuPane[j] == NULL) {
                    continue;
                }

                if (mMenuText[i] == NULL) {
                    mMenuText[i] = menuPane[j];
                }

                menuPane[j]->setFont(nameIn.font);
                menuPane[j]->setString(buf);
            }
        #if !REGION_JPN
        }
        #endif
    }

    #if TARGET_PC || !(REGION_PAL || REGION_JPN)
    IF_DUSK_BLOCK(!isPalOrJpn())
    mMenuIcon[0]->hide();
    mMenuIcon[1]->hide();
    IF_DUSK_BLOCK_END
    #endif
    mMojiPane = nameIn.NameInScr->search(MULTI_CHAR('moji_n'));

    for (u32 i = 0; i < 65; i++) {
        mMojiIcon[i] = JKR_NEW CPaneMgr(nameIn.NameInScr, l_tagName[i], 2, NULL);
        ((J2DTextBox*)mMojiIcon[i]->getPanePtr())->setFont(nameIn.font);
        ((J2DTextBox*)mMojiIcon[i]->getPanePtr())->setString(72, "");
        mMojiText[i] = ((J2DTextBox*)mMojiIcon[i]->getPanePtr())->getStringPtr();
    }

    J2DPane* nameTagPane[8];
    for (int i = 0; i < 8; i++) {
        mNameCursor[i] = JKR_NEW CPaneMgrAlpha(nameIn.NameInScr, l_nameCurTagName[i], 2, NULL);
        nameTagPane[i] = nameIn.NameInScr->search(l_nameTagName[i]);
        ((J2DTextBox*)nameTagPane[i])->setFont(nameIn.font);
        ((J2DTextBox*)nameTagPane[i])->setString(72, "");
        ((J2DTextBox*)nameTagPane[i])->setWhite(JUtility::TColor(0xC8, 0xC8, 0xC8, 0xFF));
        #if TARGET_PC || REGION_PAL
        IF_DUSK_BLOCK(dusk::version::isRegionPal())
        ((J2DTextBox*)nameTagPane[i])->resize(24.0f, 23.0f);
        IF_DUSK_BLOCK_END
        #endif
        mNameText[i] = ((J2DTextBox*)nameTagPane[i])->getStringPtr();
    }

    #if REGION_PAL && !TARGET_PC // DUSK version note: this code mutates strings. We just edit the table.
    IF_DUSK_BLOCK(dusk::version::isRegionPal())
    int idx = 2;

    static const u8 palMoji00[13] = {
        0xC0, 0xC1, 0xC2, 0xC4, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE,
    };
    static const u8 palMoji01[13] = {
        0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD6, 0x8C, 0xD9, 0xDA, 0xDB, 0xDC, 0x2D,
    };
    static const u8 palMoji10[13] = {
        0xE0, 0xE1, 0xE2, 0xE4, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE,
    };
    static const u8 palMoji11[13] = {
        0xEF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF6, 0x9C, 0xF9, 0xFA, 0xFB, 0xFC, 0xDF,
    };

    for (int i = 0; i < 13; i++, idx += 5) {
        l_mojiEisuPal_1[idx][0] = palMoji00[i];
        l_mojiEisuPal_1[idx][1] = 0;

        l_mojiEisuPal_1[idx + 1][0] = palMoji01[i];
        l_mojiEisuPal_1[idx + 1][1] = 0;

        l_mojiEisuPal_2[idx][0] = palMoji10[i];
        l_mojiEisuPal_2[idx][1] = 0;

        l_mojiEisuPal_2[idx + 1][0] = palMoji11[i];
        l_mojiEisuPal_2[idx + 1][1] = 0;
    }
    IF_DUSK_BLOCK_END
    #endif

    mCharColumn = 0;
    mCharRow = 0;

    mSelIcon = JKR_NEW dSelect_cursor_c(0, 1.0f, NULL);
    JUT_ASSERT(0, mSelIcon != NULL);

    mSelIcon->setParam(0.82f, 0.77f, 0.05f, 0.4f, 0.4f);

    Vec pos = mMojiIcon[mCharRow + mCharColumn * 5]->getGlobalVtxCenter(false, 0);
    mSelIcon->setPos(pos.x, pos.y, mMojiIcon[mCharRow + mCharColumn * 5]->getPanePtr(), true);
    mSelIcon->setAlphaRate(0.0f);
}


void dName_c::displayInit() {
    mSelIcon->setAlphaRate(0.0f);
    mCurColAnmF = 0;
    mCurTexAnmF = 0;
    mSelProc = field_0x2ac;
    field_0x2ad = field_0x2ae;

    for (int i = 0; i < 65; i++) {
        ((J2DTextBox*)mMojiIcon[i]->getPanePtr())
            ->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
    }

    for (int i = 0; i < 4; i++) {
        if (mMenuText[i] != NULL) {
            mMenuText[i]->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xFF));
        }
    }

    for (int i = 0; i < 8; i++) {
        mNameCursor[i]->hide();
        mChrInfo[i].mColumn = 7;
        mChrInfo[i].mRow = 1;
        #if TARGET_PC
        mChrInfo[i].mMojiSet = isPalOrJpn() ? MOJI_HIRA : MOJI_EIGO;
        #elif REGION_PAL || REGION_JPN
        mChrInfo[i].mMojiSet = MOJI_HIRA;
        #else
        mChrInfo[i].mMojiSet = MOJI_EIGO;
        #endif
        mChrInfo[i].field_0x3 = 1;
        mChrInfo[i].mCharacter = SPACE_MAYBE_FULL;
    }

    mIsInputEnd = false;
}

void dName_c::NameStrSet() {
    char* moji = mNextNameStr;

    int i = 0;
    while (*moji != 0) {
        #if TARGET_PC
        if (useChineseNameKeyboard() && isShiftJisLeadByte(*(u8*)moji)) {
            mChrInfo[i].mCharacter = twoByteCode(moji);

            for (int page = 0; page < CHINESE_NAME_PAGE_COUNT; page++) {
                for (int j = 0; j < CHINESE_NAME_PAGE_CHARS; j++) {
                    if (mChrInfo[i].mCharacter == l_mojiZh[page][j]) {
                        sChineseNamePage = page;
                        sShowingChineseNamePage = true;
                        mChrInfo[i].mColumn = j / 5;
                        mChrInfo[i].mRow = j % 5;
                        mChrInfo[i].mMojiSet = MOJI_KATA;
                        goto found_chinese_name_char;
                    }
                }
            }
found_chinese_name_char:
            moji += 2;
            i++;
        } else if (dusk::version::isRegionPal()) {
            mChrInfo[i].mCharacter = static_cast<u8>(*moji);

            for (int j = 0; j < 65; j++) {
                if (mChrInfo[i].mCharacter == *(u8*)getPalNameKeyboardTableCell(true, j) ||
                    mChrInfo[i].mCharacter == *(u8*)getPalNameKeyboardTableCell(false, j))
                {
                    mChrInfo[i].mColumn = j / 5;
                    mChrInfo[i].mRow = j % 5;
                    mChrInfo[i].mMojiSet = MOJI_HIRA;
                    break;
                }
            }
            moji++;
            i++;
        } else {
            if (*(u8*)moji >> 4 == 8 || *(u8*)moji >> 4 == 9) {
                mChrInfo[i].mCharacter = *(u16*)moji;

                for (int j = 0; j < 65; j++) {
                    if (mChrInfo[i].mCharacter == *(u16*)l_mojiHira[j] ||
                        mChrInfo[i].mCharacter == *(u16*)l_mojiHira2[j] ||
                        mChrInfo[i].mCharacter == *(u16*)l_mojiHira3[j])
                    {
                        mChrInfo[i].mColumn = j / 5;
                        mChrInfo[i].mRow = j % 5;
                        mChrInfo[i].mMojiSet = MOJI_HIRA;
                        break;
                    } else if (mChrInfo[i].mCharacter == *(u16*)l_mojikata[j] ||
                               mChrInfo[i].mCharacter == *(u16*)l_mojikata2[j] ||
                               mChrInfo[i].mCharacter == *(u16*)l_mojikata3[j])
                    {
                        mChrInfo[i].mColumn = j / 5;
                        mChrInfo[i].mRow = j % 5;
                        mChrInfo[i].mMojiSet = MOJI_KATA;
                        break;
                    }
                }
                moji += 2;
                i++;
            } else {
                mChrInfo[i].mCharacter = *moji;

                for (int j = 0; j < 65; j++) {
                    if (mChrInfo[i].mCharacter == *(u8*)l_mojiEisu[j]) {
                        mChrInfo[i].mColumn = j / 5;
                        mChrInfo[i].mRow = j % 5;
                        mChrInfo[i].mMojiSet = MOJI_EIGO;
                        break;
                    }
                }
                moji++;
                i++;
            }
        }
        #elif REGION_PAL
        mChrInfo[i].mCharacter = static_cast<u8>(*moji);

        for (int j = 0; j < 65; j++) {
            if (mChrInfo[i].mCharacter == *(u8*)l_mojiEisuPal_1[j] ||
                mChrInfo[i].mCharacter == *(u16*)l_mojiEisuPal_2[j])
            {
                mChrInfo[i].mColumn = j / 5;
                mChrInfo[i].mRow = j % 5;
                mChrInfo[i].mMojiSet = MOJI_HIRA;
                break;
            }
        }
        moji++;
        i++;
        #else
        if (*(u8*)moji >> 4 == 8 || *(u8*)moji >> 4 == 9) {
            mChrInfo[i].mCharacter = *(u16*)moji;

            for (int j = 0; j < 65; j++) {
                if (mChrInfo[i].mCharacter == *(u16*)l_mojiHira[j] ||
                    mChrInfo[i].mCharacter == *(u16*)l_mojiHira2[j] ||
                    mChrInfo[i].mCharacter == *(u16*)l_mojiHira3[j])
                {
                    mChrInfo[i].mColumn = j / 5;
                    mChrInfo[i].mRow = j % 5;
                    mChrInfo[i].mMojiSet = MOJI_HIRA;
                    break;
                } else if (mChrInfo[i].mCharacter == *(u16*)l_mojikata[j] ||
                           mChrInfo[i].mCharacter == *(u16*)l_mojikata2[j] ||
                           mChrInfo[i].mCharacter == *(u16*)l_mojikata3[j])
                {
                    mChrInfo[i].mColumn = j / 5;
                    mChrInfo[i].mRow = j % 5;
                    mChrInfo[i].mMojiSet = MOJI_KATA;
                    break;
                }
            }
            moji += 2;
            i++;
        } else {
            mChrInfo[i].mCharacter = *moji;

            for (int j = 0; j < 65; j++) {
                if (mChrInfo[i].mCharacter == *(u8*)l_mojiEisu[j]) {
                    mChrInfo[i].mColumn = j / 5;
                    mChrInfo[i].mRow = j % 5;
                    mChrInfo[i].mMojiSet = MOJI_EIGO;
                    break;
                }
            }
            moji++;
            i++;
        }
        #endif
    }

    mLastCurPos = mCurPos;
    mCurPos = i;
    setNameText();
    nameCursorMove();
}

s32 dName_c::getMenuPosIdx(u8 selPos) {
    s32 result;
    switch (selPos) {
    case 0:
        result = 0;
        break;
    case 1:
        result = 1;
        break;
    case 2:
        #if TARGET_PC
        result = dusk::version::isRegionPal() ? 3 : 2;
        #elif REGION_PAL
        result = 3;
        #else
        result = 2;
        #endif
        break;
    case 3:
        result = 3;
        break;
    }
    return result;
    //!@bug UB: uninitialized default return
}

void dDlst_NameIN_c::draw() {
    if (field_0xc != NULL) {
        Mtx m;
        MtxP global_mtx = (MtxP)&field_0xc->getGlbMtx()[0][0];  // fake match?

        MTXScale(m, (field_0xc->getWidth() / field_0x10->getWidth()),
                 (field_0xc->getHeight() / field_0x10->getHeight()), 1.0f);
        MTXConcat(global_mtx, m, global_mtx);
        field_0x10->setMtx(global_mtx);
    }

    J2DGrafContext* graf_ctx = dComIfGp_getCurrentGrafPort();
    NameInScr->draw(0.0f, 0.0f, graf_ctx);
}
