#include "text.hpp"

#include "string.hpp"
#include "yaml.hpp"
#include "shiftjis_table.hpp"

#include <mods/svc/flow.hpp>

#include <fmt/format.h>

#include <ranges>
#include <unordered_map>

#ifndef RANDOMIZER_ONLY
#include "JSystem/JUtility/JUTFont.h"
#include "m_Do/m_Do_ext.h"
#endif

namespace randomizer {
namespace {

std::string text_color_code(uint32_t rgba) {
    const auto message =
        mods::flow::MessageBuilder{}.text_color(rgba).build(MESSAGE_LANGUAGE_ENGLISH);
    const auto& bytes = message.text();
    return {reinterpret_cast<const char*>(bytes.data()), bytes.size() - 1};
}

const auto kDarkGreenMessageCode = text_color_code(0x4BBE4BFF);
const auto kBlueMessageCode = text_color_code(0x4B96D7FF);
const auto kSilverMessageCode = text_color_code(0xBFBFBFFF);

}  // namespace

    Text::Text(const std::string& str) {
        for (auto& text : mText) {
            text = str;
        }
    }

    void Text::Replace(const std::string& oldText, const Text& replacementText, uint32_t count/* = max*/) {
        for (size_t i = 0; i < mText.size(); ++i) {
            auto& curString = mText[i];
            curString = utility::str::Replace(curString, oldText, replacementText.mText[i], count);
        }
    }

    void Text::Replace(const std::string& oldText, const std::string& replacementText, uint32_t count/* = max*/) {
        for (auto& text : mText) {
            text = utility::str::Replace(text, oldText, replacementText, count);
        }
    }

    void Text::Capitalize() {
        try {
            // Determine the platform-specific locale string
#if defined(_WIN32) || defined(_WIN64)
            const char* localeName = "English_United States.1252";
#else
            const char* localeName = "en_US.iso88591";
#endif

            static const std::locale latin1Locale(localeName);

            for (size_t lang = 0; lang < mText.size(); ++lang) {
                auto& text = mText[lang];
                if (!text.empty() && lang != JAPANESE) {
                    text[0] = std::toupper(text[0], latin1Locale);
                }
            }
        } catch (const std::runtime_error&) {
            // Fallback incase the system completely lacks the requested locale definition
            for (size_t lang = 0; lang < mText.size(); ++lang) {
                auto& text = mText[lang];
                if (!text.empty() && lang != JAPANESE) {
                    text[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(text[0])));
                }
            }
        }
    }

    void Text::BreakLines(float maxLineWidth /*= MAX_LINE_WIDTH_ITEM_TEXTBOX*/) {
        for (size_t lang = 0; lang < mText.size(); ++lang) {
            auto& text = mText[lang];
            breakLines(text, maxLineWidth, lang);
        }
    }

    void Text::PadToNextBox() {
        BreakLines();
        for (size_t lang = 0; lang < mText.size(); ++lang) {
            auto& text = mText[lang];
            auto linesPerBox = LINES_PER_BOX_LATIN;
            if (lang == JAPANESE) {
                linesPerBox = LINES_PER_BOX_JP;
            }
            size_t numNewLines = std::ranges::count_if(text, [](char c){return c == '\n';});
            while (numNewLines == 0 || text.back() != '\n' || numNewLines % linesPerBox != 0) {
                text += '\n';
                ++numNewLines;
            }
        }
    }

    bool Text::Empty() const {
        for (auto& text : mText) {
            if (!text.empty()) {
                return false;
            }
        }
        return true;
    }

    bool Text::IsTooLong() const {
        for (auto& text : mText) {
            auto numNewLines = std::ranges::count_if(text, [](char c){return c == '\n';});
            if (numNewLines > MAX_NEWLINES_PER_MESSAGE) {
                return true;
            }
        }

        return false;
    }

    // Will split this text object into multiple objects short enough to fit into individual
    // text ids
    std::vector<Text> Text::SplitToFitTextLimits() {
        if (this->IsTooLong()) {
            // Figure out how many new text objects we need to fit all the text
            size_t numTextObjects{1};
            for (auto& text : mText) {
                double numNewLines = std::ranges::count_if(text, [](char c){return c == '\n';});
                auto curTextSplitAmount = static_cast<size_t>(std::ceil(numNewLines / MAX_NEWLINES_PER_MESSAGE));
                numTextObjects = std::max(curTextSplitAmount, numTextObjects);
            }

            std::vector<Text> splitText{numTextObjects};
            // Split each string into the appropriate number of objects
            for (size_t textIdx = 0; textIdx < mText.size(); ++textIdx) {
                auto& textStr = mText[textIdx];
                auto linesPerBox = LINES_PER_BOX_LATIN;
                if (textIdx == JAPANESE) {
                    linesPerBox = LINES_PER_BOX_JP;
                }
                // Calculate how many newlines we're allowing in this string per message
                // Different languages may have different amounts of newlines
                double numNewLines = std::ranges::count_if(textStr, [](char c){return c == '\n';});
                auto newLinesPerMessage  = static_cast<size_t>(std::ceil(numNewLines / numTextObjects));
                // Keep the number of lines as a multiple of how many lines are in a box so we don't split in the middle of a textbox
                while (newLinesPerMessage % linesPerBox != 0) {
                    ++newLinesPerMessage;
                }

                size_t splitIdx = 0;
                do {
                    size_t pos = 0;
                    for (int i = 0; i < newLinesPerMessage; ++i) {
                        pos = textStr.find('\n', pos);
                        if (pos == std::string::npos) {
                            break;
                        }
                        ++pos;
                    }

                    // Get the current split of the string
                    auto curSplit = textStr.substr(0, pos);
                    // Pop off the last newline since it's unnecessary
                    if (curSplit.back() == '\n') {
                        curSplit.pop_back();
                    }
                    splitText.at(splitIdx).mText[textIdx] = curSplit;
                    ++splitIdx;
                    if (pos == std::string::npos) {
                        textStr.clear();
                    } else {
                        textStr = textStr.substr(pos);
                    }
                } while (!textStr.empty());
            }

            // Recopy the front element to this object
            *this = splitText[0];

            // Reassign the vector everything except the first element
            splitText.assign(splitText.begin() + 1, splitText.end());
            return splitText;
        }

        return {};
    }

    Text& Text::operator+=(const Text& rhs) {
        for (size_t i = 0; i < mText.size(); ++i) {
            mText[i] += rhs.mText[i];
        }
        return *this;
    }

    Text& Text::operator+=(const std::string& rhs) {
        for (auto& text : mText) {
            text += rhs;
        }
        return *this;
    }

    Text operator+(Text lhs, const Text& rhs) {
        lhs += rhs;
        return lhs;
    }

    Text operator+(Text lhs, const std::string& rhs) {
        for (auto& text : lhs.mText) {
            text += rhs;
        }
        return lhs;
    }

    Text operator+(const std::string& lhs, const Text& rhs) {
        return Text(lhs) + rhs;
    }

    Text::Type stringToType(const std::string& str) {
        std::unordered_map<std::string, Text::Type> strToType = {
            {"Standard", Text::Type::STANDARD},
            {"Pretty", Text::Type::PRETTY},
            {"Cryptic", Text::Type::CRYPTIC},
        };

        if (strToType.contains(str))
        {
            return strToType.at(str);
        }

        throw std::runtime_error("Text type \"" + str + "\" is not recognized.");
    }

    Text::Language stringToLanguage(const std::string& str) {
        std::unordered_map<std::string, Text::Language> strToLanguage = {
            {"english",  Text::ENGLISH},
            {"spanish",  Text::SPANISH},
            {"french",   Text::FRENCH},
            {"german",   Text::GERMAN},
            {"italian",  Text::ITALIAN},
            {"dutch",    Text::DUTCH},
            {"japanese", Text::JAPANESE}
        };

        if (strToLanguage.contains(str))
        {
            return strToLanguage.at(str);
        }

        throw std::runtime_error("Language \"" + str + "\" is not recognized.");
    }


    std::string languageToString(Text::Language language) {
        switch (language) {
            case Text::ENGLISH:
                return "english";
            case Text::SPANISH:
                return "spanish";
            case Text::FRENCH:
                return "french";
            case Text::GERMAN:
                return "german";
            case Text::ITALIAN:
                return "italian";
            case Text::DUTCH:
                return "dutch";
            case Text::JAPANESE:
                return "japanese";
            default:
                return "unknown language enum";
        }
    }

    Text::Gender stringToGender(const std::string& str)
    {
        std::unordered_map<std::string, Text::Gender> strToGender = {
            {"Masculine", Text::Gender::MASCULINE},
            {"Feminine", Text::Gender::FEMININE}
        };

        if (strToGender.contains(str))
        {
            return strToGender.at(str);
        }

        return Text::Gender::NEUTRAL;
    }

    Text::Plurality stringToPlurality(const std::string& str)
    {
        if (str == "Plural") return Text::Plurality::PLURAL;
        return Text::Plurality::SINGULAR;
    }

    std::string UTF8ToLatin1(const std::string& utf8Str) {
        std::string latin1Str;
        // The output string will be equal to or shorter than the UTF-8 string
        latin1Str.reserve(utf8Str.length());

        size_t read_pos = 0;
        size_t len = utf8Str.length();

        while (read_pos < len) {
            unsigned char c = utf8Str[read_pos];

            if (c < 0x80) {
                // Standard ASCII (0x00 - 0x7F)
                latin1Str.push_back(c);
                ++read_pos;
            }
            else if ((c & 0xE0) == 0xC0 && (read_pos + 1 < len)) {
                // Two-byte UTF-8 sequence (0xC0 - 0xDF)
                unsigned char next_byte = utf8Str[read_pos + 1];

                // Reconstruct the Latin-1 character value
                unsigned char latin1_char = ((c & 0x1F) << 6) | (next_byte & 0x3F);

                latin1Str.push_back(latin1_char);
                read_pos += 2;
            }
            else {
                // Multi-byte sequences out of Latin-1 range (or malformed bytes)
                throw std::runtime_error(fmt::format("Invalid bytes when converting to Latin1 with \"{}\"", utf8Str));
            }
        }

        return latin1Str;
    }

std::string UTF8ToShiftJIS(const std::string& utf8Str) {
    std::string sjisStr;
    sjisStr.reserve(utf8Str.length());

    size_t read_pos = 0;
    size_t len = utf8Str.length();

    while (read_pos < len) {
        uint32_t codepoint = 0;
        unsigned char c = utf8Str[read_pos];

        // Decode UTF-8 sequence to Unicode codepoint
        if (c < 0x80) {
            codepoint = c;
            read_pos += 1;
        } else if ((c & 0xE0) == 0xC0 && (read_pos + 1 < len)) {
            codepoint = ((c & 0x1F) << 6) | (utf8Str[read_pos + 1] & 0x3F);
            read_pos += 2;
        } else if ((c & 0xF0) == 0xE0 && (read_pos + 2 < len)) {
            codepoint = ((c & 0x0F) << 12) |
                        ((utf8Str[read_pos + 1] & 0x3F) << 6) |
                        (utf8Str[read_pos + 2] & 0x3F);
            read_pos += 3;
        } else if ((c & 0xF8) == 0xF0 && (read_pos + 3 < len)) {
            codepoint = ((c & 0x07) << 18) |
                        ((utf8Str[read_pos + 1] & 0x3F) << 12) |
                        ((utf8Str[read_pos + 2] & 0x3F) << 6) |
                        (utf8Str[read_pos + 3] & 0x3F);
            read_pos += 4;
        } else {
            throw std::runtime_error(fmt::format("Invalid UTF-8 byte sequence in \"{}\"", utf8Str));
        }

        // Convert Codepoint to Shift-JIS
        if (codepoint < 0x80) {
            // Standard ASCII maps directly (Note: 0x5C is '\' in ASCII, '¥' in JIS X 0201)
            sjisStr.push_back(static_cast<char>(codepoint));
        } else if (codepoint >= 0xFF61 && codepoint <= 0xFF9F) {
            // Half-width Katakana (U+FF61 - U+FF9F -> 0xA1 - 0xDF)
            sjisStr.push_back(static_cast<char>(codepoint - 0xFF61 + 0xA1));
        } else {
            // Full-width Kana / Kanji lookup
            auto it = unicodeToShiftJISMap.find(codepoint);
            if (it == unicodeToShiftJISMap.end()) {
                throw std::runtime_error(fmt::format("Codepoint U+{:04X} cannot be represented in Shift-JIS", codepoint));
            }

            uint16_t sjisVal = it->second;
            sjisStr.push_back(static_cast<char>((sjisVal >> 8) & 0xFF)); // Lead byte
            sjisStr.push_back(static_cast<char>(sjisVal & 0xFF));        // Trail byte
        }
    }

    return sjisStr;
}

    static void LoadTextData(TextDatabase& tb) {
        struct LanguageEntry {
            std::string language;
            std::string languageData;
        };
        auto files = std::to_array<LanguageEntry>({
            {"english",  GET_EMBED_DATA(RANDO_DATA_PATH "text/languages/english.yaml")},
            {"spanish",  GET_EMBED_DATA(RANDO_DATA_PATH "text/languages/spanish.yaml")},
            {"french",   GET_EMBED_DATA(RANDO_DATA_PATH "text/languages/french.yaml")},
            {"german",   GET_EMBED_DATA(RANDO_DATA_PATH "text/languages/german.yaml")},
            {"italian",  GET_EMBED_DATA(RANDO_DATA_PATH "text/languages/italian.yaml")},
            {"japanese", GET_EMBED_DATA(RANDO_DATA_PATH "text/languages/japanese.yaml")},
        });

        for (const auto& file : files) {
            auto language = stringToLanguage(file.language);
            auto textData = LOAD_EMBED_DATA(file.languageData);
            for (const auto& textNode : textData) {
                const auto& name = textNode.first.as<std::string>();
                for (const auto& typeNode : textNode.second) {
                    auto type = stringToType(typeNode.first.as<std::string>());
                    auto typeData = typeNode.second;
                    const auto& text = typeData["Text"].as<std::string>();
                    if (language != Text::JAPANESE) {
                        tb[name][type].mText[language] = UTF8ToLatin1(text);
                    } else {
                        tb[name][type].mText[language] = UTF8ToShiftJIS(text);
                    }
                    if (typeData["Gender"]) {
                        tb[name][type].mGender[language] = stringToGender(typeData["Gender"].as<std::string>());
                    }
                    if (typeData["Plurality"]) {
                        tb[name][type].mPlurality[language] = stringToPlurality(typeData["Plurality"].as<std::string>());
                    }
                }
            }
        }
    }

    const TextDatabase& getTextDatabase() {
        static TextDatabase tb{};

        // If database is empty, load it up
        if (tb.empty()) {
            LoadTextData(tb);
        }

        return tb;
    }

    bool textObjectExists(const std::string& name) {
        return getTextDatabase().contains(name);
    }

    const Text& getTextObject(const std::string& name, Text::Type type /*= Text::STANDARD*/)
    {
        const auto& tb = getTextDatabase();
        if (!tb.contains(name)) {
            throw std::runtime_error("Text name \"" + name + "\" is not recognized.");
        }
        return tb.at(name).at(type);
    }

    const std::string& getTextStr(const std::string& name,
                        Text::Type type /*= Text::STANDARD*/,
                        Text::Language language /*= Text::ENGLISH*/)
    {
        const auto& tb = getTextDatabase();
        if (!tb.contains(name)) {
            throw std::runtime_error("Text name \"" + name + "\" is not recognized.");
        }

        if (!tb.at(name).at(type).mText.at(language).empty()) {
            return tb.at(name).at(type).mText.at(language);
        }

        // Return english if the other language's string is empty
        return tb.at(name).at(type).mText.at(language);
    }

    Text addColor(const Text& t, Text::Color color, int count /* = 1*/) {
        const static std::unordered_map<Text::Color, std::string> colorStrings = {
            {Text::WHITE, "<white>"},
            {Text::RED, "<red>"},
            {Text::GREEN, "<green>"},
            {Text::LIGHT_BLUE, "<light blue>"},
            {Text::YELLOW, "<yellow>"},
            {Text::PURPLE, "<purple>"},
            {Text::ORANGE, "<orange>"},
            {Text::DARK_GREEN, "<dark green>"},
            {Text::BLUE, "<blue>"},
            {Text::SILVER, "<silver>"},
        };

        if (color == Text::Color::RAW) {
            return t;
        }

        if (!colorStrings.contains(color)) {
            throw std::runtime_error("Color enum value \"" + std::to_string(color) + "\" is not recognized.");
        }

        Text text = t;
        for (auto& langText : text.mText) {
            // If we don't have brackets indicating color, then surround the entire text
            if (langText.find('{') == std::string::npos && langText.find('}') == std::string::npos) {
                langText = colorStrings.at(color) + langText + colorStrings.at(Text::WHITE);
            } else {
                langText = utility::str::Replace(langText, "{", colorStrings.at(color), count);
                langText = utility::str::Replace(langText, "}", colorStrings.at(Text::WHITE), count);
            }
        }

        return text;
    }

    using namespace std::string_view_literals;
    static const std::unordered_map<std::string_view, std::string_view> messageCodes = {
        {"<player name>",    "\x1A\x05\x00\x00\x00"sv},
        {"<fast>",           "\x1A\x05\x00\x00\x01"sv},
        {"<slow>",           "\x1A\x05\x00\x00\x02"sv},
        {"<begin choice>",   "\x1A\x05\x00\x00\x20"sv},
        {"<char kyo>",       "\x1A\x05\x04\x00\x0A"sv}, // Special JP character
        {"<male>",           "\x1A\x05\x06\x00\x02"sv},
        {"<female>",         "\x1A\x05\x06\x00\x03"sv},
        {"<2 way choice 1>", "\x1A\x06\x00\x00\x08\x01"sv},
        {"<2 way choice 2>", "\x1A\x06\x00\x00\x08\x02"sv},
        {"<3 way choice 1>", "\x1A\x06\x00\x00\x09\x01"sv},
        {"<3 way choice 2>", "\x1A\x06\x00\x00\x09\x02"sv},
        {"<3 way choice 3>", "\x1A\x06\x00\x00\x09\x03"sv},
        {"<white>",          "\x1A\x06\xFF\x00\x00\x00"sv},
        {"<red>",            "\x1A\x06\xFF\x00\x00\x01"sv},
        {"<green>",          "\x1A\x06\xFF\x00\x00\x02"sv},
        {"<light blue>",     "\x1A\x06\xFF\x00\x00\x03"sv},
        {"<yellow>",         "\x1A\x06\xFF\x00\x00\x04"sv},
        {"<purple>",         "\x1A\x06\xFF\x00\x00\x06"sv},
        {"<orange>",         "\x1A\x06\xFF\x00\x00\x08"sv},
        {"<dark green>",     kDarkGreenMessageCode},
        {"<blue>",           kBlueMessageCode},
        {"<silver>",         kSilverMessageCode},
    };

    void breakLines(std::string& str, float maxStrLength, int lang) {
        // Randomizer Only shouldn't rely on needing access to the iso
    #ifndef RANDOMIZER_ONLY
        // Get game's font
        auto gameFont = mDoExt_getMesgFont();
    #endif

        float curLineWidth = 0.f;
        size_t i = 0;
        size_t lastBreakOpportunity = 0;

        // Check if a byte is a Shift-JIS lead byte
        auto isSJISLeadByte = [&](uint8_t c) {
            return ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xFC)) && lang == Text::JAPANESE;
        };

        // Check if a Shift-JIS 2-byte character is a prohibited line-start character
        auto isProhibitedLineStart = [](uint16_t sjisChar) {
            switch (sjisChar) {
                case 0x8141: // 、 (Comma)
                case 0x8142: // 。 (Period)
                case 0x8143: // ,
                case 0x8144: // .
                case 0x8168: // 」 (Closing Quote)
                case 0x816A: // 』
                case 0x816C: // ） (Closing Paren)
                case 0x816E: // ］
                case 0x8170: // ｝
                case 0x815B: // ー (Katakana Prolonged Sound Mark)
                case 0x8145: // ・ (Middle Dot)
                case 0x8148: // ?
                case 0x8149: // !
                    return true;
                default:
                    return false;
            }
        };

        while (i < str.length()) {
            // Skip over control codes since they don't get displayed
            std::string code{};
            for (const auto& messageCode : messageCodes | std::views::keys) {
                if (str.substr(i, messageCode.length()) == messageCode) {
                    code = messageCode;
                    break;
                }
            }

            if (!code.empty()) {
                // Assume worst case for player name width.
                if (code == "<player name>") {
                    curLineWidth += 8.f;
                }
                i += code.length();
                continue;
            }

            uint8_t c1 = static_cast<uint8_t>(str[i]);

            // If we encounter an already inserted newline, reset the counter
            if (c1 == '\n') {
                curLineWidth = 0.f;
                ++i;
                lastBreakOpportunity = i;
                continue;
            }

            uint16_t sjisChar = c1;
            size_t charLen = 1;

            if (isSJISLeadByte(c1) && (i + 1 < str.length())) {
                uint8_t c2 = static_cast<uint8_t>(str[i + 1]);
                sjisChar = (static_cast<uint16_t>(c1) << 8) | c2;
                charLen = 2;
            }

            // Keep track of the previous space to replace with
            // a line break when we reach the maximum width. For Japanese,
            // we can insert a newline anywhere we don't have a prohibited
            // line start character
            if (c1 == ' ') {
                lastBreakOpportunity = i;
            } else if (charLen == 2 && !isProhibitedLineStart(sjisChar)) {
                // In Japanese, every character boundary is a potential line break (unless prohibited by punctuation)
                lastBreakOpportunity = i;
            }

    #ifndef RANDOMIZER_ONLY
            float width = static_cast<float>(gameFont->getWidth(sjisChar));
            curLineWidth += width / static_cast<float>(gameFont->getCellWidth());
    #else
            // Assume worst case with no iso access
            curLineWidth += (charLen == 2) ? 2.f : 1.f;
    #endif

            // If we exceed the maximum line width, replace the
            // previous space with a newline and start counting
            // from the newline again
            if (curLineWidth > maxStrLength && lastBreakOpportunity > 0) {
                if (str[lastBreakOpportunity] == ' ') {
                    str[lastBreakOpportunity] = '\n';
                    i = lastBreakOpportunity + 1;
                } else {
                    // Insert a newline directly between Japanese characters
                    str.insert(lastBreakOpportunity, "\n");
                    i = lastBreakOpportunity + 1;
                }
                curLineWidth = 0.f;
                lastBreakOpportunity = 0;
                continue;
            }

            i += charLen;
        }

    #ifndef RANDOMIZER_ONLY
        mDoExt_removeMesgFont();
    #endif
    }

    void applyMessageCodes(std::string& str) {
        for (const auto& [code, replacement] : messageCodes) {
            size_t pos = 0;
            while ((pos = str.find(code, pos)) != std::string::npos) {
                str.replace(pos, code.length(), replacement);
                pos += replacement.length();
            }
        }
    }

    Text makeTextListing(std::vector<Text> texts) {
        if (texts.empty()) {
            return Text{};
        }
        if (texts.size() == 1) {
            return texts.front();
        }

        // TODO: Other language listing rules
        std::string english{};
        std::string french{};
        std::string german{};
        std::string italian{};
        std::string spanish{};
        std::string japanese{};
        for (int i = 0; i < texts.size(); ++i) {
            auto& text = texts[i];

            // English rules. Move other languages out of english when we have their rules
            if (i == 0) {
                english += text.mText[Text::ENGLISH];
                french += text.mText[Text::FRENCH];
                german += text.mText[Text::GERMAN];
                italian += text.mText[Text::ITALIAN];
                spanish += text.mText[Text::SPANISH];
                japanese += text.mText[Text::JAPANESE];
            } else if (i == texts.size() - 1 && texts.size() == 2) {
                english += " and " + text.mText[Text::ENGLISH];
                french += " and " + text.mText[Text::FRENCH];
                german += " and " + text.mText[Text::GERMAN];
                italian += " and " + text.mText[Text::ITALIAN];
                spanish += " and " + text.mText[Text::SPANISH];
                japanese += " and " + text.mText[Text::JAPANESE];
            } else if (i == texts.size() - 1) {
                english += ", and " + text.mText[Text::ENGLISH];
                french += ", and " + text.mText[Text::FRENCH];
                german += ", and " + text.mText[Text::GERMAN];
                italian += ", and " + text.mText[Text::ITALIAN];
                spanish += ", and " + text.mText[Text::SPANISH];
                japanese += ", and " + text.mText[Text::JAPANESE];
            } else {
                english += ", " + text.mText[Text::ENGLISH];
                french += ", " + text.mText[Text::FRENCH];
                german += ", " + text.mText[Text::GERMAN];
                italian += ", " + text.mText[Text::ITALIAN];
                spanish += ", " + text.mText[Text::SPANISH];
                japanese += ", " + text.mText[Text::JAPANESE];
            }
        }

        Text listingText{};
        listingText.mText[Text::ENGLISH] = english;
        listingText.mText[Text::FRENCH] = french;
        listingText.mText[Text::GERMAN] = german;
        listingText.mText[Text::ITALIAN] = italian;
        listingText.mText[Text::SPANISH] = spanish;
        listingText.mText[Text::JAPANESE] = japanese;
        return listingText;
    }
}; // namespace Text
