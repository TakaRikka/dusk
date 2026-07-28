#pragma once

// Forward Declarations
namespace randomizer
{
    class Randomizer;
}

namespace randomizer::logic::spoiler_log
{
    void GenerateSpoilerLog(Randomizer* randomizer);
    void GenerateAntiSpoilerLog(Randomizer* randomizer);
} // namespace randomizer::logic::spoiler_log
