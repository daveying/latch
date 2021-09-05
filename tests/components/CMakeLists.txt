cmake_minimum_required(VERSION 3.14)

project(components C CXX)

set(SOURCES
    NORGate/NORGate.cpp
    NANDGate/NANDGate.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC gate gtest_main)
