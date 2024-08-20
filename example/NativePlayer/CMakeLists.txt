cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(NativePlayer VERSION 1.0.0 LANGUAGES CXX)

message(TRACE "========= Start configure: ${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


set(src
        main.cpp
)

add_executable(NativePlayer
        ${header_interface}
        ${src}
)

target_compile_options(NativePlayer PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall -Wextra -Wpedantic>)

target_link_libraries(NativePlayer
        PRIVATE
        ${OpenCV_LIBS}
        FrameGrabber::framegrabber

)

message(TRACE "========= Finish configure: ${PROJECT_NAME}")