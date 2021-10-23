cmake_minimum_required(VERSION 3.14)

project(sched C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SCHED_SOURCES
    IScheduler.hpp
    Scheduler.cpp
    EventQueue.hpp
    EventQueue.cpp
    SimpleScheduler.hpp
    SimpleScheduler.cpp
)

add_library(${PROJECT_NAME} ${SCHED_SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC logger)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
