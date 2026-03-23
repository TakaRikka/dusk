#ifndef DUSK_IMGUI_SAVEEDITOR_HPP
#define DUSK_IMGUI_SAVEEDITOR_HPP

#include <aurora/aurora.h>
#include <string>

#include "imgui.h"

namespace dusk {
	class ImGuiSaveEditor {
	public:
		ImGuiSaveEditor();
		
		void draw();
        void drawPlayerStatusTab();
        void drawLocationTab();
		void drawInventoryTab();
		void drawFlagsTab();
		void drawConfigTab();

	private:
		int m_selectedRegion = 0;
	};
}

#endif  // DUSK_IMGUI_SAVEEDITOR_HPP
