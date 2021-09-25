add_library(os-lib STATIC
   # Skia
   skia/skia_convert.hpp

   event.hpp
   fwd.hpp
   window.hpp
   surface.cpp
   window_base.cpp
   system.cpp
   win/win_convert.hpp
   win/win_window.cpp
   win/win_keys.cpp)

set(GUI_OS_PLATFORM_LIBS)

if(WIN32)
   list(APPEND GUI_OS_PLATFORM_LIBS comctl32)
endif()

target_include_directories(os-lib PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(os-lib skia gfx-lib base-lib ${GUI_OS_PLATFORM_LIBS})