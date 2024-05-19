# To execute the cmake build: 
#   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
#   cmake --build build
#
# For 'static' cmake build use:
#   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF
#   cmake --build build
#
cmake_minimum_required(VERSION 3.20)
message(STATUS "CMake version is: ${CMAKE_VERSION}")
# enable debugging for a subset of find calls
set (CMAKE_DEBUG_FIND_MODE=TRUE)


# required before 'project' below - detects and uses 'vcpkg' - exit it not found.
# requires env: VCPKG_ROOT  to be set to 'vcpkg' install location
message(NOTICE "++ Looking for 'vcpkg' install ++")
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  message(STATUS "Using 'vcpkg' found at: $ENV{VCPKG_ROOT}")
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
  set(VCPKG_LIBRARY_LINKAGE static)
  message(STATUS "'vcpkg' static build set")
  # below needed as 'vcpkg.json' versions feature is experimental currently
  set(VCPKG_FEATURE_FLAGS "versions")
  message(STATUS "'vcpkg' versions feature enabled")
elseif(DEFINED CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Existing 'vcpkg' toolchain set to: ${CMAKE_TOOLCHAIN_FILE}")
else ()
  message(FATAL_ERROR "[!] ABORT BUILD: 'vcpkg' not found.\n  [i] For 'vcpkg' install help: https://vcpkg.io/en/getting-started")
endif()

project(hns
        VERSION 0.5.11
        DESCRIPTION "CLI app to monitor and extract the latest Hacker News stories."
        HOMEPAGE_URL "https://github.com/wiremoons/hns"
        LANGUAGES CXX
)


# configure C++ standard to build with
set(CMAKE_CXX_STANDARD 20)
# generate a 'compile_commands.json' for editor support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# add location for built binary file
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
# If needed - installs dlls in the same directory as the executable on Windows
if(WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()
message(NOTICE "++ Final application (binary) files are output to: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ++")
# add list of c++ source code files to ${SOURCES}
file(GLOB SOURCES "./src/*.cc")

# cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
message(NOTICE "++ Checking if 'Debug' or 'Release' build requested ++")
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
    add_definitions(-DNDEBUG)
    message(STATUS "CMake build as 'Debug' enabled")
else ()
    message(STATUS "CMake build as 'Release' enabled")
endif()

# Use static library build
# to set equivalent of build flag: -DBUILD_SHARED_LIBS=OFF)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# fix '-static' compile error
IF(APPLE)
    # Fix linking on 10.14+. See https://stackoverflow.com/questions/54068035
    LINK_DIRECTORIES(/usr/local/lib)
ENDIF()

# set compiler build flags
message(NOTICE "++ Checking and setting compiler build flags ++")
add_compile_options(-Wall -Wextra -pedantic -Wno-deprecated -static)
# add additional compiler options (NB build fails with: '-Werror')
# '-Qunused-arguments' is clang only option and breaks GCC builds
# Silence 'zlib' build warnings: -Qunused-arguments
# Silence 'curl' build warnings: -Wno-deprecated
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Qunused-arguments CXX_SUPPORTS_QUNUSED-ARGUMENTS)
if(CXX_SUPPORTS_QUNUSED-ARGUMENTS)
    add_compile_options(-Qunused-arguments)
    message(STATUS "Compiler '-Qunused-arguments' option flag added")
endif()

add_executable(hns ${SOURCES})

message(NOTICE "++ Obtaining required supporting external libraries with 'vcpkg' ++")
# Use 'vcpkg' if available
if (DEFINED ENV{VCPKG_ROOT} AND DEFINED CMAKE_TOOLCHAIN_FILE)
    find_package(cpr CONFIG REQUIRED)
    find_package(fmt CONFIG REQUIRED)
    find_package(spdlog CONFIG REQUIRED)
    find_package(nlohmann_json CONFIG REQUIRED)
    find_package(argparse CONFIG REQUIRED)
endif()

target_link_libraries(hns PRIVATE cpr::cpr fmt::fmt spdlog::spdlog nlohmann_json::nlohmann_json argparse::argparse)




