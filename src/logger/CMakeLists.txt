cmake_minimum_required(VERSION 3.14)

project(logger C CXX)

set(SOURCES
    Logger.hpp
    Logger.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
