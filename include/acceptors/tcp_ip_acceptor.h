#ifndef __DANET_TCP_IP_ACCEPTOR_H
#define	__DANET_TCP_IP_ACCEPTOR_H

#include "netbase.h"
#include "acceptor.h"

#include <memory>
#include <string>

namespace danet
{
  namespace ip
  {
    namespace tcp
    {
      class acceptor : danet::acceptor
      {
      public:
        acceptor(std::string ip, int port);
        virtual ~acceptor();
      protected:
        void run(netbase *nb);
        void accept();
        void on_accept(const boost::system::error_code & ec, std::shared_ptr<connection*> con);

        // Address...
        danet::ip::tcp::address *adr;

        // Acceptor object pointer
        boost::asio::ip::tcp::acceptor *acc = nullptr;
      };
    }
  }
}

#endif

