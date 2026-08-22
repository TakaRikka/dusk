#pragma once

#include "dusk/disc_discovery_rules.hpp"

#include <vector>

namespace dusk::disc_discovery {

// True on platforms without a file dialog (tvOS): the disc must be found by scanning.
bool needed() noexcept;

// Candidates in priority order: <app bundle>/disc/*, <data dir>/discs/*, <data dir>/* (disc files
// only).
std::vector<Candidate> scan();

}  // namespace dusk::disc_discovery
