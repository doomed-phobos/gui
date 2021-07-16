include(add_executable)

add_library(os-lib STATIC
   # Skia
   skia/skia_convert.h
   skia/skia_surface.cpp

   win/win_keys.cpp
   style.cpp
   styles.cpp
   display.cpp
   system.cpp)

target_include_directories(os-lib PUBLIC ${CMAKE_SOURCE_DIR})
target_compile_definitions(os-lib PUBLIC -DNOMINMAX)
target_link_libraries(os-lib base-lib skia)

__add_executable(os-test main.cpp)
target_link_libraries(os-test os-lib gfx-lib)