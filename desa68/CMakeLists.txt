# -*- cmake -*-
#
# desa68

# ======================================================================
# cmake setup

cmake_minimum_required(VERSION 3.3)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

# ======================================================================
# Modules

include(CheckIncludeFile)
include(CheckFunctionExists)
include(GNUInstallDirs)

# ======================================================================
# Options

option(
  BUILD_SHARED_LIBS
  "Prefer shared library over static"
  ON)

option(
  VERBOSE_MAKEFILE
  "Generate verbose Makefile"
  OFF)
set(CMAKE_VERBOSE_MAKEFILE ${VERBOSE_MAKEFILE})

# ======================================================================
# Project

execute_process(
  COMMAND ./vcversion.sh 2.0.0
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  TIMEOUT 15
  RESULT_VARIABLE VC_RES
  OUTPUT_VARIABLE VC_OUT
  ERROR_QUIET)
if (NOT "0" STREQUAL "${VC_RES}")
  message(
    FATAL_ERROR
    "Failed to get revision from source control:\n${VC_RES}")
endif (NOT "0" STREQUAL "${VC_RES}")
string(STRIP "${VC_OUT}" VC_OUT)

project(
  desa68
  VERSION ${VC_OUT}
  LANGUAGES C)

set(PACKAGE "${PROJECT_NAME}")
set(VERSION "${PROJECT_VERSION}")
set(PACKAGE_BUGREPORT "http://sourceforge.net/projects/sc68")
set(PACKAGE_URL "http://sc68.atari.org")
set(PACKAGE_DESC "Motoral MC68000 disassembler library")
set(PACKAGE_SHORTDESC "${PACKAGE_DESC}")

string(REGEX REPLACE "^0*(0$|[1-9]+[0-9]*$)" "\\1"
  VNUM_MAJOR 00${PROJECT_VERSION_MAJOR})
string(REGEX MATCH "([0-9][0-9])$" VNUM_MINOR 00${PROJECT_VERSION_MINOR})
string(REGEX MATCH "([0-9][0-9])$" VNUM_PATCH 00${PROJECT_VERSION_PATCH})
string(REGEX MATCH "([0-9][0-9][0-9][0-9])$"
  VNUM_TWEAK 0000${PROJECT_VERSION_TWEAK})
set(PACKAGE_VERNUM ${VNUM_MAJOR}${VNUM_MINOR}${VNUM_PATCH}${VNUM_TWEAK})

message("${PACKAGE} version ${VERSION} (${PACKAGE_VERNUM})")

# ======================================================================
# Configs
#
# $$$ Shouldn't those file be by configuration instead of by build ?

set(prefix "${CMAKE_INSTALL_PREFIX}")
set(exec_prefix "${prefix}")
set(datarootdir "${DATAROOTDIR}")
set(datadir "${DATADIR}")
set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
set(libdir "\${prefix}/${CMAKE_INSTALL_LIBDIR}")

configure_file (
  "${PROJECT_SOURCE_DIR}/cm_config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  @ONLY)

configure_file (
  "${PROJECT_SOURCE_DIR}/desa68.pc.in"
  "${PROJECT_BINARY_DIR}/desa68.pc"
  @ONLY)

add_definitions(-DHAVE_CONFIG_H)

# ======================================================================
# Target library: desa68

CHECK_INCLUDE_FILE(assert.h HAVE_ASSERT_H)
CHECK_INCLUDE_FILE(stdint.h HAVE_STDINT_H)

add_library(desa68_lib desa68.c)

set_target_properties(
  desa68_lib PROPERTIES
  OUTPUT_NAME desa68
  C_STANDARD 99
  C_VISIBILITY_PRESET hidden
  )

install(
  TARGETS desa68_lib
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )

install(
  FILES desa68.h DESTINATION include
  )

install(
  FILES ${PROJECT_BINARY_DIR}/desa68.pc DESTINATION lib/pkgconfig
  )
