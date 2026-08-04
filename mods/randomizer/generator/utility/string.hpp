#pragma once

#include <concepts>
#include <ios>
#include <string>
#include <type_traits>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <limits>
#include <optional>
#include <cstdint>

namespace randomizer::utility::str {
    std::string toUTF8(const std::u16string& str);

    std::u16string toUTF16(const std::string& str);

    template<typename T>
    concept StringType = std::derived_from<T, std::basic_string<typename T::value_type>>;

    template<typename T> requires StringType<T>
    std::vector<T> Split(const T& string, const typename T::value_type delim) {
        std::vector<T> ret;
        T tail = string;
        auto index = tail.find_first_of(delim);

        while (index != T::npos) {
            ret.push_back(tail.substr(0, index));
            tail = tail.substr(index + 1);
            index = tail.find_first_of(delim);
        }
        ret.push_back(tail); //add anything after last line break

        return ret;
    }

    template<typename T> requires StringType<T>
    T Merge(const std::vector<T>& lines, const typename T::value_type separator) {
        T ret;
        for (const T& segment : lines) {
            ret += segment + separator;
        }

        return ret;
    }

    template<typename T> requires StringType<T>
    T assureNullTermination(const T& string) {
        if(!string.empty() && string.back() == typename T::value_type(0)) return string;

        return string + typename T::value_type(0);
    }

    template<typename T> requires std::integral<T>
    std::string intToHex(const T& i, const bool& base = true)
    {
        std::stringstream stream;
        stream << std::hex << (base ? std::showbase : std::noshowbase) << i;
        return stream.str();
    }

    template<typename T> requires std::integral<T>
    std::string intToHex(const T& i, const std::streamsize& width, const bool& base = true)
    {
        std::stringstream stream;
        stream << std::hex << (base ? std::showbase : std::noshowbase) << std::setfill('0') << std::setw(width) << i;
        return stream.str();
    }

    /**
     * @brief Checks to see if any of the passed in substrings are within a string
     *
     * @param str The string to check for substrings
     * @param substrs Paramater Pack of strings to test against the first argument
     *
     * @return true if any of the passed in substrings are found within the string, false otherwise
     */
    template<class... Types>
    bool Contains(const std::string& str, Types... substrs)
    {
        for (const auto& substr : {substrs...})
        {
            if (str.find(substr) != std::string::npos)
            {
                return true;
            }
        }
        return false;
    }

    //wrapper for a constexpr string, for use in other templates
    template<size_t N>
    struct StringLiteral {
        constexpr StringLiteral(const char (&str)[N]) {
            std::copy_n(str, N, value);
        }

        constexpr operator std::string_view() const {
            return std::string_view(value, N - 1); //leave out null terminator
        }

        char value[N];
    };

    template<class... Types>
    void Erase(std::string& s, Types... tokens)
    {
        for (const auto& token : {tokens...})
        {
            while (randomizer::utility::str::Contains(s, token))
            {
                s.erase(s.find(token), strlen(token));
            }
        }
    }

    std::optional<int> toInt(std::string_view str);
    std::string Replace(const std::string& originalStr,
                        const std::string& oldStr,
                        const std::string& replacementStr,
                        uint32_t count = std::numeric_limits<uint32_t>::max());
}