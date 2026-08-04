#pragma once

#include "hint_types.hpp"

#include <memory>
#include <vector>

namespace randomizer::logic::world {
class World;
using WorldPool = std::vector<std::unique_ptr<World>>;
}

namespace randomizer::logic::hints {

class HintGenerator {
public:
    HintGenerator(world::World* world);

    void GeneratePathHints();
    void GenerateBarrenHints();
    void GenerateItemHints();
    void GenerateRemoteLocationHints();
    void GenerateLocationHints();
    void DistributeHints();
    void FinalizeHintSignText();

private:
    world::World* _world{};
    std::vector<Hint> _pathHints{};
    std::vector<Hint> _barrenHints{};
    std::vector<Hint> _itemHints{};
    std::vector<Hint> _locationHints{};
};

void GenerateAllHints(world::WorldPool& worldPool);

}