set(TARGET advanced_macros_win)

set(HEADERS)

set(SOURCES
    src/main.cpp)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

# Note: most of it is going to be Windows specific anyway, so let's not bother with indirections
target_link_options(${TARGET} PRIVATE "/SUBSYSTEM:WINDOWS" "/DELAYLOAD:$<TARGET_FILE_NAME:windows_hook>")

target_link_libraries(${TARGET}
    macro_player
    windows_hook
    nlohmann_json)

# Move binaries to a single location, and tell VS debugger about it
set_target_properties(${TARGET} PROPERTIES
    OUTPUT_NAME "advanced-macros"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
