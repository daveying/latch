cmake_minimum_required(VERSION 3.14)

project(latch C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(REQUIRE_BUILD_VIEWER "If building viewer is required." TRUE)
option(
    DEEP_TEMPLATE_RECURSIVE_BUILD
    "If building the components require deep template recursive instantiation."
    TRUE
)
if (DEEP_TEMPLATE_RECURSIVE_BUILD)
    message(
        WARNING
        "Build the components require deep template recursive instantiation."
        "Large memory is needed for compiler."
    )
    add_compile_definitions(DEEP_TEMPLATE_RECURSIVE_BUILD)
endif()

enable_testing()

add_subdirectory(3rdparty)
add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(tests)
