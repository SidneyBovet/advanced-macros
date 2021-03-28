set(TARGET macro_player)

set(HEADERS
    include/macro_player/actions.hpp
    include/macro_player/keystroke_emulator.hpp
    include/macro_player/keystroke_listener.hpp
    include/macro_player/logging.hpp
    include/macro_player/process_launcher.hpp
    include/macro_player/settings.hpp)

set(SOURCES
    src/actions.cpp
    src/settings.cpp)

if (MSVC)
    list(APPEND SOURCES
        src/keycodes_win.hpp
        src/keycodes_win.cpp
        src/keystroke_emulator_win.cpp
        src/keystroke_listener_win.cpp
        src/process_launcher_win.cpp)
elseif(UNIX)
    # Add support for other platforms here
endif()

add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})

target_include_directories(${TARGET} PUBLIC include)

target_link_libraries(${TARGET}
    PUBLIC nlohmann_json
    PUBLIC spdlog)

if (MSVC)
    target_link_libraries(${TARGET}
        PRIVATE windows_hook)
endif()
