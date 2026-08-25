cmake_minimum_required(VERSION 3.25)
project(dusklight_transfer_tests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

add_executable(http_parse_test
    http_parse_test.cpp
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src/dusk/transfer/http_parse.cpp")
target_include_directories(http_parse_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../src")
add_test(NAME http_parse_test COMMAND http_parse_test)
