#pragma once

#include "../utility/text.hpp"

#include <string>
#include <variant>

namespace randomizer::logic::location {
class Location;
}

namespace randomizer::logic::hints {

struct PathHint {
    location::Location* goalLocation;
    location::Location* hintedLocation;
    bool operator==(const PathHint&) const = default;
};

struct BarrenHint {
    std::string region;
    bool operator==(const BarrenHint&) const = default;
};

struct ItemHint {
    location::Location* location;
    bool operator==(const ItemHint&) const = default;
};

struct LocationHint {
    location::Location* location;
    bool operator==(const LocationHint&) const = default;
};

using HintData = std::variant<std::monostate, PathHint, BarrenHint, ItemHint, LocationHint>;
struct Hint {
    Text text{};
    HintData data;

    bool operator==(const Hint&) const = default;
};
}
