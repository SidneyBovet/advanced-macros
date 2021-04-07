set(TARGET advanced_macros_win)

set(HEADERS)

set(SOURCES
    src/main.cpp)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

# Specify that we're going to be a window, and prevent DLL hijacking attacks for some DLLs
# TODO: use procmon to list all possible hijacking targets
target_link_options(${TARGET} PRIVATE
    "/SUBSYSTEM:WINDOWS"
    "/DELAYLOAD:user32.dll")

target_link_libraries(${TARGET}
    PRIVATE args
    PRIVATE macro_player)

set_target_properties(${TARGET} PROPERTIES
    OUTPUT_NAME "advanced-macros"                                       # Change the output name, matching the repository name
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"                  # Move binaries to a single location
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"   # Tell the debugger about the new binary location
    VS_DEBUGGER_COMMAND_ARGUMENTS "--verbose")                          # Launch a bersion with verbose logging when debugging
