cmake_minimum_required(VERSION 3.14)

project(viewer)

find_program(NPM npm)
find_program(NODEJS node)

if (NPM AND NODEJS)
    message(STATUS "Use NPM: ${NPM}")
    message(STATUS "Use NODEJS: ${NODEJS}")
else()
    if (REQUIRE_BUILD_VIEWER)
        message(FATAL_ERROR "Required program (npm / node) cannot be found.\nNPM: ${NPM}\nNODEJS: ${NODEJS}")
    else()
        message(WARNING "NODEJS or NPM cannot found, viewer is skipped\nNPM: ${NPM}\nNODEJS: ${NODEJS}\n")
    endif()
endif()
