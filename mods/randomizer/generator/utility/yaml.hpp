#pragma once

#include "yaml-cpp/yaml.h"

#include "../utility/file.hpp"
#include "../utility/path.hpp"
#include "battery/embed.hpp"

#define GET_EMBED_DATA(path) b::embed<path>().str()
#define LOAD_EMBED_DATA(data) YAML::Load(data)

#define LOAD_EMBED_YAML(path) LOAD_EMBED_DATA(GET_EMBED_DATA(path))

// this wrapper is here to avoid path encoding issues
// removes any possible path -> string oddities or the need to open the file manually
inline YAML::Node LoadYAML(const fspath& path, const bool& resourceFile = false) {
    std::string file;
    if (randomizer::utility::file::GetContents(path, file, resourceFile) != 0) {
        throw YAML::BadFile(
            path.string());  // exception is bad (unhandled) but it matches the old behavior
    }

    return YAML::Load(file);
}

template <class... Fields>
void YAMLVerifyFields(const YAML::Node& node, Fields... requiredFields) {
    for (const auto& field : {requiredFields...}) {
        if (!node[field]) {
            throw std::runtime_error(std::string("Field \"") + field +
                                     "\" is missing from node:\n" + YAML::Dump(node));
        }
    }
}
