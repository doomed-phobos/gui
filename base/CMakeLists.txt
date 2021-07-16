add_library(base-lib STATIC
   referenceable.h
   shared_ptr.h
   string.cpp
   thread.cpp
   debug.cpp
   exception.cpp
   mutex.cpp)

target_include_directories(base-lib PUBLIC ${CMAKE_SOURCE_DIR})