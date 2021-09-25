include(CheckIncludeFiles)
include(CheckFunctionExists)
check_include_files(stdint.h HAVE_STDINT_H)
check_function_exists(sched_yield HAVE_SCHED_YIELD)

configure_file(config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp @ONLY)

add_library(base-lib STATIC
   base.hpp
   clamp.hpp
   debug-inl.hpp
   log-inl.hpp
   files.cpp
   referenceable.hpp
   shared_ptr.hpp
   signal.hpp
   slot.hpp
   string-inl.hpp
   exception.cpp
   mutex.cpp
   string.cpp
   thread.cpp
   process.cpp)

target_include_directories(base-lib PUBLIC ${CMAKE_SOURCE_DIR})
target_precompile_headers(base-lib PUBLIC ${CMAKE_SOURCE_DIR}/core.hpp)
target_link_libraries(base-lib spdlog fmt)

add_executable(base-test test.cpp)
target_link_libraries(base-test base-lib)