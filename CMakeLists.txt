cmake_minimum_required(VERSION 3.10)


project(bridgeport CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


option(BRIDGEPORT_BUILD_TESTS "Build bridgeport unit tests." OFF)


add_library(bridgeport)
target_include_directories(bridgeport
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    )
target_sources(bridgeport
    PUBLIC ${PROJECT_SOURCE_DIR}/src/glfw_guards.cpp
    )
target_compile_features(bridgeport
    PUBLIC cxx_std_17
    )
