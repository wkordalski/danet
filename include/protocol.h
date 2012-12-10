#ifndef __DANET_PROTOCOL_H
#define	__DANET_PROTOCOL_H

namespace danet
{
  class protocol;
}

#include "netbase.h"

namespace danet
{
  class connection;

  class protocol
  {
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

    // Netbase object pointer
    netbase *nb;

    // Operują na netbase (bo protocol jest przyjacielem netbase)
    void add_received_message(packet &p, netbase::user s)
    {
      nb->message_received(p, s);
    }
    void add_message_sending(std::shared_ptr<packet> p, netbase::handle h)
    {
      nb->send_to_resource(p, h);
    }
  };
}

#endif

