#pragma once

#include "config.hpp"

namespace randomizer::seedgen::seed
{
    /**
     *  @brief Generates a random sequence of 3 words to be used as a seed.
     *
     *  @return The sequence of words as a string
     */
    std::string GenerateSeed();

    /**
     *  @brief Generates a random sequence of 3 nouns to be used as a verification hash.
     *
     *  @return The sequence of words as a string
     */
    std::string GenerateHash();

    std::string HashForConfig(const config::Config& config);
} // namespace randomizer::seedgen::seed
