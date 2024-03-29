cmake_minimum_required(VERSION 3.14)

project(components-test C CXX)

set(DEPS
    sched
    gate
    components
    gtest_main)

set(TEST_NAME component-base-test)
set(${TEST_NAME}_SOURCES ComponentBaseTests.cpp)
add_executable(${TEST_NAME} ${${TEST_NAME}_SOURCES})
target_link_libraries(${TEST_NAME} PUBLIC ${DEPS})
add_test(NAME ${TEST_NAME}_test
    COMMAND ${TEST_NAME})

set(TEST_NAME register-test)
set(${TEST_NAME}_SOURCES RegisterTests.cpp)
add_executable(${TEST_NAME} ${${TEST_NAME}_SOURCES})
target_link_libraries(${TEST_NAME} PUBLIC ${DEPS})
add_test(NAME ${TEST_NAME}_test
    COMMAND ${TEST_NAME})

set(TEST_NAME alu-test)
set(${TEST_NAME}_SOURCES
    ALUTests.hpp
    BinaryAdderTests.cpp
    ALUTests.cpp
)
add_executable(${TEST_NAME} ${${TEST_NAME}_SOURCES})
target_link_libraries(${TEST_NAME} PUBLIC ${DEPS})
target_include_directories(${TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME ${TEST_NAME}_test
    COMMAND ${TEST_NAME})

set(TEST_NAME bus-test)
set(${TEST_NAME}_SOURCES
    BusTests.cpp
)
add_executable(${TEST_NAME} ${${TEST_NAME}_SOURCES})
target_link_libraries(${TEST_NAME} PUBLIC ${DEPS})
add_test(NAME ${TEST_NAME}_test
    COMMAND ${TEST_NAME})

set(TEST_NAME pc-test)
set(${TEST_NAME}_SOURCES
    ProgramCounterTests.cpp
)
add_executable(${TEST_NAME} ${${TEST_NAME}_SOURCES})
target_link_libraries(${TEST_NAME} PUBLIC ${DEPS})
add_test(NAME ${TEST_NAME}_test
    COMMAND ${TEST_NAME})
