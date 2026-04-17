#include "FileSelectDialog.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>
#import <objc/runtime.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

static void *g_picker_delegate_key = &g_picker_delegate_key;

static void RunOnMainThread(void (^block)(void))
{
    if ([NSThread isMainThread]) {
        block();
    } else {
        dispatch_sync(dispatch_get_main_queue(), block);
    }
}

static NSError *MakeError(NSString *message)
{
    return [NSError errorWithDomain:@"org.twilitrealm.dusk.file-select"
                               code:1
                           userInfo:@{NSLocalizedDescriptionKey: message}];
}

static UIViewController *FindTopViewController(UIViewController *controller)
{
    UIViewController *current = controller;
    while (current.presentedViewController != nil) {
        current = current.presentedViewController;
    }
    return current;
}

static UIViewController *PresenterFromWindow(SDL_Window *window)
{
    if (window == nil) {
        return nil;
    }

    const SDL_PropertiesID props = SDL_GetWindowProperties(window);
    if (props == 0) {
        return nil;
    }

    UIWindow *uiwindow = (__bridge UIWindow *)SDL_GetPointerProperty(
        props, SDL_PROP_WINDOW_UIKIT_WINDOW_POINTER, NULL);
    if (uiwindow == nil || uiwindow.rootViewController == nil) {
        return nil;
    }

    return FindTopViewController(uiwindow.rootViewController);
}

static NSURL *InitialDirectoryURL(const char *default_location)
{
    if (default_location == NULL || *default_location == '\0') {
        return nil;
    }

    NSString *path = [NSString stringWithUTF8String:default_location];
    NSURL *url = [NSURL fileURLWithPath:path];
    if ([path hasSuffix:@"/"]) {
        return url;
    }

    return [url URLByDeletingLastPathComponent];
}

@interface DocumentPickerDelegate : NSObject <UIDocumentPickerDelegate>

@property(nonatomic, assign) IOSFileCallback callback;
@property(nonatomic, assign) void *userdata;

@end

@implementation DocumentPickerDelegate

- (void)finishWithPath:(const char *)path error:(const char *)error {
    if (self.callback != NULL) {
        self.callback(self.userdata, path, error);
    }
}

- (void)documentPicker:(UIDocumentPickerViewController *)controller
didPickDocumentsAtURLs:(NSArray<NSURL *> *)urls
{
    NSURL *url = urls.firstObject;
    if (url == nil) {
        [self finishWithPath:NULL error:NULL];
        return;
    }

    [self finishWithPath:url.path.UTF8String error:NULL];
    (void)controller;
}

- (void)documentPickerWasCancelled:(UIDocumentPickerViewController *)controller
{
    [self finishWithPath:NULL error:NULL];
    (void)controller;
}

@end

void Dusk_iOS_ShowFileSelect(IOSFileCallback callback, void *userdata,
                               SDL_Window *window,
                               const SDL_DialogFileFilter *filters, int nfilters,
                               const char *default_location,
                               bool allow_many)
{
    RunOnMainThread(^{
        @autoreleasepool {
            UIViewController *presenter = PresenterFromWindow(window);
            if (presenter == nil) {
                callback(userdata, NULL, "Failed to find an iOS view controller for the file picker.");
                return;
            }

            NSLog(@"[ShowFileSelect] presenting picker from %@", NSStringFromClass([presenter class]));

            UIDocumentPickerViewController *picker =
                [[UIDocumentPickerViewController alloc]
                    initForOpeningContentTypes:@[ UTTypeItem ]
                                         asCopy:YES];
            picker.allowsMultipleSelection = allow_many ? YES : NO;
            picker.shouldShowFileExtensions = YES;

            NSURL *directory_url = InitialDirectoryURL(default_location);
            if (directory_url != nil) {
                picker.directoryURL = directory_url;
            }

            DocumentPickerDelegate *delegate = [DocumentPickerDelegate new];
            delegate.callback = callback;
            delegate.userdata = userdata;
            picker.delegate = delegate;
            objc_setAssociatedObject(picker, g_picker_delegate_key, delegate,
                                     OBJC_ASSOCIATION_RETAIN_NONATOMIC);

            [presenter presentViewController:picker animated:YES completion:nil];
            (void)filters;
            (void)nfilters;
        }
    });
}
