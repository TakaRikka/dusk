cmake_minimum_required(VERSION 3.10)

project(freeverb LANGUAGES CXX)

add_library(freeverb
    comb.cpp
    allpass.cpp
    revmodel.cpp
)
target_include_directories(freeverb PRIVATE include/freeverb)
target_include_directories(freeverb INTERFACE include)
