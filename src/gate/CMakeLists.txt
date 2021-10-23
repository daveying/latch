cmake_minimum_required(VERSION 3.14)

project(gate C CXX)

set(GATE_SOURCES
    IPin.hpp
    ZeroDelayPin.hpp
    IGate.hpp
    GateTemplate.hpp
    TruthTable.hpp
    ORGate.hpp
    NORGate.hpp
    ANDGate.hpp
    NANDGate.hpp
)

add_library(${PROJECT_NAME} ${GATE_SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC sched logger)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
