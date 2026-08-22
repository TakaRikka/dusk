cmake_minimum_required(VERSION 3.25)
project(ProvisionSeed LANGUAGES C)
# Minimal tvOS app used only to make Xcode create a development provisioning profile
# for ${SEED_BUNDLE_ID} on team ${SEED_TEAM_ID} and register the target Apple TV.
set(SEED_BUNDLE_ID "" CACHE STRING "bundle id to provision")
set(SEED_TEAM_ID "" CACHE STRING "team id")
add_executable(ProvisionSeed MACOSX_BUNDLE main.c)
set_target_properties(ProvisionSeed PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER "${SEED_BUNDLE_ID}"
    MACOSX_BUNDLE_BUNDLE_NAME "ProvisionSeed"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${SEED_BUNDLE_ID}"
    XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "${SEED_TEAM_ID}"
    XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development"
    XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "3"
    XCODE_ATTRIBUTE_SDKROOT "appletvos"
    XCODE_ATTRIBUTE_TVOS_DEPLOYMENT_TARGET "14.5")
