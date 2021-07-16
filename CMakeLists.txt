cmake_minimum_required(VERSION 3.2)

project(gui CXX C)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
configure_file(${PROJECT_SOURCE_DIR}/build_and_run.py.in ${PROJECT_BINARY_DIR}/build_and_run.py)

set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")

include(FindSkia)

add_subdirectory(os)
add_subdirectory(base)
add_subdirectory(gfx)