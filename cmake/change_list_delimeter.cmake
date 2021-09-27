function(change_list_delimeter output delimeter)
   string(REPLACE ";" "${delimeter}" OUTPUT_STR "${${output}}")
   set("${output}" "${OUTPUT_STR}" PARENT_SCOPE)
endfunction()
