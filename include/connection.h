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
  /**
   * Base class for connections.
   */
  class connection
  {
    friend class netbase;
    friend class acceptor;
  public:
    /**
     * Distroys the connection.
     */
    virtual ~connection() {};

  protected:
    /**
     * Runs connection for specified namespace.
     * @param nb Pointer to netbase.
     * @param id Handle to the connection.
     * @return True if succedded, otherwise false.
     */
    virtual bool run(netbase *nb, netbase::handle id) = 0;

    /**
     * Runs connection (netbase specified earlier)
     * @param id Handle to the connection.
     * @return True if succedded, otherwise false.
     */
    virtual bool run(netbase::handle id) = 0;

    /**
     * Sends data by the connection.
     * @param data Data to send.
     */
    virtual void do_send(std::shared_ptr<packet> data) = 0;

    /**
     * Returns the address on what is the connection running.
     * @return Pointer to the address.
     */
    virtual std::shared_ptr<danet::address> get_address() = 0;

    /**
     * Netbase object pointer.
     */
    netbase *nb;

    /**
     * Connection handle
     */
    netbase::handle id;

    /**
     * Returns Boost Asio Service associated to the netbase.
     * @return Reference to Boost Asio Service.
     */
    boost::asio::io_service & get_ioservice()
    {
      return nb->_service;
    }

    /**
     * Informs communication protocol about received data.
     * @param pkg Received data.
     */
    void proto_on_recieve(packet pkg)
    {
      return nb->_proto->on_receive(move(pkg));
    }

    /**
     * Informs communication protocol about new connection.
     */
    void proto_add_connection()
    {
      return nb->_proto->connection_add(id);
    }

    /**
     * Informs communication protocol about removed connection.
     */
    void proto_rem_connection()
    {
      return nb->_proto->connection_rem(id);
    }

    /**
     * Removes connection from netbase connection pool.
     */
    void netbase_rem_connection()
    {
      nb->_connection_rem(id);
    }
  };
}
#endif

