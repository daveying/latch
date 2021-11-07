cmake_minimum_required(VERSION 3.14)

project(gate-tests C CXX)

set(SOURCES
    NORGate/NORGate.cpp
    NANDGate/NANDGate.cpp
    ORGate/ORGate.cpp
    ANDGate/ANDGate.cpp
    NOTGate/NOTGate.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC gate gtest_main)

add_test(NAME ${PROJECT_NAME}_test
    COMMAND ${PROJECT_NAME})
