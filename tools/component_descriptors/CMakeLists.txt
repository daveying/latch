cmake_minimum_required(VERSION 3.14)

project(component_descriptors C CXX)

set(SOURCES component_descriptors.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC
    sched
    gate
    components)

set(COMPONENT_DESCRIPTORS_JSON ${CMAKE_CURRENT_BINARY_DIR}/component_descriptors.json)
add_custom_command(
    OUTPUT ${COMPONENT_DESCRIPTORS_JSON}
    DEPENDS ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME} > ${COMPONENT_DESCRIPTORS_JSON}
)

add_custom_target(component_descriptors_json ALL
    DEPENDS ${COMPONENT_DESCRIPTORS_JSON}
)
