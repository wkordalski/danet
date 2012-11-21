#ifndef __DANET_TCP_IP_ACCEPTOR_H
#define	__DANET_TCP_IP_ACCEPTOR_H

#include "netbase.h"
#include "address/tcp_ip_address.h"
#include "acceptor.h"

#include <memory>
#include <string>

namespace danet
{
  namespace ip
  {
    namespace tcp
    {
      class acceptor : public danet::acceptor
      {
        friend class danet::ip::tcp::address;
      public:
        virtual ~acceptor();
      protected:
        acceptor(const danet::ip::tcp::address &adr);
        bool run(netbase *nb);
        void accept();
        void on_accept(const boost::system::error_code & ec, std::shared_ptr<connection> con);

        // Address...
        danet::ip::tcp::address adr;

        // Acceptor object pointer
        boost::asio::ip::tcp::acceptor *acc = nullptr;
      };
    }
  }
}

#endif

