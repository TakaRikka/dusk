#pragma once

#include "entrance.hpp"
#include "world.hpp"

namespace randomizer::logic::entrance_shuffle
{
    void ShuffleWorldEntrances(world::World* world);
    void SetAllEntrancesData(world::World* world);
    entrance::EntrancePools CreateEntrancePools(world::World* world);
    entrance::EntrancePools CreateTargetPools(entrance::EntrancePools& entrancePools);
    entrance::EntrancePool AssumeEntrancePool(entrance::EntrancePool& entrancePool);
    void SetPlandomizedEntrances(world::World* world,
                                 entrance::EntrancePools& entrancePools,
                                 entrance::EntrancePools& targetEntrancePools);
    void ShuffleNonAssumedEntrancesPools(world::World* world,
                                         entrance::EntrancePools& entrancePools,
                                         entrance::EntrancePools& targetEntrancePools);
    void ShuffleEntrancePool(entrance::EntrancePool& entrancePool,
                             entrance::EntrancePool& targetEntrancePool,
                             int retries = 20);
    void ShuffleEntrances(entrance::EntrancePool& entrancePool,
                          entrance::EntrancePool& targetEntrancePool,
                          std::unordered_map<entrance::Entrance*, entrance::Entrance*>& rollbacks);
    bool ReplaceEntrance(entrance::Entrance* entrance,
                         entrance::Entrance* target,
                         std::unordered_map<entrance::Entrance*, entrance::Entrance*>& rollbacks,
                         const item_pool::ItemPool& completeItemPool);

    void CheckEntrancesCompatibility(const entrance::Entrance* entrance,
                                     const entrance::Entrance* target);
    void ChangeConnections(entrance::Entrance* entrance, entrance::Entrance* target);
    void RestoreConnections(entrance::Entrance* entrance, entrance::Entrance* target);
    void ConfirmReplacement(entrance::Entrance* entrance, entrance::Entrance* target);
    void DeleteTargetEntrance(entrance::Entrance* target);
    void ValidateWorld(world::World* world,
                       entrance::Entrance* entrance,
                       const item_pool::ItemPool& completeItemPool);

    void SetShuffledEntrances(entrance::EntrancePools& entrancePools);
    entrance::EntrancePool GetReverseEntrances(const entrance::EntrancePool& entrances);
    const std::set<std::string>& GetPossibleMixedPoolTypes();

    class EntranceShuffleError: public std::runtime_error
    {
       public:
        explicit EntranceShuffleError(const std::string& message): std::runtime_error(message) {}
    };
} // namespace randomizer::logic::entrance_shuffle
