cmake_minimum_required(VERSION 3.16)

project(
    zscript
    VERSION 0.0.1
    DESCRIPTION "Script for z of math"
    LANGUAGES CXX
)

option(ZSCRIPT_BUILD_TEST "Enable unit testing" ON)

include(FetchContent)

set(POT_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    ParallelOperationsToolkit
    GIT_REPOSITORY https://github.com/esufaev/Parallel-Operations-Toolkit.git
    GIT_TAG master
)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG devel
)

FetchContent_MakeAvailable(ParallelOperationsToolkit Catch2)

set(Z_HEADERS
    include/${PROJECT_NAME}/zAST/nodes.h
    include/${PROJECT_NAME}/zparser/zlexer.h
    include/${PROJECT_NAME}/zparser/zparser.h
    include/${PROJECT_NAME}/zparser/ztoken.h
    include/${PROJECT_NAME}/zutils/zmatrix.h
)

set(Z_FLAGS
    -std=c++23
    -mavx2
    -fsanitize=address
)

add_compile_options(${Z_FLAGS})
add_link_options(${Z_FLAGS})

add_library(${PROJECT_NAME}_lib INTERFACE ${Z_HEADERS})
target_include_directories(${PROJECT_NAME}_lib INTERFACE include)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
PRIVATE
${PROJECT_NAME}_lib
pot
)

if(ZSCRIPT_BUILD_TEST)
    enable_testing()
    include(CTest)
    include(Catch)

    set(TEST_SOURCES
        # ztests/znodes_test.cpp
        ztests/zparser_test.cpp
    )

    add_executable(tests ${TEST_SOURCES})
    target_link_libraries(tests
        PRIVATE
            ${PROJECT_NAME}_lib
            Catch2::Catch2WithMain 
    )

    catch_discover_tests(tests)
endif()