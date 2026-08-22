#include "text.hpp"

#include "string.hpp"
#include "yaml.hpp"

#include <fmt/format.h>

#include <ranges>
#include <unordered_map>

#ifndef RANDOMIZER_ONLY
#include "JSystem/JUtility/JUTFont.h"
#include "m_Do/m_Do_ext.h"
#endif

namespace randomizer {

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

            for (auto& text : mText) {
                if (!text.empty()) {
                    text[0] = std::toupper(text[0], latin1Locale);
                }
            }
        } catch (const std::runtime_error&) {
            // Fallback incase the system completely lacks the requested locale definition
            for (auto& text : mText) {
                if (!text.empty()) {
                    text[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(text[0])));
                }
            }
        }
    }

    void Text::BreakLines(float maxLineWidth /*= MAX_LINE_WIDTH_ITEM_TEXTBOX*/) {
        for (auto& text : mText) {
            breakLines(text, maxLineWidth);
        }
    }

    void Text::PadToNextBox() {
        BreakLines();
        for (auto& text : mText) {
            size_t numNewLines = std::ranges::count_if(text, [](char c){return c == '\n';});
            while (numNewLines == 0 || text.back() != '\n' || numNewLines % LINES_PER_BOX != 0) {
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
                // Calculate how many newlines we're allowing in this string per message
                // Different languages may have different amounts of newlines
                double numNewLines = std::ranges::count_if(textStr, [](char c){return c == '\n';});
                auto newLinesPerMessage  = static_cast<size_t>(std::ceil(numNewLines / numTextObjects));
                // Keep the number of lines as a multiple of how many lines are in a box so we don't split in the middle of a textbox
                while (newLinesPerMessage % LINES_PER_BOX != 0) {
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

    static void LoadTextData(TextDatabase& tb) {
        struct LanguageEntry {
            std::string language;
            std::string languageData;
        };
        auto files = std::to_array<LanguageEntry>({
            {"english", GET_EMBED_DATA(RANDO_DATA_PATH "text/languages/english.yaml")},
            {"spanish", GET_EMBED_DATA(RANDO_DATA_PATH "text/languages/spanish.yaml")},
            {"french",  GET_EMBED_DATA(RANDO_DATA_PATH "text/languages/french.yaml")},
            {"german",  GET_EMBED_DATA(RANDO_DATA_PATH "text/languages/german.yaml")},
            {"italian", GET_EMBED_DATA(RANDO_DATA_PATH "text/languages/italian.yaml")},
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
                        // Probably have to handle Japanese another way at some point
                        tb[name][type].mText[language] = text;
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
        // custom colors
        {"<dark green>",     "\x1A\x06\xFF\x00\x00\x09"sv},
        {"<blue>",           "\x1A\x06\xFF\x00\x00\x0A"sv},
        {"<silver>",         "\x1A\x06\xFF\x00\x00\x0B"sv},
    };

    void breakLines(std::string& str, float maxStrLength) {

        // Randomizer Only shouldn't rely on needing access to the iso
#ifndef RANDOMIZER_ONLY
        // Get game's font
        auto gameFont = mDoExt_getMesgFont();
#endif
        float curLineWidth = 0.f;
        size_t i = 0;
        size_t previousSpace = 0;
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

            // Keep track of the previous space to replace with
            // a line break when we reach the maximum width
            if (str[i] == ' ') {
                previousSpace = i;
            }
            // If we encounter an already inserted newline, reset the counter
            else if (str[i] == '\n') {
                curLineWidth = 0.f;
                ++i;
                continue;
            }


#ifndef RANDOMIZER_ONLY
            auto width = gameFont->getWidth(str[i]);
            curLineWidth += width / static_cast<float>(gameFont->getCellWidth());
#else
            // Assume worst case with no iso access
            curLineWidth += 1;
#endif
            // If we exceed the maximum line width, replace the
            // previous space with a newline and start counting
            // from the newline again
            if (curLineWidth > maxStrLength) {
                str[previousSpace] = '\n';
                i = previousSpace;
                curLineWidth = 0.f;
            }

            ++i;
        }

#ifndef RANDOMIZER_ONLY
        // Free game's font
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
        for (int i = 0; i < texts.size(); ++i) {
            auto& text = texts[i];

            // English rules. Move other languages out of english when we have their rules
            if (i == 0) {
                english += text.mText[Text::ENGLISH];
                french += text.mText[Text::FRENCH];
                german += text.mText[Text::GERMAN];
                italian += text.mText[Text::ITALIAN];
                spanish += text.mText[Text::SPANISH];
            } else if (i == texts.size() - 1 && texts.size() == 2) {
                english += " and " + text.mText[Text::ENGLISH];
                french += " and " + text.mText[Text::FRENCH];
                german += " and " + text.mText[Text::GERMAN];
                italian += " and " + text.mText[Text::ITALIAN];
                spanish += " and " + text.mText[Text::SPANISH];
            } else if (i == texts.size() - 1) {
                english += ", and " + text.mText[Text::ENGLISH];
                french += ", and " + text.mText[Text::FRENCH];
                german += ", and " + text.mText[Text::GERMAN];
                italian += ", and " + text.mText[Text::ITALIAN];
                spanish += ", and " + text.mText[Text::SPANISH];
            } else {
                english += ", " + text.mText[Text::ENGLISH];
                french += ", " + text.mText[Text::FRENCH];
                german += ", " + text.mText[Text::GERMAN];
                italian += ", " + text.mText[Text::ITALIAN];
                spanish += ", " + text.mText[Text::SPANISH];
            }
        }

        Text listingText{};
        listingText.mText[Text::ENGLISH] = english;
        listingText.mText[Text::FRENCH] = french;
        listingText.mText[Text::GERMAN] = german;
        listingText.mText[Text::ITALIAN] = italian;
        listingText.mText[Text::SPANISH] = spanish;
        return listingText;
    }
}; // namespace Text
