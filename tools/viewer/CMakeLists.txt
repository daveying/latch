cmake_minimum_required(VERSION 3.14)

project(viewer)

find_program(NPM npm)
find_program(NODEJS node)

# if any required tool is not installed, skip this project.
# if REQUIRE_BUILLD_VIEWER is set, then it's a fatal error.
if ((NOT NPM) OR (NOT NODEJS))
    if (REQUIRE_BUILD_VIEWER)
        message(FATAL_ERROR "Required program (npm / node) cannot be found.\nNPM: ${NPM}\nNODEJS: ${NODEJS}")
    else()
        message(WARNING "NODEJS or NPM cannot found, viewer is skipped\nNPM: ${NPM}\nNODEJS: ${NODEJS}\n")
    endif()
    return()
endif()

message(STATUS "Uses NPM: ${NPM}")
message(STATUS "Uses NODEJS: ${NODEJS}")

add_subdirectory(frontend)
