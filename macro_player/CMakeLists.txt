set(TARGET macro_player)

set(HEADERS
    include/macro_player/actions.hpp
    include/macro_player/actions_executor.hpp
    include/macro_player/keystroke_emulator.hpp
    include/macro_player/keystroke_listener.hpp
    include/macro_player/logging.hpp
    include/macro_player/process_launcher.hpp
    include/macro_player/settings.hpp)

set(SOURCES
    src/actions_executor.cpp
    src/settings.cpp)

if (MSVC)
    list(APPEND SOURCES
        src/keycodes_win.hpp
        src/keycodes_win.cpp
        src/keystroke_emulator_win.cpp
        src/keystroke_listener_win.cpp
        src/logging.cpp
        src/process_launcher_win.cpp)
elseif(UNIX)
    # Add support for other platforms here
endif()

add_library(${TARGET} STATIC ${HEADERS} ${SOURCES})

# Add flatbuffer files
get_target_property(FLATBUFS_HEADERS schema GENERATED_INCLUDES_DIR)
#set(FLATBUFS_HEADERS "${FLATBUFS_HEADERS}")
message("-- Also including ${FLATBUFS_HEADERS}")
add_dependencies(${TARGET} schema)

target_include_directories(${TARGET}
    PRIVATE ${FLATBUFS_HEADERS}
    PUBLIC include)

target_link_libraries(${TARGET}
    PUBLIC spdlog
    PRIVATE flatbuffers)


# Unit tests
add_executable(macro_player_tests
    tests/listener_emulator_tests.cpp
    tests/settings_tests.cpp)

#target_include_directories(macro_player_tests PRIVATE src)

target_link_libraries(macro_player_tests PRIVATE
    nlohmann_json
    macro_player
    gmock_main
    gtest_main)

add_test(macro_player_test
    macro_player_tests)

set_target_properties(macro_player_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

# TODO: it would be nicer to query 'schema' for its source files
add_custom_command(TARGET ${TARGET}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/../schema/src/actions.fbs
    "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
