include(sandbox_executable.cmake)
sandbox_executable(base LIBRARIES base-lib)
sandbox_executable(log LIBRARIES base-lib)
sandbox_executable(gfx_io LIBRARIES os-lib)
if(MSVC)
   sandbox_executable(leaks LIBRARIES base-lib)
endif()
sandbox_executable(tool_window LIBRARIES os-lib)
sandbox_executable(simple_window LIBRARIES os-lib)
sandbox_executable(hello_world LIBRARIES os-lib)
sandbox_executable(panviewport LIBRARIES os-lib)
sandbox_executable(allevents LIBRARIES os-lib)

add_subdirectory(tests)