include(sandbox_executable.cmake)
sandbox_executable(base LIBRARIES base-lib)
sandbox_executable(gfx LIBRARIES base-lib)
if(MSVC)
   sandbox_executable(leaks LIBRARIES base-lib)
endif()

add_subdirectory(tests)