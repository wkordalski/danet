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

#ifndef __DANET_PROTOCOLS_BASIC_BASIC_0_H
#define __DANET_PROTOCOLS_BASIC_BASIC_0_H

#include "protocol_basic.h"

#include <memory>

namespace danet
{
  namespace protocols
  {
    /**
     * Basic communication protocol (version 0)
     */
    template<>
    class basic<0> : public danet::protocol
    {
    public:
      virtual ~basic();

      /**
       * Creates new protocol instance
       * @param rts Routing Table maximum size (-1 is unlimited)
       * @return The shared pointer to protocol instance.
       */
      static std::shared_ptr<protocol> create(int rts)
      {
        return std::shared_ptr<protocol>(new basic<0>(rts));
      }

    protected:
      basic(int rts);

      virtual void on_receive(packet pkg, netbase::handle h);
      virtual void do_send(std::shared_ptr<packet> p, const std::vector<netbase::user> &u);
      virtual void do_send_all(std::shared_ptr<packet> p);
      virtual void connection_add(netbase::handle h);
      virtual void connection_rem(netbase::handle h);
      virtual netbase::user get_id();
      virtual std::set<netbase::user> get_users();

      int rts;
    };
  }
}

#endif	/* BASIC_0_H */

