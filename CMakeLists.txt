cmake_minimum_required(VERSION 3.16)

project(
    zscript
    VERSION 0.0.1
    DESCRIPTION "Script for z of math"
    LANGUAGES CXX
)

include(FetchContent)

FetchContent_Declare(
    ParallelOperationsToolkit
    GIT_REPOSITORY https://github.com/esufaev/Parallel-Operations-Toolkit.git
    GIT_TAG master
)

FetchContent_MakeAvailable(ParallelOperationsToolkit)

set(Z_HEADERS
    include/${PROJECT_NAME}/AST/nodes.h
)

add_library(${PROJECT_NAME}_lib INTERFACE)
target_include_directories(${PROJECT_NAME}_lib INTERFACE include)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_NAME}_lib
        ParallelOperationsToolkit
)

add_executable(${PROJECT_NAME} main.cpp)
