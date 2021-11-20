cmake_minimum_required(VERSION 3.14)

project(components C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EVENT_COMPONENTS_SOURCES
    ForwardPin.hpp
    ComponentBase.hpp
    ComponentDescription.hpp
    ComponentFactory.hpp
    ComponentFactory.cpp
    GateComponents.hpp
    IComponent.hpp
    Timer.hpp
    Timer.cpp
    Register.hpp
    # Register.cpp
)

add_library(${PROJECT_NAME} SHARED ${EVENT_COMPONENTS_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC gate sched logger)
