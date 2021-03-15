set(TARGET macro_player)

set(HEADERS
    src/actions.hpp
    src/keystroke_emulator.hpp
    src/keystroke_listener.hpp
    src/process_launcher.hpp
    src/settings.hpp)

set(SOURCES
    src/actions.cpp
    src/main.cpp
    src/keystroke_emulator_win.cpp
    src/keystroke_listener_win.cpp
    src/process_launcher_win.cpp
    src/settings.cpp)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

# Note: most of it is going to be Windows specific anyway, so let's not bother with indirections
target_link_options(${TARGET} PRIVATE "/SUBSYSTEM:WINDOWS" "/DELAYLOAD:$<TARGET_FILE_NAME:windows_hook>")

target_link_libraries(${TARGET}
    windows_hook
    nlohmann_json)

# Move binaries to a single location, and tell VS debugger about it
set_target_properties(${TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
