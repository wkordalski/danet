/*
 * Danet (Distributed Asynchronous Network)
 * This file is a part of danet library.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * For a list of authors see AUTHORS file.
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

      virtual void data_received(packet pkg);
      virtual void send_data(packet p, const std::vector<netbase::user> &u);
      virtual void add_connection(netbase::handle h);
      virtual void rem_connection(netbase::handle h);
      virtual netbase::user get_id();
      virtual std::set<netbase::user> get_users();

      bool isserver = false;                        // Is this instance a server
      netbase::user mid = 0;                        // My user id
      std::map<netbase::user, netbase::handle> add; // Server all connections
      netbase::handle srv = 0;                      // Server handle
      std::set<netbase::user> usr;                  // All users in network
    };
  }
}

#endif	/* SERVER_H */

