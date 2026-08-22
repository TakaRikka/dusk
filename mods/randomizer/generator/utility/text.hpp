#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include <limits>
#include <vector>
#include <cstdint>

namespace randomizer {
    class Text {
    public:
        enum Language {
            // First 5 match ordering of dSv_config_language in d_save.h
            ENGLISH,
            GERMAN,
            FRENCH,
            SPANISH,
            ITALIAN,
            // End of ordering for dSv_config_language
            JAPANESE, // Not supported yet
            LANGUAGE_MAX
        };

        enum Type
        {
            STANDARD = 0,
            PRETTY,
            CRYPTIC,
            TYPE_MAX
        };

        enum Color
        {
            RAW = 0,
            WHITE,
            RED,
            GREEN,
            LIGHT_BLUE,
            YELLOW,
            PURPLE,
            ORANGE,
            DARK_GREEN,
            BLUE,
            SILVER,
        };

        enum Gender
        {
            NEUTRAL = 0,
            MASCULINE,
            FEMININE,
            GENDER_MAX,
        };

        enum Plurality
        {
            SINGULAR = 0,
            PLURAL,
            PLURALITY_MAX,
        };

        static constexpr float MAX_LINE_WIDTH_ITEM_TEXTBOX = 14.0f;
        static constexpr float MAX_LINE_WIDTH_NORMAL_TEXTBOX = 17.0f;
        static constexpr size_t MAX_NEWLINES_PER_MESSAGE = 40;
        static constexpr size_t LINES_PER_BOX = 4;

        Text() = default;
        explicit Text(const std::string& str);

        std::array<std::string, LANGUAGE_MAX> mText{};
        std::array<Gender, LANGUAGE_MAX> mGender{};
        std::array<Plurality, LANGUAGE_MAX> mPlurality{};

        /**
         *
         * @param oldText the string to replace
         * @param replacementText the Text object to replace the old string
         * @param count the number of occurrences to replace
         */
        void Replace(const std::string& oldText, const Text& replacementText, uint32_t count = std::numeric_limits<uint32_t>::max());
        void Replace(const std::string& oldText, const std::string& replacementText, uint32_t count = std::numeric_limits<uint32_t>::max());
        void BreakLines(float maxLineWidth = MAX_LINE_WIDTH_NORMAL_TEXTBOX);

        // Inserts newlines to pad the text to the next box
        void PadToNextBox();
        void Capitalize();
        bool Empty() const;
        bool IsTooLong() const;
        std::vector<Text> SplitToFitTextLimits();
        bool operator==(const Text& t) const = default;
        Text& operator+=(const Text& rhs);
        Text& operator+=(const std::string& rhs);
        friend Text operator+(Text lhs, Text& rhs);
        friend Text operator+(Text lhs, const std::string& rhs);
        friend Text operator+(const std::string& lhs, const Text& rhs);
    };

    inline constexpr std::array supportedLanguages = {
        Text::ENGLISH,
        Text::SPANISH,
        Text::FRENCH,
        Text::GERMAN,
        Text::ITALIAN
    };

    // std::u16string apply_name_color(std::u16string str, const Color& color);
    // std::u16string word_wrap_string(const std::u16string& string, const size_t& max_line_len); //IMPROVEMENT: use font data to do this "properly"
    // std::string    pad_str_4_lines(const std::string& string);
    // std::u16string pad_str_4_lines(const std::u16string& string);

    Text::Language stringToLanguage(const std::string& str);
    std::string languageToString(Text::Language language);
    Text::Gender stringToGender(const std::string& str);
    Text::Plurality stringToPlurality(const std::string& str);

    // Retrieval of Text objects keyed by name and type (standard, pretty, cryptic)
    using TextDatabase = std::unordered_map<std::string, std::array<Text, Text::TYPE_MAX>>;

    const TextDatabase& getTextDatabase();

    bool textObjectExists(const std::string& name);
    const Text& getTextObject(const std::string& name, Text::Type type = Text::STANDARD);
    const std::string& getTextStr(const std::string& name, Text::Type type = Text::STANDARD, Text::Language language = Text::ENGLISH);

    Text addColor(const Text& t, Text::Color color, int count = 1);

    // Adds newlines in appropriate places to properly break the text string for textboxes
    void breakLines(std::string& str, float maxStrLength);

    // Replaces the message codes in the string with the ingame hex equivalents
    void applyMessageCodes(std::string&);

    Text makeTextListing(std::vector<Text> texts);
}; // namespace Text