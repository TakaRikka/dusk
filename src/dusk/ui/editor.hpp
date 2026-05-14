#pragma once

#include "window.hpp"

struct MapEntry;

namespace dusk::ui {

class Pane;

Rml::String stage_option_label(const MapEntry& map, bool showInternalNames = false);
Rml::String stage_label_for_file(const Rml::String& stageFile, bool showInternalNames = false);
void populate_stage_picker(Pane& pane, std::function<Rml::String()> getStageFile,
                           std::function<void(const char*)> setStageFile,
                           bool showInternalNames = false);

class EditorWindow : public Window {
public:
    EditorWindow();
};

}  // namespace dusk::ui
