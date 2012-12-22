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

project ( danet )
cmake_minimum_required( VERSION 2.6 )

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
)

add_library(danet SHARED
  src/netbase.cpp

  src/protocols/basic/basic_00.cpp
  src/protocols/dummy/dummy.cpp

  src/address/tcp_ip_address.cpp
  src/acceptors/tcp_ip_acceptor.cpp
  src/connections/tcp_ip_connection.cpp
)

add_executable( dummy-server test/dummy-server.cpp )
add_executable( chat test/chat.cpp )

target_link_libraries(danet
  ${Boost_SYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(dummy-server
  danet
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

target_link_libraries(chat
  danet
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
)