set(TARGET advanced_macros_win)

set(HEADERS)

set(SOURCES
    src/main.cpp)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

# Note: most of it is going to be Windows specific anyway, so let's not bother with indirections
target_link_options(${TARGET} PRIVATE "/SUBSYSTEM:WINDOWS" "/DELAYLOAD:$<TARGET_FILE_NAME:windows_hook>")



target_link_libraries(${TARGET}
    args
    macro_player
    windows_hook
    nlohmann_json)


set_target_properties(${TARGET} PROPERTIES
    OUTPUT_NAME "advanced-macros"                                       # Change the output name, matching the repository name
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"                  # Move binaries to a single location
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"   # Tell the debugger about the new binary location
    VS_DEBUGGER_COMMAND_ARGUMENTS "--verbose")                          # Launch a bersion with verbose logging when debugging
