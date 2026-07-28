#include "requirement.hpp"

#include "search.hpp"
#include "world.hpp"
#include "../utility/container.hpp"
#include "../utility/log.hpp"
#include "../utility/string.hpp"

#include <algorithm>
#include <ranges>

namespace randomizer::logic::requirement
{
    namespace FormTime
    {
        const std::vector<int> ALL_FORM_TIMES = {HUMAN_DAY, HUMAN_NIGHT, WOLF_DAY, WOLF_NIGHT};
        const std::vector<int> ALL_FORM_TIMES_AND_TWILIGHT = {HUMAN_DAY, HUMAN_NIGHT, WOLF_DAY, WOLF_NIGHT, TWILIGHT};
        const std::vector<int> ALL_FORM_AND_DAY_TIMES = {HUMAN_DAY, HUMAN_NIGHT, WOLF_DAY, WOLF_NIGHT, DAY, NIGHT};

        std::string to_string(const int& formTime)
        {
            std::string formTimeStr = "";
            if (formTime & HUMAN_DAY)
                formTimeStr += " Human_Day";
            if (formTime & HUMAN_NIGHT)
                formTimeStr += " Human_Night";
            if (formTime & WOLF_DAY)
                formTimeStr += " Wolf_Day";
            if (formTime & WOLF_NIGHT)
                formTimeStr += " Wolf_Night";
            if (formTime & TWILIGHT)
                formTimeStr += " Twilight";
            return formTimeStr;
        }
    } // namespace FormTime

    const extern Requirement NO_REQUIREMENT = Requirement{Type::NOTHING, {}};
    const extern Requirement IMPOSSIBLE_REQUIREMENT = Requirement{Type::IMPOSSIBLE, {}};

    std::string Requirement::to_string() const
    {
        std::string reqStr = "";
        item::Item* item;
        Requirement nestedReq;
        int count;
        int eventIndex;
        int macroIndex;
        switch (this->_type)
        {
            case Type::NOTHING:
                return "Nothing";

            case Type::IMPOSSIBLE:
                return "Impossible (Please discover an entrance first)";

            case Type::OR:
                for (const auto& arg : this->_args)
                {
                    nestedReq = std::get<Requirement>(arg);
                    if (nestedReq._type == Type::AND || nestedReq._type == Type::OR)
                    {
                        reqStr += "(";
                        reqStr += nestedReq.to_string();
                        reqStr += ")";
                    }
                    else
                    {
                        reqStr += nestedReq.to_string();
                    }
                    reqStr += " or ";
                }
                // pop off the last " or "
                for (auto i = 0; i < 4; i++)
                {
                    reqStr.pop_back();
                }
                return reqStr;

            case Type::AND:
                for (const auto& arg : this->_args)
                {
                    nestedReq = std::get<Requirement>(arg);
                    if (nestedReq._type == Type::AND || nestedReq._type == Type::OR)
                    {
                        reqStr += "(";
                        reqStr += nestedReq.to_string();
                        reqStr += ")";
                    }
                    else
                    {
                        reqStr += nestedReq.to_string();
                    }
                    reqStr += " and ";
                }
                // pop off the last " and "
                for (auto i = 0; i < 5; i++)
                {
                    reqStr.pop_back();
                }
                return reqStr;

            case Type::ITEM:
                item = std::get<item::Item*>(this->_args[0]);
                return item->GetName();

            case Type::COUNT:
                count = std::get<int>(this->_args[0]);
                item = std::get<item::Item*>(this->_args[1]);
                return "count(" + item->GetName() + ", " + std::to_string(count) + ")";

            case Type::EVENT:
                eventIndex = std::get<int>(this->_args[0]);
                return "'Event_" + std::to_string(eventIndex) + "'";

            case Type::MACRO:
                macroIndex = std::get<int>(this->_args[0]);
                return "'Macro_" + std::to_string(macroIndex) + "'";

            case Type::DAY:
                return "Day";

            case Type::NIGHT:
                return "Night";

            case Type::HUMAN_LINK:
                return "Human Link";

            case Type::WOLF_LINK:
                return "Wolf Link";

            case Type::TWILIGHT:
                return "Twilight";

            case Type::GOLDEN_BUGS:
                count = std::get<int>(this->_args[0]);
                return "golden_bugs(" + std::to_string(count) + ")";

            case Type::HEARTS:
                count = std::get<int>(this->_args[0]);
                return "hearts(" + std::to_string(count) + ")";

            case Type::DUNGEONS_COMPLETED:
                count = std::get<int>(this->_args[0]);
                return "dungeons_completed(" + std::to_string(count) + ")";

            default:
                return reqStr;
        }
        return reqStr;
    }

    Requirement ParseRequirementString(const std::string& reqStr,
                                       world::World* world,
                                       const bool& forceLogic /* = false */)
    {
        if (world->Setting("Logic Rules") == "No Logic" && !forceLogic) {
            return NO_REQUIREMENT;
        }

        Requirement req;
        std::string logicStr(reqStr);
        // First, we make sure that the expression has no missing or extra parenthesis
        // and that the nesting level at the beginning is the same at the end.
        //
        // Logic expressions are split up via spaces, but we only want to evaluate the parts of
        // the expression at the highest nesting level for the string that was passed in.
        // (We'll recursively call the function later to evaluate deeper levels.) So we replace
        // all the spaces on the highest nesting level with an arbitrarily chosen delimeter that shouldn't appear anywhere
        // in a logic statement (in req case: '+').
        int nestingLevel = 1;
        constexpr char delimeter = '+';
        for (auto& ch : logicStr)
        {
            if (ch == '(')
            {
                nestingLevel++;
            }
            else if (ch == ')')
            {
                nestingLevel--;
            }

            if (nestingLevel == 1 && ch == ' ')
            {
                ch = delimeter;
            }
        }

        // If the nesting level isn't the same as what we started with, then the logic
        // expression is invalid.
        if (nestingLevel != 1)
        {
            throw std::runtime_error("Extra or missing parenthesis within expression: \"" + reqStr + "\"");
        }

        // Next we split up the expression by the delimeter in the previous step
        size_t pos = 0;
        std::vector<std::string> splitLogicStr = {};
        while ((pos = logicStr.find(delimeter)) != std::string::npos)
        {
            // When parsing setting checks, take the entire expression
            // and the three components individually
            auto& chBefore = logicStr[pos - 1];
            auto& chAfter = logicStr[pos + 1];
            if (chBefore != '!' && chAfter != '!' && chBefore != '=' && chAfter != '=' &&
                chBefore != '>' && chAfter != '>' && chBefore != '<' && chAfter != '<')
            {
                splitLogicStr.push_back(logicStr.substr(0, pos));
                logicStr.erase(0, pos + 1);
            }
            else
            {
                logicStr.erase(logicStr.begin() + pos);
            }
        }
        splitLogicStr.push_back(logicStr);

        // Once we have the different parts of our expression, we can use the number
        // of parts we have to determine what kind of expression it is.

        // If we only have one part...
        if (splitLogicStr.size() == 1)
        {
            std::string argStr = splitLogicStr[0];
            std::ranges::replace(argStr, '_', ' ');
            // First, see if we have nothing
            if (argStr == "Nothing")
            {
                req._type = Type::NOTHING;
                return req;
            }

            // Then Human Link...
            if (argStr == "Human Link")
            {
                req._type = Type::HUMAN_LINK;
                return req;
            }

            // Then Wolf Link...
            if (argStr == "Wolf Link")
            {
                req._type = Type::WOLF_LINK;
                return req;
            }

            // Then Twilight...
            if (argStr == "Twilight")
            {
                req._type = Type::TWILIGHT;
                return req;
            }

            // Then an event...
            if (argStr[0] == '\'')
            {
                req._type = Type::EVENT;
                std::string eventName(argStr.begin() + 1, argStr.end() - 1); // Remove quotes
                int eventId = world->GetEventIndex(eventName);

                req._args.emplace_back(eventId);
                return req;
            }

            // NOTE: Checking macros *MUST* come before checking items. Some macros use the exact same name as an item
            // and we want the macro to be used in req case instead of just the item

            // Then a macro...
            if (world->GetMacroIndex(argStr) != -1)
            {
                req._type = Type::MACRO;
                req._args.emplace_back(world->GetMacroIndex(argStr));
                return req;
            }

            // Then an item...
            if (world->GetItem(argStr, true) != nullptr)
            {
                auto item = world->GetItem(argStr);
                req._type = Type::ITEM;
                req._args.emplace_back(item);
                return req;
            }

            // Then a setting...
            else if (utility::str::Contains(argStr, "!=", "==", ">=", "<="))
            {
                bool equalComparison = utility::str::Contains(argStr, "==");
                bool notEqualComparison = utility::str::Contains(argStr, "!=");
                bool gteComparison = utility::str::Contains(argStr, ">=");
                bool lteComparison = utility::str::Contains(argStr, "<=");

                // Split up the comparison using the second comparison character (which will always be '=')
                auto compPos = argStr.rfind('=');
                std::string optionName(argStr.begin() + (compPos + 1), argStr.end());
                std::string settingName(argStr.begin(), argStr.begin() + (compPos - 1));

                // Check using the appropriate comparison function
                bool result = false;
                if (equalComparison)
                {
                    result = world->Setting(settingName) == optionName.c_str();
                }
                else if (notEqualComparison)
                {
                    result = world->Setting(settingName) != optionName.c_str();
                } 
                else if (gteComparison)
                {
                    result = world->Setting(settingName) >= optionName.c_str();
                }
                else if (lteComparison)
                {
                    result = world->Setting(settingName) <= optionName.c_str();
                }

                if (result == true)
                {
                    req._type = Type::NOTHING;
                }
                else
                {
                    req._type = Type::IMPOSSIBLE;
                }
                return req;
            }
            // Then a count...
            else if (argStr.find("count") != std::string::npos)
            {
                req._type = Type::COUNT;
                // Since a count has two arguments (a number and an item), we have
                // to split up the string in the parenthesis into those arguments.

                // Get rid of parenthesis
                std::string countArgs(argStr.begin() + argStr.find('(') + 1, argStr.end() - 1);
                // Erase any spaces
                // countArgs.erase(std::remove(countArgs.begin(), countArgs.end(), ' '), countArgs.end());

                // Split up the arguments
                pos = 0;
                splitLogicStr = {};
                while ((pos = countArgs.find(", ")) != std::string::npos)
                {
                    splitLogicStr.push_back(countArgs.substr(0, pos));
                    countArgs.erase(0, pos + 2);
                }
                splitLogicStr.push_back(countArgs);

                // For the count, if a setting is passed in, use the setting's value instead
                auto& countStr = splitLogicStr[1];
                if (seedgen::settings::GetAllSettingsInfo()->contains(countStr))
                {
                    countStr = world->Setting(countStr).GetCurrentOption();
                }

                // Get the arguments
                auto& itemName = splitLogicStr[0];
                int count = std::stoi(countStr);
                auto item = world->GetItem(itemName);
                req._args.emplace_back(count);
                req._args.emplace_back(item);
                return req;
            }

            // Then Day...
            if (argStr == "Day")
            {
                req._type = Type::DAY;
                return req;
            }

            // Then Night...
            if (argStr == "Night")
            {
                req._type = Type::NIGHT;
                return req;
            }

            // Then health
            else if (argStr.find("hearts") != std::string::npos)
            {
                req._type = Type::HEARTS;
                std::string numHeartsStr(argStr.begin() + argStr.find('(') + 1, argStr.end() - 1);
                
                // If the string for the count is a setting, use the settings current option instead
                if (seedgen::settings::GetAllSettingsInfo()->contains(numHeartsStr))
                {
                    numHeartsStr = world->Setting(numHeartsStr).GetCurrentOption();
                }

                int numHearts = std::stoi(numHeartsStr);
                req._args.emplace_back(numHearts);
                return req;
            }

            // Then Impossible...
            else if (argStr == "Impossible")
            {
                req._type = Type::IMPOSSIBLE;
                return req;
            }

            // Then golden bugs...
            else if (argStr.find("golden bugs") != std::string::npos)
            {
                req._type = Type::GOLDEN_BUGS;
                // Get rid of parenthesis
                std::string countArg(argStr.begin() + argStr.find('(') + 1, argStr.end() - 1);
                int count = std::stoi(countArg);
                req._args.emplace_back(count);
                return req;
            }

            // Then dungeons completed
            else if (argStr.find("dungeons completed") != std::string::npos)
            {
                req._type = Type::DUNGEONS_COMPLETED;
                // Get rid of parenthesis
                std::string countStr(argStr.begin() + argStr.find('(') + 1, argStr.end() - 1);

                // For the count, if a setting is passed in, use the setting's value instead
                if (seedgen::settings::GetAllSettingsInfo()->contains(countStr))
                {
                    countStr = world->Setting(countStr).GetCurrentOption();
                }

                int count = std::stoi(countStr);
                req._args.emplace_back(count);
                return req;
            }

            throw std::runtime_error("Unrecognized logic symbol: \"" + reqStr + "\"");
        }

        // If our expression has two parts, then we don't know what that is
        if (splitLogicStr.size() == 2)
        {
            throw std::runtime_error("Unrecognized 2 part expression: " + reqStr);
        }

        // If we have more than two parts to our expression, then we have either "and"
        // or "or".
        bool andType = randomizer::utility::container::ElementInContainer(splitLogicStr, "and");
        bool orType = randomizer::utility::container::ElementInContainer(splitLogicStr, "or");

        // If we have both of them, there's a problem with the logic expression
        if (andType && orType)
        {
            throw std::runtime_error("\"and\" & \"or\" in same nesting level when parsing \"" + reqStr + "\"");
        }

        if (andType || orType)
        {
            // Set the appropriate type
            if (andType)
            {
                req._type = Type::AND;
            }
            else
            {
                req._type = Type::OR;
            }

            // Once we know the type, we can erase the "and"s or "or"s and are left with just the deeper
            // expressions to be logically operated on.
            randomizer::utility::container::FilterAndEraseFromVector(splitLogicStr,
                                                                [](const std::string& arg)
                                                                { return arg == "and" || arg == "or"; });

            // For each deeper expression, parse it and add it as an argument to the
            // Requirement
            for (auto& newReqStr : splitLogicStr)
            {
                // Get rid of parenthesis surrounding each deeper expression
                if (newReqStr[0] == '(')
                {
                    newReqStr = newReqStr.substr(1, newReqStr.length() - 2);
                }
                req._args.push_back(ParseRequirementString(newReqStr, world, forceLogic));
            }
        }

        if (req._type != Type::INVALID)
        {
            return req;
        }
        // If we've reached req point, we weren't able to determine a logical operator within the expression
        throw std::runtime_error("Could not determine logical operator type from expression: \"" + reqStr + "\"");
    }

    bool EvaluateSimpleRequirement(const Requirement& req, world::World* world)
    {
        item::Item* item;
        item::Item* heartPiece;
        item::Item* heartContainer;
        int count;
        int macroIndex;
        switch (req._type)
        {
            case Type::NOTHING:
                return true;

            case Type::IMPOSSIBLE:
                return false;

            case Type::OR:
                return std::ranges::any_of(
                    req._args,
                    [&](const auto& arg)
                    { return EvaluateSimpleRequirement(std::get<Requirement>(arg), world); });

            case Type::AND:
                return std::ranges::all_of(
                    req._args,
                    [&](const auto& arg)
                    { return EvaluateSimpleRequirement(std::get<Requirement>(arg), world); });

            case Type::ITEM:
                item = std::get<item::Item*>(req._args[0]);
                return randomizer::utility::container::ElementInContainer(world->GetStartingItemPool(), item);

            case Type::COUNT:
                count = std::get<int>(req._args[0]);
                item = std::get<item::Item*>(req._args[1]);
                return std::ranges::count(world->GetStartingItemPool(), item) >= count;

            case Type::MACRO:
                macroIndex = std::get<int>(req._args[0]);
                return EvaluateSimpleRequirement(world->GetMacro(macroIndex), world);

            case Type::GOLDEN_BUGS:
                count = std::get<int>(req._args[0]);
                return std::ranges::count_if(world->GetStartingItemPool(),
                                     [](const auto& item) { return item->IsGoldenBug(); }) >= count;

            case Type::HEARTS:
                count = std::get<int>(req._args[0]);
                heartPiece = world->GetItem("Piece of Heart");
                heartContainer = world->GetItem("Heart Container");
                return std::ranges::count(world->GetStartingItemPool(), heartPiece) + 
                       std::ranges::count(world->GetStartingItemPool(), heartContainer) * 5 >= count * 5;
            default:
                return false;
        }
    }

    bool EvaluateRequirementAtFormTime(const Requirement& req,
                                       search::Search* search,
                                       const int& formTime,
                                       world::World* world)
    {
        item::Item* item;
        item::Item* heartPiece;
        item::Item* heartContainer;
        int count;
        int eventIndex;
        int macroIndex;
        switch (req._type)
        {
            case Type::NOTHING:
                return true;

            case Type::IMPOSSIBLE:
                return false;

            case Type::OR:
                return std::ranges::any_of(
                    req._args,
                    [&](const auto& arg)
                    { return EvaluateRequirementAtFormTime(std::get<Requirement>(arg), search, formTime, world); });

            case Type::AND:
                return std::ranges::all_of(
                    req._args,
                    [&](const auto& arg)
                    { return EvaluateRequirementAtFormTime(std::get<Requirement>(arg), search, formTime, world); });

            case Type::ITEM:
                item = std::get<item::Item*>(req._args[0]);
                return search->_ownedItems.contains(item);

            case Type::COUNT:
                count = std::get<int>(req._args[0]);
                item = std::get<item::Item*>(req._args[1]);
                return search->_ownedItems.count(item) >= count;

            case Type::EVENT:
                eventIndex = std::get<int>(req._args[0]);
                return search->_ownedEvents.contains(eventIndex);

            case Type::MACRO:
                macroIndex = std::get<int>(req._args[0]);
                return EvaluateRequirementAtFormTime(world->GetMacro(macroIndex), search, formTime, world);

            case Type::DAY:
                return formTime & FormTime::DAY;

            case Type::NIGHT:
                return formTime & FormTime::NIGHT;

            case Type::HUMAN_LINK:
                return formTime & FormTime::HUMAN;

            case Type::WOLF_LINK:
                return formTime & FormTime::WOLF;

            case Type::TWILIGHT:
                return formTime & FormTime::TWILIGHT;

            case Type::GOLDEN_BUGS:
                count = std::get<int>(req._args[0]);
                return std::ranges::count_if(search->_ownedItems,
                                     [](const auto& ownedItem) { return ownedItem->IsGoldenBug(); }) >= count;
            
            case Type::HEARTS:
                count = std::get<int>(req._args[0]);
                heartPiece = world->GetItem("Piece of Heart");
                heartContainer = world->GetItem("Heart Container");
                return search->_ownedItems.count(heartPiece) + 
                       (search->_ownedItems.count(heartContainer) + 3) * 5 >= count * 5;

            case Type::DUNGEONS_COMPLETED:
                count = std::get<int>(req._args[0]);
                return std::ranges::count_if(search->_ownedEvents, [&](const int eventId) {
                    const std::list<std::string> dungeonCompletionEvents = {
                        "Can Complete Forest Temple",
                        "Can Complete Goron Mines",
                        "Can Complete Lakebed Temple",
                        "Can Complete Arbiters Grounds",
                        "Can Complete Snowpeak Ruins",
                        "Can Complete Temple of Time",
                        "Can Complete City in the Sky",
                        "Can Complete Palace of Twilight"  
                    };
                    for (const auto& eventName : dungeonCompletionEvents)
                    {
                        if (world->GetEventIndex(eventName) == eventId)
                        {
                            return true;
                        }
                    }
                    return false;
                }) >= count;

            default:
                return false;
        }
        return false;
    }

    EvalSuccess EvaluateEventRequirement(search::Search* search, area::EventAccess* event)
    {
        auto& formTime = search->_areaFormTime[event->GetArea()];
        if (EvaluateRequirementAtFormTime(event->GetRequirement(), search, formTime, event->GetArea()->GetWorld()))
        {
            return EvalSuccess::COMPLETE;
        }
        return EvalSuccess::NONE;
    }

    EvalSuccess EvaluateExitRequirement(search::Search* search, entrance::Entrance* exit)
    {
        // Some exits in the middle of entrance shuffling will not have a connected area. Ignore these
        if (exit->GetConnectedArea() == nullptr)
        {
            return EvalSuccess::DISCONNECTED;
        }

        // If the exit is currently disabled, don't try it
        if (exit->IsDisabled())
        {
            return EvalSuccess::NONE;
        }

        auto& exitFormTimeCache = exit->GetWorld()->GetExitTimeFormCache();
        auto parentArea = exit->GetParentArea();
        auto connectedArea = exit->GetConnectedArea();
        auto parentAreaFormTime = search->_areaFormTime[parentArea];
        auto& connectedAreaFormTime = search->_areaFormTime[connectedArea];
        auto potentialExitFormTimes = (exitFormTimeCache.contains(exit) ? exitFormTimeCache[exit] : FormTime::ALL);

        // LOG_TO_DEBUG("Trying " + connectedArea->GetName());

        auto connectedAreaTwilightCleared = connectedArea->TwilightCleared(search);
        if (!connectedAreaTwilightCleared)
        {
            // LOG_TO_DEBUG("Added Twilight");
            parentAreaFormTime |= FormTime::TWILIGHT;
            potentialExitFormTimes |= FormTime::TWILIGHT;
        }

        // Calculate the potential form times that we could spread to the connected area. These are the form times
        // which the connected area does not have that the parent area has, and that the exit can potentially pass on
        // to the connected area
        auto potentialFormTimeSpread = ~connectedAreaFormTime & (parentAreaFormTime & potentialExitFormTimes);

        // LOG_TO_DEBUG("Potential spreads: " + FormTime::to_string(potentialFormTimeSpread));

        // If there's no potential to spread FormTime, then return early
        if (potentialFormTimeSpread == FormTime::NONE)
        {
            // LOG_TO_DEBUG("No potential formtime spread");
            return EvalSuccess::NONE;
        }

        // Check each form time individually and spread the ones which succeed. If any of them pass, set the evaluation success
        // to partial.
        auto evalSuccess = EvalSuccess::NONE;
        const auto& formTimes = connectedAreaTwilightCleared ? FormTime::ALL_FORM_TIMES : FormTime::ALL_FORM_TIMES_AND_TWILIGHT;
        for (const auto& formTime : formTimes)
        {
            if (formTime & potentialFormTimeSpread)
            {
                if (EvaluateRequirementAtFormTime(exit->GetRequirement(), search, formTime, exit->GetWorld()))
                {
                    if (!connectedAreaTwilightCleared)
                    {
                        if (~connectedAreaFormTime & FormTime::TWILIGHT)
                        {
                            // LOG_TO_DEBUG("Spread Twilight to " + connectedArea->GetName());
                            connectedAreaFormTime |= FormTime::TWILIGHT;
                            evalSuccess = EvalSuccess::PARTIAL;
                        }
                    }
                    else if (formTime != FormTime::TWILIGHT)
                    {
                        // LOG_TO_DEBUG("Spread" + FormTime::to_string(formTime) + " to " + connectedArea->GetName());
                        connectedAreaFormTime |= formTime;
                        evalSuccess = EvalSuccess::PARTIAL;
                    }
                }
            }
            else
            {
                // LOG_TO_DEBUG(FormTime::to_string(formTime) + " is not a potential timespread.");
            }
        }

        if (evalSuccess != EvalSuccess::NONE)
        {
            search->ExpandFormTimes(connectedArea);
            // If the connected area now has complete access, then we mark a complete success instead of just a partial one
        }

        if (connectedAreaTwilightCleared && ((connectedAreaFormTime & potentialExitFormTimes) == potentialExitFormTimes))
        {
            evalSuccess = EvalSuccess::COMPLETE;
        }

        return evalSuccess;
    }

    EvalSuccess EvaluateDisconnectedExitRequiremrnt(search::Search* search, entrance::Entrance* exit)
    {
        // If the exit is currently disabled, don't try it
        if (exit->IsDisabled())
        {
            return EvalSuccess::NONE;
        }

        const auto parentArea = exit->GetParentArea();
        const auto parentAreaFormTime = search->_areaFormTime[parentArea];

        // Check each form time individually and spread the ones which succeed. If any of them pass, set the evaluation success
        // to partial.
        for (const auto& formTime : FormTime::ALL_FORM_TIMES)
        {
            if (formTime & parentAreaFormTime)
            {
                if (EvaluateRequirementAtFormTime(exit->GetRequirement(), search, formTime, exit->GetWorld()))
                {
                    return EvalSuccess::PARTIAL;
                }
            }
        }
        return EvalSuccess::NONE;
    }

    EvalSuccess EvaluateLocationRequirement(search::Search* search, area::LocationAccess* locAccess)
    {
        auto& formTime = search->_areaFormTime[locAccess->GetArea()];
        if (EvaluateRequirementAtFormTime(locAccess->GetRequirement(), search, formTime, locAccess->GetArea()->GetWorld()))
        {
            return EvalSuccess::COMPLETE;
        }
        return EvalSuccess::NONE;
    }
} // namespace randomizer::logic::requirement
