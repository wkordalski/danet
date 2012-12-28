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

#ifndef __DANET_ACCEPTOR_H
#define	__DANET_ACCEPTOR_H

namespace danet
{
  class acceptor;
}

#include "netbase.h"
#include "connection.h"

#include <boost/asio.hpp>

#include <memory>
#include <string>

namespace danet
{
  /**
   * Base class for acceptors
   */
  class acceptor
  {
    friend netbase;
  public:
    /**
     * Distroys the acceptor
     */
    virtual ~acceptor() {};
  private:
  protected:
    /**
     * Runs acceptor for specified netbase
     * @param nb The netbase pointer
     * @return True if succedded, otherwise false.
     */
    virtual bool run(netbase *nb) = 0;

    /**
     * Returns the address on what is the acceptor running.
     * @return Pointer to the address.
     */
    virtual std::shared_ptr<danet::address> get_address() = 0;

    /**
     * Netbase object pointer.
     */
    netbase *nb;

    /**
     * Returns Boost Asio Service associated to the netbase.
     * @return Reference to Boost Asio Service.
     */
    boost::asio::io_service & get_ioservice()
    {
      return nb->_service;
    }

    /**
     * Adds new connection to the netbase.
     * @param con The connection to add.
     * @return Handle associated to the connection.
     */
    netbase::handle netbase_add_connection(std::shared_ptr<connection> con)
    {
      return this->nb->_connection_add(con);
    }

    /**
     * Informs communication protocol about new connection.
     * @param h Handle to the connection.
     */
    void proto_add_connection(netbase::handle h)
    {
      return nb->_proto->connection_add(h);
    }
  };
}
#endif	/* ACCEPTOR_H */

