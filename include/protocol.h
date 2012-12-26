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

  /**
   * Defines basic protocol interface.
   */
  class protocol
  {
    friend class acceptor;
    friend class connection;
    friend class netbase;
  public:
    /**
     * Distroys the protocol object.
     */
    virtual ~protocol() {};
  protected:
    /**
     * Called when a connection received data.
     * @param pkg The data received by the connection.
     */
    virtual void on_receive(packet pkg) = 0;

    /**
     * Called when network controller wants to send message.
     * @param p The data to send
     * @param u Users to send the data to.
     */
    virtual void do_send(packet p, const std::vector<netbase::user> &u) = 0;

    /**
     * Called when new connection appears.
     * @param h Handle to the new connection.
     */
    virtual void connection_add(netbase::handle h) = 0;

    /**
     * Called when a connection has broken.
     * @param h Handle to the connection.
     */
    virtual void connection_rem(netbase::handle h) = 0;

    /**
     * Returns the ID associated to the network controller and protocol.
     * @return The ID or 0 if no ID assigned.
     */
    virtual netbase::user get_id() = 0;

    /**
     * Returns the users list.
     * @return The IDs of the users.
     */
    virtual std::set<netbase::user> get_users() = 0;

    /**
     * The pointer to netbase object.
     */
    netbase *nb;

    /**
     * Informs netbase to add new message to incoming queue.
     * @param p The message
     * @param s Sender of the message
     */
    void netbase_add_received_message(packet p, netbase::user s)
    {
      nb->_on_receive(move(p), s);
    }
    /**
     * Informs netbase to send data by a connection.
     * @param p Data to send.
     * @param h Connection handle.
     */
    void netbase_do_send(std::shared_ptr<packet> p, netbase::handle h)
    {
      nb->_do_send(p, h);
    }
  };

  /**
   * Namespace with diffrent communication protocols.
   */
  namespace protocols
  {

  }
}

#endif

