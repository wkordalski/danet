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

//  /*
//  // NETWORK CLASS /////////////////////////////////////////////////////////////
//
//  */
//  /*
//   * DEPRECED
//
//  */
//  /*
//  //  */
//  //
//  // Łączy z siecią
//  //
//  void netbase::connect(string ip, int port)
//  {
//    /*
//    add_worker(2);
//    // Połącz się z ip:port - TODO
//    this->bconnect(ip, port);
//
//    // Uruchom serwer - TODO
//    boost::asio::ip::tcp::resolver resolver(this->nsvc);
//    boost::asio::ip::tcp::resolver::query query(lip,to_string(lport));
//    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
//    this->nacc.open(endpoint.protocol());
//    this->nacc.set_option(boost::asio::ip::tcp::acceptor::reuse_address(false));
//    this->nacc.bind(endpoint);
//    this->nacc.listen(boost::asio::socket_base::max_connections);
//    shared_ptr<connection> nextcon(new connection(this));
//    this->nacc.async_accept(nextcon->sock, boost::bind(&network::accepted, this, _1, nextcon));
//    */
//    // Inicjalizacja reszty
//  }
//
//  void network::send(const message & msg, int id)
//  {
//    parm.lock();
//    if(id >= 0 && id < pairs.size() && pairs[id])
//    {
//      pairs[id]->send(msg.binary());
//    }
//    else if(id == -1)
//    {
//      shared_ptr<vector<unsigned char>> data = msg.binary();
//      for(const auto c : pairs)
//      {
//        if(c != nullptr)
//          c->send(data);
//      }
//    }
//    parm.unlock();
//  }
//
//  message network::receive()
//  {
//    message msg;
//    rcvm.lock();
//    if(rcvq.empty())
//    {
//      msg.t = message::type::none;
//    }
//    else
//    {
//      msg = rcvq.front();
//      rcvq.pop();
//    }
//    rcvm.unlock();
//    return msg;
//  }
//
//  void network::accepted(const boost::system::error_code & ec, std::shared_ptr<connection> con)
//  {
//    unsm.lock();
//    // Listen for next connection...
//    shared_ptr<connection> nextcon(new connection(this));
//    this->nacc.async_accept(nextcon->sock, boost::bind(&network::accepted, this, _1, nextcon));
//    if(ec)
//    {
//      // SOME ERROR :-(
//    }
//    else
//    {
//      con->id = unsorted.size() | 0x80000000;
//      unsorted.push_back(con);
//      con->recv();
//    }
//    unsm.unlock();
//  }
//
//  void network::connected(const boost::system::error_code & ec, std::shared_ptr<connection> con)
//  {
//    parm.lock();
//    unsm.lock();
//    if(ec)
//    {
//      // SOME ERROR :-(
//    }
//    else
//    {
//      if(con->id == -1)
//      {
//        con->id = unsorted.size() | 0x80000000;
//        unsorted.push_back(con);
//      }
//      else
//      {
//        if(pairs.size() <= con->id) pairs.resize(con->id+1);
//        pairs[con->id] = con;
//      }
//      if(state == 0)
//      {
//        con->send(shared_ptr<vector<unsigned char>>(new vector<unsigned char>({0x00,0x00,0x00,0x01, 0x00,0x00,0x00,0x02, 0x01, 0x00})));
//        state = 1;
//      }
//      else if(state == 1)
//      {
//        con->send(shared_ptr<vector<unsigned char>>(new vector<unsigned char>({0x00,0x00,0x00,0x01, 0x00,0x00,0x00,0x02, 0x01, 0x01})));
//      }
//      con->recv();
//    }
//    parm.unlock();
//    unsm.unlock();
//  }
//
//
//  void network::bconnect(string ip, int port)
//  {
//    shared_ptr<connection> con(new connection(this));
//
//    // Bind to host
//    boost::asio::ip::tcp::resolver resolver(this->nsvc);
//    boost::asio::ip::tcp::resolver::query query(ip,to_string(port));
//
//    // Tu chyba trzeba sprawdzić czy znaleziono endpoint...
//    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
//    con->sock.async_connect(endpoint, bind(&network::connected, this, _1, con));
//  }
//
//  void network::bconnect(string ip, int port, int hid)
//  {
//    shared_ptr<connection> con(new connection(this));
//    con->id = hid;
//
//    // Bind to host
//    boost::asio::ip::tcp::resolver resolver(this->nsvc);
//    boost::asio::ip::tcp::resolver::query query(ip,to_string(port));
//
//    // Tu chyba trzeba sprawdzić czy znaleziono endpoint...
//    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
//    con->sock.async_connect(endpoint, bind(&network::connected, this, _1, con));
//  }
//
//  // SOME NETWORK LOGIC CODE ///////////////////////////////////////////////////
//
//  void network::inform_ips(shared_ptr<connection> c)
//  {
//    vector<byte> response = { 0x01,0x80 };
//    add2byte_vector(response, 8);
//    parm.lock();
//    int pcnt = 0;
//    int hid = 0;
//    for(auto p : pairs)
//    {
//      hid++;
//      if(!p) continue;
//      int prt = p->sock.remote_endpoint().port();
//      string s = p->sock.remote_endpoint().address().to_string();
//      int pidx = response.size();
//      add2byte_vector(response, 12+s.size());
//      pidx = set_int32_in_vector(hid, response, pidx);
//      pidx = set_int32_in_vector(s.size(), response, pidx);
//      pidx = set_string_in_vector(s, response, pidx);
//      pidx = set_int32_in_vector(prt, response, pidx);
//      pcnt++;
//    }
//    set_int32_in_vector(pcnt, response, 2);
//    set_int32_in_vector(this->myid, response, 6);
//    parm.unlock();
//    c->send(message(message::type::network, move(response)).binary());
//  }
//
//  void network::suggest_id(shared_ptr<connection> c)
//  {
//    bool hoster = true;
//    parm.lock();
//    for(int i = 0; i < myid; i++)
//      if(pairs[i])
//      {
//        hoster = false;
//        break;
//      }
//    parm.unlock();
//    if(!hoster) return;               // Not a hoster
//    if(!(c->id & 0x80000000)) return; // Has an ID
//
//
//    vector<byte> response = { 0x01,0x81 };
//    add2byte_vector(response, 4);
//    parm.lock();
//    unsm.lock();
//    // FIXME myid - tablica rośnie, ale to dla braku dwuznaczności
//    for(int i = myid; i < pairs.size(); i++)
//    {
//      if(!pairs[i])
//      {
//        pairs[i] = c;
//        unsorted[c->id].reset();
//        c->id = i;
//        set_int32_in_vector(i, response, 2);
//        break;
//      }
//    }
//    unsm.unlock();
//    parm.unlock();
//    c->send(message(message::type::network, move(response)).binary());
//  }
//
//  void network::recv_ips(message m)
//  {
//    int addr_cnt, hid;
//    get_int32_from_vector(addr_cnt, m.d, 2);
//    get_int32_from_vector(hid, m.d, 6);
//    parm.lock();
//    unsm.lock();
//    if(pairs.size() <= hid) pairs.resize(hid+1);
//    pairs[hid] = unsorted[hid];
//    pairs[hid]->id = hid;
//    unsorted[hid].reset();
//    unsm.unlock();
//    parm.unlock();
//    int rdr = 10;
//    for(int i = 0; i < addr_cnt; i++)
//    {
//      rdr = get_int32_from_vector(hid, m.d, rdr);
//      int str_len;
//      rdr = get_int32_from_vector(str_len, m.d, rdr);
//      string adr = "";
//      rdr = get_string_from_vector(adr, m.d, rdr, str_len);
//      int prt;
//      rdr = get_int32_from_vector(prt, m.d, rdr);
//      this->bconnect(adr, prt, hid);
//    }
//  }
//
//  void network::received(message m)
//  {
//    if(m.t == message::type::network)
//    {
//      if(m.d[0] == 0x01)    // Connectivity
//      {
//        if(m.d[1] == 0x00)  // IP request
//        {
//          if(m.s & 0x80000000)
//          {
//            inform_ips(unsorted[m.s & 0x7FFFFFFF]);
//            suggest_id(unsorted[m.s & 0x7FFFFFFF]);
//          }
//          else
//          {
//            inform_ips(pairs[m.s]);
//          }
//        }
//        else if(m.d[1] == 0x01)   // ID request
//        {
//          if(m.s & 0x80000000)
//          {
//            suggest_id(unsorted[m.s & 0x7FFFFFFF]);
//          }
//        }
//        else if(m.d[1] == 0x02)   // ID set
//        {
//          int sugid;
//          get_int32_from_vector(sugid, m.d, 2);
//          parm.lock();
//          unsm.lock();
//          if(m.s & 0x80000000 && !pairs[sugid])
//          {
//            pairs[sugid] = unsorted[m.s];
//            unsorted[m.s].reset();
//            pairs[sugid]->id = sugid;
//          }
//          unsm.unlock();
//          parm.unlock();
//        }
//        else if(m.d[1] == 0x80) // Client IPs get
//        {
//          recv_ips(move(m));
//        }
//        else if(m.d[1] == 0x81) // Client ID get
//        {
//          int sugid;
//          get_int32_from_vector(sugid, m.d, 2);
//          vector<shared_ptr<connection>> cons;
//          vector<byte> mssg;
//          mssg.resize(14);
//          set_int32_in_vector(1, mssg, 0);
//          set_int32_in_vector(6, mssg, 4);
//          mssg[8] = 0x01;
//          mssg[9] = 0x02;
//          set_int32_in_vector(sugid, mssg, 10);
//          auto *mssgp = new vector<byte>();
//          (*mssgp) = move(mssg);
//          shared_ptr<vector<unsigned char>> mesag(mssgp);
//          parm.lock();
//          myid = sugid;
//          for(auto p : pairs)
//            if(p) cons.push_back(p);
//          parm.unlock();
//          for(auto p : cons)
//            p->send(mesag);
//        }
//      }
//    }
//    else if(m.t == message::type::none)
//    {
//      // Błędny komunikat lub coś podobnego
//    }
//    else
//    {
//      if(m.s & 0x80000000)
//      {
//        // Error - nie jesteś w systemie sieciowym - zarejstruj się! - TODO
//      }
//      else
//      {
//        rcvm.lock();
//        rcvq.push(move(m));
//        rcvm.unlock();
//      }
//    }
//  }
//
//
//  // MESSAGE CLASS /////////////////////////////////////////////////////////////
//
//  shared_ptr<vector<unsigned char>> message::binary() const
//  {
//    auto *vec = new vector<unsigned char>();
//    vec->reserve(this->d.size() + 8);
//
//    // Typ komunikatu
//    vec->push_back((((unsigned int)this->t)>>24)&0xFF);
//    vec->push_back((((unsigned int)this->t)>>16)&0xFF);
//    vec->push_back((((unsigned int)this->t)>> 8)&0xFF);
//    vec->push_back(( (unsigned int)this->t)     &0xFF);
//
//    // Rozmiar wektora...
//    vec->push_back(((this->d.size())>>24)&0xFF);
//    vec->push_back(((this->d.size())>>16)&0xFF);
//    vec->push_back(((this->d.size())>> 8)&0xFF);
//    vec->push_back(( this->d.size())     &0xFF);
//
//    for(auto kar : this->d)
//    {
//      vec->push_back(kar);
//    }
//
//    return shared_ptr<vector<unsigned char>>(vec);
//  }
//
//  // NETWORK CONNECTION CLASS //////////////////////////////////////////////////
//
//  network::connection::connection(network *n)
//    : sock(n->nsvc), strda(n->nsvc), strdb(n->nsvc)
//  {
//    this->n = n;
//  }
//
//  void network::connection::send(std::shared_ptr<std::vector<unsigned char>> data)
//  {
//    sndm.lock();
//    bool justsend = sndq.empty();
//    sndq.push(data);
//    if(justsend)
//    {
//      boost::asio::async_write(sock, boost::asio::buffer(*(sndq.front())),
//        strda.wrap(bind(&connection::on_send, this, _1)));
//    }
//    sndm.unlock();
//  }
//
//  void network::connection::on_send(const boost::system::error_code &ec)
//  {
//    if(ec)
//    {
//      this->close();
//      return;
//    }
//    sndm.lock();
//    sndq.pop();
//    if(!sndq.empty())
//    {
//      boost::asio::async_write(sock, boost::asio::buffer(*(sndq.front())),
//        strda.wrap(boost::bind(&connection::on_send, this, _1)));
//    }
//    sndm.unlock();
//  }
//
//  void network::connection::on_head(const boost::system::error_code &ec, const size_t &bt)
//  {
//    if(ec)
//    {
//      this->close();
//      return;
//    }
//    cr += bt;
//    if(cr < 8)
//    {
//      sock.async_read_some(boost::asio::buffer(head+cr, 8-cr), strdb.wrap(bind(&connection::on_head, this, _1, _2)));
//    }
//    else
//    {
//      // Get message type and message size
//      for(int i = 0; i < 4; i++)
//      {
//        ct <<= 8;
//        ct |= head[i];
//      }
//      for(int i = 4; i < 8; i++)
//      {
//        cs <<= 8;
//        cs |= head[i];
//      }
//      buff.resize(cs);
//      cr = 0;
//      sock.async_read_some(boost::asio::buffer(&buff[0], cs), strdb.wrap(bind(&connection::on_recv, this, _1, _2)));
//    }
//  }
//
//  void network::connection::on_recv(const boost::system::error_code &ec, const size_t &bt)
//  {
//    if(ec)
//    {
//      this->close();
//      return;
//    }
//    cr += bt;
//    if(cr < cs)
//    {
//      sock.async_read_some(boost::asio::buffer(head+cr, cs-cr), strdb.wrap(bind(&connection::on_recv, this, _1, _2)));
//    }
//    else
//    {
//      if(ct == (unsigned int)message::type::network && cs > 0 && buff[0] == 0x00)
//      {
//        // Wiadomość silnie systemowa - na poziomie połączenia - wykonać tutaj
//      }
//      // Przekaż dalej...
//      n->received(message((message::type)ct, this->id, move(buff)));
//      rcvm.unlock();
//      recv();
//    }
//  }
//
//  void network::connection::close()
//  {
//    if(id != -1)
//    {
//      // Usunięto gracza o id = this->id
//      n->received(message(message::type::rm_player, id, {(id>>24)&0xFF,(id>>16)&0xFF,(id>>8)&0xFF,id&0xFF}));
//    }
//    boost::system::error_code ec;
//    sock.shutdown( boost::asio::ip::tcp::socket::shutdown_both, ec );
//    sock.close( ec );
//  }
}