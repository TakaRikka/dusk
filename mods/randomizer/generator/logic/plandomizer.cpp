#include "plandomizer.hpp"

#include "world.hpp"

#include "../utility/yaml.hpp"
#include "../utility/file.hpp"

namespace randomizer::logic::plandomizer
{
    void LoadPlandomizerData(world::WorldPool& worlds, const fspath& filepath, const bool& ignoreErrors /*false*/)
    {
        // Verify the file exists before trying to open it
        utility::file::Verify(filepath);

        auto plandoTree = LoadYAML(filepath);

        for (auto& world : worlds)
        {
            int worldId = world->GetID();
            std::string worldStr = "World " + std::to_string(world->GetID());
            if (plandoTree[worldStr])
            {
                const auto& worldNode = plandoTree[worldStr];
                if (worldNode["Locations"])
                {
                    const auto& locations = worldNode["Locations"];
                    if (!locations.IsMap())
                    {
                        if (ignoreErrors)
                        {
                            return;
                        }

                        throw std::runtime_error("Locations for " + worldStr +
                                                 " is not a map. Please check your plandomizer file syntax.");
                    }

                    for (const auto& locationNode : locations)
                    {
                        // If the location object has children instead of a value, then parse the item name and potential
                        // world id from those children. If no world id is given, the current world will be used.
                        std::string itemName;
                        if (locationNode.second.IsMap())
                        {
                            if (locationNode.second["Item"])
                            {
                                itemName = locationNode.second["Item"].as<std::string>();
                            }
                            else
                            {
                                throw std::runtime_error("Missing key \"item\" in node:\n" +
                                                         YAML::Dump(locationNode));
                            }

                            if (locationNode.second["World"])
                            {
                                worldId = locationNode.second["World"].as<int>();
                                if (worldId < 1 || worldId > worlds.size())
                                {
                                    std::string errorMsg = "Bad World ID \"" + std::to_string(worldId) +
                                                           "\"\nOnly " + std::to_string(worlds.size()) +
                                                           " worlds are being generated.";
                                    throw std::runtime_error(errorMsg);
                                }
                            }
                        }
                        // Otherwise treat the value as an item for the same world as the location
                        else
                        {
                            itemName = locationNode.second.as<std::string>();
                        }

                        const std::string locationName = locationNode.first.as<std::string>();
                        auto location = world->GetLocation(locationName);

                        auto& itemWorld = worlds.at(worldId - 1);
                        auto item = itemWorld->GetItem(itemName);

                        world->AddPlandomizedLocation(location, item);
                    }
                }

                if (worldNode["Entrances"])
                {
                    const auto& entrances = worldNode["Entrances"];
                    if (!entrances.IsMap())
                    {
                        if (ignoreErrors)
                        {
                            return;
                        }

                        throw std::runtime_error("Plandomizer file entrances for " + worldStr +
                                                 " is not a map. Please check your syntax before trying again.");
                    }

                    for (const auto& entranceNode : entrances)
                    {
                        auto entranceName = entranceNode.first.as<std::string>(); 
                        auto targetName = entranceNode.second.as<std::string>();

                        auto entrance = world->GetEntrance(entranceName);
                        auto target = world->GetEntrance(targetName);

                        world->AddPlandomizedEntrance(entrance, target);
                    }
                }
            }
        }
    }
} // namespace randomizer::logic::plandomizer
