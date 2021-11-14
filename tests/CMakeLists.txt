cmake_minimum_required(VERSION 3.14)

include(FetchContent)
FetchContent_Declare(
   googletest
   URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
   )
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(gate)
add_subdirectory(sr-latch)
add_subdirectory(d-latch)
add_subdirectory(d-flip-flop)
add_subdirectory(edge-detector)
add_subdirectory(one-bit)
add_subdirectory(timer)
add_subdirectory(component-factory)
