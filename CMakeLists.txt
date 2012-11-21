project ( danet )
cmake_minimum_required( VERSION 2.6 )

find_package( Boost COMPONENTS system REQUIRED )

find_package (Threads)

add_definitions("-std=c++11")

include_directories(
  include/

  ${Boost_SYSTEM_INCLUDE_DIR}
)

add_library(danet SHARED
  src/netbase.cpp

  src/protocols/basic_00.cpp

  src/address/tcp_ip_address.cpp
  src/acceptors/tcp_ip_acceptor.cpp
  src/connections/tcp_ip_connection.cpp
)

target_link_libraries(danet
  ${Boost_SYSTEM_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
)