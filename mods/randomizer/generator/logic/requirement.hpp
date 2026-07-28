#pragma once

#include <string>
#include <variant>
#include <vector>

// Forward declarations
namespace randomizer::logic::item
{
    class Item;
}

namespace randomizer::logic::entrance
{
    class Entrance;
}

namespace randomizer::logic::area
{
    class EventAccess;
    class LocationAccess;
} // namespace randomizer::logic::area

namespace randomizer::logic::world
{
    class World;
}

namespace randomizer::logic::search
{
    class Search;
}

namespace randomizer::logic::requirement
{
    enum class Type
    {
        INVALID,
        NOTHING,
        IMPOSSIBLE,
        OR,
        AND,
        ITEM,
        COUNT,
        EVENT,
        MACRO,
        DAY,
        NIGHT,
        HUMAN_LINK,
        WOLF_LINK,
        TWILIGHT,
        GOLDEN_BUGS,
        HEARTS,
        DUNGEONS_COMPLETED,
    };

    enum class EvalSuccess
    {
        NONE,
        PARTIAL,
        COMPLETE,
        DISCONNECTED,
    };

    // FormTime is a set of flags that cover all the possible cases of human-wolf/day-night combinations that are needed
    // for logic to work
    namespace FormTime
    {
        enum
        {
            NONE = 0b0000,
            HUMAN_DAY = 0b0001,
            HUMAN_NIGHT = 0b0010,
            WOLF_DAY = 0b0100,
            WOLF_NIGHT = 0b1000,
            HUMAN = HUMAN_DAY | HUMAN_NIGHT,
            WOLF = WOLF_DAY | WOLF_NIGHT,
            DAY = HUMAN_DAY | WOLF_DAY,
            NIGHT = HUMAN_NIGHT | WOLF_NIGHT,
            ALL = 0b1111,
            TWILIGHT = 0b10000,
        };

        extern const std::vector<int> ALL_FORM_TIMES;
        extern const std::vector<int> ALL_FORM_TIMES_AND_TWILIGHT;
        extern const std::vector<int> ALL_FORM_AND_DAY_TIMES;

        std::string to_string(const int& formTime);
    }; // namespace FormTime

    struct Requirement;
    struct Requirement
    {
        using Argument = std::variant<int, Requirement, item::Item*>;
        Type _type = Type::INVALID;
        std::vector<Argument> _args;

        std::string to_string() const;
    };

    Requirement ParseRequirementString(const std::string& reqStr,
                                       world::World* world,
                                       const bool& forceLogic = false);

    /**
     * @brief Evaluates a requirement assuming it meets a simplistic criteria. This is used
     * for checking settings when reading them in from, for example, startflags.yaml
     *
     * @param req - The simple requirement
     * @param world - The world this requirement is for
     * @return true if the requirment holds, false otherwise
     */
    bool EvaluateSimpleRequirement(const Requirement& req, world::World* world);

    bool EvaluateRequirementAtFormTime(const Requirement& req,
                                       search::Search* search,
                                       const int& formTime,
                                       world::World*);
    EvalSuccess EvaluateEventRequirement(search::Search* search, area::EventAccess* event);
    EvalSuccess EvaluateExitRequirement(search::Search* search, entrance::Entrance* exit);
    EvalSuccess EvaluateDisconnectedExitRequiremrnt(search::Search* search, entrance::Entrance* exit);
    EvalSuccess EvaluateLocationRequirement(search::Search* search,
                                            area::LocationAccess* locAccess);

    const extern Requirement NO_REQUIREMENT;
    const extern Requirement IMPOSSIBLE_REQUIREMENT;
} // namespace randomizer::logic::requirement
