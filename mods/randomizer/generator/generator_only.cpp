#include "randomizer.hpp"

int main() {
    randomizer::Randomizer rando{RANDO_SAVE_PATH};
    rando.Generate();

    return 0;
}