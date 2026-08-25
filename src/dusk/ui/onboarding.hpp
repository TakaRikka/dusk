#pragma once

// The screen shown when disc discovery finds nothing -- whether that is first run or a recovery
// after tvOS purged the data container. The app does not distinguish between those, because to the
// person in front of the TV they are the same situation.

#include "document.hpp"

namespace dusk::ui::onboarding {

// Starts the transfer server, shows its URL, tracks progress, and stops the server when dismissed.
void push(Document& host);

}  // namespace dusk::ui::onboarding
