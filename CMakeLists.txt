#
# Danet (Distributed Asynchronous Network)
# This file is a part of danet library.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# For a list of authors see AUTHORS file.
#

project ( Danet )
cmake_minimum_required( VERSION 2.6 )

option(MAKE_DOC "Build documentation of Danet library" ON)

### VERSION
set(DANET_MAJOR_VERSION 0)
set(DANET_MINOR_VERSION 0)
set(DANET_PATCH_VERSION 1)
set(DANET_VERSION
  ${DANET_MAJOR_VERSION}.${DANET_MINOR_VERSION}.${DANET_PATCH_VERSION}
)

### INSTALATION DIRECTORIES
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for library")
set(INSTALL_INC_DIR include CACHE PATH "Installation directory for header files")
set(INSTALL_DOC_DIR share/doc CACHE PATH "Installation directory for documentation files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/danet)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

foreach(p LIB BIN INC CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

### NEEDED PACKAGES
find_package( Boost COMPONENTS system program_options REQUIRED )
find_package (Threads)

# DEBUG & RELEASE
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_CSS_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

add_definitions("-std=c++11")

include_directories(
  include/

  ${Boost_SYSTEM_INCLUDE_DIR}
  ${Boost_PROGRAM_OPTIONS_INCLUDE_DIR}

  ${PROJECT_BINARY_DIR}
)

### CONFIGURATION FILE
configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

### LIBRARY GENERATION
add_library(danet SHARED
  src/netbase.cpp

  src/protocols/basic/basic_00.cpp
  src/protocols/dummy/dummy.cpp

  src/address/tcp_ip_address.cpp
  src/acceptors/tcp_ip_acceptor.cpp
  src/connections/tcp_ip_connection.cpp
)

target_link_libraries(danet
  ${Boost_SYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
)

### HEADERS
set(DANET_PUBLIC_HEADER
"include/network.h;include/netbase.h;include/protocol.h;include/acceptor.h;include/connection.h;include/address.h" )

set(DANET_PUBLIC_HEADER
"${DANET_PUBLIC_HEADER};include/address_ip_tcp.h" )

set(DANET_PUBLIC_HEADER
"${DANET_PUBLIC_HEADER};include/protocol_basic.h;include/protocol_basic_00.h;include/protocol_dummy.h")

set_target_properties(danet PROPERTIES
  PUBLIC_HEADER "${DANET_PUBLIC_HEADER};${CMAKE_CURRENT_BINARY_DIR}/config.h"
  SOVERSION ${DANET_MAJOR_VERSION}
  VERSION ${DANET_VERSION}
)

### INSTALATION
install(TARGETS danet
  EXPORT DanetLibraryDepends
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib
  PUBLIC_HEADER DESTINATION "${INSTALL_INC_DIR}/danet" COMPONENT dev)


################################################################################

### DOCUMENTATION
find_package(Doxygen)
if(DOXYGEN_FOUND AND MAKE_DOC)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc ALL
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/html/ DESTINATION "${INSTALL_DOC_DIR}/libdanet" COMPONENT doc)
endif(DOXYGEN_FOUND AND MAKE_DOC)

################################################################################

### EXPORTING
export(TARGETS danet
  FILE "${PROJECT_BINARY_DIR}/DanetLibraryDepends.cmake")

export(PACKAGE Danet)

### CONFIGURING
configure_file(DanetBuildTreeSettings.cmake.in
  "${PROJECT_BINARY_DIR}/DanetBuildTreeSettings.cmake" @ONLY)
file(RELATIVE_PATH CONF_REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INC_DIR}")
configure_file(DanetConfig.cmake.in
  "${PROJECT_BINARY_DIR}/DanetConfig.cmake" @ONLY)
configure_file(DanetConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/DanetConfigVersion.cmake" @ONLY)

### INSTALLING CMAKE SUBSYSTEM
install(FILES
  "${PROJECT_BINARY_DIR}/DanetConfig.cmake"
  "${PROJECT_BINARY_DIR}/DanetConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

install(EXPORT DanetLibraryDepends DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)


################################################################################

add_executable( dummy-server test/dummy-server.cpp )
add_executable( chat test/chat.cpp )

target_link_libraries(dummy-server
  danet
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
)
target_link_libraries(chat
  danet
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

################################################################################
