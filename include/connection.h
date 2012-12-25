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

#ifndef __DANET_CONNECTION_H
#define	__DANET_CONNECTION_H

namespace danet
{
  class connection;
}

#include "netbase.h"
#include "protocol.h"
#include "acceptor.h"

#include <boost/asio.hpp>

#include <mutex>
#include <string>
#include <vector>

namespace danet
{
  class connection
  {
    friend class netbase;
    friend class acceptor;
  public:
    virtual ~connection() {};

  protected:
    virtual bool run(netbase *nb, netbase::handle id) = 0;
    virtual bool run(netbase::handle id) = 0;

    virtual void do_send(std::shared_ptr<packet> data) = 0;

    virtual std::shared_ptr<danet::address> get_address() = 0;

    // Netbase object pointer
    netbase *nb;

    // Connection id
    netbase::handle id;

    boost::asio::io_service & get_ioservice()
    {
      return nb->_service;
    }

    void proto_on_recieve(packet pkg)
    {
      return nb->_proto->on_receive(move(pkg));
    }

    void proto_add_connection()
    {
      return nb->_proto->connection_add(id);
    }

    void proto_rem_connection()
    {
      return nb->_proto->connection_rem(id);
    }

    void netbase_rem_connection()
    {
      nb->_connection_rem(id);
    }

  private:

  };
}
#endif

