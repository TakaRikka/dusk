#include "lang.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;

std::string _(const std::string& key) {
    static json langData;
    static bool isLoaded = false;
    if (!isLoaded) {
        std::ifstream file("res/rml/lang.json");
        if (file.is_open()) {
            file >> langData;
        }
    }
    try {
        std::string pointerKey = "/" + key;
        std::replace(pointerKey.begin(), pointerKey.end(), '.', '/');
        return langData.at(json::json_pointer(pointerKey));
    } catch (...) {
        return key;
    }
}
