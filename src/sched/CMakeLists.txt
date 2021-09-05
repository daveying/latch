cmake_minimum_required(VERSION 3.10)

project(sched C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SCHED_SOURCES
    IScheduler.hpp
    Scheduler.cpp
)

add_library(sched ${SCHED_SOURCES})
target_include_directories(sched PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
