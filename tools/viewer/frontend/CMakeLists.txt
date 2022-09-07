cmake_minimum_required(VERSION 3.14)

project(frontend)

add_custom_target(
    VIEWER_TESTS ALL
    COMMAND ${NPM} install
    COMMAND DESCRIPTOR_PATH=${CMAKE_CURRENT_BINARY_DIR}/../../component_descriptors/component_descriptors.json ${NPM} run test
    DEPENDS component_descriptors_json
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
