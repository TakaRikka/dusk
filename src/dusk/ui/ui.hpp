#pragma once

#include <RmlUi/Core/Input.h>
#include <SDL3/SDL_events.h>

namespace dusk::ui {

enum class MenuAction {
    None,
    Accept,
    Back,
    Toggle,
    TabLeft,
    TabRight,
};

bool initialize();
void shutdown();

bool is_active();
void set_active(bool active);

void handle_event(const SDL_Event& event);
void update();

Rml::Input::KeyIdentifier key_for_menu_action(MenuAction action);
MenuAction menu_action_from_key(Rml::Input::KeyIdentifier key);

}  // namespace dusk::ui
