#include "netbase.h"
#include "connections/tcp_ip_connection.h"

#include <string>

using namespace std;
namespace bsys = boost::system;
namespace bnet = boost::asio;

namespace danet
{
  namespace ip
  {
    namespace tcp
    {
      connection::connection(const danet::ip::tcp::address &adr) : adr(adr)
      {
      }

      connection::connection(netbase* nb) : adr()
      {
        this->nb = nb;
        sck = new boost::asio::ip::tcp::socket(this->get_ioservice());
      }

      connection::~connection()
      {
        delete sck;
      }

      bool connection::run(netbase *nb, netbase::handle id)
      {
        this->nb = nb;
        this->id = id;

        bsys::error_code ec;

        bnet::ip::tcp::resolver resolver(this->get_ioservice());
        bnet::ip::tcp::resolver::query query(adr.ip, to_string(adr.port));
        bnet::ip::tcp::resolver::iterator endpoint = resolver.resolve(query, ec);
        if(ec) return false;

        sck = new boost::asio::ip::tcp::socket(this->get_ioservice());
        //strd_r = new boost::asio::strand(this->get_ioservice());
        //strd_w = new boost::asio::strand(this->get_ioservice());

        // TODO: I o wiele więcej...
        sck->async_connect(*endpoint, bind(&connection::on_connect, this, placeholders::_1));
        return true;
      }

      bool connection::run(netbase::handle id)
      {
        this->id = id;
        return true;
      }

      void connection::on_connect(const boost::system::error_code& ec)
      {
        if(ec)
        {
          this->netbase_rem_connection();
          return;
        }
        this->proto_add_connection();
        this->listen();
      }

      void connection::listen()
      {
        this->recv();
        this->send();
      }

      void connection::recv()
      {
        rcv_m.lock();
        bnet::async_read(*(this->sck),bnet::buffer(rcv_b, 4),bind(&connection::on_header, this, placeholders::_1, placeholders::_2));
        rcv_m.unlock();
      }

      void connection::send_data(shared_ptr<packet> data)
      {
        snd_m.lock();
        bool emptst = snd_q.empty();
        this->snd_q.push(data);
        snd_m.unlock();
        if(emptst) this->send();
      }

      void connection::send()
      {
        snd_m.lock();
        if(!snd_q.empty())
        {
          unsigned int msize = snd_q.front()->size();
          this->snd_b[3] = ((msize      ) & 0xFF);
          this->snd_b[2] = ((msize >>  8) & 0xFF);
          this->snd_b[1] = ((msize >> 16) & 0xFF);
          this->snd_b[0] = ((msize >> 24) & 0xFF);
          bnet::async_write(*(this->sck),bnet::buffer(snd_b, 4), bind(&connection::on_hsend, this, placeholders::_1, placeholders::_2));
        }
        snd_m.unlock();
      }

      void connection::on_hsend(const boost::system::error_code& ec, const size_t &bt)
      {
        snd_m.lock();
        if(ec)
        {
          // TODO: Błąd przy wysyłaniu nagłówka... (w miejscu bt)
        }
        snd_d = *(snd_q.front());
        bnet::async_write(*(this->sck),bnet::buffer(snd_d), bind(&connection::on_send, this, placeholders::_1, placeholders::_2));
        snd_m.unlock();
      }

      void connection::on_send(const boost::system::error_code& ec, const size_t &bt)
      {
        snd_m.lock();
        if(ec)
        {
          // TODO: Błąd przy wysyłaniu danych... (w miejscu bt)
        }
        snd_q.pop();
        snd_m.unlock();
        this->send();
      }

      void connection::on_header(const boost::system::error_code& ec, const size_t& bt)
      {
        rcv_m.lock();
        if(ec)
        {
          if(ec)  // FIXME
          {
            // Zamknięto połączenie
            this->proto_rem_connection();
            this->netbase_rem_connection();
            return;
          }
          // TODO: Błąd przy odbieraniu nagłówka danych
        }
        unsigned int msgsiz = 0;
        for(byte b: rcv_b)
        {
          msgsiz <<= 8;
          msgsiz |= (unsigned int)b;
        }
        if(msgsiz == 0)
        {
          rcv_m.unlock();
          this->recv();
        }
        else
        {
          rcv_d.resize(msgsiz);
          bnet::async_read(*(this->sck),bnet::buffer(rcv_d),bind(&connection::on_body, this, placeholders::_1, placeholders::_2));
          rcv_m.unlock();
        }
      }

      void connection::on_body(const boost::system::error_code& ec, const size_t& bt)
      {
        rcv_m.lock();
        if(ec)
        {
          if(ec)  // FIXME
          {
            // Zamknięto połączenie
            this->proto_rem_connection();
            this->netbase_rem_connection();
            return;
          }
          // TODO: Błąd przy odbieraniu danych
        }
        this->proto_data_recieved(rcv_d);
        rcv_m.unlock();
        this->recv();
      }

      shared_ptr<danet::address> connection::get_address()
      {
        return shared_ptr<danet::address>(new danet::ip::tcp::address(this->adr));
      }
    }
  }
}