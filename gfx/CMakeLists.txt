add_library(gfx-lib INTERFACE)
target_include_directories(gfx-lib INTERFACE ${CMAKE_SOURCE_DIR})
target_link_libraries(gfx-lib INTERFACE skia)