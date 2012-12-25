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
    virtual void on_receive(packet pkg) = 0;
    virtual void do_send(packet p, const std::vector<netbase::user> &u) = 0;
    virtual void connection_add(netbase::handle h) = 0;
    virtual void connection_rem(netbase::handle h) = 0;
    virtual netbase::user get_id() = 0;
    virtual std::set<netbase::user> get_users() = 0;

    // Netbase object pointer
    netbase *nb;

    // Operują na netbase (bo protocol jest przyjacielem netbase)
    void netbase_add_received_message(packet p, netbase::user s)
    {
      nb->_on_receive(move(p), s);
    }
    void netbase_do_send(std::shared_ptr<packet> p, netbase::handle h)
    {
      nb->_do_send(p, h);
    }
  };
}

#endif

