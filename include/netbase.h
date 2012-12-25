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
  typedef unsigned char byte;
  class acceptor;
  class connection;
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
  class netbase
  {
  public:
    typedef unsigned int user;
    typedef unsigned int handle;

  protected:
    netbase(std::shared_ptr<protocol> pro);
    netbase(const netbase&) = delete;
    netbase(netbase &&nb) = delete;
    ~netbase();

  protected:
    handle _listen(address *adr);
    handle _connect(address *adr);
    void _close(handle h);
    void _send(std::vector<byte> v, const std::vector<user> &s);
    void _recv(std::vector<byte> &v, user& s);
    void _do_send(std::shared_ptr<packet> v, handle h);
    std::set<user> _get_users_list();
    void _on_receive(packet p, user s);
    handle _connection_add(std::shared_ptr<connection> con);
    void _connection_rem(handle h);
    std::shared_ptr<danet::address> _get_address(handle h);
    netbase::user _get_id();

    boost::asio::io_service & _get_service()
    {
      return this->_service;
    }

    void _io_worker();

  private:
protected:

    friend class connection;
    friend class acceptor;
    friend class danet::protocol;

    //void received(message msg);
    //void accepted(const boost::system::error_code & ec, std::shared_ptr<connection>);
    //void connected(const boost::system::error_code & ec, std::shared_ptr< connection > con);
    //void add_message(message m);
    //void bconnect(std::string ip, int port);
    //void bconnect(std::string ip, int port, int hid);
    //void inform_ips(std::shared_ptr<connection> c);
    //void suggest_id(std::shared_ptr<connection> c);
    //void recv_ips(message m);

    //int myid = -1;
    //std::vector<std::shared_ptr<connection>> pairs;
    //std::mutex parm;
    //int pairs_count;
    //std::queue<message> rcvq;
    //std::mutex rcvm;
    //std::vector<std::shared_ptr<connection>> unsorted;
    //int state = 0;
    //std::mutex unsm;
    boost::asio::io_service _service;
    boost::asio::io_service::work _work_object;
    std::vector<std::thread *> _worker_objects;
    std::map<int,std::shared_ptr<acceptor>> _acceptors;
    int _acceptors_oid = 0;
    std::map<int,std::shared_ptr<connection>> _connections;
    int _connections_oid = 0;
    std::mutex _connections_m;

    std::shared_ptr<danet::protocol> _proto;

    std::queue<std::pair<int,packet>> _msgs;
    std::mutex _msgs_m;
  };
}

#endif	/* NETMAN_H */

