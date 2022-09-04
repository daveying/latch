cmake_minimum_required(VERSION 3.14)

project(latch C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(REQUIRE_BUILD_VIEWER "If building viewer is required." TRUE)

enable_testing()

add_subdirectory(3rdparty)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(tools)
