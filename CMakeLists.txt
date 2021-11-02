cmake_minimum_required(VERSION 3.14)

project(latch C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

