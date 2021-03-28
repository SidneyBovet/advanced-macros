cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# Allow use of project folders for IDEs like Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

project(macro_player VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Configure dependencies using CPM
include(cmake/CPM.cmake)

CPMAddPackage(
  NAME googletest
  GITHUB_REPOSITORY google/googletest
  GIT_TAG release-1.10.0
  VERSION 1.10.0
  OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"
)

CPMAddPackage(
  NAME nlohmann_json
  VERSION 3.6.1
  # the git repo is incredibly large, so we download the archived include directory
  URL https://github.com/nlohmann/json/releases/download/v3.6.1/include.zip
  URL_HASH SHA256=69cc88207ce91347ea530b227ff0776db82dcb8de6704e1a3d74f4841bc651cf
)

if (nlohmann_json_ADDED)
  add_library(nlohmann_json INTERFACE IMPORTED)
  target_include_directories(nlohmann_json INTERFACE ${nlohmann_json_SOURCE_DIR})
endif()

CPMAddPackage("gh:gabime/spdlog@1.8.5")

# Treat warnings as errors
if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# This is our main app

add_subdirectory(windows_hook_dll)
add_subdirectory(macro_player)
add_subdirectory(app_win)
