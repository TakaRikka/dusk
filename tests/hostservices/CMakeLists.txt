cmake_minimum_required(VERSION 3.25)
project(dusklight_hostservices LANGUAGES C CXX)

include(CTest)
find_package(Threads REQUIRED)
find_package(Java 17 COMPONENTS Development REQUIRED)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(http_c_abi_compile OBJECT http_c_abi_compile.c)
target_include_directories(http_c_abi_compile PRIVATE ../../sdk/include)

add_executable(http_service_core_test
    http_service_core_test.cpp
    http_transport_fake.cpp
    ../../src/dusk/mods/svc/http_core.cpp
)
target_include_directories(http_service_core_test PRIVATE ../../sdk/include ../../src)
target_link_libraries(http_service_core_test PRIVATE Threads::Threads)

add_test(NAME http_service_core COMMAND http_service_core_test)

set(HTTP_JAVA_CLASSES "${CMAKE_CURRENT_BINARY_DIR}/java-classes")
add_custom_command(
    OUTPUT "${HTTP_JAVA_CLASSES}/.compiled"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${HTTP_JAVA_CLASSES}"
    COMMAND "${Java_JAVAC_EXECUTABLE}" -d "${HTTP_JAVA_CLASSES}"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../platforms/android/app/src/main/java/com/twilitrealm/dusk/DuskHttpClient.java"
        "${CMAKE_CURRENT_SOURCE_DIR}/java/DuskHttpClientTest.java"
    COMMAND "${CMAKE_COMMAND}" -E touch "${HTTP_JAVA_CLASSES}/.compiled"
    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/../../platforms/android/app/src/main/java/com/twilitrealm/dusk/DuskHttpClient.java"
        "${CMAKE_CURRENT_SOURCE_DIR}/java/DuskHttpClientTest.java"
)
add_custom_target(http_java_test_classes ALL DEPENDS "${HTTP_JAVA_CLASSES}/.compiled")
add_test(NAME http_java_transport
    COMMAND "${Java_JAVA_EXECUTABLE}" -ea -cp "${HTTP_JAVA_CLASSES}"
        dev.twilitrealm.dusk.DuskHttpClientTest)
