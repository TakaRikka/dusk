#include "macos_menu.hpp"

#include "dusk/data.hpp"

#import <AppKit/AppKit.h>

@interface DuskAppMenuTarget : NSObject
- (void)openDataFolder:(id)sender;
@end

@implementation DuskAppMenuTarget
- (void)openDataFolder:(id)sender {
  (void)sender;
  dusk::data::open_data_path();
}
@end

namespace dusk {
namespace {

DuskAppMenuTarget *shared_app_menu_target() {
  static DuskAppMenuTarget *target = [DuskAppMenuTarget new];
  return target;
}

} // namespace

void InstallMacOSAppMenuActions() {
  NSMenu *mainMenu = [NSApp mainMenu];
  if (mainMenu == nil || [mainMenu numberOfItems] == 0) {
    return;
  }

  NSMenu *appMenu = [[mainMenu itemAtIndex:0] submenu];
  if (appMenu == nil) {
    return;
  }

  if ([appMenu itemWithTitle:@"Open Data Folder..."] != nil) {
    return;
  }

  NSUInteger insertIndex = [appMenu numberOfItems];

  for (NSMenuItem *existingItem in [appMenu itemArray]) {
    if ([existingItem action] == @selector(terminate:)) {
      insertIndex = [appMenu indexOfItem:existingItem];
      break;
    }
  }

  NSMenuItem *item =
      [[NSMenuItem alloc] initWithTitle:@"Open Data Folder..."
                                 action:@selector(openDataFolder:)
                          keyEquivalent:@""];

  [item setTarget:shared_app_menu_target()];
  [appMenu insertItem:item atIndex:insertIndex];
}
} // namespace dusk
