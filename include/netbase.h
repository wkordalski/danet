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

#ifndef __DANET_NETBASE_H
#define __DANET_NETBASE_H

#include <vector>

namespace danet
{
  class netbase;
  class acceptor;
  class connection;

  /**
   * Type representing byte
   */
  typedef unsigned char byte;

  /**
   * Type representing network packet
   */
  typedef std::vector<byte> packet;
}

#include "address.h"

#include <boost/asio.hpp>

#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <vector>


namespace danet
{
  class protocol;

  /**
   * The base class of network controllers.
   */
  class netbase
  {
  public:
    /**
     * Represents the network user id type
     */
    typedef unsigned int user;

    /**
     * Represents the network manager handle type
     */
    typedef unsigned int handle;

  protected:
    /**
     * Creates new instance of netbase.
     * @param pro Communication protocol to use.
     */
    netbase(std::shared_ptr<protocol> pro);

    netbase(const netbase&) = delete;
    netbase(netbase &&nb) = delete;

    /**
     * Distroys this netbase instance.
     */
    ~netbase();

  protected:
    /**
     * Asks the network controller to listen on specified address.
     * @param adr The addres to listen on.
     * @return Handle to acceptor.
     */
    handle _listen(address *adr);

    /**
     * Asks the network controller to connect to specified address.
     * @param adr The address to connect to.
     * @return Handle to connection.
     */
    handle _connect(address *adr);

    /**
     * Closes specified acceptor or connection
     * @param h Handle to acceptor or connection.
     */
    void _close(handle h);

    /**
     * Sends data to specified users.
     * @param v The data to send.
     * @param s Users to send the data to.
     */
    void _send(std::vector<byte> v, const std::vector<user> &s);

    /**
     * Receives data from the message incomming queue.
     * @param v The received data.
     * @param s The sender of the data.
     */
    void _recv(std::vector<byte> &v, user& s);

    /**
     * Called to send data by specified handle.
     * Used by protocols.
     * @param v The pointer to data to send.
     * @param h The handle to connection to send data with.
     */
    void _do_send(std::shared_ptr<packet> v, handle h);

    /**
     * Returns the users list in the network.
     * @return THe users' IDs.
     */
    std::set<user> _get_users_list();

    /**
     * Adds messages to incomming queue.
     * Used by protocols.
     * @param p The received data.
     * @param s The sender of the data.
     */
    void _on_receive(packet p, user s);

    /**
     * Adds the connection to connections pool.
     * @param con The connection to add.
     * @return Associated handle to the connection.
     */
    handle _connection_add(std::shared_ptr<connection> con);

    /**
     * Removes the connection from the connections pool.
     * @param h The handle to the connection.
     */
    void _connection_rem(handle h);

    void _connection_tik(handle h);

    /**
     * Returns the address associated to the acceptor or connection.
     * @param h The handle to the acceptor or connection.
     * @return The pointer to the address.
     */
    std::shared_ptr<danet::address> _get_address(handle h);

    /**
     * Returns the ID associated to your network controller instance.
     * @return The ID.
     */
    netbase::user _get_id();

    std::set<handle> _get_connections();
    std::set<handle> _get_acceptors();
    std::set<handle> _get_connecting();

    /**
     * Returns the reference to Boost Asio Service
     * @return The reference to Boost Asio Service.
     */
    boost::asio::io_service & _get_service()
    {
      return this->_service;
    }

    /**
     * Runs the Boost Asio Service tasks.
     */
    void _io_worker();

  private:
protected:

    friend class connection;
    friend class acceptor;
    friend class danet::protocol;

    /**
     * Boost Asio Service object.
     */
    boost::asio::io_service _service;

    /**
     * Boost Asio Work object.
     */
    boost::asio::io_service::work _work_object;

    /**
     * Threads that are working for Boost Asio Service.
     */
    std::vector<std::thread *> _worker_objects;

    /**
     * The acceptors pool
     */
    std::map<int,std::shared_ptr<acceptor>> _acceptors;

    /**
     * Variable used to generate new acceptor handles.
     */
    int _acceptors_oid = 0;

    /**
     * The connections pool
     */
    std::map<int,std::shared_ptr<connection>> _connections;

    std::map<int,std::shared_ptr<connection>> _connecting;

    /**
     * Variable used to generate new connection handles.
     */
    int _connections_oid = 0;

    /**
     * Mutex locking the connections pool.
     */
    std::mutex _connections_m;

    /**
     * Pointer to the communication protocol instance.
     */
    std::shared_ptr<danet::protocol> _proto;

    /**
     * Incomming message queue.
     */
    std::queue<std::pair<int,packet>> _msgs;

    /**
     * Mutex locking the incomming message queue.
     */
    std::mutex _msgs_m;
  };
}

#endif	/* NETMAN_H */

