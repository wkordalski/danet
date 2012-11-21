#ifndef __DANET_TCP_IP_ADDRESS_H
#define	__DANET_TCP_IP_ADDRESS_H

#include "address.h"

#include <string>

namespace danet
{
  namespace ip
  {
    namespace tcp
    {
      class acceptor;
      class connection;

      class address : public danet::address
      {
        friend class danet::ip::tcp::acceptor;
        friend class danet::ip::tcp::connection;

        std::string ip;
        int port;
      public:
        address(std::string ip, int port);
        ~address();

      protected:
        address();

        std::shared_ptr<danet::acceptor>     acceptor();
        std::shared_ptr<danet::connection>   connection();
      };
    }
  }
}

#endif

