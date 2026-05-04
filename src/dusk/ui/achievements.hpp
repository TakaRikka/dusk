#pragma once

#include "dusk/achievements.h"
#include "window.hpp"

#include <vector>

namespace dusk::ui {

class AchievementsWindow : public Window {
public:
    AchievementsWindow();
    void update() override;

private:
    std::vector<Achievement> mSnapshot;
};

}  // namespace dusk::ui
