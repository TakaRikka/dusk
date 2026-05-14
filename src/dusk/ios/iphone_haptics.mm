#include "dusk/iphone_haptics.hpp"

#import <CoreHaptics/CoreHaptics.h>
#import <UIKit/UIKit.h>

#include <algorithm>
#include <cmath>

namespace dusk::iphone_haptics {
namespace {

constexpr NSTimeInterval kContinuousDuration = 30.0;

CHHapticEngine* sEngine = nil;
id<CHHapticAdvancedPatternPlayer> sPlayer = nil;
float sCurrentIntensity = -1.0f;

bool isIphone() {
    return UIDevice.currentDevice.userInterfaceIdiom == UIUserInterfaceIdiomPhone;
}

bool hardwareSupportsHaptics() {
    if (@available(iOS 13.0, *)) {
        return [CHHapticEngine capabilitiesForHardware].supportsHaptics;
    }
    return false;
}

bool ensureEngine() {
    if (@available(iOS 13.0, *)) {
        if (sEngine != nil) {
            return true;
        }

        NSError* error = nil;
        sEngine = [[CHHapticEngine alloc] initAndReturnError:&error];
        if (sEngine == nil || error != nil) {
            sEngine = nil;
            return false;
        }

        sEngine.stoppedHandler = ^(CHHapticEngineStoppedReason) {
            sPlayer = nil;
            sEngine = nil;
            sCurrentIntensity = -1.0f;
        };
        sEngine.resetHandler = ^{
            NSError* startError = nil;
            [sEngine startAndReturnError:&startError];
            if (startError != nil) {
                sPlayer = nil;
                sEngine = nil;
                sCurrentIntensity = -1.0f;
            }
        };

        if (![sEngine startAndReturnError:&error] || error != nil) {
            sPlayer = nil;
            sEngine = nil;
            return false;
        }

        return true;
    }
    return false;
}

bool updateIntensity(float intensity) {
    if (@available(iOS 13.0, *)) {
        if (sPlayer == nil || std::abs(sCurrentIntensity - intensity) < 0.01f) {
            return sPlayer != nil;
        }

        CHHapticDynamicParameter* parameter =
            [[CHHapticDynamicParameter alloc] initWithParameterID:CHHapticDynamicParameterIDHapticIntensityControl
                                                           value:intensity
                                                    relativeTime:0.0];
        NSError* error = nil;
        [sPlayer sendParameters:@[ parameter ] atTime:CHHapticTimeImmediate error:&error];
        if (error == nil) {
            sCurrentIntensity = intensity;
            return true;
        }
    }
    return false;
}

}  // namespace

bool isAvailable() {
    return isIphone() && hardwareSupportsHaptics();
}

void start(float intensity) {
    @autoreleasepool {
        if (!isAvailable()) {
            return;
        }

        intensity = std::clamp(intensity, 0.0f, 1.0f);
        if (updateIntensity(intensity)) {
            return;
        }

        if (@available(iOS 13.0, *)) {
            if (!ensureEngine()) {
                return;
            }

            CHHapticEventParameter* intensityParameter =
                [[CHHapticEventParameter alloc] initWithParameterID:CHHapticEventParameterIDHapticIntensity
                                                             value:intensity];
            CHHapticEventParameter* sharpnessParameter =
                [[CHHapticEventParameter alloc] initWithParameterID:CHHapticEventParameterIDHapticSharpness
                                                             value:0.45f];
            CHHapticEvent* event = [[CHHapticEvent alloc] initWithEventType:CHHapticEventTypeHapticContinuous
                                                                 parameters:@[ intensityParameter, sharpnessParameter ]
                                                               relativeTime:0.0
                                                                   duration:kContinuousDuration];

            NSError* error = nil;
            CHHapticPattern* pattern = [[CHHapticPattern alloc] initWithEvents:@[ event ]
                                                                    parameters:@[]
                                                                         error:&error];
            if (pattern == nil || error != nil) {
                return;
            }

            sPlayer = [sEngine createAdvancedPlayerWithPattern:pattern error:&error];
            if (sPlayer == nil || error != nil) {
                sPlayer = nil;
                return;
            }

            if (![sPlayer startAtTime:CHHapticTimeImmediate error:&error] || error != nil) {
                sPlayer = nil;
                return;
            }

            sCurrentIntensity = intensity;
        }
    }
}

void stop() {
    @autoreleasepool {
        if (@available(iOS 13.0, *)) {
            if (sPlayer == nil) {
                return;
            }

            NSError* error = nil;
            [sPlayer stopAtTime:CHHapticTimeImmediate error:&error];
            sPlayer = nil;
            sCurrentIntensity = -1.0f;
        }
    }
}

}  // namespace dusk::iphone_haptics
