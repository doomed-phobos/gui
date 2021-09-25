function(sandbox_base name)
   add_executable(${name} "${name}.cpp")
   target_link_libraries(${name} base-lib)
endfunction()

sandbox_base(clamp)