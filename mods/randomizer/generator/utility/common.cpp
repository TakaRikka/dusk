#include "common.hpp"

std::string readNullTerminatedStr(std::istream& in, const unsigned int& offset) {
    in.seekg(offset, std::ios::beg);

    std::string ret;
    char character = '\0';
    do {
        if (!in.read(&character, sizeof(char))) {
            ret.clear();
            return ret;
        }
        ret += character;
    } while (character != '\0');

    return ret;
}

std::u16string readNullTerminatedWStr(std::istream& in, const unsigned int offset) {
    in.seekg(offset, std::ios::beg);

    std::u16string ret;
    char16_t character = u'\0';
    do {
        if (!in.read(reinterpret_cast<char*>(&character), sizeof(char16_t))) {
            ret.clear();
            return ret;
        }
        ret += character;
    } while (character != u'\0');

    return ret;
}


size_t padToLen(std::ostream& out, const unsigned int& len, const char pad) {
    if (len == 0) return 0; //don't pad to no alignment (also cant % by 0)

    size_t padLen = len - (static_cast<size_t>(out.tellp()) % len);
    if (padLen == len) return 0; //doesnt write any padding, return length 0

    for (size_t i = 0; i < padLen; i++) {
        out.write(&pad, 1);
    }

    return padLen; //return number of bytes written
}
