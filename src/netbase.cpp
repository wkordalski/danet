#include "netbase.h"

#include "acceptor.h"
#include "connection.h"

using namespace std;

namespace bnet = boost::asio;
namespace bsys = boost::system;

namespace danet
{
  netbase::netbase(shared_ptr<protocol> pro) : work(service)
  {
    // Dodaj wątek
    workers.push_back(new thread(bind(&netbase::io_worker, this)));
    workers.push_back(new thread(bind(&netbase::io_worker, this)));
    workers.push_back(new thread(bind(&netbase::io_worker, this)));
    workers.push_back(new thread(bind(&netbase::io_worker, this)));
    pro->nb = this;
    this->proto = pro;
    // TODO
  }

  netbase::~netbase()
  {
    this->service.stop();
    for(thread *t: this->workers)
    {
      t->join();
    }
  }

  void netbase::io_worker()
  {
    while( true )
    {
      try
      {
        boost::system::error_code ec;
        this->service.run( ec );
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

  netbase::handle netbase::listen_at(address* adr)
  {
    shared_ptr<acceptor> acc = adr->acceptor();
    if(!acc->run(this))
    {
      return 0;
    }

    acceptors_oid++;
    if(acceptors_oid > 1000000000) acceptors_oid = 1;
    while(acceptors.find(acceptors_oid) != acceptors.end()) acceptors_oid++;
    acceptors[acceptors_oid] = acc;

    return (netbase::handle)(0xFFFFFFFF - acceptors_oid);
  }

  netbase::handle netbase::connect_to(address* adr)
  {
    shared_ptr<connection> con = adr->connection();

    netbase::handle h = this->add_connection(con);

    if(!con->run(this, h))
    {
      return 0;
    }


    //this->connections.push_back(con);
    //this->proto->add_connection((netbase::handle)connections_oid); <- ŹLE
    return (netbase::handle)(h);
  }

  void netbase::close_resource(netbase::handle h)
  {
    // TODO: A co jeśli uchwyt jest niepoprawny?
    if(h < 0x7FFFFFFF)
    {
      // Usuń połączenie...
      int cid = h;
      connections.erase(cid);
    }
    else
    {
      // Usuń akceptor...
      int aid = (0xFFFFFFFF - h);
      acceptors.erase(aid);
    }
  }

  void netbase::send_to_resource(std::shared_ptr<packet> v, netbase::handle h)
  {
    // TODO: A co jeśli uchwyt jest niepoprawny?
    if(h < 0x7FFFFFFF)
    {
      connections[h]->send_data(v);
    }
    // else invalid handle...
  }

  void netbase::recv_message(packet& v, user& s)
  {
    msgs_m.lock();
    if(!msgs.empty())
    {
      v = move(msgs.front().second);
      s = msgs.front().first;
      msgs.pop();
    }
    msgs_m.unlock();
  }

  void netbase::message_received(packet& p, user s)
  {
    msgs_m.lock();
    msgs.push(pair<int,packet>(s, move(p)));
    msgs_m.unlock();
  }

  void netbase::send_message(std::vector<byte> v, const std::vector<user>& s)
  {
    this->proto->send_data(v,s);
  }

  netbase::handle netbase::add_connection(std::shared_ptr<connection> con)
  {
    connections_m.lock();
    connections_oid++;
    if(connections_oid > 1000000000) connections_oid = 1;
    while(connections.find(connections_oid) != connections.end()) connections_oid++;
    connections[connections_oid] = con;
    netbase::handle h = connections_oid;
    connections_m.unlock();
    return h;
  }

  void netbase::rem_connection(handle h)
  {
    // TODO: A co jeśli uchwyt jest niepoprawny?
    connections_m.lock();
    connections.erase(h);
    connections_m.unlock();
  }

  shared_ptr<danet::address> netbase::get_address(handle h)
  {
    // TODO: A co jeśli uchwyt jest niepoprawny?
    if(h < 0x7FFFFFFF)
    {
      // Usuń połączenie...
      int cid = h;
      return connections[cid]->get_address();
    }
    else
    {
      // Usuń akceptor...
      int aid = (0xFFFFFFFF - h);
      return acceptors[aid]->get_address();
    }
  }
}