cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(libframegrabber VERSION 1.0.0 LANGUAGES CXX)

message(TRACE "========= Start configure: ${PROJECT_NAME}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(header_interface
        include/qframgrabber/Buffer.hpp
        include/qframgrabber/Types.hpp
        include/qframgrabber/QFrameProvider.hpp
)

set(src
        src/FrameGrabberImpl.cpp
        src/QFrameProvider.cpp
)


add_library(framegrabber
        ${header_interface}
        ${src}
)

add_library(FrameGrabber::framegrabber ALIAS framegrabber)

target_include_directories(framegrabber
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(framegrabber PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -Wall -Wextra -Wpedantic>)

target_link_libraries(framegrabber PRIVATE
        ${OpenCV_LIBS}
        Qt${QT_VERSION_MAJOR}::Core
)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/FrameGrabber)

install(TARGETS framegrabber
        EXPORT framegrabber-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(framegrabber PROPERTIES EXPORT_NAME FrameGrabber)


message(TRACE "========= Finish configure: ${PROJECT_NAME}")