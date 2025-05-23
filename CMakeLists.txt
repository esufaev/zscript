cmake_minimum_required(VERSION 3.16)

project(
    zscript
    VERSION 0.0.1
    DESCRIPTION "Script for z of math"
    LANGUAGES CXX
)

include(FetchContent)
set(POT_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    ParallelOperationsToolkit
    GIT_REPOSITORY https://github.com/esufaev/Parallel-Operations-Toolkit.git
    GIT_TAG master
)

FetchContent_MakeAvailable(ParallelOperationsToolkit)

set(Z_HEADERS
    include/${PROJECT_NAME}/zAST/nodes.h
)

add_library(${PROJECT_NAME}_lib INTERFACE ${Z_HEADERS})
target_include_directories(${PROJECT_NAME}_lib INTERFACE include)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_NAME}_lib
        pot
)