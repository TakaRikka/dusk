cmake_minimum_required(VERSION 3.25)
project(dusklight_hostservices LANGUAGES C CXX)

include(CTest)
find_package(Threads REQUIRED)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(http_c_abi_compile OBJECT http_c_abi_compile.c)
target_include_directories(http_c_abi_compile PRIVATE ../../sdk/include)

add_executable(http_service_core_test
    http_service_core_test.cpp
    http_transport_fake.cpp
    ../../src/dusk/mods/svc/http_core.cpp
)
target_include_directories(http_service_core_test PRIVATE
    ../../sdk/include
    ../../src
    ../../extern/borealis/include
)
target_link_libraries(http_service_core_test PRIVATE Threads::Threads)

add_test(NAME http_service_core COMMAND http_service_core_test)
