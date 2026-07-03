#pragma once

// Only include this header on macOS
#if defined(__APPLE__) && !TARGET_OS_IOS && !TARGET_OS_TV && !TARGET_OS_MACCATALYST

namespace dusk {

// App menu actions (e.g., "Open Data Folder..." in the app name menu)
void InstallMacOSAppMenuActions();

}  // namespace dusk

#endif
