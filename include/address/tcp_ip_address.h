/*
 * File:   tcp_ip_address.h
 * Author: wojtex
 *
 * Created on 20 listopad 2012, 15:38
 */

#ifndef __DANET_TCP_IP_ADDRESS_H
#define	__DANET_TCP_IP_ADDRESS_H

#include "address.h"
#include "acceptors/tcp_ip_acceptor.h"
#include "connections/tcp_ip_connection.h"

#include <string>

namespace danet
{
  namespace ip
  {
    namespace tcp
    {
      class address : danet::address
      {
        friend class danet::ip::tcp::acceptor;
        friend class danet::ip::tcp::connection;
        typedef boost::asio::ip::basic_resolver_entry<boost::asio::ip::tcp> phys_addr;

        std::string ip;
        int port;
      public:
        address(std::string ip, int port);
        ~address();

        danet::acceptor *    acceptor();
        danet::connection *  connection();

      protected:
        phys_addr resolve();
      };
    }
  }
}

#endif

