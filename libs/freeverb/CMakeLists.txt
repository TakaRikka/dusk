cmake_minimum_required(VERSION 3.10)

project(freeverb LANGUAGES CXX)

add_library(freeverb
    comb.cpp
    allpass.cpp
    revmodel.cpp
)
