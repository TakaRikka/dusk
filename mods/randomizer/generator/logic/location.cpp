#include "location.hpp"

#include "world.hpp"
#include "../utility/log.hpp"

namespace randomizer::logic::location
{
    Location::Location(const int& id,
                       const std::string& name,
                       const std::unordered_set<std::string>& categories,
                       world::World* world,
                       item::Item* originalItem,
                       const bool& goalLocation,
                       const std::string& hintPriority,
                       const YAML::Node& metadata):
        _id(id),
        _name(name),
        _categories(categories),
        _world(world),
        _originalItem(originalItem),
        _goalLocation(goalLocation),
        _hintPriority(hintPriority),
        _metadata(metadata)
    {
        this->_computedRequirement._type = requirement::Type::IMPOSSIBLE;
    }

    int Location::GetID() const
    {
        return this->_id;
    }

    std::string Location::GetName() const
    {
        return this->_name;
    }

    world::World* Location::GetWorld() const
    {
        return this->_world;
    }

    bool Location::IsGoalLocation() const
    {
        return this->_goalLocation;
    }

    void Location::SetCurrentItem(item::Item* item)
    {
        LOG_TO_DEBUG("Placed " + item->GetName() + " at " + this->GetName());
        this->_currentItem = item;
    }

    item::Item* Location::GetCurrentItem() const
    {
        return this->_currentItem;
    }

    void Location::RemoveCurrentItem()
    {
        LOG_TO_DEBUG("Removed " + this->GetCurrentItem()->GetName() + " at " + this->GetName());
        this->_currentItem = item::Nothing.get();
    }

    bool Location::IsEmpty() const
    {
        return this->_currentItem == item::Nothing.get();
    }

    item::Item* Location::GetOriginalItem() const
    {
        return this->_originalItem;
    }

    item::Item* Location::GetTrackedItem() const
    {
        return this->_trackedItem;
    }

    void Location::SetKnownVanillaItem(const bool& hasKnownVanillaItem)
    {
        this->_hasKnownVanillaItem = hasKnownVanillaItem;
    }

    bool Location::HasKnownVanillaItem() const
    {
        return this->_hasKnownVanillaItem;
    }

    void Location::SetProgression(const bool& progression)
    {
        this->_progression = progression;
        LOG_TO_DEBUG(this->GetName() + " progression status set to " + (progression ? " true" : "false"));
    }

    bool Location::IsProgression() const
    {
        return this->_progression;
    }

    void Location::SetHinted(const bool& hinted)
    {
        this->_hinted = hinted;
    }

    bool Location::IsHinted() const
    {
        return this->_hinted;
    }

    const YAML::Node& Location::GetMetadata() const
    {
        return this->_metadata;
    }

    void Location::AddLocationAccess(area::LocationAccess* locAcc)
    {
        this->_locationAccessList.push_back(locAcc);
    }

    std::list<area::LocationAccess*> Location::GetAccessList() const
    {
        return this->_locationAccessList;
    }

    void Location::AddForbiddenItem(item::Item* forbiddenItem)
    {
        this->_forbiddenItems.insert(forbiddenItem);
        LOG_TO_DEBUG(forbiddenItem->GetName() + " is forbidden from being placed at " + this->GetName());
    }

    const std::unordered_set<item::Item*>& Location::GetForbiddenItems()
    {
        return this->_forbiddenItems;
    }

    void Location::SetComputedRequirement(const requirement::Requirement& computedRequirement)
    {
        this->_computedRequirement = computedRequirement;
    }

    requirement::Requirement Location::GetComputedRequirement()
    {
        return this->_computedRequirement;
    }

    void Location::SetRegisteredLocationCategories(std::unordered_set<std::string>* registeredLocationCategories)
    {
        this->_registeredLocationCategories = registeredLocationCategories;
    }

    const std::set<std::string>& GetAllRandomizerLocationNames() {
        static std::set<std::string> locationNames{};

        if (locationNames.empty()) {
            auto locationDataTree = LOAD_EMBED_YAML(RANDO_DATA_PATH "locations.yaml");
            for (const auto& locationNode : locationDataTree) {
                locationNames.insert(locationNode["Name"].as<std::string>());
            }
        }

        return locationNames;
    }
} // namespace randomizer::logic::location
