#include "flatten.hpp"

#include <ranges>

#include "../world.hpp"

FlattenSearch::FlattenSearch(randomizer::logic::world::World* world_)
{
    world = world_;

    for (const auto& area : world->GetAreaTable() | std::views::values)
    {
        for (const auto& exit : area->GetExits())
        {
            auto visit = visitor(exit, this);
            visitReq(exit->GetRequirement(), visit, world);
        }

        for (const auto& event : area->GetEvents())
        {
            auto visit = visitor(event, this);
            visitReq(event->GetRequirement(), visit, world);
        }
    }

    const auto root = world->GetRootArea();
    // Start with all formtimes at the root, false for everything else
    auto formTimes = randomizer::logic::requirement::FormTime::ALL_FORM_AND_DAY_TIMES;
    formTimes.push_back(randomizer::logic::requirement::FormTime::TWILIGHT);
    for (const auto& area : world->GetAreaTable() | std::views::values)
    {
        for (const auto& formTime : formTimes)
        {
            if (area.get() == root)
            {
                areaExprs[formTime][area.get()] = DNF::True();
            }
            else
            {
                areaExprs[formTime][area.get()] = DNF::False();
            }
        }
    }
    newlyUpdatedAreas.insert(root);
    newThingsFound = true;

    for (auto& exit : root->GetExits())
    {
        if (exit->GetConnectedArea() != nullptr)
        {
            exitsToTry.insert(exit);
        }
    }
}

void FlattenSearch::doSearch()
{
    // This algorithm works in three stages:
    // 1. Compute area and event requirements -> DNFs
    // 2. Compute location requirement -> DNF
    // 3. Simplify location requirement -> Requirement

    // This is step 1. This computes everything that requirements
    // can depend on in a fixpoint algorithm - namely, area access and events.
    newThingsFound = true;
    while (newThingsFound)
    {
        recentlyUpdatedAreas = newlyUpdatedAreas;
        recentlyUpdatedEvents = newlyUpdatedEvents;
        newlyUpdatedAreas = {};
        newlyUpdatedEvents = {};
        newThingsFound = false;
        tryExits();
        tryEvents();
        tryTimeFormExpansion();
    }

    std::unordered_map<std::string, std::list<randomizer::logic::area::LocationAccess*>> itemLocations = {};
    for (const auto& area : world->GetAreaTable() | std::views::values)
    {
        for (auto& locAccess : area->GetLocations())
        {
            auto locationName = locAccess->GetLocation()->GetName();
            if (!itemLocations.contains(locationName))
            {
                itemLocations[locationName] = {};
            }
            itemLocations[locationName].push_back(locAccess);
        }
    }
    // TODO this immediately combines the "local" requirements with the implicit
    // area requirement. It has been hypothesized that converting them
    // separately may produce better tooltips, but at that point you need the
    // TWWR-Tracker boolean-expression multi-level simplification code

    // Step 2: for every location, OR all the ways to access it

    auto formTimes = randomizer::logic::requirement::FormTime::ALL_FORM_AND_DAY_TIMES;
    formTimes.push_back(randomizer::logic::requirement::FormTime::TWILIGHT);
    for (auto& [locName, accessList] : itemLocations)
    {
        auto expr = DNF::False();
        for (const auto& locAcc : accessList)
        {
            for (const auto& formTime : formTimes)
            {
                expr = expr.or_(tryLocationAtFormTime(locAcc, formTime));
            }
        }

        // Step 3: simplify
        auto location = world->GetLocation(locName);
        location->SetComputedRequirement(DNFToExpr(bitIndex, expr.dedup()));
        // world->locationTable[locName]->computedRequirement.simplifyParenthesis();
        // world->locationTable[locName]->computedRequirement.sortArgs();
    }

    // Do the same for any shuffled entrances so that we can give them tooltips in the tracker
    for (auto& [name, area] : world->GetAreaTable())
    {
        for (auto& exit : area->GetExits())
        {
            if (exit->IsShuffled())
            {
                auto expr = DNF::False();
                auto& validFormTimes = exit->GetWorld()->GetExitTimeFormCache()[exit];
                for (const auto& formTime : randomizer::logic::requirement::FormTime::ALL_FORM_TIMES)
                {
                    if (formTime & validFormTimes)
                    {
                        expr = expr.or_(tryExitAtFormTime(exit, formTime));
                    }
                }
                exit->SetComputedRequirement(DNFToExpr(bitIndex, expr.dedup()));
            }
        }
    }
}

// Check for a thing in area whether its logical dependencies
// have recently been updated.
bool FlattenSearch::wasUpdated(randomizer::logic::area::Area* area, void* thing)
{
    if (recentlyUpdatedAreas.contains(area))
    {
        return true;
    }

    auto& remoteEventReqs = remoteEventRequirements[thing];
    for (auto& event : remoteEventReqs)
    {
        if (recentlyUpdatedEvents.contains(event))
        {
            return true;
        }
    }
    // auto& remoteAreaReqs = remoteAreaRequirements[thing];
    // for (auto& areaStr : remoteAreaReqs)
    // {
    //     randomizer::logic::area::Area* area2;
    //     world->GetArea(areaStr, area2);
    //     if (recentlyUpdatedAreas.contains(area2))
    //     {
    //         return true;
    //     }
    // }

    return false;
}

void FlattenSearch::tryExits()
{
    using namespace randomizer::logic::requirement;
    auto exits = exitsToTry;
    for (auto& exit : exits)
    {
        if (!wasUpdated(exit->GetParentArea(), (void*)exit))
        {
            continue;
        }
        auto& validFormTimes = exit->GetWorld()->GetExitTimeFormCache()[exit];
        auto connectedTwilight = exit->GetConnectedArea()->GetTwilightCompletedMacroIndex() != -1;
        if (connectedTwilight)
        {
            validFormTimes |= FormTime::TWILIGHT;
        }
        for (const auto& formTime : FormTime::ALL_FORM_TIMES_AND_TWILIGHT)
        {
            if (formTime & validFormTimes)
            {
                auto connectedArea = exit->GetConnectedArea();
                auto& oldExpr = areaExprs[formTime][connectedArea];
                auto newPartial = tryExitAtFormTime(exit, formTime);

                // Add the twilight completed macro for access to this area if it's part of a twilight
                if (connectedTwilight && formTime != FormTime::TWILIGHT)
                {
                    auto& oldExprTwilight = areaExprs[FormTime::TWILIGHT][connectedArea];
                    auto [useful, newExpr] = oldExprTwilight.or_useful(newPartial);
                    if (useful)
                    {
                        newlyUpdatedAreas.insert(connectedArea);
                        newThingsFound = true;
                        areaExprs[FormTime::TWILIGHT][connectedArea] = newExpr.dedup();
                        for (auto& event : connectedArea->GetEvents())
                        {
                            eventsToTry.insert(event);
                        }
                        for (auto& areaExit : connectedArea->GetExits())
                        {
                            if (areaExit->GetConnectedArea() != nullptr)
                            {
                                exitsToTry.insert(areaExit);
                            }
                        }
                        areasToTry.insert(connectedArea);
                    }

                    newPartial = newPartial.and_(
                        evaluatePartialRequirement(bitIndex,
                                                   exit->GetWorld()->GetMacro(connectedArea->GetTwilightCompletedMacroIndex()),
                                                   this,
                                                   0));
                }

                auto [useful, newExpr] = oldExpr.or_useful(newPartial);
                if (useful)
                {
                    newlyUpdatedAreas.insert(connectedArea);
                    newThingsFound = true;
                    areaExprs[formTime][connectedArea] = newExpr.dedup();
                    for (auto& event : connectedArea->GetEvents())
                    {
                        eventsToTry.insert(event);
                    }
                    for (auto& areaExit : connectedArea->GetExits())
                    {
                        if (areaExit->GetConnectedArea() != nullptr)
                        {
                            exitsToTry.insert(areaExit);
                        }
                    }
                    areasToTry.insert(connectedArea);
                }
            }
        }
    }
}

void FlattenSearch::tryEvents()
{
    for (auto& event : eventsToTry)
    {
        if (!wasUpdated(event->GetArea(), (void*)event))
        {
            continue;
        }

        auto& oldExpr = eventExprs[event->GetEventIndex()];
        auto newPartial = DNF::False();
        for (const auto& formTime : randomizer::logic::requirement::FormTime::ALL_FORM_AND_DAY_TIMES)
        {
            newPartial = newPartial.or_(tryEventAtFormTime(event, formTime));
        }
        auto [useful, newExpr] = oldExpr.or_useful(newPartial);
        if (useful)
        {
            newlyUpdatedEvents.insert(event->GetEventIndex());
            newThingsFound = true;
            eventExprs[event->GetEventIndex()] = newExpr.dedup();
        }
    }
}

void FlattenSearch::tryTimeFormExpansion()
{
    using namespace randomizer::logic::requirement;
    for (auto& area : areasToTry)
    {
        if (!recentlyUpdatedAreas.contains(area))
        {
            continue;
        }
        if (area->CanTransform())
        {
            auto shadowCrystal = area->GetWorld()->GetShadowCrystal();
            auto shadowCrystalDNF = evaluatePartialRequirement(bitIndex, Requirement {Type::ITEM, {shadowCrystal}}, this, 0);
            for (const auto& formTime : FormTime::ALL_FORM_TIMES)
            {
                auto& oldExpr = areaExprs[formTime][area];
                int oppositeFormTime = FormTime::NONE;
                switch (formTime)
                {
                    case FormTime::HUMAN_DAY:
                        oppositeFormTime = FormTime::WOLF_DAY;
                        break;
                    case FormTime::HUMAN_NIGHT:
                        oppositeFormTime = FormTime::WOLF_NIGHT;
                        break;
                    case FormTime::WOLF_DAY:
                        oppositeFormTime = FormTime::HUMAN_DAY;
                        break;
                    case FormTime::WOLF_NIGHT:
                        oppositeFormTime = FormTime::HUMAN_NIGHT;
                }
                auto newPartial = areaExprs[oppositeFormTime][area];
                if (!newPartial.isTriviallyFalse())
                {
                    // Transforming requires shadow crystal
                    newPartial = newPartial.and_(shadowCrystalDNF);
                    auto [useful, newExpr] = oldExpr.or_useful(newPartial);
                    if (useful)
                    {
                        newlyUpdatedAreas.insert(area);
                        newThingsFound = true;
                        areaExprs[formTime][area] = newExpr.dedup();
                    }
                }
            }
        }
        if (area->CanChangeTime())
        {
            for (const auto& formTime : FormTime::ALL_FORM_TIMES)
            {
                auto& oldExpr = areaExprs[formTime][area];
                int oppositeFormTime = FormTime::NONE;
                switch (formTime)
                {
                    case FormTime::HUMAN_DAY:
                        oppositeFormTime = FormTime::HUMAN_NIGHT;
                        break;
                    case FormTime::HUMAN_NIGHT:
                        oppositeFormTime = FormTime::HUMAN_DAY;
                        break;
                    case FormTime::WOLF_DAY:
                        oppositeFormTime = FormTime::WOLF_NIGHT;
                        break;
                    case FormTime::WOLF_NIGHT:
                        oppositeFormTime = FormTime::WOLF_DAY;
                }
                auto newPartial = areaExprs[oppositeFormTime][area];
                if (!newPartial.isTriviallyFalse())
                {
                    auto [useful, newExpr] = oldExpr.or_useful(newPartial);
                    if (useful)
                    {
                        newlyUpdatedAreas.insert(area);
                        newThingsFound = true;
                        areaExprs[formTime][area] = newExpr.dedup();
                    }
                }
            }
        }
        this->andAreaFormTimes(area);
    }
}

void FlattenSearch::andAreaFormTimes(randomizer::logic::area::Area* area)
{
    using namespace randomizer::logic::requirement;

    auto& areaHumanDay = this->areaExprs[FormTime::HUMAN_DAY][area];
    auto& areaWolfDay = this->areaExprs[FormTime::WOLF_DAY][area];
    auto& areaHumanNight = this->areaExprs[FormTime::HUMAN_NIGHT][area];
    auto& areaWolfNight = this->areaExprs[FormTime::WOLF_NIGHT][area];

    this->areaExprs[FormTime::DAY][area] = areaHumanDay.and_(areaWolfDay);
    this->areaExprs[FormTime::NIGHT][area] = areaHumanNight.and_(areaWolfNight);
}

DNF FlattenSearch::tryEventAtFormTime(randomizer::logic::area::EventAccess* event, const int& formTime)
{
    return areaExprs[formTime][event->GetArea()].and_(
        evaluatePartialRequirement(bitIndex, event->GetRequirement(), this, formTime));
}

DNF FlattenSearch::tryLocationAtFormTime(randomizer::logic::area::LocationAccess* location, const int& formTime)
{
    return areaExprs[formTime][location->GetArea()].and_(
        evaluatePartialRequirement(bitIndex, location->GetRequirement(), this, formTime));
}

DNF FlattenSearch::tryExitAtFormTime(randomizer::logic::entrance::Entrance* exit, const int& formTime)
{
    return areaExprs[formTime][exit->GetParentArea()].and_(
        evaluatePartialRequirement(bitIndex, exit->GetRequirement(), this, formTime));
}

DNF evaluatePartialRequirement(BitIndex& bitIndex,
                               const randomizer::logic::requirement::Requirement& req,
                               FlattenSearch* search,
                               const int& formTime)
{
    uint32_t expectedCount = 0;
    uint32_t expectedHearts = 0;
    uint32_t totalHearts = 0;
    std::bitset<512> bits = 0;
    randomizer::logic::item::Item* item;
    int event;
    DNF d = DNF();
    randomizer::logic::area::Area* area;

    switch (req._type)
    {
        case randomizer::logic::requirement::Type::NOTHING:
            return DNF::True();

        case randomizer::logic::requirement::Type::IMPOSSIBLE:
            return DNF::False();

        case randomizer::logic::requirement::Type::OR:
            d = DNF::False();
            for (auto& arg : req._args)
            {
                d = d.or_(evaluatePartialRequirement(bitIndex,
                                                     std::get<randomizer::logic::requirement::Requirement>(arg),
                                                     search,
                                                     formTime));
            }
            return d;

        case randomizer::logic::requirement::Type::AND:
            d = DNF::True();
            for (auto& arg : req._args)
            {
                d = d.and_(evaluatePartialRequirement(bitIndex,
                                                      std::get<randomizer::logic::requirement::Requirement>(arg),
                                                      search,
                                                      formTime));
            }
            return d;
        
        case randomizer::logic::requirement::Type::ITEM:
            [[fallthrough]];
        case randomizer::logic::requirement::Type::GOLDEN_BUGS:
            [[fallthrough]];
        case randomizer::logic::requirement::Type::HEARTS:
            [[fallthrough]];
        case randomizer::logic::requirement::Type::DUNGEONS_COMPLETED:
            bits[bitIndex.reqBit(req)] = 1;
            return DNF({bits});

        case randomizer::logic::requirement::Type::EVENT:
            event = std::get<int>(req._args[0]);
            return search->eventExprs[event];

        case randomizer::logic::requirement::Type::MACRO:
            return evaluatePartialRequirement(bitIndex, search->world->GetMacro(std::get<int>(req._args[0])), search, formTime);

        // count requirements frequently have to unify with weaker terms,
        // so a count requirement always requires all lesser item counts too.
        // this ensures redundant terms can be eliminated
        case randomizer::logic::requirement::Type::COUNT:
            expectedCount = std::get<int>(req._args[0]);
            item = std::get<randomizer::logic::item::Item*>(req._args[1]);
            for (auto i = 1; i <= expectedCount; i++)
            {
                randomizer::logic::requirement::Requirement newReq;
                if (i == 1)
                {
                    newReq = randomizer::logic::requirement::Requirement {randomizer::logic::requirement::Type::ITEM, {item}};
                }
                else
                {
                    newReq = randomizer::logic::requirement::Requirement {randomizer::logic::requirement::Type::COUNT, {i, item}};
                }
                bits[bitIndex.reqBit(newReq)] = 1;
            }
            return DNF({bits});

        case randomizer::logic::requirement::Type::DAY:
            return (formTime & randomizer::logic::requirement::FormTime::DAY) ? DNF::True() : DNF::False();

        case randomizer::logic::requirement::Type::NIGHT:
            return (formTime & randomizer::logic::requirement::FormTime::NIGHT) ? DNF::True() : DNF::False();

        case randomizer::logic::requirement::Type::HUMAN_LINK:
            return (formTime & randomizer::logic::requirement::FormTime::HUMAN) ? DNF::True() : DNF::False();

        case randomizer::logic::requirement::Type::WOLF_LINK:
            return (formTime & randomizer::logic::requirement::FormTime::WOLF) ? DNF::True() : DNF::False();

        case randomizer::logic::requirement::Type::TWILIGHT:
            return (formTime & randomizer::logic::requirement::FormTime::TWILIGHT) ? DNF::True() : DNF::False();

        case randomizer::logic::requirement::Type::INVALID:
        default:
            // actually needs to be some error state?
            return DNF::False();
    }
    return DNF::False();
}

void visitReq(const randomizer::logic::requirement::Requirement& req,
              std::function<void(const randomizer::logic::requirement::Requirement& req)> f,
              randomizer::logic::world::World* world)
{
    f(req);
    if (req._type == randomizer::logic::requirement::Type::AND || req._type == randomizer::logic::requirement::Type::OR)
    {
        for (auto& arg : req._args)
        {
            visitReq(std::get<randomizer::logic::requirement::Requirement>(arg), f, world);
        }
    }
    else if (req._type == randomizer::logic::requirement::Type::MACRO)
    {
        visitReq(world->GetMacro(std::get<int>(req._args[0])), f, world);
    }
}
