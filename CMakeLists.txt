cmake_minimum_required(VERSION 3.10)

project(latch C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GATE_SOURCES
    IPin.hpp
    ZeroDelayPin.hpp
    IGate.hpp
    GateTemplate.hpp
    TruthTable.hpp
    NORGate.hpp 
)

add_executable(${PROJECT_NAME}
    main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})