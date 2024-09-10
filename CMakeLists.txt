cmake_minimum_required(VERSION 3.16)
project(
        QFrameGrabber
        VERSION 1.0
        LANGUAGES CXX)

option(ENABLE_TEST "enable testing via gtest" OFF)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (UNIX)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

#list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake/Modules)

add_subdirectory(lib)

add_subdirectory(depends/sharedmat)

add_subdirectory(example)




  
