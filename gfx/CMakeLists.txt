add_library(gfx-lib STATIC
   paint.h
   color.cpp
   mask_filter.cpp)

target_include_directories(gfx-lib PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(gfx-lib skia)