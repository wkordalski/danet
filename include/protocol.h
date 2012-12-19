#ifndef __DANET_PROTOCOL_H
#define	__DANET_PROTOCOL_H

namespace danet
{
  class protocol;
}

#include "netbase.h"

#include <set>

namespace danet
{
  class acceptor;
  class connection;

  class protocol
  {
    friend class acceptor;
    friend class connection;
    friend class netbase;
  public:
    virtual ~protocol() {};
  protected:
    // Komendy od netbase
    virtual void data_received(packet &pkg) = 0;
    virtual void send_data(packet p, const std::vector<netbase::user> &u) = 0;
    virtual void add_connection(netbase::handle h) = 0;
    virtual void rem_connection(netbase::handle h) = 0;
    virtual netbase::user get_id() = 0;
    virtual std::set<netbase::user> get_users() = 0;

    // Netbase object pointer
    netbase *nb;

    // Operują na netbase (bo protocol jest przyjacielem netbase)
    void netbase_add_received_message(packet &p, netbase::user s)
    {
      nb->message_received(p, s);
    }
    void netbase_send_to_resource(std::shared_ptr<packet> p, netbase::handle h)
    {
      nb->send_to_resource(p, h);
    }
  };
}

#endif

