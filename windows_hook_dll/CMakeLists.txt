set(TARGET windows_hook)
add_library(${TARGET} SHARED include/hook.hpp hook.cpp)
target_include_directories(${TARGET} PUBLIC include)

# Move binaries to a single location
set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
