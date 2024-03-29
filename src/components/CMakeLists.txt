cmake_minimum_required(VERSION 3.14)

project(components C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(COMPONENTS_SOURCES
    ForwardPin.hpp
    ComponentBase.hpp
    ComponentDescription.hpp
    ComponentFactory.hpp
    ComponentFactory.cpp
    GateComponents.hpp
    GateComponents.cpp
    IComponent.hpp
    Clock.hpp
    Clock.cpp
    Register.hpp
    Register.cpp
    ALU.hpp
    ALU.cpp
    Bus.hpp
    Bus.cpp
    ProgramCounter.hpp
    ProgramCounter.cpp
)

add_library(${PROJECT_NAME} SHARED ${COMPONENTS_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC gate sched logger nlohmann_json::nlohmann_json)
target_compile_options(${PROJECT_NAME} PUBLIC -ftemplate-depth=2900)
