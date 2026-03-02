#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/string_TRK.h"

int TRK_strlen(const char *str) {
    // NONMATCHING for Wii - for some reason Wii uses cmplwi instead of cmpwi
    int n = -1;
    do {
        n++;
    } while (((unsigned char*)str)[n] != '\0');
    return n;
}
