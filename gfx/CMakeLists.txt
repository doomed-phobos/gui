add_library(gfx-lib STATIC
   region.cpp)

target_include_directories(gfx-lib PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(gfx-lib skia)