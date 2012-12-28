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

#include "netbase.h"

#include "acceptor.h"
#include "connection.h"

using namespace std;

namespace bnet = boost::asio;
namespace bsys = boost::system;

namespace danet
{
  netbase::netbase(std::shared_ptr<protocol> pro) : _work_object(_service)
  {
    // Dodaj wątek
    _worker_objects.push_back(new thread(bind(&netbase::_io_worker, this)));
    _worker_objects.push_back(new thread(bind(&netbase::_io_worker, this)));
    _worker_objects.push_back(new thread(bind(&netbase::_io_worker, this)));
    _worker_objects.push_back(new thread(bind(&netbase::_io_worker, this)));
    pro->nb = this;
    this->_proto = pro;
    // TODO
  }

  netbase::~netbase()
  {
    this->_service.stop();
    for(thread *t: this->_worker_objects)
    {
      t->join();
    }
  }

  void netbase::_io_worker()
  {
    while( true )
    {
      try
      {
        boost::system::error_code ec;
        this->_service.run( ec );
        if( ec )
        {
          // Internal error (e.g. io_service stopped) - exit!
        }
        break;
      }
      catch( std::exception & ex )
      {
        // Other exceptions - handle it! - and work then.
      }
    }
  }

  netbase::handle netbase::_listen(address* adr)
  {
    shared_ptr<acceptor> acc = adr->acceptor();
    if(!acc->run(this))
    {
      return 0;
    }

    _acceptors_oid++;
    if(_acceptors_oid > 1000000000) _acceptors_oid = 1;
    while(_acceptors.find(_acceptors_oid) != _acceptors.end()) _acceptors_oid++;
    _acceptors[_acceptors_oid] = acc;

    return (netbase::handle)(0xFFFFFFFF - _acceptors_oid);
  }

  netbase::handle netbase::_connect(address* adr)
  {
    shared_ptr<connection> con = adr->connection();

    netbase::handle h = this->_connection_add(con);

    if(!con->run(this, h))
    {
      return 0;
    }


    //this->connections.push_back(con);
    //this->proto->add_connection((netbase::handle)connections_oid); <- ŹLE
    return (netbase::handle)(h);
  }

  void netbase::_close(netbase::handle h)
  {
    // TODO: A co jeśli uchwyt jest niepoprawny?
    if(h < 0x7FFFFFFF)
    {
      // Usuń połączenie...
      int cid = h;
      _connections.erase(cid);
    }
    else
    {
      // Usuń akceptor...
      int aid = (0xFFFFFFFF - h);
      _acceptors.erase(aid);
    }
  }

  void netbase::_do_send(std::shared_ptr<packet> v, netbase::handle h)
  {
    // TODO: A co jeśli uchwyt jest niepoprawny?
    if(h < 0x7FFFFFFF)
    {
      _connections[h]->do_send(v);
    }
    // else invalid handle...
  }

  void netbase::_recv(packet& v, user& s)
  {
    _msgs_m.lock();
    if(!_msgs.empty())
    {
      v = move(_msgs.front().second);
      s = _msgs.front().first;
      _msgs.pop();
    }
    _msgs_m.unlock();
  }

  void netbase::_on_receive(packet p, user s)
  {
    _msgs_m.lock();
    _msgs.push(pair<int,packet>(s, move(p)));
    _msgs_m.unlock();
  }

  void netbase::_send(std::vector<byte> v, const std::vector<user>& s)
  {
    this->_proto->do_send(v,s);
  }

  netbase::handle netbase::_connection_add(std::shared_ptr<connection> con)
  {
    _connections_m.lock();
    _connections_oid++;
    if(_connections_oid > 1000000000) _connections_oid = 1;
    while(_connections.find(_connections_oid) != _connections.end()) _connections_oid++;
    _connections[_connections_oid] = con;
    netbase::handle h = _connections_oid;
    _connections_m.unlock();
    return h;
  }

  void netbase::_connection_rem(handle h)
  {
    // TODO: A co jeśli uchwyt jest niepoprawny?
    _connections_m.lock();
    _connections.erase(h);
    _connections_m.unlock();
  }

  shared_ptr<danet::address> netbase::_get_address(handle h)
  {
    // TODO: A co jeśli uchwyt jest niepoprawny?
    if(h < 0x7FFFFFFF)
    {
      // Usuń połączenie...
      int cid = h;
      return _connections[cid]->get_address();
    }
    else
    {
      // Usuń akceptor...
      int aid = (0xFFFFFFFF - h);
      return _acceptors[aid]->get_address();
    }
  }

  set<netbase::user> netbase::_get_users_list()
  {
    return this->_proto->get_users();
  }

  netbase::user netbase::_get_id()
  {
    return this->_proto->get_id();
  }
}