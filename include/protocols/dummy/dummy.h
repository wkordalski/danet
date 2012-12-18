/*
 * File:   server.h
 * Author: wojtex
 *
 * Created on 14 grudzień 2012, 19:56
 */

#ifndef __DANET_PROTOCOLS_DUMMY_DUMMY_H
#define	__DANET_PROTOCOLS_DUMMY_DUMMY_H

#include "protocol.h"

namespace danet
{
  namespace protocols
  {
    class dummy : public danet::protocol
    {
    public:
      virtual ~dummy();

      /**
       * Creates new protocol instance
       * @return The shared pointer to protocol instance.
       */
      static std::shared_ptr<protocol> create(bool server)
      {
        return std::shared_ptr<protocol>(new dummy(server));
      }

    protected:
      dummy(bool server);

      virtual void data_received(packet &pkg);
      virtual void send_data(packet p, const std::vector<netbase::user> &u);
      virtual void add_connection(netbase::handle h);
      virtual void rem_connection(netbase::handle h);
      virtual netbase::user get_id();

      bool isserver = false;                        // Is this instance a server
      netbase::user mid = 0;                        // My user id
      netbase::user sid = 0;                        // Server id
      std::map<netbase::user, netbase::handle> add; // Server all connections
      netbase::handle srv = 0;                      // Server handle
    };
  }
}

#endif	/* SERVER_H */

