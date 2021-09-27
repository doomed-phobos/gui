include(change_list_delimeter)

function(sandbox_executable name ...)
   add_executable(${name} "${name}.cpp")
   cmake_parse_arguments(GUI "" "" "LIBRARIES" ${ARGV})
   change_list_delimeter(GUI_LIBRARIES " ")
   target_link_libraries(${name} "${GUI_LIBRARIES}")
endfunction()