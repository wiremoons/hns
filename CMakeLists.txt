# To execute the cmake build: 
#   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
#   cmake --build build
#
# For 'static' cmake build use:
#   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF
#   cmake --build build
#
cmake_minimum_required(VERSION 3.20)
project(hns)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# add location for built binary file
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
# add list of c++ source code files to ${SOURCES}
file(GLOB SOURCES "./src/*.cc")

# cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
    add_definitions(-DNDEBUG)
    message("CMake build with 'DEBUG' enabled")
endif()

# Use static library build
# to set equivalent of build flag: -DBUILD_SHARED_LIBS=OFF)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# fix '-static' compile error
IF(APPLE)
    # Fix linking on 10.14+. See https://stackoverflow.com/questions/54068035
    LINK_DIRECTORIES(/usr/local/lib)
ENDIF()

# add flag to hide depreciation code warnings
#add_definitions(-DAPPROVAL_TESTS_HIDE_DEPRECATED_CODE=1)

if(WIN32) # Install dlls in the same directory as the executable on Windows
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

# add additional compiler options (NB build fails with: '-Werror')
# Silence 'zlib' build warnings: -Qunused-arguments
# Silence 'curl' build warnings: -Wno-deprecated
add_compile_options(-Wall -Wextra -pedantic -Qunused-arguments -Wno-deprecated -static)
add_executable(hns ${SOURCES})

set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "")
include(FetchContent)

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.8.3) 
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 8.1.1)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.10.0)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz
)
FetchContent_MakeAvailable(json)

target_link_libraries(hns PRIVATE cpr::cpr fmt::fmt spdlog::spdlog nlohmann_json::nlohmann_json)




