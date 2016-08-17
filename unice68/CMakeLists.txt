# -*- cmake -*-
#
# unice68

cmake_minimum_required(VERSION 3.3)

execute_process(
  COMMAND ./vcversion.sh 2.0.0
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  TIMEOUT 15
  RESULT_VARIABLE vc_res
  OUTPUT_VARIABLE vc_out
  ERROR_QUIET)
string(STRIP "${VCREV}" VCREV)

project(
  unice68
  VERSION ${VCREV}
  LANGUAGES C)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

include(CheckIncludeFile)
include(CheckFunctionExists)

# ======================================================================
# Options

option(
  BUILD_UNICE68_CLI
  "Build unice68 command line executable"
  ON)

# ======================================================================
# Packages

set(PACKAGE ${PROJECT_NAME})
set(VERSION ${PROJECT_VERSION})
set(PACKAGE_BUGREPORT http://sourceforge.net/projects/sc68)
set(PACKAGE_URL http://sc68.atari.org)
set(PACKAGE_DESC ice packer/depacker program and library.)

string(REGEX MATCH "([0-9][0-9])$" VNUM_MAJOR 00${PROJECT_VERSION_MAJOR})
string(REGEX MATCH "([0-9][0-9])$" VNUM_MINOR 00${PROJECT_VERSION_MINOR})
string(REGEX MATCH "([0-9][0-9])$" VNUM_PATCH 00${PROJECT_VERSION_PATCH})
string(REGEX MATCH "([0-9][0-9])$" VNUM_TWEAK 00${PROJECT_VERSION_TWEAK})
set(PACKAGE_VERNUM ${VNUM_MAJOR}${VNUM_MINOR}${VNUM_PATCH}${VNUM_TWEAK})

# ======================================================================
# Configs

configure_file (
  "${PROJECT_SOURCE_DIR}/cm_config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}")
add_definitions(-DHAVE_CONFIG_H)

# ======================================================================
# Target library: unice68

CHECK_INCLUDE_FILE(assert.h HAVE_ASSERT_H)
CHECK_INCLUDE_FILE(stdint.h HAVE_STDINT_H)

add_library(unice68_lib
  unice68_pack.c unice68_unpack.c unice68_version.c)

set_target_properties(
  unice68_lib PROPERTIES
  OUTPUT_NAME unice68
  C_STANDARD 99
  COMPILE_DEFINITIONS "UNICE68_EXPORT")

set(sharedlib_DEFS UNICE68_EXPORT)
if(${WIN32})
  list(APPEND sharedlib_DEFS DLL_EXPORT HAVE_DECLSPEC)
  set(UNICE68_DLL UNICE68_DLL)
endif(${WIN32})

install(
  TARGETS unice68_lib
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )

install(
  FILES unice68.h DESTINATION include
  )


# ======================================================================
# Target binary: unice68

if (BUILD_UNICE68_CLI)
  
  CHECK_INCLUDE_FILE (libgen.h HAVE_LIBGEN_H)
  CHECK_INCLUDE_FILE (unistd.h HAVE_UNISTD_H)
  CHECK_INCLUDE_FILE (fcntl.h HAVE_FCNTL_H)

  check_function_exists (basename HAVE_BASENAME)
  check_function_exists (setmode HAVE_SETMODE)
  check_function_exists (_setmode HAVE__SETMODE)

  add_executable(unice68_exe unice68.c)
  target_link_libraries(unice68_exe unice68_lib)
  set_target_properties(
    unice68_exe PROPERTIES
    OUTPUT_NAME unice68
    C_STANDARD 99
    COMPILE_DEFINITIONS "")
  install(
    TARGETS unice68_exe
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )
  
endif (BUILD_UNICE68_CLI)
