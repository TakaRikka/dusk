#include "../utility/string.hpp"

#include <locale>
#include <string>
#include <charconv>

namespace randomizer::utility::str {
    //can't use codecvt on Wii U, deprecated in c++17 and g++ hates it
    //Borrowed from https://docs.microsoft.com/en-us/cpp/standard-library/codecvt-class?view=msvc-170#out
    std::string toUTF8(const std::u16string& str) {
        if(str.empty()) return "";

        std::string ret;
        ret.resize(str.size());
        char* pszNext;
        const char16_t* pwszNext;
        std::mbstate_t state = {0}; // zero-initialization represents the initial conversion state for mbstate_t
        std::locale loc("C");
        int res = std::use_facet<std::codecvt<char16_t, char, mbstate_t>>(loc).out(state, str.c_str(), &str[str.size()], pwszNext,
        &ret[0], &ret[ret.size()], pszNext);

        if(res == std::codecvt_base::error) return "";
        return ret;
    }

    std::u16string toUTF16(const std::string& str)
    {
        if(str.empty()) return u"";

        std::u16string ret;
        ret.resize(str.size());
        const char* pszNext;
        char16_t* pwszNext;
        std::mbstate_t state = {0}; // zero-initialization represents the initial conversion state for mbstate_t
        std::locale loc("C");
        int res = std::use_facet<std::codecvt<char16_t, char, mbstate_t>>(loc).in(state, str.c_str(), &str[str.size()], pszNext,
        &ret[0], &ret[ret.size()], pwszNext);

        if(res == std::codecvt_base::error) return u"";

        // Remove extra null terminators that may have been created from multi-byte
        // UTF-8 characters
        while(ret.size() > 0 && ret[ret.size() - 1] == u'\0')
        {
            ret.pop_back();
        }

        return ret;
    }

    // Takes in a string and returns an optional integer if the string
    // could be turned into one
    std::optional<int> toInt(std::string_view str) {
        // Trim leading/trailing whitespace
        auto start = str.find_first_not_of(" \t\r\n");
        if (start == std::string_view::npos) {
            return std::nullopt;
        }
        str.remove_prefix(start);

        auto end = str.find_last_not_of(" \t\r\n");
        if (end != std::string_view::npos) {
            str = str.substr(0, end + 1);
        }

        if (str.empty()) {
            return std::nullopt;
        }

        // Identify base (only decimal/hexadecimal handled)
        int base = 10;
        if (str.size() > 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
            base = 16;
            str.remove_prefix(2);
        }

        if (str.empty()) {
            return std::nullopt;
        }

        // Parse the remaining absolute string
        int value = 0;
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value, base);

        // Ensure conversion succeeded and consumed the entire trimmed string
        if (ec == std::errc{} && ptr == str.data() + str.size()) {
            return value;
        }

        return std::nullopt;
    }
}
