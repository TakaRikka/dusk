#pragma once

#include "dusk/autosplit.h"
#include "window.hpp"

#include <string>
#include <vector>

namespace dusk::ui {

class AutosplitsWindow : public Window {
public:
    AutosplitsWindow();
    void update() override;

private:
    void snapshot();

    std::vector<autosplit::MappedSplit> mSnapshot;
    std::string mSnapshotPath;
};

}  // namespace dusk::ui
