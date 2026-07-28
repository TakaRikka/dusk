#include "item.hpp"

#include "world.hpp"

namespace randomizer::logic::item
{

    Importance ImportanceFromStr(const std::string& str)
    {
        const std::unordered_map<std::string, Importance> importances = {
            {"Major", Importance::MAJOR},
            {"Minor", Importance::MINOR},
            {"Junk", Importance::JUNK}
        };

        if (!importances.contains(str))
        {
            return Importance::INVALID;
        }

        return importances.at(str);
    }

    Item::Item(const int& id,
               const std::string& name,
               world::World* world,
               const Importance& importance,
               const bool& gameWinningItem,
               const bool& dungeonSmallKey,
               const bool& bigKey,
               const bool& compass,
               const bool& dungeonMap):
        _id(id),
        _name(name),
        _world(world),
        _importance(importance),
        _gameWinningItem(gameWinningItem),
        _dungeonSmallKey(dungeonSmallKey),
        _bigKey(bigKey),
        _compass(compass),
        _dungeonMap(dungeonMap)
    {
        if (name.starts_with("Male") || name.starts_with("Female"))
        {
            this->_goldenBug = true;
        }
        else if (name == "Shadow Crystal")
        {
            this->_shadowCrystal = true;
        }
        else if (name.starts_with("Bottle") || name == "Empty Bottle")
        {
            this->_bottle = true;
        }
        else if (name.starts_with("Stamp"))
        {
            this->_stamp = true;
        }
        // Make hearts major items if they're required for anything
        else if ((name == "Piece of Heart" || name == "Heart Container") &&
                 ((world->Setting("Hyrule Barrier Requirements") == "Hearts") ||
                  (world->Setting("Hyrule Castle Big Key Requirements") == "Hearts")))
        {
            this->_importance = Importance::MAJOR;
        }
    }

    int Item::GetID() const
    {
        return this->_id;
    }

    std::string Item::GetName() const
    {
        return this->_name;
    }

    world::World* Item::GetWorld() const
    {
        return this->_world;
    }

    Importance Item::GetImportance() const
    {
        return this->_importance;
    }

    bool Item::IsMajor() const
    {
        return this->_importance == Importance::MAJOR;
    }

    bool Item::IsMinor() const
    {
        return this->_importance == Importance::MINOR;
    }

    bool Item::isJunk() const
    {
        return this->_importance == Importance::JUNK;
    }

    bool Item::IsGameWinningItem() const
    {
        return this->_gameWinningItem;
    }

    std::list<Location*> Item::GetChainLocations() const
    {
        return this->_chainLocations;
    }

    bool Item::IsDungeonSmallKey() const
    {
        return this->_dungeonSmallKey;
    }

    bool Item::IsBigKey() const
    {
        return this->_bigKey;
    }

    bool Item::IsDungeonMap() const
    {
        return this->_dungeonMap;
    }

    bool Item::IsCompass() const
    {
        return this->_compass;
    }

    bool Item::IsGoldenBug() const
    {
        return this->_goldenBug;
    }

    bool Item::IsShadowCrystal() const
    {
        return this->_shadowCrystal;
    }

    bool Item::IsBottle() const
    {
        return this->_bottle;
    }
    
    bool Item::IsStamp() const
    {
        return this->_stamp;
    }

    bool Item::operator==(const Item& rhs) const
    {
        return this->_id == rhs._id && this->_world->GetID() == rhs._world->GetID();
    }

    bool Item::operator<(const Item& rhs) const
    {
        return (this->_world->GetID() == rhs._world->GetID()) ? this->_id < rhs._id
                                                              : this->_world->GetID() < rhs._world->GetID();
    }

    std::unique_ptr<Item> Nothing =
        std::make_unique<Item>(-1, "Nothing", nullptr, Importance::JUNK, false, false, false, false, false);
} // namespace randomizer::logic::item
