#ifndef DUSK_IMGUI_SAVEEDITOR_HPP
#define DUSK_IMGUI_SAVEEDITOR_HPP

#include <aurora/aurora.h>
#include <string>

#include "imgui.h"

namespace dusk {
	class ImGuiSaveEditor {
	public:
		ImGuiSaveEditor();
		
		void draw(bool& open);
        void drawPlayerStatusTab();
        void drawLocationTab();
		void drawInventoryTab();
		void drawCollectionTab();
		void drawFlagsTab();
        void drawMinigameTab();
		void drawConfigTab();

	private:
        struct {
            uint8_t id;
            const char* name;
        } m_selectedRegion = {0, nullptr};
	};
}

#endif  // DUSK_IMGUI_SAVEEDITOR_HPP
