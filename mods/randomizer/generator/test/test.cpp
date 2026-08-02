#include "../randomizer.hpp"
#include "../utility/string.hpp"

#include <filesystem>
#include <iostream>

int main()
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(RANDO_LOGIC_TESTS_PATH))
    {
        if (entry.path().generic_string().ends_with("settings.yaml"))
        {
            auto pathFolders = randomizer::utility::str::Split(entry.path().generic_string(), '/');
            auto& testName = pathFolders[pathFolders.size() - 2];
            std::filesystem::remove(SETTINGS_PATH);
            std::filesystem::copy_file(entry, SETTINGS_PATH);

            std::cout << "Testing " << testName << std::endl;

            try {
                randomizer::Randomizer r{RANDO_SAVE_PATH};
                r.GenerateWorlds();
            }
            catch(const std::exception& e) {
                std::cout << "Test \"" << testName << "\" failed! Failed settings saved to " << SETTINGS_PATH << std::endl;
                std::cout << "Error Message: " << e.what() << std::endl;
                return 1;
            }

            std::filesystem::remove(SETTINGS_PATH);
        }
    }
    // Remove test preferences
    std::filesystem::remove(PREFERENCES_PATH);

    std::cout << "All Settings Tests passed" << std::endl;

    return 0;
}
