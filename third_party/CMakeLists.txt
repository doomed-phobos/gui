# ==================
# fmt
# ==================
add_subdirectory(fmt)

# ==================
# catch2
# ==================
if(GUI_WITH_SANDBOX)
   add_subdirectory(catch2)
endif()