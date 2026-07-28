#pragma once

#include <filesystem>
#include <memory>
#include <vector>

using fspath = std::filesystem::path;

// Forward Declarations
namespace randomizer::logic::world
{
    class World;
    using WorldPool = std::vector<std::unique_ptr<World>>;
} // namespace randomizer::logic::world

namespace randomizer::logic::plandomizer
{
    void LoadPlandomizerData(world::WorldPool& worlds, const fspath& filepath, const bool& ignoreErrors = false);
}
