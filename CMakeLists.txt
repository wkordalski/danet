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