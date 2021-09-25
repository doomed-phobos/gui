cmake_minimum_required(VERSION 3.2)

project(gui CXX C)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(MSVC)
   set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
endif()

include(FindSkia)

add_subdirectory(third_party)
add_subdirectory(base)
add_subdirectory(gfx)
add_subdirectory(os)