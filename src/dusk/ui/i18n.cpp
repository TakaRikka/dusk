#include "i18n.hpp"

#include <absl/container/flat_hash_map.h>
#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <charconv>
#include <cctype>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>

#include "aurora/lib/logging.hpp"
#include "aurora/lib/rmlui/FileInterface_SDL.h"
#include "dusk/io.hpp"
#include "ui.hpp"

namespace dusk::ui::i18n {
namespace {

aurora::Module I18nLog{"dusk::ui::i18n"};

absl::flat_hash_map<Rml::String, Rml::String> sDictionary;
std::string sLanguage = "en";
bool sInitialized = false;

struct RmlFile {
    aurora::rmlui::FileInterface_SDL& fileInterface;
    Rml::FileHandle handle = {};

    ~RmlFile() {
        if (handle) {
            fileInterface.Close(handle);
        }
    }
};

std::string to_lower_ascii(std::string_view value) {
    std::string lowered;
    lowered.reserve(value.size());
    for (const char ch : value) {
        lowered.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
    }
    return lowered;
}

Rml::String trim_ascii(std::string_view value) {
    std::size_t begin = 0;
    std::size_t end = value.size();
    while (begin < end && std::isspace(static_cast<unsigned char>(value[begin]))) {
        ++begin;
    }
    while (end > begin && std::isspace(static_cast<unsigned char>(value[end - 1]))) {
        --end;
    }
    return Rml::String(value.substr(begin, end - begin));
}

Rml::String decode_xml_entities(std::string_view value) {
    Rml::String decoded;
    decoded.reserve(value.size());
    for (std::size_t i = 0; i < value.size(); ++i) {
        if (value[i] != '&') {
            decoded.push_back(value[i]);
            continue;
        }

        if (value.compare(i, 5, "&amp;") == 0) {
            decoded.push_back('&');
            i += 4;
            continue;
        }
        if (value.compare(i, 4, "&lt;") == 0) {
            decoded.push_back('<');
            i += 3;
            continue;
        }
        if (value.compare(i, 4, "&gt;") == 0) {
            decoded.push_back('>');
            i += 3;
            continue;
        }
        if (value.compare(i, 6, "&quot;") == 0) {
            decoded.push_back('"');
            i += 5;
            continue;
        }
        if (value.compare(i, 6, "&apos;") == 0) {
            decoded.push_back('\'');
            i += 5;
            continue;
        }

        if (i + 3 < value.size() && value[i + 1] == '#') {
            const bool isHex = (value[i + 2] == 'x' || value[i + 2] == 'X');
            const std::size_t digitsBegin = i + (isHex ? 3 : 2);
            const std::size_t semicolon = value.find(';', digitsBegin);
            if (semicolon != std::string_view::npos) {
                const auto digits = value.substr(digitsBegin, semicolon - digitsBegin);
                unsigned int codepoint = 0;
                auto [ptr, ec] = std::from_chars(
                    digits.data(), digits.data() + digits.size(), codepoint, isHex ? 16 : 10);
                if (ec == std::errc() && ptr == digits.data() + digits.size()) {
                    if (codepoint <= 0x7F) {
                        decoded.push_back(static_cast<char>(codepoint));
                    } else if (codepoint <= 0x7FF) {
                        decoded.push_back(static_cast<char>(0xC0 | (codepoint >> 6)));
                        decoded.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
                    } else if (codepoint <= 0xFFFF) {
                        decoded.push_back(static_cast<char>(0xE0 | (codepoint >> 12)));
                        decoded.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
                        decoded.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
                    } else if (codepoint <= 0x10FFFF) {
                        decoded.push_back(static_cast<char>(0xF0 | (codepoint >> 18)));
                        decoded.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
                        decoded.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
                        decoded.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
                    }
                    i = semicolon;
                    continue;
                }
            }
        }

        decoded.push_back('&');
    }
    return decoded;
}

std::string read_resource_text(const std::filesystem::path& filePath) {
    aurora::rmlui::FileInterface_SDL fileInterface;
    const Rml::String fileName = io::fs_path_to_string(filePath);
    RmlFile file{fileInterface, fileInterface.Open(fileName)};
    if (!file.handle) {
        throw std::runtime_error(fmt::format("Unable to open resource '{}'", fileName));
    }

    std::string text;
    const std::size_t length = fileInterface.Length(file.handle);
    if (length > 0) {
        text.resize(length);
        std::size_t total = 0;
        while (total < text.size()) {
            const std::size_t read = fileInterface.Read(text.data() + total, text.size() - total, file.handle);
            if (read == 0) {
                break;
            }
            total += read;
        }
        text.resize(total);
        return text;
    }

    std::array<char, 4096> buffer{};
    while (true) {
        const std::size_t read = fileInterface.Read(buffer.data(), buffer.size(), file.handle);
        if (read == 0) {
            break;
        }
        text.append(buffer.data(), read);
    }
    return text;
}

bool parse_language_xml(std::string_view xml) {
    sDictionary.clear();
    std::size_t cursor = 0;
    while (true) {
        const std::size_t entryOpen = xml.find("<entry", cursor);
        if (entryOpen == std::string_view::npos) {
            break;
        }

        const std::size_t keyAttr = xml.find("key=\"", entryOpen);
        if (keyAttr == std::string_view::npos) {
            cursor = entryOpen + 6;
            continue;
        }
        const std::size_t keyBegin = keyAttr + 5;
        const std::size_t keyEnd = xml.find('"', keyBegin);
        if (keyEnd == std::string_view::npos) {
            cursor = entryOpen + 6;
            continue;
        }

        const std::size_t tagEnd = xml.find('>', keyEnd);
        if (tagEnd == std::string_view::npos) {
            cursor = entryOpen + 6;
            continue;
        }
        const std::size_t entryClose = xml.find("</entry>", tagEnd + 1);
        if (entryClose == std::string_view::npos) {
            cursor = entryOpen + 6;
            continue;
        }

        const auto key = trim_ascii(xml.substr(keyBegin, keyEnd - keyBegin));
        const auto rawValue = xml.substr(tagEnd + 1, entryClose - (tagEnd + 1));
        sDictionary[key] = decode_xml_entities(rawValue);
        cursor = entryClose + 8;
    }
    return !sDictionary.empty();
}

bool load_dictionary(std::string_view language) {
    const std::filesystem::path filePath = resource_path(
        std::filesystem::path("i18n") / fmt::format("{}.xml", language));
    try {
        const std::string xml = read_resource_text(filePath);
        if (!parse_language_xml(xml)) {
            I18nLog.error("No valid i18n entries found in '{}'", io::fs_path_to_string(filePath));
            return false;
        }
        return true;
    } catch (const std::exception& e) {
        I18nLog.error("Failed to load i18n dictionary '{}': {}", io::fs_path_to_string(filePath), e.what());
        return false;
    }
}

bool translate_token(Rml::String& translated, std::string_view token) {
    const auto it = sDictionary.find(Rml::String(token));
    if (it == sDictionary.end()) {
        return false;
    }
    translated += it->second;
    return true;
}

}  // namespace

bool initialize() noexcept {
    sInitialized = true;
    return set_language("en");
}

void shutdown() noexcept {
    sDictionary.clear();
    sLanguage = "en";
    sInitialized = false;
}

bool set_language(std::string_view language) noexcept {
    std::string normalized = to_lower_ascii(language);
    if (normalized.empty()) {
        normalized = "en";
    }
    if (normalized != "en" && normalized != "zh-cn" && normalized != "zh-hans" &&
        normalized != "fr" && normalized != "ja")
    {
        normalized = "en";
    }

    if (normalized == sLanguage && !sDictionary.empty()) {
        return true;
    }

    if (load_dictionary(normalized)) {
        sLanguage = std::move(normalized);
        return true;
    }

    if (normalized != "en" && load_dictionary("en")) {
        // Keep requested UI language so style decisions (e.g. zh-cn default font)
        // still apply even when text falls back to en resources.
        sLanguage = std::move(normalized);
        return true;
    }

    return false;
}

const std::string& language() noexcept {
    return sLanguage;
}

bool is_simplified_chinese() noexcept {
    return sLanguage == "zh-cn" || sLanguage == "zh-hans";
}

bool use_harmonyos_font() noexcept {
    return is_simplified_chinese() || sLanguage == "ja";
}

int translate(Rml::String& translated, const Rml::String& input) {
    if (!sInitialized || sDictionary.empty()) {
        translated = input;
        return 0;
    }

    translated.clear();
    translated.reserve(input.size());

    int replacements = 0;
    std::size_t cursor = 0;
    while (cursor < input.size()) {
        const std::size_t open = input.find('[', cursor);
        if (open == Rml::String::npos) {
            translated.append(input.substr(cursor));
            break;
        }

        translated.append(input.substr(cursor, open - cursor));
        const std::size_t close = input.find(']', open + 1);
        if (close == Rml::String::npos) {
            translated.append(input.substr(open));
            break;
        }

        const std::string_view token(input.data() + open + 1, close - open - 1);
        if (translate_token(translated, token)) {
            ++replacements;
        } else {
            translated.append(input.substr(open, close - open + 1));
        }
        cursor = close + 1;
    }
    return replacements;
}

}  // namespace dusk::ui::i18n
