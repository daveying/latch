cmake_minimum_required(VERSION 3.14)

project(clock-test C CXX)

set(SOURCES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC
    sched
    gate
    components
    gtest_main)

add_test(NAME ${PROJECT_NAME}_test
    COMMAND ${PROJECT_NAME})
