# ==================
# fmt
# ==================
add_subdirectory(fmt)

# ==================
# spdlog
# ==================
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Use external fmt library instead of bundled")
add_subdirectory(spdlog)

# ==================
# catch2
# ==================
if(GUI_WITH_SANDBOX)
   add_subdirectory(catch2)
endif()