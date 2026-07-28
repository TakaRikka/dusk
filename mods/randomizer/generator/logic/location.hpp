#pragma once

#include "item.hpp"
#include "requirement.hpp"
#include "../utility/yaml.hpp"

#include <stdexcept>
#include <unordered_set>
#include <vector>

namespace randomizer::logic::world
{
    class World;
}

namespace randomizer::logic::area
{
    class LocationAccess;
}

namespace randomizer::logic::location
{
    class Location
    {
       public:
        Location(const int& id,
                 const std::string& name,
                 const std::unordered_set<std::string>& categories,
                 world::World* world,
                 item::Item* originalItem,
                 const bool& goalLocation,
                 const std::string& hintPriority,
                 const YAML::Node& metadata);

        int GetID() const;
        std::string GetName() const;
        world::World* GetWorld() const;
        bool IsGoalLocation() const;
        void SetCurrentItem(item::Item* currentItem);
        item::Item* GetCurrentItem() const;
        void RemoveCurrentItem();
        bool IsEmpty() const;
        item::Item* GetOriginalItem() const;
        item::Item* GetTrackedItem() const;
        void SetKnownVanillaItem(const bool& hasKnownVanillaItem);
        bool HasKnownVanillaItem() const;
        void SetProgression(const bool& progression);
        bool IsProgression() const;
        void SetHinted(const bool& hinted);
        bool IsHinted() const;
        const YAML::Node& GetMetadata() const;
        void AddLocationAccess(area::LocationAccess* locAcc);
        std::list<area::LocationAccess*> GetAccessList() const;
        void AddForbiddenItem(item::Item* forbiddenItem);
        const std::unordered_set<item::Item*>& GetForbiddenItems();
        void SetComputedRequirement(const requirement::Requirement& computedRequirement);
        requirement::Requirement GetComputedRequirement();
        void SetRegisteredLocationCategories(std::unordered_set<std::string>* registeredLocationCategories);

        /**
         *  @brief Checks to see if the location has all the passed in categories. If a passed in category was never registered,
         *  a std::runtime_error will be thrown.
         *  @param categoryNames parameter pack of string representations of category names
         *  @returns true if all passed in categories are present, false otherwise
         */
        template<class... Types>
        bool HasCategories(Types... categoryNames) const
        {
            for (const auto& categoryName : {categoryNames...})
            {
                if (this->_registeredLocationCategories != nullptr &&
                    !this->_registeredLocationCategories->contains(categoryName))
                {
                    throw std::runtime_error(std::string("Category \"") + categoryName + "\" is not used by any locations");
                }
                if (!this->_categories.contains(categoryName))
                {
                    return false;
                }
            }

            return true;
        }

       private:
        int _id = -1;
        std::string _name = "";
        std::unordered_set<std::string> _categories = {};
        world::World* _world;
        item::Item* _originalItem = item::Nothing.get();
        bool _goalLocation = false;
        item::Item* _currentItem = item::Nothing.get();
        bool _hasKnownVanillaItem = false;
        std::list<area::LocationAccess*> _locationAccessList = {};
        bool _progression = true; // Set as false later if applicable
        bool _hinted = false;
        std::string _hintPriority = "Never";
        std::unordered_set<item::Item*> _forbiddenItems = {};
        requirement::Requirement _computedRequirement;
        YAML::Node _metadata{};
        /**
         *  @brief _registeredLocationCategories is the set of all categories that are processed after reading locations.yaml.
         * This structure is held in the World class and every location in that world has a pointer to it.
         * We can't call it from the world directly since the function we want to use it in is templated in this class.
         */
        std::unordered_set<std::string>* _registeredLocationCategories = nullptr;

        // Potential tracker stuff
        item::Item* _trackedItem = item::Nothing.get();
    };

    using LocationPool = std::vector<Location*>;

    /**
     *
     * @return A set of all randomizer location names
     */
    const std::set<std::string>& GetAllRandomizerLocationNames();
} // namespace randomizer::logic::location
