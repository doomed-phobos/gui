macro(add_test name ...)
   sandbox_executable(${name} ${ARGV})
   target_link_libraries(${name} Catch2WithMain)
endmacro()

add_test(base_shared_ptr LIBRARIES base-lib)
add_test(gfx_clip LIBRARIES gfx-lib)