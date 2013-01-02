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

#ifndef __DANET_NETWORK_H
#define __DANET_NETWORK_H

#include "netbase.h"
#include "address.h"
#include "serializer.h"

#include <set>
#include <string>
#include <vector>


/**
 * Danet library namespace
 */
namespace danet
{
  /**
   * Network controller - manages connections and provides data
   *
   * @tparam T Messages type you want to send or receive.
   * The message type has to contain static functions get_data i set_data.
   */
  template<class T>
  class network : protected netbase
  {
  public:
    /**
     * Represents the network manager handle type
     */
    typedef netbase::handle handle;

    /**
     * Represents the network user id type
     */
    typedef netbase::user user;

    /**
     * Creates new instance of network manager.
     * @param pro The communication protocol to use.
     * @param srl The serializer to serialize messages.
     */
    network(std::shared_ptr<protocol> pro, std::shared_ptr<serializer<T>> srl)
        : netbase(pro), srl(srl)
    {

    }

    network(const network<T> &) = delete;

    /**
     * Asks the network controller to listen on specified address.
     * @param adr The address to listen on.
     * @return The handle to the listening acceptor.
     */
    handle listen(address *adr)
    {
      return this->_listen(adr);
    }

    /**
     * Asks the network controller to connect to specified address.
     * @param adr The address to connect to.
     * @return The handle to the connection.
     */
    handle connect(address *adr)
    {
      return this->_connect(adr);
    }

    /**
     * Closes the acceptor or connection
     * @param h Handle to the acceptor or connection.
     */
    void close(handle h)
    {
      this->_close(h);
    }

    /**
     * Sends a message to specified users.
     * @param m Message to send.
     * @param s Users to send the message to.
     */
    void send(const T &m, const std::vector<user> &s)
    {
      this->_send(srl->save(m), s);
    }

    /**
     * Receives messages from the incoming queue.
     * @param m Received message
     * @return User ID or 0 if no message in the queue.
     */
    user receive(T &m)
    {
      user uid = 0;
      std::vector<byte> v;
      this->_recv(v, uid);
      if(uid > 0)
        m = std::move(srl->load(v));
      return uid;
    }

    /**
     * Returns users in the network.
     * @return Users IDs in the network.
     */
    std::set<user> users()
    {
      return this->_get_users_list();
    }

    /**
     * Returns the address associated to acceptor or connection.
     * @param h The acceptor or connection handle.
     * @return The address.
     */
    std::shared_ptr<danet::address> address(handle h)
    {
      return this->_get_address(h);
    }

    /**
     * Returns the ID associated to your network controller.
     * @return The ID.
     */
    user id()
    {
      return this->_get_id();
    }

    /**
     * Returns the active connections.
     * @return Set with handles to connections.
     */
    std::set<handle> connections()
    {
      return this->_get_connections();
    }

    /**
     * Returns the active acceptors.
     * @return Set with handles to acceptors.
     */
    std::set<handle> acceptors()
    {
      return this->_get_acceptors();
    }

    /**
     * Returns the waiting for acception connections.
     * @return Set with handles to connections.
     */
    std::set<handle> connecting()
    {
      return this->_get_connecting();
    }

  private:
    /**
     * Pointer to serializer.
     */
    std::shared_ptr<danet::serializer<T>> srl;
  };
}

#endif
